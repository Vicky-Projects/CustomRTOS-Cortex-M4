				AREA | .text|, CODE, READONLY, ALIGN=2
				THUMB
			    PRESERVE8
				EXTERN currentpt
				EXPORT SysTick_Handler
				EXPORT osSchedulerLaunch
			    ;IMPORT osSchedulerRR
					
SysTick_Handler        ; SAVE REGI R0,R1,R2,R3,R12,LR,PC,PSR
	CPSID	I          ;disable interrupt
	PUSH	{R4-R11}   ;storing the rest rgidter into stack
	LDR		R0,=currentpt
	LDR		R1,[R0]    ;SAVING THE CONTEXT OF THIS THREAD R1=cuPT
	STR		SP,[R1]    ;STORING THE cuPT IN STACK POINTER
	LDR		R1,[R1,#4] ;TAKES TO 2ND ENTRY R1=cuPT
	STR 	R1,[R0]    ;cuPT=R1
	;PUSH	{R0,LR}
	;BL		osSchedulerRR
	LDR		SP,[R1]	   ;SP=cuPT->STACKPT
	POP		{R4-R11}
	CPSIE	I
	BX		LR
	
	
osSchedulerLaunch
	LDR		R0,=currentpt
	LDR		R2,[R0]         ;R2=cuPT
	LDR		SP,[R2]			;SP=currentpt-->stackpt
	POP		{R4-R11}
	POP		{R0-R3}
	POP		{R12}
	ADD		SP,SP,#4
	POP		{LR}
	ADD		SP,SP,#4
	CPSIE	I
	BX		LR
	
	
   ALIGN
   END