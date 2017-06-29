/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 6 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempVal = 0;
unsigned short i = 0;

enum States {START, INCREMENT, INCREMENTHOLD, DECREMENT, DECREMENTHOLD, RESET}state;

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
	unsigned char tempA = ~PINA & 0x03;
	
	switch(state)
	{
		case START:
			if(tempA == 0x01)
			{
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				state = DECREMENT;
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = START;
			}
			break;
		case INCREMENT:
			if(tempA == 0x01)
			{
				i = 0;
				state = INCREMENTHOLD;
			}
			else if(tempA == 0x02)
			{
				state = DECREMENT;
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = START;
			}
			break;
		case INCREMENTHOLD:
			if(tempA == 0x01)
			{
				if(i < 1000)
				{
					i++;
				}
				else
				{
					state = INCREMENT;
				}
			}
			else if(tempA == 0x02)
			{
				state = DECREMENT;
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = START;
			}
			break;
		case DECREMENT:
			if(tempA == 0x01)
			{
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				i = 0;
				state = DECREMENTHOLD;
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = START;
			}
			break;
		case DECREMENTHOLD:
			if(tempA == 0x01)
			{
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				if(i < 1000)
				{
					i++;
				}
				else
				{
					state = DECREMENT;
				}
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = START;
			}
			break;
		case RESET:
			if(tempA == 0x01)
			{
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				state = DECREMENT;
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				state = START;
			}
			break;
		default:
			state = START;
			break;
	}
}

void Outputs()
{
	switch(state)
	{
		case START:
			break;
		case INCREMENT:
			if(tempVal < 9)
			{
				tempVal++;
			}
			break;
		case INCREMENTHOLD:
			break;
		case DECREMENT:
			if(tempVal > 0)
			{
				tempVal--;
			}
			break;
		case DECREMENTHOLD:
			break;
		case RESET:
			tempVal = 0;
			break;
		default:
			tempVal = 0;
			break;
	}
	
	LCD_Cursor(1);
	LCD_WriteData(tempVal + '0');
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();
	LCD_Cursor(1);
	LCD_WriteData('0');
	TimerSet(1000);
	TimerOn();
	
	state = START;
	
    while (1) 
    {
		Sequencing();
		Outputs();
		while(!TimerFlag)
		{
			TimerFlag = 0;
		}
    }
}

