#include "Proiectil.h"

#include <Core/Engine.h>

Proiectil::Proiectil()
{
}

Proiectil::~Proiectil()
{
}

void Proiectil::setincx(float x)
{
	incx = x;
}

void Proiectil::setincy(float y)
{
	incy = y;
}

float Proiectil::speed = 800.0f;

Proiectil::Proiectil(float x, float y)
{
	this->x = x;
	this->y = y;
	initx = x;
	inity = y;
}