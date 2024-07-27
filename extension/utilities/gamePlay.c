#include "../gamePlay.h"
#include "../utils.h"
#include <stdlib.h>

void handlePlayerAce(Player *player) {
    char decision;
    if (player->tactics.anyAce) {
        remake_choice:
        printf("Use skill ANYACE? (make ace into any value) [Y]es / [N]o\n");
        printf("You can only use this skill once a day!\n");
        char choice = tolower(getchar());
        getchar(); // remove new line
        if (choice == 'y') {
            player->tactics.anyAce = false;
            new_ace:
            printf("select new value for ace\nMust be between 1 and 11\n");
            int new_ace = 0;
            scanf("%d", &new_ace);
            while (getchar() != '\n'); //clear input buffer
            if (new_ace > 11 || new_ace < 0) goto new_ace;
            player -> sum += new_ace;
            printf("Dealer: What!? Your ace appears to now hold a value of %d!\n", new_ace);
            sleep_seconds(1);
            printf("Dealer: Somehow your sum is now %d\n!", player->sum);
            return;
        }
        if (choice != 'n') {
            goto remake_choice;
        }
    }
    printf("Dealer: How much do you really value your Ace? [E]leven / [O]ne\n");
    do {
        decision = tolower(getchar()); // TODO change loop for error message
    } while (decision != 'e' && decision != 'o');
    getchar();
    if (decision == 'e') {
        printf("Dealer: In high regard, I see.\n");
        player->sum += 11;
    } else{
        printf("Dealer: Just another number you can count on one hand.\n");
        player->sum += 1;
    }
}

void handleDealerAce(Player *dealer) {
        if(dealer->tactics.anyAce) {
            printf("Dealer: How intriguing, an ace!\n");
            sleep_seconds(1);
            int newAce = 21 - dealer->sum;
            printf("Dealer: Now lets just adjust its value to %d.\n", newAce);
            sleep_seconds(2);
            printf("Dealer: I bet you can't do that!!!\n");
            dealer-> sum = 21;
            return;

        }
    if (computeValue(dealer->cards[dealer->index]) == 1) {
        if (dealer->sum + 11 <= 21) {
            printf("Dealer has chosen A as 11\n");
            dealer->sum +=11;
        } else {
            printf("Dealer has chosen A as 1\n");
            dealer->sum += 1;
        }
    }
}

void updatePlayerSum(Player *player, bool isDealer) {
    if (computeValue(player->cards[player->index]) == 1) {
        if (isDealer) handleDealerAce(player);
        else handlePlayerAce(player); }
    else {
        player->sum = player->sum + computeValue(player->cards[player->index]);
    }
}

void calculateDealerSum(Player *dealer) {
    int card1 = computeValue(dealer->cards[0]);
    int card2 = computeValue(dealer->cards[1]);
    if (card1 + card2 == 2) {
        dealer->sum = 12; //two Aces
    } else if ((card1 == 1 || card2 == 1) && card1 + card2  == 11) {
        dealer->sum = 21; //one Ace and one face card
    } else if (card1 == 1 || card2 == 1) {
        dealer->sum = card1 + card2 + (rand() % 2) * 10; //Ace and another card
    } else {
        dealer->sum = card1+ card2;
    }
}


void showInitialHand(Player* player, Player* dealer, int* deckIndex, int shuffledDeck[]) {
    printf("Dealer: Your Cards are\n");
    printCard(player->cards[0]);
    printCard(player->cards[1]);
    printf("\n");
    sleep_seconds(2);

    if (computeValue(player->cards[1]) == 10 && rand() % 2) {
        player->tactics.removeFace = true;
        printf("Dealer: A face card? Let's put that one back in the deck.\n");
        player->cards[1] = shuffledDeck[(*deckIndex)++];
        sleep_seconds(2);
        printf("Dealer: Your NEW Cards are\n");
        printCard(player->cards[0]);
        printCard(player->cards[1]);
        printf("\n");
        sleep_seconds(2);
    }

    if (computeValue(player->cards[0]) == 10 && rand() % 2) {
        player->tactics.removeFace = true;
        printf("Dealer: A face card? Let's put that one back in the deck.\n");
        player->cards[0] = shuffledDeck[(*deckIndex)++];
        sleep_seconds(2);
        printf("Dealer: Your NEW Cards are\n");
        printCard(player->cards[0]);
        printCard(player->cards[1]);
        printf("\n");
        sleep_seconds(2);
    }
    
    printf("Dealer: My Cards are\n");
    printCard(dealer->cards[0]);
    PRINT_CARD_RANDOM(PRINT_UNKNOWN_CARD, PRINT_UNKNOWN_CARD_MIRROR)
    sleep_seconds(3);
    printf("\n");
}

void updateInitialPlayerSum(Player* player, bool* blackjack) {
    // Initialize the sum of player's cards
    for (; player->index < 2; player->index++) updatePlayerSum(player, false);
    if (player->sum == 21) {
        printf("Dealer: Impressive, a blackjack.\n");
        *blackjack = true; // Indicates blackjack
    } else {
        printf("Dealer: The sum of your cards is %d\n", player->sum);
        sleep_seconds(1);
    }
}

void warnAboutHighSum(Player* player, Player* dealer) {
    char d;
    if (player->sum > 12 && rand() % 2 == 0 && !dealer->tactics.onlyHit) {
        printf("Dealer: Are you sure you want another card? I mean, you're already at %d [Y/N]\n", player->sum);
        d = tolower(getchar());
        while (d != 'y' && d != 'n') {
            printf("Dealer: Enter a valid choice please :)\n");
            d = tolower(getchar());
        }
        getchar(); // removing new line character
        printf("Dealer: I'll give you another card anyway, because I'm nice, you see :)\n");
    }
}

void dealNextCard(Player* player, Player* dealer, int* deckIndex, int shuffledDeck[]) {
    player->cards[player->index] = shuffledDeck[(*deckIndex)++];
    printf("Dealer: Your card no. %d is:\n", player->index + 1);
    if (!dealer->tactics.hideValue){
        printCard(player->cards[player->index]);
        sleep_seconds(2);
        updatePlayerSum(player, false);
        printf("Dealer: The sum of your cards is %d\n", player->sum);
    } else {
        PRINT_CARD_RANDOM(PRINT_UNKNOWN_CARD, PRINT_UNKNOWN_CARD_MIRROR)
        sleep_seconds(1);
        printf("Dealer: Let's spice it up and make it a secret eh ;) \n");
        updatePlayerSum(player, false);
    }
}

void displayDealerCards(Player* dealer) {
    printf("Dealer: Take a gander at my cards\n");
    printCard(dealer->cards[0]);
    printCard(dealer->cards[1]);
    sleep_seconds(3);
    calculateDealerSum(dealer);
    dealer->index = 2;
    printf("Dealer: The sum of my cards is %d\n", dealer->sum);
}

void dealerDrawsAdditionalCards(Player* dealer, int* deckIndex, int shuffledDeck[]) {
    for (; dealer->index < 11 && dealer->sum < 17; dealer->index++) {
        dealer->cards[dealer->index] = shuffledDeck[(*deckIndex)++];
        printf("Computer's card %d is:\n", dealer->index + 1);
        printCard(dealer->cards[dealer->index]);
        sleep_seconds(2);
        updatePlayerSum(dealer, true);
        printf("Dealer: The sum of my cards is %d\n", dealer->sum);
        sleep_seconds(1);
    }
}



void getPlayerConsent() {
    char choice[100];
    while (true) {
        fgets(choice, 100, stdin);
        if (tolower(choice[0]) == 's') {
            printf("Dealer: I like your enthusiasm, I might even let you win a round or two.\n");
            break;
        } else if (tolower(choice[0]) == 'n') {
            printf("Dealer: Well, you're playing this game anyway, so let's get started! :)\n");
            break;
        } else {
            printf("Dealer: Now, let's try to say something intelligible, shall we? ;) [S]ure / [N]ah\n");
        }
    }
}



bool anotherRound() {
    printf("Dealer: Another round? [Y/N]:\n");
    char again = getchar();
    getchar(); // to remove newline
    return tolower(again) == 'y';
}

void putCurrentPot(int pot_size){
    printf("Dealer: The current pot size is %d\n", pot_size);
}

void getValidBet(int bet, int * pot_size, int *player_money) {
wrong_bet:
if (bet > *player_money || bet < 1) {
    printf("Dealer: Cheeky! Unfortunately you will have to place a valid bet:\n");
    scanf("%d", &bet);
    goto wrong_bet;
}
    *player_money -= bet;
    *pot_size += bet;
    putCurrentPot(*pot_size);
}



void playerWins(int* player_money, int* dealer_money, int* pot_size) {
    *player_money += 2 * *pot_size;
    *dealer_money -= *pot_size;
    *pot_size = 0;
}

void dealerWins(Player dealer, int* player_money, int* dealer_money, int* pot_size) {
    if (dealer.tactics.pity) {
        sleep_seconds(1);
        printf("Dealer: I'm feeling generous today, I'll let you keep your money this one time.\n");
        *player_money += *pot_size;}
    else *dealer_money += *pot_size;
    *pot_size = 0;
}

void tie(int* player_money, int* pot_size) {
    *player_money += *pot_size;
    *pot_size = 0;
}



void makeDouble(int* bet, bool* x2, int* player_money, int* pot_size) {
    printf("Dealer: Would u like to [K]eep your bet or [D]ouble it?\n");
    char choice[100];
    fgets(choice, 100, stdin);
    if(tolower(choice[0]) == 'd') {
        if (*player_money < *bet) {
            printf("Dealer: Ha! You dont even have enough money to double!\n");
        } else {
            printf("Dealer: Cocky are we?\n");
            *x2 = true; // Bet is doubled
            getValidBet(*bet, pot_size, player_money);
        }
    } else if (tolower(choice[0]) != 'k') {
        printf("Dealer: Well thats not a valid option now is it!\n");
        makeDouble(bet, x2, player_money, pot_size); 
    }
}





bool checkBust(Player player, Player dealer, bool x2, int* player_money, int* dealer_money, int* pot_size) {
    if (player.sum > 21) {
        if (x2) {
            printf("Dealer: Ha! Too cocky, just like I thought!\n");
        } else {
            printf("Dealer: It really is this easy huh?\nDealer: You lose. :(\n");
        }
        dealerWins(dealer, player_money, dealer_money, pot_size);
        return true; 
    }
    return false;
}

void performFinalChecks(Player player, Player dealer, bool blackjack, bool x2, int* player_money, int* dealer_money, int* pot_size) {
    if ((dealer.sum > 21 || player.sum > dealer.sum) && !dealer.tactics.over21) {
        printf("Dealer: You win.\n");
        if (blackjack) {
            *pot_size *=  5 / 4;
        }
        playerWins(player_money, dealer_money, pot_size);
        sleep_seconds(2);
        return;
    }
    if (player.sum == dealer.sum) {
        printf("Dealer: We tied? You are one lucky man.\n");
        tie(player_money, pot_size);
        sleep_seconds(2);
        return;
    }
    if (player.sum < dealer.sum) {
        if(x2) {
            printf("Dealer: Too cocky, just like I thought!\n");
        }
        if(dealer.sum > 21 && dealer.tactics.over21) {
            printf("Dealer: This limit of 21 seems a little too constrained for me\n");
            sleep_seconds(1);
            printf("Dealer: Do you not think so too? I'm sure you do ;)\n");
        }
        printf("Dealer: I win!\n");
        dealerWins(dealer, player_money, dealer_money, pot_size);
        sleep_seconds(2);
    }
}

void playerDrawsCards(Player* player, Player* dealer, int* deckIndex, int shuffledDeck[], bool x2, int* player_money, int* dealer_money, int* pot_size) {
    for (; player->index < 11; player->index++) {
        if(x2) goto next_card;
        if (player->tactics.insight) {
            remake_choice:
            printf("Use skill INSIGHT? (look at next card) [Y]es / [N]o\n");
            printf("You can only use this skill once a day!\n");
            char choice = tolower(getchar());
            getchar(); // remove new line
            if (choice == 'y') {
                player->tactics.insight = false;
                sleep_seconds(1);
                printf("Next card:\n");
                printCard(shuffledDeck[*deckIndex]);
            } else if (choice != 'n') {
                goto remake_choice;
            }
        }
        char j;
        printf("Dealer: Fancy another card? [Y/N]\n");
        do {
            j = tolower(getchar());
            getchar();
        } while (j != 'y' && j != 'n');

        if (dealer->tactics.onlyHit) {
            sleep_seconds(1);
            if (j == 'n') printf("Dealer: You know what, let's take a hit, shall we :)");
            j = 'y';
        }

        if (dealer->tactics.onlyStall) {
            sleep_seconds(1);
            if (j == 'y') printf("Dealer: Calm your horses a little, I don't think you need another card\n");
            j = 'n';
        }

        if (j == 'y') {
            warnAboutHighSum(player, dealer);
            next_card:
            dealNextCard(player,dealer, deckIndex, shuffledDeck);
            if (dealer->tactics.hitTwoCards && player->sum < 22) {
                printf("Dealer: For some reason, I think I heard you say you wanted to hit twice\n");
                sleep_seconds(1);
                printf("Dealer: alright then, here's another card :)\n");
                dealNextCard(player, dealer, deckIndex, shuffledDeck);
            }
            if (x2) {
                if(!checkBust(*player,*dealer, x2, player_money, dealer_money, pot_size)) {
                    printf("Dealer: Well now its my turn.\n");
                }
                break;
            }
            if (checkBust(*player,*dealer,x2, player_money, dealer_money, pot_size)) return;
        } else {
            printf("Dealer: I thought you were more of a risk taker.\n");
            sleep_seconds(1);
            printf("Dealer: How disappointing.\n");
            sleep_seconds(1);
            printf("Dealer: The sum of your cards is %d\n", player->sum);
            sleep_seconds(2);
            break;
        }
    }
}
