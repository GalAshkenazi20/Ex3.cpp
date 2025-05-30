// galahkena1@gmail.com
#pragma once

#include <string>
#include <memory>
#include <algorithm>

using std::string, std::shared_ptr;
class Player;
class Game;

/**
 * Role class represents the role of a player in the game.
 * Each role has unique abilities and actions.
 */
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
     * Called when the player is sanctioned.
     */
    virtual void onSanctioned(Player &byPlayer, Game &g);

    /**
     * Called when the player is arrested.
     */
    virtual void onArrested();

    /**
     * Performs the invest action (specific to the Baron role).
     */
    virtual void invest(Player& p, Game& g);

    /**
     * Performs an extra coin action (specific to the Merchant role).
     */
    virtual void extraCoin(Player& p, Game& g);

    /**
     * Virtual destructor for the Role class.
     */
    virtual ~Role() = default;
};
