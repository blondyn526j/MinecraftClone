#include "chunkManager.h"
#define DRAW_DISTANCE 8
#define BUFFERWIDTH 22
#define SEALEVEL 55
#define DESERTSTEP 0.5

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
double m_clamp(const double val, const double max, const double min)
{
	return std::fmin(max, std::fmax(val, min));
}

ChunkManager::ChunkManager(Blocks* blocks, Display* display)
{
	//m_shader = shader;
	//m_transform = transform;
	m_blocks = blocks;
	m_display = display;
	m_structures = Structures();

	m_chunks.reserve(BUFFERWIDTH * BUFFERWIDTH);
	for (int i = 0; i < BUFFERWIDTH * BUFFERWIDTH; i++)
		m_chunks.push_back(nullptr);

	m_mapHeightContinental.SetNoiseType(FastNoise::Perlin);
	m_mapHeightContinental.SetFrequency(1000);
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

	m_mapTemp.SetNoiseType(FastNoise::PerlinFractal);
	m_mapTemp.SetFrequency(2000);
	m_mapTemp.SetInterp(FastNoise::Hermite);
	m_mapTemp.SetSeed(626685);

	m_mapVariety.SetNoiseType(FastNoise::Perlin);
	m_mapVariety.SetFrequency(3000);
	m_mapVariety.SetInterp(FastNoise::Hermite);
	m_mapVariety.SetSeed(80465);

	m_mapSandArea.SetNoiseType(FastNoise::PerlinFractal);
	m_mapSandArea.SetFrequency(3000);
	m_mapSandArea.SetInterp(FastNoise::Hermite);
	m_mapSandArea.SetSeed(73651);
	m_mapSandArea.SetFractalOctaves(5);

	m_mapBeachHeight.SetNoiseType(FastNoise::Perlin);
	m_mapBeachHeight.SetFrequency(18000);
	m_mapBeachHeight.SetInterp(FastNoise::Hermite);
	m_mapBeachHeight.SetSeed(761651);

	srand(624610);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Draw(float x, float z)
{
	m_display->Clear(0.7f + (m_mapTemp.GetNoise(x / 1000000, z / 1000000)) * 0.5, 0.9f + (m_mapTemp.GetNoise(x / 1000000, z / 1000000)) * 0.08, 0.98f, 1.0f);
	//std::cout << 2 * (m_mapTemp.GetNoise(x / 1000000, z / 1000000)) << std::endl;

	int xPos = x / CHUNKWIDTH;
	int zPos = z / CHUNKWIDTH;

	if (xPos != m_old_xPos || zPos != m_old_zPos)
	{
		m_display->ClearBuffer();

		if (m_loadingThread.joinable())
			m_loadingThread.join();

		m_loadingThread = std::thread(&ChunkManager::UpdateBuffer, this, xPos, zPos);
		//UpdateBuffer(xPos, zPos);

		m_old_xPos = xPos;
		m_old_zPos = zPos;
	}

	if (m_bufferNeedsToBeReAssigned)
	{
		m_display->ReassignBuffer();
		m_bufferNeedsToBeReAssigned = false;
	}
}

void ChunkManager::UpdateBuffer(int x, int z)
{
	for (int a = x - DRAW_DISTANCE; a <= x + DRAW_DISTANCE; a++)
		for (int b = z - DRAW_DISTANCE; b <= z + DRAW_DISTANCE; b++)
			if (m_chunks[m_mod(a, BUFFERWIDTH) + m_mod(b, BUFFERWIDTH) * BUFFERWIDTH]->chunkRoot != glm::vec3(CHUNKWIDTH * a, 0, CHUNKWIDTH * b))
				LoadChunkFromFile(a, b);

	for (int a = x - DRAW_DISTANCE; a <= x + DRAW_DISTANCE; a++)
		for (int b = z - DRAW_DISTANCE; b <= z + DRAW_DISTANCE; b++)
		{
			DrawChunk(a, b);
		}

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
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[18], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn + 1], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[18], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
						}
					}

					if (x == 0)
					{
						if (isTransparent(m_chunks[m_mod(ax - 1, BUFFERWIDTH) + m_mod(az, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn - 1 + CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[30], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn - 1], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[30], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
						}
					}
					//if (y == 0 || chunk->blockIDs[blocksDrawn - CHUNKWIDTH*CHUNKWIDTH] == 0)
					//	m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[12], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot));

					if (y != 0 && isTransparent(chunk->blockIDs[blocksDrawn - CHUNKWIDTH*CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[12], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), (chunk->blockIDs[blocksDrawn] != 3) ? Display::SOLID : Display::LIQUID);

					if (y == CHUNKHEIGHT - 1 || isTransparent(chunk->blockIDs[blocksDrawn + CHUNKWIDTH*CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[0], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), (chunk->blockIDs[blocksDrawn] != 3) ? Display::SOLID : Display::LIQUID);

					if (z == 0)
					{
						if (isTransparent(m_chunks[m_mod(ax, BUFFERWIDTH) + m_mod(az - 1, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn + CHUNKWIDTH * (CHUNKWIDTH - 1)], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[6], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn - CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[6], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
						}
					}

					if (z == CHUNKWIDTH - 1)
					{
						if (isTransparent(m_chunks[m_mod(ax, BUFFERWIDTH) + m_mod(az + 1, BUFFERWIDTH) * BUFFERWIDTH]->blockIDs[blocksDrawn - CHUNKWIDTH * (CHUNKWIDTH - 1)], chunk->blockIDs[blocksDrawn]))
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
					}
					else
					{
						if (isTransparent(chunk->blockIDs[blocksDrawn + CHUNKWIDTH], chunk->blockIDs[blocksDrawn]))
						{
							m_display->AppendToDrawBuffer(&(*m_blocks)[chunk->blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + chunk->chunkRoot), Display::SOLID);
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

	file.open(path);

	if (!file.good())
	{
		file.close();
		SaveChunkToFile(x, z, GenerateChunk(x, z));
		return;
		//file.open(path);
	}

	char* ids = new char[CHUNKSIZE];

	file.read(ids, sizeof(char) * CHUNKSIZE);

	delete(m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)]);
	m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)] = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z));

	file.close();

}

void ChunkManager::SaveChunkToFile(int x, int z, Chunk* chunk)
{
	std::ofstream file;
	file.open("world/" + std::to_string(x) + 'x' + std::to_string(z) + ".chf");

	file.write(chunk->blockIDs, sizeof(char) * CHUNKSIZE);

	file.close();
	std::cout << "Chunk Saved " << x << 'x' << z << std::endl;
}

Chunk* ChunkManager::GenerateChunk(int x, int z)
{
	char* ids = new char[CHUNKSIZE + 1];
	for (int az = 0; az < CHUNKWIDTH; az++)
	{
		for (int ax = 0; ax < CHUNKWIDTH; ax++)
		{
			double coordX = (double)(x * CHUNKWIDTH + ax) / 1000000;
			double coordZ = (double)(z * CHUNKWIDTH + az) / 1000000;
			double valContinental = m_clamp(m_mapHeightContinental.GetNoise(coordX, coordZ) + 0.5, 1, 0.02);
			double valMaj = m_clamp(m_mapHeightMaj.GetNoise(coordX, coordZ) + 0.5, 1, 0.02);
			double valMed = m_clamp((m_mapHeightMed.GetNoise(coordX, coordZ) + 0.5) / 2, 1, 0);
			double valMin = m_mapHeightMin.GetNoise(coordX, coordZ) + 0.5;
			double valVar = m_clamp(m_mapVariety.GetNoise(coordX, coordZ) + 0.5, 1.5, 0);
			double valTemp = (m_mapTemp.GetNoise(coordX, coordZ) + 1) / 2;
			double valSandArea = m_mapSandArea.GetNoise(coordX, coordZ);
			double valBeachHeight = m_mapBeachHeight.GetNoise(coordX, coordZ);
			
			int groundLevel =
				INFLUENCE_CONTINENTAL * valContinental +
				
				valVar * (
					INFLUENCE_MAJ * valMaj +
					INFLUENCE_MED * valMed +
					INFLUENCE_MIN * valMin);


			for (int ay = 0; ay < CHUNKHEIGHT; ay++)
			{
				if (ay < groundLevel)
				{
					if(valTemp < 0.4)
						ids[m_xyzToIndex(ax, ay, az)] = Blocks::BLOCK_GRASSC;
					else if (valTemp < 0.6)
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
			double coordX = (double)(x * CHUNKWIDTH + ax) / 1000000;
			double coordZ = (double)(z * CHUNKWIDTH + az) / 1000000;
			double valMaj = m_clamp(m_mapHeightMaj.GetNoise(coordX, coordZ) + 0.5, 1.5, 0.02);
			double valMed = m_clamp((m_mapHeightMed.GetNoise(coordX, coordZ) + 0.7) / 2, 1, 0);
			double valMin = m_mapHeightMin.GetNoise(coordX, coordZ) + 0.5;
			double valVar = m_clamp(m_mapVariety.GetNoise(coordX, coordZ) + 0.5, 1.5, 0);
			double valTemp = (m_mapTemp.GetNoise(coordX, coordZ) + 1) / 2;

			int groundLevel =
				INFLUENCE_MAJ * valMaj +
				valVar * (
					INFLUENCE_MED * valMed +
					INFLUENCE_MIN * valMin);

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

	ids[CHUNKSIZE] = 0;

	delete(m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)]);

	Chunk* chunk = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z));
	m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)] = chunk;
	return chunk;
}

void ChunkManager::GenerateTrees(int x, int z)
{
	if (m_chunks[m_mod(x, BUFFERWIDTH) + m_mod(z, BUFFERWIDTH) * BUFFERWIDTH] == 0 &&
		m_chunks[m_mod(x, BUFFERWIDTH) + m_mod(z, BUFFERWIDTH) * BUFFERWIDTH]->chunkRoot != glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z))
	{

	}
}

void ChunkManager::GenerateStructure(int type, int x, int y, int z)
{

}

bool ChunkManager::isTransparent(int idOther, int idThis)
{
	if (idOther != idThis && (idOther == Blocks::BLOCK_AIR || idOther == Blocks::BLOCK_WATER))
		return true;
	else
		return false;
}
