#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"

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
    void UpdateModel();
    void UpdateSkyBox();
    void DrawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& texture);

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
    VertexShaderPtr mp_vs;

    ConstantBufferPtr mp_cb;
    ConstantBufferPtr mp_sky_cb;

    PixelShaderPtr mp_ps;
    PixelShaderPtr mp_sky_ps;

    TexturePtr mp_wood_texture;
    TexturePtr mp_sky_texture;

    MeshPtr mp_mesh;
    MeshPtr mp_sky_mesh;

    unsigned int m_prev_time;
    float m_delta_time;
    float m_rot_x;
    float m_rot_y;
    float m_light_rot_y;
    bool m_is_lbutton_pressed;
    bool m_is_rbutton_pressed;
    float m_scale;
    float m_forward;
    float m_rightward;
    
    Matrix4x4 m_world_camera;
    Matrix4x4 m_view_camera;
    Matrix4x4 m_proj_camera;

    bool m_is_fullscreen_state;
};

