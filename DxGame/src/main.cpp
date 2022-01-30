#include "pch.h"
#include "SpaceShooterGame.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Logger log;
	try
	{
		GraphicsEngine::Create();
		InputSystem::Create();
	}
	catch (const std::exception&)
	{
		return -1;
	}

	{
		try
		{
			SpaceShooterGame main_wnd;
			while (main_wnd.IsRun());
		}
		catch (const std::exception&)
		{
			InputSystem::Release();
			GraphicsEngine::Release();
			return -1;
		}
	}

	InputSystem::Release();
	GraphicsEngine::Release();

	return 0;
}