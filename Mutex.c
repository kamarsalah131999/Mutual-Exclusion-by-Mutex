/*
 * Mutex.c
 *
 * Created: 7/20/2023 2:58:17 AM
 * Author : kamar
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

int x,y;
void task1 (void *pv);
void task2 (void *pv);
SemaphoreHandle_t  my_mutex;

int main(void)
{
	/*create tasks*/
	xTaskCreate (task1, "k", 100, NULL, 2, NULL);
	xTaskCreate (task2, "M", 100, NULL, 1, NULL);
	
	/*create semaphore*/
	my_mutex = xSemaphoreCreateMutex();
	
	/*start os*/
	vTaskStartScheduler();
   
}
 
void task1 (void *pv)
{
	DDRD|=(1<<7);                                                                  //pin 7 is output pin
	while(1)
	{
		xSemaphoreTake(my_mutex,1000);                                           // require mutex 
		if (x!=y)
		{
			PORTD^=(1<<7);                                                        //toggle led
		}
		xSemaphoreGive(my_mutex);                                                // unlocked mutex
		vTaskDelay(100);
	}
}
		
void task2 (void *pv)
{
	while(1)
	{	
		xSemaphoreTake(my_mutex,1000);                                           // require mutex
		++x;
		_delay_ms(200);
		++y;
		xSemaphoreGive(my_mutex);                                                // unlocked mutex
	}
}
				