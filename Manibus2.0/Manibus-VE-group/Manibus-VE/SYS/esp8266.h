#ifndef ESP8266_H_
#define ESP8266_H_

#include "stm32f10x.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "Common.h"
#include "stm32f10x_dma.h"

#define USE_ESP8266 1 

#define ESP8266_RIGHT                0x01
#define ESP8266_TEST_FAILED          0x02
#define ESP8266_NETMODE_ERROR        0x03
#define ESP8266_JOINAP_ERROR         0x04
#define ESP8266_BUILDAP_ERROR        0x05
#define ESP8266_MULIP_ERROR          0x06
#define ESP8266_LINKSERVER_ERROE     0x07
#define ESP8266_IF_SERVERMODE_ERROR  0x08
#define ESP8266_INQUIREAPIP_ERROR    0x09
#define ESP8266_UNVARNISHSEND_ERROR  0x10
#define ESP8266_SENDSTRING_ERROR     0X11

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
}ENUM_NetPro_TypeDef;


typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;


typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


#define RX_BUF_MAX_LEN     1024    //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	unsigned char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;


#define ESP8266_CH_PD_CLOCK  RCC_APB2Periph_GPIOB
#define ESP8266_CH_PD_PORT   GPIOB
#define ESP8266_CH_PD_PIN    GPIO_Pin_8

#define ESP8266_RST_CLOCK    RCC_APB2Periph_GPIOB
#define ESP8266_RST_PORT     GPIOB
#define ESP8266_RST_PIN      GPIO_Pin_9

#define ESP8266_USART_CLOCK   RCC_APB1Periph_USART3
#define ESP8266_USART_PORT_CLOCK  RCC_APB2Periph_GPIOB

#define ESP8266_USART_TX_PORT   GPIOB
#define ESP8266_USART_TX_PIN    GPIO_Pin_10

#define ESP8266_USART_RX_PORT   GPIOB
#define ESP8266_USART_RX_PIN    GPIO_Pin_11

#define ESP8266_DMACHANNEL      DMA1_Channel3
#define ESP8266_DMAIRQHandler   DMA1_Channel3_IRQHandler
#define esp8266_DMA_IRQ         DMA1_Channel3_IRQn
#define ESP8266_USART_          USART3
#define ESP8266_USART_IRQ       USART3_IRQn
#define ESP8266_IRQHandler      USART3_IRQHandler


#define ESP8266_CH_ENABLE    ESP8266_CH_PD_PORT->BSRR = ESP8266_CH_PD_PIN
#define ESP8266_CH_DISABLE   ESP8266_CH_PD_PORT->BRR = ESP8266_CH_PD_PIN

#define ESP8266_RST_HIGH     ESP8266_RST_PORT->BSRR = ESP8266_RST_PIN 
#define ESP8266_RST_LOW      ESP8266_RST_PORT->BRR =  ESP8266_RST_PIN



#define   macESP8266_Usart( fmt, ... )  USART_printf ( ESP8266_USART_, fmt, ##__VA_ARGS__ ) 
#define   macPC_Usart( fmt, ... )       printf ( fmt, ##__VA_ARGS__ )

extern u8 ucTcpClosedFlag;
extern u8 ESP8266_STATE;
extern u8 ESP8266_LinkStatus;
extern u8 ESP8266_ucIdLinkStatus;
extern u8 WIFI_Channel_FLAG;

void pWifiInit(void);
void pWifiReset(void);
void pWifi_AT_Test(void);
void pWifi_NetModeChoose(ENUM_Net_ModeTypeDef enumMode);
void pWifi_JoinAP(char * pSSID, char * pPassWord);
void pWifi_BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode);
void pWifi_Use_MultipleId(FunctionalState enumEnUnvarnishTx);
void pWifi_LinkServer(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
void pWifi_ifServerMode(FunctionalState enumMode, char * pPortNum, char * pTimeOver);
void pWifi_GetLinkStatus(void);
void pWifi_GetIdLinkStatus(void);
void pWifi_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength );
void pWifi_UnvarnishSend ( void );
void pWifi_ExitUnvarnishSend (void);

void pWifiOnLine(FunctionalState enumEnUnvarnishTx,ENUM_ID_NO_TypeDef ucId);
void pWifiRead_(u8 * pStr, u32 ulStrLength);
void pWifiReadOne_(u8 byte);

//void pWifi_ReceiveString(FunctionalState enumEnUnvarnishTx);


#endif



