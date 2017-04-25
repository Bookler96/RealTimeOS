#include "stm32f0xx.h" 
#include "STM_api.h"
#include "stdlib.h"

extern void AfterInit();
void Task1();
void Task2();

extern mutex  mutex_test;
int counter;

void AfterInit(){
	mutex_init(&mutex_test);
	IncludeTask(Task1);
	IncludeTask(Task2);
}

void Task1()
{
		int i;

		PrepareLED();
		OffLED(1);
    
		print("Task 1 running");
	
	  mutex_lock(&mutex_test);
	  counter = 0;
		mutex_unlock(&mutex_test);
	  
		while(1){
				
				//if(counter == 0)
					mutex_lock(&mutex_test);
			  counter++;
			  
			  if(counter >= 10)
					mutex_unlock(&mutex_test);
			
				InverseLED(1);
			  Delay(5);

		}
}

void Task2()
{
		
		PrepareLED();
		OffLED(2);
    
		print("Task 2 running");
		
		while(1){
			
			

			//		 if(mutex_trylock(&mutex_test) == 0){
						mutex_lock(&mutex_test);
							counter = 0;
							InverseLED(2);
					  mutex_unlock(&mutex_test);
						
						 
		//			 }
			 
				
			
				Delay(5);
			
			
		
			  

		}
	
}
