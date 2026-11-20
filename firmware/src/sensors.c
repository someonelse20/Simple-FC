#include <hardware/gpio.h>

#include "sensors.h"
#include "lsm6dsm.h"

void write_reg(uint8_t reg, uint8_t buf) {
	reg |= WRITE_BIT;

	gpio_put(LSM_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_write_blocking(SPI_BUS, &buf, 1);
	gpio_put(LSM_PIN_CS, 0);
}

void write_mask(uint8_t reg, uint8_t value, uint8_t mask) {
	uint8_t buf = 0x00;

	read_regs(reg, &buf, 1);

	buf &= ~mask;
	buf |= value;

	write_reg(reg, buf);
}

void read_regs(uint8_t reg, uint8_t *buf, uint16_t len) {
	reg |= READ_BIT;

	gpio_put(LSM_PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_read_blocking(SPI_BUS, 0, buf, len);
	gpio_put(LSM_PIN_CS, 0);
}

