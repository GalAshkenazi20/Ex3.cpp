//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

class General: public Role
{
    public:

    void preventCoup(Player& p, shared_ptr<Player>& target, Game& g) override;
    virtual bool canBlockCoup(shared_ptr<Player>& target) const override;
    virtual void onArrested() override;
    virtual string name() const override { return "General"; }
};