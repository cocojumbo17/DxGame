#pragma once
#include "Window.h"

class SwapChain;
class VertexBuffer;
class VertexShader;

class AppWindow :
    public Window
{
public:
    // Inherited via Window
    virtual void OnCreate() override;
    virtual void OnDestroy() override;
    virtual void OnUpdate() override;
protected:
    SwapChain* mp_swap_chain;
    VertexBuffer* mp_vb;
    VertexShader* mp_vs;
};

