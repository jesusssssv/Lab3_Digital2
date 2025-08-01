#define F_CPU 16000000UL  // Define la frecuencia del reloj del sistema (16 MHz) para funciones de retardo

#include <avr/io.h>         // Librería de E/S para trabajar con registros y periféricos del AVR
#include <util/delay.h>     // Librería para utilizar _delay_ms() y otros retardos
#include "SPI.h"            // Archivo de cabecera personalizado para el manejo de SPI
#include "UART.h"           // Archivo de cabecera personalizado para manejo de UART (funciones de escritura y lectura)
#include "UART2.h"          // Posible segunda interfaz UART (o funciones auxiliares UART)

uint8_t valor_spi = 0;      // Variable que almacena el valor recibido por SPI
float volt1 = 0;            // Voltaje leído del canal 1 (potenciómetro 1)
float volt2 = 0;            // Voltaje leído del canal 2 (potenciómetro 2)
int numero = 0;             // Variable general para números (sin uso directo en main)
unsigned char dato = 0;     // Variable auxiliar (sin uso directo en main)
uint8_t leds_val = 0;       // Valor que se envía al puerto de LEDs

void LEDS_PORT(uint8_t mascara);  // Declaración de función que controla los LEDs
unsigned char pines[8] = {2, 3, 4, 5, 6, 7, 8, 9};  // Mapeo lógico de pines para los LEDs

int main(void)
{
	// Configura pines PD2 a PD7 como salidas (6 bits del puerto D)
	DDRD |= (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7);

	// Configura pines PB0, PB1 y PB2 como salidas (por ejemplo, SPI y control LED)
	DDRB |= (1<<0)|(1<<1)|(1<<2); // PB2 se usa como CS para SPI

	// Inicializa todos los pines de los puertos D y B en bajo
	PORTD = 0;
	PORTB = 0;

	// Inicializa la interfaz SPI como maestro, con divisor de reloj por 16, MSB primero, reloj en reposo alto y toma datos en flanco de subida
	SPI_init(SPI_MASTER_OSC_DIV16, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_HIGH, SPI_CLOCK_FIRST_EDGE);

	// Inicializa UART con UBRR=103 (baudrate = 9600 si F_CPU=16MHz)
	USART_Init(103);

	// Apaga todos los LEDs al inicio
	LEDS_PORT(0);

	char opcion; // Variable para almacenar la opción del menú

	while (1)
	{
		// Mostrar menú principal por UART
		UART_Char('\n');
		UART_Write("=============== MENU ============\r\n");
		UART_Write("1. Ver voltajes de los potenciometros\r\n");
		UART_Write("2. Ingresar numero para mostrar con LEDs y SPI\r\n");
		UART_Write("Selecciona una opcion: ");

		// Espera una opción del usuario por UART
		opcion = USART_Receive();
		UART_Char(opcion);  // Eco del carácter ingresado
		UART_Char('\n');

		// Opción 1: Medición de voltajes en bucle
		if (opcion == '1') {
			UART_Write("\r\nPresiona 'x' para salir de las lecturas.\r\n");

			while (1) {
				// Verifica si hay una tecla presionada (lectura no bloqueante)
				if (UCSR0A & (1 << RXC0)) {
					char salida = UDR0; // Lee el carácter recibido directamente
					if (salida == 'x') break; // Sal del bucle si el usuario presiona 'x'
				}

				// Lectura del voltaje del canal 1 (potenciómetro 1)
				PORTB &= ~(1<<2);       // Baja CS (selecciona esclavo SPI)
				SPI_Write('a');         // Envía comando 'a' al esclavo
				valor_spi = SPI_Read(); // Lee la respuesta
				volt1 = 5.0 * valor_spi / 255.0; // Convierte a voltaje
				PORTB |= (1<<2);        // Sube CS

				// Lectura del voltaje del canal 2 (potenciómetro 2)
				PORTB &= ~(1<<2);       // Baja CS
				SPI_Write('b');         // Envía comando 'b'
				valor_spi = SPI_Read(); // Lee respuesta
				volt2 = 5.0 * valor_spi / 255.0; // Convierte a voltaje
				PORTB |= (1<<2);        // Sube CS

				// Mostrar los voltajes en UART
				UART_Char('\n');
				UART_Write("Volt 1: ");
				UART_Var(volt1);
				UART_Write(" V\r\n");

				UART_Write("Volt 2: ");
				UART_Var(volt2);
				UART_Write(" V\r\n");

				_delay_ms(500); // Pequeña pausa entre lecturas
			}
		}

		// Opción 2: Ingreso y visualización de un número
		else if (opcion == '2') {
			_delay_ms(1000); // Pausa para evitar conflictos de lectura

			// Limpia el buffer UART por si hay datos basura
			while (UCSR0A & (1 << RXC0)) {
				char temp10 = UDR0; // Descarta cualquier carácter recibido previamente
			}

			UART_Write("\r\nIngresa un numero (0-255): ");
			uint8_t numero = USART_GetNum();  // Función que recibe un número completo desde UART

			// Confirma el número ingresado
			UART_Write("\r\nNumero ingresado: ");
			UART_Num(numero);
			UART_Write("\r\n");

			// Muestra el número en los LEDs
			LEDS_PORT(numero);

			// Envía el número por SPI
			PORTB &= ~(1<<2);        // Baja CS
			SPI_Write(numero);       // Envía número
			valor_spi = SPI_Read();  // Lee respuesta (aunque no se usa aquí)
			PORTB |= (1<<2);         // Sube CS
		}

		// Manejo de opción inválida
		else {
			UART_Write("Opcion invalida. Intenta nuevamente.\r\n");
		}
	}
}

// Función para controlar el encendido/apagado de LEDs según una máscara de bits
void LEDS_PORT(uint8_t mascara) {
	for(int i = 0; i < 8; i++) {  // Itera sobre los 8 bits
		if (mascara & (1 << i)) { // Si el bit está en 1, enciende el LED correspondiente
			uint8_t pin = pines[i]; // Mapea el índice al número de pin real
			if (pin >= 8 && pin <= 13) {
				PORTB |= (1 << (pin - 8)); // Enciende LED conectado a PORTB
				} else if (pin <= 7) {
				PORTD |= (1 << pin); // Enciende LED conectado a PORTD
			}
			} else { // Si el bit está en 0, apaga el LED correspondiente
			uint8_t pin = pines[i];
			if (pin >= 8 && pin <= 13) {
				PORTB &= ~(1 << (pin - 8)); // Apaga LED en PORTB
				} else if (pin <= 7) {
				PORTD &= ~(1 << pin); // Apaga LED en PORTD
			}
		}
	}
}


