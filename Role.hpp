#pragma once

#include <string>
#include <memory>
#include <algorithm>

using std::string, std::shared_ptr;
class Player;
class Game;

class Role
{
public:
    virtual string name() const = 0;
    void gather(Player &p, Game &g);
    virtual void tax(shared_ptr<Player> &p, Game &g);
    virtual void bribe(shared_ptr<Player> &p, Game &g);
    virtual void arrest(Player &p, shared_ptr<Player> &target, Game &g);
    virtual void sanction(Player &p, shared_ptr<Player> &target, Game &g);
    void coup(Player &p, shared_ptr<Player> &target, Game &g);

    virtual bool canBlockTax() const { return false; }
    virtual bool canBlockBribe() const { return false; }
    virtual bool canBlockCoup(shared_ptr<Player> &target) const { return false; }
    virtual bool isSanctioned() const { return false; }

    virtual void preventCoup(Player &p, std::shared_ptr<Player> &target, Game &g) {}
    virtual void cancelBribe(Player &p, std::shared_ptr<Player> &target, Game &g) {}
    virtual void cancelTax(Player &p, std::shared_ptr<Player> &target, Game &g) {}
    virtual void onSanctioned(Player &byPlayer, Game &g);
    virtual void onArrested();

    virtual void seeTargetCoins(Player &p, std::shared_ptr<Player> &target){}
    virtual void blockArrest(std::shared_ptr<Player> &target){}

    virtual ~Role() = default;
};
