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

char defaultFrontDetectionCallback(unsigned long startTime)
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

char defaultRearDetectionCallback(unsigned long StartTime)
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

char detectionCallbackFrontMiddle(unsigned long StartTime)
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
	{{550, 1067, 0}, NORMAL_SPEED, FORWARD, NULL},							//izlazi iz startnog polja			0
	{{780, 550, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback},							//ide na poziciju prvog valjka		1
	{{1250, 590, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback},							//ide do drugog valjka				2
	{{960, 320, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback},							//ide do treceg valjka				3
	{{420, 290, 0}, NORMAL_SPEED, FORWARD, detectionCallbackFrontMiddle},							//ide do prve case koju kupi		4
	{{950, 280, 0}, LOW_SPEED, BACKWARD, clapperboardsKnockDownGreenSide},							//rusi prve dve nase klapne			5
	{{500, 1020, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback},							//ide da ostavi kulu				6
	{{400, 1020, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback},							//ostavlja kulu i casu				7
	{{660, 1730, 0}, LOW_SPEED, FORWARD, defaultFrontDetectionCallback},							//hvata 2 valjka kod podijuma		8
	{{240, 1690, 0}, NORMAL_SPEED, FORWARD, releaseLeftStand},										//hvata poslednji valjak			9
	{{600, 1500, 0}, NORMAL_SPEED, BACKWARD, defaultRearDetectionCallback},							//ostavlja drugu kulu  				10
	{{1550, 350, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback},							//ide do zajednickog prostora		11
	{{1950, 350, 0}, NORMAL_SPEED, FORWARD, defaultFrontDetectionCallback}							//nosi protivnicku kulu				12
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
				PORTG = 0xFF; 
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
			PORTG = 0;
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
					if(checkForStands(RIGHT_SIDE) == DETECTED)//proverava da li je na poziciji valjka
					{
						liftMove(UP, RIGHT_SIDE);//ako je na poziciji onda uhvati valjak i podigne lift
						numOfStandsRight++;//povecava broj uhvacenih valjaka u desnoj ruci na 1
					}else
					{
						moveOnDirection(50, LOW_SPEED, NULL);//ako nije na poziciji onda se pomeri jos malo napred
						if(checkForStands(RIGHT_SIDE) == DETECTED)//ponovo proverava da li ima valjka
						{
							liftMove(UP, RIGHT_SIDE);//ako ima valjka onda ga uhvati i podigne
							numOfStandsRight++;//poveca broj  uhvacenih valjaka u desnoj ruci na 1
						}
					}
					liftMove(UP, RIGHT_SIDE);//ako ne uspe da ga detektuje ipak dize lift
					if (checkLiftSensor(UP) == FAIL)//proverava da li je lift gore
					{
						liftMove(UP, RIGHT_SIDE);//ako nije onda ponovo pokusava da ga digne
						_delay_ms(500);
					}
					
				}else if(currentPosition == 2)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					if(checkForStands(RIGHT_SIDE) == DETECTED)//proverava da li je valjak usao u drzac
					{
						liftMove(DOWN, RIGHT_SIDE);//ako jeste onda spusta lift i priprema se da uhvati valjak
						if(checkLiftSensor(DOWN) == FAIL)//proverava da li se lift spustio do kraja
						{
							_delay_ms(500);
							liftMove(UP, RIGHT_SIDE);//ako nije ponovi digne lift
							rotateFor(15, LOW_SPEED, NULL);//okrene se u jednu stranu
							rotateFor(-15, LOW_SPEED, NULL);//okrene se u drugu stranu(pokusava da ga namesti u drzac
							_delay_ms(500);
							liftMove(DOWN, RIGHT_SIDE);//ponovo pokusa da spusti lift
						}
						_delay_ms(500);
						liftMove(UP, RIGHT_SIDE);//pokusa da podigne lift 
						_delay_ms(500);
						if(checkLiftSensor(UP) == FAIL)//proverava da li je lift stigao gore
						{
							liftMove(DOWN, RIGHT_SIDE);//spusti lift
							_delay_ms(500);
							liftMove(UP, RIGHT_SIDE);//ponovo pokusa da podigne valjak
						}
						numOfStandsRight++;//povecava broj uhvacenih valjkaka na 2
					}else
					{
						moveOnDirection(50, LOW_SPEED, NULL);//ako ga nije detektovao pokusava da ga namesti malo u drzac
						if(checkForStands(RIGHT_SIDE) == DETECTED)//provera da li ga je detektovao
						{
							liftMove(DOWN, RIGHT_SIDE);//ako je detektovao spusta lift i priprema se da uhvati valjak
							if(checkLiftSensor(DOWN) == FAIL)//proverava da li je lift dostigao poziciju
							{
								liftMove(UP, RIGHT_SIDE);//ako nije ponovo dize lift
								_delay_ms(500);
								liftMove(DOWN, RIGHT_SIDE);//pokusava ponovo da spusti lift
							}
							_delay_ms(500);
							liftMove(UP, RIGHT_SIDE);//dize lift da bi mogao ici po sledeci valjak 
							_delay_ms(500);
							if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
							{
								liftMove(DOWN, RIGHT_SIDE);//spusta lift
								_delay_ms(500);
								liftMove(UP, RIGHT_SIDE);//pokusava ponovo da ga digne 
							}
							numOfStandsRight++;///povecava broj uhvacenih valjkaka na 2
							
						}
					}
					liftMove(DOWN, RIGHT_SIDE);//i ako nije detektovao valjak ipak spusta lift
					_delay_ms(500);
					liftMove(UP, RIGHT_SIDE);//i pokusava da digne valjak i ako ga nije detektovao
					
				}else if(currentPosition == 3)
				{
					
				}else if(currentPosition == 4)
				{
					stop(SOFT_STOP);
					_delay_ms(50);
					if(checkForStands(RIGHT_SIDE) == DETECTED)//proverava da li je valjak usao u drzac
					{
						liftMove(DOWN, RIGHT_SIDE);//pokusa da spusti lift
						_delay_ms(500);
						if(checkLiftSensor(DOWN) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(UP, RIGHT_SIDE);//ako nije ponovo podigne lift 
							_delay_ms(500);
							liftMove(DOWN, RIGHT_SIDE);//i onda ga ponovo spusti
						}
						liftMove(UP, RIGHT_SIDE);//pokusa da podigne lift
						_delay_ms(500);
						if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(DOWN, RIGHT_SIDE);//ako ne uspe onda ga spusi
							_delay_ms(500);
							liftMove(UP, RIGHT_SIDE);//i ponovo pokusa da podigne
						}
						numOfStandsRight++;//povecava broj skupljenih valjaka u desnom drzacu na 3
					}else
					{
						moveOnDirection(50, LOW_SPEED, NULL);//ako nije detektovao valjak pokusa ponovo da ga namesti u drzac
						if(checkForStands(RIGHT_SIDE) == DETECTED)//proverava da li detektuje valjak
						{
							liftMove(DOWN, RIGHT_SIDE);//pokusa da spusti lift
							_delay_ms(500);
							if(checkLiftSensor(DOWN) == FAIL)//proverava da li je lift dostigao poziciju
							{
								liftMove(UP, RIGHT_SIDE);//ako nije podigne lift
								_delay_ms(500);
								liftMove(DOWN, RIGHT_SIDE);//i pokusa ponovo da ga spusti
							}
							liftMove(UP, RIGHT_SIDE);//dize lift
							_delay_ms(500);
							if(checkLiftSensor(UP) == FAIL)//proverava da li je dostigao poziciju
							{
								liftMove(DOWN, RIGHT_SIDE);//ako nije spusti lift
								_delay_ms(500);
								liftMove(UP, RIGHT_SIDE);//i pokusa da ga digne
							}
							numOfStandsRight++;//povecava broj skupljenih valjaka u desnom drzacu na 3
							
						}
					}
					liftMove(DOWN, RIGHT_SIDE);//pokusa da spusti lift i pripremi se da uhvati valjak i ako ga nije detektovao
					_delay_ms(500);
					liftMove(UP, RIGHT_SIDE);//pokusa da uhvati i podigne valjak
					
					//kupi prvu casu
					rotateFor(90, LOW_SPEED, popcornColectionGreenSide);
					moveOnDirection(60, LOW_SPEED, NULL);
					colectThePopcorn(LEFT_SIDE, DEACTIVATE);
					moveOnDirection(-100, LOW_SPEED, NULL);
					rotateFor(-100, LOW_SPEED, NULL);
					_delay_ms(50);
					
					compare = getPosition();
					compare.x += 80;
					compare.y -= 25;
					gotoXY(compare, LOW_SPEED, BACKWARD, NULL);
					
					_delay_ms(50);
					rotateFor(25, LOW_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(200, NORMAL_SPEED, NULL);
					_delay_ms(50);
					moveOnDirection(125, 30, NULL);
					rotateFor(-10, LOW_SPEED, NULL);
					moveOnDirection(35, 30, NULL);
					
					liftMove(DOWN, RIGHT_SIDE);//pokusa da spusti lift
					_delay_ms(500);
					if(checkForStands(ALL) == DETECTED)//proverava da li je lift dostigao poziciju
					{
						leftDiafram(ACTIVATE);//ako jeste onda aktivira levu dijafragmu
						rightDiafram(ACTIVATE);//i desnu
						numOfStandsRight++;//povecava broj uhvacenih valjaka sa densne strane na 4
						numOfStandsLeft++;//povecava broj uhvacenih valjaka sa leve strane na 1
					}else
					{
						moveOnDirection(50, 20, NULL);//ako ne detektuje pokusa da namesti valjak
						if(checkForStands(ALL) == DETECTED)//proverava da li je sada valjak usao u drzac
						{
							leftDiafram(ACTIVATE);//ako jeste onda aktivira levu dijafragmu
							rightDiafram(ACTIVATE);//aktivira desnu dijafragmu
							numOfStandsRight++;//povecava broj uhvacenih valjaka sa densne strane na 4
							numOfStandsLeft++;//povecava broj uhvacenih valjaka sa leve strane na 1
						}
					}
					leftDiafram(ACTIVATE);//pokusa da uhvati levu dijafragmu i ako nije detektovao
					rightDiafram(ACTIVATE);//pokusa da uhvati desnu dijafragmu i ako nije detektovao

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
					/*if(numOfStandsRight != 4)
					{
						rotateFor(90, LOW_SPEED, releaseCupGreen);
						_delay_ms(50);
						
						moveOnDirection(100, LOW_SPEED, NULL);
						_delay_ms(100);
						colectThePopcorn(LEFT_SIDE, ACTIVATE); //Ostavlja casu
						_delay_ms(100);
						moveOnDirection(-230, NORMAL_SPEED, NULL);
						_delay_ms(100);
						colectThePopcorn(LEFT_SIDE,CLOSE);//ne zatvori lepo
						
						liftMove(UP, BOTH);
						
						currentPosition = ++currentPosition;
					}*/
					theDoors(RIGHT_SIDE, ACTIVATE);
					rightDiafram(DEACTIVATE); //ostavlja toranj
					_delay_ms(300);
					compare = getPosition();
					compare.x += 220;
					gotoXY(compare, NORMAL_SPEED, BACKWARD, NULL);
					theDoors(RIGHT_SIDE, DEACTIVATE);
					
					liftMove(UP, LEFT_SIDE);
					_delay_ms(500);
					if(checkLiftSensor(UP) == FAIL)//proverava da li je lift postigao poziciju
					{
						liftMove(DOWN, RIGHT_SIDE);//spusti lift
						_delay_ms(500);
						liftMove(UP, RIGHT_SIDE);//i pokusa da ga podigne
					}
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
					_delay_ms(500);
					if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
					{
						liftMove(DOWN, LEFT_SIDE);//spusti lift 
						_delay_ms(500);
						liftMove(UP, LEFT_SIDE);//i pokusa da ga ponovo podigne
					}
					
				}else if(currentPosition == 8)
				{
					if(checkForStands(LEFT_SIDE) == DETECTED)//proverava da li je valjak usao u drzac
					{
						liftMove(DOWN, LEFT_SIDE);//spusti lift
						_delay_ms(500);
						if(checkLiftSensor(DOWN) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(UP, LEFT_SIDE);//digne lift
							_delay_ms(500);
							liftMove(DOWN, LEFT_SIDE);//i pokusa ponovo da ga spusti
						}
						
						liftMove(UP, LEFT_SIDE);
						_delay_ms(500);
						if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(DOWN, LEFT_SIDE);//ako nije spusti lift
							_delay_ms(500);
							liftMove(UP, LEFT_SIDE);//i pokusa ponovo da ga podigne
							numOfStandsLeft++;//poveca broj uhvacenih valjaka sa leve strane na 2
						}
					}else
					{
						moveOnDirection(50, LOW_SPEED, NULL);
						liftMove(DOWN, LEFT_SIDE);//spusti lift
						_delay_ms(500);
						if(checkLiftSensor(DOWN) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(UP, LEFT_SIDE);//digne lift
							_delay_ms(500);
							liftMove(DOWN, LEFT_SIDE);//i pokusa ponovo da ga spusti
						}
						
						liftMove(UP, LEFT_SIDE);
						_delay_ms(500);
						if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(DOWN, LEFT_SIDE);//ako nije spusti lift
							_delay_ms(500);
							liftMove(UP, LEFT_SIDE);//i pokusa ponovo da ga podigne
							numOfStandsLeft++;//poveca broj uhvacenih valjaka sa leve strane na 2
						}
					}
					liftMove(DOWN, LEFT_SIDE);//i ako nije detektovao spusti lift
					_delay_ms(500);
					liftMove(UP, LEFT_SIDE);//i pokusa da podigne
					_delay_ms(500);
					
					moveOnDirection(70, LOW_SPEED, NULL);
					
					liftMove(DOWN, LEFT_SIDE);
					_delay_ms(500);
					if(checkForStands(LEFT_SIDE) == DETECTED)//proverava da li je valjak usao u drzac
					{
						leftDiafram(ACTIVATE);//ako jeste aktivira dijafragmu
						_delay_ms(200);
						numOfStandsLeft++;////poveca broj uhvacenih valjaka sa leve strane na 3
					}
					leftDiafram(ACTIVATE);//aktivira dijafragmu i ako nije detektovao valjak
					_delay_ms(200);
					
					compare = getPosition();
					compare.y -= 310;
					gotoXY(compare, NORMAL_SPEED, BACKWARD, NULL);
				}else if(currentPosition == 9)
				{
					if(checkForStands(LEFT_SIDE) == DETECTED)
					{
						liftMove(UP, LEFT_SIDE);
						_delay_ms(500);
						if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
						{
							liftMove(DOWN, LEFT_SIDE);//ako nije onda lift spusti
							_delay_ms(500);
							liftMove(UP,LEFT_SIDE);//i pokusa ponovo da ga podigne
							_delay_ms(500);
						}
						rotateFor(15,LOW_SPEED, NULL);
						moveOnDirection(160, LOW_SPEED, NULL);
					
						liftMove(DOWN, LEFT_SIDE);
						_delay_ms(500);
						if(checkLiftSensor(DOWN) == FAIL)
						{
							liftMove(UP, LEFT_SIDE);
							_delay_ms(500);
							liftMove(DOWN, LEFT_SIDE);
							_delay_ms(500);
						}
					
						leftDiafram(ACTIVATE);
						_delay_ms(200);
					}else
					{
						moveOnDirection(50, LOW_SPEED, NULL);
						if(checkForStands(LEFT_SIDE) == DETECTED)
						{
							liftMove(UP, LEFT_SIDE);
							_delay_ms(500);
							if(checkLiftSensor(UP) == FAIL)//proverava da li je lift dostigao poziciju
							{
								liftMove(DOWN, LEFT_SIDE);//ako nije onda lift spusti
								_delay_ms(500);
								liftMove(UP,LEFT_SIDE);//i pokusa ponovo da ga podigne
								_delay_ms(500);
							}
							rotateFor(15,LOW_SPEED, NULL);
							moveOnDirection(160, LOW_SPEED, NULL);
							
							liftMove(DOWN, LEFT_SIDE);
							_delay_ms(500);
							if(checkLiftSensor(DOWN) == FAIL)
							{
								liftMove(UP, LEFT_SIDE);
								_delay_ms(500);
								liftMove(DOWN, LEFT_SIDE);
								_delay_ms(500);
							}
							
							leftDiafram(ACTIVATE);
							_delay_ms(200);
						}
					}
					
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
					while(1);
					
				}else if(currentPosition == 12)
				{
					
				}
				
				}//end for
				break;	
			}//END OF switch
	}//end while(1)
	
	
}//END OF greenSide