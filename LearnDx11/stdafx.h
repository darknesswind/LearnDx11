// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <memory>

#include <DirectXColors.h>

#include "Inc/SimpleMath.h"
#include "Inc/d3dx11effect.h"
#include "com_ptr.h"

// TODO: reference additional headers your program requires here
typedef __int64 int64;
typedef unsigned short ushort;

typedef DirectX::SimpleMath::Vector4 Vector4;
typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Vector2 Vector2;
typedef DirectX::SimpleMath::Matrix Matrix;
typedef DirectX::SimpleMath::Color DxColor;

HRESULT ReportErrorResult(HRESULT hr, LPCWSTR code);

#define LAssert assert
#if defined(_DEBUG)
#	ifndef CKHR
#		define CKHR(x) ReportErrorResult(x, L#x)
#	endif
#else
#	ifndef CKHR
#		define CKHR(x) (x)
#	endif
#endif
