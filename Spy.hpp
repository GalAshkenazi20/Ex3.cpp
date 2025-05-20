#include "Role.hpp"

class Spy : public Role
{
    virtual void arrest(Player& p, shared_ptr<Player> target, Game& g) override;
};