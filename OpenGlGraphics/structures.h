#pragma once

#include "blocks.h"
#include <vector>
#include <glm/glm.hpp>

class Structure
{
public:
	Structure(int lenght)
	{
		this->length = lenght;
		ids.reserve(lenght);
		offsets.reserve(lenght);
	}

	unsigned int length;
	std::vector< glm::vec3 > offsets;
	std::vector< char > ids;
};

class Structures
{
public:
	enum
	{
		TREE0,
		TREE1,
		FLOWER_PURPLE,
		CACTUS, 
		BUSH0,
		BUSH_D,
		TREE_C,
		TREE2,
		TREE_C2,

		NUM_STRUCTURES
	};

	Structures();
	virtual ~Structures();

	std::vector<Structure> structures;

	Structure* operator[](int structureID)
	{
		return &structures[structureID];
	}

private:
};

