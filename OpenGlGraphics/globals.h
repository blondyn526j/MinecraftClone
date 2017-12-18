#pragma once
#include "chunkManager.h"

extern int width;
extern int height;
extern bool asyncLoading;
extern int drawDistance;
extern bool noclip;

extern ChunkManager chunkManager;
class Shader;
extern Shader shader;
extern Shader waveShader;