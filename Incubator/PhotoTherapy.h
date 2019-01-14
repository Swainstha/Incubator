/*
 * PhotoTherapy.h
 *
 * Created: 1/9/2019 3:21:25 PM
 *  Author: nic
 */ 


#ifndef PHOTOTHERAPY_H_
#define PHOTOTHERAPY_H_

#include <avr/io.h>
#include <util/delay.h>
#include "PWM.h"



class PhotoTherapy {
	public:
		PWM pwm_out;
		void init_phototherapy(int frequency);
		void start_phototherapy(int duty_cycle);
		void change_brightness(int duty_cycle);
		void stop_phototherapy();
	};



#endif /* PHOTOTHERAPY_H_ */