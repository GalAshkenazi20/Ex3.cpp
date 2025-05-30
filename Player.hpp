//galahkena1@gmail.com
#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "Role.hpp"

using std::string, std::shared_ptr;
class Game; 

/**
 * Player class represents a player in the game.
 * Each player has a name, role, coins, and other attributes.
 */
class Player: public std::enable_shared_from_this<Player> {
private:
    string name; // Player's name
    int coins = 0; // Number of coins the player has
    bool alive = true; // Whether the player is alive
    bool isturn; // Whether it's the player's turn
    shared_ptr<Role> role; // The player's role
    bool isSanctioned = false; // Whether the player is sanctioned
    bool blockCoup = false; // Whether the player is protected from a coup
    bool arrestBlocked = false; // Whether the player is blocked from arresting
    bool mustCoup = false; // Whether the player must perform a coup

public:
    /**
     * Constructor for creating a player with a random role.
     */
    Player(const string& name);

    /**
     * Constructor for creating a player with a specific role.
     */
    Player(const string& name, shared_ptr<Role> chosenRole);

    /**
     * Returns the player's name.
     */
    const string& getName() const;

    /**
     * Returns the number of coins the player has.
     */
    int getCoins() const;

    /**
     * Returns whether the player is alive.
     */
    bool isAlive() const;

    /**
     * Returns the player's role.
     */
    shared_ptr<Role> getRole() const;

    /**
     * Returns whether it's the player's turn.
     */
    bool& isTurn();

    /**
     * Returns whether the player is sanctioned.
     */
    bool& getisSanctioned();

    /**
     * Returns whether the player is alive (modifiable).
     */
    bool& getisAlive();

    /**
     * Returns whether the player is protected from a coup.
     */
    bool& isBlockedCoup();

    /**
     * Returns whether the player is blocked from arresting.
     */
    bool& isArrestBlocked();

    /**
     * Returns whether the player must perform a coup.
     */
    bool& getMustCoup();

    /**
     * Adds coins to the player's total.
     */
    void addCoins(int amount);

    /**
     * Decreases coins from the player's total.
     */
    void decreaceCoins(int amount);

    /**
     * Performs the gather action to collect coins.
     */
    void gatherAction(Game& game);

    /**
     * Performs the tax action to collect coins.
     */
    void taxAction(Game& game);

    /**
     * Performs the coup action to eliminate another player.
     */
    void coupAction(shared_ptr<Player>& target, Game& game);

    /**
     * Performs the arrest action to take coins from another player.
     */
    void arrestAction(shared_ptr<Player>& target, Game& game);

    /**
     * Performs the bribe action to gain an extra turn.
     */
    void bribeAction(Game& game);

    /**
     * Performs the sanction action to penalize another player.
     */
    void sanctionAction(shared_ptr<Player>& target, Game& game);

    /**
     * Performs the invest action (specific to the Baron role).
     */
    void investAction(Game& game);
};
