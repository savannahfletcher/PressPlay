//
//  Game.h
//  PressPlay
//
//  Created by Savannah Fletcher on 12/5/22.
//  Copyright © 2022 Savannah Fletcher. All rights reserved.
//
#include <string>
using namespace std;

class Game {

private:
    string name = "name";
    string genre = "genre";
    double price = -1.0;

public:
    Game(string n, string g, double p);
    string getName();
    string getGenre();
    double getPrice();
};

Game::Game(string n, string g, double p) {
    name = n;
    genre = g;
    price = p;
}

string Game::getName() {
    return name;
}

string Game::getGenre() {
    return genre;
}

double Game::getPrice() {
    return price;
}
