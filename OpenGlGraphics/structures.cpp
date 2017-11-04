#include "structures.h"
#include <iostream>

Structures::Structures()
{
	structures.reserve(1);
	structures.push_back(Structure(6));

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
	structures[0].offsets.push_back(glm::vec3(0, 6, 0));
}


Structures::~Structures()
{
	structures.clear();
}
