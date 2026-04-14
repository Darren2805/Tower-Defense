#ifndef BIGENEMY_HPP
#define BIGENEMY_HPP
#include "Enemy.hpp"

class BigEnemy : public Enemy
{
public:
    BigEnemy(int x, int y);
    void Hit(float damage) override;
};
#endif