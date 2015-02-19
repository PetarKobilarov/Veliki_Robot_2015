#ifndef COMMON_H_
#define COMMON_H_

#include "odometry.h"

#define TACTIC_ONE_POSITION_COUNT		2

typedef struct 
{
	position point;
	unsigned char speed;
	signed char direction;
	char (*detectionCallback)(void);
}gotoCrap;



#endif 