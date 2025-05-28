//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

class Governor: public Role
{
public:

void tax(shared_ptr<Player>& p, Game& g) override;
bool canBlockTax() const override { return true;}
void cancelTax(Player& p, std::shared_ptr<Player>& target, Game& g);
virtual string name() const override { return "Governor"; }
};