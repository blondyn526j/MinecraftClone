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
		return m_meshes[ID-1];
	}

	enum
	{
		BLOCK_AIR,
		BLOCK_GRASS0,
		BLOCK_GRASS1,
		BLOCK_WATER,
		BLOCK_WOOD0
	};

private:
	std::vector<Mesh*> m_meshes;
};

