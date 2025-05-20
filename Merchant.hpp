#pragma once
#include "Role.hpp"

class Merchant: public Role
{
    void extraCoin(Player& p, shared_ptr<Player> target, Game& g);
};