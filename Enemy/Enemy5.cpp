#include <string>

#include "Enemy.hpp"
#include "Enemy5.hpp"
#include "../Scene/PlayScene.hpp"
#include "PlaneEnemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Turret/Turret.hpp"
#include "Bullet/Bullet.hpp"

Enemy5::Enemy5(int x, int y) : Enemy("play/enemy-5.png", x, y, 10, 150, 50, 100, 70)
{
    // TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
}

void Enemy5::Hit(float damage)
{
    hp -= damage;
    if (hp <= 0)
    {
        UpdateScore();
        getPlayScene()->UpdateScoreText();
        OnExplode();

        Enemy *enemy = new PlaneEnemy(Position.x, Position.y);
        getPlayScene()->EnemyGroup->AddNewObject(enemy);
        enemy->UpdatePath(getPlayScene()->mapDistance);

        // Remove all turret's reference to target.
        for (auto &it : lockedTurrets)
            it->Target = nullptr;
        for (auto &it : lockedBullets)
            it->Target = nullptr;
        getPlayScene()->EarnMoney(money);
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);

        AudioHelper::PlayAudio("explosion.wav");
    }
}
