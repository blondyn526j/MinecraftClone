#include "chunkManager.h"
#define DRAW_DISTANCE 8
#define BUFFERWIDTH 10
#define SEALEVEL 60
#define DESERTSTEP 0.5

int m_mod(int b, int a)
{
	return (b % a + a) % a;
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

	m_noiseHeightMajor.SetNoiseType(m_noiseHeight.PerlinFractal);
	m_noiseHeightMajor.SetFrequency(100);
	m_noiseHeightMajor.SetSeed(6543);

	m_noiseHeight.SetNoiseType(m_noiseHeight.Perlin);
	m_noiseHeight.SetFrequency(300);
	m_noiseHeight.SetSeed(68429);

	m_noiseHeightMajor.SetNoiseType(m_noiseHeight.Perlin);
	m_noiseBiome.SetSeed(75423);
	m_noiseBiome.SetFrequency(10);

}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Draw(float x, float z)
{
	int xPos = floorf(x / CHUNKWIDTH);
	int zPos = floorf(z / CHUNKWIDTH);

	if (xPos != m_old_xPos || zPos != m_old_zPos)
	{
		m_display->ClearBuffer();

		for (int a = xPos - DRAW_DISTANCE; a <= xPos + DRAW_DISTANCE; a++)
			for (int b = zPos - DRAW_DISTANCE; b <= zPos + DRAW_DISTANCE; b++)
			{
				if (m_chunks[m_mod(a, BUFFERWIDTH) + m_mod(b, BUFFERWIDTH) * BUFFERWIDTH]->m_chunkRoot != glm::vec3(CHUNKWIDTH * a, 0, CHUNKWIDTH * b))
					LoadChunkFromFile(a, b);

				m_chunks[m_mod(a, BUFFERWIDTH) + m_mod(b, BUFFERWIDTH) * BUFFERWIDTH]->UpdateVisibility();
			}

		m_display->ReassignBuffer();
		m_old_xPos = xPos;
		m_old_zPos = zPos;
	}
}

void ChunkManager::UpdateVisiblity(float x, float z)
{
	int xPos = floorf(x / CHUNKWIDTH);
	int zPos = floorf(z / CHUNKWIDTH);

	if (xPos != m_old_xPos || zPos != m_old_zPos)
	{
		m_display->ClearBuffer();

		for (int a = xPos - DRAW_DISTANCE; a <= xPos + DRAW_DISTANCE; a++)
			for (int b = zPos - DRAW_DISTANCE; b <= zPos + DRAW_DISTANCE; b++)
			{
				if (m_chunks[m_mod(a, BUFFERWIDTH) + m_mod(b, BUFFERWIDTH) * BUFFERWIDTH]->m_chunkRoot != glm::vec3(CHUNKWIDTH * a, 0, CHUNKWIDTH * b))
					LoadChunkFromFile(a, b);

				m_chunks[m_mod(a, BUFFERWIDTH) + m_mod(b, BUFFERWIDTH) * BUFFERWIDTH]->UpdateVisibility();
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
	m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)] = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z), m_shader, m_transform, m_blocks, m_display);

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
	int blocksDrawn = 0;
	for (int ay = 0; ay < CHUNKHEIGHT; ay++)
	{
		for (int az = 0; az < CHUNKWIDTH; az++)
		{
			for (int ax = 0; ax < CHUNKWIDTH; ax++)
			{
				double coordX = (double)(x * CHUNKWIDTH + ax) / 10000;
				double coordZ = (double)(z * CHUNKWIDTH + az) / 10000;

				if (ay < 10 * (m_noiseHeight.GetNoise(coordX, coordZ) + 1) + 55 * (m_noiseHeightMajor.GetNoise(coordX, coordZ) + 1))
				{
					if (m_noiseBiome.GetNoise(coordX, coordZ) > DESERTSTEP)
						ids[blocksDrawn] = 1;
					else
						ids[blocksDrawn] = 2;
				}
				else if (ay < SEALEVEL)
				{
					ids[blocksDrawn] = 3;
				}
				else
					ids[blocksDrawn] = 0;

				blocksDrawn++;
			}
		}
	}

	Chunk* chunk = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z), m_shader, m_transform, m_blocks, m_display);
	m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)] = chunk;
	return chunk;
}
