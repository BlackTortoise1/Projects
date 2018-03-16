#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>


class Proiectil
{
public:
	static float speed;
	float x, y, initx, inity, incx, incy;
	Proiectil();
	~Proiectil();
	void setincx(float x);
	void setincy(float y);
	Proiectil(float x, float y);

};