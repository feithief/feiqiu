#ifndef _SYSTEMTYPE_H_
#define _SYSTEMTYPE_H_

#include "el_types.h"
#include <string.h>
#include <stdbool.h>

/**@brief This macro defines PWM period counter.*/

#define btrue               1/**<boolean type true.*/
#define bfalse              0/**<boolean type false.*/

typedef unsigned short int  tick_t;
#define DMAX_SYSTEM_TICK  (0xFFFF)/**<system tick type max value.*/
#define DTickAfter(a, b) ((signed short)(b)-(signed short)(a)<=0)
#define SystemEventSlot         10    //10ms

/* 要改成共用体，目前方案12 bytes
* 16位单片机 65535 * 60mA / 20mA = 786420 = 0xBFFF4
* 取21bit。 RGB共63bit，1bit reserved。 共64bit 8字节*/

typedef union UPWM32
{
	uint64_t num64;
	struct SR
	{
		int64_t Value : 21;
		int64_t reserved2 : 43;
	}R;
	struct SG
	{
		int64_t reserved1 : 21;
		int64_t Value : 21;
		int64_t reserved2 : 22;
	}G;
	struct SB
	{
		int64_t reserved1 : 42;
		int64_t Value : 21;
		uint64_t reserved2 : 1;
	}B;
}PWMs32_t;

typedef struct SPWM16
{
	uint16_t R;
	uint16_t G;
	uint16_t B;
}PWM16_t;
#define ROM_test
#ifndef ROM_test
typedef struct SLinSignals
{
	uint16_t BCM_RGB_Slave_Adresse : 15; /**<group address and single address.*/
	uint16_t BCM_RGB_Adressierungsart : 1; /**<Whether group address is using.*/
    
	uint16_t BCM_RGB_Rot : 8; /**<R Signal.*/
	uint16_t BCM_RGB_Gruen : 8; /**<G Signal.*/
	uint16_t BCM_RGB_Blau : 8; /**<B Signal.*/
	//uint8_t  BCM_RGB_Intensitaet : 7;   /** 注意这里和实际不一样。为了运算的精度，把intensity扩了1位*/
	uint16_t BCM_RGB_Intensitaet : 8; /** 注意这里和实际不一样。为了运算的精度，把intensity扩了1位。下一位移到了最后*/
    
	uint16_t BCM_RGB_Dimmrampe : 4; /**<Dimming ramp.*/
	uint16_t BCM_RGB_Normierung_Intensitaet : 1; /**<Reserved. 注意这里和实际不一样。为了运算的精度，把intensity扩了1位*/
	uint16_t BCM_RGB_Sonderfunktion : 3; /**<Mode flag, normal mode or special functions.*/
    
	uint16_t BCM_RGB_Bewertung_Dimmzeit : 1; /**<Dimming attributes, relative or absolute.*/
	uint16_t BCM_RGB_Gueltigkeit : 1; /**<Flag that indicates if LIN signal is valid.*/
	uint16_t BCM_RGB_Dimmzeit_Zeitbasis : 1; /**<Flag that indicates the base dimming period.*/
	uint16_t unused2 : 5; /**<Reserved.*/
}LinSignals_t;


typedef union USysState
{
	uint16_t num;
	struct SLIN
	{
		uint16_t NewCmd : 1;		/* 任何一个信号改变都会置位 */
		uint16_t TimeBaseUpdate : 1;	/* rampe、basis，speed改变会触发 */
		uint16_t IntensityUpdate : 1;	/* 亮度变化会触发 */
		uint16_t ColorUpdate : 1;
		uint16_t reserved2 : 12;
	}LIN;  //bit 0~3
	struct SFlashUpdate
	{
		uint16_t reserved1 : 4;
		uint16_t Locked : 1;
		uint16_t UpdateNAD : 1;
        uint16_t FlashUpdate : 1;
		uint16_t reserved2 : 9;
	}Flash;  //bit 4~5
	struct SError
	{
		uint16_t reserved1 : 6;
		uint16_t Flag : 3;		/* bit0:Battery, bit1:LEDs, bit2:Temperature, 或许可以加计算溢出这种 */
		uint16_t reserved2 : 7;
	}Error;  //bit 6~8
	struct SCali
	{
		uint16_t reserved1 : 9;
		uint16_t Color : 3;		/* 0:OFF, 1:R, 2:G, 3:B 4:Calculate 3 CaliL*/
		uint16_t reserved2 : 4;
	}Cali;   //bit 9~10
	struct SSleep
	{
		uint16_t reserved1 : 12;
		uint16_t Flag : 2;
		uint16_t Wakeup_Flag : 2;
	}Sleep;  //bit 11~12
}SysState_t;

#else
typedef struct SLinSignals
{
	uint16_t BCM_ALU_NodeID; /**<group address and single address.*/
	uint8_t  BCM_ALU_NodeID_Mode; /**<Whether group address is using.*/
	uint8_t  BCM_ALU_Intensity; /** 注意这里和实际不一样。为了运算的精度，把intensity扩了1位。下一位移到了最后*/
	uint8_t  BCM_ALU_FadingTimeCmd; /**<Dimming ramp.*/
	uint8_t  BCM_ALU_ModeSet; /**<Mode flag, normal mode or special functions.*/
	uint8_t  BCM_ALU_AtmosphereLightBrightnessSet; /**<亮度等级*/
	uint8_t  BCM_ALU_IntensityMode;
	uint8_t  BCM_ALU_FadingCmd; /**<*/
	uint8_t  BCM_ALU_ColorCmd_Red; /**<R Signal.*/
	uint8_t  BCM_ALU_ColorCmd_Green; /**<G Signal.*/
	uint8_t  BCM_ALU_ColorCmd_Blue; /**<B Signal.*/
}LinSignals_t;

typedef struct USysState
{
	struct SLIN
	{
		uint8_t NewCmd : 1;		/* 任何一个信号改变都会置位 */
		uint8_t TimeBaseUpdate : 1;	/* rampe、basis，speed改变会触发 */
		uint8_t IntensityUpdate : 1;	/* 亮度变化会触发 */
		uint8_t ColorUpdate : 1;
		uint8_t reserved2 : 4;
	}LIN;  //bit 0~3
	struct SFlashUpdate
	{
		uint8_t Locked : 1;
		uint8_t UpdateNAD : 1;
		uint8_t FlashUpdate : 1;
        uint8_t NewLedInfo :1;
		uint8_t reserved2 : 4;
	}Flash;  //bit 4~5
	struct SError
	{
		uint8_t Flag;		/* bit0:Battery, bit1:LEDs, bit2:Temperature, 或许可以加计算溢出这种 */
	}Error;  //bit 6~8
	struct SCali
	{
		uint8_t Color;		/* 0:OFF, 1:R, 2:G, 3:B 4:Calculate 3 CaliL*/
	}Cali;   //bit 9~10
	struct SSleep
	{
		uint8_t Flag;
		uint8_t Wakeup_Flag;
	}Sleep;  //bit 11~12
}SysState_t;
#endif

typedef struct SCpstCtrl
{
	uint8_t TMCUTriggerCNT;
	uint8_t Step : 5;
	uint8_t Cause : 3;
	int16_t DeltaPWMR;
	int16_t DeltaPWMG;
	int16_t DeltaPWMB;
	uint16_t DimCNT;
}Cpst_t;


typedef struct SGlobal
{
	SysState_t* sysState;
	LinSignals_t* Lin;
	PWMs32_t* PWM32;
	PWM16_t* PWM16;
	Cpst_t*		CpstCtrl;
}Global_t;


Global_t* Main_GetSysVar(void);

/* 库里貌似有offsetof的宏定义，但是我不清楚单片机编译器是不是有，保险点自己写个范围小一点的 */
#ifndef _offsetof_
#define _offsetof_(TYPE, MEMBER) (&((TYPE*)0)->MEMBER)
#endif


#ifndef _Max_
#define _Max2_(a, b) (((a)>=(b))?(a):(b))
#endif


#define PWMFULL		32000U
#define CaliPWM		((uint16_t)PWMFULL * (uint16_t)(2u) / (uint16_t)(5u))

#endif


