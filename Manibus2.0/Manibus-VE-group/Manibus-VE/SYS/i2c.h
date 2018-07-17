#include "stm32f10x.h"

#define IIC_MAXNUM  2
#define IIC_BUFFER  80
#define IIC_CodeNum_Max  80
#define IIC_START 0x01
#define IIC_STOP 0x02

#define IIC_ONE 0x00
#define IIC_TWO 0x01

#define High      1
#define Low       0

#define delay   {unsigned int i = 0 ; for(;i<500;++i);}

void SDA_(u8 n, u8 cho);
void SCL_(u8 n, u8 cho);

#define SDA_H(c)       SDA_(High,c)        
#define SDA_L(c)       SDA_(Low,c)          

#define SCL_H(c)       SCL_(High,c)          
#define SCL_L(c)       SCL_(Low,c)            


extern char iic_cho;
extern char SCL_cho;
extern char SDA_cho;
extern char iic_waitask_Succeed;
	
struct iic_func{
   void *func; 
};

extern struct iic_func iic_name[];


void pIICInit(uint32_t IO1,uint32_t SCL_Pin,uint32_t IO2, uint32_t SDA_Pin);

void SDA_Input(char cho);
void SDA_Output(char cho);


void IIC_Start(void);                  //0x01
void IIC_Stop(void);                   //0x02
void IIC_WaitAck(void);                //0x03
void IIC_SendAck(void);                //0x04
void IIC_SendNoAck(void);              //0x05 
void IIC_SendByte(unsigned char byte);   //0x06
void IIC_RecByte(void);         //0x08

void Queue_addData(char get);
void Queue_readAllDataes(void);




