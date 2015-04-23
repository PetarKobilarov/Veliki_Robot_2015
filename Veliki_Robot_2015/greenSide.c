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

position compare;

char sensorDetectionCollbackPosition0(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}

char sensorDetectionCollbackPosition1(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}

char sensorDetectionCollbackPosition2(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}

char sensorDetectionCollbackPosition3(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}

char sensorDetectionCollbackPosition4(unsigned long startTime) {
	if((checkFrontSensors(RIGHT_SIDE) == DETECTED) || (checkFrontSensors(MIDDLE) == DETECTED)) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}
	
char sensorDetectionCollbackPosition6(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
	}
	return 0;
}

char sensorDetectionCollbackPosition7(unsigned long startTime) {
	if(checkRearSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}

char sensorDetectionCollbackPosition8(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
	return 0;
}

char sensorDetectionCollbackPosition10(unsigned long startTime) {
	if(checkFrontSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		return 1;
	}
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
		stop(SOFT_STOP);
		currentPosition -= 1;
		_delay_ms(5000);
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
	if(checkRearSensors(ALL) == DETECTED) {
		stop(SOFT_STOP);
		currentPosition -= 1;
		_delay_ms(5000);
	}
	
	if(clapperboardsClapped == 0)
	{
		_delay_ms(700);
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
	{{550, 1067, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition0},							//izlazi iz startnog polja			0
	{{780, 550, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition1},							//ide na poziciju prvog valjka		1
	{{1250, 590, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition2},							//ide do drugog valjka				2
	{{960, 290, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition3},							//ide do treceg valjka				3
	{{420, 290, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition4},							//ide do prve case koju kupi		4
	{{950, 280, 0}, LOW_SPEED, BACKWARD, clapperboardsKnockDownGreenSide},								//rusi prve dve nase klapne			5
	{{500, 1020, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition6},							//ide da ostavi kulu				6
	{{400, 1020, 0}, NORMAL_SPEED, FORWARD, sensorDetectionCollbackPosition7},							//ostavlja kulu i casu				7
	{{660, 1730, 0}, LOW_SPEED, FORWARD, sensorDetectionCollbackPosition8},								//hvata 2 valjka kod podijuma		8
	{{240, 1690, 0}, NORMAL_SPEED, FORWARD, releaseLeftStand},											//hvata poslednji valjak			9
	{{600, 1500, 0}, NORMAL_SPEED, BACKWARD, sensorDetectionCollbackPosition10}							//ostavlja drugu kulu  				10
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
					rotate(90, LOW_SPEED, popcornColectionGreenSide);
					moveOnDirection(60, LOW_SPEED, NULL);
					colectThePopcorn(LEFT_SIDE, DEACTIVATE);
					moveOnDirection(-100, LOW_SPEED, NULL);
					rotate(-100, LOW_SPEED, NULL);
					_delay_ms(500);
					compare = getPosition();
					compare.x += 80;
					compare.y -= 25;
					gotoXY(compare, LOW_SPEED, BACKWARD, NULL);
					_delay_ms(50);
					rotate(25, LOW_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(270, NORMAL_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(125, 30, NULL);
					rotate(-10, LOW_SPEED, NULL);
					moveOnDirection(40, 30, NULL);
					liftMove(DOWN, RIGHT_SIDE);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
					rightDiafram(ACTIVATE);

					//okrece se da otvori udaraljku
					_delay_ms(50);
					rotate(10, LOW_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(-50, LOW_SPEED, NULL);
					rotate(-35, LOW_SPEED, NULL);
					knockDownTheClapperboards(RIGHT_SIDE, ACTIVATE);
					rotate(35, LOW_SPEED, NULL);
					
					
					
				}else if(currentPosition == 5)
				{
					knockDownTheClapperboards(RIGHT_SIDE, DEACTIVATE);
				}else if(currentPosition == 6)
				{
					
				}else if(currentPosition == 7)
				{
					rightDiafram(DEACTIVATE); //ostavlja toranj
					_delay_ms(5000);
					moveOnDirection(-220, NORMAL_SPEED, NULL);
					liftMove(UP, LEFT_SIDE);
					rotate(90, LOW_SPEED, releaseCupGreen);
					_delay_ms(50);
					
					moveOnDirection(100, LOW_SPEED, NULL);
					_delay_ms(100);
					colectThePopcorn(LEFT_SIDE, ACTIVATE); //Ostavlja casu
					_delay_ms(100);
					moveOnDirection(-230, NORMAL_SPEED, NULL);
					colectThePopcorn(LEFT_SIDE,CLOSE);//ne zatvori lepo
					
					liftMove(UP, LEFT_SIDE);
					
					rotate(-180,NORMAL_SPEED, NULL);
				}else if(currentPosition == 8) //kupi 2 valjka gore desno 
				{
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);
					_delay_ms(500);
					moveOnDirection(90, LOW_SPEED, NULL);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
					_delay_ms(500);
					moveOnDirection(-310, NORMAL_SPEED, NULL);	
				}else if(currentPosition == 9) //hvata poslednji valjak
				{
					liftMove(UP, LEFT_SIDE);
					rotate(15,LOW_SPEED, NULL);
					moveOnDirection(170, LOW_SPEED, NULL);
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					leftDiafram(ACTIVATE);
				}else if(currentPosition == 10)
				{
					rotate(100, LOW_SPEED, NULL);
					moveOnDirection(380, LOW_SPEED, NULL);
					leftDiafram(DEACTIVATE);
					_delay_ms(5000);
					moveOnDirection(-300, LOW_SPEED, NULL);
					
					while(1);
				}
				}//end for
				break;	
			}//END OF switch
	}//end while(1)
	
	
}//END OF greenSide