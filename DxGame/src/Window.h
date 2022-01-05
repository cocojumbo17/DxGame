#pragma once


class Window
{
public:
	Window();
	virtual ~Window();
	bool IsRun();
	RECT GetClientWindowRect();
	//Events
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnSetFocus() = 0;
	virtual void OnKillFocus() = 0;
	virtual void OnResize() = 0;

protected:
	bool RegisterWin();
	bool CreateWin();
	bool Broadcast();

protected:
	bool m_is_init;
	bool m_is_run;
	HWND m_hwnd;
};

