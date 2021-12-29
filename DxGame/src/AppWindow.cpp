#include "pch.h"
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vector3d.h"
#include "Matrix4x4.h"
#include <math.h>
#include "InputSystem.h"

struct vertex
{
	Vector3d pos;
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
	InputSystem::Instance()->AddListener(this);
	GraphicsEngine::Instance()->Init();
	mp_swap_chain = GraphicsEngine::Instance()->CreateSwapChain();
	RECT rc = GetClientWindowRect();
	mp_swap_chain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex v[] = {
		{Vector3d(-0.5f, -0.5f, 0.5f),	Vector3d(0,0,1),	Vector3d(1,1,0)},
		{Vector3d(-0.5f, 0.5f, 0.5f),	Vector3d(1,0,0),	Vector3d(0,0,0)},
		{Vector3d(0.5f, -0.5f, 0.5f),	Vector3d(0,0,0),	Vector3d(1,0,0)},
		{Vector3d(0.5f, 0.5f, 0.5f),	Vector3d(1,1,0),	Vector3d(0,0,1)},
		{Vector3d(-0.5f, -0.5f, -0.5f),	Vector3d(0,0,1),	Vector3d(1,1,0)},
		{Vector3d(-0.5f, 0.5f, -0.5f),	Vector3d(1,0,0),	Vector3d(0,0,0)},
		{Vector3d(0.5f, -0.5f, -0.5f),	Vector3d(0,0,0),	Vector3d(1,0,0)},
		{Vector3d(0.5f, 0.5f, -0.5f),	Vector3d(1,1,0),	Vector3d(0,0,1)}

	};
	UINT vertex_size = sizeof(vertex);
	UINT list_size = ARRAYSIZE(v);

	mp_vb = GraphicsEngine::Instance()->CreateVertexBuffer();


	int indixes[] = {
		0,1,2,
		1,3,2,
		6,7,4,
		4,7,5,
		1,5,3,
		5,7,3,
		4,0,6,
		0,2,6,
		2,3,6,
		3,7,6,
		4,5,0,
		5,1,0
	};
	mp_ib = GraphicsEngine::Instance()->CreateIndexBuffer();
	mp_ib->Load(indixes, ARRAYSIZE(indixes));
	
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
	InputSystem::Instance()->RemoveListener(this);
	mp_cb->Release();
	mp_ib->Release();
	mp_vb->Release();
	mp_vs->Release();
	mp_ps->Release();
	mp_swap_chain->Release();
	GraphicsEngine::Instance()->Release();
	Window::OnDestroy();
	Logger::PrintLog(L"AppWindow::OnDestroy");
}

void AppWindow::OnUpdate()
{
	InputSystem::Instance()->Update();
	GraphicsEngine::Instance()->GetDeviceContext()->ClearRenderTargetColor(mp_swap_chain, 0.28f, 0.28f, 0.28f, 1);
	RECT rc = GetClientWindowRect();
	GraphicsEngine::Instance()->GetDeviceContext()->SetViewport(rc.right - rc.left, rc.bottom - rc.top);

	UpdateQuadPosition();

	GraphicsEngine::Instance()->GetDeviceContext()->SetConstantBufferVS(mp_cb);
	GraphicsEngine::Instance()->GetDeviceContext()->SetConstantBufferPS(mp_cb);


	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexShader(mp_vs);
	GraphicsEngine::Instance()->GetDeviceContext()->SetPixelShader(mp_ps);
	GraphicsEngine::Instance()->GetDeviceContext()->SetVertexBuffer(mp_vb);
	GraphicsEngine::Instance()->GetDeviceContext()->SetIndexBuffer(mp_ib);


	GraphicsEngine::Instance()->GetDeviceContext()->DrawIndexedTriangleList((UINT)mp_ib->GetListSize(), 0, 0);

	mp_swap_chain->Present(true);
}

void AppWindow::UpdateQuadPosition()
{
	RECT rc = GetClientWindowRect();
	constant cc;
	cc.m_time = ::GetTickCount();
	m_delta_time = (cc.m_time - m_prev_time) / 1000.0f;
	m_prev_time = cc.m_time;
	//m_delta_pos += m_delta_time / 5.0f;
	//if (m_delta_pos > 1.0f)
	//	m_delta_pos = 0;

	//m_delta_scale += m_delta_time * 1.0f;
	//if (m_delta_scale > 2*3.1415926f)
	//	m_delta_scale = 0.0f;
	//Matrix4x4 trans, scale;
	//trans.SetTranslation(Vector3d::lerp(Vector3d(-1, -1, 0), Vector3d(1, 1, 0), m_delta_pos));
	//scale.SetScale(Vector3d::lerp(Vector3d(0.5f, 0.5f, 1.0f), Vector3d(1.0f, 1.0f, 1.0f), (float)(sin(m_delta_scale)+1.0f)/2.0f));
	
	//cc.m_world = scale;
	//cc.m_world *= trans;
	
	Matrix4x4 rx, ry, rz;
	rx.SetRotationX(m_rot_x);
	ry.SetRotationY(m_rot_y);

	cc.m_world = rx;
	cc.m_world *= ry;

	cc.m_view.SetIdentity();
	cc.m_proj.SetOrthoLH((rc.right - rc.left)/300.0f, (rc.bottom - rc.top)/300.0f, -4.0f, 4.0f);
	mp_cb->Update(GraphicsEngine::Instance()->GetDeviceContext(), &cc);
}

void AppWindow::OnKeyDown(byte key)
{
	switch (std::toupper(key)) {
		case 'A':
			m_rot_y -= m_delta_time;
			break;
		case 'D':
			m_rot_y += m_delta_time;
			break;
		case 'S':
			m_rot_x += m_delta_time;
			break;
		case 'W':
			m_rot_x -= m_delta_time;
			break;
	}
}

void AppWindow::OnKeyUp(byte key)
{
}
