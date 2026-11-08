#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <pico/types.h>

#include "lsm6dsm.h"

static void write_register(uint8_t reg, uint8_t buf) {
	reg |= WRITE_BIT;

	gpio_put(PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_write_blocking(SPI_BUS, &buf, 1);
	gpio_put(PIN_CS, 0);
}

static void read_registers(uint8_t reg, uint8_t *buf, uint16_t len) {
	reg |= READ_BIT;

	gpio_put(PIN_CS, 0);
	spi_write_blocking(SPI_BUS, &reg, 1);
	sleep_ms(SPI_DELAY);
	spi_read_blocking(SPI_BUS, 0, buf, len);
	gpio_put(PIN_CS, 0);
}

bool LSM6DSM_check_id() {
	uint8_t id = 0x00;

	read_registers(WHO_AM_I, &id, 1);

	if (id == 0x6A) {
		return 1;
	} else {
		return 0;
	}
}
