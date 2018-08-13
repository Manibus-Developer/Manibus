#include <iostream>
#include <utility>
#include <string>

#define MUSART_API __declspec(dllexport)

#define USART_Check        0xFA
#define USART_FeedBack     0xA2


#define USART_pUsartInit__ID      0xA1
#define USART_pUsartDeInit__ID    0xA2
#define USART_pUsartWrite__ID     0xA3
#define USART_pUsartRead__ID      0xA4
#define USART_pUsartOnLine__ID    0xA5

#define UARTRIGHT           0xFA

typedef enum {

	USART2 = 0x44,
	UART4 = 0x4C

}USART_Type;

typedef enum {

	Baud_9600 = 0x01,
	Baud_56000 = 0x02,
	Baud_115200 = 0x03

}USARTBAUD_Type;


typedef enum {

	USART_WordLength_8b = 0x00,
	USART_WordLength_9b = 0x10

}USARTWORDLEN_Type;

typedef enum {

	USART_StopBits_1 =    0x00,
	USART_StopBits_0_5 =  0x10,
	USART_StopBits_2 =    0x20,
	USART_StopBits_1_5 =  0x30

}USARTSTOPBIT_Type;

typedef enum {

	USART_Parity_No =    0x00,
	USART_Parity_Even =  0x04,
	USART_Parity_Odd =   0x06

}USARTPARITY_Type;

typedef enum {

	USART_HardwareFlowControl_None =   0x00,
	USART_HardwareFlowControl_RTS =    0x01,
	USART_HardwareFlowControl_CTS =    0x02,
	USART_HardwareFlowControl_RTS_CTS =0x03

}USARTHFC_Type;

typedef enum {
	USART_Mode_Rx = 0x04,
	USART_Mode_Tx = 0x08,
	USART_Mode_Tx_Rx = 0x0C

}USARTMODE_Type;

class MUSART {

public:

	virtual MUSART_API std::pair<unsigned short, unsigned char*> UsartInit(USART_Type Usart, USARTBAUD_Type Baud,
		USARTWORDLEN_Type WordLength, USARTSTOPBIT_Type StopBits, USARTPARITY_Type Parity,
		USARTHFC_Type HFC, USARTMODE_Type Mode, unsigned char MsgMode);

	virtual MUSART_API std::pair<unsigned short, unsigned char*> UsartDeInit(USART_Type Usart);
	virtual MUSART_API std::pair<unsigned short, unsigned char*> UsartWrite(USART_Type Usart, unsigned char * Words,unsigned char Length);
	virtual MUSART_API std::pair<unsigned short, unsigned char*> UsartRead(USART_Type Usart);

	virtual MUSART_API unsigned char* UsartOnLine();

	unsigned char USART_RIGHT = 0x00;

private:

#define IS_USART_1234_PERIPH(PERIPH) (((PERIPH) == USART2) || \
                                 ((PERIPH) == UART4))

#define IS_USART_BAUD(BAUD) (((BAUD) == Baud_9600) || \
                                 ((BAUD) == Baud_56000) || \
                                 ((BAUD) == Baud_115200))

#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))

#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))

#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_CTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

#define IS_USART_MODE(MODE) (((MODE) == USART_Mode_Rx) || \
                                 ((MODE) == USART_Mode_Tx) || \
                                 ((MODE) == USART_Mode_Tx_Rx))
	bool IS_PARAMS(bool JUDGE);
	void s_To_h(const unsigned short *val , char hex[2]);
};

