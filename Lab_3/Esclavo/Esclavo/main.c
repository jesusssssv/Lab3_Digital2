/*
 * lab3S-Digital2.c
 *
 * Created: 2/11/2025 3:46:28 PM
 * Author : valen
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ADC.h"
#include "SPI.h"

uint16_t val1 = 0;
uint16_t val2 = 0;

unsigned char pines[8] = {2, 3, 4, 5, 6, 7, 8, 9};

int main(void)
{
	DDRD |= (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7);
	DDRB |= (1<<0)|(1<<1);
	PORTD = 0;
	PORTB = 0;
	
	
	SPI_init(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_HIGH, SPI_CLOCK_FIRST_EDGE);
	SPCR0 |= (1<<SPIE);
	ADC_init();
	sei();
	
    while (1) 
    {
		val1 = ADC_read(6);
		val2 = ADC_read(7);
		_delay_ms(500);

    }
}


ISR(SPI0_STC_vect){
	uint8_t spiValor = SPDR0;
	if (spiValor == 'a')
	{
		SPI_Write(val1/4);
	}
	else if (spiValor == 'b')
	{
		SPI_Write(val2/4);
	}

}

