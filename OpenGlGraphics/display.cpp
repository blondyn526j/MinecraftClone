#include "display.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "input.h"

#define bufferSize 3276800

Display::Display(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GetMouse


	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);
	
	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize" << std::endl;
	}
	else 
	{
		std::cout << "Glew succesfully initialized" << std::endl;
	}


	m_isClosed = false;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::Update()
{
	SDL_GL_SwapWindow(m_window);
}

void Display::Clear(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//m_bufferedVertices = 0;
	//positions.clear();
	//texCoords.clear();
}

void Display::ClearBuffer()
{
	m_bufferedVertices = 0;
	positions.clear();
	texCoords.clear();
}

void Display::ReassignBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * m_bufferedVertices, &(positions[0]));

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * m_bufferedVertices, &(texCoords[0]));
}

bool Display::IsClosed()
{
	return m_isClosed;
}

void Display::InitializeBuffer()
{
	positions.reserve(bufferSize);
	texCoords.reserve(bufferSize);

	//m_drawCount = numVertices;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	//std::vector<glm::vec3> positions;
	//std::vector<glm::vec2> texCoords;

	//positions.reserve(numVertices);
	//texCoords.reserve(numVertices);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindVertexArray(0);

	//glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndecies * sizeof(m_indecies[0]), &m_indecies[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Display::AppendToDrawBuffer(Vertex* vertices, int numVertices, glm::vec3* offset)
{
	for (int i = 0; i < numVertices; i++)
	{
		positions.push_back(vertices[i].pos + *offset);
		texCoords.push_back(vertices[i].texCoord);
	}
	m_bufferedVertices += numVertices;
}

void Display::DrawBuffer()
{
	//std::cout << m_bufferedVertices << std::endl;

	//glBindBuffer(0);

	glBindVertexArray(m_vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, m_bufferedVertices);

	glBindVertexArray(0);
}