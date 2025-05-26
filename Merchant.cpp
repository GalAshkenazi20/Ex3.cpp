#include "Merchant.hpp"
#include "Player.hpp"
#include "Game.hpp"

void Merchant::extraCoin(Player &p, Game &g)
{
    if(p.getCoins() >= 3) {
        g.takeFromPool(1);
        p.addCoins(1);
        std::cout << p.getName() << " (Merchant) receives bonus coin for having 3+ coins.\n";
    }
}

void Merchant::onArrested()
{
    std::cout << "Merchant pays 2 coins to pool instead of losing 1 to attacker.\n";
}