#include "display.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "input.h"

#define bufferSize 4*3276800

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_FOG);
	//glFogfv(GL_FOG_START, &fogStart);
	//glFogfv(GL_FOG_START, &fogEnd);

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
}

void Display::ClearBuffer()
{
	for (int i = 0; i < NUM_TYPES; i++)
	{
		m_bufferedVertices[i] = 0;
		//positions[i].clear();
		//texCoords[i].clear();
	}
}

void Display::ReassignBuffer()
{
	for (int i = 0; i < NUM_TYPES; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i][POSITION_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * m_bufferedVertices[i], &(positions[i][0]));

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i][TEXCOORD_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * m_bufferedVertices[i], &(texCoords[i][0]));

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i][NORMALS_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * m_bufferedVertices[i], &(normals[i][0]));

		m_drawnVertices[i] = m_bufferedVertices[i];
	}
}

bool Display::IsClosed()
{
	return m_isClosed;
}

void Display::InitializeBuffer()
{
	for (int i = 0; i < NUM_TYPES; i++)
	{
		positions[i].reserve(bufferSize);
		texCoords[i].reserve(bufferSize);
		normals[i].reserve(bufferSize);

		glGenVertexArrays(1, &m_vertexArrayObject[i]);
		glBindVertexArray(m_vertexArrayObject[i]);


		glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i][POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(glm::vec3), NULL, GL_DYNAMIC_COPY);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//glBindVertexArray(0);

		//glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i][TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(glm::vec2), NULL, GL_DYNAMIC_COPY);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//NORMALS
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i][NORMALS_VB]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(GL_INT), NULL, GL_DYNAMIC_COPY);

		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 1, GL_INT, GL_FALSE, 0);
		
		glBindVertexArray(0);
	}

}

void Display::AppendToDrawBuffer(Vertex* vertices, int numVertices, glm::vec3* offset, int dir, int type)
{
	for (int i = 0; i < numVertices; i++)
	{
		positions[type][m_bufferedVertices[type]] = vertices[i].pos + *offset;
		texCoords[type][m_bufferedVertices[type]] = vertices[i].texCoord;
		normals[type][m_bufferedVertices[type]] = dir;
		m_bufferedVertices[type]++;
	}
}

void Display::DrawBuffer(int type)
{
	glBindVertexArray(m_vertexArrayObject[type]);

	glDrawArrays(GL_TRIANGLES, 0, m_drawnVertices[type]);

	glBindVertexArray(0);
}