#include "stdafx.h"
#include "MSerial.h"

void MSerial::pADC_Init(HANDLE hCom, ADCSPEED_Type Speed, ADCCHANNEL_Type *Channel) {

	std::pair<U16, U8*>params_;
	params_ = ADC_Init(Speed, Channel);

	if (ADC_RIGHT != ADCRIGHT) { printf("ADC error about pADC_Init in parameter %d !", ADC_RIGHT); return; }

	SerialReadyRead(hCom);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == ADC_FeedBack) break;
	}
}

void MSerial::pADC_DeInit(HANDLE hCom) {

	std::pair<U16, U8*>params_;
	params_ = ADC_DeInit();

	if (ADC_RIGHT != ADCRIGHT) { printf("ADC error about pADC_DeInit in parameter %d !", ADC_RIGHT); return; }

	SerialReadyRead(hCom);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == ADC_FeedBack) break;

	}
}

float MSerial::pADC_Read(HANDLE hCom, ADCCHANNEL_Type Channel) {

	std::pair<U16, U8*>params_;
	params_ = ADC_Read( Channel);
	char temp2[4];

	float infoBack;

	if (ADC_RIGHT != ADCRIGHT) { printf("ADC error about pADC_Read in parameter %d !", ADC_RIGHT); return 0.0; }

	SerialReadyRead(hCom);
	SerialWrite(hCom, params_.second, params_.first);


	while (SerialWaitFeedBack){

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == ADC_Check) {

		SerialRead(hCom, temp2, 4);
		 infoBack = *((float *)temp2);
		 SerialRead(hCom, &temp, 1);

		 if (temp == ADC_FeedBack)return infoBack;

		}

	}

	return 0.0;
}