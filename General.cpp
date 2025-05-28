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

bool General::canBlockCoup(shared_ptr<Player>& target) const
{
    return true;
}

void General::onArrested()
{
    // General gets back the coin when arrested
    std::cout << "General recovers the stolen coin due to special ability.\n";
}