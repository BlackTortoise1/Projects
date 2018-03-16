#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <string.h>
#include "LabCamera6.h"
#include "Player.h"

typedef struct ParticleBody
{
	glm::vec3 Location;
	glm::mat4 modelMatrix;
	glm::vec3 Scale = glm::vec3(0.05);
	float LifeTime;
	float CurrentLife;
	float valA, valB;
};


class Laborator6 : public SimpleScene
{
	public:
		Laborator6();
		~Laborator6();

		void Init() override;

		Mesh * CreateMesh(const char * name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int RenderType = 1, float valA = 0, float valB = 0, float time = 0);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void SetWallsModelMatrix();
		void SetTrackObjectsModelMatrix();
		void GenerateParticles();
		void HandleParticles(float deltaTimeSeconds);
		void DrawScor();

		std::string MeshesFile = "Source/Laboratoare/Laborator6/MeshesFile.txt";
		std::string RoadConfigFile = "Source/Laboratoare/Laborator6/RoadConfig.txt";

		LabCamera6::Camera *GameCamera;
		LabCamera6::Camera *ScoreCamera;
		bool scoreCamera = false;
		std::vector<struct RaceTrack*> Race;

		std::vector<glm::mat4> Walls;
		std::vector<glm::vec3> TrackObjectsPosition;
		std::vector<glm::mat4> TrackObjects;
		float CenterOffsetDistWalls = 2;
		glm::vec3 ScaleWalls = glm::vec3(0.2, 1, 4.0);
		int SwitchDistance = 3;

		std::vector<std::string> MeshesNames = { "box", "sphere", "teapot" };

		Player *VanDiesel = new Player();

		int Transiction = 0;
		int Step = 0;
		int MaxSteps = 500;

		int RenderStyle = 1;

		bool ActiveParticle = false;
		struct ParticleBody Particles[400];

		float TargetTime = 20.0f;
};
