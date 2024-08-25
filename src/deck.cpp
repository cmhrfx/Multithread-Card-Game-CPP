#include "../headers/deck.h"

Deck::Deck()
{
    char vals[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
    string suits[] = {"Spades", "Clubs", "Diamonds", "Hearts"};

    // instantiate each card, push them to an unshuffled deck
    // also populate map of key-value pairs
    // 0 - ace of spades, 1 - ace of clubs, 2 - ace of diamonds, 3 - ace of hearts
    // and so on in that order. 
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Card card(vals[i], suits[j]);
            cards.push_back(card);
        }
    }
}

// adapted from https://www.geeksforgeeks.org/shuffle-a-deck-of-cards-3/
void Deck::Shuffle()
{
    srand(time(0));

    for (int i = 0; i < 52; i++)
    {
        int r = i + (rand() % 52 - i);
        std::swap(cards[i], cards[r]);
    }
}
