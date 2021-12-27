#include "pch.h"
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 pos;
};


void AppWindow::OnCreate()
{
	GraphicsEngine::Instance()->Init();
	mp_swap_chain = GraphicsEngine::Instance()->CreateSwapChain();
	RECT rc = GetClientWindowRect();
	mp_swap_chain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex v[] = {
		{-0.5f, -0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f}
	};
	UINT vertex_size = sizeof(vertex);
	UINT list_size = ARRAYSIZE(v);

	mp_vb = GraphicsEngine::Instance()->CreateVertexBuffer();
	GraphicsEngine::Instance()->CreateShaders();
	void* p_shader_bytecode = nullptr;
	unsigned int shader_size = 0;
	GraphicsEngine::Instance()->GetShadersBytecodeAndSize(&p_shader_bytecode, &shader_size);
	mp_vb->Load(v, vertex_size, list_size, p_shader_bytecode, shader_size);

}

void AppWindow::OnDestroy()
{
	mp_vb->Release();
	mp_swap_chain->Release();
	GraphicsEngine::Instance()->Release();
	Window::OnDestroy();
}

void AppWindow::OnUpdate()
{
	GraphicsEngine::Instance()->GetDeviceContext()->ClearRenderTargetColor(mp_swap_chain, 0.0f, 0.3f, 0.4f, 1);
	RECT rc = GetClientWindowRect();
	GraphicsEngine::Instance()->GetDeviceContext()->SetViewport(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::Instance()->SetShaders();
	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexBuffer(mp_vb);
	GraphicsEngine::Instance()->GetDeviceContext()->DrawTriangleStrip(mp_vb->GetListSize(), 0);

	mp_swap_chain->Present(true);
}
