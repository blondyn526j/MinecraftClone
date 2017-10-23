#pragma once
#include <sdl2\SDL.h>
#include <string>
#include "input.h"
#include "Vertex.h"

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Update();
	bool IsClosed();
	void Clear(float red, float green, float blue, float alpha);
	void InitializeBuffer();
	void AppendToDrawBuffer(Vertex* vertices, int numVertices, glm::vec3* offset);
	void DrawBuffer();

	virtual ~Display();
protected:
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed = false;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	int m_bufferedVertices = 0;

	enum
	{
		POSITION_VB,
		TEXCOORD_VB,

		INDEX_VB,

		NUM_BUFFERS
	};
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
};