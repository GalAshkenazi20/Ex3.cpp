#include "Baron.hpp"
#include "Player.hpp"
#include "Game.hpp"
void Baron::invest(Player &p, Game &g)
{
    if (p.getisSanctioned())
    {
        g.takeFromPool(1);
        p.addCoins(1);
        return;
    }
    if(p.getCoins() < 3)
    {
        throw std::invalid_argument("Doesnt enough money for the Player");
    }
    p.decreaceCoins(3);
    g.addToPool(3);
    g.takeFromPool(6);
    p.addCoins(6);
}