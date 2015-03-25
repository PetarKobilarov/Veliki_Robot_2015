
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "usart.h"
#include "sides.h"



int main(void)
{
	systemInit();
	while(1)
		{
			greenSide();
		}
}
