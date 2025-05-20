#pragma once
#include "Role.hpp"

class Judge: public Role
{
void cancelBribe(Player& p, shared_ptr<Player> target, Game& g);
};