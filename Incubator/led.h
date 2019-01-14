/*
 * led.h
 *
 * Created: 12/17/2018 11:17:58 PM
 *  Author: swain
 */ 


#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define OFF 0
#define ON 1

#define TS_LOW_LED 0
#define TS_HIGH_LED 1
#define RH_LOW_LED 2
#define RH_HIGH_LED 3
#define TA_LOW_LED 4
#define TA_HIGH_LED 5
#define TIMER_RUNNING_LED 6
#define DOOR_CLOSED_LED 7
#define TIMER_STOP_LED 8
#define KEYPAD_ENABLE_LED 9
#define SYSTEM_FAULT_LED 10
#define FAN_ON_LED 11
#define HEATER_ON_LED 12

class led {
	public:
		void led_init();
		void led_do(int led_num, int state);
	};



#endif /* LED_H_ */