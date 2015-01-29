#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "usart.h"

#define yellowSideRegularPositionCount		1
#define yellowSideAlternativePositionCount	1

typedef void (*yellowSideCallbackFunction)(void);
yellowSideCallbackFunction yellowSideCallbackFunctionsArray[yellowSideRegularPositionCount];

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
	int callbackNum = 0;
	
	startingPosition.x = 0;
	startingPosition.y = 0;
	startingPosition.angle = 0;
	setPosition(startingPosition);
	
	for(callbackNum; callbackNum < yellowSideRegularPositionCount; callbackNum++)
	{
		yellowSideCallbackFunctionsArray[callbackNum] = calloc(yellowSideRegularPositionCount, sizeof(callbackFunction()));
	}
	
	yellowSideCallbackFunctionsArray[0] = NULL;
	
	while (1)
	{
		for (currentPosition = nextPosition; currentPosition < yellowSideRegularPositionCount; currentPosition++)
		{
			switch(state)
			{
				default:	
					gotoXY(yellowSideRegularPositions[currentPosition], yellowSideRegularSpeed[currentPosition], yellowSideRegularDirection[currentPosition], yellowSideCallbackFunctionsArray[currentPosition]);
						break;
						
				case ALTERNATIVE:
						break;
			}//end switch states
		}//end for
	}//end while(1)
}