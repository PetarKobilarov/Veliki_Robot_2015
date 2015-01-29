#include "system.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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

	_MMIO_BYTE(baseAddress - 1) &= ~(1 << pin);
	_MMIO_BYTE(baseAddress) |= (1 << pin);

	i = inputsNumber;
	inputsNumber++;

	return i;
}

unsigned char GPIO_PinRead(unsigned char pinHandler)
{
	return !( (gpios[pinHandler]->buffer[0]) | (gpios[pinHandler]->buffer[1]) | (gpios[pinHandler]->buffer[2]) );
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
}
