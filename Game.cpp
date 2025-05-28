//galahkena1@gmail.com
#include "Game.hpp"
#include "Player.hpp"

Game::Game()
{
}

void Game::addPlayer(shared_ptr<Player>& player)
{
    if (!player->isAlive()) {
        throw std::invalid_argument("Cannot add dead player to game.");
    }
    if(players.size() >= 6)     
    {
       throw std::invalid_argument("Too much players to start the game."); 
    }
    for (const auto& p : players) {
        if (p == player || p->getName() == player->getName()) {
            throw std::invalid_argument("Player already exists in the game.");
        }
    }
    players.push_back(player);
}
shared_ptr<Player> Game::currentPlayerTurn() const
{
    if (players.empty()) {
        return nullptr;
    }
    return players[turnIndex];
}

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

vector<shared_ptr<Player>> Game::getPlayers() const
{
    return players;
}

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

void Game::addPendingBribe(shared_ptr<Player>& p)
{
    pendingBribes.push_back(p);
}

bool Game::isBribePending(shared_ptr<Player>& p) const
{
     return std::find(pendingBribes.begin(), pendingBribes.end(), p) != pendingBribes.end();
}

void Game::removePendingBribe(shared_ptr<Player>& p)
{
    pendingBribes.erase(std::remove(pendingBribes.begin(), pendingBribes.end(), p), pendingBribes.end());

}

void Game::addPendingTax(std::shared_ptr<Player>& p)
{
    pendingTaxes.push_back(p);
}

bool Game::isTaxPending(std::shared_ptr<Player>& p) const
{
    return std::find(pendingTaxes.begin(), pendingTaxes.end(), p) != pendingTaxes.end();
}

void Game::removePendingTax(shared_ptr<Player>& p)
{
    pendingTaxes.erase(std::remove(pendingTaxes.begin(), pendingTaxes.end(), p), pendingTaxes.end());
}

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
    return winner;
}

void Game::gameReset()
{
    players.clear();
    coinPool = 200;
    turnIndex = 0;
    pendingBribes.clear();
    pendingTaxes.clear();
    

}

int Game::getCoinPool() const
{
    return this->coinPool;
}

void Game::takeFromPool(int amount)
{
    this->coinPool -= amount;
}

void Game::addToPool(int amount)
{
    this->coinPool += amount;
}

string Game::turn() const {
    return players[turnIndex]->getName();
}
