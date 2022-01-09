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
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include "Matrix4x4.h"
#include <math.h>
#include "InputSystem.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Mesh.h"
struct vertex
{
	Vector3d pos;
	Vector2d texcoord;
}; 

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4d m_light_direction;
	Vector4d m_camera_position;
	Vector4d m_light_position = Vector4d(0,2,0,0);
	float m_light_radius = 4.0f;
	float m_time;
};

AppWindow::AppWindow()
	: m_is_lbutton_pressed(false)
	, m_is_rbutton_pressed(false)
	, m_scale(1.0f)
	, m_forward(0.0f)
	, m_rightward(0.0f)
	, m_light_rot_y(0.0f)
	, m_is_fullscreen_state(false)
	, m_light_radius(4.0f)
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


	mp_wall_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\wall.jpg");
	mp_sky_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\stars_map.jpg");

	mp_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\scene.obj");
	mp_sky_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\sphere.obj");

	RECT rc = GetClientWindowRect();
	mp_swap_chain = GraphicsEngine::Instance()->GetRenderSystem()->CreateSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	m_world_camera.SetTranslation(Vector3d(0, 0, -1.0f));

	constant cc;
	mp_cb = GraphicsEngine::Instance()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(cc));
	mp_sky_cb = GraphicsEngine::Instance()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(cc));

	void* p_shader_bytecode = nullptr;
	size_t shader_size = 0;
	if (GraphicsEngine::Instance()->GetRenderSystem()->CompileVertexShader(L"PointLightVertexShader.hlsl", "vsmain", &p_shader_bytecode, shader_size))
	{
		mp_vs = GraphicsEngine::Instance()->GetRenderSystem()->CreateVertexShader(p_shader_bytecode, shader_size);
	}
	GraphicsEngine::Instance()->GetRenderSystem()->ReleaseCompiledShader();

	if (GraphicsEngine::Instance()->GetRenderSystem()->CompilePixelShader(L"PointLightPixelShader.hlsl", "psmain", &p_shader_bytecode, shader_size))
	{
		mp_ps = GraphicsEngine::Instance()->GetRenderSystem()->CreatePixelShader(p_shader_bytecode, shader_size);
	}
	GraphicsEngine::Instance()->GetRenderSystem()->ReleaseCompiledShader();
	
	if (GraphicsEngine::Instance()->GetRenderSystem()->CompilePixelShader(L"SkyBoxPixelShader.hlsl", "psmain", &p_shader_bytecode, shader_size))
	{
		mp_sky_ps = GraphicsEngine::Instance()->GetRenderSystem()->CreatePixelShader(p_shader_bytecode, shader_size);
	}
	GraphicsEngine::Instance()->GetRenderSystem()->ReleaseCompiledShader();
	m_world_camera.SetTranslation(Vector3d(0, 2, -2));
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	mp_swap_chain->SetFullScreen(false, 1, 1);
	Logger::PrintLog("AppWindow::OnDestroy");
}

void AppWindow::OnUpdate()
{
	InputSystem::Instance()->Update();
	Render();
}

void AppWindow::Render()
{
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->ClearRenderTargetColor(mp_swap_chain, 0.28f, 0.28f, 0.28f, 1);
	RECT rc = GetClientWindowRect();
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetViewport(rc.right - rc.left, rc.bottom - rc.top);

	Update();

	GraphicsEngine::Instance()->GetRenderSystem()->SetRasterizerState(false);
	TexturePtr textures[] =
	{
		mp_wall_texture
	};
	DrawMesh(mp_mesh, mp_vs, mp_ps, mp_cb, textures, ARRAYSIZE(textures));
	GraphicsEngine::Instance()->GetRenderSystem()->SetRasterizerState(true);
	textures[0] = mp_sky_texture;
	DrawMesh(mp_sky_mesh, mp_vs, mp_sky_ps, mp_sky_cb, textures, 1);

	mp_swap_chain->Present(true);
}

void AppWindow::Update()
{
	unsigned int time = ::GetTickCount();
	m_delta_time = (time - m_prev_time) / 1000.0f;
	m_prev_time = time;

	UpdateCamera();
	UpdateModel();
	UpdateSkyBox();
}

void AppWindow::UpdateModel()
{
	constant cc;

	Matrix4x4 light_rot_matrix;
	light_rot_matrix.SetRotationY(m_light_rot_y);
	m_light_rot_y += 0.307f * m_delta_time;
	m_time += m_delta_time;

	cc.m_world.SetIdentity();
	cc.m_view = m_view_camera;
	cc.m_proj = m_proj_camera;
	cc.m_camera_position = m_world_camera.GetTranslation();
	cc.m_light_direction = light_rot_matrix.GetZDirection();
	cc.m_time = m_time;
	float distance_to_origin = 2.0f;
	cc.m_light_position = Vector4d(cos(2*m_light_rot_y) * distance_to_origin, 2.0f, sin(2*m_light_rot_y) * distance_to_origin, 1.0f);
	cc.m_light_radius = m_light_radius;
	mp_cb->Update(GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext(), &cc);
}

void AppWindow::UpdateSkyBox()
{
	constant cc;
	cc.m_world.SetIdentity();
	cc.m_world.SetScale(Vector3d(100.0f, 100.0f, 100.0f));
	cc.m_world.SetTranslation(m_world_camera.GetTranslation());
	cc.m_view = m_view_camera;
	cc.m_proj = m_proj_camera;
	mp_sky_cb->Update(GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext(), &cc);
}

void AppWindow::UpdateCamera()
{
	Matrix4x4 camera_matrix, temp;

	camera_matrix.SetRotationX(m_rot_x);

	temp.SetRotationY(m_rot_y);
	camera_matrix *= temp;

	Vector3d new_camera_pos = m_world_camera.GetTranslation() + camera_matrix.GetZDirection() * m_forward * 0.1f;
	new_camera_pos = new_camera_pos + camera_matrix.GetXDirection() * m_rightward * 0.1f;
	camera_matrix.SetTranslation(new_camera_pos);

	m_world_camera = camera_matrix;

	camera_matrix.Inverse();
	m_view_camera = camera_matrix;

	RECT rc = GetClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	//	m_proj_camera.SetOrthoLH(width/300.0f, height/300.0f, -4.0f, 4.0f);
	m_proj_camera.SetPerspectiveFovLH(1.57f, (float)width / height, 0.1f, 100.0f);
}

void AppWindow::DrawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, 
	const TexturePtr* textures, size_t num_textures)
{
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetConstantBufferVS(cb);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetConstantBufferPS(cb);

	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetTexturePS(textures, num_textures);

	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetVertexShader(vs);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetPixelShader(ps);
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList((UINT)mesh->GetIndexBuffer()->GetListSize(), 0, 0);

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
		case 'O':
			m_light_radius -= m_delta_time;
			break;
		case 'P':
			m_light_radius += m_delta_time;
			break;
	}
}

void AppWindow::OnKeyUp(byte key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
	switch (std::toupper(key)) {
	case 'F':
		m_is_fullscreen_state = !m_is_fullscreen_state;
		int w, h;
		GetScreenSize(w, h);
		mp_swap_chain->SetFullScreen(m_is_fullscreen_state, w, h);
		break;
	};
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

void AppWindow::OnResize()
{
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	mp_swap_chain->Resize(rect.right-rect.left, rect.bottom-rect.top);
	Render();
}
