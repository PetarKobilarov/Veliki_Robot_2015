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
	systemInit();
	
	//servo_position1(238); // servo1: 225-otvoren, 238-zatvoren
	//servo_position2(215); // servo2: 235-otvoren, 215-zatvoren
	while(1)
		{	
			greenSide();

		}//END OF while(1)
			
}
