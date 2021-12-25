#pragma once
#include <windows.h>
#include <d3d11.h>

#define SAFE_RELEASE(X) if (X) X->Release(); X=nullptr;