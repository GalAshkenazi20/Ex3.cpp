#pragma once

#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

#include <random>
#include <stdexcept>

Player::Player(const std::string &name)
: name(name), coins(0), alive(true)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 5);

    int r = dist(gen);
    switch (r) {
        case 0: role = std::make_shared<Governor>(); break;
        case 1: role = std::make_shared<Spy>(); break;
        case 2: role = std::make_shared<Baron>(); break;
        case 3: role = std::make_shared<General>(); break;
        case 4: role = std::make_shared<Judge>(); break;
        case 5: role = std::make_shared<Merchant>(); break;
        default: throw std::runtime_error("Invalid role random index");
    }
}