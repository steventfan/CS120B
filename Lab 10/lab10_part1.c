/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 10 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"
#include "bit.h"

typedef struct task
{
	unsigned char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	void (*TickFct)();
} task;

task tasks[1];
const unsigned short numTasks = 1;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char note = 0;

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

unsigned char GetKeypadKey()
{
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }

	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }

	return('\0');
}

void Keypad()
{
	unsigned char value = GetKeypadKey();
	
	switch(value)
	{
		case '\0': PORTB = 0x1F;
		break;
		case '1': PORTB = 0x01;
		break;
		case '2': PORTB = 0x02;
		break;
		case '3': PORTB = 0x03;
		break;
		case '4': PORTB = 0x04;
		break;
		case '5': PORTB = 0x05;
		break;
		case '6': PORTB = 0x06;
		break;
		case '7': PORTB = 0x07;
		break;
		case '8': PORTB = 0x08;
		break;
		case '9': PORTB = 0x09;
		break;
		case 'A': PORTB = 0x0A;
		break;
		case 'B': PORTB = 0x0B;
		break;
		case 'C': PORTB = 0x0C;
		break;
		case 'D': PORTB = 0x0D;
		break;
		case '*': PORTB = 0x0E;
		break;
		case '0': PORTB = 0x00;
		break;
		case '#': PORTB = 0x0F;
		break;
		default: PORTB = 0x1B;
		break;
	}
}

int main()
{
	DDRC = 0xF0; PORTC = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned char i = 0;
	
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Keypad;
	
	TimerSet(1);
	TimerOn();
	
	while(1)
	{
		for (i = 0; i < numTasks; i++)
		{
			if(tasks[i].elapsedTime >= tasks[i].period )
			{
				tasks[i].TickFct();
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime++;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}