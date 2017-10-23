#include "chunkManager.h"


ChunkManager::ChunkManager(Shader* shader, Transform* transform, Blocks* blocks, Display* display)
{
	m_shader = shader;
	m_transform = transform;
	m_blocks = blocks;
	m_display = display;

	char* ids = new char[512];
	for (int i = 0; i < 512; i++)
		ids[i] = 2;

	ids[0] = 0;
	ids[1] = 1;
	ids[9] = 1;
	ids[64] = 0;

	m_chunk = new Chunk(ids, shader, transform, blocks, display);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::Draw()
{
	m_chunk->UpdateVisibility();
}

void ChunkManager::LoadChunkFromFile(std::string fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (!file.good())
	{
		std::cerr << "Error: Couldn't open file!" << std::endl;
		return;
	}

	char* ids = new char[512];
	int i = 0;
	while (!file.eof())
	{
		file >> ids[i];
		i++;
	}
	delete(m_chunk->blockIDs);
	m_chunk->blockIDs = ids;

	file.close();
	std::cout << "Chunk Loaded Succesfully" << std::endl;
}

void ChunkManager::SaveChunkToFile(std::string fileName)
{
	std::ofstream file;
	file.open(fileName);
	
	for (int i = 0; i < 512; i++)
		file << m_chunk->blockIDs[i];

	file.close();
	std::cout << "Chunk Saved Succesfully" << std::endl;
}
