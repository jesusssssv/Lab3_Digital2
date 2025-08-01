/*
 * UART.c
 *
 * Created: 25/07/2025 08:49:54
 *  Author: valen
 */ 
#include "UART.h"
#include <stdlib.h>
#define BUFFER_SIZE 4  // Tamaño del buffer (3 dígitos + terminador nulo)
char rxBuffer[BUFFER_SIZE];  // Arreglo para almacenar los caracteres
uint8_t rxIndex = 0;  // Índice para almacenar caracteres en el array


void UART_Init(void) {
	unsigned int ubrr = 103;
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  // Habilitar RX, TX e interrupción
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Configurar 8 bits de datos
}

unsigned char UART_Read(void){
	if(UCSR0A&(1<<7)){			// si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;			//d evuelve el dato almacenado en el registro UDR0
	}
	else
	return 0;
}

void UART_Num(uint8_t num) {
	char buffer[4];
	itoa(num, buffer, 10);
	UART_Write(buffer);
}


void UART_Char(unsigned char caracter){
	while(!(UCSR0A&(1<<5)));    // espera mientras se libera
	UDR0 = caracter;            // listo para enviar
}

void UART_Write(char* texto){
	uint8_t i;
	for (i=0; texto[i] != '\0'; i++){
		while(!(UCSR0A&(1<<UDRE0)));
		UDR0 = texto[i];
	}
}

void float_to_stringu(float num, char *buffer, uint8_t decimales) {
	int parte_entera = (int)num;  // parte entera
	int parte_decimal = (int)((num - parte_entera) * 100);  // parte decimal
	
	if (parte_decimal < 0) parte_decimal *= -1;  // no negativo

	// Convertir a caracteres
	int i = 0;
	if (parte_entera == 0) {
		buffer[i++] = '0';
		} else {
		int temp = parte_entera;
		char temp_buffer[10];
		int j = 0;
		while (temp > 0) {  // Extraer dígitos
			temp_buffer[j++] = (temp % 10) + '0';
			temp /= 10;
		}
		while (j > 0) {  // Invertir los dígitos
			buffer[i++] = temp_buffer[--j];
		}
	}

	// punto decimal
	buffer[i++] = '.';

	// decimal a caracteres
	buffer[i++] = (parte_decimal / 10) + '0';
	buffer[i++] = (parte_decimal % 10) + '0';

	
	buffer[i] = '\0';
}

void UART_Var(float v){
	char str[10];
	float_to_stringu(v, str, 2);
	UART_Write(str);
}

void uint8_to_string(uint8_t num) {
	char buffer[4];  // Máximo 3 dígitos + '\0'
	uint8_t i = 0;
	char temp[4];  // Almacena los dígitos en orden inverso
	uint8_t j = 0;

	if (num == 0) {
		buffer[i++] = '0';
		} else {
		while (num > 0) {
			temp[j++] = (num % 10) + '0';  // Extrae el último dígito
			num /= 10;
		}
		while (j > 0) {
			buffer[i++] = temp[--j];  // Invierte el orden
		}
	}

	buffer[i] = '\0';  // Termina el string

	UART_Write(buffer); // Enviar el string por UART
}


void uint16_to_string(uint16_t num) {
	char buffer[6];  // Máximo 5 dígitos + '\0'
	uint8_t i = 0;
	char temp[6];  // Almacena los dígitos en orden inverso
	uint8_t j = 0;

	if (num == 0) {
		buffer[i++] = '0';
		} else {
		while (num > 0) {
			temp[j++] = (num % 10) + '0';  // Extrae el último dígito
			num /= 10;
		}
		while (j > 0) {
			buffer[i++] = temp[--j];  // Invierte el orden
		}
	}

	buffer[i] = '\0';  // Termina el string

	UART_Write(buffer); // Envía el string por UART
}