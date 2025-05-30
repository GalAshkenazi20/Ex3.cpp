//galahkena1@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

/**
 * Main test function using doctest framework.
 * Tests all game functionality and edge cases.
 */

/**
 * Tests basic game functionality.
 * Adding players, turn management, and basic rules.
 */
TEST_CASE("Game Basic Functionality") {
    SUBCASE("Add players to game") {
        Game game;
        auto player1 = std::make_shared<Player>("Alice");
        auto player2 = std::make_shared<Player>("Bob");
        
        game.addPlayer(player1);
        game.addPlayer(player2);
        
        CHECK(game.getPlayers().size() == 2);
        CHECK(game.currentPlayerTurn()->getName() == "Alice");
    }
    
    SUBCASE("Cannot add duplicate players") {
        Game game;
        auto player1 = std::make_shared<Player>("Alice");
        
        game.addPlayer(player1);
        CHECK_THROWS_AS(game.addPlayer(player1), std::invalid_argument);
    }
}

TEST_CASE("Player Basic Actions") {
    Game game;
    auto player = std::make_shared<Player>("TestPlayer",std::make_shared<Spy>());
    game.addPlayer(player);
    
    SUBCASE("Gather action") {
        int initialCoins = player->getCoins();
        player->gatherAction(game);
        CHECK(player->getCoins() == initialCoins + 1);
    }
    
    SUBCASE("Tax action") {
        int initialCoins = player->getCoins();
        player->taxAction(game);
        CHECK(player->getCoins() == initialCoins + 2);
    }
    
    SUBCASE("Cannot gather when sanctioned") {
        player->getisSanctioned() = true;
        CHECK_THROWS_AS(player->gatherAction(game), std::invalid_argument);
    }
    
    SUBCASE("Cannot tax when sanctioned") {
        player->getisSanctioned() = true;
        CHECK_THROWS_AS(player->taxAction(game), std::invalid_argument);
    }
}

TEST_CASE("10+ Coins Must Coup Rule") {
    Game game;
    auto player1 = std::make_shared<Player>("Rich");
    auto player2 = std::make_shared<Player>("Target");
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    SUBCASE("Must coup with 10+ coins") {
        player1->addCoins(10);
        player1->getMustCoup() = true;
        
        CHECK_THROWS_AS(player1->gatherAction(game), std::invalid_argument);
        CHECK_THROWS_AS(player1->taxAction(game), std::invalid_argument);
        CHECK_THROWS_AS(player1->bribeAction(game), std::invalid_argument);
        CHECK_THROWS_AS(player1->arrestAction(player2, game), std::invalid_argument);
        CHECK_THROWS_AS(player1->sanctionAction(player2, game), std::invalid_argument);
    }
    
    SUBCASE("Can coup with 10+ coins") {
        player1->addCoins(10);
        player1->getMustCoup() = true;
        
        CHECK_NOTHROW(player1->coupAction(player2, game));
        CHECK_FALSE(player2->isAlive());
        CHECK_FALSE(player1->getMustCoup());
    }
}

TEST_CASE("Baron Specific Tests") {
    Game game;
    auto baron = std::make_shared<Player>("Baron", std::make_shared<Baron>());
    auto other = std::make_shared<Player>("Other");
    game.addPlayer(baron);
    game.addPlayer(other);
    
    SUBCASE("Baron invest action") {
        baron->addCoins(3);
        baron->investAction(game);
        CHECK(baron->getCoins() == 6); // pays 3, gets 6, net +3
    }
    
    SUBCASE("Baron cannot invest without 3 coins") {
        std::cout<<"Baron:" <<baron->getCoins()<<std::endl;
        baron->decreaceCoins(5);
        CHECK_THROWS_AS(baron->investAction(game), std::runtime_error);
    }
    
    SUBCASE("Baron cannot invest when sanctioned") {
        baron->addCoins(5);
        baron->getisSanctioned() = true;
        CHECK_THROWS_AS(baron->investAction(game), std::runtime_error);
    }
}

TEST_CASE("Merchant Specific Tests") {
    Game game;
    auto merchant = std::make_shared<Player>("Merchant", std::make_shared<Merchant>());
    auto other = std::make_shared<Player>("Other");
    game.addPlayer(merchant);
    game.addPlayer(other);
    
    SUBCASE("Merchant gets extra coin with 3+ coins at turn start") {
        merchant->addCoins(3);
        int initialCoins = merchant->getCoins();
        
        auto merchantRole = std::dynamic_pointer_cast<Merchant>(merchant->getRole());
        merchantRole->extraCoin(*merchant, game);
        
        CHECK(merchant->getCoins() == initialCoins + 1);
    }
    
    SUBCASE("Merchant doesn't get extra coin with < 3 coins") {
        merchant->addCoins(2);
        int initialCoins = merchant->getCoins();
        
        auto merchantRole = std::dynamic_pointer_cast<Merchant>(merchant->getRole());
        merchantRole->extraCoin(*merchant, game);
        
        CHECK(merchant->getCoins() == initialCoins);
    }
    
    SUBCASE("Merchant pays 2 to pool when arrested") {
        merchant->addCoins(5);
        other->addCoins(2);
        
        int merchantInitialCoins = merchant->getCoins();
        int otherInitialCoins = other->getCoins();
        int poolInitial = game.getCoinPool();
        
        other->arrestAction(merchant, game);
        
        CHECK(merchant->getCoins() == merchantInitialCoins - 2);
        CHECK(other->getCoins() == otherInitialCoins); // Other doesn't gain
        CHECK(game.getCoinPool() == poolInitial + 2);
    }
}

TEST_CASE("General Specific Tests") {
    Game game;
    auto general = std::make_shared<Player>("General", std::make_shared<General>());
    auto attacker = std::make_shared<Player>("Attacker");
    auto target = std::make_shared<Player>("Target");
    game.addPlayer(general);
    game.addPlayer(attacker);
    game.addPlayer(target);
    
    SUBCASE("General can prevent coup") {
        general->addCoins(5);
        attacker->addCoins(7);
        
        auto generalRole = std::dynamic_pointer_cast<General>(general->getRole());
        generalRole->preventCoup(*general, target, game);
        
        CHECK(target->isBlockedCoup());
        CHECK(general->getCoins() == 0);
    }
    
    SUBCASE("General cannot prevent coup without 5 coins") {
        general->addCoins(4);
        
        auto generalRole = std::dynamic_pointer_cast<General>(general->getRole());
        CHECK_THROWS_AS(generalRole->preventCoup(*general, target, game), std::invalid_argument);
    }
    
    SUBCASE("Coup fails against protected player") {
        general->addCoins(5);
        attacker->addCoins(7);
        
        auto generalRole = std::dynamic_pointer_cast<General>(general->getRole());
        generalRole->preventCoup(*general, target, game);
        
        int attackerInitialCoins = attacker->getCoins();
        attacker->coupAction(target, game);
        
        CHECK(target->isAlive()); // Target survives
        CHECK(attacker->getCoins() == attackerInitialCoins - 7); // Attacker loses coins
        CHECK_FALSE(target->isBlockedCoup()); // Protection is consumed
    }
    
    SUBCASE("General gets back coin when arrested") {
        general->addCoins(3);
        attacker->addCoins(2);
        
        int generalInitialCoins = general->getCoins();
        int poolInitial = game.getCoinPool();
        
        attacker->arrestAction(general, game);
        
        CHECK(general->getCoins() == generalInitialCoins); // Loses 1, gains 1 back
        CHECK(game.getCoinPool() == poolInitial - 1); // Pool loses 1 for compensation
    }
}

TEST_CASE("Spy Specific Tests") {
    Game game;
    auto spy = std::make_shared<Player>("Spy", std::make_shared<Spy>());
    auto target = std::make_shared<Player>("Target");
    auto arrester = std::make_shared<Player>("Arrester");
    game.addPlayer(spy);
    game.addPlayer(target);
    game.addPlayer(arrester);
    
    SUBCASE("Spy can see target coins") {
        target->addCoins(5);
        
        auto spyRole = std::dynamic_pointer_cast<Spy>(spy->getRole());
        CHECK_NOTHROW(spyRole->seeTargetCoins(*spy, target));
    }
    
    SUBCASE("Spy can block arrest") {
        auto spyRole = std::dynamic_pointer_cast<Spy>(spy->getRole());
        spyRole->blockArrest(target);
        
        CHECK(target->isArrestBlocked());
    }
    
    SUBCASE("Cannot arrest blocked player") {
        target->isArrestBlocked() = true;
        arrester->addCoins(2);
        
        CHECK_THROWS_AS(arrester->arrestAction(target, game), std::runtime_error);
    }
}

TEST_CASE("Governor Specific Tests") {
    Game game;
    auto governor = std::make_shared<Player>("Governor", std::make_shared<Governor>());
    auto other = std::make_shared<Player>("Other", std::make_shared<Governor>());
    game.addPlayer(governor);
    game.addPlayer(other);
    
    SUBCASE("Governor takes 3 coins with tax") {
        int initialCoins = governor->getCoins();
        governor->getRole()->tax(governor, game);
        CHECK(governor->getCoins() == initialCoins + 3);
    }
    
    SUBCASE("Governor can cancel tax") {
        other->addCoins(5);
        other->taxAction(game);
        game.addPendingTax(other);
        
        int otherCoinsAfterTax = other->getCoins();
        int poolAfterTax = game.getCoinPool();
        
        governor->getRole()->cancelTax(*other, other, game);
        
        CHECK(other->getCoins() == otherCoinsAfterTax - 3); // Returns 3 coins
        CHECK(game.getCoinPool() == poolAfterTax + 3);
        CHECK_FALSE(game.isTaxPending(other));
    }
}

TEST_CASE("Judge Specific Tests") {
    Game game;
    auto judge = std::make_shared<Player>("Judge", std::make_shared<Judge>());
    auto other = std::make_shared<Player>("Other");
    game.addPlayer(judge);
    game.addPlayer(other);
    
    SUBCASE("Judge can cancel bribe") {
        other->addCoins(5);
        other->bribeAction(game);
        game.addPendingBribe(other);
        
        auto judgeRole = std::dynamic_pointer_cast<Judge>(judge->getRole());
        judgeRole->cancelBribe(*judge, other, game);
        
        CHECK_FALSE(game.isBribePending(other));
    }
    
    SUBCASE("Judge gets extra coin when sanctioned") {
        judge->addCoins(5);
        other->addCoins(5);
        
        int judgeInitialCoins = judge->getCoins();
        int otherInitialCoins = other->getCoins();
        
        other->sanctionAction(judge, game);
        CHECK(other->getCoins() == otherInitialCoins - 4); // Pays extra 1 coin
    }
}

TEST_CASE("Arrest Edge Cases") {
    Game game;
    auto player1 = std::make_shared<Player>("Player1");
    auto player2 = std::make_shared<Player>("Player2");
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    SUBCASE("Cannot arrest yourself") {
        player1->addCoins(2);
        CHECK_THROWS_AS(player1->arrestAction(player1, game), std::runtime_error);
    }
    
    SUBCASE("Cannot arrest dead player") {
        player2->getisAlive() = false;
        CHECK_THROWS_AS(player1->arrestAction(player2, game), std::runtime_error);
    }
    
    SUBCASE("Cannot arrest player with no coins") {
        player2->decreaceCoins(player2->getCoins()); // Remove all coins
        CHECK_THROWS_AS(player1->arrestAction(player2, game), std::runtime_error);
    }
}

TEST_CASE("Coup Edge Cases") {
    Game game;
    auto player1 = std::make_shared<Player>("Player1");
    auto player2 = std::make_shared<Player>("Player2");
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    SUBCASE("Cannot coup without 7 coins") {
        player1->addCoins(6);
        CHECK_THROWS_AS(player1->coupAction(player2, game), std::runtime_error);
    }
    
    SUBCASE("Cannot coup yourself") {
        player1->addCoins(7);
        CHECK_THROWS_AS(player1->coupAction(player1, game), std::runtime_error);
    }
    
    SUBCASE("Cannot coup dead player") {
        player1->addCoins(7);
        player2->getisAlive() = false;
        CHECK_THROWS_AS(player1->coupAction(player2, game), std::runtime_error);
    }
    
    SUBCASE("Successful coup kills target") {
        player1->addCoins(7);
        player1->coupAction(player2, game);
        
        CHECK_FALSE(player2->isAlive());
        CHECK(player1->getCoins() == 0);
    }
}

TEST_CASE("Sanction Edge Cases") {
    Game game;
    auto player1 = std::make_shared<Player>("Player1");
    auto player2 = std::make_shared<Player>("Player2");
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    SUBCASE("Cannot sanction without 3 coins") {
        player1->addCoins(2);
        CHECK_THROWS_AS(player1->sanctionAction(player2, game), std::runtime_error);
    }
    
    SUBCASE("Cannot sanction yourself") {
        player1->addCoins(3);
        CHECK_THROWS_AS(player1->sanctionAction(player1, game), std::runtime_error);
    }
    
    SUBCASE("Cannot sanction dead player") {
        player1->addCoins(3);
        player2->getisAlive() = false;
        CHECK_THROWS_AS(player1->sanctionAction(player2, game), std::runtime_error);
    }
    
    SUBCASE("Successful sanction") {
        player1->addCoins(3);
        player1->sanctionAction(player2, game);
        
        CHECK(player2->getisSanctioned());
        CHECK(player1->getCoins() == 0);
    }
}

TEST_CASE("Bribe Tests") {
    Game game;
    auto player = std::make_shared<Player>("Player");
    game.addPlayer(player);
    
    SUBCASE("Cannot bribe without 4 coins") {
        player->addCoins(3);
        CHECK_THROWS_AS(player->bribeAction(game), std::runtime_error);
    }
    
    SUBCASE("Successful bribe") {
        player->addCoins(4);
        player->bribeAction(game);
        
        CHECK(player->getCoins() == 0);
    }
    
    SUBCASE("Cannot perform actions when must coup") {
        player->addCoins(10);
        player->getMustCoup() = true;
        
        CHECK_THROWS_AS(player->bribeAction(game), std::invalid_argument);
    }
}

/**
 * Tests game state management.
 * Turn transitions and status resets.
 */
TEST_CASE("Game State Management") {
    Game game;
    auto player1 = std::make_shared<Player>("Player1");
    auto player2 = std::make_shared<Player>("Player2");
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    SUBCASE("Turn transitions") {
        CHECK(game.currentPlayerTurn() == player1);
        game.nextTurn();
        CHECK(game.currentPlayerTurn() == player2);
        game.nextTurn();
        CHECK(game.currentPlayerTurn() == player1);
    }
    
    SUBCASE("Sanctioned status clears on turn start") {
        player1->getisSanctioned() = true;
        game.nextTurn(); // Move to player2
        game.nextTurn(); // Back to player1
        CHECK_FALSE(player1->getisSanctioned());
    }
    
    SUBCASE("Arrest block clears on turn start") {
        player1->isArrestBlocked() = true;
        game.nextTurn(); // Move to player2
        game.nextTurn(); // Back to player1
        CHECK_FALSE(player1->isArrestBlocked());
    }
}