/*
 * Controls.h
 *
 * Created: 1/13/2019 9:32:19 PM
 *  Author: nic
 */ 


#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <avr/io.h>
#include "Pins.h"
#include "Header.h"

class Controls {
	public:
	void initControls();
	void startFan();
	void stopFan();
	void startHeater();
	void stopHeater();
	
	};




#endif /* CONTROLS_H_ */