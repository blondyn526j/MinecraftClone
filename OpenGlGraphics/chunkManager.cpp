#include "chunkManager.h"
#define DRAW_DISTANCE 1

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

	m_chunks.reserve(25);
	for (int i = 0; i < 25; i++)
		m_chunks.push_back(nullptr);

	/*char* ids = new char[512];
	for (int i = 0; i < 512; i++)
		ids[i] = 2;

	ids[0] = 0;
	ids[1] = 1;
	ids[9] = 1;
	ids[64] = 0;*/

	//m_chunk = new Chunk(ids, shader, transform, blocks, display);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Draw(float x, float z)
{
	//m_chunks[0]->UpdateVisibility();

	/*for (int i = 0; i < 25; i++)
		if (m_chunks[i] != nullptr)
			m_chunks[i]->UpdateVisibility();*/
	int xPos = floorf(x/8.0f);
	int zPos = floorf(z/8.0f);
	
	for(int a = xPos-DRAW_DISTANCE; a <= xPos+DRAW_DISTANCE; a++)
		for (int b = zPos-DRAW_DISTANCE; b <= zPos+DRAW_DISTANCE; b++)
		{
			if(m_chunks[m_mod(a, 5) + m_mod(b, 5) * 5]->m_chunkRoot != glm::vec3(8 * a, 0, 8 * b))
			LoadChunkFromFile(a, b);

			m_chunks[m_mod(a, 5) + m_mod(b, 5) * 5]->UpdateVisibility();
			//std::cout << m_chunks[m_mod(a, 5) + m_mod(b, 5) * 5]->m_chunkRoot.z << std::endl;
		}

	//m_chunk->UpdateVisibility();
}

void ChunkManager::LoadWorld()
{
	for (int x = 0; x < 5; x++)
		for (int z = 0; z < 5; z++)
		{
			LoadChunkFromFile(x, z);
		}
}

void ChunkManager::LoadChunkFromFile(int x, int z)
{
	std::ifstream file;


	file.open("world/" + std::to_string(x) + 'x' + std::to_string(z));

	if (!file.good())
	{
		file.close();
		SaveChunkToFile(x, z, GenerateChunk(x, z));
		file.open("world/" + std::to_string(x) + 'x' + std::to_string(z));
	}

	char* ids = new char[512];
	int i = 0;
	while (!file.eof())
	{
		file >> ids[i];
		i++;
	}
	
	file.close();

	delete(m_chunks[m_mod(x, 5) + 5 * m_mod(z, 5)]);
	m_chunks[m_mod(x, 5) + 5 * m_mod(z, 5)] = new Chunk(ids, glm::vec3(8 * x, 0, 8 * z), m_shader, m_transform, m_blocks, m_display);

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

	for (int i = 0; i < 512; i++)
		file << chunk->blockIDs[i];

	file.close();
	std::cout << "Chunk Saved " << x << 'x' << z << std::endl;
}

Chunk* ChunkManager::GenerateChunk(int x, int z)
{
	char* ids = new char[512];
	for (int i = 0; i < 512; i++)
		ids[i] = 2;

	ids[510] = 1;
	ids[512] = 0;
	ids[506] = 0;
	ids[500] = 0;

	return new Chunk(ids, glm::vec3(8 * x, 0, 8 * z), m_shader, m_transform, m_blocks, m_display);
}
