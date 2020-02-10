#include <gd32f1x0.h>
#include "systick.h"
#include "init.h"

int main() {
	systick_config();
	interrupts_init();
	gpio_init();
	usart_init();
	
	while(1) {
		delay_ms(1);
	}
}