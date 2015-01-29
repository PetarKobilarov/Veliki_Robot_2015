#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"

#define greedSideRegularPositionCount		1
#define greenSideAlternativePositionCount	1

/*************************************************************************************************************************************************************************************
														Pozicije na zelenoj strani u slucaju kada nije detektovan protivnik.
*************************************************************************************************************************************************************************************/
const position greenSideRegularPositions[greedSideRegularPositionCount] =
{
	{0, 0, 0}
};

const unsigned char greenSideRegularSpeed[greedSideRegularPositionCount] =
{
	LOW_SPEED	
};

const signed char greenSideRegularDirection[greedSideRegularPositionCount] =
{
	FORWARD	
};

/*************************************************************************************************************************************************************************************
														Pozicije na zelenoj strani u slucaju da je detektovan protivnik.
*************************************************************************************************************************************************************************************/
const position greenSideAlternativePositions[greenSideAlternativePositionCount] =
{
	{0, 0, 0}
};

const unsigned char greenSideAlternativeSpeed[greenSideAlternativePositionCount] =
{
	LOW_SPEED
};

const signed char greenSideAlternativeDirection[greenSideAlternativePositionCount] =
{
	FORWARD
};

/*************************************************************************************************************************************************************************************
																				ZELENA STRANA
*************************************************************************************************************************************************************************************/
void greenSide(void)
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
		for (currentPosition = nextPosition; currentPosition < greedSideRegularPositionCount; currentPosition++)
		{
			switch(state)
			{
				default:
					switch(currentPosition)
					{
						case 0:	
							gotoXY(greenSideRegularPositions[0], greenSideRegularSpeed[0], greenSideRegularDirection[0], NULL);
								break;
					
					}//end switch position
						break;
						
					case ALTERNATIVE:
						break;
			}//end switch states
		}//end for
	}//end while(1)
}