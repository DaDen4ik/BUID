
#ifndef STRUCT_H
#define	STRUCT_H
#include "config_bits.h"
//#include <xc.h> // include processor files - each processor file is guarded.  

typedef struct Dwes_st{ // Temperature
		u32 reserved_1;
		
		u32 Mod_Working;
		u32 Serial;
		
		u32 reserved_2;
		
		u32 Wind_Correct;
		
		u32 reserved[10];
		
		u32 Time_Working;
        
        float V;
		float V3;
		float V3c;
		float V120;
		float V600;
		float Vmax120;
		float Vmax600;
		float Vmax1h;
		float Qb;
		float Q3;
		float Q;
		float Q120;
		float Q600;
        
		u32 Magnet_1;
		u32 Magnet_2;
		u32 Net_Addr;
		u32 Reset_mod;
		u32 Speed_rate;
		u32 Parity;
}Dwes_param; 

typedef struct Ipses_st{ // Temperature
		u32 reserved_1; // Хз 0
		u32 Mod_Working; // Режим работы. 0 - ASCII, 1 - Modbus 2
		u32 Serial; // Серийный Номер устройства 4
		u32 Time_Working; //Время наработки 6
        
		float Humidity; // Итоговое Показание влажности 8
		float Temperature; // Итоговое показание температуры 10
		float Pressure; // Итоговое показание давления 12
        
        float HumCorrect[25]; // 14-62 регистр
        float PressCorrect[25];// 64 - 112 регистр
        float TempRangeCorrect[5]; // 114 - 122 регистр
        float TempCorrect[5]; // 124 - 132 регистр
        
        float HumTempRangeCorrect[5]; //134-142 регистр
        float PressTempRangeCorrect[5]; // 144-152 регистр
        float HumHumRangeCorrect[5]; //154 - 162 регистр
        float PressPressRangeCorrect[5];  // 164 - 172 регистр
        float reserved2[2]; // Noname
        float ADCRangeCorrect[5]; // 178 - 186 регистр
        float PressADCCorrect[5]; // 188 - 196 регистр
        //u32 Noname;
        u32 AdminAdmin; // 198
        u32 ADCtemp; // 200
		u32 Net_Addr; // 202
		u32 Reset_mod; // 204
		u32 Speed_rate; // 206
		u32 Parity; // 208
        u32 Verson; // 210
}Ipses_param;
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

