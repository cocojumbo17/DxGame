#include "pch.h"
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"

void AppWindow::OnCreate()
{
	GraphicsEngine::Instance()->Init();
	mp_swap_chain = GraphicsEngine::Instance()->CreateSwapChain();
	RECT rc = GetClientWindowRect();
	mp_swap_chain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::OnDestroy()
{
	mp_swap_chain->Release();
	GraphicsEngine::Instance()->Release();
	Window::OnDestroy();
}

void AppWindow::OnUpdate()
{
}
