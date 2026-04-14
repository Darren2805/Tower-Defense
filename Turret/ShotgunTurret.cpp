#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Bullet/FireBullet.hpp"
#include "ShotgunTurret.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int ShotgunTurret::Price = 250;
ShotgunTurret::ShotgunTurret(float x, float y) : Turret("play/tower-base.png", "play/turret-7.png", x, y, 700, Price, 1)
{
    Anchor.y += 8.0f / GetBitmapHeight();
}

void ShotgunTurret::CreateBullet()
{
    float delta = 0.1;
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));

    Engine::Point diff_l1 = Engine::Point(cos(Rotation - ALLEGRO_PI / 2 - delta), sin(Rotation - ALLEGRO_PI / 2 - delta));
    Engine::Point diff_l2 = Engine::Point(cos(Rotation - ALLEGRO_PI / 2 - 2 * delta), sin(Rotation - ALLEGRO_PI / 2 - 2 * delta));

    Engine::Point diff_r1 = Engine::Point(cos(Rotation - ALLEGRO_PI / 2 + delta), sin(Rotation - ALLEGRO_PI / 2 + delta));
    Engine::Point diff_r2 = Engine::Point(cos(Rotation - ALLEGRO_PI / 2 + 2 * delta), sin(Rotation - ALLEGRO_PI / 2 + 2 * delta));

    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);

    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position + normalized * 36, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position + normalized * 36 - normal * 2, diff_l1, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position + normalized * 36 - normal * 4, diff_l2, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position + normalized * 36 + normal * 2, diff_r1, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position + normalized * 36 + normal * 4, diff_r2, rotation, this));

    // getPlayScene()->BulletGroup->AddNewObject(new SmallBullet(Position + normalized * 36 - normal * 2, diff, rotation, this));
    // getPlayScene()->BulletGroup->AddNewObject(new SmallBullet(Position + normalized * 36 - normal * 2, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}