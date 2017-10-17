#pragma once
#include <sdl2\SDL.h>
#include <string>
#include "input.h"

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Update();
	bool IsClosed();
	void Clear(float red, float green, float blue, float alpha);

	virtual ~Display();
protected:
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed = false;
};