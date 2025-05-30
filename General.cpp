//galahkena1@gmail.com
#include "General.hpp"
#include "Player.hpp"
#include "Game.hpp"

void General::preventCoup(Player &p, shared_ptr<Player>& target, Game &g)
{
    if (p.getCoins() < 5) {
        throw std::invalid_argument("Not enough coins to prevent a coup (need 5 coins)");
    }
    p.decreaceCoins(5);             
    g.addToPool(5);                   
    target->isBlockedCoup() = true;    
    
    std::cout << p.getName() << " (General) prevented coup against " 
              << target->getName() << " for 5 coins.\n";
}

/**
 * Returns whether General can block coup actions.
 * Always returns true for General.
 */
bool General::canBlockCoup(shared_ptr<Player>& target) const
{
    return true;
}

/**
 * Special reaction when General is arrested.
 * Gets back the stolen coin due to military training.
 */
void General::onArrested()
{
    std::cout << "General recovers the stolen coin due to special ability.\n";
}