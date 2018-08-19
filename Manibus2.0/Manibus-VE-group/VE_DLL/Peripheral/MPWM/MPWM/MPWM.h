#include <iostream>
#include <utility>
#include <string>


#define MPWM_API __declspec(dllexport)

#define PWM_Check         0xFD

#define PWM_pPWMICGetPluseWid   0xDB
#define PWM_pPWMICGetFrequancy  0xDC
#define PWM_pPWMGetPluse        0xDD

#define pPWMInit__ID            0xD1
#define pPWMICInit__ID          0xD2
#define pPWMPluseInit__ID       0xD3
#define pPWMDeInit__ID          0xD4
#define pPWMSetPluseWid__ID     0xD5
#define pPWMICGetPluseWid__ID   0xD6
#define pPWMICGetFrequency__ID  0xD7
#define pPWMGetPluse__ID        0xD8
#define pPWMSetFrequency__ID    0xD9

#define PWMRIGHT          0xFD

typedef enum {
	
	TIM1 = 0x2C,
	TIM8 = 0x34,

	TIM2 = 0x00,
	TIM3 = 0x04,
	TIM4 = 0x08,
	TIM5 = 0x0C

}PWMTIM_Type;

typedef enum {

	PWMOC_Channel_1 = 0x01,
	PWMOC_Channel_2 = 0x02,
	PWMOC_Channel_3 = 0x03,
	PWMOC_Channel_4 = 0x04,
	OC_None = 0x00
}PWMOCCHANNEL_Type;

typedef enum {

	PWMIC_Channel_1 = 0x04,
	PWMIC_Channel_2 = 0x0C,
	IC_None = 0x00
}PWMICCHANNEL_Type;

typedef enum {

	PWMIC_GetValueHigh_Channel_1 = 0x11,
	PWMIC_GetValueLow_Channel_1 =  0x01,
	PWMIC_GetValueHigh_Channel_2 = 0x12,
	PWMIC_GetValueLow_Channel_2 =  0x02,


}PWMICVALUE_Type;

typedef enum {

	PWM_EncoderMode_TI1 = 0x01,
	PWM_EncoderMode_TI2 = 0x02,
	PWM_EncoderMode_TI12 = 0x03

}PWMENCODER_Type;


typedef enum {

	PWM_Encoder_Channel_1 = 0x00,
	PWM_Encoder_Channel_2 = 0x01,
	PWM_Encoder_Channel_3 = 0x02,
	PWM_Encoder_Channel_4 = 0x03,
	PWM_Encoder_Channel_5 = 0x04,
	PWM_Encoder_Channel_6 = 0x05,

}PWM_ENCODERCHANNEL_Type;


class MPWM_API MPWM
{

public:

	MPWM();
	~MPWM();

	virtual  std::pair<unsigned short, unsigned char*> PWMInit(PWMTIM_Type TIM, PWMOCCHANNEL_Type *Channel,
		unsigned short Period,unsigned short Prescalar,unsigned short Pluse);
	virtual  std::pair<unsigned short, unsigned char*> PWMICInit(PWMTIM_Type TIM, PWMICCHANNEL_Type *Channel);
	virtual  std::pair<unsigned short, unsigned char*> PWMPluseInit(PWMTIM_Type TIM, PWMENCODER_Type Mode, unsigned short Circle);
	virtual  std::pair<unsigned short, unsigned char*> PWMDeInit(PWMTIM_Type TIM);

	virtual  std::pair<unsigned short, unsigned char*> PWMSetPluseWid(PWMTIM_Type TIM, PWMOCCHANNEL_Type Channel,unsigned short PluseWid);

	virtual  std::pair<unsigned short, unsigned char*> PWMICGetPluseWid(PWMTIM_Type TIM, PWMICVALUE_Type Channel);
	virtual  std::pair<unsigned short, unsigned char*> PWMICGetFrequency(PWMTIM_Type TIM, PWMICCHANNEL_Type Channel);

	virtual  std::pair<unsigned short, unsigned char*> PWMGetPluse(PWMTIM_Type TIM, PWM_ENCODERCHANNEL_Type Channel);
	virtual  std::pair<unsigned short, unsigned char*> PWMSetFrequency(PWMTIM_Type TIM,unsigned short Prescaler);

	virtual std::pair<unsigned short, unsigned char*> MPWMSerialFeedBack();
	virtual std::pair<unsigned short, unsigned char*> MPWMWifiFeedBack(unsigned char ID);

	virtual bool PWM_FeedBack(unsigned char ID, unsigned char* Msg);
	virtual bool PWM_FeedBack(unsigned char ID, unsigned char* Msg, unsigned char* readback);


	unsigned char PWM_RIGHT = 0x00;

private:

	unsigned char *sendout_;

	bool PWM_ID_CHECK_ = false;
	bool PWM_PARAM_CHECK_ = false;
	bool PWM_CHECK_ = false;

	unsigned char continue_count = 0;

#define PWM_FBInit()    PWM_ID_CHECK_=false;PWM_PARAM_CHECK_= false;PWM_CHECK_=false;continue_count =0;

#define IS_TIM_LIST8_PERIPH(PERIPH)  (((PERIPH) == TIM1) || \
                                      ((PERIPH) == TIM2) || \
                                      ((PERIPH) == TIM3) || \
                                      ((PERIPH) == TIM4) || \
                                      ((PERIPH) == TIM5) || \
                                      ((PERIPH) == TIM8) ) 
                                      //((PERIPH) == TIM9) || \
                                      //((PERIPH) == TIM10)|| \
                                      //((PERIPH) == TIM11)|| \
                                      //((PERIPH) == TIM12)|| \
                                      //((PERIPH) == TIM13)|| \
                                      //((PERIPH) == TIM14)|| \
                                      //((PERIPH) == TIM15)|| \
                                      //((PERIPH) == TIM16)|| \
                                      //((PERIPH) == TIM17))
#define IS_TIM_OCCHANNEL(PERIPH)      (((PERIPH) == PWMOC_Channel_1) || \
                                      ((PERIPH) == PWMOC_Channel_2) || \
                                      ((PERIPH) == PWMOC_Channel_3) || \
                                      ((PERIPH) == PWMOC_Channel_4) || \
                                        (PERIPH) == OC_None)

#define IS_TIM_ICCHANNEL(PERIPH)       (((PERIPH) == PWMIC_Channel_1) || \
                                      ((PERIPH) == PWMIC_Channel_2) || \
                                        (PERIPH) == IC_None)
   
#define IS_TIM_ENCODERMODE(PERIPH)     (((PERIPH) == PWM_EncoderMode_TI1) || \
                                      ((PERIPH) == PWM_EncoderMode_TI2) || \
                                      ((PERIPH) == PWM_EncoderMode_TI12))

#define IS_TIM_ICGETVALUE(PERIPH)      (((PERIPH) == PWMIC_GetValueHigh_Channel_1) || \
                                      ((PERIPH) == PWMIC_GetValueLow_Channel_1) || \
                                      ((PERIPH) == PWMIC_GetValueHigh_Channel_2) || \
                                       ((PERIPH) == PWMIC_GetValueLow_Channel_2))

#define IS_TIM_ENCODERCHANNEL(PERIPH)  (((PERIPH) == PWM_Encoder_Channel_1) || \
	                                    ((PERIPH) == PWM_Encoder_Channel_2) || \
									    ((PERIPH) == PWM_Encoder_Channel_3) || \
									    ((PERIPH) == PWM_Encoder_Channel_4) || \
									    ((PERIPH) == PWM_Encoder_Channel_5) || \
										((PERIPH) == PWM_Encoder_Channel_6))

	bool IS_PARAMS(bool JUDGE);
	bool IS_RIGHT(unsigned char param1, unsigned char param2);
	void s_To_h(const unsigned short *val, unsigned char hex[2] );
};




