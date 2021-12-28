#include "pch.h"
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 pos;
	vec3 pos1;
	vec3 color;
	vec3 color1;
};

__declspec(align(16))
struct constant
{
	unsigned int time;
};


void AppWindow::OnCreate()
{
	GraphicsEngine::Instance()->Init();
	mp_swap_chain = GraphicsEngine::Instance()->CreateSwapChain();
	RECT rc = GetClientWindowRect();
	mp_swap_chain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex v[] = {
		{-0.5f, -0.5f, 0.0f,	-0.55f, -0.55f, 0.0f,	0,0,1,	1,1,0},
		{-0.5f, 0.5f, 0.0f,		-0.55f, 0.55f, 0.0f,	0,0,0,	1,1,1},
		{0.5f, -0.5f, 0.0f,		0.55f, -0.55f, 0.0f,	1,1,1,	0,0,0},
		{0.5f, 0.5f, 0.0f,		0.75f, 0.75f, 0.0f,		1,1,0,	0,0,1}
	};
	UINT vertex_size = sizeof(vertex);
	UINT list_size = ARRAYSIZE(v);

	mp_vb = GraphicsEngine::Instance()->CreateVertexBuffer();

	mp_cb = GraphicsEngine::Instance()->CreateConstantBuffer();
	constant cc{0};
	mp_cb->Load(&cc, sizeof(cc));

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
	mp_cb->Release();
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

	constant cc{ ::GetTickCount()};
	mp_cb->Update(GraphicsEngine::Instance()->GetDeviceContext(), &cc);
	GraphicsEngine::Instance()->GetDeviceContext()->SetConstantBufferVS(mp_cb);
	GraphicsEngine::Instance()->GetDeviceContext()->SetConstantBufferPS(mp_cb);


	GraphicsEngine::Instance()->GetDeviceContext()->SetPixelShader(mp_ps);
	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexBuffer(mp_vb);


	GraphicsEngine::Instance()->GetDeviceContext()->DrawTriangleStrip((UINT)mp_vb->GetListSize(), 0);

	mp_swap_chain->Present(false);
}
