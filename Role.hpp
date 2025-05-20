#pragma once

#include <string>
#include <memory>
using std::string, std::shared_ptr;
class Player;
class Game;

class Role {
public:
    virtual std::string name() const = 0;

    virtual void gather(Player& p, Game& g);
    virtual void tax(Player& p, Game& g);
    virtual void bribe(Player& p, Game& g);
    virtual void arrest(Player& p, shared_ptr<Player> target, Game& g);
    virtual void sanction(Player& p, shared_ptr<Player> target, Game& g);
    void coup(Player& p, shared_ptr<Player> target, Game& g);

    virtual bool canBlockTax() const { return false; }
    virtual bool canBlockBribe() const { return false; }
    virtual bool canBlockCoup(shared_ptr<Player> target) const { return false; }
    virtual bool isSanctioned() const { return false; }

    virtual void onSanctioned();
    virtual void onArrested();   

    virtual ~Role() = default;
};
