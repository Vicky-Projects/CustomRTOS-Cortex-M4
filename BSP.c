#include "BSP.h"





	
	
//delay in millisec CONFIG	
void delayms(unsigned int milli){
	TIM2_Init();
	TIM2_Start();
	while((TIM2->SR & (1<<0))==0);
	while(TIM2->CNT<milli);
}

// software delay in milli sec
void Sdelayms(int delay){
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}
	
	
	
//timer 2 CONFIG	
void TIM2_Init(void){
	RCC->APB1ENR|=(1<<0);
	TIM2->PSC=72000-1;
	TIM2->EGR|=(1<<0);	
}
void TIM2_Start(void){
	TIM2->CR1|=(1<<0);
	TIM2->CNT=0;
	TIM2->EGR|=(1<<0);
}
	
	
//timer 3 CONFIG	
void TIM3_Init(void){
  RCC->APB1ENR|=(1<<1);
	TIM3->PSC=15999;
	TIM3->EGR|=(1<<0);
}
void TIM3_Start(void){
	TIM3->CR1|=(1<<0);
	TIM3->CNT=0;
	TIM3->EGR|=(1<<0);
}


//UART 2 CONFIG PIN PA2 TX:PA3 RX

void UART2_Init(void){
	RCC->APB1ENR|=(1<<17);
	RCC->AHB1ENR|=(1<<0);
	GPIOA->AFR[0]=(7<<8)|(7<<12);
	GPIOA->MODER=(1<<5)|(1<<7);
	USART2->BRR=0x0683;
	USART2->CR1=(1<<2)|(1<<3);
	USART2->CR1|=(1<<13);
	USART2->CR1=(1<<5)|(1<<7);
}
	
void UART2_Write(unsigned int ch){
	USART2->DR=( ch & 0xFF );
	while((USART2->SR & (1<<7))==0);
}
void UART2_Print(char *string){
	while(*string)
		UART2_Write(*string++);
}
void UART2_Send(char *msg) {
    for (uint32_t i = 0; i < (strlen(msg)); i++) {
        while((USART2->SR &(1<<7))==0); 
        UART2_Write(msg[i]);
			
    }
}
unsigned int UART2_Receive(void){
	unsigned int mydata;
	while((USART2->SR & (1<<5))==0);
	mydata = USART2->DR;
	return mydata;
}

//ADC 1 Config
void ADC1_Init(void){
	RCC->APB2ENR|=(1<<8);//EN ADC1 CLK
	RCC->AHB1ENR|=(1<<0);//EN GPIOA CLK
	ADC->CCR&=(uint32_t)(~(1<<16))&(uint32_t)(~(1<<17));// ADC CLK FR = 16/2=8MHZ
	ADC1->CR1|=(1<<8);//SCAN MODE AND 10BIT RES
	ADC1->CR1&=(uint32_t)(~(1<<24))&(uint32_t)(~(1<<25));//res 12bit
	ADC1->CR2|=(1<<1)|(1<<10);//EOC AND CC
	ADC1->CR2&=(uint32_t)(~(1<<11));//RIGHT ALIGNMENT
	ADC1->SQR1|=(1<<20);//GOING TO USE 2 CHANNELS
	GPIOA->MODER|=(3<<2)|(3<<4);//ANALOG MODE FOR PIN PA 1,2
	ADC1->SMPR2|=(4<<3)|(4<<6);//sampling time ch 1,2
}
	
void ADC1_Enable(void){
	ADC1->CR2|=(1<<0);//ADC ON
}

void ADC1_Disable(void){
	ADC1->CR2&=(uint32_t)(~(1<<0));//ADC OFF
}

void ADC1_Start(unsigned int CH){
	ADC1->SQR3=0;
	ADC1->SQR3=(CH<<0);//SETTING THE SEQUENCE
	ADC1->SR=0;
	ADC1->CR2|=(1<<30);//start conversion
}

void ADC1_Wait(void){
	while((ADC1->SR & (1<<1))==0);
}

unsigned int ADC1_GetData(void){
	return ADC1->DR;
}

///ADC-DMA config 
void ADC1_Init_DMA2(void){
	RCC->APB2ENR|=(1<<8);//en adc1 clk
	RCC->AHB1ENR|=(1<<0);//en gpioa clk
	ADC->CCR|=(3<<16);// 64/8=8 mhz
	ADC1->CR1|=(1<<8);//scan mode
	ADC1->CR1&=(uint32_t)(~(1<<24))&(uint32_t)(~(1<<25));//res 12bit
	ADC1->CR2|=(1<<1)|(1<<10);//en cc,eoc
	ADC1->CR2&=(uint32_t)(~(1<<11));//right align
	ADC1->SQR1|=(2<<20);// using ch 3
	GPIOA->MODER|=(3<<2);//analog mode
	GPIOA->MODER|=(3<<4);
	ADC1->SMPR2|=(4<<3)|(4<<6);//sampling time ch 1,2
	ADC1->SMPR1|=(4<<18);//ST 12US
	ADC1->SMPR1|=(4<<21);//st ch17
	ADC->CCR|=(1<<23);//TS EN
	ADC1->CR2|=(1<<8);//en mode dma, conti req
	ADC1->CR2|=(1<<9);
	ADC1->SQR3|=(1<<0);//seq for ch 1,2
	ADC1->SQR3|=(2<<5);
	ADC1->SQR3|=(16<<10); //CH 16 TS
	ADC1->SQR3|=(17<<15);//ch 17
	
}


void ADC1_DMA2_Stream0_Init(void){
	RCC->AHB1ENR|=(1<<22);//en clk dma
	DMA2_Stream0->CR&=(uint32_t)(~(1<<0));//en dma
	DMA2_Stream0->CR&=(uint32_t)(~(3<<6));
	DMA2_Stream0->CR|=(1<<8);//circular mode
	DMA2_Stream0->CR|=(1<<10);// MEMORY ADD INCREMENT
	DMA2_Stream0->CR|=(1<<11)|(1<<13);//data , mem size
	DMA2_Stream0->CR&=(uint32_t)(~(7<<25));//sel ch0 FOR DMA
}

void ADC1_DMA2_Stream0_Config(uint32_t srcadd,uint32_t desadd, uint16_t size){
	DMA2_Stream0->NDTR=(size & 0xFFFF);
	DMA2_Stream0->PAR=(srcadd & 0xFFFFFFFF);
	DMA2_Stream0->M0AR=(desadd & 0xFFFFFFFF);
	DMA2_Stream0->CR|=(1<<0);//en dma
}
void ADC1_DMA2_Start(void){
	
	ADC1->SR=0;
	ADC1->CR2|=(1<<30);//start conversion
}


//EXTERnal Interrupt
void EXTI3_IRQHandler(void){
	//write some function run 
}

void Interrupt_Init(void){
	RCC->AHB1ENR|=(1<<3);// en clk gpiod
	GPIOD->MODER=0;
	GPIOD->PUPDR|=(1<<6);//pull-up pd3
	RCC->APB2ENR|=(1<<14);//en sys clk control
	SYSCFG->EXTICR[1]|=(3<<12);//exti line 3
	EXTI->IMR|=(1<<3);//disabled mask
	EXTI->RTSR|=(1<<3);//rising edge
	NVIC_SetPriority(EXTI3_IRQn,1);
	NVIC_EnableIRQ(EXTI3_IRQn);
}


//I2C1-Master mode-config
void I2C1_Init(void){
	RCC->APB1ENR|=(1<<21);
	RCC->AHB1ENR|=(1<<1);
	GPIOB->AFR[1]=(4<<0)|(4<<4);
	GPIOB->MODER=(2<<16)|(2<<18);
	I2C1->CR2=(16<<0);
	I2C1->CCR=(80<<0);
	I2C1->TRISE=(17<<0);
	I2C1->CR1=(1<<0);
}
	
void I2C1_Start(void){
	I2C1->CR1|=(1<<8);
	while((I2C1->SR1 & (1<<0))==0);
}
void I2C1_Write(unsigned int data){
	I2C1->DR =(data & 0xFF);
	while((I2C1->SR1 & (1<<7))==0);
}
void I2C1_Address(unsigned int add){
	I2C1->DR=(add & 0xFF);
	while((I2C1->SR1 & (1<<1))==0);
	unsigned int temp=I2C1->SR1 | I2C1->SR2;
}
void I2C1_Stop(void){
	I2C1->CR1|=(1<<9);
}
void I2C1_Print(unsigned int *data,unsigned int size){
	while((I2C1->SR1 & (1<<7))==0);
	while(size){
			while((I2C1->SR1 & (1<<7))==0){}
		  I2C1->DR=(volatile unsigned int)*data++;
		  size--;
	}
	while((I2C1->SR1 & (1<<2))==0);
}

//SPI1-config

void SPI1_Send_Init(void){
	RCC->APB2ENR|=(1<<12);//spi clk en
	SPI1->CR1|=(1<<0)|(1<<1);//CPOL=1,CPHA=1
  SPI1->CR1|=(1<<2);// AS MASTER
	//SPI1->CR1|=(1<<3);//pclk 16mhz/4 = spiclk=4mhzs
	SPI1->CR1&=(uint32_t)(~(1<<7));//MSB TRANSMITTED AT 1ST
	SPI1->CR1|=(1<<8)|(1<<9);//SSM,SSI = 1
	SPI1->CR1&=(uint32_t)(~(1<<10));//full duplex
	SPI1->CR1&=(uint32_t)(~(1<<11));//DFF 8 bit format
  SPI1->CR2=0;
	RCC->AHB1ENR|=(1<<1); //ENABLE CLK FOR GPIOB
	GPIOB->MODER=(2<<6)|(2<<8)|(2<<10)|(1<<18);//PB 3,4,5 AS AF,9 AS O/P
	GPIOB->OSPEEDR=(2<<6)|(2<<8)|(2<<10)|(2<<18);//HIGH SPEED
	GPIOB->PUPDR|=(1<<18);//pin 9 pull up
	GPIOB->AFR[0]=(5<<12)|(5<<16)|(5<<20);//AF5=0101 FOR PINS PB 3,4,5
}

	
void SPI1_Enable(void){
	SPI1->CR1|=(1<<6);//SPI ENABLE
}
void SPI1_Print(unsigned int *data,int size){
	int i=0;
	while(i<size){
		while((SPI1->SR & (1<<1))==0){}//wait till TXE to set
		SPI1->DR=data[i];//load data
		i++;
	}
	while((SPI1->SR & (1<<1))==0){}//wait till TXE to set
	while((SPI1->SR & (1<<7))==1){}//wait for BSY to reset
  unsigned int temp = SPI1->DR;
	temp=SPI1->SR;
}

void SPI1_Disable(void){
	SPI1->CR1&=(uint32_t)(~(1<<6));// SPI DISABLE
}
void SS_Enable(void){
	GPIOB->BSRR|=(1<<25);
}
void SS_Disable(void){
	GPIOB->BSRR|=(1<<9);
}

void SPI1_Transfer(unsigned int mydata){
	SPI1->DR=mydata;
	while((SPI1->SR & (1<<1))==0){}//wait till TXE to set
	while((SPI1->SR & (1<<7))==1){}//wait for BSY to reset
	}
//SPI1-Receive-config
void SPI1_Receive_Init(void){
	RCC->APB2ENR|=(1<<12);//spi1 clk en
	SPI1->CR1|=(1<<0)|(1<<1);//CPOL=1,CPHA=1
	SPI1->CR1&=(uint32_t)(~(1<<2));// AS slave
	
	//SPI1->CR1|=(1<<3);//pclk 16mhz/4 = spiclk=4mhzs
	SPI1->CR1&=(uint32_t)(~(1<<7));//MSB TRANSMITTED AT 1ST
	SPI1->CR1|=(1<<8)|(1<<9);//SSM,SSI = 1
	SPI1->CR1&=(uint32_t)(~(1<<10));//full duplex
	SPI1->CR1&=(uint32_t)(~(1<<11));//DFF 8 bit format
  SPI1->CR2=0;
	RCC->AHB1ENR|=(1<<1); //ENABLE CLK FOR GPIOB
	GPIOB->MODER=(2<<6)|(2<<8)|(2<<10);//PB 3,4,5 AS AF,9 AS I/P
	GPIOB->MODER&=(uint32_t)(~(1<<18))&(uint32_t)(~(1<<19));
	GPIOB->OSPEEDR=(2<<6)|(2<<8)|(2<<10)|(2<<12);//HIGH SPEED
	GPIOB->AFR[0]=(5<<12)|(5<<16)|(5<<20);//AF5=0101 FOR PINS PB 3,
}

void SPI1_Receive(unsigned int *data,int size){
	while(size){
		while((SPI1->SR & (1<<7))==1);//wait for BSY to reset
		SPI1->DR=0;//send dummy data
		while((SPI1->SR & (1<<0))==0);//wait till RXNE to set
		*data++=SPI1->DR;
		size--;
	}
}


unsigned int SPI1_receive(void){
	unsigned int mydata=0;
	SPI1->DR=0;
	while((SPI1->SR & (1<<7))==1);//wait for BSY to reset
	while((SPI1->SR & (1<<0))==0){}//wait till RXNE to set
  mydata=SPI1->DR;
	return mydata;
}
	
//PWM-Config

void PWM_Timer4(uint32_t period,uint32_t duty){
	
	for(int i=0;i<200;i++){
	RCC->AHB1ENR|=(1<<3);//gpiod clk en
	RCC->APB1ENR|=(1<<2);//tim4 clk en
	GPIOD->MODER|=(2<<30);//pd15 as AF
	GPIOD->AFR[1]|=(2<<28);//AF2 tim 4
	TIM4->CCMR2=0;//CC CHANNEL 4 CONFI AS O/P MODE CCMR2=0
	TIM4->CCER&=(uint32_t)(~(1<<13));//CCER ACTIVE HIGH OUTPUT POLARITY=INITIALE STATE
	TIM4->CCMR2|=(1<<13)|(1<<14);//PWM MODE 1
	TIM4->PSC=1599;//PR VALUE FOR 1KHZ
	TIM4->ARR = period; // ON TIME
	TIM4->CCR4 = (uint32_t)i; //OFF TIME
	for(int delay=0;delay<500000;delay++);
	TIM4->CCMR2|=(1<<11);//EN PRELOAD
	TIM4->CR1|=(1<<7);//ENABLE AUTO-RELOAD PRELOAD 
	//SELECT THE COUNTING MODE COUNTS UP BY RESET VALUE
	TIM4->CCER|=(1<<12);//ENABLE OUTPUT CH4
	TIM4->CR1|=(1<<0);//COUNTER ENABLE
	
	}
}

