#pragma once

#include <glm/glm.hpp> 
#include <GL/glew.h>
#include <vector>

class Vertex
{
public:
	Vertex(const glm::vec3 &pos, const glm::vec2 &texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
	}


	glm::vec3 pos;
	glm::vec2 texCoord;
protected:
private:
};