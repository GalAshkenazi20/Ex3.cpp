//galahkena1@gmail.com
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Game.hpp"
#include <random>
#include <stdexcept>
#include "Role.hpp"

/**
 * Constructor for creating a player with a random role.
 * Randomly assigns one of six roles to the player.
 */
Player::Player(const std::string &name)
    : name(name), coins(0), alive(true), isturn(false), arrestBlocked(false)
{
    // Set up random number generator for role selection
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 5);

    // Generate random number and assign corresponding role
    int r = dist(gen);
    switch (r) {
    case 0:
        role = std::make_shared<Governor>();
        break;
    case 1:
        role = std::make_shared<Spy>();
        break;
    case 2:
        role = std::make_shared<Baron>();
        break;
    case 3:
        role = std::make_shared<General>();
        break;
    case 4:
        role = std::make_shared<Judge>();
        break;
    case 5:
        role = std::make_shared<Merchant>();
        break;
    default:
        throw std::runtime_error("Invalid role random index");
    }
}

/**
 * Constructor for creating a player with a specific role.
 * Allows direct assignment of a chosen role.
 */
Player::Player(const std::string& name, std::shared_ptr<Role> chosenRole)
    : name(name), coins(0), alive(true), isSanctioned(false), role(chosenRole) {}

/**
 * Returns the player's name.
 */
const string &Player::getName() const
{
    return this->name;
}

/**
 * Returns the number of coins the player currently has.
 */
int Player::getCoins() const
{
    return this->coins;
}

/**
 * Returns whether the player is still alive in the game.
 */
bool Player::isAlive() const
{
    return this->alive;
}

/**
 * Returns the player's role object.
 */
shared_ptr<Role> Player::getRole() const
{
    return this->role;
}

/**
 * Returns reference to whether it's currently the player's turn.
 */
bool &Player::isTurn()
{
    return this->isturn;
}

/**
 * Returns reference to whether the player is sanctioned.
 * Sanctioned players cannot gather or tax.
 */
bool &Player::getisSanctioned()
{
    return this->isSanctioned;
}

/**
 * Returns reference to whether the player is alive (modifiable).
 */
bool &Player::getisAlive()
{
    return this->alive;
}

/**
 * Returns reference to whether the player is protected from coup.
 */
bool &Player::isBlockedCoup()
{
    return this->blockCoup;
}

/**
 * Returns reference to whether the player is blocked from arresting.
 */
bool &Player::isArrestBlocked()
{
    return this->arrestBlocked;
}

/**
 * Returns reference to whether the player must perform a coup.
 * Players with 10+ coins must coup.
 */
bool &Player::getMustCoup()
{
    return this->mustCoup;
}

/**
 * Adds coins to the player's total.
 * Automatically sets mustCoup flag if player reaches 10+ coins.
 */
void Player::addCoins(int amount)
{
    this->coins += amount;
    
    if (coins >= 10 && !mustCoup) {
        mustCoup = true;
        std::cout << name << " now has " << coins << " coins and MUST coup in the next turn!\n";
    }
}

/**
 * Removes coins from the player's total.
 */
void Player::decreaceCoins(int amount)
{
    this->coins -= amount;
}

/**
 * Performs the gather action to collect 1 coin.
 * Cannot be used if sanctioned or must coup.
 */
void Player::gatherAction(Game &game)
{
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    if (isSanctioned) {
        throw std::invalid_argument(name + " is sanctioned and cannot gather!");
    }
    
    try {
        this->role->gather(*this, game);
        game.nextTurn();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Gather failed: ") + e.what());
    }
}

/**
 * Performs the tax action to collect 2-3 coins.
 * Cannot be used if sanctioned or must coup.
 */
void Player::taxAction(Game &game)
{
    if (isSanctioned) {
        throw std::invalid_argument("Player is sanctioned, cannot use tax.");
    }
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    
    // Find this player's shared_ptr in the game
    shared_ptr<Player> ss;
    for (auto &p : game.getPlayers()) {
        if (p.get() == this) {
            ss = p;
            break;
        }
    }
    this->role->tax(ss, game);
    game.nextTurn();
}

/**
 * Performs the coup action to eliminate another player.
 * Costs 7 coins and kills the target.
 */
void Player::coupAction(shared_ptr<Player>& target, Game &game)
{
    try{
        this->role->coup(*this, target, game);
    } catch(const std::exception& e) {
        throw std::runtime_error(std::string("Coup failed: ") + e.what());
    }
    
    // Reset must coup flag and move to next turn
    mustCoup = false;
    game.nextTurn();
}

/**
 * Performs the arrest action to steal 1 coin from another player.
 * Cannot be used if must coup or blocked from arresting.
 */
void Player::arrestAction(shared_ptr<Player>& target, Game &game)
{
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    
    if (isArrestBlocked()) {
        throw std::invalid_argument(name + " is blocked from arresting this turn!");
    }
    
    try {
        this->role->arrest(*this, target, game);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Arrest failed: ") + e.what());
    }
    
    game.nextTurn();
}

/**
 * Performs the bribe action to get an extra turn.
 * Costs 4 coins and allows playing again.
 */
void Player::bribeAction(Game &game)
{
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }

    shared_ptr<Player> ss;
    for (auto &p : game.getPlayers()) {
        if (p.get() == this) {
            ss = p;
            break;
        }
    }

    try {
        this->role->bribe(ss, game);
        std::cout << this->getName() << " used bribe and gets to play again!\n";
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Bribe failed: ") + e.what());
    }
}

/**
 * Performs the sanction action to block another player.
 * Costs 3 coins and prevents target from gathering/taxing.
 */
void Player::sanctionAction(shared_ptr<Player>& target, Game &game)
{
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    
    try {
        this->role->sanction(*this, target, game);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Sanction failed: ") + e.what());
    }
    
    game.nextTurn();
}

/**
 * Performs the invest action (Baron only).
 * Pays 3 coins to get 6 coins back.
 */
void Player::investAction(Game &game)
{
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    
    if (this->getRole()->name() != "Baron") {
        throw std::invalid_argument("Only Baron can invest");
    }
    
    try {
        this->role->invest(*this, game);
        game.nextTurn();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Invest failed: ") + e.what());
    }
}
