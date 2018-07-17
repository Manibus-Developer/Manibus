#ifndef PERIPH_H_
#define PERIPH_H_

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "timref.h"

#define BUFFER_SPACE 500
#define REGULAR_MSG  50   
#define ADC_MAX 16

	
extern uint8_t toPc_data_U2[BUFFER_SPACE];
extern uint8_t Pc_data_U2[BUFFER_SPACE];

extern uint8_t toPc_data_U4[BUFFER_SPACE];
extern uint8_t Pc_data_U4[BUFFER_SPACE];


extern uint8_t toPc_regdata_U2[REGULAR_MSG];
extern uint8_t Pc_regdata_U2[REGULAR_MSG];

extern uint8_t toPc_regdata_U4[REGULAR_MSG];
extern uint8_t Pc_regdata_U4[REGULAR_MSG];

extern short refresh_msg_lenght_U2;
extern short refresh_msg_lenght_U4;


extern short data_count_U2;
extern short data_count_U4;

extern _Bool USART_MSG_Refresh_Mode_U2;
extern _Bool USART_MSG_Refresh_Mode_U4;




extern __IO uint16_t ADC_GetValue[ADC_MAX];

extern uint32_t getWidth2_h[2];
extern uint32_t getWidth3_h[2];
extern uint32_t getWidth4_h[2];
extern uint32_t getWidth5_h[2];

extern uint32_t getWidth2_l[2];
extern uint32_t getWidth3_l[2];
extern uint32_t getWidth4_l[2];
extern uint32_t getWidth5_l[2];

extern uint32_t getFrequancy2[2];
extern uint32_t getFrequancy3[2];
extern uint32_t getFrequancy4[2];
extern uint32_t getFrequancy5[2];

extern uint8_t ADC_Channel_registered[ADC_MAX];

void ClockCmd(uint32_t opt);

void pPinInit(uint32_t IO,uint32_t Pin,uint32_t GPIO_Mode , uint32_t GPIO_Speed);
void pPinDeInit(uint32_t IO);

void pUsartInit(uint32_t Channel,uint32_t BaudRate ,uint32_t WordLength,uint32_t StopBits,uint32_t Parity,uint32_t HardwareFlowControl ,uint32_t Mode,uint32_t msgMode);
void pUsartDeInit(uint32_t Channel);

void pADCInit(uint32_t clkdiv,uint8_t *channel,uint8_t length);
void pADCDeInit(void);

void pPWMInit(uint32_t TIM,uint8_t *Channel ,uint16_t Period, uint16_t Prescaler, uint16_t Pulse);
void pPWMICInit(uint32_t TIM,uint32_t *useMode);
void pPWMPluseInit(uint32_t TIM,uint32_t mMod,uint32_t Cir_Tim);
void pPWMDeInit(uint32_t TIM);





#endif



