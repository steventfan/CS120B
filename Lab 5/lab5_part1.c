/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 5 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

enum States {ONE, TWO, THREE}state;

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

void Sequencing()
{
	switch(state)
	{
		case ONE:
			state = TWO;
			break;
		case TWO:
			state = THREE;
			break;
		case THREE:
			state = ONE;
			break;
		default:
			state = ONE;
			break;
	}
}

void Outputs()
{
	unsigned char tempC = 0x00;
	
	switch(state)
	{
		case ONE:
			tempC = 0x01;
			break;
		case TWO:
			tempC = 0x02;
			break;
		case THREE:
			tempC = 0x04;
			break;
		default:
			tempC = 0x01;
			break;
	}
	
	PORTC = tempC;
}

int main()
{
	DDRC = 0xFF;
	PORTC = 0x00;
	
	TimerSet(1000);
	TimerOn();
	
	state = THREE;
	
	while(1)
	{
		Sequencing();
		Outputs();
		
		while (!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}
