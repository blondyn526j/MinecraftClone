#pragma once

#include "mesh.h"
#include <vector>

class Blocks
{
public:
	Blocks(Display* display, int numBlocks);
	virtual ~Blocks();
	Mesh* operator[](int ID)
	{
		return m_meshes[ID-1];
	}


private:
	std::vector<Mesh*> m_meshes;
};

