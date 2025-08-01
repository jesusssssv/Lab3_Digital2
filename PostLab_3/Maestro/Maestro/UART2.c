/*
 * UART2.c
 *
 * Created: 27/07/2025 10:58:36
 *  Author: valen
 */ 
#include "UART2.h"

char rxBuffer[BUFFER_SIZE];  // Arreglo para almacenar los caracteres
uint8_t rxIndex2 = 0;  // Índice para almacenar caracteres en el array
int receivedNumber = 0;  // Número convertido desde el buffer

void USART_Init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Habilitar RX, TX e interrupción
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Configurar 8 bits de datos
}

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Esperar hasta que el buffer esté vacío
	UDR0 = data;
}

void USART_Transmit_String(const char* str) {
	while (*str) {
		USART_Transmit(*str++);
	}
}

unsigned char USART_Read(void){
	if(UCSR0A&(1<<7)){			// si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;			//d evuelve el dato almacenado en el registro UDR0
	}
	else
	return 0;
}

unsigned char USART_Receive(void)
{
	// Espera hasta que se reciba un dato (bit RXC0 en UCSRA se pone en 1)
	while (!(UCSR0A & (1 << RXC0)));
	
	// Retorna el dato recibido de UDR0
	return UDR0;
}

uint8_t USART_GetNum(void) {
	char rxBuffer[4] = {0};
	uint8_t rxIndex = 0;
	char receivedChar;
	
	// LIMPIAR BUFFER DE RECEPCIÓN PRIMERO
	while (UCSR0A & (1 << RXC0)) {
		receivedChar = UDR0;  // Leer y descartar caracteres residuales
	}
	
	while (1) {
		// Espera a que llegue un carácter NUEVO
		while (!(UCSR0A & (1 << RXC0)));
		receivedChar = UDR0;
		
		// Si es un dígito
		if (receivedChar >= '0' && receivedChar <= '9') {
			if (rxIndex < 3) {
				rxBuffer[rxIndex++] = receivedChar;
				USART_Transmit(receivedChar);  // Eco del dígito
			}
		}
		// Enter termina la entrada
		else if (receivedChar == '\r' || receivedChar == '\n') {
			USART_Transmit('\r');
			USART_Transmit('\n');
			rxBuffer[rxIndex] = '\0';
			break;
		}
		// Ignorar otros caracteres completamente
	}
	
	if (rxIndex == 0) return 0;
	
	uint16_t temp = (uint16_t)atoi(rxBuffer);
	return (temp > 255) ? 255 : (uint8_t)temp;
}
