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
    : name(name), coins(0), alive(true), isturn(false), isArrest(false)
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


    Player::Player(const std::string& name, std::shared_ptr<Role> chosenRole)
    : name(name), coins(0), alive(true), isArrest(false), isSanctioned(false), role(chosenRole) {}

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
bool &Player::isArrested()
{
    return this->isArrest;
}
bool &Player::isBlockedCoup()
{
    return this->blockCoup;
}
void Player::addCoins(int amount)
{
    this->coins += amount;
}

void Player::decreaceCoins(int amount)
{
    this->coins -= amount;
}


void Player::gatherAction(Game &game)
{
    this->role->gather(*this, game);
    game.nextTurn();
}

void Player::taxAction(Game &game)
{
    if (this->isSanctioned)
    {
        std::cout << "is sanctioned, cannot use tax.\n";
        game.nextTurn();
        return;
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
        std::cout << "Coup failed: " << e.what() << "\n";
    }
    game.nextTurn();
}

void Player::arrestAction(shared_ptr<Player>& target, Game &game)
{
this->role->arrest(*this, target, game);
}

void Player::bribeAction(Game &game)
{
    shared_ptr<Player> ss;
    for (auto &p : game.getPlayers())
    {
        if (p.get() == this)
        {
            ss = p;
            break;
        }
    }
    try
    {
        this->role->bribe(ss, game);
        std::cout << "Bribe successful!\n";
    }
    catch(const std::exception& e)
    {
         std::cout << "Bribe failed: " << e.what() << "\n";
    }
    game.nextTurn();
}

void Player::sanctionAction(shared_ptr<Player>& target, Game &game)
{
    this->role->sanction(*this, target, game);
    target->getRole()->onSanctioned(*this, game);
    game.nextTurn();
}
