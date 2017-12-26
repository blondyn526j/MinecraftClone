#pragma once
#include "blocks.h"
#include "display.h"
#include "FastNoise.h"
#include "structures.h"

#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <glm/glm.hpp> 

#define CHUNKWIDTH 16
#define CHUNKHEIGHT 256
#define CHUNKSIZE CHUNKWIDTH*CHUNKWIDTH*CHUNKHEIGHT

class Chunk
{
public:
	int chunkSize = 8;
	bool structuresGenerated = false;
	Chunk(char* ids, glm::vec3 chunkRoot, bool structuresGenerated = false)
	{
		blockIDs = ids;
		/*m_shader = shader;
		m_transform = transform;
		m_blocks = blocks;
		m_display = display;*/
		this->chunkRoot = chunkRoot;
		this->structuresGenerated = structuresGenerated;

		//m_visiblilityArray = new char[CHUNKSIZE];
	}
	virtual ~Chunk() 
	{
		delete[] blockIDs;
	};

	char* blockIDs;

	glm::vec3 chunkRoot = glm::vec3(0, 0, 0);
private:
};

class ChunkManager
{
public:
	ChunkManager(Blocks* blocks, Display* display);
	virtual ~ChunkManager();
	void LoadChunkFromFile(int x, int z);
	void LoadWorld();
	void SaveChunkToFile(int x, int z, Chunk* chunk);
	Chunk* GenerateChunk(int x, int z);
	void Draw(float x, float z);

	char& m_xyzToBlock(float globalX, float globalY, float globalZ);
private:
	//Chunk* m_chunk;
	void DrawChunk(int ax, int az);
	void UpdateBuffer(int x, int z);
	void GenerateStructure(int type, int x, int y, int z);
	void GenerateTrees(int x, int z);
	int GetGroudLevel(double x, double z); /*GLOBAL*/
	void ReplaceBlockInFile(char c, int globalX, int globalY, int globalZ);
	void SetTreesGeneratedInFile(int globalX, int globalZ);
	Blocks* m_blocks;
	Display* m_display;
	std::vector<Chunk*> m_chunks;
	int m_old_xPos = -100;
	int m_old_zPos = -100;
	FastNoise m_mapHeightMin;
	FastNoise m_mapHeightMed;
	FastNoise m_mapHeightMaj;
	FastNoise m_mapHeightContinental;

	FastNoise m_mapTemp;
	FastNoise m_mapVariety;
	FastNoise m_mapSandArea;
	FastNoise m_mapBeachHeight;
	FastNoise m_mapTreeDensity;
	FastNoise m_mapFoliageDensity;

	std::thread m_loadingThread;

	bool isTransparent(int idOther, int idThis);
	Structures m_structures;
	bool m_bufferNeedsToBeReAssigned = true;
	bool m_loadingThreadRunning = false;

	enum {
		UP,
		FRONT,
		DOWN,
		RIGHT,
		BACK,
		LEFT
	};
};
