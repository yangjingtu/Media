// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#ifndef ASSERT
#include <assert.h>
#define ASSERT assert
#endif

// TODO: 在此处引用程序需要的其他头文件
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <iostream>
using namespace std;

#define INT32_TO_VARIANT(_val, _v)  \
	(_v).vt = VT_I4;                                         \
	(_v).lVal = _val

#define STRINGZ_TO_VARIANT(_val, _v)                    \
	(_v).vt =VT_BSTR;                                         \
	(_v).bstrVal = _com_util::ConvertStringToBSTR(_val)

#include "NPWnd.h"
#include "NPWndProc.h"
#include "Glue.h"
#include "../wellLog/Log.h"
#pragma comment(lib, "../../lib/wellLog.lib")

#include "../../include/stdint.h"
#include "../../include/typeinfo.h"
