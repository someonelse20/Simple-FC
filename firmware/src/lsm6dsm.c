#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <pico/types.h>
#include <stdint.h>

#include "lsm6dsm.h"
#include "lsm6dsm_regs.h"

static void write_reg(uint8_t reg, uint8_t buf) {
	reg |= WRITE_BIT;

	gpio_put(PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_write_blocking(SPI_BUS, &buf, 1);
	gpio_put(PIN_CS, 0);
}

static void read_regs(uint8_t reg, uint8_t *buf, uint16_t len) {
	reg |= READ_BIT;

	gpio_put(PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_read_blocking(SPI_BUS, 0, buf, len);
	gpio_put(PIN_CS, 0);
}

static void write_ctrl(uint8_t reg, uint8_t value, uint8_t mask) {
	uint8_t buf = 0x00;

	read_regs(reg, &buf, 1);

	buf &= ~mask;
	buf |= value;

	write_reg(reg, buf);
}

bool LSM6DSM_check_id() {
	uint8_t id = 0x00;

	read_regs(WHO_AM_I, &id, 1);

	if (id == 0x6A) {
		return 1;
	} else {
		return 0;
	}
}

void LSM6DSM_reset(struct LSM6DSM_h *lsm6dsm) {
	// uint8_t buf = 0x05;
	uint8_t buf = 0b00000101; // Triggers SW reset.

	write_reg(CTRL3_C, buf);

	/* also clear control regs in handler */
}

void LSM6DSM_clear_mem(struct LSM6DSM_h *lsm6dsm) {
	// uint8_t buf = 0x84;
	uint8_t buf = 0b10000100; // Clears memory.

	write_reg(CTRL3_C, buf);

	/* also clear control regs in handler */
}

// Bits are numbered 7 6 5 4 3 2 1 0
void LSM6DSM_set_XL_ODR(uint8_t ODR) {
	write_ctrl(CTRL1_XL, ODR, 0b00001111); // Clear bits 4-7
}

void LSM6DSM_set_XL_scale(uint8_t scale) {
	write_ctrl(CTRL1_XL, scale, 0b11110011); // Clear bits 2-3
}

void LSM6DSM_set_G_ODR(uint8_t ODR) {
	write_ctrl(CTRL2_G, ODR, 0b00001111); // Clear bits 4-7
}

void LSM6DSM_set_G_scale(uint8_t scale) {
	write_ctrl(CTRL2_G, scale, 0b11110011); // Clear bits 2-3
}

