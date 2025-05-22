#pragma once
#include "Role.hpp"

class Judge : public Role
{
public:
    void cancelBribe(Player &p, shared_ptr<Player>& target, Game &g);
    virtual void onSanctioned(Player &byPlayer, Game &g);
    std::string name() const override { return "Judge"; }
};