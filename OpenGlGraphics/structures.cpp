#include "structures.h"
#include <iostream>

Structures::Structures()
{
	structures.reserve(4);
	structures.push_back(Structure(15));

	structures[TREE0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE0].offsets.push_back(glm::vec3(0, 0, 0));

	structures[TREE0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE0].offsets.push_back(glm::vec3(0, 1, 0));

	structures[TREE0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE0].offsets.push_back(glm::vec3(0, 2, 0));

	structures[TREE0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE0].offsets.push_back(glm::vec3(0, 3, 0));

	structures[TREE0].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE0].offsets.push_back(glm::vec3(0, 4, 0));

	for (int x = -1; x <= 1; ++x)
		for (int z = -1; z <= 1; ++z)
		{
			structures[TREE0].ids.push_back(Blocks::BLOCK_LEAVES0);
			structures[TREE0].offsets.push_back(glm::vec3(x, 5, z));
		}

	structures[TREE0].ids.push_back(Blocks::BLOCK_LEAVES0);
	structures[TREE0].offsets.push_back(glm::vec3(0, 6, 0));


	//TREE1
	structures.push_back(Structure(40));

	structures[TREE1].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE1].offsets.push_back(glm::vec3(0, 0, 0));

	structures[TREE1].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE1].offsets.push_back(glm::vec3(0, 1, 0));

	structures[TREE1].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE1].offsets.push_back(glm::vec3(0, 2, 0));

	structures[TREE1].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE1].offsets.push_back(glm::vec3(0, 3, 0));

	structures[TREE1].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE1].offsets.push_back(glm::vec3(0, 4, 0));

	structures[TREE1].ids.push_back(Blocks::BLOCK_WOOD0);
	structures[TREE1].offsets.push_back(glm::vec3(0, 5, 0));

	for(int x = -2; x <= 2; ++x)
		for (int z = -2; z <= 2; ++z)
		{
			structures[TREE1].ids.push_back(Blocks::BLOCK_LEAVES0);
			structures[TREE1].offsets.push_back(glm::vec3(x, 6, z));
		}
	for (int x = -1; x <= 1; ++x)
		for (int z = -1; z <= 1; ++z)
		{
			structures[TREE1].ids.push_back(Blocks::BLOCK_LEAVES0);
			structures[TREE1].offsets.push_back(glm::vec3(x, 7, z));
		}
	//END TREE1

	structures.push_back(Structure(1));
	structures[FLOWER_PURPLE].ids.push_back(Blocks::BLOCK_FLOWER_PURPLE);
	structures[FLOWER_PURPLE].offsets.push_back(glm::vec3(0, 0, 0));

	structures.push_back(Structure(2));
	structures[CACTUS].ids.push_back(Blocks::BLOCK_CACTUS);
	structures[CACTUS].offsets.push_back(glm::vec3(0, 0, 0));

	structures[CACTUS].ids.push_back(Blocks::BLOCK_CACTUS);
	structures[CACTUS].offsets.push_back(glm::vec3(0, 1, 0));
}


Structures::~Structures()
{
	structures.clear();
}
