#ifndef SHOP_H
#define SHOP_H

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printShop(Player player, int day) ;
void makeChoice(char c, int * player_money, bool * deadDealer, Player* player );
bool shop(int day, Player *player, int * player_money);
#endif // !SHOP_H



