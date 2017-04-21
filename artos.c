#include "stm32f0xx.h"
#include "stdlib.h"


#define TASK_LIMIT 3

int TIMESTAMP = 0;

unsigned char Stacks[TASK_LIMIT][0x200];

char  OutputBuffer[256];
int countOutputBuffer = 0;
int blockRead = 0;
int USARTReady = 0;

//Context
typedef struct context{
	unsigned int GPR[13]; 
	void * PC; 
	void * LR;
	void * SP;
	unsigned int PSW;
	int idTask;
	int currentStatus;

} context_t;




context_t ContextARTOS[TASK_LIMIT];

context_t * CurrentContext = NULL;

extern int IncludeTask(void * Task);
void USARTSend();
void USARTInit();
int print(char * output);

int taskCount;
int tnum = 0;
int CriticalProcess;



/**
* Include Task to Schedule
* @param Task
* Return pid
**/
extern int IncludeTask( void * Task){
	
		if(tnum < 8){
		
			
			//CurrentContext = (context_t * )malloc(16);
			ContextARTOS[taskCount].idTask = taskCount;
		
			
			
			ContextARTOS[taskCount].PC = (void *)(((int)(Task)) | (1));
			ContextARTOS[taskCount].SP = &Stacks[tnum][0x200 - 4];
			ContextARTOS[taskCount].PSW = 0x01000000;
			taskCount++;
			return 0;
		}
		else 
			return -1;
	
}

int DeleteTask(){
		
	
	
}

extern int getpid(){
	 return tnum;
}

void ScheduleWorker(){
	
		tnum++;
	
	if(tnum == taskCount){
		tnum = 0;
//		if(taskCount > 1)
//			tnum = 1;
	}
	
	
}

extern void Delay(int time){
	int time1 = TIMESTAMP;
	while(TIMESTAMP - time1 < time);
}

extern void CRITICAL_START(){
	  CriticalProcess = tnum;
}

extern void CRITICAL_END(){
	  CriticalProcess = -2;
}


void  InitRTOS(){


	TIMESTAMP = 0;
	taskCount = 0;
	tnum = 0;

//	ContextARTOS = NULL;
//	ContextARTOS = malloc(sizeof(context_t));
//  ContextARTOS->next = NULL;
	CriticalProcess = -2;
	

	
	SysTick_Config(SystemCoreClock / 1000); //1000
	
  taskCount = 1;
	USARTInit();
	IncludeTask(USARTSend);
	print("OS Init success\n");
	AfterInit();

	while(1);
}

void SysTick_Handler()
{

	
	TIMESTAMP++;
	
	if(taskCount <= 0)
		return;
	
	
	
	SaveContext(&ContextARTOS[tnum]);
	
	
	
	
	
	
	if(CriticalProcess == -2){

		ScheduleWorker();

	}
	RestoreContext(&ContextARTOS[tnum]);
	__ASM("ISB");

}


void USARTInit(){
	//enable A register
	RCC->AHBENR = 0 | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
	

	
		RCC->APB2ENR = 0 | RCC_APB2ENR_USART1EN;
	
	  GPIOA->MODER &= ~ 0x003c0000;

		GPIOA->MODER |= 0x00280000;
		
		GPIOA->AFR[1] = 0x00000110;
		GPIOA->AFR[1] = 0x00000110;
	
	
	
		USART1->BRR = 8000000/38400; //USART_BRR_DIV_FRACTION;
		USART1->CR1 |= USART_CR1_UE; 
		USART1->CR1 |= USART_CR1_TE; 
		USART1->CR1 |= USART_CR1_RE;
		USART1->CR1 |= USART_CR1_RXNEIE;

		NVIC_EnableIRQ(USART1_IRQn);
		USARTReady = 1;
		countOutputBuffer = 0;
		
}

extern int print(char * output){
	 
	int i = 0;
	if(USARTReady == 0)
		USARTInit();
	
			blockRead = 1;
			while(output[i] != NULL && i + countOutputBuffer < 256){
				  OutputBuffer[countOutputBuffer + i] = output[i];
				  i++;
			}
			
		countOutputBuffer+=i;
		blockRead = 0;
		return 0;
		
}

void USARTSend(){
	
		int i = 0;

	while(1){
		while(i < countOutputBuffer && blockRead == 0 && USARTReady == 1){

		USART1->TDR = OutputBuffer[i];
		
		while((USART1->ISR & USART_ISR_TXE) == 0);
			i++;
		}
		
		if(countOutputBuffer == 256)
			countOutputBuffer = 0;
		
	}
	
}

int main(){
	return 0;
}
