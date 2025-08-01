/*
 * SPI.h
 *
 * Created: 25/07/2025 07:56:28
 *  Author: valen
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

typedef enum{
	SPI_MASTER_OSC_DIV2 = 0b01010000,
	SPI_MASTER_OSC_DIV4 = 0b01010001,
	SPI_MASTER_OSC_DIV8 = 0b01010010,
	SPI_MASTER_OSC_DIV16 = 0b01010011,
	SPI_MASTER_OSC_DIV32 = 0b01010100,
	SPI_MASTER_OSC_DIV64 = 0b01010101,
	SPI_MASTER_OSC_DIV128 = 0b01010110,
	SPI_SLAVE_SS = 0b01000000
} type;

typedef enum {
	SPI_DATA_ORDER_MSB = 0b00000000,
	SPI_DATA_ORDER_LSB = 0b00100000
} data_order;

typedef enum{
	SPI_CLOCK_IDLE_HIGH = 0b00001000,
	SPI_CLOCK_IDLE_LOW = 0b00000000
} clock_polar;

typedef enum {
	SPI_CLOCK_FIRST_EDGE = 0b00000000,
	SPI_CLOCK_LAST_EDGE = 0b00000100
} clock_phase;

void SPI_init(type, data_order, clock_polar, clock_phase);

void SPI_Write(uint8_t data);

char SPI_Read();




#endif /* SPI_H_ */



