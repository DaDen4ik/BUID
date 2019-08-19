#ifndef UART_H
#define	UART_H
#include "config_bits.h"
bit Data_UART_Init(const long int);
bit Control_UART_Init(const long int);
bit UART_TX_Empty() ;
bit UART_Data_Ready();
char UART_Read();
void UART_Read_Text(char *, unsigned int);
void UART_Write(char);
void Control_UART_Write(char);
void UART_Write_Text(char *);

bit Data_UART_Init(const long int baudrate)
{
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);
	if(x>255)
	{
		x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
		BRGH2 = 1;
	}
	if(x<256)
	{
      TRISG1 = 0;
      TRISG2 = 1;
        TXSTA2 = 0x20;
        RCSTA2 = 0x90;
        BAUDCON2 = 0x40;
        SPBRG2 = 0x11;
	  return 1;
	}
	return 0;
}
bit Control_UART_Init(const long int baudrate)
{
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);
	if(x>255)
	{
		x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
		BRGH1 = 1;
	}
	if(x<256)
	{
        TRISC7 = 1;
        TRISC6 = 0;
        TXSTA1 = 0x20;
        RCSTA1 = 0x90;
        BAUDCON1 = 0x40;
        SPBRG1 = 0x11;
	  return 1;
	}
	return 0;
}
bit UART_TX_Empty() 
{
  return TRMT;
}

bit UART_Data_Ready()
{
   return RC1IF;
}
char UART_Read()
{
  while(!RC1IF);
  return RCREG1;
}

void UART_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(i=0;i<length;i++)
		Output[i] = UART_Read();
}

void UART_Write(char data)
{
    RG0 = 1;
 // RA5 = 1;
    TXREG2 = data;
    while(!TRMT2);
 // RA5 = 0;
  //__delay_us(5);
  RG0 = 0;
}
void Control_UART_Write(char data)
{
  RA5 = 1;
    TXREG1 = data;
    while(!TRMT1);
  RA5 = 0;
  //__delay_us(5);
}

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
	  UART_Write(text[i]);
}
#endif
