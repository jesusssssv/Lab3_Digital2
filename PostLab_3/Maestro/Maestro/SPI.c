/*
 * CFile1.c
 *
 * Created: 25/07/2025 07:55:45
 *  Author: valen
 */ 

#include <avr/io.h>
#include <stdint.h>

#include "SPI.h"


// Función para inicializar la comunicación SPI
void SPI_init(type sType, data_order sDataOrder, clock_polar sClockPolar, clock_phase sClockPhase){
	
	// Verifica si el bit 4 está en alto para determinar si el modo es MASTER
	if (sType & 0b00010000){
		// Configura como salidas los pines MOSI (PB3), SCK (PB5) y SS (PB2)
		DDRB |= (1<<3)|(1<<5)|(1<<2);
		// Configura MISO (PB4) como entrada
		DDRB &= ~(1<<4);
		// Habilita el modo maestro
		SPCR0 |= (1<<MSTR);

		// Extrae los bits de velocidad (los 3 menos significativos)
		uint8_t val = sType & 0b00000111;

		// Selecciona configuración de velocidad SPI según 'val'
		switch(val){
			case 0: // fosc/2
			SPCR0 &= ~((1<<SPR1)|(1<<SPR0)); // SPR1 = 0, SPR0 = 0
			SPCR0 |= (1<<SPI2X);              // SPI2X = 1 (doble velocidad)
			break;

			case 1: // fosc/4
			SPCR0 &= ~((1<<SPR1)|(SPR0));     // SPR1 = 0, SPR0 = 0
			SPSR0 &= ~(1<<SPI2X);             // SPI2X = 0 (sin doble velocidad)
			break;

			case 2: // fosc/8
			SPCR0 |= (1<<SPR0);               // SPR0 = 1
			SPCR0 &= ~(1<<SPR1);              // SPR1 = 0
			SPSR0 |= (1<<SPI2X);              // SPI2X = 1
			break;

			case 3: // fosc/16
			SPCR0 |= (1<<SPR0);               // SPR0 = 1
			SPCR0 &= ~(1<<SPR1);              // SPR1 = 0
			SPCR0 &= ~(1<<SPI2X);             // SPI2X = 0
			break;

			case 4: // fosc/32
			SPCR0 &= ~(1<<SPR0);              // SPR0 = 0
			SPCR0 |= (1<<SPR1);               // SPR1 = 1
			SPCR0 |= (1<<SPI2X);              // SPI2X = 1
			break;

			case 5: // fosc/64
			SPCR0 &= ~(1<<SPR0);              // SPR0 = 0
			SPCR0 |= (1<<SPR1);               // SPR1 = 1
			SPSR0 &= ~(1<<SPI2X);             // SPI2X = 0
			break;

			case 6: // fosc/128
			SPCR0 |= (1<<SPR0)|(1<<SPR1);     // SPR0 = 1, SPR1 = 1
			SPSR0 &= ~(1<<SPI2X);             // SPI2X = 0
			break;
		}

		} else {
		// Configuración como esclavo

		// MISO como salida
		DDRB |= (1<<4);
		// MOSI, SCK y SS como entradas
		DDRB &= ~((1<<3)|(1<<5)|(1<<2));
		// Limpia el bit de maestro
		SPCR0 &= ~(1<<MSTR);
	}

	// Habilita SPI y configura el orden de bits, polaridad y fase del reloj
	SPCR0 |= (1<<SPE) | sDataOrder | sClockPolar | sClockPhase;
}

// Función para escribir un byte en SPI
void SPI_Write(uint8_t data){
	SPDR0 = data;  // Carga el dato en el registro de datos para iniciar la transmisión
}

// Función para leer un byte desde SPI
char SPI_Read(){
	// Espera hasta que se complete la transmisión (SPIF = 1)
	while (!(SPSR0 & (1<<SPIF)));
	// Devuelve el dato recibido
	return SPDR0;
}
