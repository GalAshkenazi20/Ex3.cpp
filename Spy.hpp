//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

/**
 * Spy role class.
 * Can see other players' coins and block arrest actions.
 */
class Spy : public Role
{
public:
    /**
     * Allows spy to see how many coins target player has.
     * Displays the target's coin count.
     */
    void seeTargetCoins(Player& p, shared_ptr<Player>& target);

    /**
     * Blocks a player from being arrested this turn.
     * Sets the target's arrest block flag to true.
     */
    void blockArrest(shared_ptr<Player>& target);

    /**
     * Returns the role name.
     */
    virtual std::string name() const override { return "Spy"; }
};