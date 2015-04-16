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
			//yellowSide();
			
			leftDiafram(ACTIVATE);
			_delay_ms(1000);
			leftDiafram(DEACTIVATE);
			_delay_ms(1000);
			rightDiafram(ACTIVATE);
			_delay_ms(1000);
			rightDiafram(DEACTIVATE);
			_delay_ms(1000);
			liftMove(ACTIVATE);
			_delay_ms(1000);
			liftMove(DEACTIVATE);
		}
}
