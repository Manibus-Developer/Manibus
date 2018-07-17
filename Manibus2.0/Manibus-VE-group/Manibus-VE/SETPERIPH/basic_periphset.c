#include "basic_periphset.h"
#include "basic_periphfunc.h"
#include "musmart.h"
#include "mpdu.h"
#include "esp8266.h"
//_Bool USE_USART2_=0;
//_Bool USE_USART3_=0;

//_Bool USE_TIM2_IC_=0;
//_Bool USE_TIM3_IC_=0;
//_Bool USE_TIM4_IC_=0;
//_Bool USE_TIM5_IC_=0;


_Bool USART_MSG_Refresh_Mode_U2 =0;
_Bool USART_MSG_Refresh_Mode_U4 =0;

uint8_t toPc_data_U2[BUFFER_SPACE];   
uint8_t Pc_data_U2[BUFFER_SPACE];       


uint8_t toPc_data_U4[BUFFER_SPACE];
uint8_t Pc_data_U4[BUFFER_SPACE];


uint8_t toPc_regdata_U2[REGULAR_MSG];
uint8_t Pc_regdata_U2[REGULAR_MSG];

uint8_t toPc_regdata_U4[REGULAR_MSG];
uint8_t Pc_regdata_U4[REGULAR_MSG];

short refresh_msg_lenght_U2 =0;
short refresh_msg_lenght_U4 =0;

short data_count_U2 =0;
short data_count_U4 =0;

short USART2_SIZE=0;
short UART4_SIZE =0;

unsigned char USART2_Interrupt_Deedback_Object = 0x00;
unsigned char UART4_Interrupt_Deedback_Object = 0x00;

void ClockCmd(uint32_t opt){

    switch(opt){
			
			case GPIOA_BASE:    //GPIOA
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			break;
			case GPIOB_BASE:    //GPIOB
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			break;
			case GPIOC_BASE:    //GPIOC
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			break;
			case GPIOD_BASE:    //GPIOD
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			break;
			case GPIOE_BASE:    //GPIOE
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			break;
			case GPIOF_BASE:    //GPIOF
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			break;
			case GPIOG_BASE:    //GPIOG
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			break;
      case USART2_BASE:    //USART2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		  //USE_USART2_ =1;		 
			break;
			case USART3_BASE:     //USART3
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		  //USE_USART3_	=1;			
			break;	
			case TIM2_BASE:   //TIM2
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
			//USE_TIM2_IC_=1;
			break;
			case TIM3_BASE:   //TIM3
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			//USE_TIM3_IC_=1;
			break;
			case TIM4_BASE:   //TIM4 
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			//USE_TIM4_IC_=1;
			break;
			case TIM5_BASE:   //TIM5 
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			//USE_TIM5_IC_=1;
			break;
			case TIM1_BASE:   //TIM1
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			break;
			case TIM8_BASE:   //TIM8
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			break;	

		}
}

uint16_t NVICCmd(uint32_t irqn){

switch(irqn){
	
case USART2_BASE: //USART2
	return USART2_IRQn;      //38

case USART3_BASE://USART3
	 return USART3_IRQn;     //39

case TIM2_BASE:   //TIM2
		return TIM2_IRQn;	     //28

case TIM3_BASE:   //TIM3
		return TIM3_IRQn;      //29

case TIM4_BASE:   //TIM4 
		return	TIM4_IRQn;     //30

case TIM5_BASE:   //TIM5 
		return	TIM5_IRQn;     //50

   }
}


void pPinInit(uint32_t IO,uint32_t Pin,uint32_t GPIO_Mode , uint32_t GPIO_Speed){
     
	   GPIO_InitTypeDef GPIO_InitStructure;
     ClockCmd(IO);

	   GPIO_InitStructure.GPIO_Pin = (uint16_t)Pin;
	   GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)GPIO_Mode;
	   GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)GPIO_Speed;
	
	   GPIO_Init((GPIO_TypeDef*)IO,&GPIO_InitStructure);
}

void pPinDeInit(uint32_t IO){

  GPIO_DeInit((GPIO_TypeDef*)IO);
	
  if((GPIO_TypeDef*)IO==GPIOA){        //这里防止GPIOA注释后 导致串口反馈无法使用
	localUsartConfig();
	}
	
}

uint32_t usart_opt( unsigned char temp){
	
	  if(temp==0x01){
		return 9600;	
		}else if(temp==0x02){
		return 56000;
		}else if(temp==0x03){
		return 115200;
		}else{
		return 9600;
		}
}


void Usart_msg_Mode(uint32_t Channel,uint8_t msgMode);

void USART2_DMAConfig(u8 mode){  //暂时只是用DMA的接收方式
	  
//	  u8* SendBuffer;
	  u8* RevBuffer;
	
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	  
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	  DMA_DeInit(DMA1_Channel7);
	   
	  
	if(mode == 1){
	   //SendBuffer = toPc_regdata_U2;
		 RevBuffer = Pc_regdata_U2;
	   USART2_SIZE = REGULAR_MSG;
	}else if(mode == 0){
		
     //SendBuffer = toPc_data_U2;
	   RevBuffer = Pc_data_U2;
		 USART2_SIZE = BUFFER_SPACE;
		 
	}
		
//	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuffer;
//	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//	  DMA_InitStructure.DMA_BufferSize = USART2_SIZE;
//	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	  DMA_InitStructure.DMA_Priority =DMA_Priority_High;
//	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	  DMA_Init(DMA1_Channel7,&DMA_InitStructure);
//	  
//	  DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
//	  DMA_ITConfig(DMA1_Channel7,DMA_IT_TE,ENABLE); 		
//		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
//		
//		DMA_Cmd(DMA1_Channel7,DISABLE);
	
	  DMA_DeInit(DMA1_Channel6); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RevBuffer;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = USART2_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority =DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel6,&DMA_InitStructure);
		
		DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
	  DMA_ITConfig(DMA1_Channel6,DMA_IT_TE,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
		
		DMA_Cmd(DMA1_Channel6,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
//		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);

}

void UART4_DMAConfig(u8 mode){     //暂时只是用DMA的接收方式
	
//  	u8* SendBuffer;
	  u8* RevBuffer;
	
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	  
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	
	  DMA_DeInit(DMA2_Channel5);
	   
	  if(mode == 1){
	   //RevBuffer = toPc_regdata_U4;
		 RevBuffer = Pc_regdata_U4;
	   UART4_SIZE = REGULAR_MSG;
			
	  }else if(mode == 0){
			
     //RevBuffer = toPc_data_U4;
	   RevBuffer = Pc_data_U4;
		 UART4_SIZE = BUFFER_SPACE;
	}

//	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuffer;
//	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//	  DMA_InitStructure.DMA_BufferSize = UART4_SIZE;
//	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	  DMA_InitStructure.DMA_Priority =DMA_Priority_High;
//	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	  DMA_Init(DMA2_Channel5,&DMA_InitStructure);
//	  
//	  DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);
//	  DMA_ITConfig(DMA2_Channel5,DMA_IT_TE,ENABLE); 		
//		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
//		
//		DMA_Cmd(DMA2_Channel5,DISABLE);
	
	  DMA_DeInit(DMA2_Channel3); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RevBuffer;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = UART4_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority =DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA2_Channel3,&DMA_InitStructure);
		
		DMA_ITConfig(DMA2_Channel3,DMA_IT_TC,ENABLE);
	  DMA_ITConfig(DMA2_Channel3,DMA_IT_TE,ENABLE);
    USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
		
		DMA_Cmd(DMA2_Channel3,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
//		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);	
		
}

void pUsartInit(uint32_t Channel,uint32_t BaudRate ,uint32_t WordLength,uint32_t StopBits,uint32_t Parity,uint32_t HardwareFlowControl ,uint32_t Mode,uint32_t msgMode){
	 
	
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	  
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
		ClockCmd(Channel);              //这里打开始时钟

	  Usart_msg_Mode(Channel ,(uint8_t)msgMode);    //这里进行信息模式选择
	
	  USART_InitStructure.USART_BaudRate =usart_opt((unsigned char)BaudRate)  ;
		USART_InitStructure.USART_WordLength = (uint16_t)WordLength;
    USART_InitStructure.USART_StopBits =(uint16_t)StopBits;
		USART_InitStructure.USART_Parity = (uint16_t) Parity;
		USART_InitStructure.USART_HardwareFlowControl=(uint16_t)HardwareFlowControl;
		USART_InitStructure.USART_Mode = (uint16_t)Mode;	
		USART_Init((USART_TypeDef *)Channel,&USART_InitStructure);
	
			
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = NVICCmd(Channel);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =NVICCmd(Channel)-37;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		USART_ITConfig(((USART_TypeDef *)Channel), USART_IT_IDLE, ENABLE);
		//USART_ITConfig(((USART_TypeDef *)Channel), USART_IT_RXNE, ENABLE);
			
    USART_Cmd(((USART_TypeDef *)Channel), ENABLE);
		
			
		if(Channel ==USART2_BASE){
		USART2_DMAConfig((uint8_t)msgMode);			
		}else if(Channel == UART4_BASE){
		UART4_DMAConfig((uint8_t)msgMode);
		}

}

void Usart_msg_Mode(uint32_t Channel,uint8_t msgMode){
	
     if(msgMode==0x00){
			 if(Channel ==USART2_BASE){
			 		 USART_MSG_Refresh_Mode_U2 =0;
				 
					 if(WIFI_Channel_FLAG){  //这里初始化中断发送对象
		       USART2_Interrupt_Deedback_Object = Wifi_Check;
		       }else{
					 USART2_Interrupt_Deedback_Object = USART_Check;
					 }
					 
			 }
       if(Channel ==UART4_BASE){
			 		 USART_MSG_Refresh_Mode_U4 =0;
				 
				 if(WIFI_Channel_FLAG){  //这里初始化中断发送对象
		       UART4_Interrupt_Deedback_Object = Wifi_Check;
		       }else{
					 UART4_Interrupt_Deedback_Object = USART_Check;
					 }
			 }		 
		 }else{
			 if(Channel ==USART2_BASE){
			 		 USART_MSG_Refresh_Mode_U2 =1;
				   refresh_msg_lenght_U2 = msgMode;
			 }
       if(Channel ==UART4_BASE){
			 		 USART_MSG_Refresh_Mode_U4 =1;
				   refresh_msg_lenght_U4 = msgMode;
			 }	 
		 }	 
}

void s_To_h_(const short *val,char hex[2]){    
	
    const char *p =(char*)(val);
    hex[0]=*((char * )p);
    hex[1]=*((char * )(p+1));
}

void USART_ICSD(u8 Object,uint8_t* bytes, u16 length){   //USART choose way to intrrupt

	  if(Object == USART_Check){
	   // Usart_ReadArray_(bytes,length);
			Usart_SendArray_(USART1_BASE,bytes,length);
	     //USART1_READ_FLAG = 0x00;   //这类放到结尾置0
	   }
		
	  if(Object == Wifi_Check){
		pWifiRead_(bytes,length);
	//	WIFI_READ_FLAG = 0x00;
		}
		
}

void USART2_IRQHandler(void){
			 
	   short data_len =0;
	   u8 *sendBuffer;
	   char len2hex[2] ={0};
		 u16 i =0;
		 u32 sum =0;
		 if(USART_GetITStatus(USART2, USART_IT_IDLE)!=RESET){
				  
				 data_len = USART2->SR;
				 data_len = USART2->DR;
				 
				 DMA_Cmd(DMA1_Channel6,DISABLE);
				 
				 if(!USART_MSG_Refresh_Mode_U2){    //该种模式设置为自动中断回复
				    
				 data_len = BUFFER_SPACE - DMA_GetCurrDataCounter(DMA1_Channel6);
				
				 DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6); 
		     DMA1_Channel6->CNDTR = BUFFER_SPACE;
					 
				 sendBuffer[0] = 0xFF;
				 sendBuffer[1] = 0xFA;
				 sendBuffer[2] = USART2_Check;
				 s_To_h_(&data_len,len2hex);
				 memcpy(sendBuffer+3,len2hex,2);
				 sendBuffer[5] = (len2hex[0] + len2hex[1])%0xFF;
				 memcpy(sendBuffer+6,Pc_data_U2,data_len);
				 for(i = data_len;i--;) sum +=Pc_data_U2[i];
				 sendBuffer[6+data_len] = sum%0xFF;
				 sendBuffer[7+data_len] = pUsartInterrupt;	
				
				 USART_ICSD(USART2_Interrupt_Deedback_Object,sendBuffer,data_len+7);
				  
				 }else if(USART_MSG_Refresh_Mode_U2){    //该种会消息式回复
					 
				 data_len = REGULAR_MSG - DMA_GetCurrDataCounter(DMA1_Channel6);
				 memcpy(toPc_regdata_U2,Pc_regdata_U2,data_len); 
				 DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6); 
		     DMA1_Channel6->CNDTR = REGULAR_MSG;  
				 data_count_U2 = data_len;	 
			   }
         
				 
         DMA_Cmd(DMA1_Channel6,ENABLE); 
			   USART_ClearITPendingBit(USART2,USART_IT_IDLE);
				 
		}
} 
		
void UART4_IRQHandler(void){

		 short data_len =0;
	   u8 *sendBuffer;
	   char len2hex[2] ={0};
		 u16 i =0;
		 u32 sum =0;
		 
		 if(USART_GetITStatus(UART4, USART_IT_IDLE)!=RESET){
			 
         data_len = UART4->SR;
				 data_len = UART4->DR;
			 
			 DMA_Cmd(DMA2_Channel3,DISABLE);
			 
			 if(!USART_MSG_Refresh_Mode_U4){    //该种模式设置为自动中断回复
			 data_len = BUFFER_SPACE - DMA_GetCurrDataCounter(DMA2_Channel3);

			 DMA_ClearFlag(DMA2_FLAG_GL3 | DMA2_FLAG_TC3 | DMA2_FLAG_TE3 | DMA2_FLAG_HT3);			 
			 DMA2_Channel3->CNDTR = BUFFER_SPACE;
				 
				 
			sendBuffer[0] = 0xFF;
			sendBuffer[1] = 0xFA;
			sendBuffer[2] = UART4_Check;
			s_To_h_(&data_len,len2hex);
			memcpy(sendBuffer+3,len2hex,2);
			sendBuffer[5] = (len2hex[0] + len2hex[1])%0xFF;
			memcpy(sendBuffer+6,Pc_data_U2,data_len);
			for(i = data_len;i--;) sum +=Pc_data_U2[i];
			sendBuffer[6+data_len] = sum%0xFF;
			sendBuffer[7+data_len] = pUsartInterrupt;	

			USART_ICSD(UART4_Interrupt_Deedback_Object,sendBuffer,data_len+7);
				 
			 }else if(USART_MSG_Refresh_Mode_U4){
				 
			 data_len = REGULAR_MSG - DMA_GetCurrDataCounter(DMA2_Channel3);
			 memcpy(toPc_regdata_U4,Pc_regdata_U4,data_len);
			 DMA_ClearFlag(DMA2_FLAG_GL3 | DMA2_FLAG_TC3 | DMA2_FLAG_TE3 | DMA2_FLAG_HT3);				 
       DMA2_Channel3->CNDTR = REGULAR_MSG;
			 data_count_U4 = data_len;	 
				 
			 }			 
			 
			  DMA_Cmd(DMA2_Channel3,ENABLE);
		    USART_ClearITPendingBit(UART4,USART_IT_IDLE);
		 }
}

//USART2
//void DMA1_Channel7_IRQHandler(void){

//    DMA_ClearITPendingBit(DMA1_IT_TC7);
//	  DMA_ClearITPendingBit(DMA1_IT_TE7);
//    DMA_Cmd(DMA1_Channel7, DISABLE);
//	 //USART1_SendFlag_End = 1; 

//}

void DMA1_Channel6_IRQHandler(void){
    
	  DMA_ClearITPendingBit(DMA1_IT_TC6);
	  DMA_ClearITPendingBit(DMA1_IT_TE6);
    DMA_Cmd(DMA1_Channel6, DISABLE); 
    DMA1_Channel6->CNDTR = USART2_SIZE;
	  DMA_Cmd(DMA1_Channel6, ENABLE);
	
}

//UART4
//void DMA2_Channel4_5_IRQHandler(void){

//    DMA_ClearITPendingBit(DMA2_IT_TC5);
//	  DMA_ClearITPendingBit(DMA2_IT_TE5);
//    DMA_Cmd(DMA2_Channel5, DISABLE);
//	 // USART1_SendFlag_End = 1; 

//}

void DMA2_Channel3_IRQHandler(void){
    
	  DMA_ClearITPendingBit(DMA2_IT_TC3);
	  DMA_ClearITPendingBit(DMA2_IT_TE3);
    DMA_Cmd(DMA2_Channel3, DISABLE); 
    DMA2_Channel3->CNDTR = UART4_SIZE;
	  DMA_Cmd(DMA2_Channel3, ENABLE);
	
}


void pUsartDeInit(uint32_t Channel){
  
     if(Channel==USART2_BASE){
			 
		 USART_DeInit(USART2);
			 
		 }else if(Channel==UART4_BASE){
			 
		 USART_DeInit(UART4);
			 
		 }
} 

__IO uint16_t ADC_GetValue[ADC_MAX]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};      //16路ADC
uint8_t ADC_Channel_registered[ADC_MAX] = {0};

//数组不行 静态不行  加 if  给ADC_RegularChannelConfig 不行
void pADCInit(uint32_t clkdiv,uint8_t *channel,uint8_t length){    //这里默认使用ADC1
		
	  uint8_t i =0;
	
		DMA_InitTypeDef DMA_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;	
			
	  for(i=0;i<length;i++){
		ADC_Channel_registered [channel[i]] =1;			
		}
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
		DMA_DeInit(DMA1_Channel1);
	  ADC_DeInit(ADC1);
	
		DMA_InitStructure.DMA_PeripheralBaseAddr= (u32)(&(ADC1->DR));
		DMA_InitStructure.DMA_MemoryBaseAddr=(u32)ADC_GetValue;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = ADC_MAX;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		DMA_Init(DMA1_Channel1,&DMA_InitStructure);

		DMA_Cmd(DMA1_Channel1,ENABLE);
		
		
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel=ADC_MAX;

		ADC_Init(ADC1,&ADC_InitStructure);

		RCC_ADCCLKConfig(clkdiv);     //不可以超过14M
	  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_2,3,ADC_SampleTime_55Cycles5);

		ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_4,5,ADC_SampleTime_55Cycles5);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_55Cycles5);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_6,7,ADC_SampleTime_55Cycles5);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_7,8,ADC_SampleTime_55Cycles5);


		ADC_RegularChannelConfig(ADC1, ADC_Channel_8,9,ADC_SampleTime_55Cycles5);		

		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,10,ADC_SampleTime_55Cycles5);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_10,11,ADC_SampleTime_55Cycles5);
		

		ADC_RegularChannelConfig(ADC1,ADC_Channel_11,12,ADC_SampleTime_55Cycles5);


		ADC_RegularChannelConfig(ADC1, ADC_Channel_12,13,ADC_SampleTime_55Cycles5);		

		ADC_RegularChannelConfig(ADC1,ADC_Channel_13,14,ADC_SampleTime_55Cycles5);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_14,15,ADC_SampleTime_55Cycles5);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_15,16,ADC_SampleTime_55Cycles5);

		ADC_DMACmd(ADC1,ENABLE);
	
		ADC_Cmd(ADC1,ENABLE);

		ADC_ResetCalibration(ADC1);
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1));
    
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void pADCDeInit(void){
	
 ADC_DeInit(ADC1);
	
}

void pPWMInit(uint32_t TIM,uint8_t *Channel ,uint16_t Period, uint16_t Prescaler, uint16_t Pulse){
	  
	  short i=0;
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef  TIM_OCInitStructure;
		ClockCmd(TIM);
	
	  TIM_DeInit((TIM_TypeDef *)TIM);
	  TIM_TimeBaseStructure.TIM_Period = Period-1; 
		TIM_TimeBaseStructure.TIM_Prescaler =Prescaler-1; 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit((TIM_TypeDef *)TIM, &TIM_TimeBaseStructure);

	  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	  TIM_OCInitStructure.TIM_Pulse = Pulse;
	
	  for(i=0;i<4;i++){	
			if(Channel[i]){
       switch(Channel[i]){
				 case 0x01:		 
			TIM_OC1Init((TIM_TypeDef *)TIM, &TIM_OCInitStructure); 
		  TIM_OC1PreloadConfig((TIM_TypeDef *)TIM, TIM_OCPreload_Enable);  
					 break;
				 case 0x02:			 
			TIM_OC2Init((TIM_TypeDef *)TIM, &TIM_OCInitStructure); 
		  TIM_OC2PreloadConfig((TIM_TypeDef *)TIM, TIM_OCPreload_Enable);  
					 break;
			   	case 0x03:		 
			TIM_OC3Init((TIM_TypeDef *)TIM, &TIM_OCInitStructure); 
		  TIM_OC3PreloadConfig((TIM_TypeDef *)TIM, TIM_OCPreload_Enable); 
					 break;
					 case 0x04:		 
			TIM_OC4Init((TIM_TypeDef *)TIM, &TIM_OCInitStructure); 
		  TIM_OC4PreloadConfig((TIM_TypeDef *)TIM, TIM_OCPreload_Enable);  
					 break;			 
			 }	
			}			
		}
		
	  TIM_CtrlPWMOutputs((TIM_TypeDef *)TIM,ENABLE);	
		
		TIM_ARRPreloadConfig((TIM_TypeDef *)TIM, ENABLE); 
	
	  TIM_Cmd((TIM_TypeDef *)TIM, ENABLE);			
}

uint16_t ic_channel_cho(uint16_t temp){

	   switch(temp){
			 case TIM_Channel_2:
				 return TIM_IT_CC2;

			 case TIM_Channel_4:
				 return TIM_IT_CC4;

			default:
			 return 0;
		 }  
}



void pPWMICInit(uint32_t TIM,uint32_t *useMode){ //useMOde 0x01
	
    int i=0;
	  uint16_t IT_CCX  =0x0000;
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_ICInitTypeDef  TIM_ICInitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
		
  	ClockCmd(TIM);	
		 
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		NVIC_InitStructure.NVIC_IRQChannel = NVICCmd(TIM);
	
	  if(NVICCmd(TIM)!=TIM5_IRQn){
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =NVICCmd(TIM)-28;
		}else{	
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;	
		}
		
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
		TIM_TimeBaseStructure.TIM_Prescaler =0; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(((TIM_TypeDef *)TIM), &TIM_TimeBaseStructure);

	  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
  	TIM_ICInitStructure.TIM_ICFilter = 0x0;
		
		for(i=0;i<2;i++){
			
			  if(useMode[i]!=0x00){
					
				IT_CCX|=ic_channel_cho(useMode[i]);
				TIM_ICInitStructure.TIM_Channel =useMode[i];
	    	TIM_PWMIConfig(((TIM_TypeDef *)TIM),&TIM_ICInitStructure);
				}
		}
		
		TIM_SelectInputTrigger(((TIM_TypeDef *)TIM),TIM_TS_TI2FP2);
		TIM_SelectSlaveMode(((TIM_TypeDef *)TIM),TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(((TIM_TypeDef *)TIM),TIM_MasterSlaveMode_Enable);
		
    TIM_ITConfig(((TIM_TypeDef *)TIM),IT_CCX,ENABLE);
    TIM_ClearFlag(((TIM_TypeDef *)TIM),IT_CCX);
    TIM_Cmd(((TIM_TypeDef *)TIM),ENABLE); 
		 
}

_Bool pluse_num(uint32_t TIM,uint32_t *temp){

	int i=0;
	for(i=0;i<use_pluse_num;i++){
	if(TIM==tim_[i]){
	return 1;
	   }
	}
	return 0;
}

void pPWMPluseInit(uint32_t TIM,uint32_t mMode,uint32_t Cir_Tim){    
	
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
    ClockCmd(TIM);
	
	  TIM_DeInit((TIM_TypeDef *)TIM);
	  TIM_TimeBaseStructure.TIM_Prescaler =5;
	  TIM_TimeBaseStructure.TIM_Period =0xffff;
	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit((TIM_TypeDef *)TIM, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig((TIM_TypeDef *)TIM,(uint16_t)mMode,TIM_ICPolarity_Rising,
	                             TIM_ICPolarity_Rising);
	
    TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter=1;
		TIM_ICInit((TIM_TypeDef *)TIM, &TIM_ICInitStructure);
			
		((TIM_TypeDef *)TIM)->CNT=0;
			
		TIM_Cmd((TIM_TypeDef *)TIM,ENABLE);
		
		
	  if(pluse_num(TIM,tim_)) return;
	  tim_limit[use_pluse_num]=Cir_Tim;
		tim_[use_pluse_num] = TIM;
		use_pluse_num++;
		encoder_Init();	
}

void pPWMDeInit(uint32_t TIM){

  TIM_DeInit(((TIM_TypeDef *)TIM));

}

uint32_t getWidth2_h[2]={0};
uint32_t getWidth3_h[2]={0};
uint32_t getWidth4_h[2]={0};
uint32_t getWidth5_h[2]={0};

uint32_t getWidth2_l[2]={0};
uint32_t getWidth3_l[2]={0};
uint32_t getWidth4_l[2]={0};
uint32_t getWidth5_l[2]={0};

uint32_t getFrequancy2[2] ={0};
uint32_t getFrequancy3[2] ={0};
uint32_t getFrequancy4[2] ={0};
uint32_t getFrequancy5[2] ={0};

void TIM2_IRQHandler(void){
		
    if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM2->CCR2;
		pulse	 = TIM2->CCR1;
		Frequency = SystemCoreClock/IC2Value;
    getWidth2_h[0] =IC2Value-pulse; 
		getWidth2_l[0] = 	pulse;
		getFrequancy2[0] =Frequency/1000; 
 			
		}
		
	  if(TIM_GetITStatus(TIM2,TIM_IT_CC4)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM2->CCR4;
		pulse	 = TIM2->CCR3;
		Frequency = SystemCoreClock/IC2Value;
    getWidth2_h[1] =IC2Value-pulse; 
		getWidth2_l[1] = 	pulse;
		getFrequancy2[1] =Frequency/1000;  	
			
		}
				
}
	
void TIM3_IRQHandler(void){
	
		if(TIM_GetITStatus(TIM3,TIM_IT_CC2)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM3->CCR2;
		pulse	 = TIM3->CCR1;
		Frequency = SystemCoreClock/IC2Value;
    getWidth3_h[0] =IC2Value-pulse; 
		getWidth3_l[0] = 	pulse;
		getFrequancy3[0] =Frequency/1000;  			
		}
		
	  if(TIM_GetITStatus(TIM3,TIM_IT_CC4)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM3->CCR4;
		pulse	 = TIM3->CCR3;
		Frequency = SystemCoreClock/IC2Value;
    getWidth3_h[1] =IC2Value-pulse; 
		getWidth3_l[1] = 	pulse;
		getFrequancy3[1] =Frequency/1000;  			
		}	
}

void TIM4_IRQHandler(void){
		    if(TIM_GetITStatus(TIM4,TIM_IT_CC2)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM4->CCR2;
		pulse	 = TIM4->CCR1;
		Frequency = SystemCoreClock/IC2Value;
    getWidth4_h[0] =IC2Value-pulse; 
		getWidth4_l[0] = 	pulse;
		getFrequancy4[0] =Frequency/1000;  			
		}
		
	  if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM4->CCR4;
		pulse	 = TIM4->CCR3;
		Frequency = SystemCoreClock/IC2Value;
    getWidth4_h[1] =IC2Value-pulse; 
		getWidth4_l[1] = 	pulse;
		getFrequancy4[1] =Frequency/1000;  			
		}				
}


void TIM5_IRQHandler(void){
	
	 if(TIM_GetITStatus(TIM5,TIM_IT_CC2)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM5->CCR2;
		pulse	 = TIM5->CCR1;
		Frequency = SystemCoreClock/IC2Value;
    getWidth5_h[0] =IC2Value-pulse; 
		getWidth5_l[0] = 	pulse;
		getFrequancy5[0] =Frequency/1000;  			
		}
		
	  if(TIM_GetITStatus(TIM5,TIM_IT_CC4)!=RESET){
			
	  int IC2Value=0; 
    float Frequency=0; 
    int pulse=0;
			
		IC2Value=TIM5->CCR4;
		pulse	 = TIM5->CCR3;
		Frequency = SystemCoreClock/IC2Value;
    getWidth5_h[1] =IC2Value-pulse; 
		getWidth5_l[1] = 	pulse;
		getFrequancy5[1] =Frequency/1000;  			
		}
}













