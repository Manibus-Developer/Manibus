#include "stdafx.h"
#include <iostream>
#include <utility>
#include <string>

#define MGPIO_API  __declspec(dllexport)

#define GPIO_Check				   0xFE
#define pPinInit__ID               0xE1
#define pPinDeInit__ID             0xE2
#define pPinOutPut__ID             0xE3
#define pPinReadBit__ID            0xE4

#define GPIORIGHT                  0xFE

typedef enum {

	GPIOA = 0x08,
	GPIOB = 0x0C,
	GPIOC = 0x10,
	GPIOD = 0x14,
	GPIOE = 0x18,
	GPIOF = 0x1C,
	GPIOG = 0x20

}GPIOIO_Type;

typedef enum {

	GPIO_Pin_0 = 0x01,
	GPIO_Pin_1 = 0x02,
	GPIO_Pin_2 = 0x04,
	GPIO_Pin_3 = 0x08,
	GPIO_Pin_4 = 0x41,
	GPIO_Pin_5 = 0x42,
	GPIO_Pin_6 = 0x44,
	GPIO_Pin_7 = 0x48,
	GPIO_Pin_8 = 0x81,
	GPIO_Pin_9 = 0x82,
	GPIO_Pin_10 = 0x84,
	GPIO_Pin_11 = 0x88,
	GPIO_Pin_12 = 0xC1,
	GPIO_Pin_13 = 0xC2,
	GPIO_Pin_14 = 0xC4,
	GPIO_Pin_15 = 0xC8,


}GPIOPIN_Type;

typedef enum {

	GPIO_Mode_AIN = 0x00,
	GPIO_Mode_IN_FLOATING = 0x04,
	GPIO_Mode_IPD = 0x28,
	GPIO_Mode_IPU = 0x48,
	GPIO_Mode_Out_OD = 0x14,
	GPIO_Mode_Out_PP = 0x10,
	GPIO_Mode_AF_OD = 0x1C,
	GPIO_Mode_AF_PP = 0x18

}GPIOMODE_Type;

typedef enum
{
	GPIO_Speed_10MHz = 1,
	GPIO_Speed_2MHz,
	GPIO_Speed_50MHz

}GPIOSpeed_Type;


typedef enum
{
	Bit_LOW= 0,
	Bit_HIGH
}BitAction;

class  MGPIO_API MGPIO {

public:

	 MGPIO();
	 ~MGPIO();

	 virtual std::pair<unsigned short, unsigned char*> PinInit(GPIOIO_Type IO, GPIOPIN_Type Pin, GPIOMODE_Type Mode, GPIOSpeed_Type Speed);
	 virtual std::pair<unsigned short, unsigned char*> PinDeInit(GPIOIO_Type IO);
	 virtual std::pair<unsigned short, unsigned char*> PinOutPut(GPIOIO_Type IO, GPIOPIN_Type Pin, BitAction Level);
	 virtual std::pair<unsigned short, unsigned char*> PinReadBit(GPIOIO_Type IO, GPIOPIN_Type Pin);

	 virtual std::pair<unsigned short, unsigned char*> MGPIOSerialFeedBack();
	 virtual std::pair<unsigned short, unsigned char*> MGPIOWifiFeedBack(unsigned char ID);

	 virtual bool GPIO_FeedBack(unsigned char ID, unsigned char *Msg);
	 virtual bool GPIO_FeedBack(unsigned char ID, unsigned char *Msg,unsigned char * readback);


	 unsigned char GPIO_RIGHT = 0x00;

private:

	 unsigned char *sendout_;

	 bool GPIO_ID_CHECK_ = false;
	 bool GPIO_PARAM_CHECK_ = false;
	 bool GPIO_CHECK_ = false;

	 unsigned char continue_count = 0;

#define  GPIO_FBInit()             GPIO_ID_CHECK_ = false;GPIO_CHECK_  = false;GPIO_PARAM_CHECK_ = false;continue_count=0;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_LOW) || ((ACTION) == Bit_HIGH))

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))


#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG))


	bool IS_PARAMS(bool JUDGE);
	bool IS_RIGHT(unsigned char param1, unsigned char param2);
};
