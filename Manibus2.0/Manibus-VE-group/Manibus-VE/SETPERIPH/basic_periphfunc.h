#ifndef PERIPHFINC_H_
#define PERIPHFINC_H_

#include "stm32f10x.h"


void i_To_h(const uint32_t *val,char hex[4]);


void pPinOutPut(uint32_t IO,uint32_t Pin, uint32_t Level);
void pPinReadBit(uint32_t IO,uint32_t Pin);

void pUsartWrite(uint32_t Channel,uint8_t *words,u16 length);
void pUsartRead(uint8_t *words,u16 datalength);

void pADCRead(uint8_t channel);

void pPWMSetPulseWid (uint32_t TIM,uint8_t Channel, uint16_t pulsewid);
void pPWMGetPulse(uint32_t TIM,uint8_t channel);
void pPWMICGetPulseWid(uint32_t TIM,uint8_t Channel);
void pPWMICGetFrequency(uint32_t TIM,uint8_t Channel);
void pPWMSetFrequency(uint32_t TIM, uint16_t Prescaler);

void Choose_Way2Send(uint8_t* bytes, u16 length);

#endif


