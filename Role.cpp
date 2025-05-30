//galahkena1@gmail.com
#include "Role.hpp"
#include "Player.hpp"
#include "Game.hpp"

/**
 * Performs the basic gather action.
 * Collects 1 coin from the pool.
 */
void Role::gather(Player &p, Game &g)
{
    g.takeFromPool(1); 
    p.addCoins(1);      
}

/**
 * Performs the tax action.
 * Default implementation collects 2 coins.
 */
void Role::tax(shared_ptr<Player> &p, Game &g)
{
    // Safety check for null pointer
    if (!p) {
        throw std::invalid_argument("Player is null");
    }
    
    g.takeFromPool(2);   
    p->addCoins(2);      
    g.addPendingTax(p);   
}

/**
 * Performs the bribe action.
 * Costs 4 coins and gives an extra turn.
 */
void Role::bribe(shared_ptr<Player> &p, Game &g)
{
    if (!p) {
        throw std::invalid_argument("Player is null");
    }
    
    if (p->getCoins() < 4) {
        throw std::invalid_argument("The player: " + p->getName() + " doesn't have enough money.");
    }
    
    p->decreaceCoins(4);  
    g.addToPool(4);       
}

/**
 * Performs the arrest action.
 * Takes 1 coin from target player.
 */
void Role::arrest(Player &p, shared_ptr<Player> &target, Game &g)
{
    if (!target) {
        throw std::invalid_argument("Target is null");
    }

    if (!target->isAlive()) {
        throw std::logic_error("Cannot arrest dead player");
    }

    if (&p == target.get()) {
        throw std::logic_error("Cannot arrest yourself");
    }

    if (target->getCoins() < 1) {
        throw std::logic_error("Target has no money, can't take from him.\nPlease choose other target");
    }

    if (p.isArrestBlocked()) {
        throw std::logic_error("Player is blocked from doing arrest this turn.");
    }
    
    // Special handling for Merchant role
    if (target->getRole()->name() == "Merchant") {
        if (target->getCoins() < 2) {
            throw std::logic_error("Cant take from target, target has less then 2 coins");
        }
        target->decreaceCoins(2);
        g.addToPool(2);
    }
    else {
        // Normal arrest - take 1 coin from target, give to attacker
        if (target->getCoins() < 1) {
            throw std::logic_error("Target has no money, can't take from him.\nPlease choose other target");
        }
        target->decreaceCoins(1);
        p.addCoins(1);
        std::cout << p.getName() << " arrested " << target->getName() << " and took 1 coin.\n";
        
        // Special General ability - gets back the arrested coin
        if (target->getRole()->name() == "General") {
            g.takeFromPool(1);
            target->addCoins(1);
            std::cout << "General effect: " << target->getName() << " gets back the arrested coin.\n";
        }
    }
}

/**
 * Performs the sanction action.
 * Costs 3 coins and blocks target from basic actions.
 */
void Role::sanction(Player &p, shared_ptr<Player> &target, Game &g)
{
    if (!target) {
        throw std::invalid_argument("Target is null");
    }

    if (!target->isAlive()) {
        throw std::logic_error("Cannot sanction dead player");
    }

    if (&p == target.get()) {
        throw std::logic_error("Cannot sanction yourself");
    }

    if (p.getCoins() < 3) {
        throw std::invalid_argument("Can't use sanction, not enough money (need 3 coins)");
    }

    if (target->getisSanctioned()) {
        throw std::logic_error("Target is already sanctioned");
    }
    
    // Special handling for Judge role
    if (target->getRole()->name() == "Judge") {
        if (p.getCoins() >= 1) {
            p.decreaceCoins(1);
            g.addToPool(1);
            std::cout << "Judge effect: " << p.getName() << " pays additional 1 coin.\n";
        }
    }

    // Special handling for Baron role
    if (target->getRole()->name() == "Baron") {
        // Baron receives compensation when sanctioned
        g.takeFromPool(1);
        target->addCoins(1);
        std::cout << "Baron effect: " << target->getName() << " receives 1 coin compensation.\n";
    }

    // Perform the sanction
    p.decreaceCoins(3);           
    g.addToPool(3);               
    target->getisSanctioned() = true; 
    std::cout << p.getName() << " sanctioned " << target->getName() << " for 3 coins.\n";
}

/**
 * Performs the coup action.
 * Costs 7 coins and eliminates target player.
 */
void Role::coup(Player &p, shared_ptr<Player> &target, Game &g)
{
    if (!target) {
        throw std::invalid_argument("Target is null");
    }

    if (!target->isAlive()) {
        throw std::logic_error("Cannot coup dead player");
    }

    if (&p == target.get()) {
        throw std::logic_error("Cannot coup yourself");
    }

    if (p.getCoins() < 7) {
        throw std::invalid_argument("Can't coup player, not enough money (need 7 coins)");
    }
    
    // Check if target is protected from coup
    if (target->isBlockedCoup()) {

        p.decreaceCoins(7);
        g.addToPool(7);
        target->isBlockedCoup() = false;  // Protection is consumed
        p.getMustCoup() = false;
        return;
    }
    p.decreaceCoins(7);           
    g.addToPool(7);               
    target->getisAlive() = false; 
    p.getMustCoup() = false;      
}

/**
 * Called when this player is sanctioned.
 * Default implementation - do nothing. Specific roles can override.
 */
void Role::onSanctioned(Player &byPlayer, Game &g)
{
    // Default implementation - do nothing
    // Specific roles can override this
}

/**
 * Called when this player is arrested.
 * Default implementation - do nothing. Specific roles can override.
 */
void Role::onArrested()
{
    // Default implementation - do nothing
    // Specific roles can override this
}

/**
 * Performs the invest action (Baron only).
 * Default throws exception for non-Baron roles.
 */
void Role::invest(Player &p, Game &g)
{
    throw std::invalid_argument("This player's role isn't Baron");
}
