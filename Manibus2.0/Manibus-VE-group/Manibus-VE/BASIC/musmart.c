#include "musmart.h"
#include "musmart_cho.h"

uint8_t USART1_Send_Branch_Buffer[USART1_BUR_MAX];
uint8_t USART1_Rev_Branch_Buffer[USART1_BUR_MAX];

uint8_t Queue_USART1_OverFrame = 0;
u8 USART1_SendFlag_End  = 1;

char USART1_BASE_HAS_BEEN_INIT_FLAG = 0;

u8 USART1_Channel_FLAG = 0x00;


void localUsartDMAConfig(void){

    DMA_InitTypeDef DMA_InitStructure;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	  DMA_DeInit(DMA1_Channel4);
	   
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_Send_Branch_Buffer;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	  DMA_InitStructure.DMA_BufferSize = 0;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority =DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	  DMA_Init(DMA1_Channel4,&DMA_InitStructure);
	  
	  DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
	  DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE); 		
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
		
		DMA_Cmd(DMA1_Channel4,DISABLE);
	
	  DMA_DeInit(DMA1_Channel5); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_Rev_Branch_Buffer;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = USART1_BUR_MAX;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority =DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel5,&DMA_InitStructure);
		
		DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
	  DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
		
		DMA_Cmd(DMA1_Channel5,ENABLE);
		
}  


static void NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;

		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
}

void localUsartConfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);


		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = 
		USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART1, &USART_InitStructure);

		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		
		USART_Cmd(USART1, ENABLE);
		USART_ClearFlag(USART1,USART_FLAG_TC);

		localUsartDMAConfig();    //Set DMA 
    NVIC_Configuration();     //Set NVIC

		USART1_BASE_HAS_BEEN_INIT_FLAG =1;
	
}


void Usart_SendByte_( USART_TypeDef * pUSARTx, uint8_t ch)
{
		USART_SendData(pUSARTx,ch);		
		while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendArray_(uint32_t Channel,uint8_t *words,u16 length){
	
    u16 c;
	  for(c=length;c--;){	
			Usart_SendByte_((USART_TypeDef *)Channel,*(words));
			words++;
		}
}


void Usart_ReadByte_(uint8_t array){

USART_SendData(USART1,array);		
while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
}


void Usart_ReadArray_(uint8_t *array,u16 length)
{
	
	   if(length){			 
		 length =  length > USART1_BUR_MAX ? 1024: length; 
		 }else{		 
		 return;
		 }	 

     while(USART1_SendFlag_End ==0);
		 
	   if(array)memcpy(USART1_Send_Branch_Buffer,array,length);
	   DMA_ClearITPendingBit(DMA1_IT_TC4);
	   DMA_ClearITPendingBit(DMA1_IT_TE4);		  
	   DMA_Cmd(DMA1_Channel4, DISABLE);
	   DMA1_Channel4->CNDTR= length;
		 DMA_Cmd(DMA1_Channel4, ENABLE);
     USART1_SendFlag_End = 0;	
		 
}


void pUsartOnLine(void){

   USART1_Channel_FLAG = 0x01;
}
//int fputc(int ch, FILE *f)
//{

//		USART_SendData(USART1, (uint8_t) ch);		
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

//int fgetc(FILE *f)
//{

//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//		return (int)USART_ReceiveData(USART1);
//}


void USART1_IRQHandler(void){

	u16 data_len;

 if(USART_GetITStatus(USART1,USART_IT_IDLE)!= RESET){

		data_len  =USART1->SR;
		data_len  =USART1->DR;	
	 
		DMA_Cmd(DMA1_Channel5,DISABLE);
		data_len = USART1_BUR_MAX - DMA_GetCurrDataCounter(DMA1_Channel5);
	 
	//	Usart_ReadArray_(USART1_Rev_Branch_Buffer,data_len);
	  LoopList_AddMultiData(&DataBuffer,USART1_Rev_Branch_Buffer,data_len);
	 
	 	DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_TC5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5); 
		DMA1_Channel5->CNDTR = USART1_BUR_MAX;
	  
    DMA_Cmd(DMA1_Channel5,ENABLE);
 
	  USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}

void DMA1_Channel4_IRQHandler(void){

    DMA_ClearITPendingBit(DMA1_IT_TC4);
	  DMA_ClearITPendingBit(DMA1_IT_TE4);
    DMA_Cmd(DMA1_Channel4, DISABLE);
	  USART1_SendFlag_End = 1; 

}

void DMA1_Channel5_IRQHandler(void){
    
	  DMA_ClearITPendingBit(DMA1_IT_TC5);
	  DMA_ClearITPendingBit(DMA1_IT_TE5);
    DMA_Cmd(DMA1_Channel5, DISABLE); 
    DMA1_Channel5->CNDTR = USART1_BUR_MAX;
	  DMA_Cmd(DMA1_Channel5, ENABLE);
	
}




