#include "musmart_cho.h"
#include "mpdu.h"
#include "i2c.h"
#include "musmart.h"
#include "esp8266.h"

#define MAX_PARAM   10
#define DATA_GET_LIMIT 2

LoopListStruct DataBuffer;

uint8_t pcOrder_data[MAX_DATA_TRANSFORM];      //write data through QueueBus


void Bytes_BUS_RecInit(void){
	
	LoopList_Init(&DataBuffer,RecDataBufferMaxSize);
	
}

void Msg_Filtrate(void){             //主线程执行函数
	
    unsigned char Tmp;
	  unsigned char dir;
	  char choState = CHO_RIGHT;
	
if(DataBuffer.DataSize>DATA_GET_LIMIT){

	
	  LoopList_GetOneData(&DataBuffer,&Tmp);
		
		if(Tmp>>4==0x0F){
		  
			dir = Tmp&0x0F;
			
			switch(dir){
				
				case 0x0E/*IO*/ :			    
        	  choState = cho_io();				
					break;
				
				case 0x0A/*USART*/:
					  choState = cho_usart();			
				break;
				
				
				case 0x0C/*ADC*/:
					  choState = cho_adc();
				break;
				
				
				case 0x0D /*PWM*/:
					  choState = cho_pwm();
					break;
				
				case 0x0B /*IIC*/:
					  choState = cho_iic();
				break;	
				
        case 0x09/*Wifi*/:
        		choState = cho_wifi();
         break;				
			 }
			
			 
			  if(choState ==CHO_ERROR){			  //防止函数出错，会传递到其他通讯对象		
					USART1_Channel_FLAG =0x00;
					WIFI_Channel_FLAG =0x00;
				}
			 
	  	}else{
		return;
		}
   }
}

void msg_feedback(uint8_t byte){
	  
	  if(USART1_Channel_FLAG){
			
			//Usart_ReadArray_(&byte,1);
			Usart_SendByte_(USART1,byte);  //暂时不使用DMA发送
	    USART1_Channel_FLAG = 0x00;   //这类放到结尾置0
			
	   }else if(WIFI_Channel_FLAG){
			
			pWifiRead_(&byte,1);
			WIFI_Channel_FLAG = 0x00;
		}
		
}

void msg_feedback_ID(uint8_t byte){
	
		  if(USART1_Channel_FLAG){
			
			Usart_SendByte_(USART1,byte);  //暂时不使用DMA发送
	   }else if(WIFI_Channel_FLAG){
			
			pWifiRead_(&byte,1);

		}	
}


u32 read_addr(u32 addr)
{
	return *(u32*)addr;
}

void write_addr(u32 addr,u32 val)
{
	*(u32*)addr=val; 	
}

bool ParamsCheckOut(unsigned short sum,unsigned short count,unsigned char compare){   //参数值校验函数

	  unsigned char temp ;
	  temp = sum % count;
	  if (compare == temp ) return true; else  return false;
	  
}



char cho_io(void){      //已确认
	  
    unsigned char Tmp;
	  unsigned char Tmp1;
	  unsigned char dir;	
	  uint32_t u32_param[MAX_PARAM];

	  unsigned short paramsCheck =0;  
	  unsigned short paramsCount = 0;
	  
	  LoopList_ReadFeature io_feature;
		io_feature = LoopList_Marked(&DataBuffer);    //这里对读取点进行标记
	
  	LoopList_GetOneData(&DataBuffer,&dir);
	
	  LoopList_GetOneData(&DataBuffer,&Tmp);  //这里得到端口
	  u32_param[0] = APB2PERIPH_BASE|(Tmp<<8);
	
	  paramsCheck += Tmp;  paramsCount ++;

	  switch(dir){
			
			case 0xDA:
				
		  LoopList_GetOneData(&DataBuffer,&Tmp);
			 Tmp1 = Tmp>>4;
			 u32_param[1]= (Tmp&0x0F)<<Tmp1;	
			
			paramsCheck += Tmp;  paramsCount ++;
			
			LoopList_GetOneData(&DataBuffer,&Tmp);	
			 u32_param[2] = Tmp;
			
			paramsCheck += Tmp;  paramsCount ++;
			
			if(u32_param[2] != 0x00 && u32_param[2] != 0x01) return CHO_ERROR;
			

			LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;  
			
			
		 	LoopList_GetOneData(&DataBuffer,&Tmp);	
			 if(Tmp!= pPinOutPut__ID ){
				 
				 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			 }	
			 
			 (*(u32(*)())usmart_nametab[4].func)(u32_param[0],u32_param[1],u32_param[2]);	
			 msg_feedback_ID(pPinOutPut__ID); //返回ID
			 
			break;
			 
			case 0xDB:
				
		  LoopList_GetOneData(&DataBuffer,&Tmp);
			 Tmp1 = Tmp>>4;
			 u32_param[1]= (Tmp&0x0F)<<Tmp1;		
			paramsCheck += Tmp;  paramsCount ++;
			
			LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR; 
			
			LoopList_GetOneData(&DataBuffer,&Tmp); 
		  if(Tmp!= pPinReadBit__ID ){
				LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			 }	
			 
			 (*(u32(*)())usmart_nametab[20].func)(u32_param[0],u32_param[1]);
			 
			 //msg_feedback_ID(pPinReadBit__ID);   //函数内已发送
			 
			break; 
			 
			case 0xCA:
				
		  LoopList_GetOneData(&DataBuffer,&Tmp);
			 Tmp1 = Tmp>>4;
			 u32_param[1]= (Tmp&0x0F)<<Tmp1;
			paramsCheck += Tmp;  paramsCount ++;			
			LoopList_GetOneData(&DataBuffer,&Tmp);
			 u32_param[2] = Tmp;
			paramsCheck += Tmp;  paramsCount ++;
			LoopList_GetOneData(&DataBuffer,&Tmp);
			 u32_param[3] = Tmp;
			paramsCheck += Tmp;  paramsCount ++;
			
			LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR; 
			
		  LoopList_GetOneData(&DataBuffer,&Tmp);	
			 if(Tmp!= pPinInit__ID ){
				 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				 
			    return CHO_ERROR;
			 }
			 
			(*(u32(*)())usmart_nametab[2].func)(u32_param[0],(uint16_t)u32_param[1],u32_param[2],u32_param[3]);	
			 msg_feedback_ID(pPinInit__ID);  //返回ID
			 
			break;
			
			case 0xAA:
				
			LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR; 
			
			 LoopList_GetOneData(&DataBuffer,&Tmp);
			 if(Tmp!= pPinDeInit__ID ){
				 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			 }
			 
			 (*(u32(*)())usmart_nametab[3].func)(u32_param[0]);
			  msg_feedback_ID(pPinDeInit__ID);  //返回ID
		    break;
			 
			 default:	
				 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				  return CHO_ERROR;

		}
		  
		  msg_feedback(IO_Check);    //结尾校验位
		  return CHO_RIGHT;
}


void s_To_h(const short *val,char hex[2]){    
	
    const char *p =(char*)(val);
    hex[0]=*((char * )p);
    hex[1]=*((char * )(p+1));
}

unsigned int byteMsgSum(unsigned char *msg,unsigned short length){
	
    unsigned short i =0;
  	unsigned int sum = 0;
	
		for(i = length-1;i--;){
		
			sum += msg[i];
		}
     return sum;
}

char cho_usart(void){      //这里使用USART2或者UART4
	
	
    unsigned char Tmp;
  	unsigned char dir;
	  uint32_t u32_param[MAX_PARAM];
	  short i=0;
	  char datalength =0;
	  char shortParam[2]={0};
		char shortParam_check[3] = {0};
	  unsigned char usart_param_check[2];
		
		unsigned short paramsCheck =0;  
	  unsigned short paramsCount = 0;
		
		LoopList_ReadFeature io_feature;		
		io_feature = LoopList_Marked(&DataBuffer);    //这里对读取点进行标记
		
	  LoopList_GetOneData(&DataBuffer,&dir);
		
		if(dir == 0xDC){ //这里为读取串口方式调用函数  特殊
			
		LoopList_GetOneData(&DataBuffer,&Tmp);
			
		if(Tmp!= pUsartOnLine__ID ){
			
			 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			 return CHO_ERROR;
		}	
			(*(u32(*)())usmart_nametab[37].func)();
		
				return CHO_RIGHT;      //这里不在到后面有反馈值
		}
		
		
    LoopList_GetOneData(&DataBuffer,&Tmp); //  Channel 
		u32_param[0] = APB1PERIPH_BASE|(Tmp<<8);
		paramsCheck += Tmp;  paramsCount ++;
		
	  switch(dir){
	
			case 0xDA:
			
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  datalength = Tmp;
			  paramsCheck += Tmp;  paramsCount ++;	
			  
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			  paramsCheck =0;paramsCount=0;
			
			  for(i=0;i<datalength;i++){
					LoopList_GetOneData(&DataBuffer,&Tmp);
					pcOrder_data[i] = Tmp;	
					paramsCheck += Tmp;  paramsCount ++;
				}

				 LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			   if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
				
				 LoopList_GetOneData(&DataBuffer,&Tmp);		
			   if(Tmp!= pUsartWrite__ID ){
					 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			   }
				 
				(*(u32(*)())usmart_nametab[7].func)(u32_param[0],pcOrder_data,datalength);			 
		  	msg_feedback_ID(pUsartWrite__ID);  //返回ID
				 
				break;
				 
			case 0xDB:
				
				 LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			   if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				 LoopList_GetOneData(&DataBuffer,&Tmp);	
			   if(Tmp!= pUsartRead__ID ){
					 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			   }
				 
				 if(u32_param[0]==USART2_BASE){
					 
					  usart_param_check[0] = pUsartRead__ID ;   //已经返回ID
					  usart_param_check[1] = USART2_Check;	
					 
	         (*(u32(*)())usmart_nametab[8].func)(usart_param_check,2);  //USART2
					 					 
					 (*(u32(*)())usmart_nametab[8].func)(toPc_regdata_U2,data_count_U2);
						 
					 

				}else if(u32_param[0]==UART4_BASE){
					
					  usart_param_check[0] = pUsartRead__ID;    //已经返回ID
					  usart_param_check[1] = UART4_Check;	
					
			     (*(u32(*)())usmart_nametab[8].func)(usart_param_check,2);    //UART4
					
					 (*(u32(*)())usmart_nametab[8].func)(toPc_regdata_U4,data_count_U4);	
											 
					 				 
		    }else{
				return CHO_ERROR;
				}				

				break;
			
			case 0xCA:
			   
			   LoopList_GetOneData(&DataBuffer,&Tmp);    //BaudRate
			   u32_param[1] = Tmp;
			   paramsCheck += Tmp;  paramsCount ++;
			
         LoopList_GetOneData(&DataBuffer,&Tmp);    //WordLength
			   u32_param[2] = Tmp<<8;
			   paramsCheck += Tmp;  paramsCount ++;
			
			   LoopList_GetOneData(&DataBuffer,&Tmp);    //StopBits
			   u32_param[3] = Tmp<<8;
			   paramsCheck += Tmp;  paramsCount ++;
			
			   LoopList_GetOneData(&DataBuffer,&Tmp);    //Parity
			   u32_param[4] = Tmp<<8;
			   paramsCheck += Tmp;  paramsCount ++;
			
			   LoopList_GetOneData(&DataBuffer,&Tmp);    //HardwareFlowControl
			   u32_param[5] = Tmp<<8;
			   paramsCheck += Tmp;  paramsCount ++;
			
			   LoopList_GetOneData(&DataBuffer,&Tmp);    //Mode
			   u32_param[6] = Tmp;
			   paramsCheck += Tmp;  paramsCount ++;
				 
			   LoopList_GetOneData(&DataBuffer,&Tmp);    //MsgMode
			   u32_param[7] = Tmp;
				 paramsCheck += Tmp;  paramsCount ++;
				 
				 LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			   if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
				 
				 LoopList_GetOneData(&DataBuffer,&Tmp);
			   if(Tmp!= pUsartInit__ID ){
					 LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR; 
			   }
			   
         (*(u32(*)())usmart_nametab[5].func)(u32_param[0],u32_param[1],u32_param[2],
				  u32_param[3],u32_param[4],u32_param[5],u32_param[6],u32_param[7]);
				 msg_feedback_ID(pUsartInit__ID);  //返回ID
				break;
				 
			case 0xAA:		
				
				 LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			   if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR; 
			
				 LoopList_GetOneData(&DataBuffer,&Tmp);
			   if(Tmp!= pUsartDeInit__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			   }
				 
				 (*(u32(*)())usmart_nametab[6].func)(u32_param[0]);
				 msg_feedback_ID(pUsartDeInit__ID);  //返回ID
				break;
				 
			  default:	
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				  return CHO_ERROR;
		}
		
		msg_feedback(USART_Check);
		return CHO_RIGHT;
}



char cho_adc(void){

    uint8_t Tmp;
	  uint8_t dir;
	  uint32_t u32_param[MAX_PARAM];
	  uint8_t u8_param[ADC_MAX];
		uint8_t u8_param_length=0;
	
		unsigned short paramsCheck =0;  
	  unsigned short paramsCount = 0;
		LoopList_ReadFeature io_feature;
	  
	  short i=0;
	
	  io_feature = LoopList_Marked(&DataBuffer);    //这里对读取点进行标记
	
	  LoopList_GetOneData(&DataBuffer,&dir); 
	
	  switch(dir){
			
			case 0xDA:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  u32_param[0] = Tmp;
			  paramsCheck += Tmp;  paramsCount ++;
			  
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= pADCRead__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				
			  (*(u32(*)())usmart_nametab[11].func)((uint8_t)u32_param[0]);
			   //msg_feedback_ID(pADCRead__ID);  //已经返回ID
	  break;
			
			case 0xCA:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);	  
			  u32_param[0] = Tmp<<12;
				paramsCheck += Tmp;  paramsCount ++; 
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);	
				u8_param_length = Tmp;
			  paramsCheck += Tmp;  paramsCount ++;
			
			  for(i=0;i<u8_param_length;i++){
				 LoopList_GetOneData(&DataBuffer,&Tmp);	
			   u8_param[i] = Tmp;
				 paramsCheck += Tmp;  paramsCount ++; 
			  }
				
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= pADCInit__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				
				
			  (*(u32(*)())usmart_nametab[9].func)(u32_param[0],u8_param,u8_param_length);
				 msg_feedback_ID(pADCInit__ID);  //返回ID
			break;
				
		  	case 0xAA:
					
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= pADCDeInit__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				 
			   (*(u32(*)())usmart_nametab[10].func)();
		    	msg_feedback_ID(pADCDeInit__ID);  //返回ID
			break;
				
			default:
				LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				  return CHO_ERROR;				
		 }
		
		 msg_feedback(ADC_Check);
		 
		 return CHO_RIGHT;
}

char cho_pwm(void){
	 
    unsigned char Tmp;
	  unsigned char dir;
	  unsigned char TmpBfr[4];
	  uint32_t u32_param[MAX_PARAM];
	  
	  unsigned char TmpBfr2[2];
	  uint16_t u16_param[3];
	  
	  unsigned short paramsCheck =0;  
	  unsigned short paramsCount = 0;
		LoopList_ReadFeature io_feature;
	  unsigned char i=0 ;
	  io_feature = LoopList_Marked(&DataBuffer);    //这里对读取点进行标记
	
   	LoopList_GetOneData(&DataBuffer,&dir);
	
	  LoopList_GetOneData(&DataBuffer,&Tmp);
	
	  if(Tmp==0x34|Tmp==0x2c){
			 u32_param[0] = APB2PERIPH_BASE+(Tmp<<8);
		 }else{
		   u32_param[0] = APB1PERIPH_BASE+(Tmp<<8); 
		 }
    paramsCheck += Tmp;  paramsCount ++;
		 
	  switch(dir){
			
			case 0xDA:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);		  
			  u32_param[1] = Tmp;
			  paramsCheck += Tmp;  paramsCount ++; 
			
			  LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);
		  	u16_param[0] = *((uint16_t*)TmpBfr2);
			  paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				LoopList_GetOneData(&DataBuffer,&Tmp);				
				if(Tmp!= pPWMSetPluseWid__ID ){
					
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				
			  (*(u32(*)())usmart_nametab[16].func)(u32_param[0],(uint8_t)u32_param[1],u16_param[0]);
			  msg_feedback_ID(pPWMSetPluseWid__ID);  //返回ID
				break;	
				
			case 0xDB:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);		  
			  u32_param[1] = Tmp;
			  paramsCheck += Tmp;  paramsCount ++; 
			  
			  LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				LoopList_GetOneData(&DataBuffer,&Tmp);
			
				if(Tmp!= pPWMICGetPluseWid__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				
			   (*(u32(*)())usmart_nametab[17].func)(u32_param[0],(uint8_t)u32_param[1]);
			   //msg_feedback_ID(pPWMICGetPluseWid__ID);  //函数内返回
				break;
				
			case 0xDC:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);		  
			  u32_param[1] = Tmp;
			  paramsCheck += Tmp;  paramsCount ++;
			  
			  LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				LoopList_GetOneData(&DataBuffer,&Tmp);
				if(Tmp!= pPWMICGetFrequency__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				
			   (*(u32(*)())usmart_nametab[18].func)(u32_param[0],(uint8_t)u32_param[1]);	
				  //msg_feedback_ID(pPWMICGetFrequency__ID);  //函数内返回
				
				break;		
				
			case 0xDD:
				  
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  u32_param[1] = Tmp;
			  paramsCheck += Tmp;  paramsCount ++;
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				LoopList_GetOneData(&DataBuffer,&Tmp);
				if(Tmp!= pPWMGetPluse__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			  }
				
			   (*(u32(*)())usmart_nametab[19].func)(u32_param[0],(uint8_t)u32_param[1] );
				  //msg_feedback_ID(pPWMGetPluse__ID);  //函数内返回
				break;
				
			case 0xDE:
				
        LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);
		  	u16_param[0] = *((uint16_t*)TmpBfr2);
			  paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;
			  
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
				LoopList_GetOneData(&DataBuffer,&Tmp);
				if(Tmp!= pPWMSetFrequency__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
			    return CHO_ERROR;
			   }
			  (*(u32(*)())usmart_nametab[38].func)(u32_param[0],u16_param[0] );
				 msg_feedback_ID(pPWMSetFrequency__ID);  //返回ID
				break;
				 
      case 0xCA:
			
				LoopList_GetMultiData(&DataBuffer,TmpBfr,4,BIGLIN);
        for(i=4;i--;){
				paramsCheck+= TmpBfr[i];
				paramsCount++;	
				}		
				
	     	LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);     
			  u16_param[0] = *((uint16_t*)TmpBfr2); 
				paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;				
			  LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);
			  u16_param[1] = *((uint16_t*)TmpBfr2); 
				paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;
			  LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);    //这里还是倒序
			  u16_param[2] = *((uint16_t*)TmpBfr2);
			  paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;
				
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= pPWMInit__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				return CHO_ERROR;
				}
				
       (*(u32(*)())usmart_nametab[12].func)(u32_param[0],TmpBfr,u16_param[0],u16_param[1],u16_param[2]);			
			  msg_feedback_ID(pPWMInit__ID);  //返回ID
				break;
				
			case 0xCB:
				
				LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);
			  paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;
			  
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			
			  if(Tmp!=pPWMICInit__ID){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				return CHO_ERROR;
				}
				(*(u32(*)())usmart_nametab[13].func)(u32_param[0],TmpBfr2);
			  msg_feedback_ID(pPWMICInit__ID);  //返回ID
				break;
			
		  case 0xCC:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  u32_param[1] = Tmp;
		  	paramsCheck += Tmp;  paramsCount ++;
			
				LoopList_GetMultiData(&DataBuffer,TmpBfr2,2,LITTERLIN);
			  u32_param[2] = *((uint16_t*)TmpBfr2);
			   paramsCheck += TmpBfr2[0];paramsCheck += TmpBfr2[1]; paramsCount +=2;
			  
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;			
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			
			  if(Tmp!=pPWMPluseInit__ID){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				return CHO_ERROR;
				}
				
			 (*(u32(*)())usmart_nametab[14].func)(u32_param[0],u32_param[1], u32_param[2]);
				msg_feedback_ID(pPWMPluseInit__ID);  //返回ID
				break;
				
			case 0xAA:
								
				LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
			  if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			
			  if(Tmp!= pPWMDeInit__ID ){
					LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
					
				return CHO_ERROR;
				}
				
				(*(u32(*)())usmart_nametab[15].func)(u32_param[0]);
			  msg_feedback_ID(pPWMDeInit__ID);  //返回ID
				
				break;
				
				
			  default:
				  LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				  return CHO_ERROR;
	 	}
		
	   msg_feedback(PWM_Check);
}

char cho_iic(void){

		unsigned char Tmp;
		unsigned char Tmp1;
		unsigned char dir;
		unsigned char i;
		
		uint32_t u32_param[MAX_PARAM];
		unsigned char iic_Code[IIC_CodeNum_Max];
	
		unsigned short paramsCheck =0;  
		unsigned short paramsCount = 0;
		LoopList_ReadFeature io_feature;
	  io_feature = LoopList_Marked(&DataBuffer);    //这里对读取点进行标记
	
		LoopList_GetOneData(&DataBuffer,&dir);
	
	 switch(dir){
		 
		 case 0xCA:
			 
		 LoopList_GetOneData(&DataBuffer,&Tmp);    //SCL
		 u32_param[0] = APB2PERIPH_BASE|(Tmp<<8);
		 paramsCheck += Tmp;  paramsCount ++;
		 
		 LoopList_GetOneData(&DataBuffer,&Tmp);
		 Tmp1 = Tmp>>4;
		 u32_param[1]= (Tmp&0x0F)<<Tmp1;
		 paramsCheck += Tmp;  paramsCount ++;
		 
		 LoopList_GetOneData(&DataBuffer,&Tmp);    //SDA
		 u32_param[2] = APB2PERIPH_BASE|(Tmp<<8);
		 paramsCheck += Tmp;  paramsCount ++;
		 
		 LoopList_GetOneData(&DataBuffer,&Tmp);
		 Tmp1 = Tmp>>4;
		 u32_param[3]= (Tmp&0x0F)<<Tmp1;
		 paramsCheck += Tmp;  paramsCount ++;
		 
		 LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
		 if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
		 
	  	LoopList_GetOneData(&DataBuffer,&Tmp);
		  if(Tmp!= pIICInit__ID ){
				LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				return CHO_ERROR;
				}
		  
			(*(u32(*)())usmart_nametab[21].func)( u32_param[0],(uint16_t)u32_param[1],u32_param[2],u32_param[3]);	
			msg_feedback_ID(pIICInit__ID);  //返回ID	
				
			break;
				
		 case 0xDA:
			 
			 LoopList_GetOneData(&DataBuffer,&Tmp);    //这里进行iic编号选择
		   iic_cho = Tmp;
		   paramsCheck += Tmp;  paramsCount ++;
		 
		   if(iic_cho == IIC_ONE){
				 
			 SCL_cho =0;
			 SDA_cho =1;	
				 
			 }else if(iic_cho == IIC_TWO){
				 
			 SCL_cho =2;
			 SDA_cho =3;
				 
			 }else{
			 return CHO_ERROR;
			 }
			 
		   LoopList_GetOneData(&DataBuffer,&Tmp);
			 Tmp1 = Tmp;
			 paramsCheck += Tmp;  paramsCount ++;
			 
		   for(i=0;i<Tmp1;i++){
				 
			 LoopList_GetOneData(&DataBuffer,&Tmp);
			 iic_Code[i] = Tmp;
			 paramsCheck += Tmp;  paramsCount ++; 
				 
			 }
			 
			 LoopList_GetOneData(&DataBuffer,&Tmp);			            //这里进行参数值校验
		   if(!ParamsCheckOut(paramsCheck,paramsCount,Tmp)) return CHO_ERROR;
			 
      LoopList_GetOneData(&DataBuffer,&Tmp);			 
		  if(Tmp!= pIICFunc__ID){
				LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				return CHO_ERROR;
				}
			
				
		  for(i=0;i<=Tmp1;i++){
				
				if(iic_Code[i] == 0x06 | (iic_Code[i]>>4) == 0x06){
					
					(*(u32(*)())iic_name[iic_Code[i]].func)(iic_Code[i+1]);	
					 i++;		
					
				}else{		
        	(*(u32(*)())iic_name[iic_Code[i]].func)();	
				}				
			}
			
			
			if(iic_waitask_Succeed==0x00){   /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
				
			    iic_waitask_Succeed =0x01;
			  	(*(u32(*)())iic_name[iic_Code[1]].func)();
				
			}
			
      Queue_readAllDataes();      //这里将数据传输到pc
			//msg_feedback_ID(pIICFunc);  //函数内返回ID
			 break;		
			
			 default:
				  LoopList_FeedBack(&DataBuffer,io_feature.ReadPoint,io_feature.DataSize);  //这里错误，回归到队列标记点
				 return CHO_ERROR;
		} 
    msg_feedback(IIC_Check); 
}


char cho_wifi(void){
	
    unsigned char Tmp;
    unsigned char dir;
	  unsigned char State_check[2];
	  uint32_t u32_param[MAX_PARAM];
	  u8 *Tmp_1;
    u8 *Tmp_2;
	
		LoopList_GetOneData(&DataBuffer,&dir);

	  switch(dir){
		
			case 0xDA:
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  u32_param[0] = Tmp;
			
			  if(Tmp >0x02){    //参数校验
				return;
				}
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!=0xA1){
				return;
				}
				
				(*(u32(*)())usmart_nametab[25].func)( (u8)u32_param[0]);
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;
				pWifiRead_(State_check,2);
				
				break;
				
			case 0xDB:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0xFF) return ;   //隔离参数校验
			  
			  while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_1++ = Tmp;
				 }
		  	}
				
				while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_2++ = Tmp;
				 }
		  	}
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!=0xA1){
				return;
				}
				
				(*(u32(*)())usmart_nametab[26].func)( (char)Tmp_1,(char)Tmp_2);
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;
				pWifiRead_(State_check,2);
				
				break;
				
			case 0xDC:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0xFF) return ;   //隔离参数校验
			
			  while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_1++ = Tmp;
				 }
		  	}
				
				while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_2++ = Tmp;
				 }
		  	}
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
				u32_param[0] = Tmp;
				if(Tmp>0x04)return;
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!=0xA1){
				return;
				}
				(*(u32(*)())usmart_nametab[27].func)( (char)Tmp_1,(char)Tmp_2);	
				
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;
				pWifiRead_(State_check,2);				
				break;
				
		  case 0xDE:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  u32_param[0] = Tmp;
			  if(Tmp>0x01)return;
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0xFF) return ;   //隔离参数校验
			
			  while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_1++ = Tmp;
				 }
		  	}
				
			  while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_2++ = Tmp;
				 }
		  	}
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!=0xA1){
				return;
				}
				
			(*(u32(*)())usmart_nametab[30].func)( (u8)u32_param[0],(char)Tmp_1,(char)Tmp_2);
				
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;
				pWifiRead_(State_check,2);	
				
				break;
				
		  case 0xDF:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0x5D) return ;   //特殊参数校验
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!=0xA1){
				return;
				}
				
				(*(u32(*)())usmart_nametab[31].func)();
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_LinkStatus;
				pWifiRead_(State_check,2);
				ESP8266_LinkStatus =0x00;
				
				break;
				
			case 0xD9:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0x9D) return ;   //特殊参数校验
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				
				(*(u32(*)())usmart_nametab[32].func)();
				
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_ucIdLinkStatus;				
				pWifiRead_(State_check,2);
				ESP8266_ucIdLinkStatus = 0x00;
				
        break;
				
			case 0xD8:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0x8A) return ;   //特殊参数校验
			  
        LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				(*(u32(*)())usmart_nametab[33].func)();
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;				
				pWifiRead_(State_check,2);				
				
				break;
				
			case 0xD7:
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0x7C) return ;   //特殊参数校验
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				
				(*(u32(*)())usmart_nametab[34].func)();
				
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;				
				pWifiRead_(State_check,2);
			
				break;
				
			case 0xD6:
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0x6B) return ;   //特殊参数校验
			
			  LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				} 
			
				(*(u32(*)())usmart_nametab[35].func)();
			
				break;
				
			case 0xD5:   //数据返回WIFI方式调用函数  特殊
				
				LoopList_GetOneData(&DataBuffer,&Tmp);
			  u32_param[0] = Tmp;
			  if(Tmp>0x01)return;
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp > 0x05) return ;   
				u32_param[1] = Tmp;
				
				LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				} 
			
			(*(u32(*)())usmart_nametab[36].func)((u8)u32_param[0],(u8)u32_param[1]);
				return;    ////这里不在到后面有反馈值

        break;
				
			case 0xCA:
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
        if(Tmp!= 0x68) return ;   //特殊参数校验
			
				LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				
			(*(u32(*)())usmart_nametab[22].func)();
			
				break;	
				
			case 0xCB:
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
        if(Tmp!= 0x69) return ;   //特殊参数校验
			
				LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				
			(*(u32(*)())usmart_nametab[23].func)();
			
				break;	
				
	 	case 0xCC:
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
        if(Tmp!= 0x6A) return ;   //特殊参数校验
			
				LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				
			(*(u32(*)())usmart_nametab[24].func)();
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;				
				pWifiRead_(State_check,2);
				
				break;
			 case 0xCD:
				
			  LoopList_GetOneData(&DataBuffer,&Tmp);
        if(Tmp>0x02) return ;   
			  u32_param[0] = Tmp;
			 
				LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
				
		 	(*(u32(*)())usmart_nametab[28].func)((u8)u32_param[0]);
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;				
				pWifiRead_(State_check,2);
				
				break;	
				
		case 0xCE:
			  LoopList_GetOneData(&DataBuffer,&Tmp);
		    if(Tmp >0x01)return;
		    u32_param[0] = Tmp;
		
		     LoopList_GetOneData(&DataBuffer,&Tmp);
			  if(Tmp!= 0xFF) return ;   //隔离参数校验
		    
		     while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_1++ = Tmp;
				 }
		  	}
				 
				 while(1){
			   LoopList_GetOneData(&DataBuffer,&Tmp);
					
				 if(Tmp == 0xFF){
           break;					 
				 }else{
				  *Tmp_2++ = Tmp;
				 }
		  	}
				 
				LoopList_GetOneData(&DataBuffer,&Tmp);
		    if(Tmp >0x05)return;
		    u32_param[1] = Tmp;
				
				LoopList_GetOneData(&DataBuffer,&Tmp);			
			  if(Tmp!=0xA1){
				return;
				}
			(*(u32(*)())usmart_nametab[29].func)((u8)u32_param[0],(char)Tmp_1,(char)Tmp_2,(u8)u32_param[1]);
				
				State_check[0] = Wifi_Check_One;
				State_check[1] = ESP8266_STATE;				
				pWifiRead_(State_check,2);
				
        break;			
		}			
				
		 msg_feedback(Wifi_Check_Last);
     ESP8266_STATE = ESP8266_RIGHT;
		
}
