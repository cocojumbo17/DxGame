#include "pch.h"
#include "Window.h"
#include <exception>

Window::Window()
    : m_is_run(false)
	, m_is_init(false)
{
	if (!RegisterWin())
		throw(std::exception("RegisterWin is failed."));
	if (!CreateWin())
		throw(std::exception("CreateWin is failed."));
}

Window::~Window()
{
	::DestroyWindow(m_hwnd);
}


bool Window::Broadcast()
{
	if (!m_is_init)
	{
		m_is_init = true;
		::SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		OnSetFocus();
		OnCreate();

	}
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
	if (m_is_run)
		Broadcast();
    return m_is_run;
}

RECT Window::GetClientWindowRect()
{
	RECT rc;
	::GetClientRect(m_hwnd, &rc);
	return rc;
}

void Window::GetScreenSize(int& w, int& h)
{
	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) 
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_DESTROY:
	{
		Window* p_wnd = (Window*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (p_wnd)
			p_wnd->OnDestroy();
		::PostQuitMessage(0);
		break;
	}
	case WM_SETFOCUS:
	{
		Window* p_wnd = (Window*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (p_wnd)
			p_wnd->OnSetFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		Window* p_wnd = (Window*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (p_wnd)
			p_wnd->OnKillFocus();
		break;
	}
	case WM_SIZE:
	{
		Window* p_wnd = (Window*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (p_wnd)
			p_wnd->OnResize();
		break;
	}
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
	m_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"DxWindowClass", L"DirectX window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, nullptr, nullptr, nullptr, NULL);

	if (!m_hwnd)
		return false;
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
	m_is_run = true;
	return true;
}
