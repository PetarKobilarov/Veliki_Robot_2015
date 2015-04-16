#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include "can.h"

#define RIGHT_SIDE	1
#define LEFT_SIDE	-1

#define ACTIVATE	1
#define DEACTIVATE	-1

void knockDownTheClapperboards(unsigned char side);
void colectThePopcorn(unsigned char side);
void leftDiafram(unsigned char state);
void rightDiafram(unsigned char state);
void liftMove(unsigned char state);

#endif