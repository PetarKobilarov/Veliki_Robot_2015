#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "usart.h"
#include "sides.h"
#include "actuators.h"



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
			
			yellowSide();
			
			/*knockDownTheClapperboards(RIGHT_SIDE);
			leftDiafram(ACTIVATE);
			rightDiafram(ACTIVATE);
			liftMove(ACTIVATE);
			_delay_ms(15000);
			leftDiafram(DEACTIVATE);
			rightDiafram(DEACTIVATE);
			liftMove(DEACTIVATE);*/
			
		}
}
