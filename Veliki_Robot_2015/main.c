
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
	Timer_Init(1000);
	DDRG = 0xFF;
	forwardLeftSensor = GPIO_PinRegister(GPIOA_BASE, 7); // backwardRight je u stvari
	forwardRightSensor = GPIO_PinRegister(GPIOA_BASE, 6); // tacan
	backwardLeftSensor = GPIO_PinRegister(GPIOA_BASE, 4); // tacan

	
    CAN_Init(4);

	Timer_Init(1000);
	_delay_ms(1000);

	while(1)
	{
		greenSide();	
	}
}
