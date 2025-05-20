#pragma once
#include "Role.hpp"

class Governor: public Role
{
public:

void tax(Player& p, Game& g) override;
};