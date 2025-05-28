//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

class Merchant: public Role
{
    public:

    void extraCoin(Player& p, Game& g) override;
    virtual void onArrested() override;
    virtual std::string name() const override { return "Merchant"; }
};
