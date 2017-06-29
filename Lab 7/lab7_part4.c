/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 7 Exercise 4
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempC = 0;
unsigned short frequency = 0;

enum States_1 {INITIAL_1, ZERO_1, ONE_1, TWO_1}state_1;
enum States_2 {INITIAL_2, ON_2, OFF_2}state_2;
enum States_3 {INITIAL_3, WAIT_3, ON_3, OFF_3}state_3;
enum States_4 {START, INCREMENT, DECREMENT, INCREMENTHOLD, DECREMENTHOLD}state_4;

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

void State1()
{
	switch(state_1)
	{
		case INITIAL_1:
			state_1 = ZERO_1;
			break;
		case ZERO_1:
			state_1 = ONE_1;
			break;
		case ONE_1:
			state_1 = TWO_1;
			break;
		case TWO_1:
			state_1 = ZERO_1;
			break;
		default:
			state_1 = ZERO_1;
			break;
	}
}

void State2()
{
	switch(state_2)
	{
		case INITIAL_2:
			state_2 = ON_2;
			break;
		case ON_2:
			state_2 = OFF_2;
			break;
		case OFF_2:
			state_2 = ON_2;
			break;
		default:
			state_2 = ON_2;
			break;
	}
}

void State3()
{
	char tempA = ~PINA & 0x01;
	
	switch(state_3)
	{
		case INITIAL_3:
			state_3 = WAIT_3;
			break;
		case WAIT_3:
			if(tempA == 1)
			{
				state_3 = ON_3;
			}
			else
			{
				state_3 = WAIT_3;
			}
			break;
		case ON_3:
			if(tempA == 1)
			{
				state_3 = OFF_3;
			}
			else
			{
				state_3 = WAIT_3;
			}
			break;
		case OFF_3:
			if(tempA == 1)
			{
				state_3 = ON_3;
			}
			else
			{
				state_3 = WAIT_3;
			}
			break;
		default:
			state_3 = WAIT_3;
			break;
	}
	
	tempA = ~PINA & 0x06;
	
	switch(state_4)
	{
		case START:
			if(tempA == 2)
			{
				state_4 = INCREMENT;
			}
			else if(tempA == 4)
			{
				state_4 = DECREMENT;
			}
			else
			{
				state_4 = START;
			}
			break;
		case INCREMENT:
			if(tempA == 2)
			{
				state_4 = INCREMENTHOLD;
			}
			else
			{
				state_4 = START;
			}
			break;
		case DECREMENT:
			if(tempA == 4)
			{
				state_4 = DECREMENTHOLD;
			}
			else
			{
				state_4 = START;
			}
			break;
		case INCREMENTHOLD:
			if(tempA == 0)
			{
				state_4 = START;
			}
			else
			{
				state_4 = INCREMENTHOLD;
			}
			break;
		case DECREMENTHOLD:
			if(tempA == 0)
			{
				state_4 = START;
			}
			else
			{
				state_4 = DECREMENTHOLD;
			}
			break;
		default:
			state_4 = START;
			break;
	}
}

void Outputs()
{
	unsigned char threeLEDS = 0;
	unsigned char blinkingLED = 0;
	
	switch(state_1)
	{
		case INITIAL_1:
			threeLEDS = 0x00;
			break;
		case ZERO_1:
			threeLEDS = 0x01;
			break;
		case ONE_1:
			threeLEDS = 0x02;
			break;
		case TWO_1:
			threeLEDS = 0x04;
			break;
		default:
			threeLEDS = 0x00;
			break;
	}
	
	switch(state_2)
	{
		case INITIAL_2:
			blinkingLED = 0x00;
			break;
		case ON_2:
			blinkingLED = 0x08;
			break;
		case OFF_2:
			blinkingLED = 0x00;
			break;
		default:
			blinkingLED = 0x00;
			break;
	}
	
	switch(state_4)
	{
		case START:
			break;
		case INCREMENT:
			if(frequency < 50)
			{
				frequency++;
			}
			break;
		case DECREMENT:
			if(frequency > 0)
			{
				frequency--;
			}
			break;
		case INCREMENTHOLD:
			break;
		case DECREMENTHOLD:
			break;
		default:
			frequency = 0;
			break;
	}
	
	tempC = threeLEDS | blinkingLED;
	PORTC = tempC;
}

void OutputsSound()
{
	unsigned char soundOutput = 0;
	
	switch(state_3)
	{
		case INITIAL_3:
			soundOutput = 0x00;
			break;
		case WAIT_3:
			soundOutput = 0x00;
			break;
		case ON_3:
			soundOutput = 0x10;
			break;
		case OFF_3:
			soundOutput = 0x00;
			break;
		default:
			soundOutput = 0x00;
			break;
	}
	
	tempC = tempC | soundOutput;
	PORTC = tempC;
}

int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(1);
	TimerOn();
	
	state_1 = INITIAL_1;
	state_2 = INITIAL_2;
	state_4 = START;
	unsigned short count1 = 0;
	unsigned short count2 = 0;
	unsigned short count3 = 0;
	
	while(1)
	{
		if(count1 >= 300)
		{
			State1();
			count1 = 0;
		}
		else
		{
			count1++;
		}
		if(count2 >= 1000)
		{
			State2();
			count2 = 0;
		}
		else
		{
			count2++;
		}
		State3();
		
		Outputs();
		if (count3 >= frequency)
		{
			OutputsSound();
			count3 = 0;
		}
		else
		{
			count3++;
		}
		
		while(!TimerFlag);
		TimerFlag = 0;
	
	}
	
	return 0;
}
