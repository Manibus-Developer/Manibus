#ifndef PDU_H_
#define PDU_H_


#define IO_Check      0xFE
#define USART_Check   0xFA
#define Wifi_Check    0xF9

#define ADC_Check_One  0xFC
#define ADC_Check_Last 0xA3

#define PWM_Check_One 0xFD
#define PWM_Check_GetPulse 0xDD
#define PWM_Check_ICGetPulseWid 0xDB
#define PWM_Check_ICGetFrequency 0xDC
#define PWM_Check_Last   0xA4

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




#endif


