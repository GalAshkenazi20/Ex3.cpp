//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

/**
 * Governor role class.
 * Can collect 3 coins with tax and cancel other players' taxes.
 */
class Governor: public Role
{
public:
    /**
     * Governor's tax action.
     * Collects 3 coins instead of the default 2.
     */
    void tax(shared_ptr<Player>& p, Game& g) override;

    /**
     * Returns whether Governor can block tax actions.
     * Always returns true for Governor.
     */
    bool canBlockTax() const override { return true;}

    /**
     * Cancels another player's tax.
     * Forces target to return the coins they gained from tax.
     */
    void cancelTax(Player& p, std::shared_ptr<Player>& target, Game& g);

    /**
     * Returns the role name.
     */
    virtual string name() const override { return "Governor"; }
};