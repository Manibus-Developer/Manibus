#include "esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "musmart_cho.h"

#if USE_ESP8266

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = {0};

u8 ESP8266_STATE = ESP8266_RIGHT;
u8 ucTcpClosedFlag = 0;

u8 ESP8266_LinkStatus = 0x00;
u8 ESP8266_ucIdLinkStatus = 0x00;


u8 WIFI_Channel_FLAG = 0x00;    //WIFI上传数据标志位
u8 WIFI_READ_FUNC = 0x00;
u8 WIFI_READ_ID = 0x00;
unsigned char * ESP8266_RevStrBuffer;

void delay_us(u16 ntime){
   u16 i;
	 u8 t;
	 for(i=ntime;i--;){
	    for(t=72;t--;);
	 }
}

void delay_ms(u16 ntime){
    u16 i;
	  for(i=ntime;i--;){
		delay_us(1000);
		}
}

static void ESP8266_USART_NVIC_Configuration ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	//NVIC_PriorityGroupConfig ( macNVIC_PriorityGroup_x );

	NVIC_InitStructure.NVIC_IRQChannel = ESP8266_USART_IRQ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	NVIC_InitStructure.NVIC_IRQChannel = esp8266_DMA_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ESP8266_UsartDMAConfig(void){

    DMA_InitTypeDef DMA_InitStructure;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	  DMA_DeInit(ESP8266_DMACHANNEL); 
	
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ESP8266_USART_->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)strEsp8266_Fram_Record .Data_RX_BUF;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = RX_BUF_MAX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority =DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(ESP8266_DMACHANNEL,&DMA_InitStructure);
		
		DMA_ITConfig(ESP8266_DMACHANNEL,DMA_IT_TC,ENABLE);
	  DMA_ITConfig(ESP8266_DMACHANNEL,DMA_IT_TE,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
		
		DMA_Cmd(ESP8266_DMACHANNEL,ENABLE);

}

void ESP8266_USARTInit(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(ESP8266_USART_CLOCK,ENABLE);
	RCC_APB2PeriphClockCmd(ESP8266_USART_PORT_CLOCK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ESP8266_USART_TX_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ESP8266_USART_TX_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ESP8266_USART_RX_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;      
	GPIO_Init(ESP8266_USART_RX_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ESP8266_USART_, &USART_InitStructure);
	
	USART_ITConfig ( ESP8266_USART_, USART_IT_RXNE, ENABLE ); //使能串口接收中断 
	USART_ITConfig ( ESP8266_USART_, USART_IT_IDLE, ENABLE ); //使能串口总线空闲中断 	
	
	USART_Cmd(ESP8266_USART_, ENABLE);
	
	ESP8266_UsartDMAConfig();
	ESP8266_USART_NVIC_Configuration();
	
}

void ESP8266_IRQHandler(void){
    
   	u16 data_len;
//	  if ( USART_GetITStatus ( ESP8266_USART_, USART_IT_RXNE ) != RESET ){
//			
//		   ucCh  = USART_ReceiveData( ESP8266_USART_ );
//			if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
//			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++] = ucCh;	
//		
//		}
		
		if ( USART_GetITStatus( ESP8266_USART_, USART_IT_IDLE ) != RESET ){
			
		data_len  =USART1->SR;
		data_len  =USART1->DR;
		
    DMA_Cmd(ESP8266_DMACHANNEL,DISABLE);
			
		data_len = RX_BUF_MAX_LEN- DMA_GetCurrDataCounter(ESP8266_DMACHANNEL);	
			
		LoopList_AddMultiData(&DataBuffer,strEsp8266_Fram_Record .Data_RX_BUF,data_len);	
    DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3); 
		ESP8266_DMACHANNEL->CNDTR = RX_BUF_MAX_LEN;			
			
		DMA_Cmd(ESP8266_DMACHANNEL,ENABLE);
			
	  strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
			
		ucTcpClosedFlag = strstr (strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);	
		}
}

void ESP8266_DMAIRQHandler(void){
    
	  DMA_ClearITPendingBit(DMA1_IT_TC3);
	  DMA_ClearITPendingBit(DMA1_IT_TE3);
    DMA_Cmd(ESP8266_DMACHANNEL, DISABLE); 
    ESP8266_DMACHANNEL->CNDTR = RX_BUF_MAX_LEN;
	  DMA_Cmd(ESP8266_DMACHANNEL, ENABLE);
	
}


   //此为可配置函数
void pWifiInit(void){
  
	
		GPIO_InitTypeDef GPIO_InitStructure;


		//CH_PD
		RCC_APB2PeriphClockCmd(ESP8266_CH_PD_CLOCK,ENABLE);

		GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(ESP8266_CH_PD_PORT, &GPIO_InitStructure );

		//RST
		RCC_APB2PeriphClockCmd(ESP8266_RST_CLOCK,ENABLE);

		GPIO_InitStructure.GPIO_Pin = ESP8266_RST_PIN;
		GPIO_Init(ESP8266_RST_PORT, &GPIO_InitStructure );

		//USART
		ESP8266_USARTInit();
	
}

void pWifiReset(void){
	
	 ESP8266_RST_LOW;
	 delay_ms ( 500 ); 
	 ESP8266_RST_HIGH;
}


//ESP8266发送AT指令
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	macESP8266_Usart( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	delay_ms ( waittime );                 //延时
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

// * 描述  ：对WF-ESP8266模块进行AT测试启动
void pWifi_AT_Test(void){
	
	char count=0;
	
	ESP8266_RST_HIGH;	
	delay_ms ( 1000 );
	while ( count < 10 )
	{
		if( ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) return;
		pWifiReset();
		++ count;
	}
	 ESP8266_STATE = ESP8266_TEST_FAILED;
}


// * 描述  ：选择WF-ESP8266模块的工作模式
void pWifi_NetModeChoose(ENUM_Net_ModeTypeDef enumMode){
	
	bool esp8266_state;
	
   switch (enumMode){
	 		case STA:
			esp8266_state = ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		  break;
	  case AP:
		  esp8266_state = ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		  break;
		case STA_AP:
		  esp8266_state = ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
	    break;
	   default:
			ESP8266_STATE = ESP8266_NETMODE_ERROR;
		  return;

	 }
	 
	 esp8266_state = true? ESP8266_STATE = ESP8266_RIGHT:ESP8266_NETMODE_ERROR;
	 
}

// * 描述  ：WF-ESP8266模块连接外部WiFi
void pWifi_JoinAP(char * pSSID, char * pPassWord){
	
		char cCmd [120];
    bool esp8266_state = true;
	
		sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
    esp8266_state = ESP8266_Cmd ( cCmd, "OK", NULL, 5000 );
	
	  esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_JOINAP_ERROR;
	
}

// * 描述  ：WF-ESP8266模块创建WiFi热点
void pWifi_BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode){
	
		char cCmd [120];
    bool esp8266_state = true;
	
		sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
    esp8266_state = ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	  
    esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_BUILDAP_ERROR;
}

// * 描述  ：WF-ESP8266模块启动多连接
void pWifi_Use_MultipleId(FunctionalState enumEnUnvarnishTx){


		char cStr [20];
    bool esp8266_state = true;
	
		sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
    esp8266_state = ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
		esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_MULIP_ERROR;

}

// * 描述  ：WF-ESP8266模块连接外部服务器
void pWifi_LinkServer(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id){

		char cStr [100] = { 0 }, cCmd [120];
		bool esp8266_state = true;
		
		switch (  enumE )
		{
			case enumTCP:
				sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
				break;
			
			case enumUDP:
				sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
				break;
			
			default:
				break;
		}

		if ( id < 5 )
			sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

		else
			sprintf ( cCmd, "AT+CIPSTART=%s", cStr );
		
     esp8266_state = ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
		 
		 esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_LINKSERVER_ERROE;

}

//：WF-ESP8266模块开启或关闭服务器模式
void pWifi_ifServerMode(FunctionalState enumMode, char * pPortNum, char * pTimeOver){

	char cCmd1 [120], cCmd2 [120];
  bool esp8266_state = true;
	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );
    esp8266_state = (ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ));
		
		esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_IF_SERVERMODE_ERROR;

	}
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );
    esp8266_state = ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
		esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_IF_SERVERMODE_ERROR;
	}
}

// * 描述  ：获取 WF-ESP8266 的连接状态，较适合单端口时使用
void pWifi_GetLinkStatus(void){

	
		if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
		{
			if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) ){
			
				ESP8266_LinkStatus = 2;
			  return;
			}			
			
			else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) ){
				ESP8266_LinkStatus = 3;
			  return;
			}

			else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) ){
				ESP8266_LinkStatus = 4;
			  return;
			}				
		}
        ESP8266_LinkStatus = 0;
}

// * 描述  ：获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
void pWifi_GetIdLinkStatus(void){

    if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ESP8266_ucIdLinkStatus |= 0x01;
		else 
			ESP8266_ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ESP8266_ucIdLinkStatus |= 0x02;
		else 
			ESP8266_ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ESP8266_ucIdLinkStatus |= 0x04;
		else 
			ESP8266_ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ESP8266_ucIdLinkStatus |= 0x08;
		else 
			ESP8266_ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ESP8266_ucIdLinkStatus |= 0x10;
		else 
			ESP8266_ucIdLinkStatus &= ~ 0x10;	

	}
}


// * 描述  ：获取 F-ESP8266 的 AP IP
void pWifi_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength ){
	
		char uc;
		char * pCh;


		ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );

		pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );

		if ( pCh ){
			
				pCh += 6;
		}else{
			
		 ESP8266_STATE =ESP8266_INQUIREAPIP_ERROR;
		return;
		}


		for ( uc = 0; uc < ucArrayLength; uc ++ )
		{
		pApIp [ uc ] = * ( pCh + uc);

		 if ( pApIp [ uc ] == '\"' )
		 {
			pApIp [ uc ] = '\0';
			break;
		 }
		}
    ESP8266_STATE =ESP8266_RIGHT;
}


// * 描述  ：配置WF-ESP8266模块进入透传发送
void pWifi_UnvarnishSend ( void ){

	if (!ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) ){
		
	  ESP8266_STATE = ESP8266_UNVARNISHSEND_ERROR;
		return;
	}
	 if(! ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 500 )){
	 
		 ESP8266_STATE = ESP8266_UNVARNISHSEND_ERROR;
	 }else{
	   ESP8266_STATE = ESP8266_RIGHT;
	 }	      
}


// * 描述  ：配置WF-ESP8266模块退出透传模式
void pWifi_ExitUnvarnishSend (void){

	delay_ms ( 1000 );
	macESP8266_Usart ( "+++" );
	delay_ms ( 500 );
	
}

// * 描述  ：WF-ESP8266模块发送字符串
void pWifiOnLine(FunctionalState enumEnUnvarnishTx,ENUM_ID_NO_TypeDef ucId ){

  WIFI_Channel_FLAG = 0x01;
  WIFI_READ_FUNC = enumEnUnvarnishTx;
	WIFI_READ_ID =ucId; 	
}

void pWifiReadOne_(u8 byte){

   USART_SendData(ESP8266_USART_,byte);		
   while (USART_GetFlagStatus(ESP8266_USART_, USART_FLAG_TXE) == RESET);
}


void pWifiRead_( u8 * pStr, u32 ulStrLength ){

    bool esp8266_state = true;
	  char cStr [20];
		
	if ( WIFI_READ_FUNC )
	{
		macESP8266_Usart ( "%s", pStr );
		
		esp8266_state = true;
		
	}
	else
	{
		
		if ( WIFI_READ_ID < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", WIFI_READ_ID, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );

		esp8266_state = ESP8266_Cmd (cStr, "SEND OK", 0, 1000 );
  }
	
  esp8266_state = true ? ESP8266_STATE =  ESP8266_RIGHT:ESP8266_SENDSTRING_ERROR;
}


//// * 描述  ：WF-ESP8266模块接收字符串
//void pWifi_ReceiveString(FunctionalState enumEnUnvarnishTx){
//	
//		strEsp8266_Fram_Record .InfBit .FramLength = 0;

//		while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag);
//	  strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
//	
//		strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';

//		if ( enumEnUnvarnishTx )
//			
//			ESP8266_RevStrBuffer = strEsp8266_Fram_Record .Data_RX_BUF;

//		else 
//		{
//			if (strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
//				ESP8266_RevStrBuffer = strEsp8266_Fram_Record .Data_RX_BUF;
//		}
//}








#endif