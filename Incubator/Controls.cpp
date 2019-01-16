/*
 * Controls.cpp
 *
 * Created: 1/13/2019 9:34:59 PM
 *  Author: nic
 */ 
#include "Controls.h"


void Controls::initControls(){
	DDR(FAN_PORT) |= (1 << FAN_PORT);
	DDR(HEATER_PORT) |= (1 << HEATER_PORT);
	DDRB |= (1 << PINB7) | (1 << PINB4);
	
}

void Controls::startFan(){
	FAN_PORT |= (1 << FAN_PIN);
	PORTB |= (1 << PINB7) | (1 << PINB4);
}

void Controls::startHeater() {
	HEATER_PORT |= (1 << HEATER_PIN);
}

void Controls::stopFan() {
	FAN_PORT &=~ (1 << FAN_PIN);
	PORTB &=~ ((1 << PINB7) | (1 << PINB4));
}

void Controls::stopHeater() {
	HEATER_PORT &=~ (1 << HEATER_PIN);
}