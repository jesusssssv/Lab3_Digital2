#define F_CPU 16000000UL  // Define la frecuencia del reloj del sistema (16 MHz) para las funciones de retardo

#include <avr/io.h>         // Librería principal de E/S del AVR
#include <util/delay.h>     // Librería para funciones de retardo como _delay_ms()
#include "SPI.h"        // Archivo de cabecera personalizado para manejo de SPI
#include "UART.h"// Archivo de cabecera personalizado para manejo de UART

uint8_t valor_spi = 0;  // Variable para almacenar el valor recibido por SPI
float volt1 = 0;        // Variable para almacenar el voltaje del canal 1
float volt2 = 0;        // Variable para almacenar el voltaje del canal 2

int main(void)
{
	// Configura el pin PB2 (Chip Select del SPI) como salida
	DDRB |= (1<<2);

	// Inicializa PB2 en nivel alto (CS inactivo)
	PORTB |= (1<<2);

	// Inicializa el SPI como maestro:
	// Frecuencia: Fosc/16, orden de bits MSB primero,
	// reloj inactivo en alto, primer flanco de subida válido
	SPI_init(SPI_MASTER_OSC_DIV16, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_HIGH, SPI_CLOCK_FIRST_EDGE);

	// Inicializa la UART con baud rate de 9600 (valor UBRR = 103 para 16 MHz)
	UART_Init();

	while (1)
	{
		// --- LABORATORIO ---

		// Comunicación SPI para leer el valor del primer potenciómetro
		PORTB &= ~(1<<2);         // Activa el esclavo (CS a 0)
		SPI_Write('a');           // Envía comando 'a' para solicitar voltaje 1
		valor_spi = SPI_Read();   // Lee la respuesta del esclavo
		volt1 = 5.00 * valor_spi / 255;  // Convierte el valor de 8 bits a voltaje (0-5V)
		PORTB |= (1<<2);          // Desactiva el esclavo (CS a 1)

		// Muestra el voltaje 1 en el monitor serial
		UART_Char('\n');
		UART_Char('\n');
		UART_Write("Volt 1: ");
		UART_Var(volt1);          // Envía el valor flotante como texto
		UART_Write(" V");

		// Comunicación SPI para leer el valor del segundo potenciómetro
		PORTB &= ~(1<<2);         // Activa el esclavo (CS a 0)
		SPI_Write('b');           // Envía comando 'b' para solicitar voltaje 2
		valor_spi = SPI_Read();   // Lee la respuesta del esclavo
		volt2 = 5.00 * valor_spi / 255;  // Convierte el valor a voltaje
		PORTB |= (1<<2);          // Desactiva el esclavo (CS a 1)

		// Muestra el voltaje 2 en el monitor serial
		UART_Char('\n');
		UART_Write("Volt 2: ");
		UART_Var(volt2);
		UART_Write(" V");

		_delay_ms(500); // Pequeño retardo para evitar saturar la UART
	}
}
