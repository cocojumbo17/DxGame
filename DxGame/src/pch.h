#pragma once
#include <windows.h>
#include <d3d11.h>

#define SAFE_RELEASE(X) if (X) X->Release(); X=nullptr;
#define MAX_NAME_STRING 256

#include "Time.h"
#include "Logger.h"
