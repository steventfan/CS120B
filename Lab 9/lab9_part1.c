/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 9 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

enum States {OFF, NOTE_C, NOTE_D, NOTE_E, HOLD}state;

//NOTE*** THIS NEW CODE TARGETS PB6 NOT PB3

void set_PWM(double frequency) {
	
	
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void Tick()
{
	unsigned char tempA = ~PINA & 0x07;
	
	switch(state)
	{
		case OFF:
			if(tempA == 0x01)
			{
				state = NOTE_C;
			}
			else if(tempA == 0x02)
			{
				state = NOTE_D;
			}
			else if(tempA == 0x04)
			{
				state = NOTE_E;
			}
			else
			{
				state = OFF;
			}
			break;
		case NOTE_C:
			if(tempA == 0x01)
			{
				state = NOTE_C;
			}
			else if(tempA == 0x02)
			{
				state = NOTE_D;
			}
			else if(tempA == 0x04)
			{
				state = NOTE_E;
			}
			else if(tempA == 0x00)
			{
				state = OFF;
			}
			else
			{
				state = HOLD;
			}
			break;
		case NOTE_D:
			if(tempA == 0x01)
			{
				state = NOTE_C;
			}
			else if(tempA == 0x02)
			{
				state = NOTE_D;
			}
			else if(tempA == 0x04)
			{
				state = NOTE_E;
			}
			else if(tempA == 0x00)
			{
				state = OFF;
			}
			else
			{
				state = HOLD;
			}
			break;
		case NOTE_E:
			if(tempA == 0x01)
			{
				state = NOTE_C;
			}
			else if(tempA == 0x02)
			{
				state = NOTE_D;
			}
			else if(tempA == 0x04)
			{
				state = NOTE_E;
			}
			else if(tempA == 0x00)
			{
				state = OFF;
			}
			else
			{
				state = HOLD;
			}
			break;
		case HOLD:
			if(tempA == 0x01)
			{
				state = NOTE_C;
			}
			else if(tempA == 0x02)
			{
				state = NOTE_D;
			}
			else if(tempA == 0x04)
			{
				state = NOTE_E;
			}
			else if(tempA == 0x00)
			{
				state = OFF;
			}
			else
			{
				state = HOLD;
			}
			break;
		default:
			state = OFF;
			break;
	}
	
	switch(state)
	{
		case OFF:
			set_PWM(0);
			break;
		case NOTE_C:
			set_PWM(261.63);
			break;
		case NOTE_D:
			set_PWM(293.66);
			break;
		case NOTE_E:
			set_PWM(329.63);
			break;
		case HOLD:
			set_PWM(0);
			break;
		default:
			set_PWM(0);
			break;
	}
}

void main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	PWM_on();
	
	state = OFF;
	
    while (1) 
    {
		Tick();
	}
}

