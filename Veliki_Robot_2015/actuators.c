#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "can.h"
#include "actuators.h"

void knockDownTheClapperboards(signed char side, signed char state)
{
	unsigned char buffer[8];
	
	buffer[0] = 'A';
	
	if(side == RIGHT_SIDE)
	{
		buffer[1] = RIGHT_SIDE;
		
	} else
	{
		buffer[1] = LEFT_SIDE;
	}
	
	if(state == ACTIVATE)
	{
		buffer[2] = ACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}else if(state == DEACTIVATE)
	{
		buffer[2] = DEACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
}//END OF knockDownTheClapperboards

void colectThePopcorn(signed char side, signed char state)
{
	unsigned char buffer[8];
	
	buffer[0] = 'B';
	
	if(side == RIGHT_SIDE)
	{
		buffer[1] = RIGHT_SIDE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	} else
	{
		buffer[1] = LEFT_SIDE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
	
	if(state == ACTIVATE)
	{
		buffer[2] = ACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}else if(state == DEACTIVATE)
	{
		buffer[2] = DEACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}else if(state == CLOSE)
	{
		buffer[2] = CLOSE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
}//END OF colectThePopcorn

void leftDiafram(signed char state)
{
	unsigned char buffer[8];
	
	buffer[0] = 'C';
	
	if(state == ACTIVATE)
	{
		buffer[1] = ACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}else
	{
		buffer[1] = DEACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
	
}//END OF leftDiafram

void rightDiafram(signed char state)
{
	unsigned char buffer[8];
	
	buffer[0] = 'D';
	
	if(state == ACTIVATE)
	{
		buffer[1] = ACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}else
	{
		buffer[1] = DEACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
	
}//END OF rightDiafram

void liftStates(signed char state)
{
	unsigned char buffer[8];
	
	buffer[0] = 'E';
	
	if(state == ACTIVATE)
	{
		buffer[1] = ACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}else
	{
		buffer[1] = DEACTIVATE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
}//END OF liftStates

signed char liftMove(signed char vertically, signed char side)
{
	if(vertically == UP)
	{	
		if(side == RIGHT_SIDE)
		{
			rightDiafram(ACTIVATE);
		}else if(side == LEFT_SIDE)
		{
			leftDiafram(ACTIVATE);
		}else
		{
			rightDiafram(ACTIVATE);
			leftDiafram(ACTIVATE);
		}
		
		
		liftStates(ACTIVATE);
		
	}else 
	{
		liftStates(DEACTIVATE);
		
		if(side == RIGHT_SIDE)
		{
			rightDiafram(DEACTIVATE);
		}else if(side == LEFT_SIDE)
		{
			leftDiafram(DEACTIVATE);
		}else
		{
			rightDiafram(DEACTIVATE);
			leftDiafram(DEACTIVATE);
		}
		
	}
	
	return SUCCESS;
}//END OF liftMove

void servo_init(unsigned int f_pwm)
{
	DDRE |= (1 << PINE3) | (1 << PINE4) | (1 << PINE5);
	
	TCNT3 = 0;
	OCR3A = 0;
	OCR3B = 0;
	OCR3C = 0;
	
	TCCR3A = (1 << COM3A1) | (1 << COM3A0) | (1 << COM3B1) | (1 << COM3B0) | (1 << COM3C1) | (1 << COM3C0) | (1 << WGM31);
	TCCR3B = (1 << WGM32) | (1 << WGM33) | (1 << CS30); // PRESKALER = 1
	ICR3 = F_CPU / f_pwm - 0.5; // FREKVENCIJA PWMA JE ~19kHz
}//END OF servo_init


void servo_position1(unsigned char dutyCycle)
{
	OCR3A = ((double)ICR3 / 255) * dutyCycle + 0.5;
	
}//END OF servo_position

void servo_position2(unsigned char dutyCycle)
{
	OCR3B = ((double)ICR3 / 255) * dutyCycle + 0.5;
	
}//END OF servo_position

void servo_position3(unsigned char dutyCycle)
{
	OCR3C = ((double)ICR3 / 255) * dutyCycle + 0.5;
	
}//END OF servo_position