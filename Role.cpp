#include "Role.hpp"
#include "Player.hpp"
#include "Game.hpp"

void Role::gather(Player& p, Game& g)
{
    if(p.getisSanctioned())
    {
        std::cout << "is sanctioned, cannot gather coins.\n";
       return; 
    }
    g.takeFromPool(1);
    p.addCoins(1);
}

void Role::tax(shared_ptr<Player>& p, Game &g)
{
    g.takeFromPool(2);
    p->addCoins(2);
    g.addPendingTax(p);
}

void Role::bribe(shared_ptr<Player>& p, Game &g)
{
    if(p->getCoins() < 4)
    {
        throw std::invalid_argument("The player: "+ p->getName() + " doesn't have enough money.");
    }
    p->decreaceCoins(4);
    g.addToPool(4);
    g.addPendingBribe(p);
    g.extraTurn(*p);
}

void Role::arrest(Player &p, shared_ptr<Player>& target, Game &g)
{
    if(target->getCoins() < 1)
    {
        throw std::logic_error("Target has no money, can't take from him.\n pls choose other target");
    }
    target->decreaceCoins(1);
    p.addCoins(1);
}

void Role::sanction(Player &p, shared_ptr<Player>& target, Game &g)
{
    if(p.getCoins() < 3)
    {
        throw std::invalid_argument("cant ust sanction, no enough money");
    }
    p.decreaceCoins(3);
    g.addToPool(3);
    target->getisSanctioned() = true;
}

void Role::coup(Player &p, shared_ptr<Player>& target, Game &g)
{
    if(p.getCoins() < 7)
    {
        throw std::invalid_argument("C'ant coup player dont have enough money");
    }
    if (target->isBlockedCoup())
    {
        std::cout << "target is blocked cant coup them\n";
        target->isBlockedCoup() = false;
        return;
    }
    p.decreaceCoins(7);
    g.addToPool(7);
    target->getisAlive() = false;
}

void Role::onSanctioned(Player& byPlayer, Game& g)
{
}

void Role::onArrested()
{
}
