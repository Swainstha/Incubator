/*
 * PWM.h
 *
 * Created: 1/9/2019 7:54:10 AM
 *  Author: nic
 */ 


#ifndef PWM_H_
#define PWM_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

class PWM {
	public:
		int top=0;
		void init_pwm(int frequency); //in Hz
		void start_pwm(int duty_cycle);
		void set_pwm(int duty_cycle);
		void stop_pwm();
	};





#endif /* PWM_H_ */