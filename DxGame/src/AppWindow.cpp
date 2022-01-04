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
#include "RenderSystem.h"

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

AppWindow::AppWindow()
	: m_is_lbutton_pressed(false)
	, m_is_rbutton_pressed(false)
	, m_scale(1.0f)
	, m_forward(0.0f)
	, m_rightward(0.0f)
{
	Logger::PrintLog("AppWindow::AppWindow");
}

AppWindow::~AppWindow()
{
	Logger::PrintLog("AppWindow::~AppWindow");
}

void AppWindow::OnCreate()
{
	Logger::PrintLog("AppWindow::OnCreate");
	RECT rc = GetClientWindowRect();
	mp_swap_chain = GraphicsEngine::Instance()->GetRenderSystem()->CreateSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex v[] = {
		{Vector3d(-0.5f, -0.5f, 0.5f),	Vector3d(0,0,1),	Vector3d(0,0,1)},
		{Vector3d(-0.5f, 0.5f, 0.5f),	Vector3d(1,0,0),	Vector3d(1,0,0)},
		{Vector3d(0.5f, -0.5f, 0.5f),	Vector3d(0,0,0),	Vector3d(0,0,0)},
		{Vector3d(0.5f, 0.5f, 0.5f),	Vector3d(1,1,0),	Vector3d(1,1,0)},
		{Vector3d(-0.5f, -0.5f, -0.5f),	Vector3d(0,0,1),	Vector3d(0,0,1)},
		{Vector3d(-0.5f, 0.5f, -0.5f),	Vector3d(1,0,0),	Vector3d(1,0,0)},
		{Vector3d(0.5f, -0.5f, -0.5f),	Vector3d(0,0,0),	Vector3d(0,0,0)},
		{Vector3d(0.5f, 0.5f, -0.5f),	Vector3d(1,1,0),	Vector3d(1,1,0)}

	};
	UINT vertex_size = sizeof(vertex);
	UINT list_size = ARRAYSIZE(v);



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
	mp_ib = GraphicsEngine::Instance()->GetRenderSystem()->CreateIndexBuffer(indixes, ARRAYSIZE(indixes));
	
	constant cc;
	mp_cb = GraphicsEngine::Instance()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(cc));

	void* p_shader_bytecode = nullptr;
	size_t shader_size = 0;
	if (GraphicsEngine::Instance()->GetRenderSystem()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &p_shader_bytecode, shader_size))
	{
		mp_vs = GraphicsEngine::Instance()->GetRenderSystem()->CreateVertexShader(p_shader_bytecode, shader_size);
	}
	mp_vb = GraphicsEngine::Instance()->GetRenderSystem()->CreateVertexBuffer(v, vertex_size, list_size, p_shader_bytecode, shader_size);
	GraphicsEngine::Instance()->GetRenderSystem()->ReleaseCompiledShader();

	if (GraphicsEngine::Instance()->GetRenderSystem()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &p_shader_bytecode, shader_size))
	{
		mp_ps = GraphicsEngine::Instance()->GetRenderSystem()->CreatePixelShader(p_shader_bytecode, shader_size);
	}
	GraphicsEngine::Instance()->GetRenderSystem()->ReleaseCompiledShader();
	m_camera_matrix.SetTranslation(Vector3d(0,0,-2.0f));
	InputSystem::Instance()->ShowCursor(false);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	Logger::PrintLog("AppWindow::OnDestroy");
}

void AppWindow::OnUpdate()
{
	InputSystem::Instance()->Update();
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->ClearRenderTargetColor(mp_swap_chain, 0.28f, 0.28f, 0.28f, 1);
	RECT rc = GetClientWindowRect();
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetViewport(rc.right - rc.left, rc.bottom - rc.top);

	Update();

	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetConstantBufferVS(mp_cb);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetConstantBufferPS(mp_cb);


	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetVertexShader(mp_vs);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetPixelShader(mp_ps);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(mp_vb);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(mp_ib);


	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList((UINT)mp_ib->GetListSize(), 0, 0);

	mp_swap_chain->Present(true);
}

void AppWindow::Update()
{
	RECT rc = GetClientWindowRect();
	constant cc;
	cc.m_time = ::GetTickCount();
	m_delta_time = (cc.m_time - m_prev_time) / 1000.0f;
	m_prev_time = cc.m_time;
	


	Matrix4x4 camera_matrix, temp;
	camera_matrix.SetRotationX(m_rot_x);

	temp.SetRotationY(m_rot_y);
	camera_matrix *= temp;

	Vector3d new_camera_pos = m_camera_matrix.GetTranslation() + camera_matrix.GetZDirection() * m_forward*0.1f;
	new_camera_pos = new_camera_pos + camera_matrix.GetXDirection() * m_rightward * 0.1f;

	camera_matrix.SetTranslation(new_camera_pos);

	m_camera_matrix = camera_matrix;
	
	camera_matrix.Inverse();
	cc.m_view = camera_matrix;

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
//	cc.m_proj.SetOrthoLH(width/300.0f, height/300.0f, -4.0f, 4.0f);
	cc.m_proj.SetPerspectiveFovLH(1.57f, (float)width / height, 0.1f, 100.0f);
	mp_cb->Update(GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext(), &cc);
}

void AppWindow::OnKeyDown(byte key)
{
	switch (std::toupper(key)) {
		case 'A':
			m_rightward = -1.0f;
			break;
		case 'D':
			m_rightward = 1.0f;
			break;
		case 'S':
			m_forward = -1.0f;
			break;
		case 'W':
			m_forward = 1.0f;
			break;
	}
}

void AppWindow::OnKeyUp(byte key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::OnMouseMove(const Point2d& offset)
{
	if (m_is_lbutton_pressed)
	{
		m_rot_x -= offset.m_y * m_delta_time * 0.1f;
		m_rot_y -= offset.m_x * m_delta_time * 0.1f;
	}
	if (m_is_rbutton_pressed)
	{
		m_scale -= offset.m_y * m_delta_time;
	}
}

void AppWindow::OnMouseLKeyDown(const Point2d& pos)
{
	m_is_lbutton_pressed = true;
}

void AppWindow::OnMouseLKeyUp(const Point2d& pos)
{
	m_is_lbutton_pressed = false;
}

void AppWindow::OnMouseRKeyDown(const Point2d& pos)
{
	m_is_rbutton_pressed = true;
}

void AppWindow::OnMouseRKeyUp(const Point2d& pos)
{
	m_is_rbutton_pressed = false;
}

void AppWindow::OnSetFocus()
{
	InputSystem::Instance()->AddListener(this);
}

void AppWindow::OnKillFocus()
{
	InputSystem::Instance()->RemoveListener(this);
}
