#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <random>
#include <time.h>
#include <algorithm>

using std::vector;

/*
FRONT of $cards vector is TOP of deck.
That is, the card popped from the front of the list is the card
pulled from the top of the deck.

BACK of $cards vector is BOTTOM of deck.
That is, cards placed at the bottom of the deck are placed in
the back of the $cards vector

*/



struct Deck{
    Deck();

    void Shuffle();

    vector<Card> cards;

};



#endif