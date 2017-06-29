/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 9 Exercise 3
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char indexNum = 0;
double note[16] = {261.63, 261.63, 392.00, 392.00, 440.00, 440.00, 392.00, 0.0, 349.23, 349.23, 329.63, 329.63, 293.66, 293.66, 261.63, 0.0};

enum States {ON, OFF, HOLD, WAIT}state;
	
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
	unsigned char tempA = ~PINA & 0x01;
	
	switch(state)
	{
		case OFF:
			if(tempA == 0x01)
			{
				indexNum = 0;
				state = ON;
			}
			else
			{
				state = OFF;
			}
			break;
		case ON:
			if(indexNum < 16)
			{
				state = ON;
			}
			else if(tempA == 0x01)
			{
				state = HOLD;
			}
			else
			{
				state = OFF;
			}
			break;
		case HOLD:
			if(tempA == 0x00)
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
		case ON:
			set_PWM(note[indexNum]);
			indexNum++;
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
	
	TimerOn();
	TimerSet(500);
	PWM_on();
	
	indexNum = 0;
	state = OFF;
	
    while (1) 
    {
		Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

