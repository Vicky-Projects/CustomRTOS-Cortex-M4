#include "osKernel.h"
#include "BSP.h"
#include "TFT.h"

#define QUANTA 10

void Task0(void);
void Task1(void);
void Task2(void);
void Task3(void);

static uint32_t count0,count1,count2,count3;



void Task0(void){
	
	while(1){
		count0++;
		//osThreadYeild();
	}
	
}


void Task1(void){
	
	while(1){
		count1++;
	}
	
}

void Task2(void){
	
	while(1){
		count2++;
	}
	
	
}

void Task3(void){
	
	while(1){
		count3++;
	}
	
}

//new method
void Task4(void){

}


///device setup call function


static void CALLdevice_Setup(void){
	
	//TFT_Setup();
	//add more
	//
	//	
	
}
	
int main(void){
	/////
	//CALLdevice_Setup();
	////	
	/////
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2,&Task3);
	osKernelLaunch(QUANTA);
	
	
	
	
}





