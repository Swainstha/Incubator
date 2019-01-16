/*
 * Timer.cpp
 *
 * Created: 8/29/2018 4:49:11 AM
 *  Author: swain
 */ 

#include "Timerr.h"
#include "Header.h"


void Timerr::setTimerNum(int timer_num) {
	this->timer_no = timer_num;
}

void Timerr::setTimerOverflow() {
	
	switch(this->timer_no) {
		case 1:
			SET_OVERFLOW(1);
			break;
		case 3:
			SET_OVERFLOW(3);
			break;
		case 4:
			SET_OVERFLOW(4);
			break;
		case 5:
			SET_OVERFLOW(5);
			break;
	}
	
}

void Timerr::startTimer() {
	switch(this->timer_no) {
		case 1:
		START_TIMERR(1,0,1);
		break;
		case 3:
		START_TIMERR(3,0,1);
		break;
		case 4:
		START_TIMERR(4,0,1);
		break;
		case 5:
		START_TIMERR(5,0,1);
		break;
	}
}

void Timerr::stopTimer() {
	
	switch(this->timer_no) {
		case 1:
		STOP_TIMER(1);
		break;
		case 3:
		STOP_TIMER(3);
		break;
		case 4:
		STOP_TIMER(4);
		break;
		case 5:
		STOP_TIMER(5);
		break;
	}
}

void Timerr::setCompareInterrupt() {
	switch(this->timer_no) {
		case 1:
		SET_COMPARE(1,A);
		break;
		case 3:
		SET_COMPARE(3,A);
		break;
		case 4:
		SET_COMPARE(4,A);
		break;
		case 5:
		SET_COMPARE(5,A);
		break;
	}
}

void Timerr::resetTimer() {
	if(this->timer_no == 5){
		TCNT5 = 0;
	} else if(this->timer_no == 3) {
		TCNT3 = 0;
	}
}
void Timerr::startCustomTimer(int milli_sec) {
	
	if(this->timer_no == 5) {
		int ocr = (15.625 * milli_sec);
		OCR5A = ocr;
		START_TIMERR(5,0,1); //for proteus use 5,0,1 and for real life use 5,0,2
	} else if(this->timer_no == 3) {
		int ocr = (15.625 * milli_sec);
		OCR3A = ocr;
		START_TIMERR(3,0,1); //for proteus use 3,0,1 and for real life use 3,0,2
	}
}
	
	