#ifndef ENEMY5_HPP
#define ENEMY5_HPP
#include "Enemy.hpp"

class Enemy5 : public Enemy
{
public:
    Enemy5(int x, int y);
    void Hit(float damage) override;
};
#endif // SOLDIERENEMY_HPP
