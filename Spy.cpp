//galahkena1@gmail.com
#include "Spy.hpp"
#include "Player.hpp"
#include <iostream>

/**
 * Allows spy to see how many coins target player has.
 * Displays the target's coin count.
 */
void Spy::seeTargetCoins(Player& p, shared_ptr<Player>& target)
{
    std::cout << target->getCoins() << std::endl; 
}

/**
 * Blocks a player from being arrested this turn.
 * Sets the target's arrest block flag to true.
 */
void Spy::blockArrest(shared_ptr<Player>& target)
{
    target->isArrestBlocked() = true;
}
