#define F_CPU 16000000  // Define la frecuencia del sistema (16 MHz) para funciones de retardo

#include <avr/io.h>         // Librer�a base para operaciones con registros de E/S
#include <avr/interrupt.h>  // Permite uso de interrupciones externas e internas
#include <util/delay.h>     // Funciones de retardo (_delay_ms)
#include "ADC.h"            // Cabecera personalizada para el m�dulo ADC
#include "SPI.h"            // Cabecera personalizada para el m�dulo SPI

// Variables globales
uint16_t val1 = 0;         // Valor le�do del potenci�metro en A6
uint16_t val2 = 0;         // Valor le�do del potenci�metro en A7
uint8_t leds_val = 0;      // Valor a mostrar en los LEDs
uint8_t last_command = 0;  // �ltimo comando SPI recibido (no usado activamente)

void LEDS_PORT(uint8_t mascara);  // Prototipo de la funci�n para mostrar en LEDs

// Arreglo para mapear los pines del puerto D
unsigned char pines[8] = {0, 1, 2, 3, 4, 5, 6, 7};  // PD0 a PD7

int main(void)
{
	// Desactiva la UART (TX y RX), si estaba habilitada
	UCSR0B &= ~((1 << TXEN0) | (1 << RXEN0));

	// Configura todos los pines del puerto D como salidas para los LEDs
	DDRD |= 0xFF;
	PORTD = 0;  // Inicializa todos los pines en bajo (LEDs apagados)

	// Inicializa SPI en modo esclavo, MSB primero, reloj en reposo alto, datos en primer flanco
	SPI_init(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_HIGH, SPI_CLOCK_FIRST_EDGE);

	// Habilita la interrupci�n por SPI (cuando se recibe un dato)
	SPCR0 |= (1<<SPIE);

	// Inicializa el convertidor anal�gico a digital
	ADC_init();

	// Habilita las interrupciones globales
	sei();

	while (1)
	{
		// Lee valores de los potenci�metros en A6 y A7 continuamente
		val1 = ADC_read(6); // Canal A6
		val2 = ADC_read(7); // Canal A7

		// Retardo para dar tiempo al ADC a estabilizarse y reducir ruido
		_delay_ms(100);
	}
}

// Interrupci�n del SPI: se ejecuta cuando se recibe un dato desde el maestro
ISR(SPI0_STC_vect){
	uint8_t spiValor = SPDR0; // Leer el valor recibido del maestro

	// Si el maestro pide el valor del potenci�metro 1
	if (spiValor == 'a') {
		SPDR0 = val1 / 4; // Env�a el valor del ADC6 en formato de 8 bits (escala 0-255)
	}
	// Si el maestro pide el valor del potenci�metro 2
	else if (spiValor == 'b') {
		SPDR0 = val2 / 4; // Env�a el valor del ADC7 en formato de 8 bits
	}
	else {
		// Si no es 'a' ni 'b', se interpreta como un n�mero para mostrar en LEDs
		LEDS_PORT(spiValor); // Muestra el valor en los LEDs
		SPDR0 = 0xFF;        // Devuelve valor fijo como "ack" (opcional)
	}
}

// Funci�n para encender/apagar LEDs conectados a PORTD seg�n una m�scara de bits
void LEDS_PORT(uint8_t mascara) {
	for(int i = 0; i < 8; i++) {  // Recorre los 8 bits del byte
		uint8_t pin = pines[i];   // Obtiene el n�mero de pin asociado
		if(mascara & (1 << i)) {  // Si el bit i est� en 1
			PORTD |= (1 << pin);  // Enciende el LED correspondiente
			} else {
			PORTD &= ~(1 << pin); // Apaga el LED correspondiente
		}
	}
}

