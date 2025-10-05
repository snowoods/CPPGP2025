#pragma once

// 윈도우 / D3D 헤더 선언

// Windows includes
#include <windows.h>
#include <tchar.h>

// Standard ANSI-C includes
#include <time.h>
#include <iostream>

// DirectX includes
#include "d3d11.h"

//------------------------------------------------------------------------------

#define randomize() srand((unsigned)time(nullptr))
#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=nullptr; }}
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=nullptr; }}
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=nullptr; }}

inline int RectWidth(RECT& rc) { return ((rc).right - (rc).left); }
inline int RectHeight(RECT& rc) { return ((rc).bottom - (rc).top); }

//------------------------------------------------------------------------------

#include "ZGraphicsGlobal.h"
#include "ZFrameworkGlobal.h"
