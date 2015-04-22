#ifndef SYSTEM_H_
#define SYSTEM_H_

#define MAX_INPUTS	    20
#define USE_TIMER_HOOK  0

#define GPIOA_BASE 0x22
#define GPIOB_BASE 0x25
#define GPIOC_BASE 0x28
#define GPIOD_BASE 0x2B
#define GPIOE_BASE 0x2E
#define GPIOG_BASE 0x34
#define GPIOF_BASE 0x31

unsigned char GPIO_PinRegister(volatile unsigned char *baseAddress, unsigned char pin);
unsigned char GPIO_PinRead(unsigned char pinHandler);
unsigned char GPIO_ReadFromRegister(unsigned char pinHandler);
void fillDebaunsingData(void);

void Timer_Init(unsigned int freq);
//void SPI_Init(void);
//unsigned char SPI_ReadWrite(unsigned char data);

unsigned char forwardUpperLeftSensor, backwardLeftSensor, forwardUpperRightSensor, backwardRightSensor, forwardLowerLeftSensor, forwardLowerRightSensor, upperLiftSensor, forwardMiddleSensor, backwardMiddleSensor, lowerLiftSensor, jumper, sidesSwitch;
unsigned char clapperboardsClapped, popcornColected, standColected;

unsigned long getSystemTime(void);
void systemInit(void);
int jumperCheck(void);
int sidesSwitchCheck(void);
int checkLiftSensor(signed char sensor);
int checkFrontSensors(signed char sensor);
int checkRearSensors(signed char sensor);


//void logger(const char *text, ...);

#endif
