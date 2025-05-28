//galahkena1@gmail.com
#include "Spy.hpp"
#include "Player.hpp"
#include <iostream>

void Spy::seeTargetCoins(Player& p,shared_ptr<Player>& target)
{
    std::cout<<target->getCoins()<<std::endl; 
}

void Spy::blockArrest(shared_ptr<Player>& target)
{
    target->isArrestBlocked() = true;  // תוקן מ-isArrested
}
