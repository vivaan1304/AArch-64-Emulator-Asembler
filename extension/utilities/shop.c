#include "../shop.h"


void printShop(Player player, int day) {
    if (!player.tactics.insight) {
        printf("Shopkeeper: [I]NSIGHT $1000\n");
        sleep_seconds(2);
        printf("Shopkeeper: This skill allows you to just take a small peak at the\n"
               "Shopkeeper: next card. Quite useful for when you're in a sticky situation!\n");
        sleep_seconds(3);
    }
    if (!player.tactics.surrender) {
        printf("Shopkeeper: [S]URRENDER $500\n");
        sleep_seconds(2);
        printf("Shopkeeper: This skill can be very useful!\n"
               "Shopkeeper: If you reckon you're in a losing situation simply surrender\n"
               "Shopkeeper: the hand and get half your original bet back!\n");
        sleep_seconds(3);
    }
    if (!player.tactics.anyAce) {
        printf("Shopkeeper: [A]NYACE $2500\n");
        sleep_seconds(2);
        printf("Shopkeeper: This one might be my favourite!\n"
               "Shopkeeper: Basically turn your ace into a joker allowing\n"
               "Shopkeeper: it to be any card, with any value!\n");
        sleep_seconds(3);
    }
    if(!player.tactics.addDay) {
        printf("Shopkeeper: ADD [D]AY $500\n");
        sleep_seconds(2);
        if (day == 2) {
            printf("Shopkeeper: So heres the deal.\n"
                   "Shopkeeper: Word on the street is the manager of the casino is onto you\n");
            sleep_seconds(3);
            printf("Shopkeeper: and is tracking you down.\nShopkeeper: It sounds like he will find you\n"
            "Shopkeeper: in %d day(s).\nShopkeeper: I can create some confusion to increase your days by one.\n", MAX_DAYS-day);
            sleep_seconds(3);
        } else {
            printf("Shopkeeper: I increase your deadline of %d day(s) by one.\n", MAX_DAYS-day);
            sleep_seconds(2);
            printf("Shopkeeper: I have told you about the deadline right!?\n");
            sleep_seconds(2);
        }
    }
    printf("Shopkeeper: [E]LIMINATE DEALER $8000\n");
    sleep_seconds(2);
    printf("Shopkeeper: I can get a contact of mine to handle the situation. Quietly.\n");
    sleep_seconds(2);
    printf("Shopkeeper: However, I advise against this option,\nShopkeeper: there's more humane ways to take the casino down.\n");
    sleep_seconds(2);
}

void makeChoice(char c, int * player_money, bool * deadDealer, Player* player ) {
    if (c != 'e') printf("Shopkeeper: Great choice!!!\n");
    sleep_seconds(2);
    switch (c) {
        case 'a' : {
            *player_money -= 2500;
            player->tactics.anyAce = true;
            printf("*SKILL ANYACE LEARNT*\n");
        } break;

        case 'i' : {
            *player_money -= 1000;
            player->tactics.insight = true;
            printf("*SKILL INSIGHT LEARNT*\n");
        } break;

        case 'd' : {
            *player_money -= 500;
            player->tactics.addDay = true;

            printf("*SKILL ADD DAY LEARNT*\n");
        } break;

        case 's' : {
            *player_money -= 500;
            player->tactics.surrender = true;
            printf("*SKILL SURRENDER LEARNT*\n");
        } break;

        case 'e' : {
            *player_money -= 8000;
            printf("Shopkeeper: I see... I'll see to it that the deed is done\n");
            *deadDealer = true;
            printf("*DEALER ELIMINATED*\n");
            printf("Shopkeeper: You must leave. Now.\n");
        } break;

        default: printf("Wrong choice\n"); exit(EXIT_FAILURE);

    }
    sleep_seconds(1);
}

bool shop(int day, Player *player, int* player_money) {
    bool elimDealer = false;
    if (day == 2) {
        printf("Shopkeeper: Welcome to my shop!\n");
        sleep_seconds(1);
        printf("Shopkeeper: Let me let you in on a little secret.\nShopkeeper: You know the world famous Fair Casino?\n");
        sleep_seconds(3);
        printf("Shopkeeper: They're cheating!\n"
               "Shopkeeper: I know they are!\n"
               "Shopkeeper: I tried to stop them but I lost all my money in the process!\n");
        sleep_seconds(3);
        printf("Shopkeeper: Luckily for you, I can teach you all I know about their tricks,\n"
               "Shopkeeper: and perhaps, you might be able to use them against them!\n");
        sleep_seconds(3);
    } else {
        printf("Shopkeeper: Welcome back my friend!\n");
    }
    printf("Shopkeeper: Now what skill should I teach you, at a price of course!\n");
    printShop(*player, day);
    if (elimDealer) printf("dealer dead\n");
    printf("Shopkeeper: So what will it be?\n");
    char choice = 'f';
     while (choice != 'l' || choice != 'e') {
        reselect:
        printf("Player money: %d\n", * player_money);
        printf("Shopkeeper: The choices available are\n");
        if (!player->tactics.anyAce) printf("[A]NYACE $2500\n");
        if (!player->tactics.addDay) printf("ADD [D]AY $500\n");
        if (!player->tactics.insight) printf("[I]NSIGHT $1000\n");
        if (!player->tactics.surrender) printf("[S]URRENDER $500\n");
        printf("[E]LIMINATE DEALER $8000\n");
        printf("[L]eave\n");

        choice = tolower(getchar());
        getchar(); // Clear newline in buffer

        if (strchr("asield", choice) == NULL) {
            printf("Shopkeeper: Invalid choice. Please select a valid option.\n");
            goto reselect;
        }

        if (choice == 'l') {
            printf("Shopkeeper: May the odds be ever in your favour!\n");
            break;
        }

        if (*player_money < 501
            || (choice == 'a' && *player_money < 2501)
            || (choice == 'd' && *player_money < 501)
            || (choice == 'i' && *player_money < 1001)
            || (choice == 'e' && *player_money < 8001)) {
            printf("Shopkeeper: I'm sorry, it seems that your card declined.\n");
            printf("Shopkeeper: Remember, you still need to have enough to play again.\n");
            goto reselect;
        }
        makeChoice(choice, player_money, &elimDealer, player);
    }
sleep_seconds(2);
printf("Exit Shop.\n");
return elimDealer;
}