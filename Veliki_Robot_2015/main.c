#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "usart.h"
#include "sides.h"
#include "actuators.h"
#include "fat.h"


int main(void)
{
	systemInit();
	
	while(1)
		{
			/*if (sidesSwitchCheck() == 0)
			{
				greenSide();
			}
			else
			{
				yellowSide();
			}*/
			
			//yellowSide();
			
			/*//if ((GPIO_PinRead(forwardLowerLeftSensor) == 1) || (GPIO_PinRead(forwardLowerRightSensor) == 1) || (GPIO_PinRead(backwardLeftSensor) == 1) || (GPIO_PinRead(backwardRightSensor) == 1) || (GPIO_PinRead(forwardUpperLeftSensor) == 1) || (GPIO_PinRead(forwardUpperRightSensor) == 1) || (GPIO_PinRead(upperLiftSensor) == 1) || (GPIO_PinRead(lowerLiftSensor) == 1))
			if((GPIO_PinRead(backwardMiddleSensor) == 0) || (GPIO_PinRead(forwardMiddleSensor) == 0))
			{
				PORTG = 0xFF;
			}else
			{
				PORTG = 0;
			}*/
			
			
		}//END OF while(1)
			
}
