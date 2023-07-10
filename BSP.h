#ifndef __BSP_H  // to prevent multiple addition or definition
#define __BSP_H
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stm32f407xx.h"


//delay in millisec CONFIG
void delayms(unsigned int milli);
// software delay in milli sec
void Sdelayms(int delay);

//timer 2 CONFIG
void TIM2_Init(void);
void TIM2_Start(void);

//timer 3 CONFIG
void TIM3_Init(void);
void TIM3_Start(void);


//UART 2 CONFIG PIN PA2 TX:PA3 RX
void UART2_Init(void);
void UART2_Write(unsigned int ch);
void UART2_Print(char *string);
void UART2_Send(char *msg);
unsigned int UART2_Receive(void);

//ADC 1 Config
void ADC1_Init(void);
void ADC1_Enable(void);
void ADC1_Disable(void);
void ADC1_Start(unsigned int CH);
void ADC1_Wait(void);
unsigned int ADC1_GetData(void);


///ADC-DMA config 
void ADC1_Init_DMA2(void);
void ADC1_DMA2_Stream0_Init(void);
void ADC1_DMA2_Stream0_Config(uint32_t srcadd,uint32_t desadd, uint16_t size);
void ADC1_DMA2_Start(void);

//EXTERnal Interrupt  -  pull-up pd3
void EXTI3_IRQHandler(void);
void Interrupt_Init(void);

//I2C1-Master mode-config
void I2C1_Init(void);
void I2C1_Start(void);
void I2C1_Write(unsigned int data);
void I2C1_Address(unsigned int add);
void I2C1_Stop(void);
void I2C1_Print(unsigned int *data,unsigned int size);

//SPI1-config

void SPI1_Send_Init(void);
void SPI1_Enable(void);
void SPI1_Print(unsigned int *data,int size);
void SPI1_Disable(void);
void SS_Enable(void);
void SS_Disable(void);
void SPI1_Transfer(unsigned int mydata);

//SPI1-Receive-config
void SPI1_Receive_Init(void);
void SPI1_Receive(unsigned int *data,int size);
unsigned int SPI1_receive(void);

//PWM-Config

void PWM_Timer4(uint32_t period,uint32_t duty);
	
	

#endif

