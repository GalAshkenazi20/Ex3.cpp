//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

/**
 * General role class.
 * Can protect players from coups and recovers coins when arrested.
 */
class General: public Role
{
public:
    /**
     * Prevents a coup against target player.
     * Costs 5 coins and blocks the next coup attempt on target.
     */
    void preventCoup(Player& p, shared_ptr<Player>& target, Game& g) override;

    /**
     * Returns whether General can block coup actions.
     * Always returns true for General.
     */
    virtual bool canBlockCoup(shared_ptr<Player>& target) const override;

    /**
     * Special reaction when General is arrested.
     * Gets back the stolen coin due to military training.
     */
    virtual void onArrested() override;

    /**
     * Returns the role name.
     */
    virtual string name() const override { return "General"; }
};