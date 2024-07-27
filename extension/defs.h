#ifndef DEFS_H
#define DEFS_H


#include <stdbool.h>
#include <stdio.h>
#define heart 03
#define diamond 04
#define club 05
#define spade 06
#define STARTING_MONEY 1000
#define MAX_ROUNDS 5
#define MAX_DAYS 5

// to include sleep function
void sleep_seconds(int seconds);

//Used to print the cards
#define PRINT_ACE(suite) printf("-------\n"); \
    printf("|A    |\n"); \
    printSuite(suite); \
    printf("|    A|\n"); \
    printf("-------\n");

#define PRINT_NUMBER(suite, value) printf("-------\n"); \
    printf("|%d    |\n", value); \
    printSuite(suite); \
    printf("|    %d|\n", value); \
    printf("-------\n");

#define PRINT_JACK(suite) printf("-------\n"); \
    printf("|J    |\n"); \
    printSuite(suite);\
    printf("|    J|\n"); \
    printf("-------\n");

#define PRINT_QUEEN(suite) printf("-------\n"); \
    printf("|Q    |\n"); \
    printSuite(suite); \
    printf("|    Q|\n"); \
    printf("-------\n");

#define PRINT_KING(suite) printf("-------\n"); \
    printf("|K    |\n"); \
    printSuite(suite);\
    printf("|    K|\n"); \
    printf("-------\n");

#define PRINT_TEN(suite, value) printf("-------\n"); \
    printf("|%d   |\n", value); \
    printSuite(suite); \
    printf("|   %d|\n", value); \
    printf("-------\n");

#define PRINT_TEN_MIRROR(suite, value) printf("-------\n"); \
    printf("|   %d|\n", value); \
    printSuite(suite); \
    printf("|%d   |\n", value); \
    printf("-------\n");


#define PRINT_UNKNOWN_CARD printf("-------\n"); \
    printf("|?    |\n"); \
    printf("|  ?  |\n"); \
    printf("|    ?|\n"); \
    printf("-------\n");

#define PRINT_ACE_MIRROR(suite) printf("-------\n"); \
    printf("|    A|\n"); \
    printSuite(suite); \
    printf("|A    |\n"); \
    printf("-------\n");

#define PRINT_NUMBER_MIRROR(suite, value) printf("-------\n"); \
    printf("|    %d|\n", value); \
    printSuite(suite); \
    printf("|%d    |\n", value); \
    printf("-------\n");

#define PRINT_JACK_MIRROR(suite) printf("-------\n"); \
    printf("|    J|\n"); \
    printSuite(suite);\
    printf("|J    |\n"); \
    printf("-------\n");

#define PRINT_QUEEN_MIRROR(suite) printf("-------\n"); \
    printf("|    Q|\n"); \
    printSuite(suite); \
    printf("|Q    |\n"); \
    printf("-------\n");

#define PRINT_KING_MIRROR(suite) printf("-------\n"); \
    printf("|    K|\n"); \
    printSuite(suite); \
    printf("|K    |\n"); \
    printf("-------\n");

#define PRINT_UNKNOWN_CARD_MIRROR printf("-------\n"); \
    printf("|    ?|\n"); \
    printf("|  ?  |\n"); \
    printf("|?    |\n"); \
    printf("-------\n");

#define PRINT_CARD_RANDOM(func, mirror) \
    if (rand() % 2) { \
        func\
    } else { \
        mirror\
    }

typedef struct {
    bool over21;        // can cross 21
    bool removeFace;    // remove face cards
    bool hitTwoCards;   // give 2 cards instead of 1
    bool hideValue;     // hide value of card
    bool anyAce;        // use Ace as any value
    bool skipDay;       // skip a day
    bool addDay;
    bool onlyHit;       // both options leads to a hit
    bool onlyStall;     // both options leads to a stall
    bool pity;          // doesn't take away the money even when they lose
    bool insight;       // shows the next card in advance
    bool surrender;     // allows surrendering a hand to reclaim half the bet
} Tactics;

typedef struct {
    int sum;
    int cards[11];
    int index;
    int balance;
    Tactics tactics;
} Player;

#endif