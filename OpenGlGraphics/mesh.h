#pragma once
#include <glm/glm.hpp> 
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "Vertex.h"
#include "display.h"

class Mesh
{
public:
	Mesh(Display* display, Vertex* vertices, unsigned int numVertices);
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

	std::vector<Vertex> vertices;

protected:
private:

	enum 
	{
		POSITION_VB,
		TEXCOORD_VB,

		INDEX_VB,

		NUM_BUFFERS
	};

	std::vector<unsigned int> m_indecies;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
	bool m_initialized = false;
	Display* m_display;
};

