#include "LoopList.h"
#include "stm32f10x.h"
#include "basic_periphset.h"

#define   RecDataBufferMaxSize			500
#define   MAX_DATA_TRANSFORM 260

#define   CHO_ERROR    0x00
#define   CHO_RIGHT    0x01

extern LoopListStruct DataBuffer;

struct _m_usmart_nametab{
   void *func;
};

extern struct _m_usmart_nametab usmart_nametab[];	

//初始化队列
void Bytes_BUS_RecInit(void);
//主线程执行函数
void Msg_Filtrate(void);

//主线程选择函数
u32 read_addr(u32 addr);
void write_addr(u32 addr,u32 val);

char cho_io(void);
char cho_usart(void);
char cho_adc(void);
char cho_pwm(void);
char cho_iic(void);
char cho_wifi(void);
