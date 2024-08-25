#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"

struct Player{
    Player();
    Player(int number, bool dealer, bool myAction, bool hasDealt, bool victory);

    bool dealer;
    bool hasDealt;
    bool myAction;
    bool victory;
    bool calledForReset;
    int number;
    vector<Card> hand;

    Card DrawCard(Deck* deck);
    void Discard(Deck* deck);
    void Deal(Deck* deck);
};



#endif