/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 10 Exercise 4
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"
#include "io.c"
#include "bit.h"

typedef struct task
{
	unsigned char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[2];
const unsigned short numTasks = 2;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char note = 0;

unsigned char toggle = 0;
unsigned char input = 0;

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

enum Keypad_States {READY, INTERPRET, HOLD};
unsigned char Keypad(unsigned char state)
{
	input = GetKeypadKey();
	
	switch(state)
	{
		case READY:
			if(input != '\0')
			{
				state = INTERPRET;
			}
			else
			{
				state = READY;
			}
			break;
		case INTERPRET:
			if(input != '\0')
			{
				state = HOLD;
			}
			else
			{
				state = READY;
			}
			break;
		case HOLD:
			if(input != '\0')
			{
				state = HOLD;
			}
			else
			{
				state = READY;
			}
			break;
		default:
			state = READY;
			break;
	}
	
	switch(state)
	{
		case READY:
			toggle = 0;
			break;
		case INTERPRET:
			toggle = 1;
			break;
		case HOLD:
			toggle = 0;
			break;
	}
	
	return state;
}

unsigned char Display(unsigned char state)
{
	static unsigned char i = 1;
	
	if(toggle)
	{
		LCD_Cursor(i);
		LCD_WriteData(input);
		if(i < 16)
		{
			i++;
		}
		else
		{
			i = 1;
		}
	}
	
	return state;
}

int main()
{
	DDRC = 0xF0; PORTC = 0x0F;
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char i = 0;
	
	tasks[i].state = READY;
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Keypad;
	i++;
	tasks[i].state = 0;
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Display;
	
	LCD_init();
	LCD_DisplayString(1, "Congratulations!");
	TimerSet(1);
	TimerOn();
	
	while(1)
	{
		for (i = 0; i < numTasks; i++)
		{
			if(tasks[i].elapsedTime >= tasks[i].period )
			{
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime++;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}