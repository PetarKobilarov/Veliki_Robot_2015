#define TACTIC_ONE_POSITION_COUNT	13

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

void yellowSide(void);
void greenSide(void);
