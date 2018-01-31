#pragma once
#include <sdl2\SDL.h>
#include <string>
#include "input.h"
#include "Vertex.h"

class Display
{
public:

	enum
	{
		SOLID,
		LIQUID,

		NUM_TYPES
	};

	Display(int width, int height, const std::string& title);

	void Update();
	bool IsClosed();
	void Clear(float red, float green, float blue, float alpha);
	void InitializeBuffer();
	void AppendToDrawBuffer(Vertex* vertices, int numVertices, glm::vec3* offset, int dir, int type);
	void DrawBuffer(int type);
	void ClearBuffer();
	void ReassignBuffer();

	void BindFrameBuffer(GLuint num)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, num);
	}

	virtual ~Display();
	GLuint m_texColor, m_texDepth, m_texSSAO;
	GLuint m_depthBuffer;
	GLuint m_fbo;
	GLuint m_rectangleVAO;

protected:
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed = false;
	
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,

		NORMALS_VB,

		NUM_BUFFERS
	};

	std::vector<glm::vec3> positions[NUM_TYPES];
	std::vector<glm::vec2> texCoords[NUM_TYPES];
	std::vector<int> normals[NUM_TYPES];
	int m_bufferedVertices[NUM_TYPES] = { 0, 0 };
	int m_drawnVertices[NUM_TYPES] = { 0, 0 };
	GLuint m_vertexArrayObject[NUM_TYPES];
	GLuint m_vertexArrayBuffers[NUM_TYPES][NUM_BUFFERS];
	
	//float m_texColorData[800][600];

	GLfloat fogStart = 10.0f;
	GLfloat fogEnd = 15.0f;
	int width = 800, height = 600;
};