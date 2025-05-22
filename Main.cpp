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

    // Demonstrate Tax action
    cout << game.currentPlayerTurn()->getName() << " is collecting tax...\n";
    game.currentPlayerTurn()->taxAction(game);
    printFunction(p3, game);

    cout << "---- BRIBE ACTION ----\n";
    game.currentPlayerTurn()->bribeAction(game);

    cout << game.currentPlayerTurn()->getName() << endl;

    cout << p4->getName() << " is arresting " << game.currentPlayerTurn()->getName() << " to steal 1 coin...\n";

    auto arr = game.currentPlayerTurn();

    p4->arrestAction(arr, game);

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

    cout << " p3 = charlie , sanction p4 = diana \n";
    game.currentPlayerTurn()->sanctionAction(p4, game);
    cout << "charlie coins after sanctioned diana--> " << p3->getCoins() << endl;

    cout << "diana trying using tax\n";
    game.currentPlayerTurn()->taxAction(game);

    cout << "check if diana sacnction removed\n";

    if (p4->getisSanctioned())
    {
        cout << "still sanctioned\n";
    }
    else
    {
        cout << "not sanctioned, code ok!\n";
    }
    cout << game.currentPlayerTurn()->getName() << "couping p4(diana)\n";
    game.currentPlayerTurn()->coupAction(p4, game);
    cout << "---check if diana dead(couped)---\n";
    if (!p4->isAlive())
    {
        cout << "diana dead\n";
        cout << "alice coins: " << p1->getCoins() << endl;
    }
    else
    {
        cout << "diana alive , code not ok!\n";
    }
    cout << game.currentPlayerTurn()->getName() << " sanction p3(charlie)\n";
    game.currentPlayerTurn()->sanctionAction(p3, game);
    cout << game.currentPlayerTurn()->getName() << " choosee gather while sanctioned\n";
    cout << "charlie coins before: " << game.currentPlayerTurn()->getCoins() << endl;
    game.currentPlayerTurn()->gatherAction(game);
    cout << "charlie coins after: " << p3->getCoins() << endl;

    cout << "\n"
         << "after diana couped check if is alice turn(check if next turn work while someone coup)\n";
    cout << game.currentPlayerTurn()->getName() << endl;

    cout << "\n";
    cout << "---- adding 5 coins each player---\n";
    for (auto player : game.getPlayers())
    {
        player->addCoins(5);
    }

    cout << "\n";
    cout << "alice trying to coup charile while alice dont have enough money\n";
    game.currentPlayerTurn()->coupAction(p3, game);

    cout << "---check if charlie dead(couped)---\n";
    if (!p3->isAlive())
    {
        cout << "charlie dead\n";
        cout << "alice coins: " << p1->getCoins() << endl;
    }
    else
    {
        cout << "charlie alive\n";
    }
    cout << "alice coins: " << p1->getCoins() << endl;
    cout << "bob using gather\n";
    game.currentPlayerTurn()->gatherAction(game);

    cout<<"charlie coup bob\n";
    game.currentPlayerTurn()->coupAction(p2,game);
    cout<<"\n"<<"---Checking Active Players---\n";
    for(auto str : game.activePlayerNames())
    {
        cout<<str<<endl;
    }
    cout<<"-----------------\n";
    cout<<"checking if we have winner\n";
    try{
      game.winner();  
    }catch(const std::exception& e) 
    {
        cout << "No winner yet: " << e.what() << "\n";
    }
    
    cout<<"adding 7 coins to Alice to check winner\n";
    game.currentPlayerTurn()->addCoins(7);
    game.currentPlayerTurn()->coupAction(p3,game);
    
    cout<<"The winner is..."<<game.winner() << endl;
    game.gameReset();
    cout<<"\n\n\n";
    cout<<"------GAME NUMBER 2------\n\n";
    
    auto player1 = make_shared<Player>("Alice", make_shared<Governor>());
    auto player2 = make_shared<Player>("Bob", make_shared<Spy>());
    auto player3 = make_shared<Player>("Charlie", make_shared<Baron>());
    auto player4 = make_shared<Player>("Diana", make_shared<General>());
    auto player5 = make_shared<Player>("Gal", make_shared<Judge>());
    auto player6 = make_shared<Player>("Yossi", make_shared<Merchant>());

    // Add players to the game
    cout << "Adding players to the game...\n";
    game.addPlayer(player1);
    game.addPlayer(player2);
    game.addPlayer(player3);
    game.addPlayer(player4);
    game.addPlayer(player5);
    game.addPlayer(player6);

    for (auto p : game.getPlayers())
    {
        cout << "Active Players : " << p->getName() << " || role: " << printROLE(p) << endl;
    }
    cout<<"\n"<<"---Checking Active Players---\n";
    for(auto str : game.activePlayerNames())
    {
        cout<<str<<endl;
    }
    cout<<"-----------------\n";
    cout<<"alice collect tax and she need to get 3coins\n";
    game.currentPlayerTurn()->taxAction(game);
    cout<< "Alice coins: "<<player1->getCoins()<<endl;
    cout << "---- adding 5 coins each player---\n";
    for (auto player : game.getPlayers())
    {
        player->addCoins(5);
    }

    cout << "\n\n";
    cout<<"Check if Bob(Spy) can see other players coins\n";
    game.currentPlayerTurn()->getRole()->seeTargetCoins(*game.currentPlayerTurn(),player1);
    game.currentPlayerTurn()->getRole()->seeTargetCoins(*game.currentPlayerTurn(),player6);

    cout<<"test Spy function: prevent some player from use arrest\n";
    game.currentPlayerTurn()->getRole()->blockArrest(player3);
    game.currentPlayerTurn()->gatherAction(game);
    cout<<"--------\n";
    cout<<"current player: "<<game.currentPlayerTurn()->getName()<<endl;
    cout<<"player5 coins: "<<player5->getCoins()<<"\n"<<"player3 coins: " << player3->getCoins()<<endl;
    game.currentPlayerTurn()->arrestAction(player5,game);
    cout<<"player5 coins: "<<player5->getCoins()<<"\n"<<"player3 coins: " << player3->getCoins()<<endl;
    cout<<game.currentPlayerTurn()->getName()<< " turn"<<endl;
    //cout<<




}
