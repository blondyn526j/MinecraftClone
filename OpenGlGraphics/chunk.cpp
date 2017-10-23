#include "chunk.h"

Chunk::Chunk(char* ids, glm::vec3 chunkRoot, Shader* shader, Transform* transform, Blocks* blocks, Display* display)
{
	blockIDs = ids;
	m_shader = shader;
	m_transform = transform;
	m_blocks = blocks;
	m_display = display;
	m_chunkRoot = chunkRoot;

	m_visiblilityArray = new char[chunkSize * chunkSize * chunkSize];
}

Chunk::~Chunk()
{

}

void Chunk::DrawChunk()
{
	int blocksDrawn = 0;
	for (int y = 0; y < chunkSize; y++)
	{
		for (int z = 0; z < chunkSize; z++)
		{
			for (int x = 0; x < chunkSize; x++)
			{
				if (blockIDs[blocksDrawn]>0)
					m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[0], 36, &(glm::vec3(x, y, z)+m_chunkRoot));
				blocksDrawn++;
			}
		}
	}

	//m_shader->UpdateTransform(*m_transform);
	m_transform->pos = glm::vec3(0, 0, 0);
}

void Chunk::UpdateVisibility()
{
	int blocksDrawn = 0;
	for (int y = 0; y < chunkSize; y++)
	{
		for (int z = 0; z < chunkSize; z++)
		{
			for (int x = 0; x < chunkSize; x++)
			{
				if (blockIDs[blocksDrawn] > 0)
				{
					//0 - UP - 0
					//1 - FRONT - 6
					//2 - DOWN - 12
					//3 - RIGHT - 18
					//4 - BACK - 24
					//5 - LEFT - 30
					if (x == 7 || blockIDs[blocksDrawn + 1] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[18], 6, &(glm::vec3(x, y, z) + m_chunkRoot));

					if (x == 0 || blockIDs[blocksDrawn - 1] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[30], 6, &(glm::vec3(x, y, z) + m_chunkRoot));

					if (y == 0 || blockIDs[blocksDrawn - chunkSize*chunkSize] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[12], 6, &(glm::vec3(x, y, z) + m_chunkRoot));

					if (y == 7 || blockIDs[blocksDrawn + chunkSize*chunkSize] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[0], 6, &(glm::vec3(x, y, z) + m_chunkRoot));

					if (z == 0 || blockIDs[blocksDrawn - chunkSize] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[6], 6, &(glm::vec3(x, y, z) + m_chunkRoot));

					if (z == 7 || blockIDs[blocksDrawn + chunkSize] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + m_chunkRoot));

					//m_visiblilityArray[blocksDrawn] |= 1 << 0;
				}
				blocksDrawn++;
			}
		}
	}
}
