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
Shader postShader("./res/postShader");

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

	while (!isClosed)
	{
		//INPUT TEST
		input.Update();

		camera.Update(&input);

		shader.Bind();

		shader.Update(transform, camera);

		chunkManager.camPosition = camera.position;
		chunkManager.Draw(camera.position.x, camera.position.z);
		
		display.BindFrameBuffer(display.m_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display.DrawBuffer(Display::SOLID);

		waveShader.Bind();
		waveShader.Update(transform, camera);
		
		display.DrawBuffer(Display::LIQUID);
		
		display.BindFrameBuffer(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		postShader.Bind();

		glBegin(GL_QUADS);
		glVertex2f(-1, -1);
		glVertex2f(1, -1);
		glVertex2f(1, 1);
		glVertex2f(-1, 1);
		glEnd();
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