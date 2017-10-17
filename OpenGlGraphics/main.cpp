#include "main.h"
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "input.h"
#include "blocks.h"
#include <vector>

//#include <GL/GLU.h>
#include <GL/glut.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sdl2\SDL.h>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv)
{
	std::cout << "Initializing Components" << std::endl;

	bool isClosed = false;
	Input input;
	Display display(WIDTH, HEIGHT, "Hello Screen");
	Camera camera(glm::vec3(0.0f, 0, -3.0f), 80.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
	Blocks blocks(2);

	Shader shader("./res/basicShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;

	float counter = 0.0f;
	float xMouse = 0, yMouse = 0;

	while (!isClosed)
	{
		//INPUT TEST

		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				isClosed = true;
			if (e.type == SDL_MOUSEMOTION)
			{
				xMouse = e.motion.xrel;
				yMouse = e.motion.yrel;
			}
			if (e.type == SDL_KEYDOWN)
			{
				//std::cout << "KEY DOWN" << std::endl;
			}
		}

		const Uint8* keys = SDL_GetKeyboardState(NULL);
		short vertical = (keys[SDL_SCANCODE_W] ? 1 : 0) + (keys[SDL_SCANCODE_S] ? -1 : 0);
		short horizontal = (keys[SDL_SCANCODE_A] ? 1 : 0) + (keys[SDL_SCANCODE_D] ? -1 : 0);

		camera.position += camera.forward * 0.1f * (float)vertical + glm::cross(camera.forward, camera.up) * 0.1f * (float)horizontal;

		camera.RotateBy(xMouse/100.0f, yMouse/100.0f);

		//INPUT TEST END

		//RAYCAST TEST

		/*double t = cube.RayFaceIntersectionTEST(camera.position, camera.forward, glm::vec3(0, 0, -1), cubeVertices);
		std::cout << t << std::endl;
		if(t == -1)
			display.Clear(0.6f, 0.15f, 0.2f, 1.0f);
		else
			display.Clear(0.15f, 0.6f, 0.15f, 1.0f);*/

		//RAYCAST TEST END

		display.Clear(0.6f, 0.15f, 0.2f, 1.0f);
		shader.Bind();
		texture.Bind(0);

		xMouse = 0;
		yMouse = 0;
		

		shader.Update(transform, camera);
		transform.pos = glm::vec3(0, 0, 0);
		blocks[0]->Draw();

		shader.Update(transform, camera);
		transform.pos += glm::vec3(2, 0, 0);
		blocks[1]->Draw();

		display.Update();
		counter += 0.05f;
	}
	return 0;
}