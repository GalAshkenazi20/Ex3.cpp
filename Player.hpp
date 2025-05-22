#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "Role.hpp"

using std::string, std::shared_ptr;
class Game; 

class Player: public std::enable_shared_from_this<Player> {
private:
    string name;
    int coins = 0;
    bool alive = true;
    bool isturn;
    shared_ptr<Role> role;
    bool isSanctioned = false;
    bool isArrest;
    bool blockCoup = false;

public:
    Player(const string& name);
    Player(const string& name, shared_ptr<Role> chosenRole);

    const string& getName() const;
    int getCoins() const;
    bool isAlive() const;
    shared_ptr<Role> getRole() const;
    bool& isTurn();
    bool& getisSanctioned();
    bool& getisAlive();
    bool& isArrested();
    bool& isBlockedCoup();

    void addCoins(int amount);
    void decreaceCoins(int amount);

    void gatherAction(Game& game);
    void taxAction(Game& game);
    void coupAction(shared_ptr<Player>& target, Game& game);
    void arrestAction(shared_ptr<Player>& target, Game& game);
    void bribeAction(Game& game);
    void sanctionAction(shared_ptr<Player>& target, Game& game);
};
