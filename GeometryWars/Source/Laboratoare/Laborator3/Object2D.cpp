#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 mid, float length, glm::vec3 color, bool fill)
{
	glm::vec3 mij = mid;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(mij, color),
		VertexFormat(mij + glm::vec3(length/2, length/2, 0), color),
		VertexFormat(mij + glm::vec3(length/2, -length/2, 0), color),
		VertexFormat(mij + glm::vec3(-length/2, -length/2, 0), color),
		VertexFormat(mij + glm::vec3(-length/2, length/2, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = {1,2,0,3,4,0};
	square->SetDrawMode(GL_TRIANGLES);


	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateProiectil(std::string name, glm::vec3 mid, float length, glm::vec3 color, bool fill)
{
	glm::vec3 mij = mid;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(mij, color),
		VertexFormat(mij + glm::vec3(length / 4, length , 0), color),
		VertexFormat(mij + glm::vec3(length / 4, -length, 0), color),
		VertexFormat(mij + glm::vec3(-length / 4, -length, 0), color),
		VertexFormat(mij + glm::vec3(-length / 4, length, 0), color)
	};

	Mesh* proiectil = new Mesh(name);
	std::vector<unsigned short> indices = { 1,2,3,3,4,1 };
	proiectil->SetDrawMode(GL_TRIANGLES);


	proiectil->InitFromData(vertices, indices);
	return proiectil;
}

Mesh* Object2D::CreatePeanut(std::string name, glm::vec3 mid, float length, glm::vec3 color, bool fill)
{
	glm::vec3 mij = mid;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(mij, color),
		VertexFormat(mij + glm::vec3(length , length , 0), color),
		VertexFormat(mij + glm::vec3(length , -length, 0), color),
		VertexFormat(mij + glm::vec3(0 , -(length*1.5f), 0), color),
		VertexFormat(mij + glm::vec3(-length , -length, 0), color),
		VertexFormat(mij + glm::vec3(-length , length, 0), color),
		VertexFormat(mij + glm::vec3(0, length*1.5f, 0), color)
	};

	Mesh* peanut = new Mesh(name);
	std::vector<unsigned short> indices = { 1,2,4,4,5,1,1,5,6,2,3,4};
	peanut->SetDrawMode(GL_TRIANGLES);


	peanut->InitFromData(vertices, indices);
	return peanut;
}