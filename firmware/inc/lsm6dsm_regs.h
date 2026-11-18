#ifndef LSM6DSM_REGS_H
#define LSM6DSM_REGS_H

#define PIN_INT1 18
#define PIN_INT2 23
#define PIN_CS 21
#define SPI_BUS spi0
#define SPI_DELAY 10

#define READ_BIT              1
#define WRITE_BIT             0

#define FIFO_CTRL1            0x06
#define FIFO_CTRL2            0x07
#define FIFO_CTRL3            0x08
#define FIFO_CTRL4            0x09
#define FIFO_CTRL5            0x0A
#define DRDY_PULSE_CFG        0x0B
#define INT1_CTRL             0x0D
#define INT2_CTRL             0x0E
#define WHO_AM_I              0x0F
#define CTRL1_XL              0x10
#define CTRL2_G               0x11
#define CTRL3_C               0x12
#define CTRL4_C               0x13
#define CTRL5_C               0x14
#define CTRL6_C               0x15
#define CTRL7_G               0x16
#define CTRL8_XL              0x17
#define CTRL9_XL              0x18
#define CTRL10_C              0x19

#define FIFO_DATA_OUT_L       0x3E
#define FIFO_DATA_OUT_H       0x3F

#endif
