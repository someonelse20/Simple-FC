#ifndef LSM6DSM_REGS_H
#define LSM6DSM_REGS_H

#define PIN_INT1 18
#define PIN_INT2 23
#define PIN_CS 21
#define SPI_BUS spi0
#define SPI_DELAY 10

#define READ_BIT              1
#define WRITE_BIT             0

#define WHO_AM_I              0x0F
#define CTRL1_XL              0x10
#define CTRL2_G               0x11
#define CTRL3_C               0x12

#endif
