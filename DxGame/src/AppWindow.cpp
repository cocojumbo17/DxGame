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
#include "Material.h"

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
	float m_light_radius = 500.0f;
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
	, m_light_radius(500.0f)
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


	mp_terrain_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\terrain.obj");
	mp_scene_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\house.obj");
	mp_sky_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\sphere.obj");

	RECT rc = GetClientWindowRect();
	mp_swap_chain = GraphicsEngine::Instance()->GetRenderSystem()->CreateSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	m_world_camera.SetTranslation(Vector3d(0, 0, -1.0f));


	TexturePtr p_sand_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\sand.jpg");
	TexturePtr p_sky_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\sky.jpg");
	TexturePtr p_brick_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\house_brick.jpg");
	TexturePtr p_window_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\house_windows.jpg");
	TexturePtr p_wood_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\house_wood.jpg");
	TexturePtr p_barrel_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\barrel.jpg");



	mp_terrain_mat = GraphicsEngine::Instance()->CreateMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	mp_terrain_mat->AddTexture(p_sand_texture);
	mp_terrain_mat->SetCullMode(CULL_MODE_BACK);

	mp_brick_mat = GraphicsEngine::Instance()->CreateMaterial(mp_terrain_mat);
	mp_brick_mat->AddTexture(p_brick_texture);

	mp_window_mat = GraphicsEngine::Instance()->CreateMaterial(mp_terrain_mat);
	mp_window_mat->AddTexture(p_window_texture);

	mp_wood_mat = GraphicsEngine::Instance()->CreateMaterial(mp_terrain_mat);
	mp_wood_mat->AddTexture(p_wood_texture);

	mp_barrel_mat = GraphicsEngine::Instance()->CreateMaterial(mp_terrain_mat);
	mp_barrel_mat->AddTexture(p_barrel_texture);

	mp_sky_mat = GraphicsEngine::Instance()->CreateMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	mp_sky_mat->AddTexture(p_sky_texture);
	mp_sky_mat->SetCullMode(CULL_MODE_FRONT);



	m_world_camera.SetTranslation(Vector3d(0, 3, -3));
	m_mat_list.reserve(32);
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

	m_mat_list.clear();
	m_mat_list.push_back(mp_terrain_mat);
	UpdateModel(Vector3d(0, 0, 0), m_mat_list);
	DrawMesh(mp_terrain_mesh, m_mat_list);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_mat_list.clear();
			m_mat_list.push_back(mp_barrel_mat);
			m_mat_list.push_back(mp_brick_mat);
			m_mat_list.push_back(mp_window_mat);
			m_mat_list.push_back(mp_wood_mat);
			UpdateModel(Vector3d(-10.0f + 10.0f * i, 0.0f, -10.0f + 10.0f * j), m_mat_list);
			DrawMesh(mp_scene_mesh, m_mat_list);
		}
	}

	//UpdateModel(Vector3d(0, -2.0f, 0), mp_stone_mat);
	//DrawMesh(mp_plane_mesh, mp_stone_mat);

	m_mat_list.clear();
	m_mat_list.push_back(mp_sky_mat);
//	UpdateModel(Vector3d(0, 0, 0), m_mat_list);
	DrawMesh(mp_sky_mesh, m_mat_list);

	mp_swap_chain->Present(true);
}

void AppWindow::Update()
{
	unsigned int time = ::GetTickCount();
	m_delta_time = (time - m_prev_time) / 1000.0f;
	m_prev_time = time;

	UpdateCamera();
	UpdateLight();
	UpdateSkyBox();
}

void AppWindow::UpdateLight()
{
	m_light_rot_y += 0.707f * m_delta_time;
	float distance_to_origin = 2.0f;
//	m_light_position = Vector4d(cos(2 * m_light_rot_y) * distance_to_origin, 2.0f, sin(2 * m_light_rot_y) * distance_to_origin, 1.0f);
	m_light_position = Vector4d(180, 140, 70, 1.0f);
}

void AppWindow::UpdateModel(Vector3d pos, const std::vector<MaterialPtr>& materials)
{
	constant cc;

	Matrix4x4 light_rot_matrix;
	light_rot_matrix.SetRotationY(m_light_rot_y);
	m_time += m_delta_time;

	cc.m_world.SetIdentity();
	cc.m_world.SetTranslation(pos);
	cc.m_view = m_view_camera;
	cc.m_proj = m_proj_camera;
	cc.m_camera_position = m_world_camera.GetTranslation();
	cc.m_light_direction = light_rot_matrix.GetZDirection();
	cc.m_time = m_time;
	cc.m_light_position = m_light_position;
	cc.m_light_radius = m_light_radius;

	for (auto p_mat : materials)
	{
		p_mat->SetData(&cc, sizeof(cc));
	}
}

void AppWindow::UpdateSkyBox()
{
	constant cc;
	cc.m_world.SetIdentity();
	cc.m_world.SetScale(Vector3d(100.0f, 100.0f, 100.0f));
	cc.m_world.SetTranslation(m_world_camera.GetTranslation());
	cc.m_view = m_view_camera;
	cc.m_proj = m_proj_camera;
	mp_sky_mat->SetData(&cc, sizeof(cc));
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

void AppWindow::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
{
	auto p_ctx = GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext();
	p_ctx->SetVertexBuffer(mesh->GetVertexBuffer());
	p_ctx->SetIndexBuffer(mesh->GetIndexBuffer());

	for (size_t m = 0; m < mesh->GetNumMaterialSlots(); ++m)
	{
		if (m >= materials.size())
			return;
		auto p_mat = materials[m];
		auto mat_slot = mesh->GetMaterialSlot(m);
		GraphicsEngine::Instance()->SetMaterial(p_mat);
		GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList((UINT)mat_slot.m_num_indices, 0, (int)mat_slot.m_start_index);
	}
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
