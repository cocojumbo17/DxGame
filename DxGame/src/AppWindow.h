#pragma once
#include "Window.h"

class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

class AppWindow :
    public Window
{
public:
    // Inherited via Window
    virtual void OnCreate() override;
    virtual void OnDestroy() override;
    virtual void OnUpdate() override;
protected:
    void UpdateQuadPosition();
protected:
    SwapChain* mp_swap_chain;
    VertexBuffer* mp_vb;
    ConstantBuffer* mp_cb;
    VertexShader* mp_vs;
    PixelShader* mp_ps;
    unsigned int m_prev_time;
    float m_delta_pos;
    float m_delta_scale;
};

