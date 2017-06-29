/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 9 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char note = 0;

enum States_Frequency {WAIT, INCREMENT, DECREMENT, HOLD}state_frequency;
enum States_Toggle {OFF, ON}state_toggle;
	
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

void Tick_Frequency()
{
	unsigned char tempA = ~PINA & 0x05;
	
	switch(state_frequency)
	{
		case WAIT:
			if(tempA == 0x01)
			{
				state_frequency = INCREMENT;
			}
			else if(tempA == 0x04)
			{
				state_frequency = DECREMENT;
			}
			else if(tempA == 5)
			{
				state_frequency = HOLD;
			}
			else
			{
				state_frequency = WAIT;
			}
			break;
		case INCREMENT:
			if(tempA == 0x04)
			{
				state_frequency = DECREMENT;
			}
			else if(tempA == 0x00)
			{
				state_frequency = WAIT;
			}
			else
			{
				state_frequency = HOLD;
			}
			break;
		case DECREMENT:
			if(tempA == 0x01)
			{
				state_frequency = INCREMENT;
			}
			else if(tempA == 0x00)
			{
				state_frequency = WAIT;
			}
			else
			{
				state_frequency = HOLD;
			}
			break;
		case HOLD:
			if(tempA == 0x00)
			{
				state_frequency = WAIT;
			}
			else
			{
				state_frequency = HOLD;
			}
			break;
		default:
			state_frequency = WAIT;
			break;
	}
	
	switch(state_frequency)
	{
		case WAIT:
			break;
		case INCREMENT:
			if(note < 7)
			{
				note++;
			}
			break;
		case DECREMENT:
			if(note > 0)
			{
				note--;
			}
			break;
		case HOLD:
			break;
		default:
			set_PWM(261.63);
			break;
	}
}

void Tick_Toggle()
{
	unsigned char tempA = ~PINA & 0x02;
	
	switch(state_toggle)
	{
		case OFF:
			if(tempA == 0x02)
			{
				state_toggle = ON;
			}
			else
			{
				state_toggle = OFF;
			}
			break;
		case ON:
			if(tempA == 0x00)
			{
				state_toggle = OFF;
			}
			else
			{
				state_toggle = ON;
			}
			break;
		default:
			state_toggle = OFF;
			break;
	}
	
	switch(state_toggle)
	{
		case OFF:
			set_PWM(0);
			break;
		case ON:
			if(note == 0)
			{
				set_PWM(261.63);
			}
			else if(note == 1)
			{
				set_PWM(293.66);
			}
			else if(note == 2)
			{
				set_PWM(329.63);
			}
			else if(note == 3)
			{
				set_PWM(349.23);
			}
			else if(note == 4)
			{
				set_PWM(392.00);
			}
			else if(note == 5)
			{
				set_PWM(440.00);
			}
			else if(note == 6)
			{
				set_PWM(493.88);
			}
			else
			{
				set_PWM(523.25);
			}
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
	TimerSet(1);
	PWM_on();
	
	note = 0;
	state_frequency = WAIT;
	state_toggle = OFF;
	
    while (1) 
    {
		Tick_Frequency();
		Tick_Toggle();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

