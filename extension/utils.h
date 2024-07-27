
#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

void init_tactics(Tactics *t);

void init_Player(Player *player, bool isPl);

void randomizePlayerTactic(Player *player);

int computeValue(int a);

void printSuite(int suite);

void printCard(int num);

#endif 