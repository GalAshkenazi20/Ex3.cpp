#include "Merchant.hpp"
#include "Player.hpp"
#include "Game.hpp"

void Merchant::extraCoin(Player &p, Game &g)
{
    if(p.getCoins() >= 3)
    {
        g.takeFromPool(1);
        p.addCoins(1);
    }
}
void Merchant::onArrested(){}