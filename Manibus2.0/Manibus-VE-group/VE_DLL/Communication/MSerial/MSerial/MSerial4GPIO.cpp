#include "stdafx.h"
#include "MSerial.h"

void MSerial::pPinInit(HANDLE hCom,GPIOIO_Type IO, GPIOPIN_Type Pin, GPIOMODE_Type Mode, GPIOSpeed_Type Speed) {

	std::pair<U16, U8*>params_;
	params_ = PinInit(IO, Pin, Mode, Speed);

	if (GPIO_RIGHT != GPIORIGHT) { printf("GPIO error about pPinInit in parameter %d !", GPIO_RIGHT); return; }

	SerialWrite(hCom,MGPIOSerialFeedBack(),3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == GPIO_FeedBack) break;
	}
}

void MSerial::pPinDeInit(HANDLE hCom, GPIOIO_Type IO) {


	std::pair<U16, U8*>params_;
	params_ = PinDeInit(IO);

	if (GPIO_RIGHT != GPIORIGHT) { printf("GPIO error about pPinDeInit in parameter %d !", GPIO_RIGHT); return; }
	SerialWrite(hCom, MGPIOSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == GPIO_FeedBack) break;

	}
}

void MSerial::pPinOutPut(HANDLE hCom, GPIOIO_Type IO, GPIOPIN_Type Pin, BitAction Level) {

	std::pair<U16, U8*>params_;
	params_ = PinOutPut(IO,Pin,Level);

	if (GPIO_RIGHT != GPIORIGHT) { printf("GPIO error about pPinOutPut in parameter %d !", GPIO_RIGHT); return; }

	SerialWrite(hCom, MGPIOSerialFeedBack(), 3);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack ) {

		U8 temp;
		SerialRead(hCom, &temp, 1);
		if (temp == GPIO_FeedBack) break;
	}

}

char MSerial::pPinReadBit(HANDLE hCom, GPIOIO_Type IO, GPIOPIN_Type Pin) {

	std::pair<U16, U8*>params_;
	U8 temp,temp2;

	params_ = PinReadBit(IO, Pin);

	if (GPIO_RIGHT != GPIORIGHT) { printf("GPIO error about pPinReadBit in parameter %d !", GPIO_RIGHT); return 0xFF; }


	SerialReadyRead(hCom);
	SerialWrite(hCom, params_.second, params_.first);

	while (SerialWaitFeedBack){
	    
		SerialRead(hCom, &temp, 1);

		if (temp == GPIO_Check) {

			SerialRead(hCom, &temp2, 1);
			SerialRead(hCom, &temp, 1);
			if (temp == GPIO_FeedBack) {
				return temp2;
			}
		}	
	}
	return 0xFF;
}















