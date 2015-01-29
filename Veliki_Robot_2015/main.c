
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "usart.h"

unsigned char forwardLeftSensor, backwardLeftSensor, forwardRightSensor, backwardRightSensor;
void mars(void)
{
	if(GPIO_PinRead(backwardLeftSensor) == 0 || GPIO_PinRead(forwardRightSensor) == 0 || GPIO_PinRead(forwardLeftSensor) == 0)
	{
		stop(SOFT_STOP);
		PORTG = 0xFF;
		while(1);
	}
}


const position positions[3] = 
{
	{200, 0, 0},
	{1000, 0, 0},
	{0, 0, 0}	
};

int main(void)
{
	Timer_Init(1000);
	DDRG = 0xFF;
	forwardLeftSensor = GPIO_PinRegister(GPIOE_BASE, 4); // backwardRight je u stvari
	forwardRightSensor = GPIO_PinRegister(GPIOB_BASE, 0); // tacan
	backwardLeftSensor = GPIO_PinRegister(GPIOE_BASE, 2); // tacan
	position start, p1;
	start.x = 0;
	start.y = 0;
	start.angle = 0;
	
    CAN_Init(4);

	Timer_Init(1000);
	_delay_ms(1000);
	setPosition(start);
	char i = 0;
	while(1)
		{
			for(i = 0; i < 3; ++i)
			{
				gotoXY(positions[i], 70, 1, NULL);
			}
			
			while(1);
		}
}
