#include "main.h"
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "input.h"
#include "blocks.h"
#include "chunk.h"
#include "chunkManager.h"

#include <vector>
#include <iostream>
#include <thread>

#include <GL/glut.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sdl2\SDL.h>


bool isClosed = false;
Display display(width, height, "Hello Screen");
Blocks blocks;
Input input(&isClosed);

ChunkManager chunkManager(&blocks, &display);
Camera camera(glm::vec3(8.0f, 100.0, 8.0f), 80.0f, (float)width / (float)height, 0.01f, 1000.0f);

Shader shader("./res/basicShader");
Shader waveShader("./res/waveShader");
Shader ssaoShader("./res/ssaoShader");
Shader blurShader("./res/blurShader");

Texture texture("./res/Texture.png");
Texture normalTexture("./res/NormalMap.png");

Transform transform;


int main(int argc, char** argv)
{
	std::cout << "Initializing Components" << std::endl;

	std::thread consoleReadThread(ReadConsoleCommand);

	float counter = 0.0f;


	chunkManager.LoadWorld();
	display.InitializeBuffer();

	texture.Bind(0);
	normalTexture.Bind(1);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, display.m_texColor);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, display.m_texDepth);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, display.m_texSSAO);

	while (!isClosed)
	{
		input.Update();

		camera.Update(&input);

		shader.Bind();

		shader.Update(transform, camera);

		chunkManager.camPosition = camera.position;
		chunkManager.Draw(camera.position.x, camera.position.z);
		
		display.BindFrameBuffer(display.m_fboRender);
		//glViewport(0, 0, width/2, height/2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display.DrawBuffer(Display::SOLID);

		waveShader.Bind();
		waveShader.Update(transform, camera);
		
		display.DrawBuffer(Display::LIQUID);
		
		display.BindFrameBuffer(display.m_fboSSAO);
		//glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ssaoShader.Bind();

		glBindVertexArray(display.m_rectangleVAO);
		glDrawArrays(GL_QUADS, 0, 4);
		//glBindVertexArray(0);

		display.BindFrameBuffer(0);
		blurShader.Bind();
		glDrawArrays(GL_QUADS, 0, 4);


		display.Update();
		counter += 0.05f;
	}
	return 0;
}

void ReadConsoleCommand()
{
	std::string input;

	while (!isClosed)
	{
		std::cin >> input;
		std::string command = input.substr(0, input.find('/'));
		std::string sValue = input.substr(input.find('/') + 1);

		//std::cout << command << std::endl;
		//std::cout << sValue << std::endl;

		if (command == "loadWorld")
			chunkManager.LoadWorld();
		//else if(command == "saveChunk")
		//chunkManager.SaveChunkToFile(0, 0);
		else
		{
			std::cout << "Command Not Recognised" << std::endl;
		}
	}

}