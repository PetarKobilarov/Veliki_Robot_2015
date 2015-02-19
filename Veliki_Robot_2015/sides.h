typedef enum
{
	TACTIC_ONE = 1,
	TACTIC_TWO	
}robotStates;

unsigned char forwardLeftSensor, backwardLeftSensor, forwardRightSensor, backwardRightSensor;

void yellowSide(void);
void greenSide(void);