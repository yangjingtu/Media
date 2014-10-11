#pragma once

// fsNVRPlugin.h : fsNVRPlugin.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号


// CfsNVRPluginApp : 有关实现的信息，请参阅 fsNVRPlugin.cpp。

class CfsNVRPluginApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();

private:
	BOOL m_bInit;
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

