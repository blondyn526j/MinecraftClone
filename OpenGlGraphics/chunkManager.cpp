#include "chunkManager.h"
#define DRAW_DISTANCE 3
#define BUFFERWIDTH 10

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
				//std::cout << m_chunks[m_mod(a, 5) + m_mod(b, 5) * 5]->m_chunkRoot.z << std::endl;
			}

		m_display->ReassignBuffer();
		m_old_xPos = xPos;
		m_old_zPos = zPos;
	}

	//m_chunk->UpdateVisibility();
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
				//std::cout << m_chunks[m_mod(a, 5) + m_mod(b, 5) * 5]->m_chunkRoot.z << std::endl;
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
	std::string dir = "world/" + std::to_string(x) + 'x' + std::to_string(z);
	FILE* pFile;

	pFile = fopen(dir.c_str(), "r");

	if (pFile == NULL)
	{
		SaveChunkToFile(x, z, GenerateChunk(x, z));
		pFile = fopen(dir.c_str(), "r");
	}


	/*std::ifstream file;

	file.open("world/" + std::to_string(x) + 'x' + std::to_string(z));

	if (!file.good())
	{
		file.close();
		SaveChunkToFile(x, z, GenerateChunk(x, z));
		file.open("world/" + std::to_string(x) + 'x' + std::to_string(z));
	}*/

	char* ids = new char[CHUNKSIZE];
	/*int i = 0;
	while (!file.eof())
	{
		file >> ids[i];
		i++;
	}*/

	std::fread(ids, sizeof(char), CHUNKSIZE, pFile);
	//file.close();

	delete(m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)]);
	m_chunks[m_mod(x, BUFFERWIDTH) + BUFFERWIDTH * m_mod(z, BUFFERWIDTH)] = new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z), m_shader, m_transform, m_blocks, m_display);

	fclose(pFile);

	/*std::ifstream file;
	file.open("world/" + fileName);

	if (!file.good())
	{
		std::cerr << "Error: Couldn't open file!" << std::endl;
		return nullptr;
	}

	char* ids = new char[512];
	int i = 0;
	while (!file.eof())
	{
		file >> ids[i];
		i++;
	}
	//delete(m_chunk->blockIDs);
	//m_chunk->blockIDs = ids;

	file.close();
	std::cout << "Chunk Loaded Succesfully" << std::endl;

	return new Chunk(ids, m_shader, m_transform, m_blocks, m_display);*/
}

void ChunkManager::SaveChunkToFile(int x, int z, Chunk* chunk)
{
	std::ofstream file;
	file.open("world/" + std::to_string(x) + 'x' + std::to_string(z));

	for (int i = 0; i < CHUNKSIZE; i++)
		file << chunk->blockIDs[i];

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
				if (ay < 100 * sin((float)az / 180) + 1)
					ids[blocksDrawn] = 1;
				else
					ids[blocksDrawn] = 0;

				blocksDrawn++;
			}
		}
	}

	return new Chunk(ids, glm::vec3(CHUNKWIDTH * x, 0, CHUNKWIDTH * z), m_shader, m_transform, m_blocks, m_display);
}
