#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <pico/types.h>

#include "lsm6dsm.h"

#define PIN_INT1 18
#define PIN_INT2 23
#define PIN_CS 21
#define SPI_BUS spi0
#define SPI_DELAY 10

#define READ_BIT              1
#define WRITE_BIT             0

#define WHO_AM_I              0x0F
#define CTRL3_C               0x12

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

void LSM6DSM_reset(struct LSM6DSM_h *lsm6dsm) {
	// uint8_t buf = 0x05;
	uint8_t buf = 0b00000101; // Triggers SW reset.

	write_register(CTRL3_C, buf);

	/* also clear control regs in handler */
}

void LSM6DSM_clear_mem(struct LSM6DSM_h *lsm6dsm) {
	// uint8_t buf = 0x84;
	uint8_t buf = 0b10000100; // Clears memory.

	write_register(CTRL3_C, buf);

	/* also clear control regs in handler */
}

