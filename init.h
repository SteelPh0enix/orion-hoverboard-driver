#ifndef INIT_H
#define INIT_H

#include <gd32f1x0.h>
#include <gd32f1x0_usart.h>
#include <gd32f1x0_dma.h>
#include <gd32f1x0_exti.h>
#include <gd32f1x0_gpio.h>

void interrupts_init(void);
void gpio_init(void);
void pwm_init(void);
void usart_init(void);

#endif