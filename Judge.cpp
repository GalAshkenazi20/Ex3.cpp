#include "Judge.hpp"
#include "Player.hpp"
#include "Game.hpp"

void Judge::cancelBribe(Player &p, shared_ptr<Player>& target, Game &g)
{
    if (!g.isBribePending(target)) {
        throw std::invalid_argument("No bribe to cancel for this player.");
    }

    g.removePendingBribe(target); 
    std::cout << p.getName() << " (Judge) cancelled bribe of " << target->getName() << ". Coins lost.\n";
}

void Judge::onSanctioned(Player &p, Game &g)
{
    if (p.getCoins() < 1) {
        throw std::runtime_error("Player cannot afford Judge's retaliation for sanction.");
    }
    p.decreaceCoins(1);
    g.addToPool(1);

}
