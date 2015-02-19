#define TACTIC_ONE_POSITION_COUNT	1

typedef enum
{
	TACTIC_ONE = 1,
	TACTIC_TWO
}robotStates;

typedef struct
{
	position point;
	unsigned char speed;
	signed char direction;
	char (*detectionCallback)(void);
}gotoFields;

unsigned char forwardLeftSensor, backwardLeftSensor, forwardRightSensor, backwardRightSensor;

void yellowSide(void);
void greenSide(void);