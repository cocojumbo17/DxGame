#pragma once


class Window
{
public:
	Window();
	virtual ~Window();
	bool Init();
	bool Release();
	bool Broadcast();
	bool IsRun();
	RECT GetClientWindowRect();
	void SetHWND(HWND hwnd);
	//Events
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate() = 0;

protected:
	bool RegisterWin();
	bool CreateWin();

protected:
	bool m_is_run;
	HWND m_hwnd;
};

