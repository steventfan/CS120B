/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 4 Exercise 3
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

enum States {OFFHOLD, OFF, FORWARDREADY, FORWARD, FORWARDHOLD, OFF1HOLD, OFF1, BACKWARDREADY, BACKWARD, BACKWARDHOLD} state;

char tempC = 0x00;

void Sequencing();
void Outputs();

int main(void)
{
	DDRA = 0x00;
	DDRC = 0xFF;
	PORTA = 0xFF;
	PORTC = 0x00;
	state = OFF;
	
	while(1)
	{
		Sequencing();
		Outputs();
	}
	
	return 0;
}

void Sequencing()
{
	char tempA = ~PINA & 1;
	
	switch(state)
	{
		case OFFHOLD:
			if(tempA)
			{
				state = OFFHOLD;
			}
			else
			{
				state = OFF;
			}
			break;
		case OFF:
			if(tempA)
			{
				state = FORWARD;
			}
			else
			{
				state = OFF;
			}
			break;
		case FORWARDREADY:
			if(tempA)
			{
				if(PORTC < 0xFF)
				{
					state = FORWARD;
				}
				else
				{
					state = OFF1HOLD;
				}
			}
			else
			{
				state = FORWARDREADY;
			}
			break;
		case FORWARD:
			if(tempA)
			{
				state = FORWARDHOLD;
			}
			else
			{
				state = FORWARDREADY;
			}
			break;
		case FORWARDHOLD:
			if(tempA)
			{
				state = FORWARDHOLD;
			}
			else
			{
				state = FORWARDREADY;
			}
			break;
		case OFF1HOLD:
			if(tempA)
			{
				state = OFF1HOLD;
			}
			else
			{
				state = OFF1;
			}
			break;
		case OFF1:
			if(tempA)
			{
				state = BACKWARD;
			}
			else
			{
				state = OFF1;
			}
			break;
		case BACKWARDREADY:
			if(tempA)
			{
				if(PORTC < 0xFF)
				{
					state = BACKWARD;
				}
				else
				{
					state = OFFHOLD;
				}
			}
			else
			{
				state = BACKWARDREADY;
			}
			break;
		case BACKWARD:
			if(tempA)
			{
				state = BACKWARDHOLD;
			}
			else
			{
				state = BACKWARDREADY;
			}
			break;
		case BACKWARDHOLD:
			if(tempA)
			{
				state = BACKWARDHOLD;
			}
			else
			{
				state = BACKWARDREADY;
			}
			break;
		default:
			state = OFF;
			break;
	}
	
}

void Outputs()
{
	switch(state)
	{
		case OFFHOLD:
			tempC = 0x00;
			break;
		case OFF:
			tempC = 0x00;
			break;
		case FORWARDREADY:
			break;
		case FORWARD:
			tempC = (tempC << 1) | 0x01;
			break;
		case FORWARDHOLD:
			break;
		case OFF1HOLD:
			tempC = 0x00;
			break;
		case OFF1:
			tempC = 0x00;
			break;
		case BACKWARDREADY:
			break;
		case BACKWARD:
			tempC = (tempC >> 1) | 0x80;
			break;
		case BACKWARDHOLD:
			break;
		default:
			tempC = 0x00;
			break;
	}
	
	PORTC = tempC;
}