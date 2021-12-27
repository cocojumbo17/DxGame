#include "pch.h"
#include "Window.h"

Window* temp_win_obj;

Window::Window()
    :m_is_run(false)
{
}

Window::~Window()
{
}

bool Window::Init()
{
	temp_win_obj = this;
    if (!RegisterWin())
        return false;
    if (!CreateWin())
        return false;
    return true;
}

bool Window::Release()
{
	if (!::DestroyWindow(m_hwnd))
		return false;
    return true;
}

bool Window::Broadcast()
{
	MSG msg{ 0 };
	OnUpdate();
	while (::PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	Sleep(1);
	return true;
}

bool Window::IsRun()
{
    return m_is_run;
}

RECT Window::GetClientWindowRect()
{
	RECT rc;
	::GetClientRect(m_hwnd, &rc);
	return rc;
}

void Window::SetHWND(HWND hwnd)
{
	m_hwnd = hwnd;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) 
	{
	case WM_CREATE:
		if (temp_win_obj)
		{
			temp_win_obj->SetHWND(hwnd);
			temp_win_obj->OnCreate();
		}
		break;
	case WM_DESTROY:
		if (temp_win_obj)
			temp_win_obj->OnDestroy();
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}

bool Window::RegisterWin()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = 0;// CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"DxWindowClass";
	wcex.hInstance = nullptr;
	wcex.lpfnWndProc = WinProc;
	return RegisterClassEx(&wcex);
}

void Window::OnDestroy()
{
	m_is_run = false;
}

bool Window::CreateWin()
{
	m_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"DxWindowClass", L"DirectX window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, nullptr, nullptr);

	if (!m_hwnd)
		return false;
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
	m_is_run = true;
	return true;
}
