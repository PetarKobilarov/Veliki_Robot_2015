#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "usart.h"
#include "fat.h"

char detectionCallback(void)
{
	/*if(GPIO_PinRead(backwardLeftSensor) == 1 || GPIO_PinRead(forwardRightSensor) == 1)
	{
		stop(SOFT_STOP);
		PORTG = 0xFF;
		return 1;
	}

	PORTG = 0;*/
	
	return 0;
}

/*************************************************************************************************************************************************************************************
																POZICIJE,BRZINE,SMEROVI I DETEKCIJE ZA ZELENU STRANU
*************************************************************************************************************************************************************************************/
const gotoFields greenSideTacticOnePositions[TACTIC_ONE_POSITION_COUNT] =
{
	/*{{500, 500, 0}, LOW_SPEED, FORWARD, NULL},
	{{1200, 500, 0}, LOW_SPEED, FORWARD, detectionCallback}*/
	{{2300, 1030, 0}, LOW_SPEED, FORWARD, NULL}, //1
	{{2130, 1355, 0}, LOW_SPEED, FORWARD, NULL}, //2
	{{2700, 1800, 0}, LOW_SPEED, FORWARD, NULL}  //3
};




/*************************************************************************************************************************************************************************************
																				ZELENA STRANA
*************************************************************************************************************************************************************************************/
void greenSide(void)
{
	position startingPosition;
	unsigned char currentPosition = 0, nextPosition = 0, odometryStatus;
	unsigned char activeState = TACTIC_ONE;
	
	startingPosition.x = 2770;
	startingPosition.y = 1030;
	startingPosition.angle = 0;
	setPosition(startingPosition);
	
	while (1)
	{
		switch(activeState)
		{
			case TACTIC_ONE:
			for (currentPosition = nextPosition; currentPosition < TACTIC_ONE_POSITION_COUNT; currentPosition++)
			{
				// mozda ubaciti if-else sa akcijama tipa regularno- kretanje, i alternativno- sta god
				odometryStatus = gotoXY(greenSideTacticOnePositions[currentPosition].point, greenSideTacticOnePositions[currentPosition].speed,
										greenSideTacticOnePositions[currentPosition].direction, greenSideTacticOnePositions[currentPosition].detectionCallback);
				
				if(odometryStatus == ODOMETRY_FAIL)
				{

				}
				else if(odometryStatus == ODOMETRY_STUCK)
				{
					
				}
				
				if(currentPosition == 0)
				{
					
				}
				else if(currentPosition == 1)
				{
					/*while(greenSideTacticOnePositions[currentPosition].detectionCallback() != 0)
						_delay_ms(100);
										
					currentPosition--;
					while(1);*/
				} else if (currentPosition == 2)
				{
					while(1);
				}
			}//end for
			break;
		}
	}//end while(1)
	
	
}