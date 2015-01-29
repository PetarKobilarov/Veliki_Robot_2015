#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "usart.h"

#define greenSideRegularPositionCount		1
#define greenSideAlternativePositionCount	1

typedef void (*greenSideCallbackFunction)(void);
greenSideCallbackFunction greenSideCallbackFunctionsArray[greenSideRegularPositionCount];

/*************************************************************************************************************************************************************************************
														Pozicije na zelenoj strani u slucaju kada nije detektovan protivnik.
*************************************************************************************************************************************************************************************/
const position greenSideRegularPositions[greenSideRegularPositionCount] =
{
	{0, 0, 0}
};

const unsigned char greenSideRegularSpeed[greenSideRegularPositionCount] =
{
	LOW_SPEED	
};

const signed char greenSideRegularDirection[greenSideRegularPositionCount] =
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
	int callbackNum = 0;
	
	startingPosition.x = 0;
	startingPosition.y = 0;
	startingPosition.angle = 0;
	setPosition(startingPosition);
	
	for(callbackNum; callbackNum < greenSideRegularPositionCount; callbackNum++)
	{
		greenSideCallbackFunctionsArray[callbackNum] = calloc(greenSideRegularPositionCount, sizeof(callbackFunction()));
	}
	
	greenSideCallbackFunctionsArray[0] = NULL;
	
	while (1)
	{
		for (currentPosition = nextPosition; currentPosition < greenSideRegularPositionCount; currentPosition++)
		{
			switch(state)
			{
				default:
					gotoXY(greenSideRegularPositions[currentPosition], greenSideRegularSpeed[currentPosition], greenSideRegularDirection[currentPosition], greenSideCallbackFunctionsArray[currentPosition]);
						break;
					
				case ALTERNATIVE:
						break;
			}//end switch states
		}//end for
	}//end while(1)
}