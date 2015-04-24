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

position positinDetected, compare;

char greenDefaultFrontDetectionCallback(unsigned long startTime)
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

char greenDefaultRearDetectionCallback(unsigned long StartTime)
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

char greenDetectionCallbackFrontMiddle(unsigned long StartTime)
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

char correctLeftStand(unsigned long startTime)
{
	standColected = 0;
	if(standColected == 0)
	{
		if(getSystemTime() - startTime >= 1000)
		{
			liftMove(DOWN, LEFT_SIDE);
			standColected = 1;
		}
	}
	
	return 0;
}

char releaseLeftStand(unsigned long startTime)
{
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(HARD_STOP);
	
	}
	
	standColected = 0;
	if(standColected == 0)
	{
		leftDiafram(DEACTIVATE);
		standColected = 1;
	}
	
	return 0;
}

char releaseCupGreen(unsigned long startTime)
{
	colectThePopcorn(LEFT_SIDE, DEACTIVATE);
	return 0;
}

char clapperboardsKnockDownGreenSide(unsigned long startTime)
{
	if(checkRearSensors(MIDDLE) == DETECTED) 
	{
		stop(HARD_STOP);
	}
	
	if(clapperboardsClapped == 0)
	{
		_delay_ms(350);
		knockDownTheClapperboards(RIGHT_SIDE, DEACTIVATE);
		_delay_ms(1200);
		knockDownTheClapperboards(RIGHT_SIDE, ACTIVATE);
		clapperboardsClapped = 1;
	}
	
	return 0;
}

char colectTheFirstStandGreen(unsigned long startTime)
{
	if(standColected == 0)
	{
		if(getSystemTime() - startTime >= 300)
		{
			_delay_ms(500);
			liftMove(DOWN, RIGHT_SIDE);
			_delay_ms(500);
			liftMove(UP, RIGHT_SIDE);
		}
		
		standColected = 1;
	}
	
	return 0;
}

char popcornColectionGreenSide(unsigned long startTime)
{
	if(popcornColected == 0)
	{
		colectThePopcorn(LEFT_SIDE, ACTIVATE);
		popcornColected = 1;
	}
	
	return 0;
}

/*************************************************************************************************************************************************************************************
																POZICIJE,BRZINE,SMEROVI I DETEKCIJE ZA ZELENU STRANU
*************************************************************************************************************************************************************************************/
const gotoFields greenSideTacticOnePositions[TACTIC_ONE_POSITION_COUNT] =
{
	{{550, 1067, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},		//izlazi iz startnog polja		0
	{{780, 550, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},			//ide na poziciju prvog valjka	1
	{{1250, 590, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},		//ide do drugog valjka			2
	{{960, 320, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},			//ide do treceg valjka			3
	{{420, 330, 0}, NORMAL_SPEED, FORWARD, greenDetectionCallbackFrontMiddle},			//ide do prve case koju kupi	4
	{{950, 280, 0}, LOW_SPEED, BACKWARD, clapperboardsKnockDownGreenSide},				//rusi prve dve nase klapne		5
	{{500, 1020, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},		//ide da ostavi kulu			6
	{{400, 1020, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},		//ostavlja kulu i casu			7
	{{660, 1730, 0}, LOW_SPEED, FORWARD, greenDefaultFrontDetectionCallback},			//hvata 2 valjka kod podijuma	8
	{{240, 1710, 0}, NORMAL_SPEED, FORWARD, releaseLeftStand},							//hvata poslednji valjak		9
	{{600, 1500, 0}, NORMAL_SPEED, BACKWARD, greenDefaultRearDetectionCallback},		//ostavlja drugu kulu  			10
	{{1550, 350, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback},		//ide do zajednickog prostora	11
	{{1950, 350, 0}, NORMAL_SPEED, FORWARD, greenDefaultFrontDetectionCallback}			//nosi protivnicku kulu			12
};

/*************************************************************************************************************************************************************************************
																				ZELENA STRANA
*************************************************************************************************************************************************************************************/
void greenSide(void)
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
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 1)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
						break;
				}
				
				if(currentPosition == 2)
				{
					_delay_ms(200);
					positinDetected = getPosition();
					if((positinDetected.x >= 1150) || (checkForStands(RIGHT_SIDE) == DETECTED))
					{
						stop(HARD_STOP);
					}else
					{
						_delay_ms(1000);
					}
					liftMove(DOWN, RIGHT_SIDE);
					_delay_ms(500);
					rightDiafram(ACTIVATE);
					_delay_ms(500);
					liftMove(UP, RIGHT_SIDE);
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
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 4)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 5)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
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
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 8)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 9)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 10)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 11)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
						_delay_ms(100);
					nextPosition = currentPosition;
					activeState = TACTIC_ONE;
					break;
				}
				
				if(currentPosition == 12)
				{
					_delay_ms(200);
					while(greenSideTacticOnePositions[currentPosition].detectionCallback(0) != 0)
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
				// mozda ubaciti if-else sa akcijama tipa regularno- kretanje, i alternativno- sta god
				odometryStatus = gotoXY(greenSideTacticOnePositions[currentPosition].point, greenSideTacticOnePositions[currentPosition].speed,
										greenSideTacticOnePositions[currentPosition].direction, greenSideTacticOnePositions[currentPosition].detectionCallback);
				
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
					//uhvati  i podigne prvi valjak
					liftMove(UP, RIGHT_SIDE);
					
				}else if(currentPosition == 2)
				{
					//stane da bi uhvatio i podigao drugi valjak
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, RIGHT_SIDE);
					_delay_ms(500);
					liftMove(UP, RIGHT_SIDE);
					
					
				}else if(currentPosition == 3)
				{
					
				}else if(currentPosition == 4)
				{
					//stane da uhvati i podigne treci valjak
					stop(SOFT_STOP);
					_delay_ms(50);
					liftMove(DOWN, RIGHT_SIDE);
					_delay_ms(500);
					liftMove(UP, RIGHT_SIDE);
					
					//kupi prvu casu
					moveOnDirection(-10, LOW_SPEED, NULL);
					rotateFor(90, LOW_SPEED, popcornColectionGreenSide);
					_delay_ms(500);
					moveOnDirection(60, LOW_SPEED, NULL);//50
					colectThePopcorn(LEFT_SIDE, DEACTIVATE);
					moveOnDirection(-100, LOW_SPEED, NULL);
					rotateFor(-100, LOW_SPEED, NULL);
					_delay_ms(500);
					compare = getPosition();
					compare.x += 80;
					compare.y -= 25;
					gotoXY(compare, LOW_SPEED, BACKWARD, NULL);
					_delay_ms(50);
					rotateFor(30, LOW_SPEED, NULL);//25
					_delay_ms(50);
					moveOnDirection(270, NORMAL_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(125, 30, NULL);
					rotateFor(-5, LOW_SPEED, NULL);
					moveOnDirection(40, 30, NULL);
					liftMove(DOWN, RIGHT_SIDE);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
					rightDiafram(ACTIVATE);

					//okrece se da otvori udaraljku
					_delay_ms(50);
					rotateFor(10, LOW_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(-50, LOW_SPEED, NULL);
					rotateFor(-35, LOW_SPEED, NULL);
					knockDownTheClapperboards(RIGHT_SIDE, ACTIVATE);
					rotateFor(35, LOW_SPEED, NULL);
					
					
					
				}else if(currentPosition == 5)
				{
					knockDownTheClapperboards(RIGHT_SIDE, DEACTIVATE);
				}else if(currentPosition == 6)
				{
					
				}else if(currentPosition == 7)
				{
					theDoors(RIGHT_SIDE, ACTIVATE);
					rightDiafram(DEACTIVATE); //ostavlja toranj
					_delay_ms(300);
					compare = getPosition();
					compare.x += 220;
					gotoXY(compare, NORMAL_SPEED, BACKWARD, NULL);
					theDoors(RIGHT_SIDE, DEACTIVATE);
					liftMove(UP, LEFT_SIDE);
					rotateFor(90, LOW_SPEED, releaseCupGreen);
					_delay_ms(50);
					
					moveOnDirection(100, LOW_SPEED, NULL);
					_delay_ms(100);
					colectThePopcorn(LEFT_SIDE, ACTIVATE); //Ostavlja casu
					_delay_ms(100);
					moveOnDirection(-230, NORMAL_SPEED, NULL);
					_delay_ms(100);
					colectThePopcorn(LEFT_SIDE,CLOSE);//ne zatvori lepo
					
					liftMove(UP, LEFT_SIDE);
					
				}else if(currentPosition == 8) //kupi 2 valjka gore desno 
				{
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
					_delay_ms(800); //Povecan delay da bi imao vremena da podigne do kraja
					moveOnDirection(70, LOW_SPEED, NULL);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
					_delay_ms(500);
					compare = getPosition();
					compare.y -= 310;
					gotoXY(compare, NORMAL_SPEED, BACKWARD, NULL);
					//moveOnDirection(-310, NORMAL_SPEED, NULL);	
				}else if(currentPosition == 9) //hvata poslednji valjak
				{
					liftMove(UP, LEFT_SIDE);
					rotateFor(15,LOW_SPEED, NULL);
					moveOnDirection(170, LOW_SPEED, NULL);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
					_delay_ms(200);
				}else if(currentPosition == 10)
				{
					rotateFor(100, LOW_SPEED, NULL);
					moveOnDirection(380, LOW_SPEED, NULL);
					leftDiafram(DEACTIVATE);
					theDoors(LEFT_SIDE, ACTIVATE);
					moveOnDirection(-300, LOW_SPEED, NULL);
					theDoors(LEFT_SIDE, DEACTIVATE);
					
					
				}else if(currentPosition == 11)
				{
					rotateFor(25, LOW_SPEED, NULL);
					colectThePopcorn(LEFT_SIDE, ACTIVATE);
					
				}else if(currentPosition == 12)
				{
					while(1);
				}
				
				}//end for
				break;	
			}//END OF switch
	}//end while(1)
	
	
}//END OF greenSide