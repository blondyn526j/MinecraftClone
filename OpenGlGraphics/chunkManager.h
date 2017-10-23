#pragma once
#include "chunk.h"
#include "shader.h"
#include "transform.h"
#include "blocks.h"
#include "display.h"

#include <iostream>
#include <fstream>
#include <string>

class ChunkManager
{
public:
	ChunkManager(Shader* shader, Transform* transform, Blocks* blocks, Display* display);
	virtual ~ChunkManager();
	void LoadChunkFromFile(std::string fileName);
	void SaveChunkToFile(std::string fileName);
	void Draw();

private:
	Chunk* m_chunk;

	Shader* m_shader;
	Transform* m_transform;
	Blocks* m_blocks;
	Display* m_display;
};

