#ifndef CONFIG_H
#define CONFIG_H

#define PWM_FREQ         		16000     // PWM frequency in Hz
#define DEAD_TIME        		60        // PWM deadtime (60 = 1µs, measured by oscilloscope)

#define DC_CUR_LIMIT     		15        // Motor DC current limit in amps

#define SPEED_COEFFICIENT   -1
#define STEER_COEFFICIENT   1

#define DELAY_IN_MAIN_LOOP 	5         // Delay in ms

#endif