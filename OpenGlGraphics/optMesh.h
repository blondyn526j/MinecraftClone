#pragma once

#include <glm/glm.hpp> 
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"
#include "display.h"

class OptMesh
{
public:
	OptMesh(Display* display, Vertex* up, Vertex* down, Vertex* front);
	virtual ~OptMesh();

	void AppendToBuffer();

private:
	std::vector<Vertex> m_up;
	std::vector<Vertex> m_down;
	std::vector<Vertex> m_front;


	bool m_frontVisible = true;
	bool m_backVisible = true;
	bool m_leftVisible = true;
	bool m_rightVisible = true;
	bool m_upVisible = true;
	bool m_downVisible = true;

	Display* m_display;
};