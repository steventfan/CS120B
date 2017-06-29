/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 6 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempB = 1;
unsigned char tempVal = 5;

enum States {ONE, TWO, THREE, FOUR, STOPINCREMENT, STOPDECREMENT, STOPHOLD, STOP, WAIT, ONEHOLD, TWOHOLD, THREEHOLD, FOURHOLD}state;

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
	unsigned char tempA = ~PINA & 0x01;
	
	switch(state)
	{
		case ONE:
			if(tempA)
			{
				state = STOPDECREMENT;
			}
			else
			{
				state = TWO;
			}
			break;
		case TWO:
			if(tempA)
			{
				state = STOPINCREMENT;
			}
			else
			{
				state = THREE;
			}
			break;
		case THREE:
			if(tempA)
			{
				state = STOPDECREMENT;
			}
			else
			{
				state = FOUR;
			}
			break;
		case FOUR:
			if(tempA)
			{
				state = STOPINCREMENT;
			}
			else
			{
				state = ONE;
			}
			break;
		case STOPINCREMENT:
			if(tempA)
			{
				state = STOPHOLD;
			}
			else
			{
				state = STOP;
			}
			break;
		case STOPDECREMENT:
			if(tempA)
			{
				state = STOPHOLD;
			}
			else
			{
				state = STOP;
			}
			break;
		case STOPHOLD:
			if(!tempA)
			{
				state = STOP;
			}
			else
			{
				state = STOPHOLD;
			}
			break;
		case STOP:
			if(tempA)
			{
				state = ONEHOLD;
			}
			else
			{
				state = STOP;
			}
			break;
		case ONEHOLD:
			if(tempA)
			{
				state = TWOHOLD;
			}
			else
			{
				state = TWO;
			}
			break;
		case TWOHOLD:
			if(tempA)
			{
				state = THREEHOLD;
			}
			else
			{
				state = THREE;
			}
			break;
		case THREEHOLD:
			if(tempA)
			{
				state = FOURHOLD;
			}
			else
			{
				state = FOUR;
			}
			break;
		case FOURHOLD:
			if(tempA)
			{
				state = ONEHOLD;
			}
			else
			{
				state = ONE;
			}
			break;
		default:
			state = ONE;
			break;
	}
}

void Outputs()
{
	switch(state)
	{
		case ONE:
			tempB = 1;
			if(tempVal == 9)
			{
				tempVal = 5;
				LCD_ClearScreen();
			}
			break;
		case TWO:
			tempB = 2;
			break;
		case THREE:
			tempB = 4;
			break;
		case FOUR:
			tempB = 2;
			break;
		case STOPINCREMENT:
			if(tempVal < 9)
			{
				tempVal++;
			}
			if(tempVal == 9)
			{
				LCD_DisplayString(1, "VICTORY MESSAGE");
			}
			break;
		case STOPDECREMENT:
			if(tempVal > 0)
			{
				tempVal--;
			}
			break;
		case STOPHOLD:
			break;
		case STOP:
			break;
		case WAIT:
			break;
		case ONEHOLD:
			tempB = 1;
			if(tempVal == 9)
			{
				tempVal = 5;
				LCD_ClearScreen();
			}
			break;
		case TWOHOLD:
			tempB = 2;
			break;
		case THREEHOLD:
			tempB = 4;
			break;
		case FOURHOLD:
			tempB = 2;
			break;
		default:
			tempB = 1;
			tempVal = 5;
			break;
	}
	
	PORTB = tempB;
	if(tempVal != 9)
	{
		LCD_Cursor(1);
		LCD_WriteData(tempVal + '0');
	}
}

int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();
	LCD_Cursor(1);
	LCD_WriteData('5');
	TimerSet(300);
	TimerOn();
	
	state = FOUR;
	
	while(1)
	{
		Sequencing();
		Outputs();
		
		while (!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}
