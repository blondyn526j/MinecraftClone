#include "chunkManager.h"

#define DRAW_DISTANCE 8
#define BUFFERWIDTH 20
#define SEALEVEL 55
#define DESERTSTEP 0.5
#define ASYNC_LOADING 1

#define INFLUENCE_CONTINENTAL 120
#define INFLUENCE_MAJ 80
#define INFLUENCE_MED 70
#define INFLUENCE_MIN 1.5

int m_mod(const int b, const int a)
{
	return (b % a + a) % a;
}
int m_xyzToIndex(const int x, const int y, const int z)
{
	return x + CHUNKWIDTH * z + CHUNKWIDTH * CHUNKWIDTH * y;
}

int m_xzToChunkIndex(int x, int z)
{
	return m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH);
}

double m_clamp(const double val, const double max, const double min)
{
	return std::fmin(max, std::fmax(val, min));
}

void GetCoordsOnMap(int chunkX, int chunkZ, int blockX, int blockZ, double& xCoord, double& zCoord)
{
	xCoord = (double)(chunkX * CHUNKWIDTH + blockX) / 1000000;
	zCoord = (double)(chunkZ * CHUNKWIDTH + blockZ) / 1000000;
}

char& ChunkManager::m_xyzToBlock(int globalX, int globalY, int globalZ)
{
	int chunkX = floor((double)globalX / CHUNKWIDTH);
	int chunkZ = floor((double)globalZ / CHUNKWIDTH);

	int blockX = m_mod(globalX, CHUNKWIDTH);
	int blockZ = m_mod(globalZ, CHUNKWIDTH);
	int blockY = globalY;

	Chunk* chunk = m_chunks[m_xzToChunkIndex(chunkX, chunkZ)];

	return chunk->blockIDs[m_xyzToIndex(blockX, blockY, blockZ)];
}

ChunkManager::ChunkManager(Blocks* blocks, Display* display)
{
	m_blocks = blocks;
	m_display = display;
	m_structures = Structures();

	m_chunks.reserve(BUFFERWIDTH * BUFFERWIDTH);
	for (int i = 0; i < BUFFERWIDTH * BUFFERWIDTH; i++)
		m_chunks.push_back(nullptr);

	m_mapHeightContinental.SetNoiseType(FastNoise::Perlin);
	m_mapHeightContinental.SetFrequency(850);
	m_mapHeightContinental.SetInterp(FastNoise::Hermite);
	m_mapHeightContinental.SetSeed(1075434);

	m_mapHeightMaj.SetNoiseType(FastNoise::Perlin);
	m_mapHeightMaj.SetFrequency(8000);
	m_mapHeightMaj.SetInterp(FastNoise::Hermite);
	m_mapHeightMaj.SetSeed(108134);

	m_mapHeightMed.SetNoiseType(FastNoise::PerlinFractal);
	m_mapHeightMed.SetFrequency(8000);
	m_mapHeightMed.SetInterp(FastNoise::Hermite);
	m_mapHeightMed.SetSeed(9663);

	m_mapHeightMin.SetNoiseType(FastNoise::Perlin);
	m_mapHeightMin.SetFrequency(110000);
	m_mapHeightMin.SetInterp(FastNoise::Hermite);
	m_mapHeightMin.SetSeed(626685);

	m_mapTemp.SetNoiseType(FastNoise::Perlin);
	m_mapTemp.SetFrequency(800);
	m_mapTemp.SetInterp(FastNoise::Hermite);
	m_mapTemp.SetSeed(626685);

	m_mapVariety.SetNoiseType(FastNoise::Perlin);
	m_mapVariety.SetFrequency(3000);
	m_mapVariety.SetInterp(FastNoise::Hermite);
	m_mapVariety.SetSeed(80465);

	m_mapTreeDensity.SetNoiseType(FastNoise::Perlin);
	m_mapTreeDensity.SetFrequency(4000);
	m_mapTreeDensity.SetInterp(FastNoise::Hermite);
	m_mapTreeDensity.SetSeed(19135);

	m_mapSandArea.SetNoiseType(FastNoise::PerlinFractal);
	m_mapSandArea.SetFrequency(3000);
	m_mapSandArea.SetInterp(FastNoise::Hermite);
	m_mapSandArea.SetSeed(73651);
	m_mapSandArea.SetFractalOctaves(5);

	m_mapBeachHeight.SetNoiseType(FastNoise::Perlin);
	m_mapBeachHeight.SetFrequency(18000);
	m_mapBeachHeight.SetInterp(FastNoise::Hermite);
	m_mapBeachHeight.SetSeed(761651);

	srand(4610);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Draw(float x, float z)
{
	//std::cout << x << " " << z << std::endl;

	m_display->Clear(0.7f + (m_mapTemp.GetNoise(x / 1000000, z / 1000000)) * 0.5, 0.9f + (m_mapTemp.GetNoise(x / 1000000, z / 1000000)) * 0.08, 0.98f, 1.0f);
	//std::cout << 2 * (m_mapTemp.GetNoise(x / 1000000, z / 1000000)) << std::endl;

	int xPos = floor((double)x / CHUNKWIDTH);
	int zPos = floor((double)z / CHUNKWIDTH);

	if (xPos != m_old_xPos || zPos != m_old_zPos)
	{
		m_display->ClearBuffer();

		if (m_loadingThread.joinable())
			m_loadingThread.join();

#if ASYNC_LOADING
		m_loadingThread = std::thread(&ChunkManager::UpdateBuffer, this, xPos, zPos);
#else
		UpdateBuffer(xPos, zPos);
#endif // ASYNC_LOADING

		m_old_xPos = xPos;
		m_old_zPos = zPos;
	}

	if (m_bufferNeedsToBeReAssigned)
	{
		m_display->ReassignBuffer();
		m_bufferNeedsToBeReAssigned = false;
	}

	//ReplaceBlockInFile(Blocks::BLOCK_WOOD0, x, 110 , z);
}

void ChunkManager::UpdateBuffer(int chunkX, int chunkZ)
{
	for (int x = chunkX - DRAW_DISTANCE; x <= chunkX + DRAW_DISTANCE; x++)
		for (int z = chunkZ - DRAW_DISTANCE; z <= chunkZ + DRAW_DISTANCE; z++)
			if (m_chunks[m_mod(x, BUFFERWIDTH) + m_mod(z, BUFFERWIDTH) * BUFFERWIDTH]->chunkRoot != glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z))
				LoadChunkFromFile(x, z);

	//GENERATING TREES
	for (int x = chunkX - DRAW_DISTANCE + 1; x <= chunkX + DRAW_DISTANCE - 1; x++)
		for (int z = chunkZ - DRAW_DISTANCE + 1; z <= chunkZ + DRAW_DISTANCE - 1; z++)
		{
			//std::cout << x << '-' << z << std::endl;
			GenerateTrees(x, z);
		}
	//END GENERATING TREES

	//DRAW CHUNKS
	for (int x = chunkX - DRAW_DISTANCE; x <= chunkX + DRAW_DISTANCE; x++)
		for (int z = chunkZ - DRAW_DISTANCE; z <= chunkZ + DRAW_DISTANCE; z++)
		{
			DrawChunk(x, z);
		}
	//END DRAWING CHUNKS

	//std::cout << "Trees: " << m_chunks[m_xzToChunkIndex()]

	m_bufferNeedsToBeReAssigned = true;
}

void ChunkManager::DrawChunk(int ax, int az)
{
	Chunk* chunk = m_chunks[m_mod(ax, BUFFERWIDTH) + m_mod(az, BUFFERWIDTH) * BUFFERWIDTH];
	int blocksDrawn = 0;
	for (int y = 0; y < CHUNKHEIGHT; y++)
	{
		for (int z = 0; z < CHUNKWIDTH; z++)
		{
			for (int x = 0; x < CHUNKWIDTH; x++)
			{
				if (chunk->blockIDs[blocksDrawn] > 0)
				{
					//0 - UP - 0
					//1 - FRONT - 6
					//2 - DOWN - 12
					//3 - RIGHT - 18
					//4 - BACK - 24
					//5 - LEFT - 30
					if (x == CHUNKWIDTH - 1)
					{
						if (isTransparent(m_chunks[m_mod(ax + 1, BUFFERWIDTH) + m_mod(az, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn + 1 - CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[18], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), RIGHT, Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn + 1], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[18], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), RIGHT, Display::SOLID);
						}
					}

					if (x == 0)
					{
						if (isTransparent(m_chunks[m_mod(ax - 1, BUFFERWIDTH) + m_mod(az, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn - 1 + CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[30], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), LEFT, Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn - 1], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[30], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), LEFT, Display::SOLID);
						}
					}
					//if (y == 0 || chunk->blockIDs[blocksDrawn - CHUNKWIDTH*CHUNKWIDTH] == 0)
					//	m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[12], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot));

					if (y != 0 && isTransparent(chunk->blockIDs[blocksDrawn - CHUNKWIDTH*CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[12], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), DOWN, (chunk->blockIDs[blocksDrawn] != 3) ? Display::SOLID : Display::LIQUID);

					if (y == CHUNKHEIGHT - 1 || isTransparent(chunk->blockIDs[blocksDrawn + CHUNKWIDTH*CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[0], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), DOWN, (chunk->blockIDs[blocksDrawn] != 3) ? Display::SOLID : Display::LIQUID);

					if (z == 0)
					{
						if (isTransparent(m_chunks[m_mod(ax, BUFFERWIDTH) + m_mod(az - 1, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn + CHUNKWIDTH * (CHUNKWIDTH - 1)], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[6], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), FRONT, Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn - CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[6], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), FRONT, Display::SOLID);
						}
					}

					if (z == CHUNKWIDTH - 1)
					{
						if (isTransparent(m_chunks[m_mod(ax, BUFFERWIDTH) + m_mod(az + 1, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn - CHUNKWIDTH * (CHUNKWIDTH - 1)], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), BACK, Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn + CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), BACK, Display::SOLID);
						}
					}

					//if (z == CHUNKWIDTH - 1 || chunk->blockIDs[blocksDrawn + CHUNKWIDTH] == 0)
					//	m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot));

					//m_visiblilityArray[blocksDrawn] |= 1 << 0;
				}
				blocksDrawn++;
			}
		}
	}
}

void ChunkManager::LoadWorld()
{
	for (int x = 0; x < BUFFERWIDTH; x++)
		for (int z = 0; z < BUFFERWIDTH; z++)
		{
			LoadChunkFromFile(x, z);
		}
}

void ChunkManager::LoadChunkFromFile(int x, int z)
{
	std::string path = "world/" + std::to_string(x) + 'x' + std::to_string(z) + ".chf";

	std::ifstream file;

	file.open(path, std::fstream::binary | std::fstream::in);

	if (!file.good())
	{
		file.close();
		SaveChunkToFile(x, z, GenerateChunk(x, z));
		return;
	}

	char* ids = new char[CHUNKSIZE];

	file.read(ids, sizeof(char) * CHUNKSIZE);
	char* treesInput = new char[1];
	file.read(treesInput, sizeof(char));

	if (m_chunks[m_xzToChunkIndex(x, z)] != nullptr)
		delete(m_chunks[m_xzToChunkIndex(x, z)]);
	m_chunks[m_xzToChunkIndex(x, z)] = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z), treesInput[0] == 't' ? true : false);

	file.close();
}

void ChunkManager::SaveChunkToFile(int x, int z, Chunk* chunk)
{
	std::ofstream file;
	file.open("world/" + std::to_string(x) + 'x' + std::to_string(z) + ".chf", std::fstream::binary | std::fstream::out);

	file.write(chunk->blockIDs, sizeof(char) * CHUNKSIZE);
	file.write((chunk->structuresGenerated ? "t" : "f"), sizeof(char));

	file.close();
	std::cout << "Chunk Saved " << x << 'x' << z << std::endl;
}

Chunk* ChunkManager::GenerateChunk(int x, int z)
{
	char* ids = new char[CHUNKSIZE];
	for (int az = 0; az < CHUNKWIDTH; az++)
	{
		for (int ax = 0; ax < CHUNKWIDTH; ax++)
		{
			double xCoord = 0;
			double zCoord = 0;
			GetCoordsOnMap(x, z, ax, az, xCoord, zCoord);
			double valTemp = (m_mapTemp.GetNoise(xCoord, zCoord) + 1) / 2;
			double valSandArea = m_mapSandArea.GetNoise(xCoord, zCoord);
			double valBeachHeight = m_mapBeachHeight.GetNoise(xCoord, zCoord);

			int groundLevel = GetGroudLevel(xCoord, zCoord);


			for (int ay = 0; ay < CHUNKHEIGHT; ay++)
			{
				if (ay < groundLevel)
				{
					if (valTemp < 0.45)
						ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASSC;
					else if (valTemp < 0.55)
						ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASS0;
					else
						ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_SAND0;

					/*if (ay < SEALEVEL + 2 + valBeachHeight * 4)
					{
						double sandRand = (double)(rand() % 200 - 100) * 0.0003;
						if(valSandArea > 0.03)
							ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_SAND0;
						else if(valSandArea < -0.03)
							ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASS0;
						else if(sandRand < valSandArea)
							ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_SAND0;
						else
							ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASS0;
					}
					else
					{
						if (valVar > DESERTSTEP)
							ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASS0;
						else
							ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASS1;
					}*/

				}
				else if (ay < SEALEVEL)
				{
					ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_WATER;
				}
				else
					ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_AIR;
			}
		}
	}

	/*
	for (int az = 0; az < CHUNKWIDTH; az++)
	{
		for (int ax = 0; ax < CHUNKWIDTH; ax++)
		{
			double xCoord = 0;
			double zCoord = 0;
			GetCoordsOnMap(x, z, ax, az, xCoord, zCoord);
			double valTemp = (m_mapTemp.GetNoise(xCoord, zCoord) + 1) / 2;
			double valSandArea = m_mapSandArea.GetNoise(xCoord, zCoord);
			double valBeachHeight = m_mapBeachHeight.GetNoise(xCoord, zCoord);

			int groundLevel = GetGroudLevel(xCoord, zCoord);

			if (ax == 0 && az == 0)
			{
				Structure* structure = m_structures[0];
				for (int i = 0; i < structure->length; i < i++)
				{
					ids[m_xyzToIndex(ax + structure->offsets[i].x, groundLevel + structure->offsets[i].y, az + structure->offsets[i].z)] = structure->ids[i];
				}
			}

		}
	}*/

	delete(m_chunks[m_xzToChunkIndex(x, z)]);

	Chunk* chunk = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z));
	m_chunks[m_xzToChunkIndex(x, z)] = chunk;

	//GenerateTrees(x, z);

	return chunk;
}

int ChunkManager::GetGroudLevel(double x, double z)
{
	double valContinental = m_clamp(m_mapHeightContinental.GetNoise(x, z) + 0.5, 1, 0.02);
	double valMaj = m_clamp(m_mapHeightMaj.GetNoise(x, z) + 0.5, 1, 0.02);
	double valMed = m_clamp((m_mapHeightMed.GetNoise(x, z) + 0.5) / 2, 1, 0);
	double valMin = m_mapHeightMin.GetNoise(x, z) + 0.5;
	double valVar = m_clamp(m_mapVariety.GetNoise(x, z) + 0.5, 1.5, 0);

	int groundLevel =
		INFLUENCE_CONTINENTAL * valContinental +
		valVar * (
			INFLUENCE_MAJ * valMaj +
			INFLUENCE_MED * valMed +
			INFLUENCE_MIN * valMin);

	return groundLevel;
}

void ChunkManager::GenerateTrees(int chunkX, int chunkZ)
{
	if (m_chunks[m_xzToChunkIndex(chunkX, chunkZ)]->structuresGenerated)
	{
		return;
	}

	m_chunks[m_xzToChunkIndex(chunkX, chunkZ)]->structuresGenerated = true;
	SetTreesGeneratedInFile(chunkX, chunkZ);

	double xCoord = 0;
	double zCoord = 0;
	for (int blockX = 0; blockX < 16; blockX++)
		for (int blockZ = 0; blockZ < 16; blockZ++)
		{
			GetCoordsOnMap(chunkX, chunkZ, blockX, blockZ, xCoord, zCoord);
			int groundLevel = GetGroudLevel(xCoord, zCoord);
			if (groundLevel >= SEALEVEL)
			{
				double valTreeDensity = m_mapTreeDensity.GetNoise(xCoord, zCoord) * 20 + 4;
				int random = rand() % 1000;
				if (random < valTreeDensity)
				{
					switch (m_xyzToBlock(chunkX * CHUNKWIDTH + blockX, groundLevel - 1, chunkZ * CHUNKWIDTH + blockZ))
					{
					case Blocks::BLOCK_SAND0:
						if(rand() % 3 == 0)
						GenerateStructure(Structures::CACTUS, chunkX * CHUNKWIDTH + blockX, groundLevel, chunkZ * CHUNKWIDTH + blockZ);
					break;

					case Blocks::BLOCK_GRASS0:
						if (rand() % 2 != 0)
						GenerateStructure(Structures::TREE0, chunkX * CHUNKWIDTH + blockX, groundLevel, chunkZ * CHUNKWIDTH + blockZ);
						else
						GenerateStructure(Structures::TREE1, chunkX * CHUNKWIDTH + blockX, groundLevel, chunkZ * CHUNKWIDTH + blockZ);
						break;
					}
					//GenerateStructure(Structures::CACTUS, chunkX * CHUNKWIDTH + blockX, groundLevel, chunkZ * CHUNKWIDTH + blockZ);
					//std::cout << random << '-' << valTreeDensity << std::endl;
				}
			}

		}
}

void ChunkManager::GenerateStructure(int type, int globalX, int globalY, int globalZ)
{
	std::string oldPath = "000";
	Structure* structure = m_structures[type];
	std::fstream file;

	for (int i = 0; i < structure->length; i < ++i)
	{
		m_xyzToBlock(globalX + structure->offsets[i].x, globalY + structure->offsets[i].y, globalZ + structure->offsets[i].z) = structure->ids[i];

		int chunkX = floor((double)(globalX + structure->offsets[i].x) / CHUNKWIDTH);
		int chunkZ = floor((double)(globalZ + structure->offsets[i].z) / CHUNKWIDTH);

		int blockX = m_mod(globalX + structure->offsets[i].x, CHUNKWIDTH);
		int blockZ = m_mod(globalZ + structure->offsets[i].z, CHUNKWIDTH);
		int blockY = globalY + structure->offsets[i].y;

		std::string path = "world/" + std::to_string(chunkX) + 'x' + std::to_string(chunkZ) + ".chf";

		if (path != oldPath)
		{
			if (file.is_open())
				file.close();

			file.open(path, std::fstream::binary | std::fstream::out | std::fstream::in);
		}
		//file.open(path, std::fstream::binary | std::fstream::out | std::fstream::in);
		char writeBuffer[] = { structure->ids[i] };

		file.seekp(m_xyzToIndex(blockX, blockY, blockZ));
		file.write(writeBuffer, sizeof(char));

		oldPath = path;
		//ReplaceBlockInFile((char)type, globalX + structure->offsets[i].x, globalY + structure->offsets[i].y, globalZ + structure->offsets[i].z);
	}
}

void ChunkManager::ReplaceBlockInFile(char c, int globalX, int globalY, int globalZ)
{
	///USELESS
	int chunkX = floor((double)globalX / CHUNKWIDTH);
	int chunkZ = floor((double)globalZ / CHUNKWIDTH);

	int blockX = m_mod(globalX, CHUNKWIDTH);
	int blockZ = m_mod(globalZ, CHUNKWIDTH);
	int blockY = globalY;

	std::string path = "world/" + std::to_string(chunkX) + 'x' + std::to_string(chunkZ) + ".chf";

	std::fstream file;
	file.open(path, std::fstream::binary | std::fstream::out | std::fstream::in);
	char writeBuffer[] = { c };


	file.seekp(m_xyzToIndex(blockX, blockY, blockZ));
	file.write(writeBuffer, sizeof(char));
	file.close();
}

void ChunkManager::SetTreesGeneratedInFile(int chunkX, int chunkZ)
{
	std::string path = "world/" + std::to_string(chunkX) + 'x' + std::to_string(chunkZ) + ".chf";

	std::fstream file;
	file.open(path, std::fstream::binary | std::fstream::out | std::fstream::in);
	char writeBuffer[] = { 't' };
	file.seekp(CHUNKWIDTH*CHUNKWIDTH*CHUNKHEIGHT);
	file.write(writeBuffer, sizeof(char));
	file.close();
}

bool ChunkManager::isTransparent(int idOther, int idThis)
{
	if (idOther == Blocks::BLOCK_FLOWER_PURPLE || idOther == Blocks::BLOCK_CACTUS)
		return true;

	if (idOther != idThis && (idOther == Blocks::BLOCK_AIR || idOther == Blocks::BLOCK_WATER))
		return true;
	else
		return false;
}