#pragma once

#include "blocks.h"
#include <vector>
#include <glm/glm.hpp>

#define NUM_STRUCTURES 1

class Structure
{
public:
	enum
	{
		TREE0
	};

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
	Structures();
	virtual ~Structures();

	std::vector<Structure> structures;

	Structure* operator[](int structureID)
	{
		return &structures[structureID];
	}

private:
};

