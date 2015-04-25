#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "usart.h"
#include "sides.h"
#include "actuators.h"
#include "odometry.h"

unsigned char i;

void TimerHook(void)
{
	if(matchIsStarted() == 1 && getSystemTime() > 90000)
	{
		PORTG = 0xFF;
		stop(HARD_STOP);
		while(1);
	}
}

int main(void)
{
<<<<<<< HEAD
	
	systemInit();
=======
	systemInit();
	_delay_ms(1500);
>>>>>>> fc4f9d83ff2ab8c0174ab1becc50da0b740b78e8
	//servo_position1(238); // servo1: 225-otvoren, 238-zatvoren
	//servo_position2(235); // servo2: 235-otvoren, 215-zatvoren
	while(1)
		{	
<<<<<<< HEAD
			if(sidesSwitchCheck() == 0)
			{
				greenSide();
			}else
			{
				yellowSide();
			}
			
=======
			greenSide();
>>>>>>> fc4f9d83ff2ab8c0174ab1becc50da0b740b78e8
		}//END OF while(1)
			
}
