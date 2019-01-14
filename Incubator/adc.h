/*
 * adc.h
 *
 * Created: 9/5/2018 10:51:45 PM
 *  Author: swain
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000UL

#include <avr/io.h>


void setADC()
{
	ADCSRA|=(1<<ADEN);  //enable interrupts
	ADMUX|=(1<<REFS0);	//AVcc with external capacitor at AREF
	//ADMUX|=(1<<ADLAR); //data left adjusted and reading only the 8 high bits
	ADMUX&=~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4));   //selecting the ADC0 pin for input
	ADCSRA|=((1<<ADPS1)|(1<<ADPS2)); //setting adc clock cycle to 125kHz
	ADCSRA|=(1<<ADPS0);
	//first reading known as dummy reading usually taken to test ADC
	ADCSRA|=(1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	ADCSRA&=~(1<<ADEN);
	
}
uint16_t adc_read(uint8_t ch)
{
	ch&=0b00000111;		//channel must be b/w 0 and 7
	ADMUX=(ADMUX & 0XF8)|ch;			//selecting channel
	ADCSRA|=(1<<ADEN)|(1<<ADSC);	//start conversion
	//_delay_ms(2);
	while(ADCSRA &(1<<ADSC));	//waiting for ADIF,conversion complete
	return(ADC);
}
uint16_t adcRead(uint8_t ch)
{
	uint16_t sensorSum=0,mean=0;
	//unsigned char wrongCount=0;
	unsigned char correctCount=0;
	uint16_t sensorSample[3]={0,0,0};
	for(unsigned char k=0;k<3;k++)
	{
		sensorSample[k]=adc_read(ch);
		sensorSum+=sensorSample[k];
	}
	mean=sensorSum/3;
	sensorSum=0;
	for(unsigned char k=0;k<3;k++)
	{
		if(abs(mean-sensorSample[k])<(0.5*mean))
		{
			sensorSum+=sensorSample[k];
			correctCount++;
		}
	}
	if(correctCount)
	return (sensorSum/correctCount);
	return mean;
}




#endif /* ADC_H_ */