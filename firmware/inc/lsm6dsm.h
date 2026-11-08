#ifndef LSM6DSM_H
#define LSM6DSM_H

#include <pico/stdlib.h>
#include <hardware/spi.h>

#define PIN_INT1 18
#define PIN_INT2 23
#define PIN_CS 21
#define SPI_BUS spi0
#define SPI_DELAY 10

#define READ_BIT              1
#define WRITE_BIT             0

#define WHO_AM_I              0x0F

struct LSM6DSM{
	float gyro[3];
	float accel[3];
	uint16_t temp;
};

void LSM6DSM_init(struct LSM6DSM *lsm6dsm);
bool LSM6DSM_check_id();

#endif
