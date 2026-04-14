#include <string>

#include "BigEnemy.hpp"
#include "../Scene/PlayScene.hpp"
#include "Enemy5.hpp"
#include "Engine/AudioHelper.hpp"
#include "Turret/Turret.hpp"
#include "Bullet/Bullet.hpp"

BigEnemy::BigEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 20, 10, 200, 250, 80)
{
}

void BigEnemy::Hit(float damage)
{
    hp -= damage;
    if (hp <= 0)
    {
        UpdateScore();
        getPlayScene()->UpdateScoreText();
        OnExplode();

        Enemy *enemy = new Enemy5(Position.x, Position.y);
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