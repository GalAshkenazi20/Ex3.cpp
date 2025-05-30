//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

/**
 * Judge role class.
 * Can cancel bribes and forces extra payment when sanctioned.
 */
class Judge : public Role
{
public:
    /**
     * Cancels another player's bribe action.
     * Removes the target from pending bribes list.
     */
    void cancelBribe(Player &p, shared_ptr<Player>& target, Game &g);

    /**
     * Special reaction when Judge is sanctioned.
     * Forces the attacker to pay an extra coin.
     */
    virtual void onSanctioned(Player &byPlayer, Game &g);

    /**
     * Returns the role name.
     */
    std::string name() const override { return "Judge"; }
};