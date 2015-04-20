#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include "can.h"

#define RIGHT_SIDE	1
#define LEFT_SIDE	-1
#define BOTH		2

#define ACTIVATE	1
#define DEACTIVATE	-1

#define UP			1
#define DOWN		-1

#define SUCCESS		1
#define FAIL		-1

void knockDownTheClapperboards(signed char side, signed char state);
void colectThePopcorn(signed char side, signed char state);
void leftDiafram(signed char state);
void rightDiafram(signed char state);
void liftStates(signed char state);
signed char liftMove(signed char vertically, signed char side);

#endif