/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 8 Exercise 4
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

void main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	ADC_init();
	unsigned short input = 0x00;
	unsigned char tempC = 0x00;
	
    while (1) 
    {
		input = ADC;
		tempC = 0x00;
		if(input - 48 >= 0)
		{
			tempC = 0x01;
		}
		if(input - 48 >= 6)
		{
			tempC = 0x03;
		}
		if(input - 48 >= 12)
		{
			tempC = 0x07;
		}
		if(input - 48 >= 18)
		{
			tempC = 0x0F;
		}
		if(input - 48 >= 24)
		{
			tempC = 0x1F;
		}
		if(input - 48 >= 30)
		{
			tempC = 0x3F;
		}
		if(input - 48 >= 36)
		{
			tempC = 0x7F;
		}
		if(input - 48 >= 42)
		{
			tempC = 0xFF;
		}
		
		PORTC = tempC;
    }
}

