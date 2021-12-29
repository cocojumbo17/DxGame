#pragma once
#include "Window.h"
#include "IInputListener.h"

class SwapChain;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;


class AppWindow :
    public Window,
    public IInputListener
{
public:
    // Inherited via Window
    virtual void OnCreate() override;
    virtual void OnDestroy() override;
    virtual void OnUpdate() override;
protected:
    void UpdateQuadPosition();

    // Inherited via IInputListener
    virtual void OnKeyDown(byte key) override;
    virtual void OnKeyUp(byte key) override;
protected:
    SwapChain* mp_swap_chain;
    VertexBuffer* mp_vb;
    IndexBuffer* mp_ib;
    ConstantBuffer* mp_cb;
    VertexShader* mp_vs;
    PixelShader* mp_ps;
    unsigned int m_prev_time;
    float m_delta_pos;
    float m_delta_scale;
    float m_delta_time;
    float m_rot_x;
    float m_rot_y;
};

