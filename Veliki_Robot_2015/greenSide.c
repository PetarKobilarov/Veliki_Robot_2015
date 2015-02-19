#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "usart.h"
#include "common.h"

//#define greenSideAlternativePositionCount	1
/*
typedef void (*greenSideCallbackFunction)(void);
greenSideCallbackFunction greenSideCallbackFunctionsArray[greenSideRegularPositionCount];


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
*/

char fuckItOne(void)
{
	if(GPIO_PinRead(backwardLeftSensor) == 1 || GPIO_PinRead(forwardRightSensor) == 1 || GPIO_PinRead(forwardLeftSensor) == 1)
	{
		PORTG = 0x01;	
	}
	
	return 0;
}

char fuckItTwo(void)
{
	if(GPIO_PinRead(backwardLeftSensor) == 1 || GPIO_PinRead(forwardRightSensor) == 1 || GPIO_PinRead(forwardLeftSensor) == 1)
	{
		PORTG = 0x03;
		stop(HARD_STOP);
		
		while(GPIO_PinRead(backwardLeftSensor) == 1 || GPIO_PinRead(forwardRightSensor) == 1 || GPIO_PinRead(forwardLeftSensor) == 1);
		_delay_ms(100);
		
		return 2;
	}
	
	return 0;
}

const gotoCrap tacticOnePositions[TACTIC_ONE_POSITION_COUNT] = 
{
	{{500, 500, 0}, LOW_SPEED, FORWARD, fuckItOne},
	{{0, 500, 0}, LOW_SPEED, BACKWARD, fuckItTwo}
};

void greenSide(void)
{
	
	position startingPosition;
	unsigned char currentPosition = 0, nextPosition = 0, odometryStatus;
	unsigned char activeState = TACTIC_ONE;
	
	startingPosition.x = 0;
	startingPosition.y = 500;
	startingPosition.angle = 0;
	setPosition(startingPosition);
	
	while (1)
	{
		_delay_ms(50);
		PORTG = 0;
		switch(activeState)
		{
			case TACTIC_ONE:
				for (currentPosition = nextPosition; currentPosition < TACTIC_ONE_POSITION_COUNT; currentPosition++)
				{
					// mozda ubaciti if-else sa akcijama tipa regularno- kretanje, i alternativno- sta god
					odometryStatus = gotoXY(tacticOnePositions[currentPosition].point, tacticOnePositions[currentPosition].speed, 
						   tacticOnePositions[currentPosition].direction, tacticOnePositions[currentPosition].detectionCallback);
						   
					if(odometryStatus == ODOMETRY_FAIL) // fail
					{
						PORTG = 0xFF;
						_delay_ms(5000);
					}
					else if(odometryStatus == ODOMETRY_STUCK)
					{
						
					}
					
					if(currentPosition == 0)
					{
						
					}
					else if(currentPosition == 1)
					{
						
					}
				}//end for
				break;
		}
	}//end while(1)
}