//galahkena1@gmail.com
#include "Game.hpp"
#include "Player.hpp"

/**
 * Constructor for the Game class.
 * Initializes the game with default values.
 */
Game::Game() {}

/**
 * Adds a new player to the game.
 * Throws an exception if the player is dead, already exists, or if the game has too many players.
 */
void Game::addPlayer(shared_ptr<Player>& player)
{
    if (!player->isAlive()) {
        throw std::invalid_argument("Cannot add dead player to game.");
    }
    // Maximum 6 players allowed in the game
    if(players.size() >= 6)     
    {
       throw std::invalid_argument("Too much players to start the game."); 
    }
    // Check for duplicate players (same object or same name)
    for (const auto& p : players) {
        if (p == player || p->getName() == player->getName()) {
            throw std::invalid_argument("Player already exists in the game.");
        }
    }
    players.push_back(player);
}

/**
 * Returns the current player's turn.
 * If no players exist, returns nullptr.
 */
shared_ptr<Player> Game::currentPlayerTurn() const
{
    if (players.empty()) {
        return nullptr;
    }
    return players[turnIndex];
}

/**
 * Moves to the next player's turn.
 * Skips dead players and resets certain player states.
 */
void Game::nextTurn()
{
    if(players[turnIndex]->getisSanctioned())
    {
       players[turnIndex]->getisSanctioned() = false; 
    } 
    if(players[turnIndex]->isArrestBlocked())
    {
       players[turnIndex]->isArrestBlocked() = false; 
    } 
    if(players[turnIndex]->getCoins() < 10) 
    {
     players[turnIndex]->getMustCoup() = false; 
    }
    do {
        turnIndex = (turnIndex + 1) % players.size();
    } while (!players[turnIndex]->isAlive());


    removePendingBribe(players[turnIndex]);
    removePendingTax(players[turnIndex]);
    if (players[turnIndex]->getRole()->name() == "Merchant") {
        players[turnIndex]->getRole()->extraCoin(*players[turnIndex], *this);
    }
    if (players[turnIndex]->getCoins() >= 10) {
        players[turnIndex]->getMustCoup() = true;
        std::cout << players[turnIndex]->getName() << " has " << players[turnIndex]->getCoins() 
                  << " coins and MUST coup this turn!\n";
    }

}

/**
 * Gives an extra turn to a specific player.
 * Adjusts the turn index to make the player play again.
 */
void Game::extraTurn(Player &p)
{
    // Find the player in the players vector
    auto it = std::find_if(players.begin(), players.end(), [&p](const shared_ptr<Player>& player) {
        return player.get() == &p;
    });
    
    if (it == players.end()) {
        throw std::invalid_argument("Player not found in game.");
    }
    
    // Set the next turn to be this player's turn
    // We don't need to modify the player vector, just adjust turnIndex
    size_t playerIndex = std::distance(players.begin(), it);
    
    // Only adjust if the current player isn't already the next player
    if ((turnIndex + 1) % players.size() != playerIndex) {
        turnIndex = (playerIndex == 0) ? players.size() - 1 : playerIndex - 1;
    }
}

/**
 * Returns a list of all players in the game.
 */
vector<shared_ptr<Player>> Game::getPlayers() const
{
    return players;
}

/**
 * Returns a list of names of all active (alive) players.
 */
vector<string> Game::activePlayerNames() const
{
    vector<string> result;
    for (const shared_ptr<Player> &p : players)
    {
        if (p->isAlive())
        {
            result.push_back(p->getName());
        }
    }
    return result;
}

/**
 * Adds a player to the pending bribes list.
 */
void Game::addPendingBribe(shared_ptr<Player>& p)
{
    pendingBribes.push_back(p);
}

/**
 * Checks if a player has a pending bribe.
 */
bool Game::isBribePending(shared_ptr<Player>& p) const
{
     return std::find(pendingBribes.begin(), pendingBribes.end(), p) != pendingBribes.end();
}

/**
 * Removes a player from the pending bribes list.
 */
void Game::removePendingBribe(shared_ptr<Player>& p)
{
    pendingBribes.erase(std::remove(pendingBribes.begin(), pendingBribes.end(), p), pendingBribes.end());

}

/**
 * Adds a player to the pending taxes list.
 */
void Game::addPendingTax(shared_ptr<Player>& p)
{
    pendingTaxes.push_back(p);
}

/**
 * Checks if a player has a pending tax.
 */
bool Game::isTaxPending(shared_ptr<Player>& p) const
{
    return std::find(pendingTaxes.begin(), pendingTaxes.end(), p) != pendingTaxes.end();
}

/**
 * Removes a player from the pending taxes list.
 */
void Game::removePendingTax(shared_ptr<Player>& p)
{
    pendingTaxes.erase(std::remove(pendingTaxes.begin(), pendingTaxes.end(), p), pendingTaxes.end());
}

/**
 * Returns the winner of the game if only one player is alive.
 * Resets the game after declaring the winner.
 */
string Game::winner() const
{
    string winner;
    int count = 0;
    for (const auto &p : players)
    {
        if (p->isAlive())
        {
            count++;
            winner = p->getName();
        }
    }
    if (count != 1)
    {
        throw std::invalid_argument("Game not end");
    }
    const_cast<Game*>(this)->gameReset();
    return winner;
}

/**
 * Resets the game to its initial state.
 * Clears players, resets the coin pool, and clears pending actions.
 */
void Game::gameReset()
{
    players.clear();
    coinPool = 200;
    turnIndex = 0;
    pendingBribes.clear();
    pendingTaxes.clear();
    std::cout << "Game has been reset. You can now add new players.\n";
}

/**
 * Returns the total coins in the game pool.
 */
int Game::getCoinPool() const
{
    return this->coinPool;
}

/**
 * Removes a specific amount of coins from the pool.
 */
void Game::takeFromPool(int amount)
{
    this->coinPool -= amount;
}

/**
 * Adds a specific amount of coins to the pool.
 */
void Game::addToPool(int amount)
{
    this->coinPool += amount;
}

/**
 * Returns the name of the player whose turn it is.
 */
string Game::turn() const {
    return players[turnIndex]->getName();
}
