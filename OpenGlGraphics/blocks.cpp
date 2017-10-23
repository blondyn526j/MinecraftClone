#include "blocks.h"
#include <iostream>

Blocks::Blocks(Display* display, int numBlocks)
{
	m_meshes.reserve(numBlocks);
	for (int i = 0; i < numBlocks; i++)
	{
		float a = (float)i / numBlocks;
		float b = (float)(i + 1) / numBlocks;

		Vertex cubeVertices[] = {
			//UP
			Vertex(glm::vec3(0, 1, 1), glm::vec2(b,0)),//0
			Vertex(glm::vec3(1, 1, 1), glm::vec2(a,0)),//1
			Vertex(glm::vec3(1, 1, 0), glm::vec2(a,0.33f)),//2

			Vertex(glm::vec3(0, 1, 1), glm::vec2(b,0)),//0
			Vertex(glm::vec3(1, 1, 0), glm::vec2(a,0.33f)),//2
			Vertex(glm::vec3(0, 1, 0), glm::vec2(b,0.33f)),//3
			//FRONT
			
			Vertex(glm::vec3(1, 1, 0), glm::vec2(a,0.33f)),//2
			Vertex(glm::vec3(0, 0, 0), glm::vec2(b,0.66f)),//4
			Vertex(glm::vec3(0, 1, 0), glm::vec2(b,0.33f)),//3

			Vertex(glm::vec3(1, 1, 0), glm::vec2(a,0.33f)),//2
			Vertex(glm::vec3(1, 0, 0), glm::vec2(a,0.66f)),//5
			Vertex(glm::vec3(0, 0, 0), glm::vec2(b,0.66f)),//4

			//DOWN
			Vertex(glm::vec3(0, 0, 0), glm::vec2(b,0.66f)),//4
			Vertex(glm::vec3(1, 0, 0), glm::vec2(a,0.66f)),//5
			Vertex(glm::vec3(1, 0, 1), glm::vec2(a,1)),//7

			Vertex(glm::vec3(0, 0, 0), glm::vec2(b,0.66f)),//4
			Vertex(glm::vec3(1, 0, 1), glm::vec2(a,1)),//7
			Vertex(glm::vec3(0, 0, 1), glm::vec2(b,1)),//6

			//RIGHT
			Vertex(glm::vec3(1, 1, 0), glm::vec2(b, 0.33f)),//8
			Vertex(glm::vec3(1, 1, 1), glm::vec2(a, 0.33f)),//9
			Vertex(glm::vec3(1, 0, 1), glm::vec2(a, 0.66f)),//10

			Vertex(glm::vec3(1, 1, 0), glm::vec2(b, 0.33f)),//8
			Vertex(glm::vec3(1, 0, 1), glm::vec2(a, 0.66f)),//10
			Vertex(glm::vec3(1, 0, 0), glm::vec2(b, 0.66f)),//11
			//BACK
			Vertex(glm::vec3(0, 1, 1), glm::vec2(a, 0.33f)),//12
			Vertex(glm::vec3(1, 0, 1), glm::vec2(b, 0.66f)),//14
			Vertex(glm::vec3(1, 1, 1), glm::vec2(b, 0.33f)),//13

			Vertex(glm::vec3(0, 1, 1), glm::vec2(a, 0.33f)),//12
			Vertex(glm::vec3(0, 0, 1), glm::vec2(a, 0.66f)),//15
			Vertex(glm::vec3(1, 0, 1), glm::vec2(b, 0.66f)),//14

			//LEFT
			Vertex(glm::vec3(0, 1, 0), glm::vec2(a, 0.33f)),//16
			Vertex(glm::vec3(0, 0, 1), glm::vec2(b, 0.66f)),//18
			Vertex(glm::vec3(0, 1, 1), glm::vec2(b, 0.33f)),//17

			Vertex(glm::vec3(0, 1, 0), glm::vec2(a, 0.33f)),//16
			Vertex(glm::vec3(0, 0, 0), glm::vec2(a, 0.66f)), //19
			Vertex(glm::vec3(0, 0, 1), glm::vec2(b, 0.66f)) };//18



		/*unsigned int indecies[] = {
			//UP
			0,1,2,
			0,2,3,
			//FRONT
			2,5,4,
			2,4,3,
			//DOWN
			4,5,7,
			4,7,6,
			//RIGHT
			8,9,10,
			8,10,11,
			//BACK
			12,14,13,
			12,15,14,
			//LEFT
			16,18,17,
			16,19,18
		};*/
		m_meshes.push_back(new Mesh(display, cubeVertices, 36));
	}
}

Blocks::~Blocks()
{
	//m_meshes.clear();
}
