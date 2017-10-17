#pragma once

#include "mesh.h"
#include <vector>

class Blocks
{
public:
	Blocks(int numBlocks);
	virtual ~Blocks();
	Mesh* operator[](int ID)
	{
		return m_meshes[ID];
	}


private:
	std::vector<Mesh*> m_meshes;
};

