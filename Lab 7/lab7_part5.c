/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 7 Exercise 5
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempVal = 0;
unsigned short i = 0;
unsigned short j = 0;
unsigned short k = 0;

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
				j = 0;
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				j = 0;
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
				k = 0;
				state = INCREMENTHOLD;
			}
			else if(tempA == 0x02)
			{
				j = 0;
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
				if(i < 1000 && j < 3000)
				{
					i++;
					j++;
				}
				else if(i >= 1000 && j < 3000)
				{
					j++;
					state = INCREMENT;
				}
				else
				{
					if(k < 400)
					{
						k++;
					}
					else
					{
						state = INCREMENT;
					}
				}
			}
			else if(tempA == 0x02)
			{
				j = 0;
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
				j = 0;
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				i = 0;
				k = 0;
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
				j = 0;
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				if(i < 1000 && j < 3000)
				{
					i++;
					j++;
				}
				else if(i >= 1000 && j < 3000)
				{
					j++;
					state = DECREMENT;
				}
				else
				{
					if(k < 400)
					{
						k++;
					}
					else
					{
						state = DECREMENT;
					}
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
				j = 0;
				state = INCREMENT;
			}
			else if(tempA == 0x02)
			{
				j = 0;
				state = DECREMENT;
			}
			else if(tempA == 0x03)
			{
				state = RESET;
			}
			else
			{
				j = 0;
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
	
	PORTC = tempVal;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(1);
	TimerOn();
	
	state = START;
	
    while (1) 
    {
		Sequencing();
		Outputs();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

