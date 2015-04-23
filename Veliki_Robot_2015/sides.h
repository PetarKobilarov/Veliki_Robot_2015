#define TACTIC_ONE_POSITION_COUNT	11

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
	char (*detectionCallback)(unsigned long startTime);
}gotoFields;

unsigned char currentPosition, nextPosition;

void yellowSide(void);
void greenSide(void);
