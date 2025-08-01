/*
 * UART2.h
 *
 * Created: 27/07/2025 10:59:15
 *  Author: valen
 */ 


#ifndef UART2_H_
#define UART2_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define BUFFER_SIZE 4  // Tamaño del buffer (3 dígitos + terminador nulo)

void USART_Init(unsigned int ubrr);
unsigned char USART_Read(void);
void USART_Transmit(char data);
void USART_Transmit_String(const char* str);
int USART_GetReceivedNumber(void);
uint8_t USART_GetNum(void);
unsigned char USART_Receive(void);




#endif /* UART2_H_ */