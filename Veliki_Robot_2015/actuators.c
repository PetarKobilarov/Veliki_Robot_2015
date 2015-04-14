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