#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"
#include "Vector4d.h"

class AppWindow :
    public Window,
    public IInputListener
{
public:
    AppWindow();
    virtual ~AppWindow();
protected:
    void Render();
    void Update();
    void UpdateCamera();
    void UpdateModel(Vector3d pos, const std::vector<MaterialPtr>& materials);
    void UpdateSkyBox();
    void UpdateLight();
    void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);

    // Inherited via Window
    virtual void OnCreate() override;
    virtual void OnDestroy() override;
    virtual void OnUpdate() override;
    virtual void OnSetFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnResize() override;

    // Inherited via IInputListener
    virtual void OnKeyDown(byte key) override;
    virtual void OnKeyUp(byte key) override;
    virtual void OnMouseMove(const Point2d& offset) override;
    virtual void OnMouseLKeyDown(const Point2d& pos) override;
    virtual void OnMouseLKeyUp(const Point2d& pos) override;
    virtual void OnMouseRKeyDown(const Point2d& pos) override;
    virtual void OnMouseRKeyUp(const Point2d& pos) override;

protected:
    SwapChainPtr mp_swap_chain;
    
    MaterialPtr mp_terrain_mat;
    MaterialPtr mp_brick_mat;
    MaterialPtr mp_window_mat;
    MaterialPtr mp_wood_mat;
    MaterialPtr mp_barrel_mat;
    MaterialPtr mp_sky_mat;

    std::vector<MaterialPtr> m_mat_list;

    MeshPtr mp_terrain_mesh;
    MeshPtr mp_scene_mesh;
    MeshPtr mp_sky_mesh;

    unsigned int m_prev_time;
    float m_delta_time;
    float m_time;
    float m_rot_x;
    float m_rot_y;
    float m_light_rot_y;
    bool m_is_lbutton_pressed;
    bool m_is_rbutton_pressed;
    float m_scale;
    float m_forward;
    float m_rightward;
    float m_light_radius;
    
    Matrix4x4 m_world_camera;
    Matrix4x4 m_view_camera;
    Matrix4x4 m_proj_camera;

    bool m_is_fullscreen_state;
    Vector4d m_light_position;
};

