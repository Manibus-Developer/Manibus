#include "i2c.h"
#include "basic_periphset.h"
#include "musmart.h"
#include "mpdu.h"

uint32_t IO_Sum[2*IIC_MAXNUM] ={0};
uint32_t Pin_Sum[2*IIC_MAXNUM] ={0};


char iic_count=0;
char iic_cho =0;
char SCL_cho =0;
char SDA_cho =0;

char iic_waitask_Succeed = 0x01;
char iic_error  = 0x00;

void judge_iic(uint32_t IO1,uint32_t SCL_Pin_,uint32_t IO2, uint32_t SDA_Pin_){   
	 
	  IO_Sum[iic_count] = IO1;
    IO_Sum[iic_count+1] = IO2;		 
	  Pin_Sum[iic_count] = SCL_Pin_;
	  Pin_Sum[iic_count+1] = SDA_Pin_;  
	  iic_count +=2;
}

void pIICInit(uint32_t IO1,uint32_t SCL_Pin_,uint32_t IO2, uint32_t SDA_Pin_){

	   GPIO_InitTypeDef GPIO_InitStructure;
     ClockCmd(IO1);
	   ClockCmd(IO2);
	   GPIO_InitStructure.GPIO_Pin = (uint16_t)(SCL_Pin_);
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	   GPIO_Init((GPIO_TypeDef*)IO1,&GPIO_InitStructure);
	
		 GPIO_InitStructure.GPIO_Pin = (uint16_t)(SDA_Pin_);
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	   GPIO_Init((GPIO_TypeDef*)IO2,&GPIO_InitStructure);
	
	
	   ((GPIO_TypeDef*)IO1)->BSRR=(uint16_t)SCL_Pin_;       //都设置为高电平
	   ((GPIO_TypeDef*)IO2)->BSRR=(uint16_t)SDA_Pin_; 
	   
	   judge_iic(IO1,SCL_Pin_,IO2,SDA_Pin_);
}

void SDA_Input(char cho){
	
    GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = Pin_Sum[cho];
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
    GPIO_Init((GPIO_TypeDef*)IO_Sum[cho],&GPIO_InitStructure);
}

void SDA_Output(char cho){

    GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = Pin_Sum[cho];
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_Init((GPIO_TypeDef*)IO_Sum[cho],&GPIO_InitStructure);
}

char SDA_ReadIn(){

	uint8_t bitstatus = 0x00;
	
	 if (((((GPIO_TypeDef*) (IO_Sum[SDA_cho]))->IDR & Pin_Sum[SDA_cho])) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

void SDA_(u8 n, u8 cho){
	
    n != Low ? (((GPIO_TypeDef*) (IO_Sum[cho]))->BSRR = Pin_Sum[cho]):
               (((GPIO_TypeDef*) (IO_Sum[cho]))->BRR = Pin_Sum[cho]);
}
void SCL_(u8 n, u8 cho){
	
    n != Low ? (((GPIO_TypeDef*) (IO_Sum[cho]))->BSRR = Pin_Sum[cho]):
      	       (((GPIO_TypeDef*) (IO_Sum[cho]))->BRR = Pin_Sum[cho]);
}


void IIC_Start(void){
	 
	if(iic_waitask_Succeed){
	 SDA_H(SDA_cho);delay;
	 SCL_H(SCL_cho);delay;
	 SDA_L(SDA_cho);delay;
	 SCL_L(SCL_cho);delay;
	}

}

void IIC_Stop(void){
	
   if(iic_waitask_Succeed){
		 
	 SCL_L(SCL_cho);delay;
	 SDA_L(SDA_cho);delay;
	 SCL_H(SCL_cho);delay;
	 SDA_H(SDA_cho);delay;
	 
	 }

}

void IIC_WaitAck(void){
    
	 if(iic_waitask_Succeed){
		 
	 u8 ErrorTime = 5;
	 SDA_Input(SDA_cho);
	 delay;
	 SCL_H(SCL_cho);delay;
	 while(SDA_ReadIn()){
		 
	   ErrorTime--;
		 delay;
		 if(0 ==ErrorTime){
			 
		    SDA_Output(SDA_cho);
			  IIC_Stop();
			  iic_waitask_Succeed = 0x00;
			  return;	 
		 }	 
	 }
	 
	 SDA_Output(SDA_cho);
	 SCL_L(SCL_cho);delay;
	 iic_waitask_Succeed = 0x01;
   return;
	 }
}

void IIC_SendAck(void){
	
  if(iic_waitask_Succeed){
		
	SDA_L(SDA_cho);delay;
	delay;
	SCL_H(SCL_cho);delay;
	SCL_L(SCL_cho);delay;
		
	}
}

void IIC_SendNoAck(void){
	 
	 if(iic_waitask_Succeed){
		 
   SDA_H(SDA_cho);delay;
	 delay;
	 SCL_H(SCL_cho);delay;
	 SCL_L(SCL_cho);delay;
		 
	}
}

void IIC_SendByte(unsigned char byte){
   
	unsigned char t = 8;
	
	if(iic_waitask_Succeed){
		
	 for(;t>0;t--){
	   SCL_L(SCL_cho);delay;
	   SDA_(byte & 0x80,SDA_cho);delay;
		 byte+=byte;
		 delay;
		 SCL_H(SCL_cho);delay;
	 }
   SCL_L(SCL_cho);delay;
 }
}

void IIC_RecByte(void){
	
   unsigned char t = 8;
   unsigned char RevByte = 0x00;
	
if(iic_waitask_Succeed){
	
		SDA_Input(SDA_cho);
	
	 for(;t>0;t--){
	   RevByte+=RevByte;
		 SCL_L(SCL_cho);delay;
		 delay;
		 SCL_H(SCL_cho);delay;
		 RevByte|= SDA_ReadIn();
	 }
	 SCL_L(SCL_cho);delay;
	 SDA_Output(SDA_cho);
	 
	 Queue_addData(RevByte);
	 
	 }
}

unsigned char Queue_Size[2][IIC_BUFFER]={0};
unsigned short Queue_readPoint[2] ={0};
unsigned short Queue_writePoint[2] ={0};
unsigned short Queue_dataSize[2]={0};


void Queue_addData(char get){

   if(Queue_writePoint[iic_cho]<IIC_BUFFER){	
		 
	 Queue_Size[iic_cho][Queue_writePoint[iic_cho]] = get;		 
		 
	 }else{ 
		 
	 Queue_writePoint[iic_cho] = 0;
	 Queue_Size[iic_cho][Queue_writePoint[iic_cho]] = get;
		 
	 }
	 
	 Queue_writePoint[iic_cho]++;
	 Queue_dataSize[iic_cho]++;
	 
}

uint8_t Queue_readData(void){
	
	  char tmp;
    if(Queue_dataSize>0){
      
			if( Queue_readPoint[iic_cho] < IIC_BUFFER){
				
			tmp = Queue_Size[iic_cho][Queue_readPoint[iic_cho]];
				
			}else{
				
			Queue_readPoint[iic_cho] =0;
			tmp = Queue_Size[iic_cho][Queue_readPoint[iic_cho]];	
	
			}
			
			Queue_readPoint[iic_cho]++;
      Queue_dataSize[iic_cho]--;	
			
			return tmp;
			
     }else{
		 //iic_error = 0x01|(iic_cho<<4);  //iic无数据  前四位表示 iic编号 后四位1表示错误
		 return 0x00;
		}
}

void Queue_readAllDataes(void){
    
	  u8 temp;
	  u16 temp2 = Queue_dataSize[iic_cho]+4;
		unsigned char iic_data_buffer[259];
	
	  unsigned short paramsCheck =0;  
	  
	  if(Queue_dataSize[iic_cho] == 0)return;

	   iic_data_buffer[0] = pIICFunc__ID;
	   iic_data_buffer[1] = (uint8_t)iic_cho;
	   iic_data_buffer[2] = (uint8_t)Queue_dataSize[iic_cho];		
		
	  for(temp=0;temp<temp2-1;temp++){
			
		paramsCheck += iic_data_buffer[temp];
			
		if(temp>=3){
				iic_data_buffer[temp] = Queue_readData();
			}
		}
		
		iic_data_buffer[temp2-1] = paramsCheck % 0xFF;
		
     Usart_ReadArray_(iic_data_buffer,temp2);	  
}

