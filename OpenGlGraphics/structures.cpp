#include "structures.h"
#include <iostream>

Structures::Structures()
{
	structures.reserve(1);
	structures.push_back(Structure(11));

	structures[0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[0].offsets.push_back(glm::vec3(0, 0, 0));

	structures[0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[0].offsets.push_back(glm::vec3(0, 1, 0));

	structures[0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[0].offsets.push_back(glm::vec3(0, 2, 0));

	structures[0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[0].offsets.push_back(glm::vec3(0, 3, 0));

	structures[0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[0].offsets.push_back(glm::vec3(0, 4, 0));

	structures[0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[0].offsets.push_back(glm::vec3(0, 5, 0));

	structures[0].ids.push_back(Blocks::BLOCK_GRASS0);
	structures[0].offsets.push_back(glm::vec3(1, 6, 0));

	structures[0].ids.push_back(Blocks::BLOCK_GRASS0);
	structures[0].offsets.push_back(glm::vec3(-1, 6, 0));

	structures[0].ids.push_back(Blocks::BLOCK_GRASS0);
	structures[0].offsets.push_back(glm::vec3(0, 6, 0));

	structures[0].ids.push_back(Blocks::BLOCK_GRASS0);
	structures[0].offsets.push_back(glm::vec3(0, 6, 1));

	structures[0].ids.push_back(Blocks::BLOCK_GRASS0);
	structures[0].offsets.push_back(glm::vec3(0, 6, -1));
}


Structures::~Structures()
{
	structures.clear();
}
