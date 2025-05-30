//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

/**
 * Baron role class.
 * Can invest coins for profit and gets compensation when sanctioned.
 */
class Baron: public Role
{
public:
    /**
     * Baron's special invest action.
     * Pays 3 coins to get 6 coins back (net gain of 3).
     */
    void invest(Player& p, Game& g) override;

    /**
     * Special reaction when Baron is sanctioned.
     * Receives 1 coin as compensation.
     */
    virtual void onSanctioned(Player &byPlayer, Game &g) override;

    /**
     * Returns the role name.
     */
    virtual std::string name() const override { return "Baron"; }
};