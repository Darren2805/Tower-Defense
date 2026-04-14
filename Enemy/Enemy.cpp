#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include <iostream>

#include "Engine/AudioHelper.hpp"
#include "Bullet/Bullet.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Shared/shared.hpp"

PlayScene *Enemy::getPlayScene()
{
	return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void Enemy::UpdateScore()
{
	int enemyDeathDist = getPlayScene()->mapDistance[(int)(Position.y / getPlayScene()->BlockSize)][(int)(Position.x / getPlayScene()->BlockSize)];
	/*
		min1, max1, min2, max2, x1, x2

		(max1 - x1) / (max1 - min1) = (max2 - x2) / (max2 - min2)
		x2 =  max2 - (max1 - x1) * (max2 - min2) / (max1 - min1)
	*/
	float multiplier = 10 - (getPlayScene()->mapMaxDist - enemyDeathDist) * 9.0 / getPlayScene()->mapMaxDist;

	gameScore += multiplier * scoreWorth;
}

void Enemy::OnExplode()
{

	getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
	for (int i = 0; i < 10; i++)
	{
		// Random add 10 dirty effects.
		getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
	}
}
Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money, int scoreWorth) : Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money), scoreWorth(scoreWorth)
{
	CollisionRadius = radius;
	reachEndTime = 0;
}
void Enemy::Hit(float damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		UpdateScore();
		getPlayScene()->UpdateScoreText();
		OnExplode();
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
void Enemy::UpdatePath(const std::vector<std::vector<int>> &mapDistance)
{
	// std::cout << "UpdatePath\n";
	int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
	int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
	if (x < 0)
		x = 0;
	if (x >= PlayScene::MapWidth)
		x = PlayScene::MapWidth - 1;
	if (y < 0)
		y = 0;
	if (y >= PlayScene::MapHeight)
		y = PlayScene::MapHeight - 1;

	// printf("Up to this point x is 0: %d\n", x);

	Engine::Point pos(x, y);
	int num = mapDistance[y][x];
	if (num == -1)
	{
		num = 0;
		Engine::LOG(Engine::ERROR) << "Enemy path finding error";
	}

	path = std::vector<Engine::Point>(num + 1);
	while (num != 0)
	{
		// std::cout << "num: " << num << std::endl;
		std::vector<Engine::Point> nextHops;
		// std::cout << "OK?\n";
		for (auto &dir : PlayScene::directions)
		{
			// std::cout << "Here?\n";
			// printf("%d\n", 0.0 + 0.0);

			int _x = pos.x + dir.x;
			// printf("X? %d = %f + %f\n", _x, pos.x, dir.x);
			int _y = pos.y + dir.y;
			// printf("Y? %d = %f + %f\n", _y, pos.y, dir.y);

			// printf("[%d, %d]\n", _y, _x);

			// printf("[%d, %d], %d | %d\n", _y, _x, mapDistance[_y][_x], num);
			// if (_x < 0)
			// std::cout << "x lt 0\n";
			// if (_x >= PlayScene::MapWidth)
			// std::cout << "x gte mapwidth\n";
			// if (_y < 0)
			// std::cout << "y lt 0\n";
			// if (_y >= PlayScene::MapHeight)
			// std::cout << "y gte mapheight\n";
			// if (mapDistance[_y][_x] != num - 1)
			// std::cout << "mapdist != num - 1\n";
			if (_x < 0 || _x >= PlayScene::MapWidth || _y < 0 || _y >= PlayScene::MapHeight || mapDistance[_y][_x] != num - 1)
				continue;

			// std::cout << "emplace\n";
			nextHops.emplace_back(_x, _y);
		}

		// std::cout << "beep\n";
		// Choose arbitrary one.
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
		// std::cout << "yeet\n";
		// std::cout << "rng: " << rng << "\n";
		// std::cout << "d: " << dist(rng) << " | size: " << nextHops.size() << std::endl;
		pos = nextHops[dist(rng)];
		path[num] = pos;
		num--;
		// std::cout << "next\n";
	}

	path[0] = PlayScene::EndGridPoint;
}
void Enemy::Update(float deltaTime)
{
	// Pre-calculate the velocity.
	float remainSpeed = speed * deltaTime;
	while (remainSpeed != 0)
	{
		if (path.empty())
		{
			// Reach end point.
			Hit(hp);
			getPlayScene()->Hit();
			reachEndTime = 0;
			return;
		}
		Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
		Engine::Point vec = target - Position;
		// Add up the distances:
		// 1. to path.back()
		// 2. path.back() to border
		// 3. All intermediate block size
		// 4. to end point
		reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
		Engine::Point normalized = vec.Normalize();
		if (remainSpeed - vec.Magnitude() > 0)
		{
			Position = target;
			path.pop_back();
			remainSpeed -= vec.Magnitude();
		}
		else
		{
			Velocity = normalized * remainSpeed / deltaTime;
			remainSpeed = 0;
		}
	}
	Rotation = atan2(Velocity.y, Velocity.x);
	Sprite::Update(deltaTime);
}
void Enemy::Draw() const
{
	Sprite::Draw();
	if (PlayScene::DebugMode)
	{
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
	}
}
