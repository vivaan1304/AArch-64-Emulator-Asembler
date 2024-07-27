#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "defs.h"

void handlePlayerAce(Player *player);

void handleDealerAce(Player *dealer);

void updatePlayerSum(Player *player, bool isDealer);

void calculateDealerSum(Player *dealer);

void showInitialHand(Player* player, Player* dealer, int* deckIndex, int shuffledDeck[]);

void updateInitialPlayerSum(Player* player, bool* blackjack);

void warnAboutHighSum(Player* player, Player* dealer);

void dealNextCard(Player* player, Player* dealer, int* deckIndex, int shuffledDeck[]) ;

void displayDealerCards(Player* dealer);

void dealerDrawsAdditionalCards(Player* dealer, int* deckIndex, int shuffledDeck[]);

void getPlayerConsent();

bool anotherRound();

void putCurrentPot(int pot_size);

void getValidBet(int bet, int * pot_size, int *player_money);

void playerWins(int* player_money, int* dealer_money, int* pot_size);

void dealerWins(Player dealer, int* player_money, int* dealer_money, int* pot_size);

void tie(int* player_money, int* pot_size);

void makeDouble(int* bet, bool* x2, int* player_money, int* pot_size);

bool checkBust(Player player, Player dealer, bool x2, int* player_money, int* dealer_money, int* pot_size);

void performFinalChecks(Player player, Player dealer, bool blackjack, bool x2, int* player_money, int* dealer_money, int* pot_size);

void playerDrawsCards(Player* player, Player* dealer, int* deckIndex, int shuffledDeck[], bool x2, int* player_money, int* dealer_money, int* pot_size);
#endif