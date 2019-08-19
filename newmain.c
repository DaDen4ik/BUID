#include <stdlib.h>
#include<stdio.h>
#include<math.h>
//#include <plib.h>
//#include "config_bits.h"
#include"lcd.h"
#include"uart.h"
#include"struct.h"
#include"user_function.h"


#define SizeBuff 220
#define SizeIpParam 220
#define SizeDwParam 80
// Variable
u8 TempDev = 0; u8 ChkFlag = 0xF;  // Обнаружение устройств
u8 m_bFlag = 1; // флаг разрешения чтения с устройства Modbus
u8 m_fFlag = 0; // флаг получения кадра по Modbus
u16 CRCtemp; 
u8 Deflag = 0;
u16 StReg = 0; // Запись стартового регистра для операций записи
u8 WordSuccess = 0;	// Количество слов в кадре Modbus
u8 Wait = 0; int i,j;
u8 Menu_Level = 0; // переменная перемещения по пунктам меню
u8 fChange_menu = 0; // флаг перемещения между элементами меню
u8 ErrFlag = 0; // Флаг ошбки
u16 IPout = 0; //
u16 DWout = 0; //
u8 admin1234 = 0; // Флаг включения защищенного режима
u8 Selectflag = 0; // переменная отображения параметров среды на ГЭ
u8 Column = 0; u8 Stan = 0; // СТроки столбцы// для запоминания нахождения на экране
u8 fTemp_acc = 4,fHum_acc = 4,fPress_acc = 6,fSpeed_acc = 4, fAngle_acc = 5;
u8 * accPtr;
u8 Changeflag= 0; 
u16 Timer = 0;
char SpeedMas[12];
char AngleMas[12];
char TempeMas[12];
char HumMas[12];
char PresMas[12];
char MaxV[12];
char AvgV[12];
char AvgQ[12];

// Massive
u8 ModBuff[SizeBuff];
u8 ModBuss[SizeBuff]; u8 ConWord = 0;
u16 DWparam[SizeDwParam];
u16 IPparam[SizeIpParam];

Dwes_param * DwesParam; // Указатель на стру
Ipses_param * IpsesParam; //

// functions
void IntConfig();
void PinConfig();
void Border();
void Mode_menu();
void Watch_menu(); // Функция описания реакции на нажатие клавиш
void ErrConnect();
void decoShet(const u32);

u8 ParamToModbus(u8,u8*,u32,u8); // Буду тестить эту темку
u8 CRCcheck(const u8 *,u8); 
u8 ModBusToParam(u8,u8*,u16);
u16 mb_Message_read(u8,u8*,u8,u8);
u8 Find_Devises(u8);
static const u16 CRCTable[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };
u16 CRC16(const u8 *nData,const u16 *wCRCTable, u16 wLength)	// ??????? ??????? ??????????? ????? CRC-16
{
	u8 nTemp;
	u16 wCRCWord = 0xFFFF;

	while (wLength--)
	{
		nTemp = *nData++ ^ wCRCWord;
		wCRCWord >>= 8;
		wCRCWord ^= wCRCTable[nTemp];
	}
	return wCRCWord;
}
void low_priority interrupt  isr_low (void)
{
    if (RC1IF) 
    {
        LATD5 = 1;
       
        ModBuss[ConWord++] = RCREG1;
        if (ConWord == SizeBuff)
            ConWord = 0;
        if (CRCcheck(ModBuss,ConWord))
        {
            if ((ModBuss[0] == 0x21) &  (ModBuss[1] == 0x04) )
            {
                    Control_UART_Write(DWparam[32]>>8);
                    Control_UART_Write(DWparam[32]);
                    Control_UART_Write(DWparam[34]>>8);
                    Control_UART_Write(DWparam[32]);
                    ms_Delay(1);
            }
            ConWord = 0;
            return;
        }
    }
//    else
}
void interrupt  isr (void)
{

    if (RC2IF) 
    {
        ModBuff[WordSuccess++] = RCREG2;
        if (WordSuccess == SizeBuff)
            WordSuccess = 0;
    
        // Определение подключенных устройств по первым двум байтам ответа по Modbus
        if (CRCcheck(ModBuff,WordSuccess))
        {
            if ( ChkFlag == 0xF ) // Если Девайсы ещё не определены
            {
                if (ModBuff[0] == 0x40) // Если обнаружили ДВЭС
                    TempDev = (TempDev == 2) ? 0 : 1;
                else // Если обнаружили ИПСЭС
                    if (ModBuff[0] == 0x44)
                       TempDev = ( TempDev == 1 ) ? 0 : 2;
            }
            else
                m_fFlag = 1;

            WordSuccess = 0;
            return;
        }
    }
    if (TMR1IF & TMR1IE)
    {
        
        TMR1IF = 0; //  Снять прерывание по таймеру
        Mode_menu();  
            Timer++;
            if (Timer == 0xA0)
            {
                Timer = 0;
                if (ChkFlag != 1)
                    if ((Menu_Level >0 ) & (Menu_Level < 5))
                    {
                        if (Menu_Level!=4)
                        {
                            Menu_Level++;
                        }
                        else
                            Menu_Level = ChkFlag;
                        fChange_menu = 1;

                    }
                
//                if  (!Menu_Level) // Завели таймер на 30 секунд
//                {
                    Changeflag++;
                    if (Changeflag == 3)
                        Changeflag = 0;
//                }
            }
    }
    return;
} 
int main(void)
{
    
    DwesParam = (Dwes_param *) DWparam;
    IpsesParam = (Ipses_param* ) IPparam;
    // 
    PinConfig(); // Выставляем значения и направления на выходах
    lcd_init(); // ПОдрубаем Экран и функции его
    Hello_screen();
    Data_UART_Init(9600);
    Control_UART_Init(9600);
    IntConfig();
    ///////////////
    //Control_UART_Write(0xFF);
    /////////////////
    // Определение подключенных Modbus устройств
    Wait = 5; TempDev = ChkFlag;
    while ( Wait-- !=0 )
    {
        if (TempDev != 1)
        {
            mb_Message_read(1,ModBuff,2,1);
            __delay_ms(50);
        }
        if (TempDev != 2)
        {
            mb_Message_read(2,ModBuff,2,1);
            __delay_ms(50);
        }
        if (TempDev == 0)
            break;
    }
    // Вывод на экран списка подключенных устройств
    ChkFlag = Find_Devises(TempDev);
    Menu_Level = Deflag = ChkFlag;

    while(1)
    {
        CLRWDT(); // Сброс ВотчТаймера
        // Если имеются подключенные устройства
        if (ChkFlag != 0xF)
        {
            // Если разрешено отправлять запросы по Modbus
            if (m_bFlag)//
            {
                switch (ChkFlag)
                {
                    case 1: // Еслиподключен только ИПСЭС
                        StReg = mb_Message_read(1,ModBuff,178,34);
                        ms_Delay(600);
                        ModBusToParam(1,ModBuff,StReg);
                        
                        StReg = mb_Message_read(1,ModBuff,2,14);
                        ms_Delay(600); 
                    break;
                    case 2: // Если подключен только ДВЭС
                        StReg = mb_Message_read(2,ModBuff,2,28);
                        ms_Delay(600);
                        ModBusToParam(2,ModBuff,StReg);

                        StReg = mb_Message_read(2,ModBuff,62,8);
                        ms_Delay(600);
                        ModBusToParam(2,ModBuff,StReg);

                        StReg = mb_Message_read(2,ModBuff,32,26);
                        ms_Delay(600);
                    break;
                    case 0: // Если подключены ИПСЭС иДВЭС
                        switch (Deflag)
                        {
                            case 0:
                                Deflag = 1;
                            case 1:
                                StReg = mb_Message_read(1,ModBuff,178,34);
                                ms_Delay(600);
                                ModBusToParam(1,ModBuff,StReg);
                                if (admin1234 == 1)
                                {
                                    for (i = 0; i<6;i++)
                                    StReg = mb_Message_read(1,ModBuff,14+(i*32),32+(i*32));
                                    ms_Delay(600);
                                    ModBusToParam(1,ModBuff,16);
                                }
                                StReg = mb_Message_read(1,ModBuff,2,14);
                                ms_Delay(600);
                            break;
                            case 2:
                                StReg = mb_Message_read(2,ModBuff,2,28);
                                ms_Delay(600);
                                ModBusToParam(2,ModBuff,StReg);
                                
                                StReg = mb_Message_read(2,ModBuff,62,8);
                                ms_Delay(600);
                                ModBusToParam(2,ModBuff,StReg);
                                
                                StReg = mb_Message_read(2,ModBuff,32,26);
                                ms_Delay(600);
                            break;
                        }
                        break; 
                }
                // Если был получен кадр для разбора
                if (m_fFlag)
                { // Залог для функции записи/изменения
                    if (ModBuff[1] == 0x10 ) 
                        continue;
                    else
                    {
                    //Если запись параметров в память БУ успешна
                        if (ModBusToParam(Deflag,ModBuff,StReg))
                        {
                            // Выбор устройства приемника
                            if (ChkFlag == 0)
                                Deflag = (Deflag == 1) ? 2:1;
                            // Проверка метеопараметров на адекватность и пороги
                            Border();
                            // Задержка для того чтобы экран не спешил
                            ms_Delay(500);
                            // Выбираем параметры для вывода на экран
                            Watch_menu();
                            // Разрешаем дальнейшую отправку запросов
                            m_bFlag = 1;
                        }
                    }
                }
                else // Обработка ошибок
                {
                  ErrConnect();
                }
            }
        }
    }
} 
// Отправка сообщения Modbus на чтение
u16 mb_Message_read(u8 device,u8 * Mb,u8 Start,u8 Count) 
{
    u16 Begin = 0; u8 t;
    switch (device)
    {
        case 1: // IPSES
            Mb[0] =  0x40;
            break;
        case 2: //  DWES
            Mb[0] =  0x44;
            break;
    }
            Mb[1] =  0x04;
            Mb[2] =  0x00;
            Mb[3] =  Start;
            Mb[4] =  0x00;
            Mb[5] =  Count;
            CRCtemp = CRC16(ModBuff,CRCTable,6);
            Mb[6] = CRCtemp;
            Mb[7] = CRCtemp>>8;
           
            Begin =  (((u16)Mb[2])<<8) + Mb[3];
            for (t = 0; t<8;t++)
            {
                UART_Write(Mb[t]);
                Mb[t] = 0;   
            }
    return Begin;
}
 // ПРоверка контрольной суммы полученого пакета Modbus
u8 CRCcheck(const u8 * Buff,u8 Count)
{
    u8 flag = 0;
    if (((Buff[0] == 0x40 ) || ( Buff[0] == 0x44 )) || ( Buff[0] == 0x21 ))
    {
        if ((( Count == 5+Buff[2]) & (Buff[1] == 0x4)) || ((Count == 5) & (Buff[1] == 0x84))
                                || ((Count == 8) & (Buff[1] == 0x10)))
        {
                    CRCtemp = (((u16)Buff[Count-1])<<8) + Buff[Count-2];       
                    if ( CRCtemp == CRC16(Buff,CRCTable,Count-2))
                        flag = 1;
        }
    }
    return flag;
}
 // Запись полученных параметров по массивам
u8 ModBusToParam(u8 Device,u8 *Mb,u16 StReg)
{
    u8 mb_Index = 0; u16 pr_Index = StReg; // Индексы
    u16 temp; u16 * TempMass; u16 t;
    u8 begin,end;
    
    if ( ModBuff[1] == 0x04)
    {
        switch (Device)
        {
            case 1: // ИПСЭС параметры для float переворота
                TempMass = IPparam; begin = 8; end = 14;
                break;
            case 2: // ДВЭС
                TempMass = DWparam; begin = 32; end = 58;
                break;
        }  
        // заполнение массива параметров
        for (mb_Index = 3; mb_Index <= 3 + Mb[2]; mb_Index += 2)
        {
            TempMass[pr_Index++] = (((u16)Mb[mb_Index])<<8) + Mb[mb_Index+1];
        }
        // Замена little endian на big endian для вывода float
        for (t = begin; t < end; t= t+2)
        {
            temp = TempMass[t];
            TempMass[t] = TempMass[t+1];
            TempMass[t+1] = temp; 
        }
        while (t-- != 0)
            Mb[t] = 0;
        
        m_fFlag = 0; // Переписали кадр, сняли флаг получения
        
        return 1;
    }
    return 0;
}
void PinConfig ()
{
    PORTA = 0;
    LATA = 0;
    TRISA = 0xDF;
    ADCON1 = 0xF;
    
    PORTB = 0x00;
    LATB = 0x00;
    TRISB = 0x07; 
    
    PORTD = 0x00;
    LATD = 0x00;
    TRISD = 0x00;
    
    PORTC = 0;
    LATC = 0;
    TRISC = 0xC0;

    PORTF = 0x00;
    LATF = 0x00;
    TRISF = 0x07;
    
    CMCON = 0x7;
    PORTG = 0;
    LATG = 0;
    TRISG = 0xE4; // G3,g0-1 - Output
    
    PORTE = 0;
    LATE = 0x00;
    TRISE = 0x00; // G3,g0-1 - Output
    return;
}
void IntConfig()
{
    // Настраиваем прерывания для кнопок
    INTCON2bits.RBPU = 0;   // Включаем подтяжку
    INTCON2bits.INTEDG0 = 0; // Прерывание RB0 по спаду
    INTCON2bits.INTEDG1 = 0; // Прерывание RB1 по спаду
    INTCON2bits.INTEDG2 = 0; // Прерывание RB2 по спаду
    
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IF = 0;
    // Настраиваем таймер1
    PIE1bits.TMR1IE   = 1; 
    T1CONbits.T1CKPS = 0b11;  // делитель частоты 1:8
    TMR1 = 0x0000; // ??????? TMR1L ? TMR1H. 
    PIR1bits.TMR1IF   = 0; // Сбросили прерывание по таймеру1
    T1CONbits.TMR1ON  = 1;  // Включили таймер1
   /////////////////////////////
    //RC1IF = 0;
    INTCONbits.PEIE = 1;            // Включили глобальные прерывания
    INTCONbits.GIE  = 1;            // Включил прерывания переферии
    //      USART INIT   
    RCON |= (1<<7);
    IPR1 = 0;
    IPR2 = 0;
    IPR3 = 0;
    
    IPR1 |= (1<<0);
    IPR3 |= (1<<5);
    //FOR USART1
    //////////////////////////////////////////// 
    PIE1 = 0x21; // включили прерывания RCIF1 и TMR1
    
    TXSTA1 |= (1<<6); // Разрешили передачу по Усарт 2
    RCSTA1 |= (1<<6); // Разрешили прием по Усарт 2
    
    TX9D1 = 0; // Разрешли девятый бит в передаче 
    RX9D1 = 0; // Разрешли девятый бит в посылке
    
    //FOR USART2
    //////////////////////////////////////////// 
    
    PIE3 = 0x20; // включили прерывания RCIF2

    TXSTA2 |= (1<<6); // Разрешили передачу по Усарт 2
    RCSTA2 |= (1<<6); // Разрешили прием по Усарт 2
    
    TX9D2 = 1; // Разрешли девятый бит в передаче 
    RX9D2 = 1; // Разрешли девятый бит в посылке
    ////////////////////////////////////////////
    RC1IF = 0;
    RC2IF = 0;
    return;
}
void Border()
{
    u8 Border_flag = 0;

    
// Ограничения к значению в ячейке
	// 
    if (IpsesParam->Humidity < 0.0)
        Border_flag = 1;
    if  (IpsesParam->Temperature < 0.0)
        Border_flag = 2;
    if  (IpsesParam->Pressure < 0.0)
         Border_flag = 3;
    if ( DwesParam->V3c < 0.0 )
        Border_flag = 4;
    if ( DwesParam->Q3 < 0.0 )
        Border_flag = 5;
    if  (IpsesParam->Humidity >= 99.0 )
        Border_flag = 6;
    if  (IpsesParam->Temperature >= 50.0 )
        Border_flag = 7;
    if  (IpsesParam->Pressure >= 1100.0)
        Border_flag = 8;
    if ( DwesParam->V3c >= 60.0 )
        Border_flag = 9;
    if ( DwesParam->Q3 >= 360.0 )
        Border_flag = 10;
    
    switch (Border_flag)
    {
        case 0: LATD = 0; break; // lcd_print("Нет проблем с измерениями",0);
        case 1: RD1 ^= 1; break;//lcd_print("Проблема с нижней границей влажности",0);
        case 2: RD1 ^= 1; break; // lcd_print("Проблеа с нижней границей температуры",0); 
        case 3: RD1 ^= 1; break; // lcd_print("Проблема с нижней границей давления",0);
        case 4: RD4 ^= 1; break; // lcd_print("Проблема с нижней границей скорости ветра",0);
        case 5: RD4 ^= 1; break; // lcd_print("Проблема с нижней границей направления",0); 

        case 6: RD0 ^= 1; break; // lcd_print("Проблема с нижней границей влажности",0); 
        case 7: RD0 ^= 1; break; //lcd_print("Проблеа с нижней границей температуры",0);
        case 8: RD0 ^= 1; break; // lcd_print("Проблема с нижней границей давления",0);
        case 9: RD3 ^= 1; break; // lcd_print("Проблема с нижней границей скорости воздушного потока",0);
        case 10:RD3 ^= 1; break; // lcd_print("Проблема с нижней границей направления",0);
    }
}

//u8 ParamToModbus(u8 Device,u8* Mb,u8 Start)
//{
//    u16  *mPtr; u16 t;
//    switch (Device)
//    {
//        case 1: // IPSES
//            Mb[0] =  0x40;
//            mPtr = IPparam;
//        break;
//        case 2: //  DWES
//            Mb[0] =  0x44;
//            mPtr = DWparam;
//        break;
//    }
//            Mb[1] =  0x10;
//            Mb[2] =  0x00;
//            Mb[3] =  Start;
//	switch (Device)
//	{
//		case 1:
//			if ((( Start > 12 ) & (  Start < 178) || (( Start < 2) || Start > 180 )) )
//				return 0;
//		case 2:
//			if ((( Start > 4 ) & (  Start < 58) || (( Start < 2) || Start > 68)) )
//				return 0;
//	}
//	Mb[4]  = 0x00;
//	Mb[5]  = 2;
//	Mb[6]  = 4;
//	Mb[7]  = mPtr[Start]>>8;
//	Mb[8]  = mPtr[Start];
//	Mb[9]  = mPtr[Start+1]>>8;
//	Mb[10] = mPtr[Start+1];
//
//	CRCtemp = CRC16(Mb,CRCTable,11);
//	Mb[11] = CRCtemp;
//	Mb[12] = CRCtemp>>8;
//
//	for (t = 0; t<12;t++)
//	{
//		UART_Write(Mb[t]);
//		Mb[t] = 0;   
//	}
//	return 1;
//}
void decoShet(const u32 In)
{
    if (In == 0) 
        return;
    u16 Temp1 = In; u16 Temp2 = (In>>16);
    u32 Tempo1 = Temp2;
    Tempo1 = (Tempo1<<32)+Temp1;
    u8 Tempo2; u8 n = 0;
	while (Tempo1 > 0)
	{   
		Tempo1 /= 10;
		n++;
	}
    Tempo1 = Temp2;
    Tempo1 = (Tempo1<<32)+Temp1;
	while (n > 0 )
	{
        Tempo2 = Tempo1/sqr(10,n-1);
        if ( Tempo2 > 0)
        {
            Tempo1 -= Tempo2*sqr(10,n-1);
            lcd_char(0x30+Tempo2);
        }
		else
            if (n > 1 )
                lcd_char(0x30);
            else
                lcd_char(0x30 + Tempo1);
        n--;
	}
}
void Mode_menu(void) // Функция описания реакции на нажатие клавиш
{
    u8 temp1, temp2;
    if ( INT0IE & INT0IF )
    {
        if  ( RB3 & !RB4 ) // Если нажата кнопка "Плюс"
        {
            switch (Menu_Level)
            {
                case 0:
                    if (Changeflag != 2)
                    {
                        Changeflag++;
                    }
                    else
                    {
                        Changeflag = 0;
                        lcd_goto(1,22);Clearing(18);
                    }
                    //fChange_menu = 1;
                    break;
                case 1:
                    break;
                case 2:
                    Menu_Level = 3;
                    fChange_menu = 1;
                break;
                case 3:
                    Menu_Level = 4;
                    fChange_menu = 1;
                break;
                case 4:
                    Menu_Level = 2;
                    fChange_menu = 1;
                break;
                case 11:
                    Menu_Level = 13;
                    fChange_menu = 1;
                break;
                case 12:
                    Menu_Level = 14;
                    fChange_menu = 1;
                break;
                case 13:
                    Menu_Level = 15;
                    fChange_menu = 1;
                    break;
                case 14:
                    Menu_Level = 16;
                    fChange_menu = 1;
                    break;
                case 15:
                    if (!Selectflag)
                    {
                        Menu_Level = 11;
                        fChange_menu = 1;
                    }
                    else
                        if (Selectflag == 2)
                        {
                            switch (Column)
                            {
                                case 0:
                                    temp1 = ++fHum_acc;
                                break;
                                case 1:
                                    if (!Stan)
                                    {
                                        if (fTemp_acc < 10 )
                                            temp1 = ++fTemp_acc-2;
                                    }
                                    else
                                    {
                                        if (fPress_acc < 10 )
                                            temp1 = ++fPress_acc-4;
                                    }
                                break;
                            }
                            if (temp1 <=6)
                            {
                                decoShet(temp1);
                                lcd_left(1);
                            }
                        }
                break;
                case 16:
                    if (!Selectflag)
                    {
                        Menu_Level = 12;
                        fChange_menu = 1;
                    }
                    else
                        if (Selectflag == 2)
                        {
                            switch (Column)
                            {
                                case 0:
                                    if (fAngle_acc < 10 )
                                        temp1 = ++fAngle_acc-3;
                                break;
                                case 1:
                                    if (fSpeed_acc < 10 )
                                        temp1 = ++fSpeed_acc-2;
                                break;
                            }
                            if (temp1 <=6)
                            {
                                decoShet(temp1);
                                lcd_left(1);
                            }
                        }
                break;
            }
        }
        else 
            if ( !RB3 & RB4 ) // Если нажата кнопка "Ввод"
            {
                switch (Menu_Level)
                {
                    case 0: // Меню общей информации по двум приборам
                        Menu_Level = 10;
                        fChange_menu = 1;
                    break;
                    case 1: // Меню информации ИПСЭС
                        Menu_Level = 11;
                        fChange_menu = 1;
                    break;
                    case 2: // Меню информации ДВЭС
                        Menu_Level = 12;
                        fChange_menu = 1;
                    break;
//                    case 3: 
//                        Menu_Level = 13;
//                        fChange_menu = 1;
//                    break;
//                    case 4:
//                        Menu_Level = 14;
//                        fChange_menu = 1;
//                    break;
//                    case 10: // Если находимся в меню информации главного экрана и нажимаем на ввод
//                        Menu_Level = 10;
//                    break;
//                    case 11: // Если находимся в меню информации ИПСЭС и нажимаем на ввод
//                        Menu_Level = 11;
//                    break;
//                    case 12: // Если находимся в меню информации Д и нажимаем на ввод
//                        Menu_Level = 12;
//                    break;
                    case 11:
                    case 12:
                    break;
                    case 15:
                    case 16:
                        if (Selectflag == 1)
                        {
                            lcd_cmd(0x0D);ms_Delay(1); lcd_goto(Column,Stan); Selectflag = 2; lcd_right(13); 
                        }
                        else
                        {
                            if (Selectflag == 0)
                            {
                                Selectflag = 1; Column = 1; fChange_menu = 1;
                                lcd_goto(1,0); 
                            }
                            else
                            {
                               lcd_cmd(0x0C); ms_Delay(1);lcd_goto(Column,0);  Selectflag = 0;
                            }
                        }
                        ms_Delay(50);
                        break;
                }
            }
        INT0IF = 0;
    }
    if ( INT1IE & INT1IF )
    {
        if ( RB3 & !RB4 ) // Если нажата кнопка "Выход"
        {
            switch (Menu_Level)
            {
                case 15:
                case 16: 
                    if (Selectflag == 2)
                    {
                        lcd_cmd(0x0E);
                        Selectflag = 1;
                    }
                    else
                    {
                        if (Selectflag == 1)
                        {
                            fChange_menu = 1;
                            Selectflag = 0;
                        }
                        else 
                        {
                            fChange_menu = 1;
                            Menu_Level = (Menu_Level == 15) ? 1:2;
                        }
                    }
                    break;
                default:
                    fChange_menu = 1;
                    Menu_Level = ChkFlag;
                    break;
            }
        }
        else 
            if ( !RB3 & RB4 ) // Если нажата кнопка "Вниз"
            {
                switch (Menu_Level)
                {
                    case 0:
                         fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 1; // Отображение параметров ИПСЭС
                    break;
                    case 1:
                         fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 2; // Отображение параметров ДВЭС
                    break;
                    case 2:
                    case 3:
                    case 4:
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 0; // Отображение смешаных параметров
                    break;
                    case 11:
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 12; // Отображение параметров ДВЭС
                        break;
                    case 12:
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 10; // Отображение параметров ДВЭС
                        break;
                    case 15:
                    case 16:
                        if (Selectflag == 1)
                        {
                            fChange_menu = 1;
                            switch (Column)
                            {
                                case 0:
                                    Column = 1;
                                    if (Menu_Level == 15)
                                        Stan = 20;
                                    else
                                        Stan = 0;
                                break;
                                case 1:
                                    if  ((Menu_Level == 15) & (Stan == 20))
                                    {
                                        Stan = 0;
                                    }
                                    else
                                    {
                                        Column = 0; Stan = 20;
                                    }
                                break;
                            }
                            lcd_goto(Column,Stan);
                        }
                        break;
                }
            }
        INT1IF = 0;
    }
    if ( INT2IE & INT2IF ) 
    {
        if ( RB3 & !RB4) // Если нажата кнопка "Минус"
        {
            switch (Menu_Level)
            {
                case 2:
                    Menu_Level = 4; // Отображение усреднения за 600 сек
                    fChange_menu = 1;
                break;
                case 3:
                    Menu_Level = 2; // Отображение ДВЕС
                    fChange_menu = 1;
                break;
                case 4:
                    Menu_Level = 3; // Отображение усреднения за 120 сек
                    fChange_menu = 1;
                break;
                case 11:
                    Menu_Level = 15; // Отображение усреднения за 120 сек
                    fChange_menu = 1;
                    break;
                case 12:
                    Menu_Level = 16; // Отображение усреднения за 120 сек
                    fChange_menu = 1;
                    break;
                case 13:
                    Menu_Level = 11; // Отображение усреднения за 120 сек
                    fChange_menu = 1;
                    break;
                case 14:
                    Menu_Level = 12; // Отображение усреднения за 120 сек
                    fChange_menu = 1;
                    break;
                case 15:
                    if (!Selectflag)
                    {
                        Menu_Level = 11;
                        fChange_menu = 1;
                    }
                    else
                        if (Selectflag == 2)
                        {
                            switch (Column)
                            {
                                case 0:
                                    temp1 = --fHum_acc;
                                break;
                                case 1:
                                    if (!Stan)
                                    {
                                        if (fTemp_acc > 0 )
                                            temp1 = --fTemp_acc-2;
                                    }
                                    else
                                    {
                                        if (fPress_acc > 0 )
                                            temp1 = --fPress_acc-4;
                                    }
                                break;
                            }
                            if  (temp1 <=6)
                            {
                                decoShet(temp1);
                                lcd_left(1);
                            }
                        }
                break;
                case 16:
                    if (!Selectflag)
                    {
                        Menu_Level = 12;
                        fChange_menu = 1;
                    }
                    else
                        if (Selectflag == 2)
                        {
                            switch (Column)
                            {
                                case 0:
                                    if ( fAngle_acc > 0)
                                        temp1 = --fAngle_acc-3;
                                break;
                                case 1:
                                    if ( fSpeed_acc > 0)
                                        temp1 = --fSpeed_acc-2;
                                break;
                            }
                            if (temp1 <=6)
                            {
                                decoShet(temp1);
                                lcd_left(1);
                            }
                        }
                break;
            }
        }
        else 
            if ( !RB3 & RB4 ) // Если нажата кнопка "Вверх"
            {
                switch (Menu_Level)
                {
                    case 0:
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 2; // Отображение параметров ДВЭС
                    break;
                    case 1:
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 0; // Отображение Смешаных параметров
                    break;
                    case 2:
                    case 3:
                    case 4:
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 1; // Отображение усреднения за 120 сек
                    break;
                    case 11: 
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 10; // Отображение Смешаных параметров
                    case 12: 
                        fChange_menu = 1;
                        if (ChkFlag == 0)
                            Menu_Level = 11; // Отображение Смешаных параметров
                    break;
                    case 15:
                    case 16: 
                        if (Selectflag == 1)
                        { 
                            
                            fChange_menu = 1;
                            switch (Column)
                            {
                                case 0:
                                {
                                    Column = 1;
                                    Stan = 0;
                                }
                                break;
                                case 1:
                                    if  ((Menu_Level == 15) & (Stan == 0))
                                    {
                                        Stan = 20;
                                    }
                                    else
                                    {
                                        Column = 0; Stan = 20;
                                    }
                                break;
                            }
                            lcd_goto(Column,Stan);
                        }
                        break;
                }
            }
        INT2IF = 0;
    }
        RB3 = RB4;
        RB4 = !RB3;
        ms_Delay(2);
}
void Watch_menu(void) // Функция описания реакции на нажатие клавиш
{
    u8 temp1; 
    if (fChange_menu)
    {
        lcd_clear();
        lcd_cmd(0x0C);
        ms_Delay(10);
    }
    switch ( Menu_Level) // Функция вывода на экран параметров, согласно выбраному меню
    {
        case 0:
            // Подготовка float-массивов
            switch (Changeflag)
            {
                case 0:
                    sprintf(SpeedMas, "%f", DwesParam->V3c);break;
                case 1:
                    sprintf(SpeedMas, "%f", DwesParam->V120);break;
                case 2:
                    sprintf(SpeedMas, "%f", DwesParam->V600);break;
            }
            
            switch (Changeflag)
            {
                case 0:
                    sprintf(AngleMas, "%f", DwesParam->Q3);break;
                case 1:
                    sprintf(AngleMas, "%f", DwesParam->Q120);break;
                case 2:
                    sprintf(AngleMas, "%f", DwesParam->Q600);break;
            }
            
            sprintf(HumMas, "%f", IpsesParam->Humidity);
            sprintf(TempeMas, "%f", IpsesParam->Temperature);
            sprintf(PresMas, "%f", IpsesParam->Pressure);
            // Вывод параметров на экран
            lcd_goto(0,1);
            if (TempeMas[0] == '-') 
            {
                lcd_print(TempeMas,5); 
            }
            else 
            {
                lcd_char('+');
                lcd_print(TempeMas,4);
            }
            lcd_char(0xDF); lcd_char('C');
            lcd_goto(0,12);
            lcd_print(HumMas,4); lcd_char('%'); lcd_char(0x20);lcd_char('R');lcd_char('H');
            lcd_goto(1,5);
            lcd_print(PresMas,6); lcd_char(0xB4); lcd_char(0xA8); lcd_char('a'); // гПа
            lcd_goto(0,21);
            lcd_print(SpeedMas,4); lcd_char(0xBC); lcd_char(0x2F); lcd_char(0x63);// м/c
//            lcd_goto(1,31);
//            lcd_print(AngleMas,5); lcd_char(0xB4); lcd_char(0x70); lcd_char(0xE3); // грд
            if (TempeMas[3] == '.')
            {
                lcd_goto(0,31);
                lcd_print(AngleMas,5); lcd_char(0xB4); lcd_char(0x70); lcd_char(0xE3); // грд
            }
            else 
            {
                lcd_goto(0,32);
                lcd_print(AngleMas,5); lcd_char(0xB4); lcd_char(0x70); lcd_char(0xE3); // грд
            }
            switch (Changeflag)
            {
                case 0: lcd_goto(1,22);Clearing(18);
                    break;
                case 1:
                    sprintf(MaxV, "%f", DwesParam->Vmax120); lcd_goto(1,22);Clearing(18);lcd_goto(1,22); lcd_print("Vmax120",0); Par_screen_SPEED(MaxV); break;
                case 2:
                    sprintf(MaxV, "%f", DwesParam->Vmax600); lcd_goto(1,22);Clearing(18);lcd_goto(1,22); lcd_print("Vmax600",0); Par_screen_SPEED(MaxV); break;
            }
//            if (Changeflag)
//            {
//                lcd_goto(1,5); lcd_print(SpeedMas,4); lcd_char(0xBC); lcd_char(0x2F); lcd_char(0x63);// м/c
//            }
        break;
        case 1:
            // Подготовка float-массивов
            sprintf(HumMas, "%f", IpsesParam->Humidity);
            sprintf(TempeMas, "%f", IpsesParam->Temperature);
            sprintf(PresMas, "%f", IpsesParam->Pressure);
            lcd_goto(0,0);
            lcd_char('T'); screen_Raven();  // T = 
            if (TempeMas[0] == '-') 
            {
                //lcd_goto(1,1);
                lcd_print(TempeMas,fTemp_acc+1); 
            }
            else 
            {
                //lcd_goto(1,2);
                lcd_print(TempeMas,fTemp_acc);
            }
            lcd_char(0xEF); lcd_char('C');
            lcd_goto(1,0);
            lcd_char('H'); screen_Raven();  // H = 

            lcd_print(HumMas,fHum_acc); lcd_char('%');
            lcd_goto(0,20);
            lcd_char('P'); screen_Raven();   // P =
            lcd_print(PresMas,fPress_acc); lcd_char(0xB4); lcd_char(0xA8); lcd_char('a'); // гПа
        
        break;
        case 2:
            // Подготовка float-массивов
            sprintf(SpeedMas, "%f", DwesParam->V3c);
            sprintf(AngleMas, "%f", DwesParam->Q3);
            // Вывод параметров ДВЭС на экран
            lcd_goto(0,0);
            lcd_char('V'); screen_Raven();   // P =
            lcd_print(SpeedMas,fSpeed_acc); lcd_char(0xBC); lcd_char(0x2F); lcd_char(0x63);// м/c
            lcd_goto(1,0);
            lcd_char('Q'); screen_Raven();   // P =
            if (AngleMas[3] == '.')
            {
                lcd_print(AngleMas,fAngle_acc); lcd_char(0xB4); lcd_char(0x70); lcd_char(0xE3); // грд
            }
            else 
            {
                lcd_print(AngleMas,fAngle_acc-1); lcd_char(0xB4); lcd_char(0x70); lcd_char(0xE3); // грд
            }
        break;
        case 3:
            // Подготовка float-массивов
            sprintf(AvgV, "%f", DwesParam->V120);
            sprintf(AvgQ, "%f", DwesParam->Q120);
            sprintf(MaxV, "%f", DwesParam->Vmax120);
            // Вывод параметров ДВЭС на экран
            lcd_goto(0,00);
            lcd_print("V120",0); Par_screen_SPEED(AvgV);
            lcd_goto(1,0);
            lcd_print("Q120",0);
            Par_screen_ANGLE(AvgQ);
            lcd_goto(0,20);   
            lcd_print("Vmax120",0); Par_screen_SPEED(MaxV);
        break;
        case 4:
            // Подготовка float-массивов
            sprintf(AvgV, "%f", DwesParam->V600);
            sprintf(AvgQ, "%f", DwesParam->Q600);
            sprintf(MaxV, "%f", DwesParam->Vmax600);
            // Вывод параметров ДВЭС на экран
            lcd_goto(0,00);
            lcd_print("V600",0); Par_screen_SPEED(AvgV);
            lcd_goto(1,0);
            lcd_print("Q600",0); Par_screen_ANGLE(AvgQ);
            lcd_goto(0,20);
            lcd_print("Vmax600",0); Par_screen_SPEED(MaxV);
        break;
        ////////////////////////////////////////////////////////
        // Дальше идут менюшки, которые не надо обновлять каждый раз.
        case 10: // Здесь должно быть действие, выполняемое постоянно в 10 моде
            if (fChange_menu)
            {
                lcd_goto(0,0);
                Clearing(20);
                lcd_goto(0,1);
                //lcd_char(0xA5);
                lcd_goto(1,0);
                Clearing(20); 
                lcd_goto(0,20);
                Clearing(20);

                lcd_goto(1,2);
                lcd_char(0xA5);lcd_char(0xA8); lcd_char(0x43); lcd_char(0xAF); lcd_char(0x43);lcd_goto(0,28); // ИПСЭС

                lcd_goto(1,12);
                
                decoShet(IpsesParam->Serial);

                lcd_goto(0,22); 
                lcd_char(0xE0); lcd_char(0x42); lcd_char(0xAF);lcd_char(0x43);// ДВЭС
                lcd_goto(0,1);
                lcd_char(0xa8);lcd_char(0x70);lcd_char(0xb8);lcd_char(0xb2);lcd_char(0x6f);lcd_char(0x70);
                lcd_goto(0,9);
                
                lcd_char(0xa4); lcd_char(0x61); lcd_char(0xb3); lcd_char(0x2e);
                lcd_char(0xbd); lcd_char(0x6f); lcd_char(0xbc); lcd_char(0x65); lcd_char(0x70);
                lcd_goto(0,32); 
                decoShet(DwesParam->Serial);
            }
        break;
        // Отображение основных параметров устройств
        case 11: temp1 = 1;
        case 12: if ( Menu_Level == 12) temp1 = 2;
            if (fChange_menu)
            {
                lcd_goto(0,0);
                Clearing(80);
                lcd_goto(0,0);
                ////////
                ///////
                lcd_char(0x48);lcd_char(0x61);lcd_char(0xB8);
                lcd_char(0xBC);lcd_char(0x65);lcd_char(0xBD);
                lcd_char(0x6f);lcd_char(0xB3);lcd_char(0x61);
                lcd_char(0xBD);lcd_char(0xB8);lcd_char(0x65);lcd_char(0x3a); // Наименование
                switch (temp1)
                {
                    case 1:
                        lcd_char(0xA5);lcd_char(0xA8);lcd_char(0x43);
                        lcd_char(0xAF);lcd_char(0x43);lcd_char(0x2d);lcd_char(0x31);// ИПСЭС
                    break;
                    case 2:
                        lcd_char(0xE0); lcd_char(0x42);lcd_char(0xAF);
                        lcd_char(0x43);lcd_char(0x2d);lcd_char(0x32);// Двэс
                    break;   
                }
                lcd_goto(1,0);
                lcd_char(0x43);lcd_char(0x65);lcd_char(0x70);lcd_char(0x2e); // Сер.
                lcd_char(0xBD);lcd_char(0x6f);lcd_char(0xBC);
                lcd_char(0x65);lcd_char(0x70); lcd_char(0x3a);// Номер:
                lcd_char(0x20);
                switch (temp1)
                {
                    
                    case 1:
                        decoShet(IpsesParam->Serial);
                    break;
                    case 2:
                        decoShet(DwesParam->Serial);
                    break;
                }
                lcd_goto(0,20);
                lcd_char(0x50);lcd_char(0x65);lcd_char(0xB6);
                lcd_char(0xB8);lcd_char(0xBC); // Режим
                lcd_char(0x20);
                lcd_char(0xBE);lcd_char(0x70);lcd_char(0xB8);
                lcd_char(0xB5);lcd_char(0xBC);lcd_char(0x61);lcd_char(0x3A); // Передачи
                lcd_char(0x20);
                switch (temp1)
                {
                    case 1:
                        if (IpsesParam->Mod_Working)
                            lcd_print("Modbus",0);
                    break;
                    case 2:
                        if (DwesParam->Mod_Working)
                    lcd_print("Modbus",0);
                    break;
                }
            }
            break;
//          
        // Отображение параметров связи с устройствами
        case 13: temp1 = 1;
        case 14: if ( Menu_Level == 14) temp1 = 2;
            if (fChange_menu)
            {
                lcd_goto(0,3);
                    //fChange_menu = 1;
                //////////////////////
                lcd_char(0xA8);lcd_char(0x61);lcd_char(0x70);
                lcd_char(0x61);lcd_char(0xBC);lcd_char(0x65);
                lcd_char(0xBF);lcd_char(0x70);lcd_char(0xC3);
                lcd_char(0x20);
                lcd_char(0x63);lcd_char(0xB3);lcd_char(0xC7);
                lcd_char(0xB7);lcd_char(0xB8); // Параметры связи
                lcd_right(1);

                lcd_goto(1,0);
                //                  
                lcd_char(0x43);lcd_char(0x65);lcd_char(0xBF);
                lcd_char(0x65);lcd_char(0xB3);lcd_char(0x6f);lcd_char(0xB9);
                lcd_char(0x20);
                lcd_char(0x61);lcd_char(0xE3);lcd_char(0x70);
                lcd_char(0x65);lcd_char(0x63);lcd_char(0x3A);// Сетевой адрес: 
                lcd_right(1);
                switch (temp1)
                {
                    case 1:
                        decoShet(IpsesParam->Net_Addr);
                    break;
                    case 2:
                        decoShet(DwesParam->Net_Addr);
                    break;
                }
                lcd_goto(0,20);
                lcd_char(0x43);lcd_char(0xBA);lcd_char(0x6f);
                lcd_char(0x70);lcd_char(0x6f);lcd_char(0x63);
                lcd_char(0xBF);lcd_char(0xC4);lcd_char(0x3A); // Скорость:
                lcd_right(1);
                switch (temp1)
                {
                    case 1:
                        decoShet(IpsesParam->Speed_rate);
                    break;
                    case 2:
                        decoShet(DwesParam->Speed_rate);
                    break;
                }
                lcd_char(0x20);lcd_char(0xB2);lcd_char(0x2F);lcd_char(0x63);
                lcd_goto(1,20);
                lcd_char(0xAB);lcd_char(0xA2);lcd_char(0xBF);
                lcd_char(0xBD);lcd_char(0x6f);lcd_char(0x63);
                lcd_char(0xBF);lcd_char(0xC4);lcd_char(0x3A); //Четность:
                lcd_right(1);
                switch (temp1)
                {
                    case 1:
                        if (IpsesParam->Parity)
                        {
                            lcd_char(0xE0);lcd_char(0x61);
                        }
                        else
                        {
                            lcd_char(0x48);lcd_char(0x65);lcd_char(0xBF);
                        }
                    break;
                    case 2:
                        if (DwesParam->Parity)
                        {
                            lcd_char(0xE0);lcd_char(0x61);
                        }
                        else
                        {
                            lcd_char(0x48);lcd_char(0x65);lcd_char(0xBF);
                        }
                    break;
                }
                //////////////////////////////////////
            }
        break;
        //Меню настройки Выберите точность отображения параметров
        case 15: temp1 = 1;
        case 16: 
            if ( Menu_Level == 16) 
                temp1 = 2;
            if (fChange_menu)
            { 
                lcd_right(1);
                lcd_char(0x48);lcd_char(0x61);lcd_char(0x63);lcd_char(0xbf);lcd_char(0x70);lcd_char(0x6f);lcd_char(0xb9);lcd_char(0xba);lcd_char(0xb8);
                lcd_right(1);
                lcd_char(0xbf);lcd_char(0x6f);lcd_char(0xc0);lcd_char(0xbd);lcd_char(0x6f);lcd_char(0x63);lcd_char(0xbf);lcd_char(0xb8);
                if (temp1 == 1)
                {
                lcd_goto(1,1); //
                if ((Selectflag == 1) & (Column == 1) & (Stan == 0))
                {
                    lcd_left(1);
                    lcd_char(0x3e);
                    
                }
                lcd_char(0x54);lcd_char(0x65 );lcd_char(0xbc );lcd_char(0xbe );lcd_char(0x70 );lcd_char(0x61 );lcd_char(0xbf );lcd_char(0x79 );lcd_char(0x70 );lcd_char(0x61);lcd_char(0x3a);
                lcd_goto(1,13);
                decoShet(fTemp_acc-2);//lcd_print("XX.X",0);
                lcd_goto(0,21); //
                if ((Selectflag == 1) & (Column == 0) & (Stan == 20))
                {
                    lcd_left(1);
                    lcd_char(0x3e);
                    
                }
                lcd_char(0x42);lcd_char(0xbb);lcd_char(0x61);lcd_char(0xb6);lcd_char(0xbd);lcd_char(0x6f);lcd_char(0x63);lcd_char(0xbf);lcd_char(0xc4);lcd_char(0x3a);
                lcd_goto(0,33);
                decoShet(fHum_acc-2);
                lcd_goto(1,21); //
                if ((Selectflag == 1) & (Column == 1) & (Stan == 20))
                {
                    lcd_left(1);
                    lcd_char(0x3e);
                    
                }
                lcd_char(0xe0);lcd_char(0x61);lcd_char(0xb3);lcd_char(0xbb);lcd_char(0x65);lcd_char(0xbd);lcd_char(0xb8);lcd_char(0x65);lcd_char(0x3a);
                lcd_goto(1,33);
                decoShet(fPress_acc-4);
                }
                else
                {
                    if (temp1 == 2)
                    {
                        lcd_goto(1,1); //
                        if ((Selectflag == 1) & (Column == 1) & (Stan == 0))
                        {
                            lcd_left(1);
                            lcd_char(0x3e);
                            
                        }
                        lcd_char(0x43);lcd_char(0xba );lcd_char(0x6f );lcd_char(0x70 );lcd_char(0x6f );lcd_char(0x63 );lcd_char(0xbf );lcd_char(0xc4 );lcd_char(0x3a);
                        lcd_goto(1,13);
                        decoShet(fSpeed_acc-2);
                        lcd_goto(0,21); //
                        if ((Selectflag == 1) & (Column == 0) & (Stan == 20))
                        {
                            lcd_left(1);
                            lcd_char(0x3e);
                            
                        }
                        lcd_char(0x48);lcd_char(0x61);lcd_char(0xbe);lcd_char(0x70);lcd_char(0x61);lcd_char(0xb3);lcd_char(0xbb);lcd_char(0x65);lcd_char(0xbd);lcd_char(0xb8);lcd_char(0x65);lcd_char(0x3a);
                        lcd_goto(0,33);
                        decoShet(fAngle_acc-3);
                    }
                }
            }
            break;
    }
   
    fChange_menu = (fChange_menu == 1) ? 0:0;
    return;
}
void ErrConnect(void)
{
    u8 temp1;
    switch (Deflag)
    {
        case 0:
            break;
        case 1:
            RD2 = 1; //RB5 = 1; 
            Deflag = 2;
            temp1 = DWout;
            break;
        case 2:
            RD5 = 1; //RB5 = 1; 
            Deflag = 1;
            temp1 = IPout;
            break;
    }
    for ( i = 0; i< 50; i++)
    {
        //RB5 = 1;
        ms_Delay(50);
        //RB5 = 0; 
        temp1++;
        if ( temp1 == 0xf0 )
        {
            temp1 = 0;
            if (ChkFlag == 0)
            {
                ChkFlag = 3 - Deflag;
                continue;
            }
            else
                ChkFlag = Find_Devises(0xF);
        }
    }
    LATD = 0;
    return;
}
u8 ParamToModbus(u8 Device,u8* Mb,u32 x,u8 Step)
{
    u8 t; u8 Start = Step;  //u16  *mPtr;
    switch (Device)
    {
        case 1: // IPSES
            Mb[0] =  0x40;
            //mPtr = IPparam;
            break;
        case 2: //  DWES
            Mb[0] =  0x44;
            //mPtr = DWparam;
            break;
    }
	Mb[1] =  0x10;
	Mb[2] =  0x00;
	Mb[3] =  Start;

	Mb[4] =  0x00;
	Mb[5] =  2;
	Mb[6] =  4;
//	Mb[7] = x;
//	Mb[8] = x>>8;
//	Mb[9] = x>>16;
//	Mb[10] = x>>24;
    // Обратный порнядок передачи
        Mb[10] = x;
    	Mb[9] = x>>8;
    	Mb[8] = x>>16;
    	Mb[7] = x>>24;

	CRCtemp = CRC16(Mb,CRCTable,11);
	Mb[11] = CRCtemp;
	Mb[12] = CRCtemp>>8;

	for (t = 0; t<13;t++)
	{
		UART_Write(Mb[t]);
		Mb[t] = 0;   
	}
	return 1;
}
void PC_Request()
{
}
