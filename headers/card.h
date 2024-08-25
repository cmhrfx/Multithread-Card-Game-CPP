#ifndef CARD_H
#define CARD_H

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

struct Card {
    Card();
    Card(char value, string suit);

    char value;
    string suit;


};


#endif