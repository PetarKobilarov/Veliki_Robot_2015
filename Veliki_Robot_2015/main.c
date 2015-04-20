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
			liftMove(UP, LEFT_SIDE);
			yellowSide();
		
		}//END OF while(1)
			
}
