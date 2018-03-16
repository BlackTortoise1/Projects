#include "Laborator3_Vis2D.h"


using namespace std;

Laborator3_Vis2D::Laborator3_Vis2D()
{
}

Laborator3_Vis2D::~Laborator3_Vis2D()
{
}

bool sus = 0, stg = 0, dr = 0, jos = 0, aproape = 0, departe = 0;
float x, y, mx, my, time, spawntime = 2.0f, sumtime = 0.0f,bfire;
float playerspeed = 150.0f, rosu = 0.0f, alunesptime, angle, rosiisptime;
int gameover = 3, score = 0, active_peanuts = 0, dir;
std::vector<Proiectil> proiectile;
std::vector<Enemy> inamici;
std::vector<Peanut> alune;
std::vector<Redpeanut> alunerosii;

void Laborator3_Vis2D::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	
	glm::ivec2 resolution = window->GetResolution();
	x = (float)resolution.x/ 2;
	y = (float)resolution.y/ 2;
	
	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = 100;	// logic width
	logicSpace.height = 100;	// logic height

	
	glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
	length = 40.0f;
	angle = 0.0f;
	bfire = 0.0f;
	dir = 1;
	alunesptime = (float)(10 + (rand() % 6));
	rosiisptime = (float)(10 + (rand() % 6));

	Mesh* square1 = Object2D::CreateSquare("square1", corner, length, glm::vec3(0, 1, 0));
	AddMeshToList(square1);
	Mesh* enemy1 = Object2D::CreateSquare("enemy1", corner, length, glm::vec3(0, 0, 1));
	AddMeshToList(enemy1);
	Mesh* enemy2 = Object2D::CreateSquare("enemy2", corner, length, glm::vec3(0, 1, 1));
	AddMeshToList(enemy2);
	Mesh* enemy3 = Object2D::CreateSquare("enemy3", corner, length, glm::vec3(1, 1, 0));
	AddMeshToList(enemy3);
	enemy3 = Object2D::CreateSquare("enemy4", corner, length, glm::vec3(0.5f, 0.6f, 0.7f));
	AddMeshToList(enemy3);
	Mesh* proiectil = Object2D::CreateProiectil("proiectil", corner, length/4, glm::vec3(1, 0, 0));
	AddMeshToList(proiectil);
	Mesh* peanut = Object2D::CreatePeanut("peanut", corner, length / 4, glm::vec3(1.0f, 178.0f/255.0f , 102.0f/255.0f));
	AddMeshToList(peanut);
	Mesh* redpeanut = Object2D::CreatePeanut("redpeanut", corner, length / 4, glm::vec3(1.0f, 0.0f, 0.0f));
	AddMeshToList(redpeanut);
}

// 2D visualization matrix
glm::mat3 Laborator3_Vis2D::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Laborator3_Vis2D::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Laborator3_Vis2D::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(rosu, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Laborator3_Vis2D::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	if (gameover <= 0 && rosu < 1.0f) {
		rosu = (rosu + time) > 1.0f ? 1.0f : rosu + time ;
	}
	glClearColor(rosu, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Laborator3_Vis2D::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);
	
	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	time = deltaTimeSeconds;
	DrawScene(visMatrix);

	// The viewport is now the right half of the window

	//viewSpace = ViewportSpace(resolution.x / 2, 0, resolution.x / 2, resolution.y);
	//SetViewportArea(viewSpace, glm::vec3(0.5f), true);

	// Compute uniform 2D visualization matrix
	//visMatrix = glm::mat3(1);
	//visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	//DrawScene(visMatrix);
}

void Laborator3_Vis2D::FrameEnd()
{

}

void split(float angle, float x, float y)
{
	float length = 30.0f;
	inamici.push_back(Enemy(1, x + cos(angle)*length,y + sin(angle)*length));
	inamici.push_back(Enemy(1, x - cos(angle)*length, y - sin(angle)*length));
}

void Laborator3_Vis2D::DrawScene(glm::mat3 visMatrix)
{
	int hp, i, j, k;
	float dist, r = ((float)(rand() % 628)) / 100.0f;

	if (gameover > 0)
	{
		if(bfire>0) bfire = bfire - time;

		angle = angle + 2.0f*time;
		if (angle > 6.28f) angle = 0.0f;

		if (sus) y = y + playerspeed*time;

		if (jos) y = y - playerspeed*time;

		if (stg) x = x - playerspeed*time;

		if (dr)	x = x + playerspeed*time;
	}
	glm::ivec2 resolution = window->GetResolution();

	modelMatrix = glm::mat3(1);
	modelMatrix = modelMatrix * Transform2D::Translate(x, y);
	modelMatrix = modelMatrix * Transform2D::Rotate(atan((mx-x)/(my+y-resolution.y)));
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	
	sumtime = sumtime + time;
	alunesptime = alunesptime - time;
	rosiisptime = rosiisptime - time;
	
	if (alunesptime < 0 && gameover > 0)
	{
		alune.push_back(Peanut((float)(20 + (rand() % (resolution.x - 20))), (float)(20 + (rand() % (resolution.y - 20)))));
		alunesptime = (float)(10 + (rand() % 6));
	}

	if (rosiisptime < 0 && gameover > 0)
	{
		alunerosii.push_back(Redpeanut((float)(20 + (rand() % (resolution.x - 20))), (float)(20 + (rand() % (resolution.y - 20)))));
		rosiisptime = (float)(10 + (rand() % 6));
	}

	if (sumtime >= spawntime && gameover>0)
	{
		sumtime = 0;
		if (spawntime > 0.5f)
		{
			spawntime = spawntime - time;
		}
		int aux=rand()%6;
		hp = ((aux < 4) ? 1 : (aux==4 ? 2 : 3));

		Enemy enemy(hp,(1+sinf(r))*resolution.x/2,(1+cosf(r))*resolution.y/2);
		inamici.push_back(enemy);
	}
	//coliziunea aluna-player
	for (j = 0; j <(int) alune.size() && active_peanuts < 2; j++)
	{
		dist = sqrtf((x - alune[j].x)*(x - alune[j].x) + ((y - alune[j].y)*(y - alune[j].y)));

		if (!alune[j].active && dist < length)
		{
			alune[j].active = 1;
			alune[j].dir = dir;
			dir = (dir == 1) ? -1 : 1;
			active_peanuts++;
		}
	}

	//coliziune aluna rosie - player
	for (j = 0; j < (int)alunerosii.size(); j++)
	{
		dist = sqrtf((x - alunerosii[j].x)*(x - alunerosii[j].x) + ((y - alunerosii[j].y)*(y - alunerosii[j].y)));
		
		if (dist < length)
		{
			bfire = bfire + 10.0f;
			for (k = j; k < (int)alunerosii.size() - 1; k++)
				alunerosii[k] = alunerosii[k + 1];
			alunerosii.pop_back();
		}
	}

	//coliziune inamic-player si inamic-proiectil
	for (i = 0; i < (int)inamici.size(); i++)
	{
		//inamic - player
		dist = sqrtf((x - inamici[i].x)*(x - inamici[i].x) + ((y - inamici[i].y)*(y - inamici[i].y)));
		if (dist < length)
		{
			gameover--;
			inamici.erase(inamici.begin() + i);
			i--;
			if (gameover < 1)
				break;
		}

		//inamic - proiectil
		for (j = 0; j < proiectile.size() && i>=0; j++)
		{
			dist = sqrtf((proiectile[j].x - inamici[i].x)*(proiectile[j].x - inamici[i].x)
				+ ((proiectile[j].y - inamici[i].y)*(proiectile[j].y - inamici[i].y)));
			if (dist < inamici[i].scale*length / 2)
			{
				if (inamici[i].hp == 3)
				{
					inamici[i].hp = 0;
					split(atan(-(inamici[i].x - x) / (inamici[i].y - y)),inamici[i].x,inamici[i].y);
				}
				else
					inamici[i].change();
				if (inamici[i].hp == 0)
				{
					for (k = i; k < (int)inamici.size() - 1; k++)
						inamici[k] = inamici[k + 1];
					inamici.pop_back();
					i--;
				}
				for (k = j; k < (int)proiectile.size() - 1; k++)
					proiectile[k] = proiectile[k + 1];
				proiectile.pop_back();
				break;
			}
		}

		//inamic-aluna
		for (j = 0; i>=0 && j < (int)alune.size() && active_peanuts > 0; j++)
		{
			dist = sqrtf((inamici[i].x - alune[j].x)*(inamici[i].x - alune[j].x) + ((inamici[i].y - alune[j].y)*(inamici[i].y - alune[j].y)));
			if (alune[j].active && dist < length)
			{
				inamici[i].change();
				if (inamici[i].hp == 0)
				{
					for (k = i; k < inamici.size() - 1; k++)
						inamici[k] = inamici[k + 1];
					inamici.pop_back();
					i--;
				}
				for (k = j; k < alune.size() - 1; k++)
					alune[k] = alune[k + 1];
				alune.pop_back();
				active_peanuts--;				
			}
		}
	}

	//randare proiectile
	for (i = 0; i < proiectile.size(); i++)
	{
		modelMatrix = glm::mat3(1);
		modelMatrix = modelMatrix * Transform2D::Translate(proiectile[i].x, proiectile[i].y);
		if (gameover > 0)
		{
			proiectile[i].x = proiectile[i].x - proiectile[i].incx*time;
			proiectile[i].y = proiectile[i].y + proiectile[i].incy*time;
		}
		modelMatrix = modelMatrix * Transform2D::Rotate(atan((proiectile[i].incx) / (proiectile[i].incy)));
		if ((proiectile[i].x > resolution.x + length) || (proiectile[i].x < -length) || (proiectile[i].y > resolution.y + length) || (proiectile[i].y < -length))
		{
			for (k = i; k < proiectile.size() - 1; k++)
				proiectile[k] = proiectile[k + 1];
			proiectile.pop_back();
			i--;
		}
		RenderMesh2D(meshes["proiectil"], shaders["VertexColor"], modelMatrix);
	}

	//randare inamici
	for (i = 0; i < inamici.size(); i++)
	{
		modelMatrix = glm::mat3(1);
		dist = sqrtf((inamici[i].x - x)*(inamici[i].x - x) + ((inamici[i].y - y)*(inamici[i].y - y)));
		if (gameover>0)
		{
			inamici[i].x = inamici[i].x - (time*(inamici[i].x - x)*inamici[i].speed / dist);
			inamici[i].y = inamici[i].y - (time*(inamici[i].y - y)*inamici[i].speed / dist);
		}
		modelMatrix = modelMatrix * Transform2D::Translate(inamici[i].x, inamici[i].y);
		modelMatrix = modelMatrix * Transform2D::Rotate(atan(-(inamici[i].x - x) / (inamici[i].y - y)));

		if (inamici[i].hp == 1)
		{
			if (inamici[i].speed < playerspeed)
			{
				modelMatrix = modelMatrix * Transform2D::Scale(inamici[i].scale, inamici[i].scale);
				RenderMesh2D(meshes["enemy1"], shaders["VertexColor"], modelMatrix);
			}
			else
			{
				if(inamici[i].scale > 0.8f)
					inamici[i].scale=inamici[i].scale - 3.0f*time;
				modelMatrix = modelMatrix * Transform2D::Scale(inamici[i].scale, inamici[i].scale);
				RenderMesh2D(meshes["enemy3"], shaders["VertexColor"], modelMatrix);
			}
		}
		else
		{
			if (inamici[i].hp == 2)
			{
				modelMatrix = modelMatrix * Transform2D::Scale(inamici[i].scale, inamici[i].scale);
				RenderMesh2D(meshes["enemy2"], shaders["VertexColor"], modelMatrix);
			}
			else
			{
				modelMatrix = modelMatrix * Transform2D::Scale(inamici[i].scale, inamici[i].scale);
				RenderMesh2D(meshes["enemy4"], shaders["VertexColor"], modelMatrix);
			}
		}
	}

	//randare alune
	for (i = 0; i < alune.size(); i++)
	{
		modelMatrix = glm::mat3(1);
		if (alune[i].active)
		{
			//printf("%d ", active_peanuts);
			alune[i].x = (float)x + (float)alune[i].dir*(sin(angle)*length);
			alune[i].y = (float)y + (float)alune[i].dir*(cos(angle)*length);
		}
		modelMatrix = modelMatrix * Transform2D::Translate(alune[i].x, alune[i].y);
		RenderMesh2D(meshes["peanut"], shaders["VertexColor"], modelMatrix);
	}
	
	//randare alune rosii
	for (i = 0; i < alunerosii.size(); i++)
	{
		modelMatrix = glm::mat3(1);
		modelMatrix = modelMatrix * Transform2D::Translate(alunerosii[i].x, alunerosii[i].y);
		RenderMesh2D(meshes["redpeanut"], shaders["VertexColor"], modelMatrix);
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate((float)(resolution.x), (float)(resolution.y - 40));
	modelMatrix *= Transform2D::Scale(3.0f, 3.0f);
	for (i = 0; i < gameover; i++)
	{
		modelMatrix *= Transform2D::Translate(-10.0, 0.0f);
		RenderMesh2D(meshes["proiectil"], shaders["Simple"], modelMatrix);
	}
}
	

void Laborator3_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
	//TODO move the logic window with W, A, S, D (up, left, down, right)
	/*if (sus)
	{
		logicSpace.y = logicSpace.y + 0.1f;
	}
	
	if (jos)
	{
		logicSpace.y = logicSpace.y - 0.1f;
	}
	
	if (stg)
	{
		logicSpace.x = logicSpace.x - 0.1f;
	}

	if (dr)
	{
		logicSpace.x = logicSpace.x + 0.1f;
	}

	if (aproape)
	{
		logicSpace.width = logicSpace.width + 0.1f;
		logicSpace.height = logicSpace.height + 0.1f;
	}

	if (departe)
	{
		logicSpace.width = logicSpace.width - 0.1f;
		logicSpace.height = logicSpace.height - 0.1f;
	}
	*/
	//TODO zoom in and zoom out logic window with Z and X
}

void Laborator3_Vis2D::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_W)
	{
		sus = 1;
	}
	
	if (key == GLFW_KEY_S)
	{
		jos = 1;
	}

	if (key == GLFW_KEY_A)
	{
		stg = 1;
	}
	
	if (key == GLFW_KEY_D)
	{
		dr = 1;
	}

}

void Laborator3_Vis2D::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_W)
	{
		sus = 0;
	}
	
	if (key == GLFW_KEY_S)
	{
		jos = 0;
	}

	if (key == GLFW_KEY_A)
	{
		stg = 0;
	}
	
	if (key == GLFW_KEY_D)
	{
		dr = 0;
	}

}

void Laborator3_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (gameover > 0)
	{ 
		mx = (float)mouseX;
		my = (float)mouseY;
	}
}

void Laborator3_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (gameover > 0)
	{
		glm::ivec2 resolution = window->GetResolution();
		if (button == 1)
		{
			Proiectil proiectil(x, y);
			float l;
			l = sqrtf((x - (float)mouseX)*(x - (float)mouseX) + (y - resolution.y + (float)mouseY)*(y - resolution.y + (float)mouseY));
			proiectil.setincx((x - (float)mouseX)*(Proiectil::speed) / l);
			proiectil.setincy(-(y - resolution.y + (float)mouseY)*(Proiectil::speed) / l);
			proiectile.push_back(proiectil);
			if (bfire > 0)
			{
				Proiectil proiectil2(x, y);
				proiectil2.setincx(-proiectil.incx);
				proiectil2.setincy(-proiectil.incy);
				proiectile.push_back(proiectil2);
			}
		}
		else
		{
			x = (float) mouseX;
			y = resolution.y - (float) mouseY;
		}
	}
}

void Laborator3_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}