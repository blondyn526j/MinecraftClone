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

#define WIDTH 1280
#define HEIGHT 720

bool isClosed = false;
Display display(WIDTH, HEIGHT, "Hello Screen");
Camera camera(glm::vec3(8.0f, 100.0, 8.0f), 80.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
Blocks blocks(4);
Input input(&isClosed);

Shader shader("./res/basicShader");
Shader waveShader("./res/waveShader");

Texture texture("./res/Texture.jpg");
Transform transform;

ChunkManager chunkManager(&blocks, &display);

int main(int argc, char** argv)
{
	std::cout << "Initializing Components" << std::endl;

	std::thread consoleReadThread(ReadConsoleCommand);

	float counter = 0.0f;


	chunkManager.LoadWorld();
	display.InitializeBuffer();



	while (!isClosed)
	{
		//INPUT TEST
		input.Update();

		camera.Update(input.vertical, input.horizontal, input.xMouse, input.yMouse);

		//INPUT TEST END

		//RAYCAST TEST

		/*double t = cube.RayFaceIntersectionTEST(camera.position, camera.forward, glm::vec3(0, 0, -1), cubeVertices);
		std::cout << t << std::endl;
		if(t == -1)
			display.Clear(0.6f, 0.15f, 0.2f, 1.0f);
		else
			display.Clear(0.15f, 0.6f, 0.15f, 1.0f);*/

		//RAYCAST TEST END

		shader.Bind();
		texture.Bind(0);

		shader.Update(transform, camera);

		chunkManager.Draw(camera.position.x, camera.position.z);
		display.DrawBuffer(Display::SOLID);
		
		waveShader.Bind();
		waveShader.Update(transform, camera);

		display.DrawBuffer(Display::LIQUID);
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

		if(command == "loadWorld")
		chunkManager.LoadWorld();
		//else if(command == "saveChunk")
		//chunkManager.SaveChunkToFile(0, 0);
		else
		{
			std::cout << "Command Not Recognised" << std::endl;
		}
	}

}
