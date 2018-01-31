#pragma once

#include "mesh.h"
#include <vector>

class Blocks
{
public:
	Blocks();
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
		BLOCK_WOOD0,
		BLOCK_GRASSC,
		BLOCK_SAND0,
		BLOCK_LEAVES0,
		BLOCK_FLOWER_PURPLE,
		BLOCK_CACTUS,
		BLOCK_BUSH0,
		BLOCK_BUSH1,
		BLOCK_LEAVES_SNOWY,
		BLOCK_WOOD1,
		NUM_BLOCKS
	};

private:
	std::vector<Mesh*> m_meshes;
};

