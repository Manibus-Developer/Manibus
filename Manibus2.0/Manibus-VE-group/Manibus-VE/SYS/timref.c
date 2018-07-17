#include "timref.h"

static void NVIC_TIM6_Config(void){

	NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void encoder_Init(void){       //1∫¡√Î“ª¥Œ

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler= 7200-1;
  TIM_TimeBaseStructure.TIM_Period=10-1;
	TIM_TimeBaseStructure.TIM_ClockDivision =0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	TIM_Cmd(TIM6,ENABLE);
	NVIC_TIM6_Config();
	
}

unsigned int tim_count[6]={0};
unsigned int tim_limit[6]={0};
float encoder_get[6] ={0};
u8 use_pluse_num=0;
uint32_t tim_[6] ={0x00000000};

void TIM6_IRQHandler(void){ 
	
    char i=0;
		for(i=0;i<use_pluse_num;i++){
			tim_count[i]++;
			if(tim_count[i]>tim_limit[i]){
		  encoder_get[0] = (((TIM_TypeDef *)tim_[i])->CNT);
		  ((TIM_TypeDef *)tim_[i])->CNT=0;
			}
		}
}











