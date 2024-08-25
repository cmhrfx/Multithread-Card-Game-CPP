#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "deck.h"
#include "player.h"

struct Game{

    static Card target;
    static Deck deck;
    static int round;
    static int dealer;
    static std::vector<Player*> players;
    static int playerTurn;
    static bool gameOver;
    static bool roundOver;
    static bool roundDealt;
    static int roundResetCount;

    static void roundReset();

};

#endif