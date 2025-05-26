#include "Role.hpp"
#include "Player.hpp"
#include "Game.hpp"

void Role::gather(Player& p, Game& g)
{
    if(p.getisSanctioned())
    {
throw std::invalid_argument(p.getName() + " is sanctioned and cannot gather!");
    
    }
    g.takeFromPool(1);
    p.addCoins(1);
}

void Role::tax(shared_ptr<Player>& p, Game &g)
{
    if (!p) {
        throw std::invalid_argument("Player is null");
    }
    if (p->getisSanctioned()) {
        throw std::invalid_argument(p->getName() + " is sanctioned and cannot use tax!");
    }
    g.takeFromPool(2);
    p->addCoins(2);
    g.addPendingTax(p);
}

void Role::bribe(shared_ptr<Player>& p, Game &g)
{
    if (!p) {
        throw std::invalid_argument("Player is null");
    }
    if(p->getCoins() < 4)
    {
        throw std::invalid_argument("The player: "+ p->getName() + " doesn't have enough money.");
    }
    p->decreaceCoins(4);
    g.addToPool(4);
}

void Role::arrest(Player &p, shared_ptr<Player>& target, Game &g)
{
    // Add comprehensive safety checks
    if (!target) {
        throw std::invalid_argument("Target is null");
    }
    
    if (!target->isAlive()) {
        throw std::logic_error("Cannot arrest dead player");
    }
    
    if (&p == target.get()) {
        throw std::logic_error("Cannot arrest yourself");
    }
    
    if(target->getCoins() < 1)
    {
        throw std::logic_error("Target has no money, can't take from him.\nPlease choose other target");
    }
    
    if(p.isArrestBlocked())
    {
        throw std::logic_error("Player is blocked from doing arrest this turn.");  
    }

    target->decreaceCoins(1);
    p.addCoins(1);
}

void Role::sanction(Player &p, shared_ptr<Player>& target, Game &g)
{
    // Add comprehensive safety checks
    if (!target) {
        throw std::invalid_argument("Target is null");
    }
    
    if (!target->isAlive()) {
        throw std::logic_error("Cannot sanction dead player");
    }
    
    if (&p == target.get()) {
        throw std::logic_error("Cannot sanction yourself");
    }
    
    if(p.getCoins() < 3)
    {
        throw std::invalid_argument("Can't use sanction, not enough money (need 3 coins)");
    }
    
    if(target->getisSanctioned()) {
        throw std::logic_error("Target is already sanctioned");
    }
    
    // Perform the sanction
    p.decreaceCoins(3);
    g.addToPool(3);
    target->getisSanctioned() = true;
     std::cout << p.getName() << " sanctioned " << target->getName() << " for 3 coins.\n";
    

     if(target->getRole()->name() == "Judge") {
        // שופט - השחקן שעושה sanction משלם מטבע נוסף
        if(p.getCoins() >= 1) {
            p.decreaceCoins(1);
            g.addToPool(1);
            std::cout << "Judge effect: " << p.getName() << " pays additional 1 coin.\n";
        }
    }
    
    if(target->getRole()->name() == "Baron") {
        // ברון - מקבל מטבע כפיצוי
        g.takeFromPool(1);
        target->addCoins(1);
        std::cout << "Baron effect: " << target->getName() << " receives 1 coin compensation.\n";
    }
}

void Role::coup(Player &p, shared_ptr<Player>& target, Game &g)
{
    // Add comprehensive safety checks
    if (!target) {
        throw std::invalid_argument("Target is null");
    }
    
    if (!target->isAlive()) {
        throw std::logic_error("Cannot coup dead player");
    }
    
    if (&p == target.get()) {
        throw std::logic_error("Cannot coup yourself");
    }
    
    if(p.getCoins() < 7)
    {
        throw std::invalid_argument("Can't coup player, not enough money (need 7 coins)");
    }
    if(target->isBlockedCoup()) {
        throw std::invalid_argument("Coup is blocked by General!");
    }
   
    p.decreaceCoins(7);
    g.addToPool(7);
    target->getisAlive() = false;
    // Perform the coup
   p.getMustCoup() = false;
    target->getisAlive() = false;
}

void Role::onSanctioned(Player& byPlayer, Game& g)
{
    // Default implementation - do nothing
    // Specific roles can override this
}

void Role::onArrested()
{
    // Default implementation - do nothing
    // Specific roles can override this
}

void Role::invest(Player& p, Game& g)
{
    throw std::invalid_argument("This player's role isn't Baron");
}
