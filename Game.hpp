//galahkena1@gmail.com
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
using std::string, std::shared_ptr, std::vector;
class Player;

/**
 * Game class manages the overall game logic, including players, turns, and actions.
 */
class Game
{
private:
    vector<shared_ptr<Player>> players; // List of players in the game
    vector<shared_ptr<Player>> pendingBribes; // Players with pending bribes
    vector<shared_ptr<Player>> pendingTaxes; // Players with pending taxes
    size_t turnIndex = 0; // Index of the current player's turn
    int coinPool = 200; // Total coins in the game pool

public:
    /**
     * Constructor for the Game class.
     */
    Game();

    /**
     * Adds a new player to the game.
     * Throws an exception if the player is dead or already exists.
     */
    void addPlayer(shared_ptr<Player>& player);

    /**
     * Returns the current player's turn.
     * If no players exist, returns nullptr.
     */
    shared_ptr<Player> currentPlayerTurn() const;

    /**
     * Moves to the next player's turn.
     * Skips dead players and resets certain player states.
     */
    void nextTurn();

    /**
     * Gives an extra turn to a specific player.
     */
    void extraTurn(Player &p);

    /**
     * Adds a player to the pending bribes list.
     */
    void addPendingBribe(shared_ptr<Player>& p);

    /**
     * Checks if a player has a pending bribe.
     */
    bool isBribePending(shared_ptr<Player>& p) const;

    /**
     * Removes a player from the pending bribes list.
     */
    void removePendingBribe(shared_ptr<Player>& p);

    /**
     * Adds a player to the pending taxes list.
     */
    void addPendingTax(shared_ptr<Player>& p);

    /**
     * Checks if a player has a pending tax.
     */
    bool isTaxPending(shared_ptr<Player>& p) const;

    /**
     * Removes a player from the pending taxes list.
     */
    void removePendingTax(std::shared_ptr<Player>& p);

    /**
     * Returns a list of all players in the game.
     */
    vector<shared_ptr<Player>> getPlayers() const;

    /**
     * Returns a list of names of all active (alive) players.
     */
    vector<string> activePlayerNames() const;

    /**
     * Returns the winner of the game if only one player is alive.
     * Resets the game after declaring the winner.
     */
    string winner() const;

    /**
     * Resets the game to its initial state.
     * Clears players, resets the coin pool, and clears pending actions.
     */
    void gameReset();

    /**
     * Returns the total coins in the game pool.
     */
    int getCoinPool() const;

    /**
     * Removes a specific amount of coins from the pool.
     */
    void takeFromPool(int amount);

    /**
     * Adds a specific amount of coins to the pool.
     */
    void addToPool(int amount);

    /**
     * Returns the name of the player whose turn it is.
     */
    string turn() const;
};