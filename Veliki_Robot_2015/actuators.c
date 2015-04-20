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
		
		if(checkLiftSensor(UP) == 0)
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
			rightDiafram(ACTIVATE);
			leftDiafram(ACTIVATE);
		}
		
		if(checkLiftSensor(DOWN) == 0)
		{
			return FAIL;
		}
		
	}
	
	return SUCCESS;
}//END OF liftMove