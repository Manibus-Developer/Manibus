#include "musmart_cho.h"
#include "basic_periphset.h"
#include "basic_periphfunc.h"
#include "i2c.h"
#include "esp8266.h"
#include "musmart.h"

struct _m_usmart_nametab usmart_nametab[]={
	
	(void*)read_addr,
	(void*)write_addr,	

	(void*)pPinInit,
	(void*)pPinDeInit,
	(void*)pPinOutPut,  //4

	(void*)pUsartInit,	
	(void*)pUsartDeInit,
	(void*)pUsartWrite,
	(void*)pUsartRead,   //8

	(void*)pADCInit,
	(void*)pADCDeInit,
	(void*)pADCRead,     //11

	(void*)pPWMInit,
	(void*)pPWMICInit,
	(void*)pPWMPluseInit,
	(void*)pPWMDeInit,      //15
	(void*)pPWMSetPulseWid,
	(void*)pPWMICGetPulseWid,
	(void*)pPWMICGetFrequency,//18
	(void*)pPWMGetPulse,    // 19

	(void*)pPinReadBit,      //20

	(void*)pIICInit,         //21
   
	(void*)pWifiInit,         //22
	(void*)pWifiReset,
	(void*)pWifi_AT_Test,     //24
  (void*)pWifi_NetModeChoose,
  (void*)pWifi_JoinAP,
  (void*)pWifi_BuildAP,      //27
  (void*)pWifi_Use_MultipleId,
  (void*)pWifi_LinkServer,
  (void*)pWifi_ifServerMode,    //30
  (void*)pWifi_GetLinkStatus,
  (void*)pWifi_GetIdLinkStatus,
  (void*)pWifi_Inquire_ApIp,     //33
  (void*)pWifi_UnvarnishSend,
  (void*)pWifi_ExitUnvarnishSend,
		
  (void*)pWifiOnLine,               //36 此函数每次读取数据都必须使用一次		
  (void*)pUsartOnLine,		          //37 此函数每次读取数据都必须使用一次
		
  (void*)pPWMSetFrequency            //38
		
};


struct iic_func iic_name[]={
	
	(void*)IIC_Start,      //1
	(void*)IIC_Stop,       //2
	(void*)IIC_WaitAck,    //3
	(void*)IIC_SendAck,    //4
	(void*)IIC_SendNoAck,  //5
	(void*)IIC_SendByte,   //6
	(void*)IIC_RecByte	   //7

};

