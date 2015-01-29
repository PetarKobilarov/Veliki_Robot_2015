#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"

#define yellowSideRegularPositionCount		1
#define yellowSideAlternativePositionCount	1

/*************************************************************************************************************************************************************************************
														Pozicije na zutoj strani u slucaju kada nije detektovan protivnik.
*************************************************************************************************************************************************************************************/
const position yellowSideRegularPositions[yellowSideRegularPositionCount] =
{
	{0, 0, 0}
};

const unsigned char yellowSideRegularSpeed[yellowSideRegularPositionCount] =
{
	LOW_SPEED	
};

const signed char yellowSideRegularDirection[yellowSideRegularPositionCount] =
{
	FORWARD	
};

/*************************************************************************************************************************************************************************************
														Pozicije na zutoj strani u slucaju da je detektovan protivnik.
*************************************************************************************************************************************************************************************/
const position yellowSideAlternativePositions[yellowSideAlternativePositionCount] =
{
	{0, 0, 0}
};

const unsigned char yellowSideAlternativeSpeed[yellowSideAlternativePositionCount] =
{
	LOW_SPEED
};

const signed char yellowSideAlternativeDirection[yellowSideAlternativePositionCount] =
{
	FORWARD
};

/*************************************************************************************************************************************************************************************
																				ZUTA STRANA
*************************************************************************************************************************************************************************************/
void yellowSide(void)
{
	position startingPosition;
	unsigned char currentPosition = 0, nextPosition = 0;
	Movementstates	state = REGULAR;
	
	startingPosition.x = 0;
	startingPosition.y = 0;
	startingPosition.angle = 0;
	setPosition(startingPosition);
	
	while (1)
	{
		for (currentPosition = nextPosition; currentPosition < yellowSideRegularPositionCount; currentPosition++)
		{
			switch(state)
			{
				default:
					switch(currentPosition)
					{
						case 0:	
							gotoXY(yellowSideRegularPositions[0], yellowSideRegularSpeed[0], yellowSideRegularDirection[0], NULL);
								break;
					
					}//end switch position
						break;
						
					case ALTERNATIVE:
						break;
			}//end switch states
		}//end for
	}//end while(1)
}