/*
 * UART.h
 *
 * Created: 25/07/2025 08:51:29
 *  Author: valen
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>

void UART_Init(void);

unsigned char UART_Read(void);

void UART_Num(uint8_t num);

void UART_Char(unsigned char caracter);

void UART_Write(char* cadena);

// convierte float a string
void float_to_stringu(float num, char *buffer, uint8_t decimales);

void UART_Var(float v);

void uint8_to_string(uint8_t num);

void uint16_to_string(uint16_t num);



#endif /* UART_H_ */