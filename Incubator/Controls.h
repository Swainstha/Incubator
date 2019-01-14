/*
 * Controls.h
 *
 * Created: 1/13/2019 9:32:19 PM
 *  Author: nic
 */ 


#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <avr/io.h>
class Controls {
	public:
	void initControls();
	void startFan();
	void stopFan();
	void startHeater();
	void stopHeater();
	
	};




#endif /* CONTROLS_H_ */