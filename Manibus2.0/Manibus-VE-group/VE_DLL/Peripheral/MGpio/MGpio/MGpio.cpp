#include "stdafx.h"
#include "MGpio.h"


#define GPIO_pPinInit                   0xCA
#define GPIO_pPinDeInit                 0xAA
#define GPIO_pPinOutPut                 0xDA
#define GPIO_pPinReadBit                0xDB
#define GPIO_CheckLast                  0xA1


std::pair<unsigned short, unsigned char*> MGPIO::PinInit(GPIOIO_Type IO, GPIOPIN_Type Pin, GPIOMODE_Type Mode, GPIOSpeed_Type Speed) {

	unsigned char *sendout_;
	unsigned short length_ = 7;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	GPIO_RIGHT = GPIORIGHT;

	sendout_[0] = GPIO_Check;
	sendout_[1] = GPIO_pPinInit;

	sendout_[2] = IO;
	if (!IS_PARAMS(IS_GPIO_ALL_PERIPH(IO))) { GPIO_RIGHT = 0x01;  return { length_ ,sendout_ }; 
	}

	sendout_[3] = Pin;
	if (!IS_PARAMS(IS_GET_GPIO_PIN(Pin))) { GPIO_RIGHT = 0x02;  return { length_ ,sendout_ };
	}

	sendout_[4] = Mode;
	if (!IS_PARAMS(IS_GPIO_MODE(Mode))) { GPIO_RIGHT = 0x03; return { length_ ,sendout_ };
	}

	sendout_[5] = Speed;
	if (!IS_PARAMS(IS_GPIO_SPEED(Speed))) { GPIO_RIGHT = 0x04;  return { length_ ,sendout_ };
	}

	sendout_[6] = GPIO_CheckLast;

	return { length_ ,sendout_ };

}


std::pair<unsigned short, unsigned char*> MGPIO::PinDeInit(GPIOIO_Type IO) {

	unsigned char *sendout_;
	unsigned short length_ = 4;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	GPIO_RIGHT = GPIORIGHT;

	sendout_[0] = GPIO_Check;
	sendout_[1] = GPIO_pPinDeInit;

	sendout_[2] = IO;
	if (!IS_PARAMS(IS_GPIO_ALL_PERIPH(IO))) { GPIO_RIGHT = 0x01; return { length_ ,sendout_ };
	}

	sendout_[3] = GPIO_CheckLast;

	return { length_,sendout_ };
}

std::pair<unsigned short, unsigned char*> MGPIO::PinOutPut(GPIOIO_Type IO, GPIOPIN_Type Pin, BitAction Level) {

	unsigned char *sendout_;
	unsigned short length_ = 6;
	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	 GPIO_RIGHT = GPIORIGHT;
	 sendout_[0] = GPIO_Check;
	 sendout_[1] = GPIO_pPinOutPut;

	 sendout_[2] = IO;
	 if (!IS_PARAMS(IS_GPIO_ALL_PERIPH(IO))) { GPIO_RIGHT = 0x01; return { length_,sendout_ };
	 }

	 sendout_[3] = Pin;
	 if (!IS_PARAMS(IS_GET_GPIO_PIN(Pin))) { GPIO_RIGHT = 0x02; return { length_,sendout_ };
	 }

	 sendout_[4] = Level;
	 if (!IS_PARAMS(IS_GPIO_BIT_ACTION(Level))) { GPIO_RIGHT = 0x03;  return { length_,sendout_ };
	 }

	 sendout_[5] = GPIO_CheckLast;

	 return { length_,sendout_ };
}

std::pair<unsigned short, unsigned char*> MGPIO::PinReadBit(GPIOIO_Type IO, GPIOPIN_Type Pin) {

	unsigned char *sendout_;
	unsigned short length_ = 5;
	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	GPIO_RIGHT = GPIORIGHT;

	sendout_[0] = GPIO_Check;
	sendout_[1] = GPIO_pPinReadBit;

	sendout_[2] = IO;
	if (!IS_PARAMS(IS_GPIO_ALL_PERIPH(IO))) { GPIO_RIGHT = 0x01; return { length_,sendout_ };
	}

	sendout_[3] = Pin;
	if (!IS_PARAMS(IS_GET_GPIO_PIN(Pin))) { GPIO_RIGHT = 0x02;  return { length_,sendout_ };
	}

	sendout_[4] = GPIO_CheckLast;

	return { length_,sendout_ };
 }

unsigned char* MGPIO::MGPIOSerialFeedBack() {

	unsigned char *sendout_;
	unsigned short length_ = 3;
	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	//sendout_[0] = 0xFA;
	//sendout_[1] = 0xDC;
	//sendout_[2] = 0xA1;

	*sendout_-- = 0xA1;
	*sendout_-- = 0xDC;
	*sendout_   = 0xFA;

	return sendout_;
}

unsigned char* MGPIO::MGPIOWifiFeedBack(unsigned char ID) {

	unsigned char *sendout_;
	unsigned short length_ = 5;
	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	*sendout_-- = 0xA1;
	*sendout_-- = ID;
	*sendout_-- = 0x01;
	*sendout_-- = 0xD5;
	*sendout_ = 0xF9;

	return sendout_;
}


bool MGPIO::IS_PARAMS(bool JUDGE) {
	if (JUDGE) return true; else return false;
}

