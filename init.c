#include "init.h"

void Interrupt_init(void)
{
  // Set IRQ priority configuration
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
}