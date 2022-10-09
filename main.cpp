#include "SnowGame.h"

int main()
{
	try
	{
	GraphicsEngine::create();
	HUD::create();
	InputSystem::create();
	AudioEngine::create();
	
    }
	catch (...) { return -1; }

	try
	{
		SnowGame Game;
		while (Game.IsRun());

	}
	catch (...)
	{
	GraphicsEngine::release();
	HUD::release();
	InputSystem::release();
	AudioEngine::release();
	return -1;
	}

	GraphicsEngine::release();
	HUD::release();
	InputSystem::release();
	AudioEngine::release();

	return 0;
	
}
