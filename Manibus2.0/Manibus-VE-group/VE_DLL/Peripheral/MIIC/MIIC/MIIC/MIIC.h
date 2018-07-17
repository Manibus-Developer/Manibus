#include <iostream>
#include <utility>
#include <string>

#define MIIC_API __declspec(dllexport)

#define IIC_Check         0xFB
#define IIC_FeedBack      0xA5

#define IIC_NUMMBER       0x02

typedef enum {

	IIC_Num_1 = 0x00,
	IIC_Num_2 = 0x01

}IICNUM_Type;

typedef enum {

	IIC_Start = 0x01,
	IIC_Stop = 0x02,
	IIC_WaitAck = 0x03,
	IIC_SendAck = 0x04,
	IIC_SendNoAck = 0x05,
	IIC_SendByte  = 0x06,
	IIC_RecByte   = 0x07,

	IIC_SB_WA = 0x63,
	IIC_RB_WA = 0x73

}IICFUNC_Type;

class MIIC {

public:

	virtual MIIC_API std::pair<char, char*> IICInit(IICNUM_Type IIC);
	virtual MIIC_API std::pair<char, char*> IICFunc(IICNUM_Type IIC, IICFUNC_Type *Func);

	char IIC_RIGHT = 0x00;

	
private:

#define IS_GET_IIC_NUM(PERIPH) (((PERIPH) == IIC_Num_1) || \
                              ((PERIPH) == IIC_Num_2))

#define IS_GET_IIC_FUNC(FUNC) (((FUNC) == IIC_Start) || \
                              ((FUNC) == IIC_Stop) || \
                              ((FUNC) == IIC_WaitAck) || \
                              ((FUNC) == IIC_SendAck) || \
                              ((FUNC) == IIC_SendNoAck) || \
                              ((FUNC) == IIC_SendByte) || \
                              ((FUNC) == IIC_RecByte) || \
                              ((FUNC) == IIC_SB_WA) || \
                              ((FUNC) == IIC_RB_WA))

	bool IS_PARAMS(bool JUDGE);
	char *IIC_OrderBuffer;
};


