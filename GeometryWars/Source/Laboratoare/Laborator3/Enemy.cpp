#include "Enemy.h"


Enemy::Enemy()
{
}

void Enemy::change()
{
	hp--;
	speed = 200.0f;
}


Enemy::Enemy(int hp, float x, float y)
{
	this->x = x;
	this->y = y;
	this->hp = hp;
	scale = (hp == 1 ? 1.2f : (hp == 2 ? 1.4f : 1.6f));

	speed = 100.0f;
}

Enemy::~Enemy()
{
}