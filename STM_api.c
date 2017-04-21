#include "stm32f0xx.h" 
#include "STM_api.h"

void PrepareLED()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER &= ~0x000f0000; //~0x00030000;  
  GPIOC->MODER |= 0x00050000; //0x00010000;   
}

void OffLED(int type)
{
	if( type == 1 )
	{
		GPIOC->ODR &= ~0x00000100; 
	}
	else if( type == 2 )
	{
		GPIOC->ODR &= ~0x00000200; 
	}
}

void InverseLED(int type)
{
	if( type == 1 )
	{
		GPIOC->ODR ^= 0x00000100; 
	}
	else if( type == 2 )
	{

		GPIOC->ODR ^= 0x00000200; 
	}
}

int mutex_init(mutex *mutex){
	mutex->owner = -1;
	return 0;
}
int mutex_lock(mutex *mutex){
	while(mutex->owner != getpid() && mutex->owner != -1);
	CRITICAL_START();
	mutex->owner = getpid();
	CRITICAL_END();
	return 0;
}

int mutex_trylock(mutex *mutex){
	
	if(mutex->owner != -2)
		return -1;
	CRITICAL_START();
	mutex->owner = getpid();
	CRITICAL_END();
	return 0;
}


int mutex_unlock(mutex *mutex){
	mutex->owner = 0;
	return 0;
}

