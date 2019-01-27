/*
 * Incubator.cpp
 *
 * Created: 9/4/2018 6:28:45 AM
 *  Author: swain
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#include "Header.h"
#include "Timerr.h"
#include "Pins.h"
#include "adc.h"
#include "led.h"
#include "PhotoTherapy.h"
#include "Controls.h"

int MAX = 700;
int MIN = 300;

void init_devices();
void setADC();
uint16_t adc_read(uint8_t ch);
void display();
void set(int key);
void check();
void update_time();
float calculateHeaterOffTime(float airTemp);

void Request();
void Response();
uint8_t Receive_data();		
void readTempHumid(float &temper, float &humid);

volatile int second = 30;
volatile int minute = 3;
volatile int hour = 0;

int time[6] = {0,0,0,0,0,0};
int count = 0;
int lcd_pos = 0;

int incubator_state = 1; //1: idle, 0: stop, 2: start
bool timer_set = false;

bool timer_state = false;
bool door_closed = true;
bool incubator_start = false;
bool incubator_running = false;
bool photo_start = false;

bool pressed_photo_start = false;
bool pressed_photo_stop = false;

bool pressed_incubator_start = false;
bool pressed_incubator_stop = false;

bool pressed_restart_system = false;
bool pressed_system_fault = false;
bool second_lapsed = false;

bool keypad_enable = false;
bool pressed_keypad = false;

bool system_fault = false;
bool restart_system = false;
float humidity=0;
float temperature = 0;
float air_temp = 0.0, skin_temp = 0.0;
int last_key = 0;

int heaterOnTime = 60;
int heaterOffTime = 0;

int const heaterOffTimeMin = 2;
int const heaterOffTimeMax = 12; 

int optimumTemp = 35;

volatile int heaterCount = heaterOnTime;
volatile bool heatOn = true;
volatile bool heaterCountUpdate = false; 

#define DHT11_PIN PINF4

uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

Timerr timerr;
Timerr timerHeater;
PhotoTherapy phototherapy;
led leds;
Controls controls;

Timerr tempTimer;
int main(void)
{
	sei();
	init_devices();
	
	
	/*leds.led_do(TS_HIGH_LED, ON);
	leds.led_do(TS_LOW_LED, ON);
	leds.led_do(RH_HIGH_LED, ON);
	leds.led_do(TA_HIGH_LED, ON);
	leds.led_do(TA_LOW_LED, ON);
	leds.led_do(TIMER_RUNNING_LED, ON);
	leds.led_do(DOOR_CLOSED_LED, ON);
	leds.led_do(TIMER_STOP_LED, ON);
	leds.led_do(KEYPAD_ENABLE_LED, ON);
	leds.led_do(SYSTEM_FAULT_LED, ON);
	leds.led_do(FAN_ON_LED, ON);
	leds.led_do(HEATER_ON_LED, ON);
	*/
	
	
	
	Printf(1,"TA:%fC   TS:%fC",air_temp,skin_temp);
	Printf(2,"RH:%f%",humidity);
	Printf(3,"00:00:00");
    while(1)
    {
		if(second_lapsed) {
			readTempHumid(temperature, humidity);
			
			int adc2 = adc_read(LM35_1);
			air_temp = ((adc2 / 1024.0) * 5) * 100;
			
			int adc1 = adc_read(LM35_2);
			skin_temp = ((adc1 / 1024.0)*5) * 100; //10mv per degree celsius
			Printf(2, "T:%fC RH:%f%",temperature, humidity);
			
			lcd_gotoxy(3,0);
			Printf(5,"%fC",air_temp);
			lcd_gotoxy(13,0);
			Printf(5,"%fC",skin_temp);
		}
		display();
		check();
		
		/*For Led */
		int adc = adc_read(POT);
		
		if(incubator_running || photo_start) {
			int dutycycle = (adc/1024.0) * 100;
			phototherapy.change_brightness(dutycycle);
			
		}
		//air_temp = adc_read(LM35_1);
		
		/*_delay_ms(20);
		
		humidity = adc_read(HUMIDITY);
		*/
		
		
		if(incubator_running) {
			update_time();
			if(heaterCountUpdate) {
				heaterCountUpdate = false;
				heaterCount--;
				if(temperature > 34) {
					heaterOnTime = 10;
				} else if(temperature <30) {
					heaterOnTime = 60;
				} else {
					heaterOnTime = 20;
				}
				if(heaterCount <= 0 && heatOn) {
					heaterCount = calculateHeaterOffTime(temperature);
					//lcd_gotoxy(12,3);
					Printf(4, "Off time: %d", heaterCount);
					heatOn = false;
					controls.stopHeater();
					leds.led_do(HEATER_ON_LED, OFF);
				} else if(heaterCount <= 0 && !heatOn) {
					heaterCount = heaterOnTime;
					heatOn = true;
					controls.startHeater();
					leds.led_do(HEATER_ON_LED, ON);
				}
			}
		} 
		//display();
		if(keypad_enable) {
			int key = get_key();
			if(key != last_key) {
				last_key = key;
				switch(key) {
					case BUTTON_A:
						timer_set = !timer_set;
						break;
					case BUTTON_1:
					case BUTTON_2:
					case BUTTON_3:
					case BUTTON_4:
					case BUTTON_5:
					case BUTTON_6:
					case BUTTON_7:
					case BUTTON_8:
					case BUTTON_9:
					case BUTTON_0:
						
						if(timer_set) {
							set(key);
						}
						break;
			
				}
			} 
		} 
    }
}

void init_devices() {
	
	setADC();
	keypad_init();
	lcd_init();
	lcd_clear();
	leds.led_init();
	
	phototherapy.init_phototherapy(4000);
	
	controls.initControls();
	
	timerr.setTimerNum(5);
	timerr.setCompareInterrupt();
	
	//tempTimer.setTimerNum(5);
	timerHeater.setTimerNum(3);
	timerHeater.setCompareInterrupt();
}


void display(){
	//lcd_gotoxy(3,0);
	//Printf(5,"%fC",air_temp);
	//lcd_gotoxy(13,0);
	//Printf(5,"%fC",skin_temp);
	//lcd_gotoxy(3,1);
	//Printf(5,"%f%",humidity);
	//lcd_gotoxy(10,1);
	//Printf(5, "T:%f", temperature);
	if(hour == 0 && minute == 0 && second == 0) {
		Printf(3,"00:00:00");
	} else if(hour < 10 && minute < 10 && second < 10) {
		Printf(3,"0%d:0%d:0%d",hour, minute, second);
	} else if(hour < 10 && minute < 10) {
		Printf(3,"0%d:0%d:%d",hour, minute, second);
	} else if(hour < 10 && second < 10) {
		Printf(3,"0%d:%d:0%d",hour, minute, second);
	} else if(minute < 10 && second < 10) {
		Printf(3,"%d:0%d:0%d",hour, minute, second);
	} else if(minute < 10) {
		Printf(3,"%d:0%d:%d",hour, minute, second);
	} else if(second < 10) {
		Printf(3,"%d:%d:0%d",hour, minute, second);
	} else if(hour < 10) {
		Printf(3,"0%d:%d:%d",hour, minute, second);
	}else {
		Printf(3,"%d:%d:%d",hour, minute, second);
	}
	
	if(timer_set) {
		lcd_gotoxy(lcd_pos,4);
	}
}

void set(int key) {
	time[count] = key;
	count++;
	hour = hour = time[0] * 10 + time[1];
	if(hour < 0 || hour > 23) {
		count = 0;
		lcd_pos = -1;
	}
	minute = time[2] * 10 + time[3];
	if(minute > 59 || minute < 0) {
		count = 2;
	}
	second = time[4] * 10 + time[5];
	if(second > 59 || second < 0) {
		count = 4;
	}
	
	if(count == 2) {
		lcd_pos = count + 1;
	} else if(count == 4) {
		lcd_pos = count + 2;
	}else {
		lcd_pos++;
	}
	if(count == 6) {
		count = 0;
		lcd_pos = 0;
	}
}

ISR(TIMER5_COMPA_vect) {
	TCNT5 = 0;
	second_lapsed = true;
}

ISR(TIMER3_COMPA_vect) {
	TCNT3 = 0;
	heaterCountUpdate = true;
}

void check() {
	
	if(temperature > 35) {
		leds.led_do(TA_LOW_LED,OFF);
		leds.led_do(TA_HIGH_LED,ON);
		//REGISTER_SET1(LEDP,LED0);
		} else if(temperature < 31) {
			leds.led_do(TA_HIGH_LED,OFF);
			leds.led_do(TA_LOW_LED, ON);
		//REGISTER_SET1(LEDP,LED1);
		} else {
			leds.led_do(TA_HIGH_LED,OFF);
			leds.led_do(TA_LOW_LED, OFF);
		//REGISTER_RESET(LEDP, LED0);
		//REGISTER_RESET(LEDP, LED1);
	}
	
	if(skin_temp > 35) {
		leds.led_do(TS_LOW_LED,OFF);
		leds.led_do(TS_HIGH_LED,ON);
		//REGISTER_SET1(LEDP,LED2);
		} else if(skin_temp < 31) {
			leds.led_do(TS_HIGH_LED,OFF);
			leds.led_do(TS_LOW_LED, ON);
		//REGISTER_SET1(LEDP,LED3);
		} else {
			leds.led_do(TS_HIGH_LED,OFF);
			leds.led_do(TS_LOW_LED, OFF);
		//REGISTER_RESET(LEDP, LED2);
		//REGISTER_RESET(LEDP, LED3);
	}
	
	
	if(humidity >60) {
		leds.led_do(RH_LOW_LED,OFF);
		leds.led_do(RH_HIGH_LED, ON);
		//REGISTER_SET1(LEDP,LED4);
		} else if(humidity < 35) {
			leds.led_do(RH_LOW_LED,ON);
			leds.led_do(RH_HIGH_LED, OFF);
		//REGISTER_SET1(LEDP, LED5);
		} else {
			leds.led_do(RH_HIGH_LED,OFF);
			leds.led_do(RH_LOW_LED, OFF);
		//REGISTER_RESET(LEDP, LED4);
		//REGISTER_RESET(LEDP, LED5);
	}
	
	
	/*if(bit_is_clear(PINE, PINE2)) {
		door_closed = true;
		DOOR_CLOSED;
		} else {
		door_closed = false;
		photo_start = false;
		incubator_state = false;
		system_fault = true;
		DOOR_OPEN;
		STOP_PHOTO;
		INCUBATOR_STOP;
		STOP_FAN;
		STOP_HEATER;
	}
	
	if(!door_closed && incubator_running) {
		INCUBATOR_STOP;
		STOP_FAN;
		STOP_HEATER;
		incubator_running = false;
		timerr.stopTimer();
	}*/
	
	if(bit_is_clear(KEYPAD_BUTTON_PORT, KEYPAD_BUTTON_PIN) && !pressed_keypad) {
		
		//lcd_clear();
		
		keypad_enable = !keypad_enable;
		Printf(4, "Keypad %b", keypad_enable);
		if(keypad_enable) {
			leds.led_do(KEYPAD_ENABLE_LED, ON);
		} else {
			leds.led_do(KEYPAD_ENABLE_LED, OFF);
		}
		pressed_keypad = true;
	} else {
		pressed_keypad = false;
	}
	
	if(bit_is_clear(START_PHOTO_BUTTON_PORT, START_PHOTO_BUTTON_PIN) && !pressed_photo_start) {
		
		//lcd_clear();
		Printf(4, "START PHOTO");
		if(door_closed) {
			photo_start = true;
			phototherapy.start_phototherapy(50);
		}
		pressed_photo_start = true;
		} else if(bit_is_set(START_PHOTO_BUTTON_PORT, START_PHOTO_BUTTON_PIN)) {
		pressed_photo_start = false;
	}
	
	if(bit_is_clear(STOP_PHOTO_BUTTON_PORT, STOP_PHOTO_BUTTON_PIN) && !pressed_photo_stop) {
		//lcd_clear();
		Printf(4,"STOP PHOTO");
		
		photo_start = false;
		phototherapy.stop_phototherapy();
		pressed_photo_stop = true;
		} else if(bit_is_set(STOP_PHOTO_BUTTON_PORT, STOP_PHOTO_BUTTON_PIN)) {
		pressed_photo_stop = false;
	}
	
	if(bit_is_clear(START_INCUBATOR_BUTTON_PORT, START_INCUBATOR_BUTTON_PIN) && !pressed_incubator_start) {
		
		//lcd_clear();
		Printf(4, "START INCUBATOR");
		
		if(door_closed) {  //is door closed
			controls.startFan();
			controls.startHeater();
			
			heaterCount = heaterOnTime;
			heatOn = true;
			timerHeater.startCustomTimer(1000);
			
			incubator_running = true;
			timer_state = true;
			phototherapy.start_phototherapy(50);
			photo_start = true;
			timer_set = false;
			timerr.startCustomTimer(1000);
			leds.led_do(TIMER_RUNNING_LED, ON);
			leds.led_do(HEATER_ON_LED, ON);
			leds.led_do(FAN_ON_LED, ON);
		}
		pressed_incubator_start = true;
		} else if(bit_is_set(START_INCUBATOR_BUTTON_PORT, START_INCUBATOR_BUTTON_PIN)) {
		pressed_incubator_start = false;
	}
	
	if(!timer_state && incubator_running) {
		phototherapy.stop_phototherapy();
		incubator_running = false;
		timerr.stopTimer();
		leds.led_do(TIMER_RUNNING_LED, OFF);
		leds.led_do(HEATER_ON_LED, OFF);
		leds.led_do(FAN_ON_LED, OFF);
		
		heaterCount = heaterOnTime;
		heatOn = true;
		timerHeater.stopTimer();
		
		photo_start = false;
		controls.stopFan();
		controls.stopHeater();
	}
	
	if(bit_is_clear(STOP_INCUBATOR_BUTTON_PORT, STOP_INCUBATOR_BUTTON_PIN) && !pressed_incubator_stop) {
		
		//lcd_clear();
		Printf(4, "STOP INCUBATOR");
		incubator_running = false;
		photo_start = false;
		phototherapy.stop_phototherapy();
		timerr.stopTimer();
		pressed_incubator_stop = true;
		leds.led_do(TIMER_RUNNING_LED, OFF);
		leds.led_do(HEATER_ON_LED, OFF);
		leds.led_do(FAN_ON_LED, OFF);
		controls.stopFan();
		controls.stopHeater();
		
		heaterCount = heaterOnTime;
		heatOn = true;
		timerHeater.stopTimer();
		
		} else if(bit_is_set(STOP_INCUBATOR_BUTTON_PORT, STOP_INCUBATOR_BUTTON_PIN)) {
		pressed_incubator_stop = false;
	}
	
	/*if(bit_is_clear(PINA, PINA4) && !pressed_system_fault) {
		pressed_system_fault = true;
		system_fault = true;
		incubator_running = false;
		//INCUBATOR_STOP;
		//STOP_FAN;
		//STOP_HEATER;
		timerr.stopTimer();
	} else if(bit_is_clear(PINA, PINA4)) {
		pressed_system_fault = false;
	}*/
	
	if(bit_is_clear(RESET_BUTTON_PORT, RESET_BUTTON_PIN) && !pressed_restart_system) {
		//lcd_clear();
		Printf(4, "RESET");
		pressed_restart_system = true;
		system_fault = true; //for testing purpose only
		if(system_fault && door_closed) {
			system_fault = false;
			restart_system = true;
			incubator_running = true;
			timer_state = true;
			phototherapy.start_phototherapy(50);
			photo_start = true;
			timer_set = false;
			timerr.startCustomTimer(1000);
			leds.led_do(TIMER_RUNNING_LED, ON);
			leds.led_do(HEATER_ON_LED, ON);
			leds.led_do(FAN_ON_LED, ON);
			
			controls.startFan();
			controls.startHeater();
			heaterCount = heaterOnTime;
			heatOn = true;
			timerHeater.startCustomTimer(1000);
		}
	} else if(bit_is_clear(RESET_BUTTON_PORT, RESET_BUTTON_PIN)){
		pressed_restart_system = false;
	}
}

void update_time() {
	if(second_lapsed) {
		second_lapsed = false;
		second--;
		if(second < 0) {
			minute--;
			second = 59;
			if(minute < 0) {
				hour--;
				minute = 59;
				if(hour < 0) {
					second = 0;
					minute = 0;
					hour = 0;
					timer_state = false;
				}
			}
		}
	}
}

float calculateHeaterOffTime (float airTemp) {
	float kp = 0.5;
	float heaterOffTimePID = heaterOffTimeMax - ((optimumTemp - airTemp) * kp);
	if(heaterOffTimePID > heaterOffTimeMax) {
		heaterOffTimePID = heaterOffTimeMax;
	} else if( heaterOffTimePID < heaterOffTimeMin) {
		heaterOffTimePID = heaterOffTimeMin;
	}
	return heaterOffTimePID;
	
}

void Request()				/* Microcontroller send start pulse/request */
{
	DDRF |= (1<<DHT11_PIN);
	PORTF &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(5);			/* wait for 20ms */
	PORTF |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response()				/* receive response from DHT11 */
{
	int count = 0;
	DDRF &= ~(1<<DHT11_PIN);
	while(PINF & (1<<DHT11_PIN)){
		count++;
		_delay_us(1);
		if(count> 85)
		break;
	}
	count = 0;
	while((PINF & (1<<DHT11_PIN))==0) {
		count++;
		_delay_us(1);
		if(count> 85)
		break;
	}
	count = 0;
	while(PINF & (1<<DHT11_PIN)) {
		count++;
		_delay_us(1);
		if(count> 85)
		break;
	}
}

uint8_t Receive_data()			/* receive data */
{
	for (int q=0; q<8; q++)
	{
		int count = 0;
		while((PINF & (1<<DHT11_PIN)) == 0)  /* check received bit 0 or 1 */
		{
			count++;
			_delay_us(1);
			if(count > 55)
			return 0;
		}
		_delay_us(30);
		if(PINF & (1<<DHT11_PIN))/* if high pulse is greater than 30us */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		count = 0;
		while(PINF & (1<<DHT11_PIN)){
			count++;
			_delay_us(1);
			if(count > 45)
			return 0;
		}
	}
	return c;
}

void readTempHumid(float &temper, float &humid) {
	Request();		/* send start pulse */
	Response();		/* receive response */
	I_RH=Receive_data();	/* store first eight bit in I_RH */
	D_RH=Receive_data();	/* store next eight bit in D_RH */
	I_Temp=Receive_data();	/* store next eight bit in I_Temp */
	D_Temp=Receive_data();	/* store next eight bit in D_Temp */
	CheckSum=Receive_data();/* store next eight bit in CheckSum */
	
	uint16_t temp16 = 0;
	uint16_t humidity16 = 0;
	_8bitTo16bit(temp16, I_Temp, D_Temp);
	_8bitTo16bit(humidity16, I_RH, D_RH);
	if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
	{
		Printf(4, "Not okay");
		} else {
		//Printf(4, "Yes Okay");
	}
	temper = temp16 / 10.0;
	humid = humidity16 / 10.0; 
}