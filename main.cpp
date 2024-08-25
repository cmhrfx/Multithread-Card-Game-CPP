#include "main.h"

pthread_mutex_t mutex1;
pthread_cond_t cond;

void* run(void* player);
void drawTarget(Player* player);
void endTurn(int myIndex);
void deal(Player* player);
void draw(Player* player);
void checkHand(Player* player);
void discard(Player* player);
void discardAll(Player* player);
void roundOver(Player* player);
void showDeck();


int main()
{
    // create a list of players.
    for (int i = 0; i < 6; i++)
    {
        Player* newplayer = new Player(i+1, false, false, false, false);
        if (i == 0)
        {
            newplayer->dealer = true;
            newplayer->myAction = true;
        }
        Game::players.push_back(newplayer);
    }

    // create necessary pthread objects
    vector<pthread_t> threads(6);

    // mutex
    pthread_mutex_init(&mutex1, NULL);
    pthread_cond_init(&cond, NULL);

    // run pthreads
    for (int i = 0; i < 6; i++)
    {
        pthread_create(&threads[i], NULL, run, (void*) Game::players[i]);
    }

    // join pthreads back to main
    for (int i = 0; i < 6; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // destroy mutex
    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&cond);

    cout << "\n\nGame Complete\n";

    cout << "\nDeck:\n";
    showDeck();

    return 0;
}



/*
Run is the core 'game' logic for each player.
With all of the necessary structures in place,
each player will perform their actions relative to the state of the game. 

*/
void *run(void* player)
{

    Player *thisPlayer = (Player *) (player);
    int playerIndex = thisPlayer->number - 1;
    thisPlayer->calledForReset = false;

    while (!(Game::gameOver))
    {
        pthread_mutex_lock(&mutex1);
        while (Game::players[playerIndex]->myAction == false)
        {
            pthread_cond_wait(&cond, &mutex1);
        }

        if (Game::players[playerIndex]->myAction && !Game::roundOver && !Game::gameOver)
        {

            thisPlayer->calledForReset = false;

            // DEALER ROUTINE
            if (thisPlayer->dealer && !Game::roundDealt)
            {  

                // SHUFFLE
                Game::deck.Shuffle();

                // Announce Round
                cout << "\nDealer: " << Game::dealer + 1 << endl;
                cout << "ROUND: " << Game::round << "\n" << endl;

                // DRAW TARGET
                drawTarget(thisPlayer);
                cout << "Current target card is: " << Game::target.value << endl;

                // DEAL ROUTINE
                deal(thisPlayer);

                // END TURN
                cout << "Ends dealer turn\n" << endl;

                // dealer-specific turn end
                Game::players[playerIndex]->hasDealt = true;
                Game::roundDealt = true;

                // end turn
                endTurn(playerIndex);
            }
            else if (thisPlayer->dealer && Game::roundDealt)
            {
                endTurn(playerIndex);
            }

            // NON-DEALER ROUTINE
            else if (!thisPlayer->dealer && Game::roundDealt)
            {

                // DRAW ROUTINE
                draw(thisPlayer);

                // check if card match exists (Victory?)
                checkHand(thisPlayer);

                // DISCARD
                if (!thisPlayer->victory)
                {
                    discard(thisPlayer);
                    endTurn(playerIndex);
                } else {
                    roundOver(thisPlayer);
                }
            }
            else if (!thisPlayer->dealer && !Game::roundDealt)
            {
                endTurn(playerIndex);
            }
        }

        // ROUND ALREADY OVER BRANCH
        else if (Game::roundOver == true && thisPlayer->calledForReset == false && !Game::gameOver)
        {
            roundOver(thisPlayer);
        } 
        else if (Game::roundOver == true && thisPlayer->calledForReset == true && !Game::gameOver)
        {
            endTurn(playerIndex);
        }

        // game over
        if (Game::round == 6 && thisPlayer->victory)
        {
            Game::gameOver = true;
        }

        if (Game::gameOver)
        {
            discardAll(thisPlayer);
            endTurn(playerIndex);
        }

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex1); 
        
        usleep(10000);

        

    }

    pthread_exit(NULL);

}

void endTurn(int myIndex)
{
    Game::playerTurn = ((myIndex + 1) % 6);
    Game::players[myIndex]->myAction = false;
    Game::players[Game::playerTurn]->myAction = true;
    cout << "Player " << myIndex + 1 << " ends turn" << endl;
}

void drawTarget(Player* player)
{
    Card targetCard = Game::deck.cards.front();
    Game::deck.cards.erase(Game::deck.cards.begin());
    Game::target = targetCard;
    player->hand.push_back(targetCard);
}

void deal(Player* player)
{
    int playerIndex = player->number - 1;

    for (int i = 1; i < 6; i++)
    {
        int targetPlayer = ((playerIndex + i) % 6);
        Card currentCard = Game::deck.cards.front();
        Game::deck.cards.erase(Game::deck.cards.begin());
        Game::players[targetPlayer]->hand.push_back(currentCard);
        cout << "Deals " << currentCard.value << " to: " << "Player " << targetPlayer + 1 << endl;
    }
}

void draw(Player* player)
{
    int playerIndex = player->number - 1;

    Card drawCard = Game::deck.cards.front();
    Game::deck.cards.erase(Game::deck.cards.begin());
    Game::players[playerIndex]->hand.push_back(drawCard);
    cout << "Player " << playerIndex + 1 << " draws: " << drawCard.value << endl;
}

void checkHand(Player* player)
{
    int playerIndex = player->number - 1;
    for (auto& card : Game::players[playerIndex]->hand)
    {
        if (card.value == Game::target.value)
        {
            player->victory = true;
        }
    }

    if (player->victory == true)
    {
        Game::roundOver = true;
        cout << "Player " << playerIndex + 1<< " wins the round." << endl;
    }

}

void discard(Player* player)
{
    float randNum = rand() % 2;
    Card discard;
    int playerIndex = player->number - 1;
    if (randNum == 0)
    {
        discard = Game::players[playerIndex]->hand.front();
        Game::players[playerIndex]->hand.erase(Game::players[playerIndex]->hand.begin());   
    } else {
        discard = Game::players[playerIndex]->hand.back();
        Game::players[playerIndex]->hand.pop_back();
    }
    cout << "Player " << playerIndex + 1 << " discards: " << discard.value << endl;
    Game::deck.cards.push_back(discard);
}

void discardAll(Player* player)
{
    int playerIndex = player->number - 1;
    for (auto& card : player->hand)
    {
        Card discard = Game::players[playerIndex]->hand.front();
        Game::players[playerIndex]->hand.erase(Game::players[playerIndex]->hand.begin());
        Game::deck.cards.push_back(discard);
    }
}

void roundOver(Player* player)
{
    int playerIndex = player->number - 1;
    discardAll(player);
    Game::roundResetCount++;
    player->calledForReset = true;
    endTurn(playerIndex);
    Game::roundReset();

}

void showDeck()
{
    for (int i = 0; i < 52; i++)
    {
        cout << Game::deck.cards[i].value << " " << Game::deck.cards[i].suit << endl;
    }

}