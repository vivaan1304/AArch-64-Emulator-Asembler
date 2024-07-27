#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "gamePlay.h"
#include "defs.h"
#include "shop.h"
#include "utils.h"

int deck[52];
int pot_size = 0;
int player_money = STARTING_MONEY , dealer_money = 10000;
int day = 1;
int rounds = 1;
Player player;
Player dealer;


void init_deck() {
    for (int i = 0; i < 52; i++)
        deck[i] = (i / 13 + 3) * 100 + i % 13 + 1; 
        //starts from 301 (Ace of Hearts) to 613 (King of Spades)
}

void shuffle(int shuffledDeck[]) {
    init_deck();
    int t;
    srand(time(NULL)); // to generate random seed
    for (int i = 0; i < 52; i++) {
        do {
            t = rand() % 52;
        } while (deck[t] == 0);
        shuffledDeck[i] = deck[t];
        deck[t] = 0;
    }

}
void initGame(int* bet, Player* player, Player* dealer, int* deckIndex, int shuffledDeck[]) {
    printf("Dealer: Round %d.\n", rounds);
    printf("Dealer: Enter your initial bet:\n");
    scanf("%d", bet);
    getchar(); // to remove new-line character
    getValidBet(*bet, &pot_size, &player_money);

    init_Player(player, true);
    init_Player(dealer, false);
    shuffle(shuffledDeck);

    player->cards[0] = shuffledDeck[(*deckIndex)++];
    player->cards[1] = shuffledDeck[(*deckIndex)++];
    dealer->cards[0] = shuffledDeck[(*deckIndex)++];
    dealer->cards[1] = shuffledDeck[(*deckIndex)++];
}





void greetPlayer() {
    printf("Dealer: Welcome to (Un)fair Casino!\n");
    sleep_seconds(1);
    printf("Dealer: I will be your dealer and I *promise* you'll leave with a fortune ;)\n");
    sleep_seconds(1);
    printf("Dealer: Seeing as you have already deposited $%d,\n"
           "Dealer: let's play a game of BlackJack, shall we?\n[S]ure / [N]ah\n", player_money);
}

void surrender() {
    printf("Dealer: What?! You want to quit?!\n");
    sleep_seconds(2);
    printf("Dealer: I knew you didn't have what it takes to beat me!\n");
    sleep_seconds(2);
    printf("Dealer: Here take this for the bus ride back home\n");
    sleep_seconds(2);
    player_money += pot_size / 2;
    dealer_money += pot_size/2;
    pot_size = 0;
    printf("Dealer: Your current balance is %d\n", player_money);
    sleep_seconds(1);
    printf("Casino Bank: %d\n", dealer_money);
    sleep_seconds(1);
    printf("Dealer: Unless of course you want to try your luck again!\n");
    sleep_seconds(2);
}
void play() {
    int bet = 0;
    bool blackjack = false, x2 = false;
    int deckIndex = 0,shuffledDeck[52];

    initGame(&bet, &player, &dealer, &deckIndex, shuffledDeck);    
    showInitialHand(&player, &dealer, &deckIndex, shuffledDeck);
    putCurrentPot(pot_size);
    
    updateInitialPlayerSum(&player, &blackjack);
    if (player.tactics.surrender) {
        remake_choice:
        printf("Use skill SURRENDER? [Y]es / [N]o\n");
        printf("You can only use this skill once a day!\n");
        char choice = tolower(getchar());
        getchar(); // remove new line
        if (choice == 'y') {
            player.tactics.surrender = false;
            sleep_seconds(1);
            goto surrender;
        } else if (choice != 'n') {
            goto remake_choice;
        }
    }

    if (blackjack) goto dealers_turn;
    makeDouble(&bet, &x2, &player_money, &pot_size);
    playerDrawsCards(&player, &dealer, &deckIndex, shuffledDeck, x2, &player_money, &dealer_money, &pot_size);
    if (player.sum > 21) return;
    putCurrentPot(pot_size);

    dealers_turn:
    displayDealerCards(&dealer);
    dealerDrawsAdditionalCards(&dealer, &deckIndex, shuffledDeck);
    performFinalChecks(player, dealer, blackjack,x2, &player_money, &dealer_money, &pot_size);
    return;
    surrender:
    surrender();
}

void playGame() {
    play();
    printf("Dealer: Your current balance is %d\n", player_money);
    printf("Casino Bank: %d\n", dealer_money);
    sleep_seconds(1);
    if (player_money < 1) {
        printf("Dealer: Ha! Looks like you've ran out of money!\n");
        sleep_seconds(2);
        printf("Dealer: Well at least you tried to beat the casino :)\n");
        sleep_seconds(2);
    }
}

void playRounds() {
    printf("Dealer: Another round? [Y/N]:\n");
    char again = getchar();
    getchar(); // to remove newline
    while (tolower(again) != 'n') {
        if (tolower(again) == 'y') {
            if (player_money < 200) {
                printf("Dealer: Alright, but your wallet is looking kind of thin!\n");
            } else {
                printf("Dealer: Okay Mr Confident, lets go again.\n");
            }
            sleep_seconds(2);
            play();
            rounds ++;
            if (rounds > MAX_ROUNDS) {
                printf("Dealer: Looks like I'm getting a call from the manager!\n");
                sleep_seconds(1);
                printf("Dealer: Its probably to congratulate me on my excellent performance.\n"
                       "Dealer: You'll have to leave now.\n");
                sleep_seconds(2);
                return;
            }
            printf("Dealer: Your current balance is: %d\n", player_money);
            printf("Dealer: Casino Bank: %d\n", dealer_money);
            if (player_money < 1) {
                printf("Dealer: Ha! Looks like you've ran out of money!\n");
                break;
            }
            if (dealer_money < 1) {
                printf("Dealer: What!? How!? Take your money and leave! Now!\n");
                return;
            }
            printf("Dealer: Another round? [Y/N]:\n");
        } else {
            printf("Dealer: If you enter a valid choice, I promise I'll boost your odds!\n");
        }
        again = getchar();
        getchar(); // to remove newline
    }
    printf("Dealer: Well at least you tried to beat the casino. :)\n");
}


int main(void) {
    bool deadDealer = false;
    init_tactics(&player.tactics);
    printf("Today is the day.\n");
    sleep_seconds(2);
    printf("Today is the day we take them down.\n");
    sleep_seconds(2);
    printf("Exit home.\n");
    Tactics curtacs;
    new_day:
    if (day != 1) {
        deadDealer = shop(day, &player, &player_money);
         curtacs = player.tactics;

    }
    if (deadDealer) goto GAME_OVER;
    greetPlayer();
    getPlayerConsent();
    playGame();
    rounds = 2;
    if (player_money < 1)  goto GAME_OVER;
    playRounds();
    if (player_money < 1)  goto GAME_OVER;
    if ((player.tactics.addDay && day > MAX_DAYS + 1 )|| (!player.tactics.addDay &&day > MAX_DAYS)) {
        printf("Manager: Your time here is up! Nice try.\n");
    } else {
        if(day != 1) {
            player.tactics = curtacs;
        }
        day++;
        sleep_seconds(1);
        printf("Exit Casino.\n");
        sleep_seconds(1);
        printf("Time to relax.\n");
        sleep_seconds(1);
        printf("Enter Home.\n");
        sleep_seconds(2);
        printf("*Sunrise*\n");
        sleep_seconds(2);
        printf("Day %d\n", day);
        sleep_seconds(1);
        int days_left = player.tactics.addDay ? MAX_DAYS - day + 1: MAX_DAYS - day;
        if (day > 2) printf("Day(s) left: %d\n", days_left);
        printf("Here we go!\n");
        sleep_seconds(1);
        printf("Exit Home.\n");
        sleep_seconds(1);
        goto new_day;
    }
    GAME_OVER:
    if (deadDealer) {
        printf("Enter Home.\n");
        sleep_seconds(3);
        printf("Dealer: You should have listened to the shop keeper...\n");
        sleep_seconds(3);
    }
    printf("GAME OVER\n");
    sleep_seconds(3);
    return 0;
}