/*
 * CPPFile1.cpp
 *
 * Created: 1/9/2019 7:52:01 AM
 *  Author: nic
 */ 

#include "PWM.h"

void PWM::init_pwm(int frequency) {
	TCCR1A |= (1 << COM1A1); //clear on compare match
	TCCR1A |= (1 << WGM11); ////for fast pwm whose max is decided by ICR1
	TCCR1B |= (1 << WGM13) | (1 <<WGM12); //for fast pwm whose max is decided by ICR1
	//TIMSK1 |= (1 << ICIE1) | (1 << OCIE1A); //interrupt enable for output compare match
	TCCR1B |= (1 << CS11); //prescalar 8
	//top = (F_CPU/(8 * frequency * 1.0)) - 1;
	top = 2000;
	ICR1 = top;
	OCR1A = 0;
	
}

void PWM::set_pwm(int duty_cycle) {
	OCR1A = (duty_cycle/ 100.0) * top;
}

void PWM::start_pwm(int duty_cycle) {
	
	PWM::set_pwm(duty_cycle); 
}

void PWM::stop_pwm() {
	//TCCR1B &=~ ((1 << CS12) | (1 << CS11) | (1 << CS10));
	OCR1A = 0;
}