#pragma once
#include "Role.hpp"

class General: public Role
{
void preventCoup(Player& p, shared_ptr<Player> target, Game& g);
};