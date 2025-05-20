#pragma once

#include <vector>
#include <string>
#include <memory>
using std::string, std::shared_ptr, std::vector;
class Player;

class Game {
private:
    vector<shared_ptr<Player>> players;
    size_t turnIndex = 0;
    int coinPool = 50;

public:
    Game();

    void addPlayer(shared_ptr<Player> player);
    shared_ptr<Player> currentPlayer() const;
    void nextTurn();

    vector<shared_ptr<Player>> getPlayers() const;
    vector<string> activePlayerNames() const;

    string winner() const;
    int getCoinPool() const;
    void takeFromPool(int amount);
    void addToPool(int amount);
};
