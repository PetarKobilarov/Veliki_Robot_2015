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
		
		if(checkLiftSensor(UP) == FAIL)
		{
			return FAIL;
		}
		
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
		
		if(checkLiftSensor(DOWN) == FAIL)
		{
			return FAIL;
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
	TCCR3B = (1 << WGM32) | (1 << WGM33) | (1 << CS31); // PRESKALER = 1
	ICR3 = ((double)F_CPU) / (8 * f_pwm) - 0.5; // FREKVENCIJA PWMA JE ~19kHz
}//END OF servo_init


void servo_position1(unsigned char dutyCycle)
{
	/*if((dutyCycle < 65) || (dutyCycle > 175))
	{
		dutyCycle = 150;
	}*/
	OCR3A = ((double)ICR3 / 255) * dutyCycle + 0.5;
	
}//END OF servo_position

void servo_position2(unsigned char dutyCycle)
{
	/*if((dutyCycle < 110) || (dutyCycle >220))
	{
		dutyCycle = 160;
	}*/
		OCR3B = ((double)ICR3 / 255) * dutyCycle + 0.5;
	
}//END OF servo_position

//levi servo skroz otvoren 225
//levi servo skroz zatvoren 240
//levi servo je servo_position2
//levi servo je pinE4

//desni servo skeoz otvoren 235
//desni servo skroz zatvoren 215
//desni servo je servo_position1
//desni servo je pinE3
void theDoors(signed char side, signed char state)
{
	if(side == RIGHT_SIDE)
	{
		if(state == ACTIVATE)
		{
			servo_position1(227);
		}else
		{
			servo_position1(240);
		}
		
	}else
	{
		if(state == ACTIVATE)
		{
			servo_position2(235);
		}else
		{
			servo_position2(215);
		}
	}
}//END OF theDoors