#include "pch.h"
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 pos;
	vec3 color;
};


void AppWindow::OnCreate()
{
	GraphicsEngine::Instance()->Init();
	mp_swap_chain = GraphicsEngine::Instance()->CreateSwapChain();
	RECT rc = GetClientWindowRect();
	mp_swap_chain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex v[] = {
		{-0.5f, -0.5f, 0.0f,	1,0,0},
		{-0.5f, 0.5f, 0.0f,		0,1,0},
		{0.5f, -0.5f, 0.0f,		0,0,1},
		{0.5f, 0.5f, 0.0f,		1,0,1}
	};
	UINT vertex_size = sizeof(vertex);
	UINT list_size = ARRAYSIZE(v);

	mp_vb = GraphicsEngine::Instance()->CreateVertexBuffer();


	void* p_shader_bytecode = nullptr;
	size_t shader_size = 0;
	if (GraphicsEngine::Instance()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &p_shader_bytecode, shader_size))
	{
		mp_vs = GraphicsEngine::Instance()->CreateVertexShader(p_shader_bytecode, shader_size);
	}
	mp_vb->Load(v, vertex_size, list_size, p_shader_bytecode, shader_size);
	GraphicsEngine::Instance()->ReleaseCompiledShader();

	if (GraphicsEngine::Instance()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &p_shader_bytecode, shader_size))
	{
		mp_ps = GraphicsEngine::Instance()->CreatePixelShader(p_shader_bytecode, shader_size);
	}
	GraphicsEngine::Instance()->ReleaseCompiledShader();

}

void AppWindow::OnDestroy()
{
	mp_vs->Release();
	mp_ps->Release();
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
	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexShader(mp_vs);
	GraphicsEngine::Instance()->GetDeviceContext()->SetPixelShader(mp_ps);
	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexBuffer(mp_vb);
	GraphicsEngine::Instance()->GetDeviceContext()->DrawTriangleStrip(mp_vb->GetListSize(), 0);

	mp_swap_chain->Present(true);
}
