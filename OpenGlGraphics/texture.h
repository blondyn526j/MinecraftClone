#pragma once

#include "GL/glew.h"
#include <string>

class Texture
{
public:
	Texture(const std::string &fileName);

	void Bind(unsigned int unit);

	virtual ~Texture();

private:

	GLuint m_texture;
};

