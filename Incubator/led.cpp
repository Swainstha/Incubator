/*
 * led.cpp
 *
 * Created: 12/17/2018 11:19:50 PM
 *  Author: swain
 */ 

#include "led.h"
#include "Pins.h"
#include <avr/io.h>

#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

void led::led_init() {
	DDR(TS_HIGH_PORT) |= (1 << TS_HIGH_PIN);
	DDR(TS_LOW_PORT) |= (1 << TS_LOW_PIN);
	DDR(RH_LOW_PORT) |= (1 << RH_LOW_PIN);	
	DDR(TA_HIGH_PORT) |= (1 << TA_HIGH_PIN);
	DDR(TA_LOW_PORT) |= (1 << TA_LOW_PIN);
	DDR(TIMER_RUNNING_LED_PORT) |= (1 << TIMER_RUNNING_LED_PIN);
	DDR(TIMER_STOP_LED_PORT) |= (1 << TIMER_STOP_LED_PIN);
	DDR(DOOR_CLOSED_LED_PORT) |= (1 << DOOR_CLOSED_LED_PIN);
	DDR(KEYPAD_ENABLE_LED_PORT) |= (1 << KEYPAD_ENABLE_LED_PIN);
	DDR(SYSTEM_FAULT_LED_PORT) |= (1 << SYSTEM_FAULT_LED_PIN);
	DDR(FAN_ON_LED_PORT) |= (1 << FAN_ON_LED_PIN);
	DDR(HEATER_ON_LED_PORT) |= (1 << HEATER_ON_LED_PIN);
}


void led::led_do(int led_num, int state) {
	switch(led_num) {
		case TS_HIGH_LED:
			REGISTER_LED(TS_HIGH_PORT, TS_HIGH_PIN, state);
			break;
		case TS_LOW_LED:
			REGISTER_LED(TS_LOW_PORT, TS_LOW_PIN, state);
			break;
		case RH_HIGH_LED:
			REGISTER_LED(RH_HIGH_PORT, RH_HIGH_PIN, state);
			break;
		case RH_LOW_LED:
			REGISTER_LED(RH_LOW_PORT, RH_LOW_PIN, state);
			break;
		case TA_HIGH_LED:
			REGISTER_LED(TA_HIGH_PORT, TA_HIGH_PIN, state);
			break;
		case TA_LOW_LED:
			REGISTER_LED(TA_LOW_PORT, TA_LOW_PIN, state);
			break;
		case TIMER_RUNNING_LED:
			REGISTER_LED(TIMER_RUNNING_LED_PORT, TIMER_RUNNING_LED_PIN, state);
			break;
		case DOOR_CLOSED_LED:
			REGISTER_LED(DOOR_CLOSED_LED_PORT, DOOR_CLOSED_LED_PIN, state);
			break;
		case TIMER_STOP_LED:
			REGISTER_LED(TIMER_STOP_LED_PORT, TIMER_STOP_LED_PIN, state);
			break;
		case KEYPAD_ENABLE_LED:
			REGISTER_LED(KEYPAD_ENABLE_LED_PORT, KEYPAD_ENABLE_LED_PIN, state);
			break;
		case SYSTEM_FAULT_LED:
			REGISTER_LED(SYSTEM_FAULT_LED_PORT, SYSTEM_FAULT_LED_PIN, state);
			break;
		case FAN_ON_LED:
			REGISTER_LED(FAN_ON_LED_PORT, FAN_ON_LED_PIN, state);
			break;
		case HEATER_ON_LED:
			REGISTER_LED(HEATER_ON_LED_PORT, HEATER_ON_LED_PIN, state);
			break;
		
	}
}
