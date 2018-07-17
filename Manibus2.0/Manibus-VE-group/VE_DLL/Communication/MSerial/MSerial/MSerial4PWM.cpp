#include "stdafx.h"
#include "MSerial.h"


void MSerial::pPWMInit(HANDLE hCom, PWMTIM_Type TIM, PWMOCCHANNEL_Type *Channel,
	unsigned short Period, unsigned short Prescalar, unsigned short Pluse) {

	std::pair<U16, U8*>params_;

	params_ = PWMInit(TIM, Channel, Period, Prescalar, Pluse);
	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMInit in parameter %d !", PWM_RIGHT); return; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == PWM_FeedBack) break;
	}
}

void MSerial::pPWMICInit(HANDLE hCom, PWMTIM_Type TIM, PWMICCHANNEL_Type *Channel) {

	std::pair<U16, U8*>params_;
	params_ = PWMICInit(TIM, Channel);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMICInit in parameter %d !", PWM_RIGHT); return; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == PWM_FeedBack) break;

	}
}

void MSerial::pPWMPluseInit(HANDLE hCom, PWMTIM_Type TIM, PWMENCODER_Type Mode, unsigned short Circle) {

	std::pair<U16, U8*>params_;

	params_ = PWMPluseInit(TIM, Mode, Circle);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMPluseInit in parameter %d !", PWM_RIGHT); return; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {
		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == PWM_FeedBack) break;
	}
}

void MSerial::pPWMDeInit(HANDLE hCom, PWMTIM_Type TIM) {

	std::pair<U16, U8*>params_;

	params_ = PWMDeInit(TIM);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMDeInit in parameter %d !", PWM_RIGHT); return; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {
		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == PWM_FeedBack) break;
	}
}

void MSerial::pPWMSetPluseWid(HANDLE hCom, PWMTIM_Type TIM, PWMOCCHANNEL_Type Channel, unsigned short PluseWid) {

	std::pair<U16, U8*>params_;

	params_ = PWMSetPluseWid(TIM, Channel, PluseWid);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMSetPluseWid in parameter %d !", PWM_RIGHT); return; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {
		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == PWM_FeedBack) break;
	}

}

unsigned int MSerial::pPWMICGetPluseWid(HANDLE hCom,PWMTIM_Type TIM, PWMICVALUE_Type Channel) {

	std::pair<U16, U8*>params_;
	U8 temp;
	U8 temp2[4];
	unsigned int infoBack;

	params_ = PWMICGetPluseWid(TIM, Channel);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMICGetPluseWid in parameter %d !", PWM_RIGHT); return 0; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {


		SerialRead(hCom, &temp, 1);
		if (temp == PWM_Check) {
			SerialRead(hCom, &temp, 1);
			if (temp == PWM_pPWMICGetPluseWid) {
				SerialRead(hCom, temp2, 4);
				infoBack = *((unsigned int *)temp2);
				SerialRead(hCom, &temp, 1);
			if(temp == PWM_FeedBack)return infoBack;
			}

		}
	}
	return 0;
}

unsigned int MSerial::pPWMICGetFrequency(HANDLE hCom, PWMTIM_Type TIM, PWMICCHANNEL_Type Channel) {


	std::pair<U16, U8*>params_;
	U8 temp;
	U8 temp2[4];
	unsigned int infoBack;

	params_ = PWMICGetFrequency(TIM, Channel);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMICGetFrequency in parameter %d !", PWM_RIGHT); return 0; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {


		SerialRead(hCom, &temp, 1);
		if (temp == PWM_Check) {
			SerialRead(hCom, &temp, 1);
			if (temp == PWM_pPWMICGetFrequancy) {
				SerialRead(hCom, temp2, 4);
				infoBack = *((unsigned int *)temp2);
				SerialRead(hCom, &temp, 1);
				if (temp == PWM_FeedBack)return infoBack;
			}
		}
	}
	return 0;

}

unsigned int MSerial::pPWMGetPluse(HANDLE hCom, PWMTIM_Type TIM, PWM_ENCODERCHANNEL_Type Channel) {

	std::pair<U16, U8*>params_;
	U8 temp;
	U8 temp2[4];
	unsigned int infoBack;

	params_ = PWMGetPluse(TIM, Channel);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMGetPluse in parameter %d !", PWM_RIGHT); return 0; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		SerialRead(hCom, &temp, 1);
		if (temp == PWM_Check) {
			SerialRead(hCom, &temp, 1);
			if (temp == PWM_pPWMGetPluse) {
				SerialRead(hCom, temp2, 4);
				infoBack = *((unsigned int *)temp2);
				SerialRead(hCom, &temp, 1);
				if (temp == PWM_FeedBack)return infoBack;
			}
		}
	}
	return 0;
}

void MSerial::pPWMSetFrequency(HANDLE hCom, PWMTIM_Type TIM, unsigned short Prescaler) {

	std::pair<U16, U8*>params_;
	params_ = PWMSetFrequency(TIM, Prescaler);

	if (PWM_RIGHT != PWMRIGHT) { printf("MPWM error about pPWMSetFrequency in parameter %d !", PWM_RIGHT); return; }

	SerialWrite(hCom, MPWMSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {
		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == PWM_FeedBack) break;
	}

}




