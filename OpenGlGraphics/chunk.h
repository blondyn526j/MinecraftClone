#pragma once
#include <vector>

#define CHUNKSIZE 8
class Chunk
{
public:
	int chunkSize = 8;
	int* blockIDs;
	Chunk(int* ids);
	virtual ~Chunk();
};

