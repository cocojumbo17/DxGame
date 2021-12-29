#include "pch.h"
#include "AppWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//Logger log;
	AppWindow main_wnd;
	main_wnd.Init();
	while (main_wnd.IsRun())
	{
		main_wnd.Broadcast();
	}
	main_wnd.Release();
	return 0;
}