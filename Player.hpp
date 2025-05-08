#include <iostream>

using namespace std;

class Player
{
private:
    string name;
    string role;
    int coins;
public:
    Player(string name, string role, int coins): name(name), role(role), coins(coins){}
    ~Player();
};

Player::Player(string name,string role,int coins)
{
    
}

Player::~Player()
{
}



