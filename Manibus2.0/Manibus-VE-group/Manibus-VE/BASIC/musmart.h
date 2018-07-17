#ifndef USMART_H_
#define USMART_H_

#include "stm32f10x.h"
#include "stdio.h"


#define USART1_BUR_MAX   500


extern char USART1_BASE_HAS_BEEN_INIT_FLAG;
extern u8 USART1_Channel_FLAG;

static void NVIC_Configuration(void);
void localUsartConfig(void);

void Usart_SendByte_( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendArray_(uint32_t Channel,uint8_t *words,u16 length);

void Usart_ReadByte_(uint8_t array);
void Usart_ReadArray_(uint8_t *array,u16 length);

void pUsartOnLine(void);
#endif



