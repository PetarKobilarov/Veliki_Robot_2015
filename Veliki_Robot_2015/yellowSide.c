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
		if(getSystemTime() - startTime >= 200)
		{
			knockDownTheClapperboards(LEFT_SIDE);
			knockDownTheClapperboards(LEFT_SIDE);
		}
		clapperboardsClapped = 1;
	}
	
	return 0;
}//END OF clapperboardsKnockDownYellowSide

char colectTheFirstStand(unsigned long startTime)
{
	if(standColected == 0)
	{
		if(getSystemTime() - startTime >= 500)
		{
			liftMove(UP, RIGHT_SIDE);	
		}
		
		standColected = 1;
	}
	
	return 0;
}


char popcornColectionYellowSide(unsigned long startTime)
{
	colectThePopcorn(RIGHT_SIDE);
	
	return 0;
}//END OF popcornColectionYellowSide

/*************************************************************************************************************************************************************************************
																POZICIJE,BRZINE,SMEROVI I DETEKCIJE ZA ZUTU STRANU
*************************************************************************************************************************************************************************************/
const gotoFields yellowSideTacticOnePositions[TACTIC_ONE_POSITION_COUNT] =
{
	//gotoXY({x,y,angle},speed,direction,callback)
	{{600, 1067, 0}, NORMAL_SPEED, FORWARD, NULL},//izlazi iz startnog polja								//0
	{{850, 1355, 0}, NORMAL_SPEED, FORWARD, NULL},//ide na poziciju prvog valjka							//1
	{{1300, 1400, 0}, NORMAL_SPEED, FORWARD, colectTheFirstStand},//ide do drugog valjka					//2
	{{1100, 1710, 0}, NORMAL_SPEED, FORWARD, NULL},//ide do treceg valjka									//3
	{{450, 1710, 0}, NORMAL_SPEED, FORWARD, NULL},//ide do prve case koju kupi								//4
	{{200, 1800, 0}, NORMAL_SPEED, FORWARD, NULL},//ide po cetvrti i peti valjak							//5 
	{{900, 1800, 0}, LOW_SPEED, BACKWARD, clapperboardsKnockDownYellowSide},//rusi prve dve klapne			//6
	{{400, 350, 0}, NORMAL_SPEED, FORWARD, NULL}//ide po sesti i sedmi valjak								//7
	
	
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
					
				}else if(currentPosition == 2)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, RIGHT_SIDE);
					_delay_ms(500);
					liftMove(UP, RIGHT_SIDE);
					_delay_ms(500);
					
				}else if(currentPosition == 3)
				{
					
				}else if(currentPosition == 4)
				{
					rotate(-35, LOW_SPEED, NULL);
					moveOnDirection(100, LOW_SPEED, popcornColectionYellowSide);
					_delay_ms(250);
					moveOnDirection(-100, LOW_SPEED, NULL);
					_delay_ms(250);
					
				}else if(currentPosition == 5)
				{
					rotate(10, LOW_SPEED, NULL);
					moveOnDirection(20, LOW_SPEED, NULL);
				}else if(currentPosition == 6)
				{
				
				}else if(currentPosition == 7)
				{
					while(1);
				}
				
			}//end for
			break;
		}
	}//end while(1)
	
}