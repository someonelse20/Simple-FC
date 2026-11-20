#include <hardware/gpio.h>
#include <stdint.h>

#include "lsm6dsm.h"
#include "spi.h"

void spi_write_reg(uint16_t cs_pin, uint8_t reg, uint8_t buf) {
	reg |= WRITE_BIT;

	gpio_put(LSM_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_write_blocking(SPI_BUS, &buf, 1);
	gpio_put(LSM_PIN_CS, 0);
}

void spi_write_mask(uint16_t cs_pin, uint8_t reg, uint8_t value, uint8_t mask) {
	uint8_t buf = 0x00;

	spi_read_regs(cs_pin, reg, &buf, 1);

	buf &= ~mask;
	buf |= value;

	spi_write_reg(cs_pin, reg, buf);
}

void spi_read_regs(uint16_t cs_pin, uint8_t reg, uint8_t *buf, uint16_t len) {
	reg |= READ_BIT;

	gpio_put(LSM_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_read_blocking(SPI_BUS, 0, buf, len);
	gpio_put(LSM_PIN_CS, 0);
}

