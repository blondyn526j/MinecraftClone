#pragma once

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Shader
{
public:
	Shader(const std::string fileName);

	void Bind();
	void Update(Transform &transform, const Camera &camera);

	virtual ~Shader();

private:
	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string &text, GLenum shaderType);

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	static const unsigned int NUM_SHADERS = 2;
	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
	GLuint m_timer;
	float timer;
};

