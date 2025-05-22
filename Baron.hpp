#pragma once
#include "Role.hpp"

 class Baron: public Role
 {
    public:

    void invest(Player& p, Game& g);
    virtual std::string name() const override { return "Baron"; }
 };