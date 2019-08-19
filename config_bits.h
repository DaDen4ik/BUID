/*
 * File:   config_bits.h
 * Author: PRO-DIOD.RU
 *
 */

#ifndef CONFIG_BITS_H
#define	CONFIG_BITS_H
#define _XTAL_FREQ 16000000 
typedef unsigned char u8; //stdint
typedef unsigned int u16; //stdint
typedef unsigned long int u32; //stdint

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#include <xc.h>
#include <PIC18F6525.h>


/** C O N F I G U R AT I O N B I T S ******************************/
#pragma config BOR = ON // Brown-out Reset Enable bit (BOR enabled)
/* CONFIG1H */
//#pragma config FOSC = INTIO67   // ������ ���������� ������������ ��� I/O (1000)
#pragma config OSC = HS  // ������ ���������� ������������ ��� ������ �������� ������� (�������� �� 4)
//#pragma config FOSC = 0b1001 // �� ��� � 1000, �� ������ ��� ���������?
                                /* � ������� FOSC (3:0) ������������� ������:
                                XT
                                XT
                                HS
                                HSPLL
                                RC
                                RCIO
                                INTOSC
                                INTOSCIO
                                EC
                                ECIO
                        11xx = External RC oscillator, CLKOUT function on RA6
                        101x = External RC oscillator, CLKOUT function on RA6
                        1001 = Internal oscillator block, CLKOUT function on RA6, port function on RA7
                        1000 = Internal oscillator block, port function on RA6 and RA7
                        0111 = External RC oscillator, port function on RA6 // �� ���������
                        0110 = HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1)
                        0101 = EC oscillator, port function on RA6
                        0100 = EC oscillator, CLKOUT function on RA6
                        0011 = External RC oscillator, CLKOUT function on RA6
                        0010 = HS oscillator
                        0001 = XT oscillator
                        0000 = LP oscillator
                                 */
//#pragma config IESO = OFF       // ����� ������������ ����� ������� � ������� ����������� ��������
//#pragma config FCMEN = OFF      // ������� �������� ������ ��������� ���������� ��������

/* CONFIG2L */
//#pragma config PWRT = OFF       // ������ ������� �� ��������� �������
//#pragma config BOREN = SBORDIS  // �������� �� �������� ���������� �������
#pragma config BORV = 1         // ������� ������ �� �������� �������
                                /*
                                3 = VBOR set to 1.8V nominal
                                2 = VBOR set to 2.2V nominal
                                1 = VBOR set to 2.7V nominal
                                0 = VBOR set to 3.0V nominal
                                 */

/* CONFIG2H */
#pragma config WDT = ON      // ���������� ������ ��������
#pragma config WDTPS = 32768    // ������������ ������� ��������� �� 32768

/* CONFIG3H */
#pragma config MCLRE = ON       // 1 = MCLR pin enabled; RE3 input pin disabled
//#pragma config LPT1OSC = OFF    // 1 = ������1 ��������������� ��� ������ � ������ ������������������
//#pragma config PBAD = OFF     // PORTB = 1 - ���������� ����� PORTB, ����� ��������
//#pragma config CCP2MX = PORTC   // 1 = CCP2 input/output is multiplexed with RC1, 0 - multiplexed with RB3

/* CONFIG4L */
#pragma config STVR = ON      // ����� �� ������������ ����� �������
#pragma config LVP = OFF        // ������������ ���������������� ���������, ����� ������������ ��� ����
//#pragma config XINST = OFF      // ����������� ���������� ���������
#pragma config DEBUG = OFF      // ����� ��������� ��������

/* CONFIG5L */
#pragma config CP0 = OFF        // ������ ���� ���������
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#//pragma config CP3 = OFF

/* CONFIG5H */
#pragma config CPB = OFF        // ������ ���� ����������
#pragma config CPD = OFF        // ������ ���� EEPROM

/* CONFIG6L */
#pragma config WRT0 = OFF       // ������ ������ �� ����������
#pragma config WRT1 = OFF       //
#pragma config WRT2 = OFF       //
//#pragma config WRT3 = OFF       //

/* CONFIG6H */
#pragma config WRTB = OFF       // ������ �� ���������� ���������� ���������
#pragma config WRTC = OFF       // ������ �� ���������� ����������������� ��������(300000-3000FFh) ���������
#pragma config WRTD = OFF       // ������ �� ���������� EEPROM ���������

/*CONFIG7L*/
#pragma config EBTR0 = OFF      // ������ ������ �� ���������� ������
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
//#pragma config EBTR3 = OFF

/* CONFIG7H */
#pragma config EBTRB = OFF      // ������ ������ �� ���������� ������

#endif	/* CONFIG_BITS_H */

