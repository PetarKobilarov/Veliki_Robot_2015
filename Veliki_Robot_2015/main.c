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
	_delay_ms(100);
	
	//PORT.F4
	while(1)
		{	
			if(PORTF4 == 1)
			{
				//_delay_ms(500);
				yellowSide();
			}else
			{
				//_delay_ms(500);
				greenSide();
			}
			
			
			//greenSide();
		}//END OF while(1)
			
}
