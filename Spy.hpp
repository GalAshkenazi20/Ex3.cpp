//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

class Spy : public Role
{
    public:

    void seeTargetCoins(Player& p,shared_ptr<Player>& target) override;
    void blockArrest(shared_ptr<Player>& target) override;
    virtual std::string name() const override { return "Spy"; }
};