#ifndef LCD_H
#define	LCD_H
#include "config_bits.h"
// define LCD lines connected to MCU's pins (can be used any pins)
#include"user_function.h"
// LCD line RW connected to Vss (ground)
#define RW_PIN  TRISG4=0
#define RW_H    RG4=1
#define RW_L    RG4=0
// LCD line RS connected to RD6 pin
#define RS_PIN  TRISF3=0
#define RS_H    RF3=1
#define RS_L    RF3=0

// LCD line E connected to RD7 pin
#define EN_PIN  TRISG3=0
#define EN_H    RG3=1
#define EN_L    RG3=0

// LCD data bus (4-bit interface)
// data line 4..7 connected to RB0..RB3 pins
#define D4_PIN  TRISF4=0
#define D4_H    RF4=1
#define D4_L    RF4=0

#define D5_PIN  TRISF5=0
#define D5_H    RF5=1
#define D5_L    RF5=0

#define D6_PIN  TRISF6=0
#define D6_H    RF6=1
#define D6_L    RF6=0

#define D7_PIN  TRISF7=0
#define D7_H    RF7=1
#define D7_L    RF7=0
/*
// LCD backlight line connected to RD5 pin
#define BL_PIN  TRISD5=0
#define BL_ON   RD5=1
#define BL_OFF  RD5=0
*/

//typedef unsigned char u8; //stdint


void lcd_init(void);
void lcd_write(u8 data);
void lcd_cmd(u8 data);
void lcd_clear(void);
void lcd_home(void);
void lcd_goto(u8, u8);
void lcd_char(u8);
void lcd_print(const char *str,const u8);
void lcd_right(u8);
void lcd_left(u8);
u8 lcd_read(void);
void lcd_right_shift(u8);
u8 Find_Devises(u8);

void Hello_screen(void);
void Par_screen_DWES(const char *,const char *);
void Par_screen_IPSES(const char*,const char*,const char*);
void Par_screen_SPEED(const char*);
void Par_screen_ANGLE(const char*);
void Par_screen_TEMPO(const char*);
void Par_screen_HUM(const char*);
void Par_screen_PRESS(const char*);
void Par_screen_WORKTIME(const char*);
void screen_Raven();
void Clearing(u8);

///////////////////////////////////////////////////////////////////////////////
// Initialization LCD
void lcd_init(void)
{
    RS_H;
    RW_H;
    __delay_ms(20); //delay on power up
    RS_L;
    RW_L;
    lcd_write(0x3);
    RW_H;
    RS_H;
    __delay_ms(10); //wait for the instruction to complete
    RS_L;
    RW_L;
    lcd_write(0x3);
    RW_H;
    RS_H;
    __delay_us(250); //wait for the instruction to complete
    RS_L;
    RW_L;
    lcd_write(0x3);
    RW_H;
    RS_H;
    __delay_us(50);
    RS_L;
    RW_L;
    lcd_write(0x2);
    RW_H;
    RS_H;
    __delay_us(50);
    lcd_cmd(0x28);
    /////////////////////////// 08
    __delay_us(50);
    lcd_cmd(0x0C);
    ///////////////////////// 01
    __delay_us(50);
    lcd_cmd(0x01);
    __delay_us(500);
}

///////////////////////////////////////////////////////////////////////////////
void lcd_write(u8 data) //write data to LCD
{
    LATF &= ~(0xF0);
    __delay_us(20);
    EN_H;
    __delay_us(20);
    TRISF &= 0x0F;
    if(data & 1)
        LATF |= (1<<4);
    if(data & 2)
        LATF |= (1<<5);
    if(data & 4)
        LATF |= (1<<6);
    if(data & 8)
        LATF |= (1<<7);  
    __delay_us(30); 
    EN_L;
    __delay_us(20);
    LATF &= ~(0xF0);
    TRISF &= ~(0xF0);
}

///////////////////////////////////////////////////////////////////////////////
void lcd_cmd(u8 data) //send command to LCD
{ 
    RS_L;
    RW_L;
    __delay_us(20);
    lcd_write(data>>4);
    lcd_write(data);
    RW_H;
    RS_H;
    __delay_us(30);
}

///////////////////////////////////////////////////////////////////////////////
void lcd_char(u8 sign) //print a character
{
    RS_L;
    __delay_us(5); 
    RS_H;
    RW_L;
    __delay_us(20);
    lcd_write(sign>>4);
    lcd_write(sign);
    RW_H;
    RS_L;
    __delay_us(50);
}

///////////////////////////////////////////////////////////////////////////////
void lcd_clear(void) //clear screen
{
    RW_L;
    lcd_cmd(1);
    __delay_ms(1);
}

///////////////////////////////////////////////////////////////////////////////
void lcd_goto(u8 line, u8 column) //line 0..1, column 0..39
{
    //lcd_cmd(((0x80+(line<<6))+column)>>4);
    lcd_cmd((0x80+(line<<6))+column);
    Delay(50);
}


///////////////////////////////////////////////////////////////////////////////
void lcd_print(const char *str,const u8 p) //print a string
{
    u8 count = 0;
    while(*str) 
    {
        lcd_char(*str++);
        if (++count == p)
            break;
    }
    __delay_us(100);
}


///////////////////////////////////////////////////////////////////////////////
void lcd_right(u8 Count ) //shift right display 
{
    while ( Count-->0 )
        lcd_cmd(0x14);
    __delay_us(50);
}
void lcd_right_shift(u8 Count ) //shift right display 
{
    while ( Count-->0 )
        lcd_cmd(0x18);
    __delay_us(50);
}

///////////////////////////////////////////////////////////////////////////////
void lcd_left(u8 Count ) //shift left display
{
  while ( Count-->0 )
    lcd_cmd(0x10);
    __delay_us(50);
}
void lcd_home()
{
    lcd_cmd(2);
        __delay_ms(2);
}
u8 lcd_read_BF(void) //print a character
{
    u8 i;
    RW_H;
    RS_L;
    TRISF |= (1<<7);
    __delay_us(20);
    EN_H;
    __delay_us(15);
    EN_L;
    i = RF7;
    TRISF &= ~(1<<7);
    return (i);
}
//////////////////////Пользовательские функции //////////////////

void Hello_screen(void) // Экран приветствия: Название прибора, версия проивки
{
    lcd_right(4);

    lcd_char(0xA0);lcd_char(0xBB);lcd_char(0x6F);lcd_char(0xBA);// Блок
    lcd_char(0x20);
    lcd_char(0x79);lcd_char(0xBE);lcd_char(0x70);
    lcd_char(0x61);lcd_char(0xB3);lcd_char(0xBB);
    lcd_char(0x65);lcd_char(0xBD);lcd_char(0xB8);lcd_char(0xC7);// Управления
    lcd_right(5);
    lcd_char(0xA5);lcd_char(0xA8);lcd_char(0x43);
    lcd_char(0xAF);lcd_char(0x43);// Ипсэс
    lcd_char(0x2F); //
    lcd_char(0xE0);lcd_char(0x42); lcd_char(0xAF);lcd_char(0x43);  // ДВЭС 

    lcd_char(0x20);
    lcd_char(0x76);  
    lcd_char(0x31);
    lcd_char(0x2E);
    lcd_char(0x30);
    lcd_cmd(0x02);
    
    ///////////////////////////
    RB5 = 1;
    ms_Delay(100);
    RB5 = 0;
    //lcd_clear;
    ms_Delay(6000);
    //lcd_cmd(1);
    lcd_goto(0,0);
    Clearing(80);
}
u8 Find_Devises(u8 Device_Flag)
{
    //lcd_right(1);
	switch (Device_Flag)
	{
        
		case 0xF:
            lcd_cmd(1);
            lcd_goto(0,4);
            lcd_char(0xa9);lcd_char(0x63);lcd_char(0xbf);
            lcd_char(0x70);lcd_char(0x6f);lcd_char(0xb9);
            lcd_char(0x63);lcd_char(0xbf);lcd_char(0xb3);lcd_char(0x61);// Устройства
            lcd_goto(0,23);
            lcd_char(0xbd);lcd_char(0x65); // не
            lcd_right(1);
            lcd_char(0xbe);lcd_char(0x6f);lcd_char(0xe3);
            lcd_char(0xba);lcd_char(0xbb);lcd_char(0xc6);
            lcd_char(0xc0);lcd_char(0x65);lcd_char(0xbd);
            lcd_char(0xc3);lcd_char(0x3a); // подключены
            return 0xF;
//        break;
        		case 0:
            lcd_right(5);
            lcd_char(0xA8);lcd_char(0x6f);lcd_char(0xba); 
            lcd_char(0xbb);lcd_char(0xc6);lcd_char(0xc0); 
            lcd_char(0x65);lcd_char(0xbd);lcd_char(0xc3);lcd_char(0x3a); // Подключены
            
            lcd_right(8);
            
            lcd_char(0xE0); // Д
            lcd_char(0x42); // в
            lcd_char(0xAF); // э
            lcd_char(0x43); // с
            
            lcd_right(4);
            
            lcd_char(0xA5); // И
            lcd_char(0xA8); // п    
            lcd_char(0x43); // с
            lcd_char(0xAF); // э
            lcd_char(0x43); // с
        break;
		case 1:
            lcd_right(5);
            lcd_char(0xA8);lcd_char(0x6f);lcd_char(0xe3);
            lcd_char(0xba);lcd_char(0xbb);lcd_char(0xc6);
            lcd_char(0xc0);lcd_char(0x65);lcd_char(0xbd);
            lcd_char(0xc3);lcd_char(0x3a); // Подключены
            
            lcd_right(8);
            
            lcd_char(0xA5); // И
            lcd_char(0xA8); // п    
            lcd_char(0x43); // с
            lcd_char(0xAF); // э
            lcd_char(0x43); // с
        break;
		case 2:

            lcd_right(5);
            lcd_char(0xA8);lcd_char(0x6f);lcd_char(0xba); 
            lcd_char(0xbb);lcd_char(0xc6);lcd_char(0xc0); 
            lcd_char(0x65);lcd_char(0xbd);lcd_char(0xc3);lcd_char(0x3a); // Подключены
            
            lcd_right(8);
            
            lcd_char(0xE0); // Д
            lcd_char(0x42); // в
            lcd_char(0xAF); // э
            lcd_char(0x43); // с
        break;
	}
    ms_Delay(4000);
    lcd_goto(0,0);
    Clearing(80);
	return Device_Flag;
}
void Par_screen_DWES(const char * speed,const char * angle)
{
    lcd_goto(0,0);
    
    lcd_char('V');
    Par_screen_SPEED(speed);
    
    lcd_goto(1,0);
    
    lcd_char('Q');
    Par_screen_ANGLE(angle);
}
void Par_screen_IPSES(const char * tempo,const char *hum,const char *press)
{
    lcd_goto(0,0);
    lcd_char('T');Par_screen_TEMPO(tempo);
    lcd_goto(1,0);
    lcd_char('H');Par_screen_HUM(hum);
    lcd_goto(0,20);
    Par_screen_PRESS(press);
}

void Par_screen_SPEED(const char * speed)
{
    char flag = 0;
    if (speed[1] != '.')
            flag = 1;
    lcd_right(1);
    lcd_char('=');
    if ( flag == 1 )
        lcd_char(speed[0]);
    else
    {
        lcd_right(1);
        lcd_char(speed[0]);
    }
    lcd_char(speed[1]);
    lcd_char(speed[2]);
    lcd_char(speed[3]);
    lcd_char(0xBC);lcd_char(0x2F);lcd_char(0x63);// м/c
}

void Par_screen_ANGLE(const char * angle)
{
    char flag = 0;
    screen_Raven();
    lcd_char(angle[0]);
    lcd_char(angle[1]);
    lcd_char(angle[2]);
    if ( angle[2] == '.')
    lcd_char(angle[3]);
    lcd_char(0xB4);
    lcd_char(0x70);
    lcd_char(0xE3);
}
void Par_screen_WORKTIME(const char * time)
{
    char flag = 0;
    if (time[1] != '.')
            flag = 1;
    lcd_char('T');
    lcd_char('w');
    lcd_char(time[0]);
    lcd_char(time[1]);
    lcd_char(time[2]);
	if (flag)
		lcd_char(time[2]);
}

void Par_screen_HUM(const char * hum)
{
	char flag = 0;
    
    screen_Raven();
    if (hum[1] != '.')
        flag = 1;
    
    if ( flag == 1 )
        lcd_char(hum[0]);
    else
        lcd_right(1);
    lcd_char(hum[1]);
    lcd_char(hum[2]);
    lcd_char(hum[3]);
    lcd_char('%');
}
void Par_screen_TEMPO(const char * tempo)
{
	char flag = 0;
    
    screen_Raven();
//    if (tempo[0] != '-')
//        lcd_right(1);
    lcd_char(tempo[0]);
    lcd_char(tempo[1]);
    lcd_char(tempo[2]);
    lcd_char(tempo[3]);
    if (tempo[0] == '-')
        lcd_char(tempo[4]);
    lcd_char(0xEF);
    lcd_char('c');
}

void Par_screen_PRESS(const char * press)
{
    lcd_char('P');
    screen_Raven();
    lcd_char(press[0]);
    lcd_char(press[1]);
    lcd_char(press[2]);
	lcd_char(press[3]);
    lcd_char(press[4]);
    if ( press[4] == '.' )
        lcd_char(press[5]);
    lcd_char(0xB4);lcd_char(0xA8);lcd_char('a'); // гПа
}


void screen_Raven()
{
    lcd_right(1);
    lcd_char('=');
    lcd_right(1);
}
// Функция затирания ячеек без функции очищения экрана
void Clearing(u8 Count)
{
    u8 temp = Count;
    while (temp-- != 0)
        lcd_char(0x20);
    lcd_goto(0,0);
}






#endif