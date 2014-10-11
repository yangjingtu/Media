// fsNVRPlugin.cpp : CfsNVRPluginApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "fsNVRPlugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include  <comcat.h>
#include <Objsafe.h>

//本控件的CLSID,注册表用
const GUID CDECL CLSID_SafeItem = 
{0x35ea3d63, 0x87a2, 0x4f31, { 0xa5, 0x96, 0xa1, 0xdf, 0x79, 0x33, 0xdf, 0xa5}};

//创建组件种类 
HRESULT   CreateComponentCategory(CATID   catid,   WCHAR*   catDescription) 
{ 
	ICatRegister*   pcr   =   NULL   ; 
	HRESULT   hr   =   S_OK   ; 
	hr   =   CoCreateInstance(CLSID_StdComponentCategoriesMgr,   
		NULL,   CLSCTX_INPROC_SERVER,   IID_ICatRegister,   (void**)&pcr); 
	if   (FAILED(hr)) 
		return   hr; 
	//   Make   sure   the   HKCR\Component   Categories\{..catid...} 
	//   key   is   registered. 
	CATEGORYINFO   catinfo; 
	catinfo.catid   =   catid; 
	catinfo.lcid   =   0x0409   ;   //   english 
	//   Make   sure   the   provided   description   is   not   too   long. 
	//   Only   copy   the   first   127   characters   if   it   is. 
	int   len   =   wcslen(catDescription); 
	if   (len> 127) 
		len   =   127; 
	wcsncpy(catinfo.szDescription,   catDescription,   len); 
	//   Make   sure   the   description   is   null   terminated. 
	catinfo.szDescription[len]   =   '\0 '; 
	hr   =   pcr-> RegisterCategories(1,   &catinfo); 
	pcr-> Release(); 
	return   hr; 
} 

//注册组件种类 
HRESULT   RegisterCLSIDInCategory(REFCLSID   clsid,   CATID   catid) 
{ 
	//Register   your   component   categories   information. 
	ICatRegister*   pcr   =   NULL   ; 
	HRESULT hr = S_OK   ; 
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if(SUCCEEDED(hr)) 
	{ 
		// Register   this   category   as   being   "implemented "   by   the   class. 
		CATID rgcatid[1]; 
		rgcatid[0] = catid; 
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid); 
	} 
	if(pcr != NULL) 
		pcr->Release(); 
	return   hr; 
} 

//卸载组件种类 
HRESULT   UnRegisterCLSIDInCategory(REFCLSID   clsid,   CATID   catid) 
{ 
	ICatRegister*   pcr   =   NULL   ; 
	HRESULT   hr   =   S_OK   ; 
	hr   =   CoCreateInstance(CLSID_StdComponentCategoriesMgr,   
		NULL,   CLSCTX_INPROC_SERVER,   IID_ICatRegister,   (void**)&pcr); 
	if   (SUCCEEDED(hr)) 
	{ 
		//   Unregister   this   category   as   being   "implemented "   by   the   class. 
		CATID   rgcatid[1]   ; 
		rgcatid[0]   =   catid; 
		hr   =   pcr-> UnRegisterClassImplCategories(clsid,   1,   rgcatid); 
	} 
	if   (pcr   !=   NULL) 
		pcr-> Release(); 
	return   hr; 
} 


CfsNVRPluginApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x7268A885, 0xF493, 0x46F2, { 0x8D, 0xED, 0xF0, 0x61, 0x62, 0x5C, 0x78, 0xBA } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CfsNVRPluginApp::InitInstance - DLL 初始化

BOOL CfsNVRPluginApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	WRITELOG("/*********************************************************************/");
	WRITELOG("/**************************ActiveX Log Begin**************************/");
	WRITELOG("/*********************************************************************/");

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
		m_bInit = bInit;
	}
	return bInit;
}



// CfsNVRPluginApp::ExitInstance - DLL 终止

int CfsNVRPluginApp::ExitInstance()
{
	if(m_bInit)
	{
		//WRITELOG("/*********************************************************************/");
		//WRITELOG("/**************************ActiveX Log END**************************/");
		//WRITELOG("/*********************************************************************/");
		m_bInit = FALSE;

		return COleControlModule::ExitInstance();
	}
	return 0;
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	/*AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;*/

	HRESULT hr; 
	AFX_MANAGE_STATE(_afxModuleAddrThis); 
	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid)) 
		return ResultFromScode(SELFREG_E_TYPELIB); 
	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE)) 
		return ResultFromScode(SELFREG_E_CLASS); 
	//标记控件初始化安全. 
	//创建初始化安全组件种类 
	hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!"); 
	if (FAILED(hr)) 
		return hr; 
	//注册初始化安全 
	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing); 
	if (FAILED(hr)) 
		return hr; 
	//标记控件脚本安全 
	//创建脚本安全组件种类   
	hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!"); 
	if (FAILED(hr)) 
		return hr; 
	//注册脚本安全组件种类 
	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting); 
	if (FAILED(hr)) 
		return hr; 
	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	/*AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;*/

	AFX_MANAGE_STATE(_afxModuleAddrThis); 
	HRESULT hr; 
	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor)) 
		return ResultFromScode(SELFREG_E_TYPELIB); 
	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE)) 
		return ResultFromScode(SELFREG_E_CLASS); 
	//删除控件初始化安全入口. 
	hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing); 
	if(FAILED(hr)) 
		return   hr; 
	//删除控件脚本安全入口 
	hr = UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting); 
	if (FAILED(hr)) 
		return   hr; 

	return NOERROR; 
}
