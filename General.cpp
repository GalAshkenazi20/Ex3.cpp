#include "General.hpp"
#include "Player.hpp"
#include "Game.hpp"
void General::preventCoup(Player &p, shared_ptr<Player>& target, Game &g)
{
    if (p.getCoins() < 5)
    {
        throw std::invalid_argument("Not enough coins to prevent a coup.");
    }

    p.decreaceCoins(5);
    g.addToPool(5);
    target->isBlockedCoup() = true;
}
bool General::canBlockCoup(shared_ptr<Player>& target) const
{
    return true;
}
 void General::onArrested()
 {
    
 }