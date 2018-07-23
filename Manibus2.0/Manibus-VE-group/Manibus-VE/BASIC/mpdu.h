#ifndef PDU_H_
#define PDU_H_


#define IO_Check      0xFE
#define USART_Check   0xFA
#define Wifi_Check    0xF9
#define ADC_Check     0xFC
#define PWM_Check     0xFD
#define IIC_Check     0xFB

#define PWM_Check_One 0xFD
#define PWM_Check_GetPulse 0xDD
#define PWM_Check_ICGetPulseWid 0xDB
#define PWM_Check_ICGetFrequency 0xDC


#define USART_Check_One   0xFA
#define USART2_Check      0x44
#define UART4_Check       0x4C
#define USART_Check_Last  0xA2

#define IIC_Check_One     0xFB
#define IIC_Check_Last    0xA5

#define Wifi_Check_One    0xF9
#define Wifi_Check_Last   0xA6


#define pPinInit__ID       0xE1
#define pPinDeInit__ID     0xE2
#define pPinOutPut__ID     0xE3
#define pPinReadBit__ID    0xE4

#define pUsartInit__ID     0xA1
#define pUsartDeInit__ID   0xA2
#define pUsartWrite__ID    0xA3
#define pUsartRead__ID     0xA4
#define pUsartOnLine__ID   0xA5

#define pUsartInterrupt    0xA6

#define pADCInit__ID       0xC1
#define pADCDeInit__ID     0xC2
#define pADCRead__ID       0xC3

#define pPWMInit__ID       0xD1
#define pPWMICInit__ID     0xD2
#define pPWMPluseInit__ID  0xD3
#define pPWMDeInit__ID     0xD4
#define pPWMSetPluseWid__ID 0xD5
#define pPWMICGetPluseWid__ID 0xD6
#define pPWMICGetFrequency__ID 0xD7
#define pPWMGetPluse__ID   0xD8
#define pPWMSetFrequency__ID 0xD9

#define pIICInit__ID       0xB1
#define pIICFunc__ID       0xB2

#endif


