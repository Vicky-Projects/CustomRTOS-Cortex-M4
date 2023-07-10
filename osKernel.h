#include <stdint.h>
#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include "stm32f4xx.h"               

void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);

uint8_t osKernelAddThreads
(
void(*task0)(void),
void(*task1)(void),
void(*task2)(void),
void(*task3)(void)
	);
                           														 
void osThreadYeild(void);	 
//new
void osSchedulerRR(void);

#define PERIOD 100
void Task4(void);


#endif

