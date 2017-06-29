/* Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 3 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

enum States {START, ONEON, ONEOFF, ZEROON} state;

int main(void)
{	
	state = START;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x01;
	
    while(1)
    {
		tmpA = PINA;
		
        switch(state)
        {
	   		case START:
	       	tmpB = 1;
	        if( (tmpA & 1) == 1)
	        {
		        state = ONEON;
	        }
	        break;
	        case ONEON:
	        tmpB = 2;
	        if( (tmpA & 1) == 0)
	        {
		        state = ONEOFF;
	        }
	        break;
	        case ONEOFF:
	        tmpB = 2;
	        if( (tmpA & 1) == 1)
	        {
		        state = ZEROON;
	        }
	        break;
	        case ZEROON:
	        tmpB = 1;
	        if( (tmpA & 1) == 0)
	        {
		        state = START;
	        }
	        break;
        }
        	
        PORTB = tmpB;
    }
	
	return 0;
}