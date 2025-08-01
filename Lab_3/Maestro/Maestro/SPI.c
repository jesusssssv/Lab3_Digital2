/*
 * CFile1.c
 *
 * Created: 25/07/2025 07:55:45
 *  Author: valen
 */ 

#include <avr/io.h>
#include <stdint.h>

#include "SPI.h"


void SPI_init(type sType, data_order sDataOrder, clock_polar sClockPolar, clock_phase sClockPhase){
	if (sType&0b00010000){
		DDRB |= (1<<3)|(1<<5)|(1<<2); // MOSI, SCK, SS
		DDRB &= ~(1<<4); // MISO
		SPCR0 |= (1<<MSTR); // MASTER
		
		uint8_t val = sType & 0b00000111;
		switch(val){
			case 0:
			SPCR0 &= ~((1<<SPR1)|(1<<SPR0));
			SPCR0 |= (1<<SPI2X);
			break;
			case 1:
			SPCR0 &= ~((1<<SPR1)|(SPR0));
			SPSR0 &= ~(1<<SPI2X);
			break;
			case 2:
			SPCR0 |= (1<<SPR0);
			SPCR0 &= ~(1<<SPR1);
			SPSR0 |= (1<<SPI2X);
			break;
			case 3:
			SPCR0 |= (1<<SPR0);
			SPCR0 &= ~(1<<SPR1);
			SPCR0 &= ~(1<<SPI2X);
			break;
			case 4:
			SPCR0 &= ~(1<<SPR0);
			SPCR0 |= (1<<SPR1);
			SPCR0 |= (1<<SPI2X);
			break;
			case 5:
			SPCR0 &= ~(1<<SPR0);
			SPCR0 |= (1<<SPR1);
			SPSR0 &= ~ (1<<SPI2X);
			
			break;
			case 6:
			SPCR0 |= (1<< SPR0)|(1<<SPR1);
			SPSR0 &= ~(1<<SPI2X);
			break;
		}
		} else {
		DDRB |= (1<<4);
		DDRB &= ~((1<<3)|(1<<5)|(1<<2));
		SPCR0 &= ~(1<<MSTR);
	}
	SPCR0 |= (1<<SPE)|sDataOrder|sClockPolar|sClockPhase;
}

void SPI_Write(uint8_t data){
	SPDR0 = data;
}

char SPI_Read(){
	while(!(SPSR0&(1<<SPIF)));
	return(SPDR0);
}