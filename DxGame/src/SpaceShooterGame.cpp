#include "pch.h"
#include "SpaceShooterGame.h"
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
};

SpaceShooterGame::SpaceShooterGame()
	: m_is_lbutton_pressed(false)
	, m_is_rbutton_pressed(false)
	, m_forward(0.0f)
	, m_rightward(0.0f) 
	, m_is_fullscreen_state(false)
	, m_is_inside_game_state(false)
	, m_prev_inside_game_state(false)
	, m_camera_distance(14.0f)
	, m_ship_speed(12.0f)
{
	Logger::PrintLog("SpaceShooterGame::SpaceShooterGame");
}

SpaceShooterGame::~SpaceShooterGame()
{
	Logger::PrintLog("SpaceShooterGame::~SpaceShooterGame");
}

void SpaceShooterGame::OnCreate()
{
	Logger::PrintLog("SpaceShooterGame::OnCreate");

	RECT rc = GetClientWindowRect();
	mp_swap_chain = GraphicsEngine::Instance()->GetRenderSystem()->CreateSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_camera.SetTranslation(Vector3d(0, 0, -1.0f));


	mp_sky_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\sphere.obj");
	TexturePtr p_sky_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\stars_map.jpg");

	mp_ship_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\spaceship.obj");
	TexturePtr p_ship_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\spaceship.jpg");

	mp_asteroid_mesh = GraphicsEngine::Instance()->GetMeshManager()->CreateMeshFromFile(L"assets\\meshes\\asteroid.obj");
	TexturePtr p_asteroid_texture = GraphicsEngine::Instance()->GetTextureManager()->CreateTextureFromFile(L"assets\\textures\\asteroid.jpg");

	mp_base_mat = GraphicsEngine::Instance()->CreateMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	mp_base_mat->SetCullMode(CULL_MODE_BACK);

	mp_ship_mat = GraphicsEngine::Instance()->CreateMaterial(mp_base_mat);
	mp_ship_mat->AddTexture(p_ship_texture);

	mp_asteroid_mat = GraphicsEngine::Instance()->CreateMaterial(mp_base_mat);
	mp_asteroid_mat->AddTexture(p_asteroid_texture);

	mp_sky_mat = GraphicsEngine::Instance()->CreateMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	mp_sky_mat->AddTexture(p_sky_texture);
	mp_sky_mat->SetCullMode(CULL_MODE_FRONT);



	m_world_camera.SetTranslation(Vector3d(0, 3, -3));
	m_mat_list.reserve(32);
}

void SpaceShooterGame::OnDestroy()
{
	Window::OnDestroy();
	mp_swap_chain->SetFullScreen(false, 1, 1);
	Logger::PrintLog("SpaceShooterGame::OnDestroy");
}

void SpaceShooterGame::OnUpdate()
{
	InputSystem::Instance()->Update();
	UpdateCursor();
	Update();
	Render();
}

void SpaceShooterGame::Render()
{
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->ClearRenderTargetColor(mp_swap_chain, 0.28f, 0.28f, 0.28f, 1);
	RECT rc = GetClientWindowRect();
	GraphicsEngine::Instance()->GetRenderSystem()->GetDeviceContext()->SetViewport(rc.right - rc.left, rc.bottom - rc.top);


	m_mat_list.clear();
	m_mat_list.push_back(mp_ship_mat);
	UpdateModel(m_ship_pos, m_ship_rot, Vector3d(1, 1, 1), m_mat_list);
	DrawMesh(mp_ship_mesh, m_mat_list);


	m_mat_list.clear();
	m_mat_list.push_back(mp_asteroid_mat);
	UpdateModel(Vector3d(0, 0, 20), Vector3d(), Vector3d(1,1,1), m_mat_list);
	DrawMesh(mp_asteroid_mesh, m_mat_list);


	m_mat_list.clear();
	m_mat_list.push_back(mp_sky_mat);
	DrawMesh(mp_sky_mesh, m_mat_list);

	mp_swap_chain->Present(true);
}

void SpaceShooterGame::Update()
{
	unsigned int time = ::GetTickCount();
	m_delta_time = (time - m_prev_time) / 1000.0f;
	m_prev_time = time;

	UpdateLight();
	UpdateSkyBox();
	UpdateShip();
	Update3dPersoneCamera();
}

void SpaceShooterGame::UpdateLight()
{
	m_light_rot_matrix.SetIdentity();
	Matrix4x4 temp;
	temp.SetRotationX(-0.707f);
	m_light_rot_matrix *= temp;
	temp.SetIdentity();
	temp.SetRotationY(0.707f);
	m_light_rot_matrix *= temp;
}

void SpaceShooterGame::UpdateModel(Vector3d pos, Vector3d rot, Vector3d scale, const std::vector<MaterialPtr>& materials)
{
	constant cc;

	m_time += m_delta_time;

	cc.m_world.SetIdentity();
	cc.m_world.SetScale(scale);

	Matrix4x4 temp;
	temp.SetRotationX(rot.m_x);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(rot.m_y);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.SetRotationZ(rot.m_z);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.SetTranslation(pos);
	cc.m_world *= temp;

	cc.m_view = m_view_camera;
	cc.m_proj = m_proj_camera;
	cc.m_camera_position = Vector4d(m_world_camera.GetTranslation());
	cc.m_light_direction = Vector4d(m_light_rot_matrix.GetZDirection());

	for (auto p_mat : materials)
	{
		p_mat->SetData(&cc, sizeof(cc));
	}
}

void SpaceShooterGame::UpdateSkyBox()
{
	constant cc;
	cc.m_world.SetIdentity();
	cc.m_world.SetScale(Vector3d(100.0f, 100.0f, 100.0f));
	cc.m_world.SetTranslation(m_world_camera.GetTranslation());
	cc.m_view = m_view_camera;
	cc.m_proj = m_proj_camera;
	mp_sky_mat->SetData(&cc, sizeof(cc));
}

void SpaceShooterGame::UpdateCamera()
{
	Matrix4x4 camera_matrix, temp;

	m_camera_rot.m_x += m_mouse_delta_y * m_delta_time * 0.1f;
	m_camera_rot.m_y += m_mouse_delta_x * m_delta_time * 0.1f;

	camera_matrix.SetRotationX(m_camera_rot.m_x);

	temp.SetRotationY(m_camera_rot.m_y);
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

void SpaceShooterGame::Update3dPersoneCamera()
{
	Matrix4x4 camera_matrix, temp;

	m_camera_rot.m_x += m_mouse_delta_y * m_delta_time * 0.1f;
	m_camera_rot.m_y += m_mouse_delta_x * m_delta_time * 0.1f;

	if (m_camera_rot.m_x > 1.57f)
		m_camera_rot.m_x = 1.57f;
	if (m_camera_rot.m_x < -1.57f)
		m_camera_rot.m_x = -1.57f;

	camera_matrix.SetRotationX(m_camera_rot.m_x);

	temp.SetRotationY(m_camera_rot.m_y);
	camera_matrix *= temp;


	m_camera_pos = m_ship_pos;

	Vector3d new_camera_pos = m_camera_pos + camera_matrix.GetZDirection() * (-m_camera_distance);
	new_camera_pos = new_camera_pos + (camera_matrix.GetYDirection() * 5.0f);
	
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

void SpaceShooterGame::UpdateShip()
{
	Matrix4x4 camera_matrix, temp;

	m_ship_rot.m_x += m_mouse_delta_y * m_delta_time * 0.1f;
	m_ship_rot.m_y += m_mouse_delta_x * m_delta_time * 0.1f;

	if (m_ship_rot.m_x > 1.57f)
		m_ship_rot.m_x = 1.57f;
	if (m_ship_rot.m_x < -1.57f)
		m_ship_rot.m_x = -1.57f;

	camera_matrix.SetRotationX(m_ship_rot.m_x);

	temp.SetRotationY(m_ship_rot.m_y);
	camera_matrix *= temp;

	m_ship_pos = m_ship_pos + camera_matrix.GetZDirection() * m_forward * m_ship_speed * m_delta_time;

}

void SpaceShooterGame::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
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

void SpaceShooterGame::OnKeyDown(byte key)
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

void SpaceShooterGame::OnKeyUp(byte key)
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
	case VK_ESCAPE:
		m_is_inside_game_state = false;
		break;
	};
}

void SpaceShooterGame::UpdateCursor()
{
	if (m_prev_inside_game_state != m_is_inside_game_state)
	{
		m_prev_inside_game_state = m_is_inside_game_state;
		InputSystem::Instance()->ShowCursor(!m_is_inside_game_state);
	}
}

void SpaceShooterGame::OnMouseMove(const Point2d& cur_pos)
{
	if (m_is_inside_game_state)
	{
		RECT r = GetClientWindowRect();
		Point2d mid_pos((r.right + r.left) / 2, (r.bottom + r.top) / 2);
		m_mouse_delta_x = cur_pos.m_x - mid_pos.m_x;
		m_mouse_delta_y = cur_pos.m_y - mid_pos.m_y;
		InputSystem::Instance()->SetPosCursor(mid_pos);
	}
}

void SpaceShooterGame::OnMouseLKeyDown(const Point2d& pos)
{
	m_is_lbutton_pressed = true;
	m_is_inside_game_state = true;
}

void SpaceShooterGame::OnMouseLKeyUp(const Point2d& pos)
{
	m_is_lbutton_pressed = false;
}

void SpaceShooterGame::OnMouseRKeyDown(const Point2d& pos)
{
	m_is_rbutton_pressed = true;
}

void SpaceShooterGame::OnMouseRKeyUp(const Point2d& pos)
{
	m_is_rbutton_pressed = false;
}

void SpaceShooterGame::OnSetFocus()
{
	InputSystem::Instance()->AddListener(this);
}

void SpaceShooterGame::OnKillFocus()
{
	InputSystem::Instance()->RemoveListener(this);
}

void SpaceShooterGame::OnResize()
{
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	mp_swap_chain->Resize(rect.right-rect.left, rect.bottom-rect.top);
	Render();
}
