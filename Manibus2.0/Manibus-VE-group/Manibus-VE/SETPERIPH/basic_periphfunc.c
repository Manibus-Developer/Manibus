#include "basic_periphfunc.h"
#include "basic_periphset.h"
#include "musmart.h"
#include "mpdu.h"
#include "esp8266.h"

void Choose_Way2Send(uint8_t* bytes, u16 length){

	  if(USART1_Channel_FLAG){
	   // Usart_ReadArray_(bytes,length);
			Usart_SendArray_(USART1_BASE,bytes,length);
	     //USART1_READ_FLAG = 0x00;   //这类放到结尾置0
	   }
		
	  if(WIFI_Channel_FLAG){
		pWifiRead_(bytes,length);
	//	WIFI_READ_FLAG = 0x00;
		}

}



void pPinOutPut(uint32_t IO,uint32_t Pin, uint32_t Level){
	
  if((char)Level==0x01){	
		
	((GPIO_TypeDef*)IO)->BSRR=(uint16_t)Pin;
			
	}else if((char)Level==0x00){
		
	((GPIO_TypeDef*)IO)->BRR=(uint16_t)Pin;

	}
}

void pPinReadBit(uint32_t IO,uint32_t Pin){
	
	  u8 temp=0;
	  u8 temp2[2];
	  if((((GPIO_TypeDef*)IO)->IDR & Pin)!=(uint32_t)Bit_RESET){
		
		 temp =0x01; 
		}else{
		temp =0x00; 
		}
		temp2[0] = pPinReadBit__ID;
		temp2[1] = temp;
    Choose_Way2Send(temp2,2);
}

/*
function: receive data from pc and send datas to another equipment
*/
void pUsartWrite(uint32_t Channel,uint8_t *words,u16 length){   //写给下位机二号
  
	Usart_SendArray_(Channel,words,length);
	
}

/*
function: receive data from equipment and send datas to another pc
*/

void pUsartRead(uint8_t *words,u16 datalength){    //读取给上位机
    
   Choose_Way2Send(words, datalength);
}

/*
function: get the adc value and send to the pc by USART1
*/

void f_To_h(const float *val,char hex[4]){    
	
    const char *p =(char*)(val);
    hex[0]=*((char * )p);
    hex[1]=*((char * )(p+1));
    hex[2]=*((char * )(p+2));
    hex[3]=*((char * )(p+3));	
}


void pADCRead(uint8_t channel){
	
    char Tmp[4];
	  u8 Tmp2[6];
	  float val;
	  unsigned short paramsCheck =0;  
	  unsigned char i = 0;
	
	  //if(ADC_Channel_registered[channel] ==1){
			val =  (float)ADC_GetValue[channel]/4096*3.3;
//		}else{
//		  val = 0.0;
//		}

	  f_To_h(&val,Tmp);
	
	  Tmp2[0] = pADCRead__ID;
	
	  for(i=0;i<4;i++){			
	  paramsCheck += Tmp[i];
			
	  }
		
	  memcpy(Tmp2+1,Tmp,4);

	  Tmp2[5] = paramsCheck % 0xFF;
	  Choose_Way2Send(Tmp2,6);	
}

/*
function: set PWM Pulse width
*/
void pPWMSetPulseWid (uint32_t TIM,uint8_t Channel,uint16_t pulsewid){

	switch(Channel){
		case 0x01:
				((TIM_TypeDef*)TIM)->CCR1=pulsewid;
			break;
		case 0x02:
			  ((TIM_TypeDef*)TIM)->CCR2=pulsewid;
			break;
		case 0x03:
				((TIM_TypeDef*)TIM)->CCR3=pulsewid;
			break;
		case 0x04:
			  ((TIM_TypeDef*)TIM)->CCR4=pulsewid;
			break;	
	}
}

/*
function: set PWM Pulse width   
*/

void pPWMSetFrequency(uint32_t TIM, uint16_t Prescaler){

		assert_param(IS_TIM_ALL_PERIPH(TIMx));
		((TIM_TypeDef*)TIM)->PSC = Prescaler;
		((TIM_TypeDef*)TIM)->EGR = TIM_PSCReloadMode_Immediate;
	
}

/*
function: set Get Pluse
*/

void pPWMGetPulse(uint32_t TIM,uint8_t channel){     //返回定时间脉冲值的校验为FD DD
	
	char Tmp[4];
	u8 Tmp2[6];
	float val;
	unsigned short paramsCheck =0;  
	unsigned char i = 0;
	
	val = encoder_get[channel];
	f_To_h(&val,Tmp);
	
	Tmp2[0] = pPWMGetPluse__ID;
	
	for(i=0;i<4;i++){			
	  paramsCheck += Tmp[i];
	}
	
	memcpy(Tmp2+1,Tmp,4);
	Tmp2[5] = paramsCheck % 0xFF;
	
  Choose_Way2Send(Tmp2,6);
			
}

/*
function: set Get in PWM PluseWid
*/

void i_To_h(const uint32_t *val,char hex[4]){    
	
    const char *p =(char*)(val);
    hex[0]=*((char * )p);
    hex[1]=*((char * )(p+1));
    hex[2]=*((char * )(p+2));
    hex[3]=*((char * )(p+3));
}

 void pPWMICGetPulseWid(uint32_t TIM,uint8_t Channel){

		char Tmp[4];
	  u8 Tmp2[6];
  	uint32_t val;
    uint8_t temp;
	  unsigned short paramsCheck =0; 
	  unsigned char i=0;
	 
	  temp = Channel>>4;
	  Channel &=0x0F;
	 
	  switch(TIM){
			case TIM2_BASE:
				if(temp==0x01){
				val= getWidth2_h[Channel-1];
				}else{
				val= getWidth2_l[Channel-1];
				}
      break;
			case TIM3_BASE:
				if(temp==0x01){
				val= getWidth3_h[Channel-1];
				}else{
				val= getWidth3_l[Channel-1];
				}	
      break;
			case TIM4_BASE:
				if(temp==0x01){
				val= getWidth4_h[Channel-1];
				}else{
				val= getWidth4_l[Channel-1];
				}
      break;
			case TIM5_BASE:
				if(temp==0x01){
				val= getWidth5_h[Channel-1];
				}else{
				val= getWidth5_l[Channel-1];
				}
      break;
		}

	  i_To_h(&val,Tmp);
		
		Tmp2[0] = pPWMICGetPluseWid__ID;
			
		for(i=0;i<4;i++){
		paramsCheck += Tmp[i];
		}
		memcpy(Tmp2+1,Tmp,4);
		
    Tmp2[5] = paramsCheck% 0xFF;
		
    
    Choose_Way2Send(Tmp2,6);
		
}
 
void pPWMICGetFrequency(uint32_t TIM,uint8_t Channel){

		char Tmp[4];
	  u8 Tmp2[6];
  	uint32_t val;
	  unsigned short paramsCheck =0; 
	  unsigned char i=0;
	
	  switch(TIM){
			case TIM2_BASE:
				val= getFrequancy2[Channel-1];
      break;
			case TIM3_BASE:
				val= getFrequancy3[Channel-1];
      break;
			case TIM4_BASE:
				val= getFrequancy4[Channel-1];
      break;
			case TIM5_BASE:
				val= getFrequancy5[Channel-1];
      break;
		}

	  i_To_h(&val,Tmp);
		
		Tmp2[0] = pPWMICGetFrequency__ID;
		
		for(i=0;i<4;i++){
		paramsCheck += Tmp[i];
		}
		
		memcpy(Tmp2+1,Tmp,4);
		
    Tmp2[5] = paramsCheck % 0xFF;
		
    
    Choose_Way2Send(Tmp2,6);		
}


