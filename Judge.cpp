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

void Judge::onSanctioned(Player &byPlayer, Game &g)
{
    if (byPlayer.getCoins() < 1) {
        throw std::runtime_error("Player cannot afford Judge's retaliation for sanction.");
    }
    byPlayer.decreaceCoins(1);
    g.addToPool(1);
    std::cout << "Judge forces " << byPlayer.getName() << " to pay an extra coin!\n";
}
