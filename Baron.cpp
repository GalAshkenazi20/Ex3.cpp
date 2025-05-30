//galahkena1@gmail.com
#include "Baron.hpp"
#include "Player.hpp"
#include "Game.hpp"

/**
 * Baron's special invest action.
 * Pays 3 coins to get 6 coins back (net gain of 3).
 */
void Baron::invest(Player &p, Game &g)
{
    // Verify this is actually a Baron
    if(p.getRole()->name() != "Baron"){
        throw std::invalid_argument("Only Baron can invest");
    }
    if (p.getisSanctioned()) {
        throw std::invalid_argument("Baron is sanctioned, cannot invest");
    }
    if(p.getCoins() < 3) {
        throw std::invalid_argument("Baron doesn't have enough money to invest (need 3 coins)");
    }
    p.decreaceCoins(3);  
    g.addToPool(3);      
    g.takeFromPool(6);  
    p.addCoins(6);      
    
    std::cout << p.getName() << " (Baron) invested 3 coins and received 6 coins.\n";
}

/**
 * Special reaction when Baron is sanctioned.
 * Receives 1 coin as compensation.
 */
void Baron::onSanctioned(Player &byPlayer, Game &g)
{
    g.takeFromPool(1);
    std::cout << "Baron receives 1 coin compensation for being sanctioned.\n";
}