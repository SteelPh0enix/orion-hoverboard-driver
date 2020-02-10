#include <gd32f1x0.h>
#include "systick.h"

int main() {
	systick_config();
	
	while(1) {
		delay_ms(1);
	}
}