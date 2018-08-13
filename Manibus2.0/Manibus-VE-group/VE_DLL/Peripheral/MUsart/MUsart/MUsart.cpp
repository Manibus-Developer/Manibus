#include "stdafx.h"
#include "MUsart.h"

#define USART_pUsartInit     0xCA
#define USART_pUsartDeInit   0xAA
#define USART_pUsartWrite    0xDA
#define USART_pUsartRead     0xDB
#define USART_pUsartOnLine   0xDC



std::pair<unsigned short, unsigned char*> MUSART::UsartInit(USART_Type Usart, USARTBAUD_Type Baud,
	USARTWORDLEN_Type WordLength, USARTSTOPBIT_Type StopBits, USARTPARITY_Type Parity,
	USARTHFC_Type HFC, USARTMODE_Type Mode,unsigned char MsgMode) {

	unsigned char *sendout_;
	unsigned short length_ = 12;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	USART_RIGHT = UARTRIGHT;

	sendout_[0] = USART_Check;
	sendout_[1] = USART_pUsartInit;

	sendout_[2] = Usart;
	if (!IS_PARAMS(IS_USART_1234_PERIPH(Usart))) { USART_RIGHT = 0x01; return { length_, sendout_ }; }

	sendout_[3] = Baud;
	if (!IS_PARAMS(IS_USART_BAUD(Baud))) { USART_RIGHT = 0x02; return { length_, sendout_ }; }

	sendout_[4] = WordLength;
	if (!IS_PARAMS(IS_USART_WORD_LENGTH(WordLength))) { USART_RIGHT = 0x03; return { length_, sendout_ }; }

	sendout_[5] = StopBits;
	if (!IS_PARAMS(IS_USART_STOPBITS(StopBits))) { USART_RIGHT = 0x04; return { length_, sendout_ }; }

	sendout_[6] = Parity;
	if (!IS_PARAMS(IS_USART_PARITY(Parity))) { USART_RIGHT = 0x05; return { length_, sendout_ }; }

	sendout_[7] = HFC;
	if (!IS_PARAMS(IS_USART_HARDWARE_FLOW_CONTROL(HFC))) { USART_RIGHT = 0x06; return { length_, sendout_ }; }

	sendout_[8] = Mode;
	if (!IS_PARAMS(IS_USART_MODE(Mode))) { USART_RIGHT = 0x07; return { length_, sendout_ }; }

	sendout_[9] = MsgMode;

	short temp = 0;

	for (char i = 2; i < 10; i++) {

		temp += sendout_[i];
	}

	sendout_[10] = temp % 0xFF;


	sendout_[11] = USART_pUsartInit__ID;

	return { length_,sendout_ };
}

std::pair<unsigned short, unsigned char*> MUSART::UsartDeInit(USART_Type Usart) {

	unsigned char *sendout_;
	unsigned short length_ = 5;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	USART_RIGHT = UARTRIGHT;

	sendout_[0] = USART_Check;
	sendout_[1] = USART_pUsartDeInit;

	sendout_[2] = Usart;
	if (!IS_PARAMS(IS_USART_1234_PERIPH(Usart))) { USART_RIGHT = 0x01; return { length_, sendout_ }; }

	sendout_[3] = sendout_[2] % 0xFF;

	sendout_[4] = USART_pUsartDeInit__ID;

	return { length_,sendout_ };
}

std::pair<unsigned short,unsigned char*> MUSART::UsartWrite(USART_Type Usart, unsigned char * Words, unsigned char Length) {

	
	unsigned char *sendout_;
	unsigned short length_ = 7 + Length;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	USART_RIGHT = UARTRIGHT;    	

	sendout_[0] = USART_Check;
	sendout_[1] = USART_pUsartWrite;

	sendout_[2] = Usart;
	if (!IS_PARAMS(IS_USART_1234_PERIPH(Usart))) { USART_RIGHT = 0x01; return { length_, sendout_ }; }
	sendout_[3] = Length;

	sendout_[4] = (Usart + Length) % 0xFF;

	memcpy(sendout_+5, Words, Length);

	short temp = 0;

	for (char i = 0; i < Length; i++) {

		temp += Words[i];
	}

	sendout_[length_ - 2] = temp % 0xFF;
	
	sendout_[length_ - 1] = USART_pUsartWrite__ID;

	return { length_,sendout_ };

}

std::pair<unsigned short, unsigned char*> MUSART::UsartRead(USART_Type Usart) {

	unsigned char *sendout_;
	unsigned short length_ = 5;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	USART_RIGHT = UARTRIGHT;

	sendout_[0] = USART_Check;
	sendout_[1] = USART_pUsartRead;

	sendout_[2] = Usart;
	if (!IS_PARAMS(IS_USART_1234_PERIPH(Usart))) { USART_RIGHT = 0x01; return { length_, sendout_ }; }

	sendout_[3] = sendout_[2] % 0xFF;

	sendout_[4] = USART_pUsartRead__ID;

	return { length_,sendout_ };

}

unsigned char* MUSART::UsartOnLine() {

	unsigned char *sendout_;
	unsigned short length_ = 3;

	sendout_ = (unsigned char*)malloc(length_ * sizeof(unsigned char));

	sendout_[0] = USART_Check;
	sendout_[1] = USART_pUsartOnLine;
	sendout_[2] = USART_pUsartOnLine__ID;

	return sendout_;
}




bool MUSART::IS_PARAMS(bool JUDGE) {
	if (JUDGE) return true; else return false;
}

void MUSART::s_To_h(const unsigned short *val, char hex[2]) {

	const char *p = (char*)(val);
	hex[0] = *((char*)p);
	hex[1] = *((char*)(p + 1));

}

