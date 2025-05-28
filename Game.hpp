//galahkena1@gmail.com
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
using std::string, std::shared_ptr, std::vector;
class Player;

class Game
{
private:
    vector<shared_ptr<Player>> players;
    vector<shared_ptr<Player>> pendingBribes;
    vector<shared_ptr<Player>> pendingTaxes;
    size_t turnIndex = 0;
    int coinPool = 200;

public:
    Game();

    void addPlayer(shared_ptr<Player>& player);
    shared_ptr<Player> currentPlayerTurn() const;
    void nextTurn();
    void extraTurn(Player &p);

    void addPendingBribe(shared_ptr<Player>& p);
    bool isBribePending(shared_ptr<Player>& p) const;
    void removePendingBribe(shared_ptr<Player>& p);

    void addPendingTax(shared_ptr<Player>& p);
    bool isTaxPending(shared_ptr<Player>& p) const;
    void removePendingTax(std::shared_ptr<Player>& p);

    vector<shared_ptr<Player>> getPlayers() const;
    vector<string> activePlayerNames() const;

    string winner() const;
    void gameReset();
    int getCoinPool() const;
    void takeFromPool(int amount);
    void addToPool(int amount);
    string turn() const;
};