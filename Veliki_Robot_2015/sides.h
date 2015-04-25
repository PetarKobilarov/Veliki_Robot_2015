#define TACTIC_ONE_POSITION_COUNT	11

typedef enum
{
	TACTIC_ONE = 1,
	TACTIC_TWO,
	COLLISION
}robotStates;

typedef struct
{
	position point;
	unsigned char speed;
	signed char direction;
	char (*detectionCallback)(unsigned long startTime);
}gotoFields;

unsigned char numOfStandsRight, numOfStandsLeft;
position positinDetected, compare;

void yellowSide(void);
void greenSide(void);
