#pragma once
#include <vector>
#include <glm/glm.hpp> 
#include "shader.h"
#include "transform.h"
#include "blocks.h"
#include "display.h"
#include <bitset>

#define CHUNKWIDTH 16
#define CHUNKHEIGHT 256
#define CHUNKSIZE CHUNKWIDTH*CHUNKWIDTH*CHUNKHEIGHT

class Chunk
{
public:
	int chunkSize = 8;
	bool treesGenerated = false;
	Chunk(char* ids, glm::vec3 chunkRoot);
	virtual ~Chunk();
	
	char* blockIDs;

	glm::vec3 chunkRoot = glm::vec3(0, 0, 0);
private:
};

