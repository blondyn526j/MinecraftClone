#include "optMesh.h"

OptMesh::OptMesh(Display* display, Vertex* up, Vertex* down, Vertex* front)
{
	m_up.reserve(6);
	m_down.reserve(6);
	m_front.reserve(6);

	for (int i = 0; i < 6; i++)
	{
		m_up.push_back(up[i]);
		m_down.push_back(up[i]);
		m_front.push_back(up[i]);
	}
}

OptMesh::~OptMesh()
{
	for (int i = 0; i < 6; i++)
	{
		delete(&m_up[i]);
		delete(&m_down[i]);
		delete(&m_front[i]);
	}
	m_up.clear();
	m_down.clear();
	m_front.clear();
}

void OptMesh::AppendToBuffer()
{
	//if(m_frontVisible)
}
