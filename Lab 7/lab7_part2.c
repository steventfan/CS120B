/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 7 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char threeLEDS = 0;
unsigned char blinkingLED = 0;

enum States_1 {INITIAL_1, ZERO_1, ONE_1, TWO_1}state_1;
enum States_2 {INITIAL_2, ON_2, OFF_2}state_2;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void State1()
{
	switch(state_1)
	{
		case INITIAL_1:
			state_1 = ZERO_1;
			break;
		case ZERO_1:
			state_1 = ONE_1;
			break;
		case ONE_1:
			state_1 = TWO_1;
			break;
		case TWO_1:
			state_1 = ZERO_1;
			break;
		default:
			state_1 = ZERO_1;
			break;
	}
}

void State2()
{
	switch(state_2)
	{
		case INITIAL_2:
			state_2 = ON_2;
			break;
		case ON_2:
			state_2 = OFF_2;
			break;
		case OFF_2:
			state_2 = ON_2;
			break;
		default:
			state_2 = ON_2;
			break;
	}
}

void Outputs()
{
	switch(state_1)
	{
		case INITIAL_1:
			threeLEDS = 0x00;
			break;
		case ZERO_1:
			threeLEDS = 0x01;
			break;
		case ONE_1:
			threeLEDS = 0x02;
			break;
		case TWO_1:
			threeLEDS = 0x04;
			break;
		default:
			threeLEDS = 0x00;
			break;
	}
	
	switch(state_2)
	{
		case INITIAL_2:
			blinkingLED = 0x00;
			break;
		case ON_2:
			blinkingLED = 0x08;
			break;
		case OFF_2:
			blinkingLED = 0x00;
			break;
		default:
			blinkingLED = 0x00;
			break;
	}
	
	PORTC = threeLEDS | blinkingLED;
}

int main()
{
	DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(100);
	TimerOn();
	
	state_1 = INITIAL_1;
	state_2 = INITIAL_2;
	unsigned char count1 = 0;
	unsigned char count2 = 0;
	
	while(1)
	{
		if(count1 >= 3)
		{
			State1();
			count1 = 0;
		}
		else
		{
			count1++;
		}
		if(count2 >= 10)
		{
			State2();
			count2 = 0;
		}
		else
		{
			count2++;
		}
		Outputs();
		
		while(!TimerFlag);
		TimerFlag = 0;
	
	}
	
	return 0;
}
