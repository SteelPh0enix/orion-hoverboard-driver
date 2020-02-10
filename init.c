#include "init.h"
#include "defines.h"
#include "globals.h"
#include "comms.h"

// PWM timer Parameter structs
timer_parameter_struct timerBldc_paramter_struct;	
timer_break_parameter_struct timerBldc_break_parameter_struct;
timer_oc_parameter_struct timerBldc_oc_parameter_struct;

// DMA (USART) structs
dma_parameter_struct dma_init_struct_usart;
uint8_t usartMasterSlave_rx_buf[USART_COMMS_RX_BUFFERSIZE];

// DMA (ADC) structs
dma_parameter_struct dma_init_struct_adc;

void interrupts_init(void)
{
  // Set IRQ priority configuration
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
}

void gpio_init(void) {
	// Enable all GPIO clocks
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOF);
	
	// Init mosfet output
	gpio_mode_set(MOSFET_OUT_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MOSFET_OUT_PIN);	
	gpio_output_options_set(MOSFET_OUT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MOSFET_OUT_PIN);
	
	// Init HAL input
	gpio_mode_set(HALL_A_PORT , GPIO_MODE_INPUT, GPIO_PUPD_NONE, HALL_A_PIN);
	gpio_mode_set(HALL_B_PORT , GPIO_MODE_INPUT, GPIO_PUPD_NONE, HALL_B_PIN);
	gpio_mode_set(HALL_C_PORT , GPIO_MODE_INPUT, GPIO_PUPD_NONE, HALL_C_PIN);
	
	// Init ADC pins
	gpio_mode_set(VBATT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, VBATT_PIN);
	gpio_mode_set(CURRENT_DC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CURRENT_DC_PIN);
	gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_6);
	gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);
	
	// Init USART_MASTERSLAVE
	gpio_mode_set(USART_MASTERSLAVE_TX_PORT , GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_MASTERSLAVE_TX_PIN);	
	gpio_mode_set(USART_MASTERSLAVE_RX_PORT , GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_MASTERSLAVE_RX_PIN);
	gpio_output_options_set(USART_MASTERSLAVE_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_MASTERSLAVE_TX_PIN);
	gpio_output_options_set(USART_MASTERSLAVE_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_MASTERSLAVE_RX_PIN);	
	gpio_af_set(USART_MASTERSLAVE_TX_PORT, GPIO_AF_1, USART_MASTERSLAVE_TX_PIN);
	gpio_af_set(USART_MASTERSLAVE_RX_PORT, GPIO_AF_1, USART_MASTERSLAVE_RX_PIN);
	
	// Init debug pin
	gpio_mode_set(DEBUG_PORT , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DEBUG_PIN);	
	gpio_output_options_set(DEBUG_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DEBUG_PIN);
	
	// Init PWM output Pins (Configure as alternate functions, push-pull, no pullup)
  gpio_mode_set(TIMER_BLDC_GH_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TIMER_BLDC_GH_PIN);
	gpio_mode_set(TIMER_BLDC_BH_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TIMER_BLDC_BH_PIN);
	gpio_mode_set(TIMER_BLDC_YH_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TIMER_BLDC_YH_PIN);
	gpio_mode_set(TIMER_BLDC_GL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TIMER_BLDC_GL_PIN);
	gpio_mode_set(TIMER_BLDC_BL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TIMER_BLDC_BL_PIN);
	gpio_mode_set(TIMER_BLDC_YL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TIMER_BLDC_YL_PIN);
	
  gpio_output_options_set(TIMER_BLDC_GH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TIMER_BLDC_GH_PIN);
  gpio_output_options_set(TIMER_BLDC_BH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TIMER_BLDC_BH_PIN);
  gpio_output_options_set(TIMER_BLDC_YH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TIMER_BLDC_YH_PIN);
	gpio_output_options_set(TIMER_BLDC_GL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TIMER_BLDC_GL_PIN);
  gpio_output_options_set(TIMER_BLDC_BL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TIMER_BLDC_BL_PIN);
  gpio_output_options_set(TIMER_BLDC_YL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TIMER_BLDC_YL_PIN);

  gpio_af_set(TIMER_BLDC_GH_PORT, GPIO_AF_2, TIMER_BLDC_GH_PIN);
  gpio_af_set(TIMER_BLDC_BH_PORT, GPIO_AF_2, TIMER_BLDC_BH_PIN);
	gpio_af_set(TIMER_BLDC_YH_PORT, GPIO_AF_2, TIMER_BLDC_YH_PIN);
	gpio_af_set(TIMER_BLDC_GL_PORT, GPIO_AF_2, TIMER_BLDC_GL_PIN);
  gpio_af_set(TIMER_BLDC_BL_PORT, GPIO_AF_2, TIMER_BLDC_BL_PIN);
	gpio_af_set(TIMER_BLDC_YL_PORT, GPIO_AF_2, TIMER_BLDC_YL_PIN);
	
	// Init buzzer
	gpio_mode_set(BUZZER_PORT , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZER_PIN);	
	gpio_output_options_set(BUZZER_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BUZZER_PIN);
}

void pwm_init(void) {
	// Enable timer clock
	rcu_periph_clock_enable(RCU_TIMER_BLDC);
	
	// Initial deinitialize of the timer
	timer_deinit(TIMER_BLDC);
	
	// Set up the basic parameter struct for the timer
	timerBldc_paramter_struct.counterdirection 	= TIMER_COUNTER_UP;
	timerBldc_paramter_struct.prescaler 				= 0;
	timerBldc_paramter_struct.alignedmode 			= TIMER_COUNTER_CENTER_DOWN;
	timerBldc_paramter_struct.period						= 72000000 / 2 / PWM_FREQ;
	timerBldc_paramter_struct.clockdivision 		= TIMER_CKDIV_DIV1;
	timerBldc_paramter_struct.repetitioncounter = 0;
	timer_auto_reload_shadow_disable(TIMER_BLDC);
	
	// Initialize timer with basic parameter struct
	timer_init(TIMER_BLDC, &timerBldc_paramter_struct);

	// Deactivate output channel fastmode
	timer_channel_output_fast_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, TIMER_OC_FAST_DISABLE);
	timer_channel_output_fast_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, TIMER_OC_FAST_DISABLE);
	timer_channel_output_fast_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, TIMER_OC_FAST_DISABLE);
	
	// Deactivate output channel shadow function
	timer_channel_output_shadow_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, TIMER_OC_SHADOW_DISABLE);
	
	// Set output channel PWM type to PWM1
	timer_channel_output_mode_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, TIMER_OC_MODE_PWM1);
	timer_channel_output_mode_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, TIMER_OC_MODE_PWM1);
	timer_channel_output_mode_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, TIMER_OC_MODE_PWM1);

	// Initialize pulse length with value 0 (pulse duty factor = zero)
	timer_channel_output_pulse_value_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, 0);
	timer_channel_output_pulse_value_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, 0);
	timer_channel_output_pulse_value_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, 0);
	
	// Set up the output channel parameter struct
	timerBldc_oc_parameter_struct.ocpolarity 		= TIMER_OC_POLARITY_HIGH;
	timerBldc_oc_parameter_struct.ocnpolarity 	= TIMER_OCN_POLARITY_LOW;
	timerBldc_oc_parameter_struct.ocidlestate 	= TIMER_OC_IDLE_STATE_LOW;
	timerBldc_oc_parameter_struct.ocnidlestate 	= TIMER_OCN_IDLE_STATE_HIGH;
	
	// Configure all three output channels with the output channel parameter struct
	timer_channel_output_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, &timerBldc_oc_parameter_struct);
  timer_channel_output_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, &timerBldc_oc_parameter_struct);
	timer_channel_output_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, &timerBldc_oc_parameter_struct);

	// Set up the break parameter struct
	timerBldc_break_parameter_struct.runoffstate			= TIMER_ROS_STATE_ENABLE;
	timerBldc_break_parameter_struct.ideloffstate 		= TIMER_IOS_STATE_DISABLE;
	timerBldc_break_parameter_struct.protectmode			= TIMER_CCHP_PROT_OFF;
	timerBldc_break_parameter_struct.deadtime 				= DEAD_TIME;
	timerBldc_break_parameter_struct.breakstate				= TIMER_BREAK_ENABLE;
	timerBldc_break_parameter_struct.breakpolarity		= TIMER_BREAK_POLARITY_LOW;
	timerBldc_break_parameter_struct.outputautostate 	= TIMER_OUTAUTO_ENABLE;
	
	// Configure the timer with the break parameter struct
	timer_break_config(TIMER_BLDC, &timerBldc_break_parameter_struct);

	// Disable until all channels are set for PWM output
	timer_disable(TIMER_BLDC);

	// Enable all three channels for PWM output
	timer_channel_output_state_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, TIMER_CCX_ENABLE);
	timer_channel_output_state_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, TIMER_CCX_ENABLE);
	timer_channel_output_state_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, TIMER_CCX_ENABLE);

	// Enable all three complemenary channels for PWM output
	timer_channel_complementary_output_state_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_G, TIMER_CCXN_ENABLE);
	timer_channel_complementary_output_state_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_B, TIMER_CCXN_ENABLE);
	timer_channel_complementary_output_state_config(TIMER_BLDC, TIMER_BLDC_CHANNEL_Y, TIMER_CCXN_ENABLE);
	
	// Enable TIMER_INT_UP interrupt and set priority
	nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn, 0, 0);
	timer_interrupt_enable(TIMER_BLDC, TIMER_INT_UP);
	
	// Enable the timer and start PWM
	timer_enable(TIMER_BLDC);
}

void usart_init(void) {
	// Enable ADC and DMA clock
	rcu_periph_clock_enable(RCU_USART1);
	rcu_periph_clock_enable(RCU_DMA);
	
	// Init USART for 115200 baud, 8N1
	usart_baudrate_set(USART_MASTERSLAVE, 115200);
	usart_parity_config(USART_MASTERSLAVE, USART_PM_NONE);
	usart_word_length_set(USART_MASTERSLAVE, USART_WL_8BIT);
	usart_stop_bit_set(USART_MASTERSLAVE, USART_STB_1BIT);
	usart_oversample_config(USART_MASTERSLAVE, USART_OVSMOD_16);
	
	// Enable both transmitter and receiver
	usart_transmit_config(USART_MASTERSLAVE, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART_MASTERSLAVE, USART_RECEIVE_ENABLE);
	
	// Enable USART
	usart_enable(USART_MASTERSLAVE);
	
	// Interrupt channel 3/4 enable
	nvic_irq_enable(DMA_Channel3_4_IRQn, 2, 0);
	
	// Initialize DMA channel 4 for USART_SLAVE RX
	dma_deinit(DMA_CH4);
	dma_init_struct_usart.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct_usart.memory_addr = (uint32_t)usartMasterSlave_rx_buf;
	dma_init_struct_usart.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct_usart.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct_usart.number = USART_COMMS_RX_BUFFERSIZE;
	dma_init_struct_usart.periph_addr = (uint32_t)(USART_RX_BUFFER);
	dma_init_struct_usart.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct_usart.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct_usart.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA_CH4, dma_init_struct_usart);
	
	// Configure DMA mode
	dma_circulation_enable(DMA_CH4);
	dma_memory_to_memory_disable(DMA_CH4);

	// USART DMA enable for transmission and receive
	usart_dma_receive_config(USART_MASTERSLAVE, USART_DENR_ENABLE);
	
	// Enable DMA transfer complete interrupt
	dma_interrupt_enable(DMA_CH4, DMA_CHXCTL_FTFIE);
	
	// At least clear number of remaining data to be transferred by the DMA 
	dma_transfer_number_config(DMA_CH4, 1);
	
	// Enable dma receive channel
	dma_channel_enable(DMA_CH4);
}