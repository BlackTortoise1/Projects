#pragma once
#include "Laborator3_Vis2D.h"
class Enemy
{
public:

	int hp;
	float x, y, speed, scale;

	void change();

	Enemy();

	Enemy(int hp, float x, float y);

	~Enemy();

};
