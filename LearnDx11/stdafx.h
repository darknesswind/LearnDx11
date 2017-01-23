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
#include "Inc/SimpleMath.h"
#include "Inc/d3dx11effect.h"
#include "com_ptr.h"

// TODO: reference additional headers your program requires here
typedef __int64 int64;
typedef unsigned short ushort;

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
