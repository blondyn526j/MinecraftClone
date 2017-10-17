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

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int* indecies, unsigned int numVertices, unsigned int numIndecies);
	Mesh() {}
	//Mesh(Vertex* vertices, unsigned int* indecies);
	virtual ~Mesh();
	void Draw();

	float RayFaceIntersectionTEST(const glm::vec3 rayOrigin, const glm::vec3 rayDir, const glm::vec3 normal, Vertex *vertices)
	{
		float rayDotNorm = glm::dot(rayDir, normal);
		if (rayDotNorm == 0)
			return -1;

		glm::vec3 intersect = vertices[0].pos - rayOrigin;
		double t = glm::dot(intersect, normal) / rayDotNorm;
		if (t < 0)
			return -1;

		//bool positive = false;
		//double firstPtDot = glm::dot(rayDir, glm::cross((rayOrigin - vertices[0].pos), (rayOrigin - vertices[1].pos)));
		//if (firstPtDot <= 0) return -1;
		//else if (firstPtDot < 0) positive = false;
		//else return -1;

		for (int i = 0; i < 4; i++)
		{
			int nextPoint = (i + 1) % 4;
			double rayDotPt = glm::dot(rayDir, glm::cross((rayOrigin - vertices[i].pos), (rayOrigin - vertices[nextPoint].pos)));

			if (rayDotPt <= 0)
				return -1;
		}
		return t;
	}

protected:
private:

	enum 
	{
		POSITION_VB,
		TEXCOORD_VB,

		INDEX_VB,

		NUM_BUFFERS
	};

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indecies;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
	bool m_initialized = false;
};

