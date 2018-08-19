#include <iostream>
#include <utility>
#include <string>

#define MADC_API __declspec(dllexport)

#define ADC_Check          0xFC

#define pADCInit__ID       0xC1
#define pADCDeInit__ID     0xC2
#define pADCRead__ID       0xC3

#define ADCRIGHT           0xFC




typedef enum {

	ADC_12MHz = 0x08,
	ADC_9MHz  = 0x0C

}ADCSPEED_Type;     //This is based on STM32F10X  with 72MHz

typedef enum {

	ADC_Channel_0 = 0x00,
	ADC_Channel_1 = 0x01,
	ADC_Channel_2 = 0x02,
	ADC_Channel_3 = 0x03,
	ADC_Channel_4 = 0x04,
	ADC_Channel_5 = 0x05,
	ADC_Channel_6 = 0x06,
	ADC_Channel_7 = 0x07,
	ADC_Channel_8 = 0x08,
	ADC_Channel_9 = 0x09,
	ADC_Channel_10 = 0x0A,
	ADC_Channel_11 = 0x0B,
	ADC_Channel_12 = 0x0C,
	ADC_Channel_13 = 0x0D,
	ADC_Channel_14 = 0x0E,
	ADC_Channel_15 = 0x0F,
	ADC_Channel_16 = 0x10,
	ADC_Channel_17 = 0x11,

}ADCCHANNEL_Type;


class MADC_API MADC {
	 

public:

	MADC();
	~MADC();

	virtual  std::pair<unsigned short, unsigned char*> ADCInit(ADCSPEED_Type Speed, ADCCHANNEL_Type *Channel);
	virtual  std::pair<unsigned short, unsigned char*> ADCDeInit();
	virtual  std::pair<unsigned short, unsigned char*> ADCRead(ADCCHANNEL_Type Channel);

	virtual  std::pair<unsigned short, unsigned char*> MADCSerialFeedBack();
	virtual  std::pair<unsigned short, unsigned char*> MADCWifiFeedBack(unsigned char ID);

	virtual bool ADC_FeedBack(unsigned char ID,unsigned char *Msg);
	virtual bool ADC_FeedBack(unsigned char ID,unsigned char *Msg, unsigned char* readback);


	unsigned char ADC_RIGHT = 0x00;

private:

	unsigned char *sendout_;

	bool ADC_ID_CHECK_ = false;
	bool ADC_PARAM_CHECK_ = false;
	bool ADC_CHECK_ = false;

	unsigned char continue_count = 0;

#define  ADC_FBInit()             ADC_ID_CHECK_ = false;ADC_CHECK_  = false;ADC_PARAM_CHECK_ = false;continue_count=0;

#define IS_ADC_SPEED(SPEED)          (((SPEED) == ADC_12MHz) || \
                                     ((SPEED) == ADC_9MHz))

#define IS_ADC_CHANNEL(CHANNEL)  (((CHANNEL) == ADC_Channel_0) || ((CHANNEL) == ADC_Channel_1) || \
                                 ((CHANNEL) == ADC_Channel_2) || ((CHANNEL) == ADC_Channel_3) || \
                                 ((CHANNEL) == ADC_Channel_4) || ((CHANNEL) == ADC_Channel_5) || \
                                 ((CHANNEL) == ADC_Channel_6) || ((CHANNEL) == ADC_Channel_7) || \
                                 ((CHANNEL) == ADC_Channel_8) || ((CHANNEL) == ADC_Channel_9) || \
                                 ((CHANNEL) == ADC_Channel_10) || ((CHANNEL) == ADC_Channel_11) || \
                                 ((CHANNEL) == ADC_Channel_12) || ((CHANNEL) == ADC_Channel_13) || \
                                 ((CHANNEL) == ADC_Channel_14) || ((CHANNEL) == ADC_Channel_15) || \
                                 ((CHANNEL) == ADC_Channel_16) || ((CHANNEL) == ADC_Channel_17))



	bool IS_PARAMS(bool JUDGE);
	bool IS_RIGHT(unsigned char param1,unsigned char param2);
};







