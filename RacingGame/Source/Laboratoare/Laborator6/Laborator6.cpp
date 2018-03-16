#include "Laborator6.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator6::Laborator6()
{
}

Laborator6::~Laborator6()
{
}

void Laborator6::Init()
{
	GameCamera = new LabCamera6::Camera(window);
	ScoreCamera = new LabCamera6::Camera(window);
	// Deschidere fisier
	FILE* FileRoadConfig = fopen(RoadConfigFile.c_str(), "r");

	if (FileRoadConfig == NULL)
	{
		printf("Error openning file.\n");
		return;
	}

	// Initializare variabile folosite pentru citirea datelor
	// a - tipul drumului, b - distanta; c - unghi de inclinare
	int a = 0, b = 0, c = 0;

	// Pozitia primei bucati de drum
	float CurrentOZ = 0.0f;
	float CurrentOY = 0.0f;
	float CurrentOX = 0.0f;

	glm::mat4 modelMatrix;

	bool Right, Left;

	// Citire fisier 
	while (!feof(FileRoadConfig))
	{
		fscanf(FileRoadConfig, "%d %d", &a, &b);

		Right = true;
		Left = true;

		// Daca dumul se duce spre stanga sau dreapta o parte din bucatile de drum trebuie randate in paralel cu cele deja existente;
		// se creeaza astfel o portiune de drum cu latime dubla(pentru a putea permite jucatorului sa schimbe lane-ul)
		// De asemenea, se seteza bool-ul pe ultma bucata de drum pentru a se verifica coliziunea cu masina
		if (a == 1 || a == 2)
		{
			b += SwitchDistance;
			CurrentOZ -= SwitchDistance * 4;
			Race[Race.size() - 1]->SwitchDirection = true;
		}

		// portiune de drum spre stanga
		if (a == 1)
		{
			CurrentOX += 4;
			for (int i = 1; i <= SwitchDistance; i++)
				Race[Race.size() - i]->LeftWall = false;
			Right = false;
		}

		// portiune de drum spre dreapta
		if (a == 2)
		{
			CurrentOX -= 4;
			for (int i = 1; i <= SwitchDistance; i++)
				Race[Race.size() - i]->RightWall = false;
			Left = false;
		}
		
		for (int i = 0; i < b; i++)
		{
			// Se creeaz o bucata de drum (un patrat) pentru care se seteza pozitia, si laturile pe care are coliziune
			// De asemenea, se calculeaza si matricile de modelare (deoarece obiectele sunt statice nu e necesar aca matricile sa se refacala fiecare frame)
			RaceTrack *newTrack = new RaceTrack;
			newTrack->Location = glm::vec3(CurrentOX, CurrentOY, CurrentOZ);
			newTrack->angle = 0;

			// Se creeaza matricea de modelare pentru fiecare bucata de drum
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, newTrack->Location);
			modelMatrix = glm::rotate(modelMatrix, RADIANS(newTrack->angle), glm::vec3(1, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
			newTrack->modelMatrix = modelMatrix;

			if ((Left || Right) && i < SwitchDistance)
			{
				newTrack->LeftWall = Left;
				newTrack->RightWall = Right;
			}

			Race.push_back(newTrack);
			
			CurrentOZ += 4;
		}
	}

	// Se creeaza mesh.ele necesare 
	for(int i = 0; i < MeshesNames.size(); i++)
	{
		Mesh* mesh = new Mesh(MeshesNames[i]);
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", MeshesNames[i] + ".obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Se creeaza un patrat format din 2 triunghiuri
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3( 1, 0, 1), glm::vec3(0, 1, 1), glm::vec3(0)),
			VertexFormat(glm::vec3(1, 0, -1), glm::vec3(0, 1, 1), glm::vec3(0)),
			VertexFormat(glm::vec3(-1, 0, 1), glm::vec3(0, 1, 1), glm::vec3(0)),
			VertexFormat(glm::vec3(-1, 0, -1), glm::vec3(0, 1, 1), glm::vec3(0))
		};

		vector<unsigned short> indices =
		{
			1, 0, 2,	2, 3, 1
		};

		CreateMesh("plane", vertices, indices);
	}

	// Instantierea shader-ului nostru
	{
		Shader *shader = new Shader("ShaderLab6");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Se creeaza matricile de modelare pentru pereti
	SetWallsModelMatrix();
	// Se creeaza matricile de modelare pentru obstacolele de pe drum
	SetTrackObjectsModelMatrix();

	VanDiesel->RaceRef = Race;
	VanDiesel->TrackObjectsRef = TrackObjectsPosition;
	VanDiesel->CenterOffsetDistWalls = CenterOffsetDistWalls;

}

Mesh* Laborator6::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

void Laborator6::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator6::Update(float deltaTimeSeconds)
{
	// Se randeaza cerul
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, VanDiesel->GetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(50));
		RenderMesh(meshes["sphere"], shaders["ShaderLab6"], modelMatrix, 0);
	}

	// Se randeaza pamantul
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.51, 0) + VanDiesel->GetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(60));
		RenderMesh(meshes["plane"], shaders["ShaderLab6"], modelMatrix, 1);
	}

	// Se randeaza drumul
	for (int i = 0; i < Race.size(); i++)
		RenderMesh(meshes["plane"], shaders["ShaderLab6"], Race[i]->modelMatrix, 2);
	// Se randeaza obstacolele de pe drum
	for (int i = 0; i < TrackObjects.size(); i++)
		RenderMesh(meshes["teapot"], shaders["ShaderLab6"], TrackObjects[i], 3);
	// Se randeaza peretii de pe marginea drumului
	for (int i = 0; i < Walls.size(); i++)
		RenderMesh(meshes["box"], shaders["ShaderLab6"], Walls[i], 4);
	// Se verifica daca masina a ajuns la sfarsitul hartii
	// daca da, atunci se opreste miscarea jucatorului
	if (VanDiesel->GetPosition().z > Race[Race.size() - 1]->Location.z && !VanDiesel->StopMovement)
	{
		VanDiesel->StopMovement = true;
		if (Engine::GetElapsedTime() < TargetTime)
			VanDiesel->Score += abs(Engine::GetElapsedTime() - TargetTime) * 300;
	}

	// Se realizeaza update pe miscarea masinii
	VanDiesel->Update(deltaTimeSeconds);

	// Daca camera este 3rd Person se randeaza masina si rotile
	if (GameCamera->Camera3rd)
	{
		RenderMesh(meshes["box"], shaders["ShaderLab6"], VanDiesel->modelMatrix, 5);

		for (int i = 0; i < 4; i++)
			RenderMesh(meshes["sphere"], shaders["ShaderLab6"], VanDiesel->modelMatrixWheels[i], 6);
	}

	// Se face update pe camera (pozitia si rotatie daca camera 1st Person este activa)
	GameCamera->Update(VanDiesel->GetPosition(), VanDiesel->GetRotations() / 2.0f);
 
	Step++;
	if (Step == MaxSteps)
	{
		Transiction = (Transiction + 1) % 3;
		Step = 0;
	}

	printf("%f\n", VanDiesel->Score);
	// Daca masina s-a lovit si nu sunt alte particule active se reseteaza vectorul de particule 
	if (VanDiesel->Crash && !ActiveParticle)
		GenerateParticles();

	if (ActiveParticle)
		HandleParticles(deltaTimeSeconds);

	DrawScor();
}

void Laborator6::FrameEnd()
{
}

void Laborator6::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int RenderType, float valA, float valB, float time)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	shader->Use();

	if (scoreCamera) {
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(ScoreCamera->GetViewMatrix()));
	}
	else
	{
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GameCamera->GetViewMatrix()));
	}
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GameCamera->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int RenderTypeLocation = glGetUniformLocation(shader->GetProgramID(), "RenderType");
	glUniform1i(RenderTypeLocation, RenderType);

	// Se trimit datele necesare ciclului zi-noapte
	int StepLocation = glGetUniformLocation(shader->GetProgramID(), "Step");
	glUniform1i(StepLocation, Step);

	int TransictionLocation = glGetUniformLocation(shader->GetProgramID(), "Transiction");
	glUniform1i(TransictionLocation, Transiction);

	int MaxStepsLocation = glGetUniformLocation(shader->GetProgramID(), "MaxSteps");
	glUniform1f(MaxStepsLocation, MaxSteps);

	// Se trimit variabilele necesare pentru particulele de fum
	int valALocation = glGetUniformLocation(shader->GetProgramID(), "valA");
	glUniform1f(valALocation, valA);

	int valBLocation = glGetUniformLocation(shader->GetProgramID(), "valB");
	glUniform1f(valBLocation, valB);

	int timeLocation = glGetUniformLocation(shader->GetProgramID(), "time");
	glUniform1f(timeLocation, time);

	// Setare modului de desenare in functie de apasarea tastei
	mesh->SetDrawMode((RenderStyle == 2) ? GL_LINES : GL_TRIANGLES);

	mesh->Render();
}

void Laborator6::OnInputUpdate(float deltaTime, int mods)
{	
	
}

void Laborator6::OnKeyPress(int key, int mods)
{
	VanDiesel->ManageMovement(key, true);
	GameCamera->ManageInput(key);

	// Setarea modului de randare
	if (key == GLFW_KEY_S)
		RenderStyle = (RenderStyle == 1) ? 2 : 1;
}

void Laborator6::OnKeyRelease(int key, int mods)
{
	VanDiesel->ManageMovement(key);
}

void Laborator6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator6::OnWindowResize(int width, int height)
{
}

void Laborator6::SetWallsModelMatrix()
{
	glm::mat4 modelMatrix;

	for (int i = 0; i < Race.size(); i++)
	{
		// Daca este necesar, se creeaza un perete pe partea stanga a bucatii respective de drum
		if (Race[i]->LeftWall)
		{
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(CenterOffsetDistWalls, 0.5, 0) + Race[i]->Location);
			modelMatrix = glm::scale(modelMatrix, ScaleWalls);

			Walls.push_back(modelMatrix);
		}
		// Daca este necesar, se creeaza un perete pe partea dreapta a bucatii respective de drum
		if (Race[i]->RightWall)
		{
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-CenterOffsetDistWalls, 0.5, 0) + Race[i]->Location);
			modelMatrix = glm::scale(modelMatrix, ScaleWalls);

			Walls.push_back(modelMatrix);
		}
		// Daca este necesar, se creeaza un perete in fata bucatii respective de drum
		if (Race[i]->SwitchDirection)
		{
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, CenterOffsetDistWalls) + Race[i]->Location);
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
			modelMatrix = glm::scale(modelMatrix, ScaleWalls);

			Walls.push_back(modelMatrix);

			// De asemenea, se calculeaza unde a inceput portiune de respectiva de drum de care apartine bucata si se creeaza un perete in spate
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, -CenterOffsetDistWalls) + Race[i + 1]->Location);
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
			modelMatrix = glm::scale(modelMatrix, ScaleWalls);

			Walls.push_back(modelMatrix);
		}
	}
}

void Laborator6::SetTrackObjectsModelMatrix()
{
	glm::mat4 modelMatrix;
	float OXOffset = 0.0;
	float OZOffset = 0.0f;

	for (int i = 2; i < Race.size(); i++)
	{
		// Exista o sansa de 30% ca sa apara un nou obstacol pe drum
		if (rand() % 100 > 70)
		{
			// Se creeza offset-uri random 
			OXOffset = (rand() % 8) / 10.0;
			OZOffset = (rand() % 8) / 10.0;
			OXOffset *= (rand() % 2 == 0) ? 1 : -1;
			OZOffset *= (rand() % 2 == 0) ? 1 : -1;
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(OXOffset, 0, OZOffset) + Race[i]->Location);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
			// Se adauga datele despre noul obstacol in vector
			TrackObjects.push_back(modelMatrix);

			glm::vec3 in = glm::vec3(OXOffset, 0, OZOffset) + Race[i]->Location;
			TrackObjectsPosition.push_back(in);
		}
	}
}

void Laborator6::GenerateParticles()
{
	for (int i = 0; i < 400; i++)
	{
		Particles[i].CurrentLife = 0.0f;
		Particles[i].LifeTime = (rand() % 100) / 100.0f;
		Particles[i].Location = VanDiesel->GetPosition() + glm::vec3(0, 0, 1);
		glm::mat4  modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, Particles[i].Location);
		modelMatrix = glm::scale(modelMatrix, Particles[i].Scale);

		Particles[i].modelMatrix = modelMatrix;
		Particles[i].valA = rand() % 10;
		Particles[i].valB = rand() % 10;

		Particles[i].valA *= (rand() % 2 == 0) ? 1 : 0;
	}

	ActiveParticle = true;
}

void Laborator6::HandleParticles(float deltaTimeSeconds)
{
	int count = 0;
	for (int i = 0; i < 400; i++)
	{
		Particles[i].CurrentLife += deltaTimeSeconds;
		if (Particles[i].CurrentLife < Particles[i].LifeTime)
		{
			RenderMesh(meshes["sphere"], shaders["ShaderLab6"], Particles[i].modelMatrix, 7, Particles[i].valA, Particles[i].valB, Particles[i].CurrentLife);
			count++;
		}
	}

	if (count == 0)
	{
		ActiveParticle = false;
		VanDiesel->Crash = false;
	}

}

void Laborator6::DrawScor()
{
	scoreCamera = true;
	glm::ivec2 resolution = window->GetResolution();
	float score = VanDiesel->Score;
	int gold = score / 1000;
	score -= gold * 1000;
	int silver = score / 100;
	score -= silver * 100;
	int bronze = score / 10;
	// sets the screen area where to draw
	glViewport(0, resolution.y * 3/4 , resolution.x/2, resolution.y / 4);
	for(int i = 0; i < gold; i++)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4 -0.5f *i, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderMesh(meshes["sphere"], shaders["ShaderLab6"], modelMatrix, 8);
	}
	for (int i = 0; i < silver; i++)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4.5f -0.5f * i, 0.0f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderMesh(meshes["sphere"], shaders["ShaderLab6"], modelMatrix, 9);
	}
	for (int i = 0; i < bronze; i++)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5 - 0.5f * i, -1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderMesh(meshes["sphere"], shaders["ShaderLab6"], modelMatrix, 10);
	}

	scoreCamera = false;
	
}
