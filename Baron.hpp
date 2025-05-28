//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

 class Baron: public Role
 {
    public:

    void invest(Player& p, Game& g) override;
    virtual void onSanctioned(Player &byPlayer, Game &g) override;  // הוספה
    virtual std::string name() const override { return "Baron"; }
 };