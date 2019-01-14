/*
 * Controls.cpp
 *
 * Created: 1/13/2019 9:34:59 PM
 *  Author: nic
 */ 
#include "Controls.h"
#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#define FAN_PORT PORTB
#define FAN_PIN PINB7

#define HEATER_PORT PORTB
#define HEATER_PIN PINB4
void Controls::initControls(){
	DDR(FAN_PORT) |= (1 << FAN_PORT);
	DDR(HEATER_PORT) |= (1 << HEATER_PORT);
}

void Controls::startFan(){
	FAN_PORT |= (1 << FAN_PIN);
}

void Controls::startHeater() {
	HEATER_PORT |= (1 << HEATER_PIN);
}

void Controls::stopFan() {
	FAN_PORT &=~ (1 << FAN_PIN);
}

void Controls::stopHeater() {
	HEATER_PORT &=~ (1 << HEATER_PIN);
}