#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <string.h>
#include "LabCamera6.h"

typedef struct RaceTrack
{
	glm::vec3 Location;
	glm::mat4 modelMatrix;
	float angle = 0.0f;
	bool SwitchDirection = false;
	bool RightWall = true;
	bool LeftWall = true;
};

class Player
{
public :
	Player();
	~Player();

	void ManageMovement(int key, bool Press = false);
	void Update(float deltaTimeSeconds);
	void UpdateWheel(int i);

	void VerifyCollision();

	glm::vec3 GetPosition();
	glm::vec3 GetRotations();

	glm::mat4 modelMatrix;
	glm::mat4 modelMatrixWheels[4];
	glm::vec3 CarOffset[4];
	float CenterOffsetDistWalls;
	float CollisionDistance = 0.3;

	std::vector<struct RaceTrack*> RaceRef;
	std::vector<glm::vec3> TrackObjectsRef;

	float Score = 0;
	bool Crash = false;
	bool StopMovement = false;

private:

	glm::vec3 Position;
	glm::vec3 Rotations;
	glm::vec3 Scale;

	bool Accelerating = false;
	bool Breaking = false;

	bool MoveOX = true;
	bool MoveOZ = true;

	// 0 - nu se roteste masina; 1 - vireaza stanga; 2 - vireza dreapta; 
	//3 - ambele taste sunt apasate simultan (masina nu se va duce in nici o directie)
	int TurnDirection = 0;
	float Speed = 0.0f;
	float RotatiOZWheel = 0.0f;

	float MaxSpeedPerFrame = 0.15;
	float MaxRotation = RADIANS(40);
};
