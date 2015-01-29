#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#define HARD_STOP	0
#define SOFT_STOP	1

#define ODOMETRY_SUCCESS	0
#define ODOMETRY_FAIL		1
#define ODOMETRY_STUCK		2

typedef struct
{
	signed int x;
	signed int y;	
	signed int angle;
}position;

typedef enum
{
	IDLE = 'I',
	MOVING = 'M',
	ROTATING = 'R',
	STUCK = 'S',
	ERROR = 'E'
}states;

char stop(char type);
char moveOnDirection(int distance, unsigned char speed, void (*callback)(void));
char gotoXY(position coordinates, unsigned char speed, signed char direction, void (*callback)(void));
char setPosition(position coordinates);
char rotate(int angle,unsigned char speed, void (*callback)(void));
char setAngle(int angle, unsigned char speed, void (*callback)(void));
char getState(void);
position getPosition(void);



#endif /* ODOMETRY_H_ */