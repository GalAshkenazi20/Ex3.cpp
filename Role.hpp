// galahkena1@gmail.com
#pragma once

#include <string>
#include <memory>
#include <algorithm>

using std::string, std::shared_ptr;
class Player;
class Game;

class Role
{
public:
    /**
     * Returns the name of the role.
     */
    virtual string name() const = 0;
    /**
     * Performs the gather action to collect coins.
     */
    void gather(Player &p, Game &g);
    /**
     * Performs the tax action to collect coins.
     */
    virtual void tax(shared_ptr<Player> &p, Game &g);
    /**
     * Performs the bribe action to gain an extra turn.
     */
    virtual void bribe(shared_ptr<Player> &p, Game &g);
    /**
     * Performs the arrest action to take coins from another player.
     */
    void arrest(Player &p, shared_ptr<Player> &target, Game &g);
    /**
     * Performs the sanction action to penalize another player.
     */
    void sanction(Player &p, shared_ptr<Player> &target, Game &g);
    /**
     * Performs the coup action to eliminate another player.
     */
    void coup(Player &p, shared_ptr<Player> &target, Game &g);
    /**
     * Check if the Player can block other player from tax
     */
    virtual bool canBlockTax() const { return false; }
    /**
     * Check if the Player can block other player from bribe
     */
    virtual bool canBlockBribe() const { return false; }
    /**
     * Check if the Player can block other player from coup
     */
    virtual bool canBlockCoup(shared_ptr<Player> &target) const { return false; }
    /**
     * Check if the Player is sacnctioned
     */
    virtual bool isSanctioned() const { return false; }
    /**
     * Check if the Player is prevent from doing coup action
     */
    virtual void preventCoup(Player &p, std::shared_ptr<Player> &target, Game &g) {}
    /**
     * Check if the Player can cancel other player bribe
     */
    virtual void cancelBribe(Player &p, std::shared_ptr<Player> &target, Game &g) {}
    /**
     * Check if the Player can cancel other player bribe
     */
    virtual void cancelTax(Player &p, std::shared_ptr<Player> &target, Game &g) {}
    /**
     * Called when the player is sanctioned.
     */
    virtual void onSanctioned(Player &byPlayer, Game &g);
    /**
     * Cakked wgeb the player is arrested
     */
    virtual void onArrested();

    // Spy actions
    virtual void seeTargetCoins(Player &p, std::shared_ptr<Player> &target) {}
    virtual void blockArrest(std::shared_ptr<Player> &target) {}
    /**
     * Performs the invest action (specific to the Baron role).
     */
    virtual void invest(Player &p, Game &g);
    /**
     * Performs an extra coin action (specific to the Merchant role).
     */
    virtual void extraCoin(Player &p, Game &g) {}
    /**
     * Virtual destructor for the Role class.
     */
    virtual ~Role() = default;
};
