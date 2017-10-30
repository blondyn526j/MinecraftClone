#pragma once
#include <sdl2\SDL.h>
#include <string>
#include "input.h"
#include "Vertex.h"

class Display
{
public:

	enum bufferTypes
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
	void DrawBuffer(bufferTypes type);
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

	std::vector<glm::vec3> positions[bufferTypes::NUM_TYPES];
	std::vector<glm::vec2> texCoords[bufferTypes::NUM_TYPES];
	int m_bufferedVertices[bufferTypes::NUM_TYPES] = { 0, 0 };
	GLuint m_vertexArrayObject[bufferTypes::NUM_TYPES];
	GLuint m_vertexArrayBuffers[bufferTypes::NUM_TYPES][NUM_BUFFERS];
};