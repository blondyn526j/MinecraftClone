#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

void ReadConsoleCommand();

int width = 800;
int height = 600;
int drawDistance = 4;
bool asyncLoading = true;

bool ReadConfigFile()
{
	std::fstream file;
	file.open("./config.txt");
	std::string tempS;
	int tempI;
	
	file >> tempS;
	file >> tempI;
	width = tempI;

	file >> tempS;
	file >> tempI;
	height = tempI;

	file >> tempS;
	file >> tempI;
	asyncLoading = (tempI == 1) ? true : false;

	file >> tempS;
	file >> tempI;
	drawDistance = tempI;

	file.close();
	return true;
}
bool b = ReadConfigFile();