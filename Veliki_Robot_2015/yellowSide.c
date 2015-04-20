#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "usart.h"
#include "actuators.h"
#include "fat.h"

char clapperboardsKnockDownYellowSide(unsigned long startTime)
{
	if(clapperboardsClapped == 0)
	{
		
		if(getSystemTime() - startTime < 200 || (getSystemTime() - startTime > 700 && getSystemTime() - startTime<1500))
		{
			knockDownTheClapperboards(LEFT_SIDE, ACTIVATE);
		}
		if((getSystemTime() - startTime >= 200 && getSystemTime() - startTime <700)|| (getSystemTime() - startTime>1500))
		{
			knockDownTheClapperboards(LEFT_SIDE, DEACTIVATE);
		}
		
		clapperboardsClapped = 1;
	}
	
	return 0;
}//END OF clapperboardsKnockDownYellowSide

char colectTheFirstStand(unsigned long startTime)
{
	if(standColected == 0)
	{
		if(getSystemTime() - startTime >= 300)
		{
			_delay_ms(500);
			liftMove(DOWN, LEFT_SIDE);
			_delay_ms(500);
			liftMove(UP, LEFT_SIDE);	
		}
		
		standColected = 1;
	}
	
	return 0;
}


char popcornColectionYellowSide(unsigned long startTime)
{
	if(popcornColected == 0)
	{
		//colectThePopcorn(RIGHT_SIDE);
		colectThePopcorn(RIGHT_SIDE, ACTIVATE);
		
		popcornColected = 1;
	}
		
	return 0;
}//END OF popcornColectionYellowSide

/*************************************************************************************************************************************************************************************
																POZICIJE,BRZINE,SMEROVI I DETEKCIJE ZA ZUTU STRANU
*************************************************************************************************************************************************************************************/
const gotoFields yellowSideTacticOnePositions[TACTIC_ONE_POSITION_COUNT] =
{
	//gotoXY({x,y,angle},speed,direction,callback)
	{{500, 1067, 0}, NORMAL_SPEED, FORWARD, NULL},//izlazi iz startnog polja								//0
	{{730, 1450, 0}, NORMAL_SPEED, FORWARD, NULL},//ide na poziciju prvog valjka							//1
	{{1200, 1500, 0}, NORMAL_SPEED, FORWARD, NULL},//ide do drugog valjka									//2
	{{1030, 1710, 0}, NORMAL_SPEED, FORWARD, NULL},//ide do treceg valjka									//3
	{{420, 1710, 0}, NORMAL_SPEED, FORWARD, NULL},//ide do prve case koju kupi								//4
	{{420, 1850, 0}, NORMAL_SPEED, FORWARD, NULL},	
	{{120, 1850, 0}, NORMAL_SPEED, FORWARD, NULL},//ide po cetvrti i peti valjak							//5 
	{{900, 1800, 0}, LOW_SPEED, BACKWARD, NULL},//rusi prve dve nase klapne		//6
	{{400, 350, 0}, NORMAL_SPEED, FORWARD, NULL}//ostavlja kulu na crveno polje								//7
	
	
};




/*************************************************************************************************************************************************************************************
																				ZUTA STRANA
*************************************************************************************************************************************************************************************/
void yellowSide(void)
{
	position startingPosition;
	unsigned char currentPosition = 0, nextPosition = 0, odometryStatus;
	unsigned char activeState = TACTIC_ONE;
	
	startingPosition.x = 94;
	startingPosition.y = 1067;
	startingPosition.angle = 0;
	setPosition(startingPosition);
	
	while (1)
	{
		switch(activeState)
		{
			case TACTIC_ONE:
			for (currentPosition = nextPosition; currentPosition < TACTIC_ONE_POSITION_COUNT; currentPosition++)
			{
				//logger("Active TACTIC_ONE: going to position %d ", currentPosition);
				// mozda ubaciti if-else sa akcijama tipa regularno- kretanje, i alternativno- sta god
				
				odometryStatus = gotoXY(yellowSideTacticOnePositions[currentPosition].point, yellowSideTacticOnePositions[currentPosition].speed,
				yellowSideTacticOnePositions[currentPosition].direction, yellowSideTacticOnePositions[currentPosition].detectionCallback);
				
				if(odometryStatus == ODOMETRY_FAIL)
				{

				}
				else if(odometryStatus == ODOMETRY_STUCK)
				{
					
				}
				
				if(currentPosition == 0)
				{
					//rotate(55, LOW_SPEED, NULL);
				}
				else if(currentPosition == 1)
				{
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
				}else if(currentPosition == 2)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
					_delay_ms(500);
					
				}else if(currentPosition == 3)
				{
					
				}else if(currentPosition == 4)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
					_delay_ms(500);
					
					
					rotate(-90, LOW_SPEED, popcornColectionYellowSide);
					moveOnDirection(80, LOW_SPEED, NULL);
					colectThePopcorn(RIGHT_SIDE, DEACTIVATE);
					_delay_ms(250);
					rotate(90, LOW_SPEED, NULL);
					moveOnDirection(-80, LOW_SPEED, NULL);
				}else if(currentPosition == 5)
				{
					
				}else if(currentPosition == 6)
				{
					//rotate(10, LOW_SPEED, NULL);
					//moveOnDirection(10, LOW_SPEED, NULL);
					_delay_ms(50);
					liftMove(DOWN, BOTH);
					_delay_ms(1000);
					leftDiafram(ACTIVATE);
					rightDiafram(ACTIVATE);
					while(1);
					moveOnDirection(-30, LOW_SPEED, NULL);
					rotate(30, LOW_SPEED, NULL);
					knockDownTheClapperboards(LEFT_SIDE, ACTIVATE);
					rotate(-30, LOW_SPEED, NULL);
				}else if(currentPosition == 7)
				{
					knockDownTheClapperboards(LEFT_SIDE, DEACTIVATE);
				}else if(currentPosition == 8)
				{
					while(1);
				}
				
			}//end for
			break;
		}
	}//end while(1)
	
}