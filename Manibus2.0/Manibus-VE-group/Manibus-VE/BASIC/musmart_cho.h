#include "LoopList.h"
#include "stm32f10x.h"
#include "basic_periphset.h"

#define   RecDataBufferMaxSize			500
#define   MAX_DATA_TRANSFORM 260

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
void cho_io(void);
void cho_usart(void);
void cho_adc(void);
void cho_pwm(void);
void cho_iic(void);
void cho_wifi(void);
