#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "can.h"
#include "actuators.h"

void knockDownTheClapperboards(unsigned char side)
{
	unsigned char buffer[8];
	
	buffer[0] = 'A';
	
	if(side == RIGHT_SIDE)
	{
		buffer[1] = RIGHT_SIDE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
			PORTG = 0xff;
	} else
	{
		buffer[1] = LEFT_SIDE;
		
		while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
			_delay_ms(50);
	}
}//END OF knockDownTheClapperboards

void colectThePopcorn(unsigned char side)
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
}//END OF colectThePopcorn

void leftDiafram(unsigned char state)
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

void rightDiafram(unsigned char state)
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

void liftMove(unsigned char state)
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
}//END OF liftMove