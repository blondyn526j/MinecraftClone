#include "chunk.h"
/*
Chunk::Chunk(char* ids, glm::vec3 chunkRoot, bool structuresGenerated)
{
	blockIDs = ids;
	/*m_shader = shader;
	m_transform = transform;
	m_blocks = blocks;
	m_display = display;
	this->chunkRoot = chunkRoot;
	this->structuresGenerated = structuresGenerated;

	//m_visiblilityArray = new char[CHUNKSIZE];
}

Chunk::~Chunk()
{
	delete[](blockIDs);
}
*/
/*
void Chunk::DrawChunk()
{
	int blocksDrawn = 0;
	for (int y = 0; y < CHUNKHEIGHT; y++)
	{
		for (int z = 0; z < CHUNKWIDTH; z++)
		{
			for (int x = 0; x < CHUNKWIDTH; x++)
			{
				if (blockIDs[blocksDrawn] > 0)
					m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[0], 36, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);
				blocksDrawn++;
			}
		}
	}

	//m_shader->UpdateTransform(*m_transform);
	m_transform->pos = glm::vec3(0, 0, 0);
}
*/

/*
void Chunk::UpdateVisibility()
{
	int blocksDrawn = 0;
	for (int y = 0; y < CHUNKHEIGHT; y++)
	{
		for (int z = 0; z < CHUNKWIDTH; z++)
		{
			for (int x = 0; x < CHUNKWIDTH; x++)
			{
				if (blockIDs[blocksDrawn] > 0)
				{
					//0 - UP - 0
					//1 - FRONT - 6
					//2 - DOWN - 12
					//3 - RIGHT - 18
					//4 - BACK - 24
					//5 - LEFT - 30
					if (x == CHUNKWIDTH-1 || blockIDs[blocksDrawn + 1] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[18], 6, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);

					if (x == 0 || blockIDs[blocksDrawn - 1] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[30], 6, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);

					if (y == 0 || blockIDs[blocksDrawn - CHUNKWIDTH*CHUNKWIDTH] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[12], 6, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);

					if (y == CHUNKHEIGHT-1 || blockIDs[blocksDrawn + CHUNKWIDTH*CHUNKWIDTH] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[0], 6, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);

					if (z == 0 || blockIDs[blocksDrawn - CHUNKWIDTH] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[6], 6, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);

					if (z == CHUNKWIDTH-1 || blockIDs[blocksDrawn + CHUNKWIDTH] == 0)
						m_display->AppendToDrawBuffer(&(*m_blocks)[blockIDs[blocksDrawn]]->vertices[24], 6, &(glm::vec3(x, y, z) + chunkRoot), Display::SOLID);

					//m_visiblilityArray[blocksDrawn] |= 1 << 0;
				}
				blocksDrawn++;
			}
		}
	}
}
*/
