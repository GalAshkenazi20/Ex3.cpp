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

Player::Player(const std::string &name)
    : name(name), coins(0), alive(true), isturn(false), arrestBlocked(false)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 5);

    int r = dist(gen);
    switch (r)
    {
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


    // Player::Player(const std::string& name, std::shared_ptr<Role> chosenRole)
    // : name(name), coins(0), alive(true), isArrest(false), isSanctioned(false), role(chosenRole) {}

const string &Player::getName() const
{
    return this->name;
}

int Player::getCoins() const
{
    return this->coins;
}

bool Player::isAlive() const
{
    return this->alive;
}

shared_ptr<Role> Player::getRole() const
{
    return this->role;
}

bool &Player::isTurn()
{
    return this->isturn;
}

bool &Player::getisSanctioned()
{
    return this->isSanctioned;
}

bool &Player::getisAlive()
{
    return this->alive;
}

bool &Player::isBlockedCoup()
{
    return this->blockCoup;
}
bool &Player::isArrestBlocked()
{
    return this->arrestBlocked;
}
bool &Player::getMustCoup()
{
    return this->mustCoup;
}
void Player::addCoins(int amount)
{
    this->coins += amount;
    if (coins >= 10 && !mustCoup) {
        mustCoup = true;
        std::cout << name << " now has " << coins << " coins and MUST coup in the next turn!\n";
    }
}

void Player::decreaceCoins(int amount)
{
    this->coins -= amount;
}


void Player::gatherAction(Game &game)
{
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    if (getisSanctioned()) {
        throw std::invalid_argument(name + " is sanctioned and cannot gather!");
    }
    try {
        this->role->gather(*this, game);
        game.nextTurn();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Gather failed: ") + e.what());
    }
}

void Player::taxAction(Game &game)
{
    if (this->isSanctioned)
    {
        throw std::invalid_argument("Player is sanctioned, cannot use tax.");
    }
    if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }
    
    shared_ptr<Player> ss;
    for (auto &p : game.getPlayers())
    {
        if (p.get() == this)
        {
            ss = p;
            break;
        }
    }

    this->role->tax(ss, game);
    game.nextTurn();
}

void Player::coupAction(shared_ptr<Player>& target, Game &game)
{
    try{
        this->role->coup(*this, target, game);
    } catch(const std::exception& e) {
        throw std::runtime_error(std::string("Coup failed: ") + e.what());
    }
    mustCoup =false;
    game.nextTurn();
}

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

void Player::bribeAction(Game &game)
{
     if (mustCoup) {
        throw std::invalid_argument(name + " has 10+ coins and must coup - cannot perform other actions!");
    }

    shared_ptr<Player> ss;
    for (auto &p : game.getPlayers())
    {
        if (p.get() == this)
        {
            ss = p;
            break;
        }
    }

    try {
        this->role->bribe(ss, game);
        // אל תקרא ל-nextTurn()! השחקן אמור לקבל תור נוסף
        std::cout << this->getName() << " used bribe and gets to play again!\n";
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Bribe failed: ") + e.what());
    }
}

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
