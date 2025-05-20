#pragma once

#include <string>
#include <memory>
#include "Role.hpp"
using std::string, std::shared_ptr;
class Game; 

class Player {
private:
    string name;
    int coins = 0;
    bool alive = true;
    shared_ptr<Role> role;
    bool isSanctioned = false;

public:
    Player(const std::string& name);

    const string& getName() const;
    int getCoins() const;
    bool isAlive() const;
    shared_ptr<Role> getRole() const;

    void addCoins(int amount);
    void decreaceCoins(int amount);
    void coup();

    void gatherAction(Game& game);
    void taxAction(Game& game);
    void coupAction(shared_ptr<Player> target, Game& game);
    void arrestAction(shared_ptr<Player> target, Game& game);
    void bribeAction(Game& game);
    void sanctionAction(shared_ptr<Player> target, Game& game);
};
