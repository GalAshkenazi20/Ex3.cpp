//galahkena1@gmail.com
#include "Governor.hpp"
#include "Player.hpp"
#include "Game.hpp"
void Governor::tax(shared_ptr<Player>& p, Game& g) 
{
g.takeFromPool(3);
p->addCoins(3);
g.addPendingTax(p);
}

void Governor::cancelTax(Player& p, std::shared_ptr<Player>& target, Game& g)
{
    if (!g.isTaxPending(target)) {
        throw std::invalid_argument("No tax to cancel for this player.");
    }

    int refund = (target->getRole()->name() == "Governor") ? 3 : 2;
    target->decreaceCoins(refund);
    g.addToPool(refund);

    g.removePendingTax(target);
    std::cout << p.getName() << " (Governor) cancelled tax of " << target->getName() << ".\n";
}
