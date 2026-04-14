#ifndef SMALLBULLET_HPP
#define SMALLBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine
{
    struct Point;
} // namespace Engine

class SmallBullet : public Bullet
{
public:
    explicit SmallBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
};
#endif // FIREBULLET_HPP
