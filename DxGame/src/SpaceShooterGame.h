#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"
#include "Vector3d.h"
#include "Vector4d.h"

class SpaceShooterGame :
    public Window,
    public IInputListener
{
public:
    SpaceShooterGame();
    virtual ~SpaceShooterGame();
protected:
    void Render();
    void Update();
    void UpdateCamera();
    void Update3dPersoneCamera();
    void UpdateModel(Vector3d pos, Vector3d rot, Vector3d scale, const std::vector<MaterialPtr>& materials);
    void UpdateShip();
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
    virtual void OnMouseMove(const Point2d& cur_pos) override;
    virtual void OnMouseLKeyDown(const Point2d& pos) override;
    virtual void OnMouseLKeyUp(const Point2d& pos) override;
    virtual void OnMouseRKeyDown(const Point2d& pos) override;
    virtual void OnMouseRKeyUp(const Point2d& pos) override;
protected:
    void UpdateCursor();
protected:
    SwapChainPtr mp_swap_chain;
    
    MaterialPtr mp_base_mat;

    MaterialPtr mp_sky_mat;
    MeshPtr mp_sky_mesh;

    MaterialPtr mp_ship_mat;
    MeshPtr mp_ship_mesh;

    MaterialPtr mp_asteroid_mat;
    MeshPtr mp_asteroid_mesh;

    std::vector<MaterialPtr> m_mat_list;


    unsigned int m_prev_time;
    float m_delta_time;
    float m_time;
    int m_mouse_delta_x;
    int m_mouse_delta_y;
    bool m_is_lbutton_pressed;
    bool m_is_rbutton_pressed;
    float m_forward;
    float m_rightward;
    
    Vector3d m_camera_rot;
    Vector3d m_camera_pos;
    Vector3d m_ship_pos;
    Vector3d m_ship_rot;
    float m_ship_speed;
    float m_camera_distance;

    Matrix4x4 m_world_camera;
    Matrix4x4 m_view_camera;
    Matrix4x4 m_proj_camera;

    Matrix4x4 m_light_rot_matrix;


    bool m_is_fullscreen_state;
    bool m_is_inside_game_state;
    bool m_prev_inside_game_state;
};

