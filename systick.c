#include "systick.h"
#include <gd32f1x0.h>

volatile static uint32_t delay_counter;

void systick_config(void) {
	if (SysTick_Config(SystemCoreClock / 1000U)) {
		while (1) {
		}
	}
	
	NVIC_SetPriority(SysTick_IRQn, 0U);
}

void delay_ms(uint32_t time) {
	delay_counter = time;
	
	while(delay_counter != 0U) {
	}
}

void delay_decrement(void) {
	if (delay_counter != 0U) {
		delay_counter--;
	}
}