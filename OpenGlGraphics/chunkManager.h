#pragma once
#include "chunk.h"
#include "shader.h"
#include "transform.h"
#include "blocks.h"
#include "display.h"
#include "FastNoise.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp> 


class ChunkManager
{
public:
	ChunkManager(Shader* shader, Transform* transform, Blocks* blocks, Display* display);
	virtual ~ChunkManager();
	void LoadChunkFromFile(int x, int z);
	void LoadWorld();
	void SaveChunkToFile(int x, int z, Chunk* chunk);
	Chunk* GenerateChunk(int x, int z);
	void Draw(float x, float z);

private:
	//Chunk* m_chunk;
	void DrawChunk(int ax, int az);
	Shader* m_shader;
	Transform* m_transform;
	Blocks* m_blocks;
	Display* m_display;
	std::vector<Chunk*> m_chunks;
	int m_old_xPos = -100;
	int m_old_zPos = -100;
	FastNoise m_mapHeightMin;
	FastNoise m_mapHeightMed;
	FastNoise m_mapHeightMaj;
	FastNoise m_mapTemp;
	FastNoise m_mapVariety;

	bool isTransparent(int idOther, int idThis);
};

