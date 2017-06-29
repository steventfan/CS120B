/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 10 Exercise 2
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

void Display()
{
	unsigned char data[44] = {' ', 'L', 'e', 'g', 'e' ,'n', 'd', '.', '.', '.', ' ', 'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	static unsigned char display[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	static unsigned char i = 0;
	
	for(unsigned char j = 1; j <= 16; j++)
	{
		LCD_Cursor(j);
		if(j < 16)
		{
			display[j - 1] = display[j];
		}
		else
		{
			display[j - 1] = data[i];
		}
		LCD_WriteData(display[j - 1]);
	}
	if(i >= 43)
	{
		i = 0;
	}
	else
	{
		i++;
	}
}

int main()
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char i = 0;
	
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Display;
	
	LCD_init();
	TimerSet(300);
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