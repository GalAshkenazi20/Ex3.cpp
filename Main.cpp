#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <set>      // for std::set
#include <unistd.h> // for sleep
#include "Game.hpp"
#include "Player.hpp"
#include "Spy.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Baron.hpp"
#include "Merchant.hpp"
#include "Judge.hpp"

using namespace std;

void printFunction(shared_ptr<Player> p, Game g)
{
    cout << "Player Name: " << p->getName() << endl;
    cout << "Player Coins: " << p->getCoins() << endl;
    if (p->isAlive())
    {
        cout << "is Alive ? true\n";
    }
    else
    {
        cout << "is Alive ? false\n";
    }
}

string printROLE(shared_ptr<Player> p)
{
    if (p->getRole()->name() == "General")
    {
        return " General";
    }
    else if (p->getRole()->name() == "Baron")
    {
        return "Baron";
    }
    else if (p->getRole()->name() == "Governor")
    {
        return "Governor";
    }
    else if (p->getRole()->name() == "Judge")
    {
        return "Judge";
    }
    else if (p->getRole()->name() == "Merchant")
    {
        return "Merchant";
    }
    else
    {
        return "Spy";
    }
}

int main()
{
    try {
        cout << "===== COUP GAME DEMONSTRATION =====\n";

        // Initialize the game
        Game game;

        // Create players with different names
        auto p1 = make_shared<Player>("Alice");
        auto p2 = make_shared<Player>("Bob");
        auto p3 = make_shared<Player>("Charlie");
        auto p4 = make_shared<Player>("Diana");

        // Add players to the game
        cout << "Adding players to the game...\n";
        game.addPlayer(p1);
        game.addPlayer(p2);
        game.addPlayer(p3);
        game.addPlayer(p4);

        for (auto p : game.getPlayers())
        {
            cout << "Active Players : " << p->getName() << " || role: " << printROLE(p) << endl;
        }
        cout << "\n";
        
        cout << game.currentPlayerTurn()->getName() << " is gathering coins...\n";
        game.currentPlayerTurn()->gatherAction(game);
        printFunction(p1, game);

        cout << game.currentPlayerTurn()->getName() << " is gathering coins...\n";
        game.currentPlayerTurn()->gatherAction(game);
        printFunction(p2, game);

        cout << game.currentPlayerTurn()->getName() << " is collecting tax...\n";
        game.currentPlayerTurn()->taxAction(game);
        printFunction(p3, game);

        cout << "---- BRIBE ACTION ----\n";
        cout << "Current player: " << game.currentPlayerTurn()->getName() 
             << " with " << game.currentPlayerTurn()->getCoins() << " coins\n";
        
        if (game.currentPlayerTurn()->getCoins() >= 4) {
            game.currentPlayerTurn()->bribeAction(game);
        } else {
            cout << game.currentPlayerTurn()->getName() << " doesn't have enough coins for bribe (has " 
                 << game.currentPlayerTurn()->getCoins() << ", needs 4)\n";
            cout << "Adding 4 coins to " << game.currentPlayerTurn()->getName() << " for demonstration...\n";
            game.currentPlayerTurn()->addCoins(4);
            game.currentPlayerTurn()->bribeAction(game);
        }

        cout << game.currentPlayerTurn()->getName() << endl;

        // תיקון בעיית מעצר עצמי
        auto arrestTarget = (game.currentPlayerTurn() == p1) ? p2 : p1;
        if (arrestTarget->getCoins() == 0) {
            arrestTarget->addCoins(2);
        }
        
        cout << game.currentPlayerTurn()->getName() << " is arresting " 
             << arrestTarget->getName() << " to steal 1 coin...\n";
        game.currentPlayerTurn()->arrestAction(arrestTarget, game);

        printFunction(p1, game);
        printFunction(p4, game);

        cout << game.currentPlayerTurn()->getName() << " using Tax\n";
        game.currentPlayerTurn()->taxAction(game);

        cout << game.currentPlayerTurn()->getName() << " using gather\n";
        game.currentPlayerTurn()->gatherAction(game);
        cout << "\n";
        
        cout << "---- adding 5 coins each player---\n";
        for (auto player : game.getPlayers())
        {
            player->addCoins(5);
        }

        cout << "\n";
        cout << "---- game players coins status //after adding 5 coins---\n";
        printFunction(p1, game);
        printFunction(p2, game);
        printFunction(p3, game);
        printFunction(p4, game);
        cout << "-------------------------------------------------\n";
        cout << "\n";

        auto sanctionTarget = (game.currentPlayerTurn() == p4) ? p3 : p4;
        cout << game.currentPlayerTurn()->getName() << " sanctioning " 
             << sanctionTarget->getName() << "\n";
        game.currentPlayerTurn()->sanctionAction(sanctionTarget, game);
        cout << game.currentPlayerTurn()->getName() << " coins after sanctioning --> " 
             << game.currentPlayerTurn()->getCoins() << endl;

        cout << sanctionTarget->getName() << " trying using tax\n";
        try {
            sanctionTarget->taxAction(game);
        } catch (const std::exception& e) {
            cout << "Tax failed (expected - sanctioned): " << e.what() << "\n";
        }

        cout << "check if " << sanctionTarget->getName() << " sanction removed\n";
        if (sanctionTarget->getisSanctioned())
        {
            cout << "still sanctioned\n";
        }
        else
        {
            cout << "not sanctioned, code ok!\n";
        }
        
        // וידוא כסף מספיק ל-coup
        if (game.currentPlayerTurn()->getCoins() < 7) {
            game.currentPlayerTurn()->addCoins(7 - game.currentPlayerTurn()->getCoins());
        }
        
        auto coupTarget = (game.currentPlayerTurn() == p4) ? p3 : p4;
        cout << game.currentPlayerTurn()->getName() << " couping " << coupTarget->getName() << "\n";
        game.currentPlayerTurn()->coupAction(coupTarget, game);
        
        cout << "---check if " << coupTarget->getName() << " dead(couped)---\n";
        if (!coupTarget->isAlive())
        {
            cout << coupTarget->getName() << " dead\n";
            cout << game.currentPlayerTurn()->getName() << " coins: " 
                 << game.currentPlayerTurn()->getCoins() << endl;
        }
        else
        {
            cout << coupTarget->getName() << " alive , code not ok!\n";
        }
        
        // המשך עד שנשאר רק שחקן אחד
        cout << "\n=== ENSURING WINNER ===\n";
        while (game.activePlayerNames().size() > 1) {
            auto currentPlayer = game.currentPlayerTurn();
            
            // מצא מטרה חיה
            shared_ptr<Player> target = nullptr;
            for (auto player : game.getPlayers()) {
                if (player->isAlive() && player != currentPlayer) {
                    target = player;
                    break;
                }
            }
            
            if (target) {
                // וודא כסף מספיק ל-coup
                if (currentPlayer->getCoins() < 7) {
                    currentPlayer->addCoins(7 - currentPlayer->getCoins());
                }
                cout << currentPlayer->getName() << " couping " << target->getName() << "\n";
                currentPlayer->coupAction(target, game);
            } else {
                break;
            }
        }

        cout << "\n---Checking Active Players---\n";
        for (auto str : game.activePlayerNames())
        {
            cout << str << endl;
        }
        cout << "-----------------\n";

        cout << "The winner is..." << game.winner() << endl;

    } catch (const std::exception& e) {
        cout << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    cout << "\nProgram completed successfully!" << endl;
    return 0;
}
