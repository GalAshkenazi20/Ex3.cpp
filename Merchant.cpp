//galahkena1@gmail.com
#include "Merchant.hpp"
#include "Player.hpp"
#include "Game.hpp"

/**
 * Gives Merchant an extra coin at turn start.
 * Only works if Merchant has 3 or more coins.
 */
void Merchant::extraCoin(Player &p, Game &g)
{
    if(p.getCoins() >= 3) {
        g.takeFromPool(1); 
        p.addCoins(1); 
        std::cout << p.getName() << " (Merchant) receives bonus coin for having 3+ coins.\n";
    }
}

/**
 * Special reaction when Merchant is arrested.
 * Pays 2 coins to pool instead of giving 1 to attacker.
 */
void Merchant::onArrested()
{
    std::cout << "Merchant pays 2 coins to pool instead of losing 1 to attacker.\n";
}