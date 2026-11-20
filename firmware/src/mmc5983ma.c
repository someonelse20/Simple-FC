#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/stdlib.h>

#include "mmc5983ma_regs.h"
#include "mmc5983ma.h"

#define SPI_BUS spi0
#define SPI_DELAY 10

static float mag[3];

static void write_reg(uint8_t reg, uint8_t buf);
static void write_mask(uint8_t reg, uint8_t value, uint8_t mask);
static void read_regs(uint8_t reg, uint8_t *buf, uint16_t len);
static void set_conf();

void MMC5983MA_init() {

}

bool MMC5983MA_check_id() {
	uint8_t id = 0x00;

	read_regs(WHO_AM_I, &id, 1);

	if (id == 0x30) {
		return 1;
	} else {
		return 0;
	}
}

void MMC5983MA_reset() {
	// uint8_t buf = 0x05;
	uint8_t buf = 0b10000000; // Triggers SW reset.

	// Wait for device to boot
	sleep_ms(15); // Datasheet specifies 10ms to boot, using 15ms for good measure

	write_reg(CTRL1, buf);
}

void MMC5983MA_set_ODR(uint8_t ODR) {
	write_mask(CTRL1, MMC_ODR_800, 0b11111100);
}

void MMC5983MA_set_scale(uint8_t scale) {

}

void MMC5983MA_int_handler(uint gpio, uint32_t events) {

}

void MMC5983MA_poll() {

}

static void write_reg(uint8_t reg, uint8_t buf) {
	reg |= WRITE_BIT;

	gpio_put(MMC_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_write_blocking(SPI_BUS, &buf, 1);
	gpio_put(MMC_PIN_CS, 0);
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

	gpio_put(MMC_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_read_blocking(SPI_BUS, 0, buf, len);
	gpio_put(MMC_PIN_CS, 0);
}

static void set_conf() {
	// Enable IRQ and auto set/reset
	write_reg(CTRL0, 0b00100100);
}

