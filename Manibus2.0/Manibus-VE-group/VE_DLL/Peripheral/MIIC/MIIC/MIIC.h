#include <iostream>
#include <utility>
#include <string>

#define MIIC_API __declspec(dllexport)

#define IIC_Check          0xFB

#define pIICInit__ID       0xB1
#define pIICFunc__ID       0xB2

#define IIC_NUMMBER        0x02
#define IICRIGHT           0xFB
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

class MIIC_API MIIC {

public:

	MIIC();
	~MIIC();

	virtual  std::pair<unsigned short ,unsigned char*> IICInit(IICNUM_Type IIC);
	virtual  std::pair<unsigned short, unsigned char*> IICFunc(IICNUM_Type IIC, IICFUNC_Type *Func);

	virtual std::pair<unsigned short, unsigned char*> MIICSerialFeedBack();
	virtual std::pair<unsigned short, unsigned char*> MIICWifiFeedBack(unsigned char ID);

	virtual bool IIC_FeedBack(unsigned char ID, unsigned char *Msg); 
	virtual bool IIC_FeedBack(unsigned char ID, unsigned char *Msg, unsigned char* readback,unsigned char length);
	unsigned char IIC_RIGHT = 0x00;
	
private:

	unsigned char *sendout_;

	bool IIC_ID_CHECK_ = false;
	bool IIC_PARAM_CHECK_ = false;
	bool IIC_CHECK_ = false;

	unsigned char continue_count = 0;

#define  IIC_FBInit()            IIC_ID_CHECK_ = false;IIC_CHECK_  = false;IIC_PARAM_CHECK_ = false; continue_count=0;

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
	bool IS_RIGHT(unsigned char param1, unsigned char param2);
};


