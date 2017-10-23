#pragma once

#include <iostream>
#include <Windows.h>

class Input
{
public:
	float xMouse = 0, yMouse = 0;
	char horizontal = 0;
	char vertical = 0;
	bool* isClosed;

	void Update()
	{
		SDL_Event e;

		xMouse = 0;
		yMouse = 0;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				*isClosed = true;
			if (e.type == SDL_MOUSEMOTION)
			{
				xMouse = e.motion.xrel;
				yMouse = e.motion.yrel;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == SDL_SCANCODE_EQUALS)
				{
					::ShowWindow(::GetConsoleWindow(), !::IsWindowVisible(::GetConsoleWindow()));
				}
			}
		}

		const Uint8* keys = SDL_GetKeyboardState(NULL);
		vertical = (keys[SDL_SCANCODE_W] ? 1 : 0) + (keys[SDL_SCANCODE_S] ? -1 : 0);
		horizontal = (keys[SDL_SCANCODE_A] ? 1 : 0) + (keys[SDL_SCANCODE_D] ? -1 : 0);
	}

	Input(bool* closed)
	{
		isClosed = closed;
	}
};

