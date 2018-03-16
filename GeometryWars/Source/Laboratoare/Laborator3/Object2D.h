#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
	Mesh* CreateProiectil(std::string name, glm::vec3 mid, float length, glm::vec3 color, bool fill=true);
	Mesh* CreatePeanut(std::string name, glm::vec3 mid, float length, glm::vec3 color, bool fill = true);
}

