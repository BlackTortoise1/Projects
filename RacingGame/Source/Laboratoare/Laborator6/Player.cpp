#include "Player.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

Player::Player()
{
	Position = glm::vec3(0, 0.5, 0);
	Rotations = glm::vec3(0);
	Scale = glm::vec3(0.7, 0.5, 1.4);

	CarOffset[0] = glm::vec3(-0.4, -0.2, -0.5);
	CarOffset[1] = glm::vec3(0.4, -0.2, -0.5);
	CarOffset[2] = glm::vec3(-0.4, -0.2, 0.5);
	CarOffset[3] = glm::vec3(0.4, -0.2, 0.5);
}

Player::~Player()
{

}

void Player::ManageMovement(int key, bool Press)
{
	// Accelerare
	if (key == GLFW_KEY_UP)
	{
		if (Press)
			Accelerating = true;
		else
			Accelerating = false;
	}
	// Franare
	if (key == GLFW_KEY_DOWN)
	{
		if (Press)
			Breaking = true;
		else
			Breaking = false;
	}

	// Viraj dreapta
	if (key == GLFW_KEY_RIGHT)
	{
		if (Press)
			TurnDirection += 2;
		else
			TurnDirection -= 2;
	}
	// Viraj stanga
	if (key == GLFW_KEY_LEFT)
	{
		if (Press)
			TurnDirection += 1;
		else
			TurnDirection -= 1;
	}
}

void Player::Update(float deltaTimeSeconds)
{
	// Daca jucatorul a ajuns la sg hartii nu se mai poate misca
	if (StopMovement)
		return;

	// In functie de directia in care incearca sa vireze jucatorul se seteaza rotatia masinii
	if (TurnDirection == 1 && Rotations.y < MaxRotation)
		Rotations.y += deltaTimeSeconds;
	if (TurnDirection == 2 && Rotations.y > -MaxRotation)
		Rotations.y -= deltaTimeSeconds;

	// Daca jucatorul accelereza se mareste viteza masinii pana la MaxSpeedPerFrame
	if (Accelerating && Speed < MaxSpeedPerFrame)
		Speed += deltaTimeSeconds / 50;

	if (!Accelerating)
		Speed -= deltaTimeSeconds / 10;

	if (Breaking && Speed > 0)
		Speed -= deltaTimeSeconds;

	if (Speed < 0)
		Speed = 0;
	// Daca masina re viteza se realizeaza deplasarea aceteia
	if (Speed > 0)
	{
		MoveOX = true;
		MoveOZ = true;

		VerifyCollision();
		// Deplasarea se realizeaza doar daca mosina nu colizioneza cu nimi pe directia respectiva
		if (MoveOZ)
		{
			Score += cos(Rotations.y);
			Position.z += cos(Rotations.y) * Speed;
		}
		if (MoveOX)
			Position.x += sin(Rotations.y) * Speed;
	}

	// Se recalculeaza matricea de modelare a masinii
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, Position);
	modelMatrix = glm::rotate(modelMatrix, Rotations.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, Scale);

	RotatiOZWheel += Speed;
	// Se recalculeaza matricile de modelare ale rotilor
	for (int i = 0; i < 4; i++)
		UpdateWheel(i);

}

void Player::UpdateWheel(int i)
{
	modelMatrixWheels[i] = glm::mat4(1);
	modelMatrixWheels[i] = glm::translate(modelMatrixWheels[i], Position);
	modelMatrixWheels[i] = glm::rotate(modelMatrixWheels[i], Rotations.y, glm::vec3(0, 1, 0));
	modelMatrixWheels[i] = glm::translate(modelMatrixWheels[i], CarOffset[i]);
	modelMatrixWheels[i] = glm::rotate(modelMatrixWheels[i], RotatiOZWheel, glm::vec3(1, 0, 0));
	modelMatrixWheels[i] = glm::scale(modelMatrixWheels[i], glm::vec3(0.2, 0.5, 0.5));
}

void Player::VerifyCollision()
{
	// Se verifica coliziune cu peretii de pe marginea drumului
	for (int i = 0; i < RaceRef.size(); i++)
	{
		if (abs(Position.z - RaceRef[i]->Location.z) < 4)
		{
			for (int j = 0; j < 4; j++)
			{
				// pereti care se afla im fata masinii
				if (RaceRef[i]->SwitchDirection && abs(Position.x + CarOffset[j].x - RaceRef[i]->Location.x) < 2.1 &&
					abs(Position.z + CarOffset[j].z - RaceRef[i]->Location.z - CenterOffsetDistWalls) < CollisionDistance)
					MoveOZ = false;
				// pereti care se afla pe o parte si pe alta a drumului
				if ((Rotations.y > 0 && RaceRef[i]->LeftWall && abs(Position.x + CarOffset[j].x - RaceRef[i]->Location.x - CenterOffsetDistWalls) < CollisionDistance) ||
					(Rotations.y < 0 && RaceRef[i]->RightWall && abs(Position.x + CarOffset[j].x - RaceRef[i]->Location.x + CenterOffsetDistWalls) < CollisionDistance))
					MoveOX = false;
			}
		}
	}

	// Se verifica coliziuna cu obiectele de pe drum
	for (int i = 0; i < TrackObjectsRef.size(); i++)
	{
		float TrackObjectOXCollision = 0.5;
		float TrackObjectOZCollision = 0.25;

		float FrontWheelsDistance = CarOffset[3].x - CarOffset[2].x;
		for (int j = 0; j < 10; j++)
		{
			if (abs(Position.z + CarOffset[2].z - TrackObjectsRef[i].z) < TrackObjectOZCollision + 0.5)
			{
				if (abs(Position.x + CarOffset[2].x + FrontWheelsDistance/10 * j - TrackObjectsRef[i].x) < TrackObjectOXCollision &&
					Position.z + CarOffset[2].z < TrackObjectsRef[i].z + TrackObjectOZCollision)
					MoveOZ = false;

			}
		}
	}

	// Daca s-a depistat o coliziune se seteaza bool-ul de Crash pentru a se randa particulele de fum
	if (!MoveOX || !MoveOZ)
	{
		Score -= 1;
		Crash = true;
	}
}

glm::vec3 Player::GetPosition()
{
	return Position;
}

glm::vec3 Player::GetRotations()
{
	return Rotations;
}
