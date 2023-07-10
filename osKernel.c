//scheduller
//TCB --> going to be array to hold the threads
//stack


#include "osKernel.h"

#define SYSPRI3 (*((volatile uint32_t *)0xE000ED20))
#define INTCTRL (*((volatile uint32_t *)0xE000ED04))

void osSchedulerLaunch(void);
void osKernelStackInit(int i);

#define NUM_THREAD 4
#define STACKSIZE 100

#define BUS_FRE 16000000
static uint32_t MILLIS_PRESCALAR;



struct tcb{
	int32_t *stackpt;
	struct tcb *nextpt;
};

typedef struct tcb tcbtype;
tcbtype tcbs[NUM_THREAD];
tcbtype *currentpt;

static int32_t TCB_STACK[NUM_THREAD][STACKSIZE];


void osKernelStackInit(int i){//stack init,pc init
	tcbs[i].stackpt=&TCB_STACK[i][STACKSIZE-16];//to point thread address to top of the stack
	TCB_STACK[i][STACKSIZE-1]=0x01000000;//to select the thumb mode
}


uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void),void(*task3)(void))
{
	__disable_irq();
	tcbs[0].nextpt=&tcbs[1];
	tcbs[1].nextpt=&tcbs[2];
	tcbs[2].nextpt=&tcbs[3];
	tcbs[3].nextpt=&tcbs[0];
	
	osKernelStackInit(0);
	TCB_STACK[0][STACKSIZE-2]=(int32_t)(task0);//-2 corespondes to pc location then point to thread
	
	osKernelStackInit(1);
	TCB_STACK[1][STACKSIZE-2]=(int32_t)(task1);
	
	osKernelStackInit(2);
	TCB_STACK[2][STACKSIZE-2]=(int32_t)(task2);
	
	osKernelStackInit(3);
	TCB_STACK[3][STACKSIZE-2]=(int32_t)(task3);
	
	currentpt=&tcbs[0];
	
	__enable_irq();
	
	return 1;
}


void osKernelInit(void){
	__disable_irq();
	
	MILLIS_PRESCALAR=BUS_FRE/1000;
}

void osKernelLaunch(uint32_t quanta){
	SysTick->CTRL=0;
	SysTick->VAL=0;
	SYSPRI3=(SYSPRI3&0x00FFFFFF)|0xE0000000;//systick priroity 7
	SysTick->LOAD=(quanta*MILLIS_PRESCALAR)-1;//to get milli sec
	SysTick->CTRL=0x00000007;
	osSchedulerLaunch();
}
	



////cooperative 

void osThreadYeild(void){
	SysTick->VAL=0; 
	INTCTRL=0x04000000;//trigger interrupt
}


//new method

static uint32_t periodic_tick;

void osSchedulerRR(void)
{
	//if(++periodic_tick==PERIOD){
		//(*Task4)();
		//periodic_tick=0;
	//}
	currentpt=currentpt->nextpt;
	
}

	 
	
	
	
	
	 
	


