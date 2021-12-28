#include "pch.h"
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vector3d.h"
#include "Matrix4x4.h"
#include <math.h>


struct vertex
{
	Vector3d pos;
	Vector3d pos1;
	Vector3d color;
	Vector3d color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


void AppWindow::OnCreate()
{
	Logger::PrintLog(L"AppWindow::OnCreate");
	GraphicsEngine::Instance()->Init();
	mp_swap_chain = GraphicsEngine::Instance()->CreateSwapChain();
	RECT rc = GetClientWindowRect();
	mp_swap_chain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex v[] = {
		{Vector3d(-0.5f, -0.5f, 0.0f),	Vector3d(-0.55f, -0.55f, 0.0f),	Vector3d(0,0,1),	Vector3d(1,1,0)},
		{Vector3d(-0.5f, 0.5f, 0.0f),	Vector3d(-0.55f, 0.55f, 0.0f),	Vector3d(1,0,0),	Vector3d(0,0,0)},
		{Vector3d(0.5f, -0.5f, 0.0f),	Vector3d(0.55f, -0.55f, 0.0f),	Vector3d(0,0,0),	Vector3d(1,0,0)},
		{Vector3d(0.5f, 0.5f, 0.0f),	Vector3d(0.75f, 0.75f, 0.0f),	Vector3d(1,1,0),	Vector3d(0,0,1)}
	};
	UINT vertex_size = sizeof(vertex);
	UINT list_size = ARRAYSIZE(v);

	mp_vb = GraphicsEngine::Instance()->CreateVertexBuffer();

	mp_cb = GraphicsEngine::Instance()->CreateConstantBuffer();
	constant cc;
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

	m_delta_pos = 0;
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
	Logger::PrintLog(L"AppWindow::OnDestroy");
}

void AppWindow::OnUpdate()
{
	GraphicsEngine::Instance()->GetDeviceContext()->ClearRenderTargetColor(mp_swap_chain, 0.08f, 0.08f, 0.08f, 1);
	RECT rc = GetClientWindowRect();
	GraphicsEngine::Instance()->GetDeviceContext()->SetViewport(rc.right - rc.left, rc.bottom - rc.top);

	UpdateQuadPosition();

	GraphicsEngine::Instance()->GetDeviceContext()->SetConstantBufferVS(mp_cb);
	GraphicsEngine::Instance()->GetDeviceContext()->SetConstantBufferPS(mp_cb);


	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexShader(mp_vs);
	GraphicsEngine::Instance()->GetDeviceContext()->SetPixelShader(mp_ps);
	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexBuffer(mp_vb);


	GraphicsEngine::Instance()->GetDeviceContext()->DrawTriangleStrip((UINT)mp_vb->GetListSize(), 0);

	mp_swap_chain->Present(false);
}

void AppWindow::UpdateQuadPosition()
{
	RECT rc = GetClientWindowRect();
	constant cc;
	cc.m_time = ::GetTickCount();
	float delta_time = (cc.m_time - m_prev_time) / 1000.0f;
	m_prev_time = cc.m_time;
	m_delta_pos += delta_time / 5.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	m_delta_scale += delta_time * 10.0f;
	if (m_delta_scale > 2*3.1415926f)
		m_delta_scale = 0.0f;
	Matrix4x4 trans, scale;
	trans.SetTranslation(Vector3d::lerp(Vector3d(-1, -1, 0), Vector3d(1, 1, 0), m_delta_pos));
	scale.SetScale(Vector3d::lerp(Vector3d(0.5f, 0.5f, 1.0f), Vector3d(1.0f, 1.0f, 1.0f), (float)(sin(m_delta_scale)+1.0f)/2.0f));
	
	cc.m_world = scale;
	cc.m_world *= trans;
	
	cc.m_view.SetIdentity();
	cc.m_proj.SetOrthoLH((rc.right - rc.left)/400.0f, (rc.bottom - rc.top)/400.0f, -4.0f, 4.0f);
	mp_cb->Update(GraphicsEngine::Instance()->GetDeviceContext(), &cc);
}
