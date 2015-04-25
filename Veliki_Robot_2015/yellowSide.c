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

char yellowDefaultDetectionCallbackWithoutRight(unsigned long startTime)
{
	if((checkFrontSensors(LEFT_SIDE) == DETECTED) || (checkFrontSensors(MIDDLE) == DETECTED))
	{
		stop(HARD_STOP);
		return 1;
	}
	return 0;
}

char yellowDefaultDetectionCallbackWithoutLeft(unsigned long startTime)
{
	if((checkFrontSensors(RIGHT_SIDE) == DETECTED) || (checkFrontSensors(MIDDLE) == DETECTED))
	{
		stop(HARD_STOP);
		return 1;
	}
	return 0;
}

char yellowDetectionCallbackFrontMiddle(unsigned long StartTime)
{
	if(checkFrontSensors(MIDDLE) == DETECTED)
	{
		PORTG = 0xff;
		stop(HARD_STOP);
		return 1;
	}
	PORTG = 0;
	return 0;
}


char yellowDefaultFrontDetectionCallback(unsigned long startTime)
{
	if(checkFrontSensors(ALL) == DETECTED)
	{
		PORTG = 0xff;
		stop(HARD_STOP);
		return 1;
	}
	PORTG = 0;
	return 0;
}

char yellowDefaultRearDetectionCallback(unsigned long StartTime)
{
	if(checkRearSensors(ALL) == DETECTED)
	{
		PORTG = 0xff;
		stop(HARD_STOP);
		return 1;
	}
	PORTG = 0;
	return 0;
}


char correctRightStand(unsigned long startTime)
{
		
	standColected = 0;
	if(standColected == 0)
	{
		liftMove(DOWN, RIGHT_SIDE);
		standColected = 1;
	}
	
	return 0;
}

char releaseRightStand(unsigned long startTime)
{
	if(checkFrontSensors(ALL) == DETECTED) 
	{
		stop(HARD_STOP);
		return 1;	
	}
	
	standColected = 0;
	if(standColected == 0)
	{
		rightDiafram(DEACTIVATE);
		standColected = 1;
	}
	
	return 0;
}

char releaseCup(unsigned long startTime)
{
	colectThePopcorn(RIGHT_SIDE, DEACTIVATE);
	return 0;
}

char clapperboardsKnockDownYellowSide(unsigned long startTime)
{
	if(clapperboardsClapped == 0)
	{
		_delay_ms(200);
		knockDownTheClapperboards(LEFT_SIDE, DEACTIVATE);
		_delay_ms(1200);
		knockDownTheClapperboards(LEFT_SIDE, ACTIVATE);
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
			_delay_ms(150);
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
	{{500, 1067, 0}, NORMAL_SPEED, FORWARD, yellowDefaultDetectionCallbackWithoutLeft},	//izlazi iz startnog polja		//0 u
	{{730, 1450, 0}, NORMAL_SPEED, FORWARD, yellowDefaultFrontDetectionCallback},	//ide na poziciju prvog valjka	//1 u
	{{1200, 1520, 0}, NORMAL_SPEED, FORWARD, yellowDefaultFrontDetectionCallback},	//ide do drugog valjka			//2 u
	{{1030, 1710, 0}, NORMAL_SPEED, FORWARD, yellowDefaultFrontDetectionCallback},	//ide do treceg valjka			//3 u
	{{430, 1710, 0}, NORMAL_SPEED, FORWARD, yellowDetectionCallbackFrontMiddle},	//ide do prve case koju kupi	//4 u
	{{900, 1800, 0}, LOW_SPEED, BACKWARD, clapperboardsKnockDownYellowSide},				//rusi prve dve nase klapne		//5 //nije uradjeno 
	{{500, 1067, 0}, NORMAL_SPEED, FORWARD, correctRightStand},						//ide da ostavi kulu			//6 u
	{{450, 1067, 0}, NORMAL_SPEED, FORWARD, yellowDefaultFrontDetectionCallback},	//ostavlja kulu i casu			//7 u
	{{700, 380, 0}, NORMAL_SPEED, FORWARD, yellowDefaultDetectionCallbackWithoutLeft},	//hvata 2 valjka kod podijuma	//8 u
	{{300, 400, 0}, NORMAL_SPEED, FORWARD, releaseRightStand},						//hvata poslednji valjak		//9 u
	{{600, 500, 0}, NORMAL_SPEED, BACKWARD, yellowDefaultRearDetectionCallback}		//ostavlja drugu kulu  			//10 u
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
			case COLLISION:
			if(currentPosition == 0)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
					_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 1)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
					_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 2)
			{
				_delay_ms(200);
				positinDetected = getPosition();
				if(positinDetected.x >= 1150)
				{
					stop(HARD_STOP);
				}else
				{
					_delay_ms(1000);
				}
				liftMove(DOWN, LEFT_SIDE);
				_delay_ms(500);
				rightDiafram(ACTIVATE);
				_delay_ms(500);
				liftMove(UP, LEFT_SIDE);
				_delay_ms(500);
				positinDetected = getPosition();
				positinDetected.x -= 200;
				gotoXY(positinDetected, NORMAL_SPEED, BACKWARD, NULL);
				nextPosition = ++currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 3)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
					_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 4)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
					_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 5)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
					_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 6)
			{
				_delay_ms(200);
				stop(HARD_STOP);
				_delay_ms(100);
				positinDetected.x = 1490;
				positinDetected.y = 1020;
				gotoXY(positinDetected, NORMAL_SPEED, FORWARD, NULL);
				_delay_ms(200);
				nextPosition = ++currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 7)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
				_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 8)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
				_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 9)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
				_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			if(currentPosition == 10)
			{
				_delay_ms(200);
				while(yellowSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
				_delay_ms(100);
				nextPosition = currentPosition;
				activeState = TACTIC_ONE;
				break;
			}
			
			
			case STUCK:
			_delay_ms(1000);
			activeState = TACTIC_ONE;
			nextPosition = currentPosition;
			
			break;
			
			case TACTIC_ONE:
			for (currentPosition = nextPosition; currentPosition < TACTIC_ONE_POSITION_COUNT; currentPosition++)
			{
				//logger("Active TACTIC_ONE: going to position %d ", currentPosition);
				// mozda ubaciti if-else sa akcijama tipa regularno- kretanje, i alternativno- sta god
				
				odometryStatus = gotoXY(yellowSideTacticOnePositions[currentPosition].point, yellowSideTacticOnePositions[currentPosition].speed,
				yellowSideTacticOnePositions[currentPosition].direction, yellowSideTacticOnePositions[currentPosition].detectionCallback);
				
				if(odometryStatus == ODOMETRY_FAIL)
				{
					activeState = COLLISION;
					break;
				}
				else if(odometryStatus == ODOMETRY_STUCK)
				{
					
				}
				
				if(currentPosition == 0)
				{
					
				}
				else if(currentPosition == 1)
				{
					liftMove(UP, LEFT_SIDE);
					
				}else if(currentPosition == 2)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
					
				}else if(currentPosition == 3)
				{
					
				}else if(currentPosition == 4)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
					
					rotateFor(-90, LOW_SPEED,popcornColectionYellowSide);	
					moveOnDirection(80, LOW_SPEED, NULL);
					colectThePopcorn(RIGHT_SIDE, DEACTIVATE);
					rotateFor(100, LOW_SPEED, NULL);
					moveOnDirection(-80, LOW_SPEED, NULL);
					rotateFor(-15, LOW_SPEED, NULL);
					moveOnDirection(300, NORMAL_SPEED, NULL);
					moveOnDirection(100, 30, NULL);
					rotateFor(5, LOW_SPEED, NULL);
					_delay_ms(50);
					liftMove(DOWN, BOTH);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
					rightDiafram(ACTIVATE);
					_delay_ms(50);
					moveOnDirection(-30, LOW_SPEED, NULL);
					rotateFor(30, LOW_SPEED, NULL);
					knockDownTheClapperboards(LEFT_SIDE, ACTIVATE);
					rotateFor(-30, LOW_SPEED, NULL);
					
				}else if(currentPosition == 5)
				{
					knockDownTheClapperboards(LEFT_SIDE, DEACTIVATE);
					
				}else if(currentPosition == 6)
				{
					rightDiafram(ACTIVATE);
					
				}else if(currentPosition == 7)
				{
					theDoors(LEFT_SIDE, ACTIVATE);
					leftDiafram(DEACTIVATE);
					_delay_ms(300);
					moveOnDirection(-400, LOW_SPEED, NULL);
					_delay_ms(100);
					moveOnDirection(200, LOW_SPEED, NULL);
					theDoors(LEFT_SIDE, DEACTIVATE);
					liftMove(UP, RIGHT_SIDE);
					rotateFor(-90, LOW_SPEED, releaseCup);
					_delay_ms(50);
					
					moveOnDirection(100, LOW_SPEED, NULL);
					_delay_ms(100);
					colectThePopcorn(RIGHT_SIDE, ACTIVATE); //Ostavlja casu
					_delay_ms(100);
					moveOnDirection(-180, NORMAL_SPEED, NULL);
					_delay_ms(100);
					colectThePopcorn(RIGHT_SIDE, CLOSE);
					
					liftMove(UP, RIGHT_SIDE);
					
			}else if(currentPosition == 8)
			{  
				liftMove(DOWN, RIGHT_SIDE);
				_delay_ms(500);
				liftMove(UP, RIGHT_SIDE);
				_delay_ms(500);
				moveOnDirection(80, LOW_SPEED, NULL);
				liftMove(DOWN, RIGHT_SIDE);
				_delay_ms(500);
				rightDiafram(ACTIVATE);
				_delay_ms(500);
				moveOnDirection(-290, NORMAL_SPEED, NULL);
				
			}else if(currentPosition == 9)
			{
				liftMove(UP, RIGHT_SIDE);
				rotateFor(-15,LOW_SPEED, NULL);
				moveOnDirection(170, LOW_SPEED, NULL);
				liftMove(DOWN, RIGHT_SIDE);
				_delay_ms(500);
				rightDiafram(ACTIVATE);
				_delay_ms(200);
			
			}else if(currentPosition == 10)
			{
				rotateFor(-100, LOW_SPEED, NULL);
				moveOnDirection(380, LOW_SPEED, NULL);
				theDoors(RIGHT_SIDE, ACTIVATE);
				rightDiafram(DEACTIVATE);
				moveOnDirection(-300, LOW_SPEED, NULL);
				theDoors(RIGHT_SIDE, DEACTIVATE);
				while(1);
			}
			
			}//end for
			break;
		}
	}//end while(1)
	
}