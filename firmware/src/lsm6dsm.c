#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>
#include <stdint.h>

#include "lsm6dsm.h"
#include "lsm6dsm_regs.h"

#define SPI_BUS spi0
#define SPI_DELAY 10

static float gyro[3];
static float accel[3];

static void write_reg(uint8_t reg, uint8_t buf);
static void write_mask(uint8_t reg, uint8_t value, uint8_t mask);
static void read_regs(uint8_t reg, uint8_t *buf, uint16_t len);
static void read_fifo(uint16_t *gyro, uint16_t *accel);
static void set_conf();

bool LSM6DSM_check_id() {
	uint8_t id = 0x00;

	read_regs(WHO_AM_I, &id, 1);

	if (id == 0x6A) {
		return 1;
	} else {
		return 0;
	}
}

void LSM6DSM_reset() {
	// uint8_t buf = 0x05;
	uint8_t buf = 0b00000101; // Triggers SW reset.

	// Wait for device to boot
	sleep_ms(40); // Datasheet specifies 35ms to boot, using 40ms for good measure

	write_reg(CTRL3_C, buf);

	LSM6DSM_clear_mem();
}

void LSM6DSM_clear_mem() {
	// uint8_t buf = 0x84;
	uint8_t buf = 0b10000100; // Clears memory.

	write_reg(CTRL3_C, buf);
}

// Bits are numbered 7 6 5 4 3 2 1 0
void LSM6DSM_set_XL_ODR(uint8_t ODR) {
	write_mask(CTRL1_XL, ODR, 0b00001111); // Clear bits 4-7
}

void LSM6DSM_set_XL_scale(uint8_t scale) {
	write_mask(CTRL1_XL, scale, 0b11110011); // Clear bits 2-3
}

void LSM6DSM_set_G_ODR(uint8_t ODR) {
	write_mask(CTRL2_G, ODR, 0b00001111); // Clear bits 4-7
}

void LSM6DSM_set_G_scale(uint8_t scale) {
	write_mask(CTRL2_G, scale, 0b11110011); // Clear bits 2-3
}

void LSM6DSM_init() {
	gpio_init(LSM_PIN_CS);
	gpio_init(LSM_PIN_INT1);
	gpio_init(LSM_PIN_INT2);

	gpio_set_irq_enabled_with_callback(LSM_PIN_INT1, GPIO_IRQ_EDGE_RISE, true, LSM6DSM_int_handler);

	if (!LSM6DSM_check_id()) {
		return;
	}

	LSM6DSM_reset();

	set_conf();
}

void LSM6DSM_int_handler(uint gpio, uint32_t events) {
	if (events != GPIO_IRQ_EDGE_RISE)
		return;

	if (gpio == LSM_PIN_INT1) {
		LSM6DSM_poll();
	}
}

void LSM6DSM_poll() {
	uint16_t gyro_raw[3];
	uint16_t accel_raw[3];

	read_fifo(gyro_raw, accel_raw);

	for (int i = 0; i < 3; i++) {
		gyro[i]  = gyro_raw[i]  * LSM_G_SO_1000;
		accel[i] = accel_raw[i] * LSM_XL_SO_8;
	}
}

float *LSM6DSM_get_gyro() {
	return gyro;
}

float *LSM6DSM_get_accel() {
	return accel;
}

static void write_reg(uint8_t reg, uint8_t buf) {
	reg |= WRITE_BIT;

	gpio_put(LSM_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_write_blocking(SPI_BUS, &buf, 1);
	gpio_put(LSM_PIN_CS, 0);
}

static void write_mask(uint8_t reg, uint8_t value, uint8_t mask) {
	uint8_t buf = 0x00;

	read_regs(reg, &buf, 1);

	buf &= ~mask;
	buf |= value;

	write_reg(reg, buf);
}

static void read_regs(uint8_t reg, uint8_t *buf, uint16_t len) {
	reg |= READ_BIT;

	gpio_put(LSM_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_read_blocking(SPI_BUS, 0, buf, len);
	gpio_put(LSM_PIN_CS, 0);
}

static void read_fifo(uint16_t *gyro, uint16_t *accel) {
	uint8_t buf[2];

	for (int i = 0; i < 3; i++) {
		read_regs(FIFO_DATA_OUT_L, buf, 2);

		gyro[i] = (buf[1] >> 8) | buf[0];
	}

	for (int i = 2; i < 6; i++) {
		read_regs(FIFO_DATA_OUT_L, buf, 2);

		accel[i] = (buf[1] >> 8) | buf[0];
	}
}

static void set_conf() {
	LSM6DSM_set_G_scale(LSM_G_FS_1000);
	LSM6DSM_set_G_ODR(LSM_ODR_6660);

	LSM6DSM_set_XL_scale(LSM_XL_FS_8);
	LSM6DSM_set_XL_ODR(LSM_ODR_6660);

	// Set FIFO threshold
	const uint8_t FIFO_threshold = 12; // 6 samples with 2 bytes per samples
	write_reg(FIFO_CTRL1, FIFO_threshold);

	// Set FIFO decimation to none
	write_mask(FIFO_CTRL3, LSM_FIFO_DEC_0, 0b11000111);
	write_mask(FIFO_CTRL3, LSM_FIFO_DEC_0, 0b11111000);

	// Set FIFO to stop on threshold
	write_reg(FIFO_CTRL4, 0b1000000);

	// Set FIFO ODR
	write_mask(FIFO_CTRL5, LSM_ODR_6660, 0b10000111);

	// Set FIFO mode to continuous
	write_mask(FIFO_CTRL5, LSM_FIFO_MODE_CONT, 0b11111000);

	// Set INT1 to triger on FIFO threshold
	write_reg(INT1_CTRL, 0b00001000);

	// Set INT2 to trigger on temp data ready
	write_reg(INT2_CTRL, 0b00000100);

	// Disable I2C
	write_reg(CTRL4_C, 0b00000100);
}

