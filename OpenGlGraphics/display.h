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
	void AppendToDrawBuffer(Vertex* vertices, int numVertices, glm::vec3* offset, int type);
	void DrawBuffer(int type);
	void ClearBuffer();
	void ReassignBuffer();

	virtual ~Display();
protected:
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed = false;
	
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,

		INDEX_VB,

		NUM_BUFFERS
	};

	std::vector<glm::vec3> positions[NUM_TYPES];
	std::vector<glm::vec2> texCoords[NUM_TYPES];
	int m_bufferedVertices[NUM_TYPES] = { 0, 0 };
	GLuint m_vertexArrayObject[NUM_TYPES];
	GLuint m_vertexArrayBuffers[NUM_TYPES][NUM_BUFFERS];
};