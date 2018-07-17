#include<TCHAR.H>
#include<windows.h> 
#include <stdio.h>
#include <iostream>
#include <string>
#include "MGpio.h"
#include "MPWM.h"
#include "MADC.h"

#define MSERIAL_API __declspec(dllexport)
#define U8           unsigned char
#define U16          unsigned short

class MSerial: public MGPIO ,MPWM,MADC {

public: //these are located in MSerial.c

    HANDLE Port;

	MSERIAL_API HANDLE SerialInit(LPCWSTR COMx, int BaudRate);
	MSERIAL_API U16 SerialRead(HANDLE hCom, void* OutBuf, U16 size);
	MSERIAL_API U16 SerialWrite(HANDLE hCom, const void*Buf, U16 size);
	MSERIAL_API void SerialClose(HANDLE hCom);
	MSERIAL_API void SerialClearBuffer(U8*buf, U16 N);

	MSERIAL_API void SerialReadyRead(HANDLE hCom);

	bool SerialWaitFeedBack = false;    //it uses that  if wait Manibus board feed back 

public://these are located in MSerial4Periph.c

	//these are function of GPIO:
	MSERIAL_API void pPinInit(HANDLE hCom,GPIOIO_Type IO, GPIOPIN_Type Pin, GPIOMODE_Type Mode, GPIOSpeed_Type Speed);
	MSERIAL_API void pPinDeInit(HANDLE hCom, GPIOIO_Type IO);
	MSERIAL_API void pPinOutPut(HANDLE hCom, GPIOIO_Type IO, GPIOPIN_Type Pin, BitAction Level);
	MSERIAL_API char pPinReadBit(HANDLE hCom, GPIOIO_Type IO, GPIOPIN_Type Pin);

public://these are function of PWM(Major Example)

	MSERIAL_API void pPWMInit(HANDLE hCom, PWMTIM_Type TIM, PWMOCCHANNEL_Type *Channel,
		unsigned short Period, unsigned short Prescalar, unsigned short Pluse);
	MSERIAL_API void pPWMICInit(HANDLE hCom, PWMTIM_Type TIM, PWMICCHANNEL_Type *Channel);
	MSERIAL_API void pPWMPluseInit(HANDLE hCom, PWMTIM_Type TIM, PWMENCODER_Type Mode, unsigned short Circle);
	MSERIAL_API void pPWMDeInit(HANDLE hCom, PWMTIM_Type TIM);
	MSERIAL_API void pPWMSetPluseWid(HANDLE hCom,PWMTIM_Type TIM, PWMOCCHANNEL_Type Channel, unsigned short PluseWid);
	MSERIAL_API unsigned int pPWMICGetPluseWid(HANDLE hCom,PWMTIM_Type TIM, PWMICVALUE_Type Channel);
	MSERIAL_API unsigned int pPWMICGetFrequency(HANDLE hCom,PWMTIM_Type TIM, PWMICCHANNEL_Type Channel);
	MSERIAL_API unsigned int pPWMGetPluse(HANDLE hCom,PWMTIM_Type TIM, PWM_ENCODERCHANNEL_Type Channel);
	MSERIAL_API void pPWMSetFrequency(HANDLE hCom, PWMTIM_Type TIM, unsigned short Prescaler);

public://these are function of ADC(Major Example)
	MSERIAL_API void pADC_Init(HANDLE hCom,ADCSPEED_Type Speed, ADCCHANNEL_Type *Channel);
	MSERIAL_API void pADC_DeInit(HANDLE hCom);
	MSERIAL_API float pADC_Read(HANDLE hCom, ADCCHANNEL_Type Channel);


private:


};







