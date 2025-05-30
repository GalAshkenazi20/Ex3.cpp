//galahkena1@gmail.com
#pragma once
#include "Role.hpp"

/**
 * Merchant role class.
 * Gets bonus coins and pays differently when arrested.
 */
class Merchant: public Role
{
public:
    /**
     * Gives Merchant an extra coin at turn start.
     * Only works if Merchant has 3 or more coins.
     */
    void extraCoin(Player& p, Game& g) override;

    /**
     * Special reaction when Merchant is arrested.
     * Pays 2 coins to pool instead of giving 1 to attacker.
     */
    virtual void onArrested() override;

    /**
     * Returns the role name.
     */
    virtual std::string name() const override { return "Merchant"; }
};
