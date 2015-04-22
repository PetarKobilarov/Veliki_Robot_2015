#include "system.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fat.h"
#include "actuators.h"

#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))

struct gpio
{
	volatile unsigned char *baseAddress;
	volatile unsigned char pinPosition;
	volatile unsigned char buffer[3];
};

typedef struct gpio GPIOData;

static volatile GPIOData *gpios[MAX_INPUTS];
static volatile unsigned char inputsNumber = 0;
static volatile unsigned long systemTime;

unsigned char GPIO_PinRegister(volatile unsigned char *baseAddress, unsigned char pin)
{
	if(inputsNumber >= MAX_INPUTS)
		return 0;

	unsigned char i;

	gpios[inputsNumber] = (GPIOData *)malloc(sizeof(GPIOData));
	if(gpios[inputsNumber] == NULL)
		return -1;

	gpios[inputsNumber]->baseAddress = baseAddress;
	gpios[inputsNumber]->pinPosition = pin;
	for(i = 0; i < 3; i++)
		gpios[inputsNumber]->buffer[i] = 0;

	_MMIO_BYTE(baseAddress - 1) &= (0 << pin);
	_MMIO_BYTE(baseAddress) &= (0 << pin);

	i = inputsNumber;
	inputsNumber++;

	return i;
}

unsigned char GPIO_PinRead(unsigned char pinHandler)
{
	return ( (gpios[pinHandler]->buffer[0]) & (gpios[pinHandler]->buffer[1]) & (gpios[pinHandler]->buffer[2]) );
}

unsigned char GPIO_ReadFromRegister(unsigned char pinHandler)
{
	unsigned char state = 0;

	state = ((_MMIO_BYTE(gpios[pinHandler]->baseAddress - 2)) >> (gpios[pinHandler]->pinPosition)) & 0x01;

	return state;
}

void fillDebaunsingData(void)
{
	unsigned char i;
	static char j = 0;

	if(++j >= 3)
		j = 0;

	for(i = 0; i < inputsNumber; ++i)
		gpios[i]->buffer[j] = GPIO_ReadFromRegister(i);
}

void Timer_Init(unsigned int freq)
{
    TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = (double)F_CPU / freq + 0.5;
	TIMSK1 = 1 << OCIE1A;

	SREG |= 0x80;
}

ISR(TIMER1_COMPA_vect)
{
	fillDebaunsingData();
    #if USE_TIMER_HOOK == 1
    Timer_Hook();
    #endif // USE_TIMER_HOOK
	systemTime++;
}

//static unsigned char logFile = 0;

/*void logger(const char *text, ...)
{
	va_list list;
	unsigned int maxSize = 50;
	signed int status;
	char *buff = (char *)calloc(maxSize, sizeof(char));
	while(1)
	{
		if(buff == NULL)
			return NULL;

		va_start(list, text);
		status = vsnprintf(buff, maxSize, text, list);
		va_end(list);

		if(status > -1 && status < maxSize)
			break;

		maxSize *= 2;
		buff = (char *)realloc(buff, maxSize);
	}
	
	FAT_WriteFile(logFile, ADD_TO_END, text);
	free(buff);
}*/

void systemInit(void)
{
	DDRG = 0xFF;

	
	/*_delay_ms(700);
	if(SD_Init(SPI_Init, SPI_ReadWrite, PB0, &PORTB) != -1)
		FAT_Init();
	_delay_ms(700);*/
	
	//logFile = FAT_OpenFile("LOG     ", 0);
	//if(logFile <= 0)
	//logFile = FAT_CreateFile("LOG", 0);
	//logger("System init started.");
	
	Timer_Init(1000);
	CAN_Init(4);
	
	//logger("Initializing digital inputs...\n\r");
	forwardUpperLeftSensor = GPIO_PinRegister(GPIOA_BASE, 4);//prednji gornji levi senzor za detekciju protivnika		//radi
	forwardUpperRightSensor = GPIO_PinRegister(GPIOA_BASE, 5); //prednji gornji desni senzor za detekciju protivnika	//radi
	forwardLowerLeftSensor = GPIO_PinRegister(GPIOA_BASE, 0);//prednji donji levi senzor za detekciju valjka			//radi
	forwardLowerRightSensor = GPIO_PinRegister(GPIOA_BASE, 1);//prednji levi levi senzor za detekciju valjka			//radi
	forwardMiddleSensor = GPIO_PinRegister(GPIOF_BASE, 0);//prednji srednji senzor za detekciju protivnika
	upperLiftSensor = GPIO_PinRegister(GPIOA_BASE, 7);//gornji senzor za detekciju pozicije lifta						//radi
	lowerLiftSensor = GPIO_PinRegister(GPIOA_BASE, 6);//donji senzor za detekciju pozicije lifta						//radi
	backwardLeftSensor = GPIO_PinRegister(GPIOA_BASE, 2);//zadnji senzor za detekciju protivnika						//radi
	backwardRightSensor = GPIO_PinRegister(GPIOA_BASE, 3);//zadnji senzor za detekciju protivnika						//radi
	backwardMiddleSensor = GPIO_PinRegister(GPIOF_BASE, 1);//zadnji srednji senzor za detekciju protivnika
	//jumper = GPIO_PinRegister(GPIOE_BASE, 2);//jumper koji sluzi za startovanje robota
	//sidesSwitch = GPIO_PinRegister(GPIOE_BASE, 3);//prekidac za menjanje strana
	
	//logger("Init done. Waiting for start jumper...\n\r");
	//while(jumperCheck() == 1);
	systemTime = 0;
	
	servo_init(100);
	
	clapperboardsClapped = 0;
	popcornColected = 0;
	standColected = 0;
	
	//logger("Match started.\n\r");
}

unsigned long getSystemTime(void)
{
	return systemTime;
}

/*void SPI_Init(void)
{
	DDRB |= ((1 << PB2) | (1 << PB1) | (1 << PB0));
	DDRB &= ~(1 << PB3);
	PORTB |= (1 << PB0);

	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
}

unsigned char SPI_ReadWrite(unsigned char data)
{
	PORTB &= ~(1 << PB0); // CS Low
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	PORTB |= (1 << PB0); // CS high

	return SPDR;
}*/

int checkLiftSensor(signed char sensor)
{
	if(sensor == UP)
	{
		if(upperLiftSensor == 1)
		{
			return SUCCESS;
		}
	}else if(sensor == DOWN)
	{
		if(lowerLiftSensor == 1)
		{
			return SUCCESS;
		}
	}
	
	return FAIL;
	
}//END OF checkLiftSensor

int checkFrontSensors(signed char sensor)
{
	if(sensor == RIGHT_SIDE)
	{
		if(forwardUpperRightSensor == 1)
		{
			
		}
		
	}else if(sensor == LEFT_SIDE)
	{
		if(forwardUpperLeftSensor)
		{
		}
		
	}else if(sensor == MIDDLE)
	{
		
	}else if(sensor == ALL)
	{
		
	}
}