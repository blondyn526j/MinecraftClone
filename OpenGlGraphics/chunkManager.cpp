#include "chunkManager.h"
#define DRAW_DISTANCE 9
#define BUFFERWIDTH 22
#define SEALEVEL 55
#define DESERTSTEP 0.5

#define INFLUENCE_MAJ 160
#define INFLUENCE_MED 90
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

ChunkManager::ChunkManager(Shader* shader, Transform* transform, Blocks* blocks, Display* display)
{
	m_shader = shader;
	m_transform = transform;
	m_blocks = blocks;
	m_display = display;

	m_chunks.reserve(BUFFERWIDTH * BUFFERWIDTH);
	for (int i = 0; i < BUFFERWIDTH * BUFFERWIDTH; i++)
		m_chunks.push_back(nullptr);

	m_mapHeightMaj.SetNoiseType(FastNoise::PerlinFractal);
	m_mapHeightMaj.SetFrequency(1500);
	m_mapHeightMaj.SetInterp(FastNoise::Hermite);
	m_mapHeightMaj.SetSeed(10134);

	m_mapHeightMed.SetNoiseType(FastNoise::PerlinFractal);
	m_mapHeightMed.SetFrequency(8000);
	m_mapHeightMed.SetInterp(FastNoise::Hermite);
	m_mapHeightMed.SetSeed(9663);

	m_mapHeightMin.SetNoiseType(FastNoise::Perlin);
	m_mapHeightMin.SetFrequency(110000);
	m_mapHeightMin.SetInterp(FastNoise::Hermite);
	m_mapHeightMin.SetSeed(626685);

	m_mapTemp.SetNoiseType(FastNoise::Perlin);
	m_mapTemp.SetFrequency(700);
	m_mapTemp.SetInterp(FastNoise::Hermite);
	m_mapTemp.SetSeed(626685);

	m_mapVariety.SetNoiseType(FastNoise::Perlin);
	m_mapVariety.SetFrequency(3000);
	m_mapVariety.SetInterp(FastNoise::Hermite);
	m_mapVariety.SetSeed(80465);

	//for (float f = 0; f < 255; f += 0.01f)
	//	std::cout << m_mapVeriety.GetNoise(f, 0) << std::endl;

}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Draw(float x, float z)
{
	int xPos = x / CHUNKWIDTH;
	int zPos = z / CHUNKWIDTH;

	if (xPos != m_old_xPos || zPos != m_old_zPos)
	{
		m_display->ClearBuffer();

		for (int a = xPos - DRAW_DISTANCE; a <= xPos + DRAW_DISTANCE; a++)
			for (int b = zPos - DRAW_DISTANCE; b <= zPos + DRAW_DISTANCE; b++)
				if (m_chunks[m_mod(a, BUFFERWIDTH) + m_mod(b, BUFFERWIDTH) * BUFFERWIDTH]->chunkRoot != glm::vec3(CHUNKWIDTH * a, 0, CHUNKWIDTH * b))
					LoadChunkFromFile(a, b);

		for (int a = xPos - DRAW_DISTANCE; a <= xPos + DRAW_DISTANCE; a++)
			for (int b = zPos - DRAW_DISTANCE; b <= zPos + DRAW_DISTANCE; b++)
			{
				DrawChunk(a, b);
			}

		m_display->ReassignBuffer();
		m_old_xPos = xPos;
		m_old_zPos = zPos;
	}
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
	std::string path = "world/" + std::to_string(x) + 'x' + std::to_string(z) + ".cf";

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
	file.open("world/" + std::to_string(x) + 'x' + std::to_string(z) + ".cf");

	file.write(chunk->blockIDs, sizeof(char) * CHUNKSIZE);

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
			double coordX = (double)(x * CHUNKWIDTH + ax) / 1000000;
			double coordZ = (double)(z * CHUNKWIDTH + az) / 1000000;
			double valMaj = m_clamp(m_mapHeightMaj.GetNoise(coordX, coordZ) + 0.5, 1.5, 0.02);
			double valMed = m_clamp((m_mapHeightMed.GetNoise(coordX, coordZ) + 0.7) / 2, 1, 0);
			double valMin = m_mapHeightMin.GetNoise(coordX, coordZ) + 0.5;
			double valVar = m_clamp(m_mapVariety.GetNoise(coordX, coordZ) + 0.5, 1.5, 0);
			double valTemp = (m_mapTemp.GetNoise(coordX, coordZ) + 1) / 2;
			

			for (int ay = 0; ay < CHUNKHEIGHT; ay++)
			{
				int groundLevel =
					INFLUENCE_MAJ * valMaj +
					valVar * (
						INFLUENCE_MED * valMed +
						INFLUENCE_MIN * valMin);

				if (ay < groundLevel)
				{
					if (valVar > DESERTSTEP)
						ids[m_xyzToIndex(ax, ay, az)] = 1;
					else
						ids[m_xyzToIndex(ax, ay, az)] = 2;
				}
				else if (ay < SEALEVEL)
				{
					ids[m_xyzToIndex(ax, ay, az)] = 3;
				}
				else
					ids[m_xyzToIndex(ax, ay, az)] = 0;

				//if(ax == 0)
				//	ids[m_xyzToIndex(ax, ay, az)] = 0;
			}
		}
	}

	Chunk* chunk = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z));
	m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)] = chunk;
	return chunk;
}

bool ChunkManager::isTransparent(int idOther, int idThis)
{
	if (idOther != idThis && (idOther == 0 || idOther == 3))
		return true;
	else
		return false;
}
