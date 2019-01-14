/*
 * PhotoTherapy.cpp
 *
 * Created: 1/9/2019 3:22:08 PM
 *  Author: nic
 */ 
#include "PhotoTherapy.h"


#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#define PHOTO_PORT PORTB
#define PHOTO_PIN PINB6

#define PHOTO_PWM_PORT PORTB
#define PHOTO_PWM_PIN PINB5

void PhotoTherapy::init_phototherapy(int frequency) {
	pwm_out.init_pwm(frequency);
	DDR(PHOTO_PORT) |= (1 << PHOTO_PIN);
	DDR(PHOTO_PWM_PORT) |= (1 << PHOTO_PWM_PIN);
	PHOTO_PWM_PORT &=~ (1 << PHOTO_PWM_PIN);
}

void PhotoTherapy::start_phototherapy(int duty_cycle) {
	
	PHOTO_PORT |= (1 << PHOTO_PIN);
	_delay_ms(100);
	pwm_out.start_pwm(duty_cycle);
	
}

void PhotoTherapy::change_brightness(int duty_cycle) {
	pwm_out.set_pwm(duty_cycle);
}

void PhotoTherapy::stop_phototherapy() {
	pwm_out.stop_pwm();
	_delay_ms(100);
	PHOTO_PORT &=~ (1 << PHOTO_PIN);
}