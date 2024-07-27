#include "../utils.h"
#include <stdlib.h>
#include <time.h>

void init_tactics(Tactics *t) {
    t->over21 = false; //done 
    t->removeFace = false; //done
    t->hitTwoCards = false; //done
    t->hideValue = false; //done
    t->anyAce = false; //done
    t->onlyHit = false; //done
    t->onlyStall = false; //done
    t->pity = false; //done
    t->insight = false; //done
    t->surrender = false; //done
    t->addDay = false; //done
}

void init_Player(Player *player, bool isPl) {
    player->sum = 0;
    for (int i = 0; i < 11; i++) {
        player->cards[i] = 0;
    }
    player->index = 0;
    player->balance = STARTING_MONEY;
    if (isPl) return;
    init_tactics(&player->tactics);
}

void randomizePlayerTactic(Player *player) {
    srand(time(NULL)); 
    int tactic = rand() % 10; 
    switch (tactic) {
        case 0:  player->tactics.over21 = true; break;
        case 1:  player->tactics.hitTwoCards = true; break;
        case 2:  player->tactics.hideValue = true; break;
        case 3:  player->tactics.anyAce = true; break;
        case 4:  player->tactics.skipDay = true; break;
        case 5:  player->tactics.onlyHit = true; break;
        case 6:  player->tactics.onlyStall = true; break;
        case 7:  player->tactics.pity = true; break;
        default: break;
    }
}
int computeValue(int a) {
    return ((a % 100 == 11) || (a % 100 == 12) || (a % 100 == 13)) ? 10 : a % 100;
}

void printSuite(int suite) {
    switch (suite) {
        case 6: // Spades
            printf("|  \u2660  |\n");
            break;
        case 3: // Hearts
            printf("|  \u2665  |\n");
            break;
        case 4: // Diamonds
            printf("|  \u2666  |\n");
            break;
        case 5: // Clubs
            printf("|  \u2663  |\n");
            break;
        default:
            printf("?\n");
    }
}

void printCard(int num) {
    char suite = num / 100;
    int value = num % 100;

    switch (value) {
        case 1: {
            PRINT_CARD_RANDOM(PRINT_ACE(suite), PRINT_ACE_MIRROR(suite))
            break;
        }
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9: {
            PRINT_CARD_RANDOM(PRINT_NUMBER(suite, value), PRINT_NUMBER_MIRROR(suite, value))
            break;
        }
        case 10: {
            PRINT_CARD_RANDOM(PRINT_TEN(suite, value), PRINT_TEN_MIRROR(suite, value))
            break;
        }
        case 11: {
            PRINT_CARD_RANDOM(PRINT_JACK(suite), PRINT_JACK_MIRROR(suite))
            break;
        }
        case 12: {
            PRINT_CARD_RANDOM(PRINT_QUEEN(suite), PRINT_QUEEN_MIRROR(suite))
            break;
        }
        case 13: {
            PRINT_CARD_RANDOM(PRINT_KING(suite), PRINT_KING_MIRROR(suite))
            break;
        }

    }
}