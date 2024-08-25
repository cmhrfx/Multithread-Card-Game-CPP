#include "../headers/game.h"

Card Game::target;
Deck Game::deck;
std::vector<Player*> Game::players;
int Game::round = 1;
int Game::dealer = 0;
int Game::playerTurn = 1;
int Game::roundResetCount = 0;
bool Game::roundDealt = false;
bool Game::gameOver = false;
bool Game::roundOver = false;

void Game::roundReset()
{
    if (Game::roundResetCount == 6)
    {
        players[dealer]->dealer = false;
        dealer++;
        dealer = dealer%6; 
        round++;
        roundResetCount = 0;
        roundOver = false;
        roundDealt = false;

        for (int i = 0; i < 6; i++)
        {
            players[i]->victory = false;
            players[i]->hasDealt = false;
            players[i]->myAction = false;
            players[i]->calledForReset = false;
        }

        players[dealer]->myAction = true;
        players[dealer]->dealer = true;

        cout << "post round deck length: " << deck.cards.size() << endl;
    }
}