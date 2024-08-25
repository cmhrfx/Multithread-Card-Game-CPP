#include "../headers/player.h"

Player::Player() : number(0), dealer(false), myAction(false), hasDealt(false), victory(false) {};

Player::Player(int number, bool dealer, bool myAction, bool hasDealt, bool victory) : 
        number(number), dealer(dealer), myAction(myAction), hasDealt(hasDealt), victory(victory) {};

