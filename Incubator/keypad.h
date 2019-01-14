/*
 * keypad.h
 *
 * Created: 8/30/2018 9:33:49 AM
 *  Author: swain
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define Pressed 1
#define Release 0

#include <avr/io.h>
#include <util/delay.h>
#include "Pins.h"

#define select_s1 KEYP&=~(1<<KEY1)
#define select_s2 KEYP&=~(1<<KEY2)
#define select_s3 KEYP&=~(1<<KEY3)
#define select_s4 KEYP&=~(1<<KEY4)
#define unselect_s1 KEYP|=(1<<KEY1)
#define unselect_s2 KEYP|=(1<<KEY2)
#define unselect_s3 KEYP|=(1<<KEY3)
#define unselect_s4 KEYP|=(1<<KEY4)
enum buttons_t {BUTTON_0,BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4,BUTTON_5,BUTTON_6,BUTTON_7,BUTTON_8,BUTTON_9,NO_BUTTON,BUTTON_HASH,BUTTON_AST,BUTTON_A,BUTTON_B,BUTTON_C,BUTTON_D};
unsigned int last_button=NO_BUTTON;

void keypad_init()
{
	//DDRB = 0xff;
	KEYDDR|=((1<<KEY1)|(1<<KEY4));
	KEYDDR|=((1<<KEY2)|(1<<KEY3));
	DDRA&=~((1<<KEYA)|(1<<KEYB)|(1<<KEYC)|(1<<KEYD));
	KEYP|=((1<<KEY1)|(1<<KEY2)|(1<<KEY3)|(1<<KEY4));
	PORTA|=((1<<KEYA)|(1<<KEYB)|(1<<KEYC)|(1<<KEYD));
}

unsigned int keypad_check()
{
	select_s1;
	unselect_s2;
	unselect_s3;
	unselect_s4;
	
	_delay_ms(15);
	if (bit_is_clear(KEYCC,KEYA) && bit_is_clear(KEY11,KEY1))
	{
		return BUTTON_1;
	}
	else if(bit_is_clear(KEYCC,KEYB) && bit_is_clear(KEY11,KEY1))
	{
		return BUTTON_4;
	}
	else if(bit_is_clear(KEYCC,KEYC) && bit_is_clear(KEY11,KEY1))
	{
		return BUTTON_7;
	}
	else if(bit_is_clear(KEYCC,KEYD) && bit_is_clear(KEY11,KEY1))
	{
		return BUTTON_AST;
	}
	unselect_s1;
	_delay_ms(5);

	select_s2;
	_delay_ms(15);
	if(bit_is_clear(KEYCC,KEYA) && bit_is_clear(KEY11,KEY2))
	{
		return BUTTON_2;
	}
	else if(bit_is_clear(KEYCC,KEYB) && bit_is_clear(KEY11,KEY2))
	{
		return BUTTON_5;
	}
	else if(bit_is_clear(KEYCC,KEYC) && bit_is_clear(KEY11,KEY2))
	{
		return BUTTON_8;
	}
	else if(bit_is_clear(KEYCC,KEYD) && bit_is_clear(KEY11,KEY2))
	{
		return BUTTON_0;
	}
	unselect_s2;
	_delay_ms(5);
	select_s3;
	_delay_ms(15);
	if(bit_is_clear(KEYCC,KEYA) && bit_is_clear(KEY11,KEY3))
	{
		return BUTTON_3;
	}
	else if(bit_is_clear(KEYCC,KEYB) && bit_is_clear(KEY11,KEY3))
	{
		return BUTTON_6;
	}
	else if(bit_is_clear(KEYCC,KEYC) && bit_is_clear(KEY11,KEY3))
	{
		return BUTTON_9;
	}
	else if(bit_is_clear(KEYCC,KEYD) && bit_is_clear(KEY11,KEY3))
	{
		return BUTTON_HASH;
	}
	unselect_s3;
	_delay_ms(5);
	select_s4;
	_delay_ms(15);
	if(bit_is_clear(KEYCC,KEYA) && bit_is_clear(KEY11,KEY4))
	{
		return BUTTON_A;
	}
	else if(bit_is_clear(KEYCC,KEYB) && bit_is_clear(KEY11,KEY4))
	{
		return BUTTON_B;
	}
	else if(bit_is_clear(KEYCC,KEYC) && bit_is_clear(KEY11,KEY4))
	{
		return BUTTON_C;
	}
	else if(bit_is_clear(KEYCC,KEYD) && bit_is_clear(KEY11,KEY4))
	{
		return BUTTON_D;
	}
	unselect_s4;
	_delay_ms(5);
	last_button=NO_BUTTON;
	return NO_BUTTON;
}
unsigned int get_status()
{
	select_s1;
	select_s2;
	select_s3;
	select_s4;
	if(bit_is_clear(KEYCC,KEYA))
	return Pressed;
	else if(bit_is_clear(KEYCC,KEYB))
	return Pressed;
	else if(bit_is_clear(KEYCC,KEYC))
	return Pressed;
	else if(bit_is_clear(KEYCC,KEYD))
	return Pressed;
	else
	return Release;
}
unsigned int get_key()
{
	if((get_status()==Pressed) && (last_button!=NO_BUTTON))
	{
		return last_button;
	} else
	{
		last_button=keypad_check();
		return last_button;
	}
}
#endif *//* KEYPAD_H_ */