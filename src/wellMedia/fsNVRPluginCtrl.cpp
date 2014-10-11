// fsNVRPluginCtrl.cpp : CfsNVRPluginCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "fsNVRPlugin.h"
#include "fsNVRPluginCtrl.h"
#include "fsNVRPluginPropPage.h"
//#include "Log.h"
#include "CommDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CfsNVRPluginCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CfsNVRPluginCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_EVENT_CALLBACK, OnEventCallBack)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CfsNVRPluginCtrl, COleControl)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "test", dispidtest, test, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "SetWndStyle", dispidSetWndStyle, SetWndStyle, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "ChangePage", dispidChangePage, ChangePage, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Polling", dispidPolling, Polling, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "FullScreen", dispidFullScreen, FullScreen, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "OpenMedia", dispidOpenMedia, OpenMedia, VT_I2, VTS_BSTR VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "CloseMedia", dispidCloseMedia, CloseMedia, VT_I2, VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Play", dispidPlay, Play, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Pause", dispidPause, Pause, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Stop", dispidStop, Stop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Forward", dispidForward, Forward, VT_BOOL, VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Reverse", dispidReverse, Reverse, VT_BOOL, VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "PreFrame", dispidPreFrame, PreFrame, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "NextFrame", dispidNextFrame, NextFrame, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Seek", dispidSeek, Seek, VT_BOOL, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Snap", dispidSnap, Snap, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "SetSelectWnd", dispidSetSelectWnd, SetSelectWnd, VT_BOOL, VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "SetWndChannels", dispidSetWndChannels, SetWndChannels, VT_I2, VTS_I2 VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "Exit", dispidExit, Exit, VT_I2, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "GetPlayPos", dispidGetPlayPos, GetPlayPos, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "GetCurPos", dispidGetCurPos, GetCurPos, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "GetTotalFrames", dispidGetTotalFrames, GetTotalFrames, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "SetWndChannelSelect", dispidSetWndChannelSelect, SetWndChannelSelect, VT_I2, VTS_I2 VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "GetWndChannelSelect", dispidGetWndChannelSelect, GetWndChannelSelect, VT_I2, VTS_I2)
	DISP_FUNCTION_ID(CfsNVRPluginCtrl, "GetWndChannelCount", dispidGetWndChannelCount, GetWndChannelCount, VT_I2, VTS_I2)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CfsNVRPluginCtrl, COleControl)
EVENT_CUSTOM_ID("OnTwoWndSwap", eventidOnTwoWndSwap, OnTwoWndSwap, VTS_I4 VTS_I4)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CfsNVRPluginCtrl, 1)
	PROPPAGEID(CfsNVRPluginPropPage::guid)
END_PROPPAGEIDS(CfsNVRPluginCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CfsNVRPluginCtrl, "FSNVRPLUGIN.fsNVRPluginCtrl.1",
	0x35ea3d63, 0x87a2, 0x4f31, 0xa5, 0x96, 0xa1, 0xdf, 0x79, 0x33, 0xdf, 0xa5)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CfsNVRPluginCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_DfsNVRPlugin =
		{ 0x83DDB94A, 0x2188, 0x4895, { 0xBC, 0x62, 0xAF, 0xFE, 0xE1, 0xA4, 0xA5, 0x9E } };
const IID BASED_CODE IID_DfsNVRPluginEvents =
		{ 0xA89C02DB, 0x6452, 0x43F4, { 0x94, 0x65, 0xEA, 0xBC, 0x6B, 0x7C, 0x8B, 0xD8 } };



// 控件类型信息

static const DWORD BASED_CODE _dwfsNVRPluginOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CfsNVRPluginCtrl, IDS_FSNVRPLUGIN, _dwfsNVRPluginOleMisc)



// CfsNVRPluginCtrl::CfsNVRPluginCtrlFactory::UpdateRegistry -
// 添加或移除 CfsNVRPluginCtrl 的系统注册表项

BOOL CfsNVRPluginCtrl::CfsNVRPluginCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_FSNVRPLUGIN,
			IDB_FSNVRPLUGIN,
			afxRegApartmentThreading,
			_dwfsNVRPluginOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CfsNVRPluginCtrl::CfsNVRPluginCtrl - 构造函数

CfsNVRPluginCtrl::CfsNVRPluginCtrl()
{
	InitializeIIDs(&IID_DfsNVRPlugin, &IID_DfsNVRPluginEvents);
	// TODO: 在此初始化控件的实例数据。

// 	m_iMaxWnd = MAXCHANNEL;
// 	m_iCurMaxWnd = MAXCHANNEL;
// 	m_iCurSelChannel = 0;

	WRITELOG("Glue_InitializeGlue %s %d", __FILE__, __LINE__);
	mGlue = Glue_InitializeGlue();
	m_bIsPolling = false;
}



// CfsNVRPluginCtrl::~CfsNVRPluginCtrl - 析构函数

CfsNVRPluginCtrl::~CfsNVRPluginCtrl()
{
	// TODO: 在此清理控件的实例数据。
	if(mGlue)
	{
		Glue_DeleteGlue(mGlue);
	}
	WRITELOG("Glue_DeleteGlue %s %d", __FILE__, __LINE__);
	mGlue = NULL;
}



// CfsNVRPluginCtrl::OnDraw - 绘图函数

void CfsNVRPluginCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CfsNVRPluginCtrl::DoPropExchange - 持久性支持

void CfsNVRPluginCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
	long nChannel = 100;
	long nCurChannel = 100;
	long nCurStyle = 100;
	if   (pPX-> GetVersion()   ==   (DWORD)MAKELONG(_wVerMinor,   _wVerMajor)) 
	{
		PX_Long(pPX, _T("MaxChannel"), nChannel);
		PX_Long(pPX, _T("CurChannel"), nCurChannel);
		PX_Long(pPX, _T("CurStyle"), nCurStyle);
 		if (nChannel <= 9 && nChannel >= 1 && nCurChannel <= 9 && nCurChannel >= 1 && nCurStyle <= 1 && nCurChannel >=0 )
 		{
			Glue_SetProperty_Long(mGlue, "MaxChannel", nChannel);
			Glue_SetProperty_Long(mGlue, "CurChannel", nCurChannel);
			Glue_SetProperty_Long(mGlue, "CurStyle", nCurStyle);
			WRITELOG("Set MaxChannel %d %s %d", nChannel, __FILE__, __LINE__);
			WRITELOG("Set CurChannel %d %s %d", nCurChannel, __FILE__, __LINE__);
			WRITELOG("Set CurStyle %d %s %d", nCurStyle, __FILE__, __LINE__);
 		}
	} 
}



// CfsNVRPluginCtrl::OnResetState - 将控件重置为默认状态

void CfsNVRPluginCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CfsNVRPluginCtrl 消息处理程序

void CfsNVRPluginCtrl::test(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码

	AfxMessageBox(_T("test"));
}

int CfsNVRPluginCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	Glue_OnCreate(mGlue, m_hWnd, AfxGetInstanceHandle());
	WRITELOG("Create Children Windows");
	return 0;
}

void CfsNVRPluginCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CfsNVRPluginCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Glue_OnSize(mGlue, cx, cy);
	WRITELOG("Change Windows Size");
}

void CfsNVRPluginCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:

	COleControl::OnTimer(nIDEvent);
	if(nIDEvent == TIMERPOLLING)
	{
		Glue_GetNextPage(mGlue);
	}
}

LRESULT CfsNVRPluginCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(message)
	{
	case VK_ESCAPE:
		{
			MessageBox("Parent Message");
			//FullScreen();
		}
		break;
	}
	
	return COleControl::WindowProc(message, wParam, lParam);
}

void CfsNVRPluginCtrl::OnSetClientSite()
{
	// TODO: 在此添加专用代码和/或调用基类
	try
	{
		LPOLECLIENTSITE pClientSite = GetClientSite();
		LPOLECONTAINER  pOleContainer;
		if (pClientSite == NULL)
		{
			goto RET;
		}

		HRESULT hr = pClientSite->GetContainer(&pOleContainer);
		if (FAILED(hr))
		{
			goto RET;
		}

		IHTMLDocument* doc = NULL;
		hr =  pOleContainer->QueryInterface(IID_IHTMLDocument2,reinterpret_cast <void   **> (&doc)); 
		if (FAILED(hr))
		{
			goto RET;
		}

		IDispatch* script = NULL;
		hr = doc->get_Script(&script);
		doc->Release();
		if(FAILED(hr))
		{
			goto RET;
		}

		m_pScript = script;
	}
	catch (...)
	{

	}
RET:
	COleControl::OnSetClientSite();
}

int CfsNVRPluginCtrl::InvokeJavaScriptFunc(OLECHAR FAR* bstrMember, VARIANT* argv, int argc, bool npass)
{
	if (npass)
	{
		return 0;
	}

	if(m_pScript == NULL)
	{ 
		return -1; 
	}

	DISPID dispid;

	HRESULT hr = m_pScript->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_USER_DEFAULT, &dispid); 
	if(FAILED(hr))
	{	
#ifdef _DEBUG 
		switch(hr) 
		{ 
		case E_OUTOFMEMORY:
			break; 
		case DISP_E_UNKNOWNNAME:
			break; 
		case DISP_E_UNKNOWNLCID: 
			break; 
		} 
#endif 
		return -1; 
	}

	DISPPARAMS dispparams; 
	memset(&dispparams, 0, sizeof(dispparams));

	dispparams.rgvarg = argv;
	dispparams.cArgs = argc;

	EXCEPINFO   excepInfo; 
	memset(&excepInfo, 0, sizeof(excepInfo)); 
	CComVariant vaResult; 
	UINT nArgErr = (UINT)-1;
	hr = m_pScript->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr);
	return 0;
}

void CfsNVRPluginCtrl::SetWndStyle(LONG style, LONG CHNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加切换视频路数代码
	if (style >= 0 && style <=1 && CHNum > 0 && CHNum <= 9)
	{
		Glue_SetProperty_Long(mGlue, "CurChannel", CHNum);
		Glue_SetProperty_Long(mGlue, "CurStyle", style);
		WRITELOG("SetWndStyle");

		RECT rt;
		GetWindowRect(&rt);
		Glue_OnSize(mGlue, rt.right - rt.left, rt.bottom - rt.top);
		WRITELOG("SetWndStyle end");
	}

	
}
void CfsNVRPluginCtrl::ChangePage(LONG bNext)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加上一页、下一页代码
	if (bNext)
	{
		Glue_GetPrevPage(mGlue);
	}else{
		Glue_GetNextPage(mGlue);
	}
	WRITELOG("ChangePage %d", bNext);
}

void CfsNVRPluginCtrl::Polling(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	if(m_bIsPolling == false)
	{
		SetTimer(TIMERPOLLING, 3000, NULL);
		m_bIsPolling = true;
		WRITELOG("Start Timer");
	}
	else
	{
		KillTimer(TIMERPOLLING);
		m_bIsPolling = false;
		WRITELOG("Kill Timer");
	}
}

void CfsNVRPluginCtrl::FullScreen(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	Glue_FullScreen(mGlue);
	WRITELOG("Full Screen");
}

BOOL CfsNVRPluginCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:

	cs.dwExStyle = WS_EX_TOOLWINDOW;
	return COleControl::PreCreateWindow(cs);
}
#if 1
SHORT CfsNVRPluginCtrl::OpenMedia(LPCTSTR bstrPath, SHORT nType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	bool isOk = Glue_OpenMedia(mGlue, bstrPath, nType);
	WRITELOG("Glue_OpenMedia rlt:%d", isOk);
	return 0;
}


SHORT CfsNVRPluginCtrl::CloseMedia(SHORT nWndIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_CloseMedia(mGlue);
	WRITELOG("Glue_CloseMedia");
	return 0;
}


VARIANT_BOOL CfsNVRPluginCtrl::Play(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Play(mGlue);
	WRITELOG("Glue_Play");

	return VARIANT_TRUE;
}


VARIANT_BOOL CfsNVRPluginCtrl::Pause(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Pause(mGlue);
	WRITELOG("Glue_Pause");

	return VARIANT_TRUE;
}


VARIANT_BOOL CfsNVRPluginCtrl::Stop(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Stop(mGlue);
	WRITELOG("Glue_Stop");
	return VARIANT_TRUE;
}


VARIANT_BOOL CfsNVRPluginCtrl::Forward(SHORT nSpeed)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Forward(mGlue, nSpeed);
	WRITELOG("Glue_Forward speed:%c", nSpeed);

	return VARIANT_TRUE;
}


VARIANT_BOOL CfsNVRPluginCtrl::Reverse(SHORT nSpeed)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Reverse(mGlue, nSpeed);
	WRITELOG("Reverse speed:%d", nSpeed);

	return VARIANT_TRUE;
}


void CfsNVRPluginCtrl::PreFrame(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_PreFrame(mGlue);
	WRITELOG("Glue_PreFrame");
}


void CfsNVRPluginCtrl::NextFrame(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_NextFrame(mGlue);
	WRITELOG("NextFrame");
}


VARIANT_BOOL CfsNVRPluginCtrl::Seek(LONG lPos, LONG minVal, LONG maxVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Seek(mGlue, lPos, minVal, maxVal);
	WRITELOG("Seek");

	return VARIANT_TRUE;
}


SHORT CfsNVRPluginCtrl::Snap(LPCTSTR savePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	SHORT rlt = Glue_Snap(mGlue, savePath);
	WRITELOG("Glue_Snap return[%d], savepath:%s", rlt,  savePath);
	return rlt;
}


VARIANT_BOOL CfsNVRPluginCtrl::SetSelectWnd(SHORT nWndIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRlt = Glue_SetSelectWnd(mGlue, nWndIndex);
	WRITELOG("Glue_SetSelectWnd [%d], return[%d]", nWndIndex, bRlt);
	return bRlt;
}


SHORT CfsNVRPluginCtrl::SetWndChannels(SHORT nWndId, SHORT nChannels)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	WRITELOG("Glue_SetWndChannels [%d], [%d]", nWndId, nChannels);
	return Glue_SetWndChannels(mGlue, nWndId, nChannels);
}


SHORT CfsNVRPluginCtrl::Exit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	Glue_Exit(mGlue);
	WRITELOG("Glue_Exit");
	Sleep(300);
	if(mGlue)
	{
		Glue_DeleteGlue(mGlue);
		mGlue = NULL;
	}

	return 0;
}


void CfsNVRPluginCtrl::GetPlayPos(LONG* curPos, LONG* totalFrames)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return Glue_GetPlayPos(mGlue, curPos, totalFrames);
}

LONG CfsNVRPluginCtrl::GetCurPos(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG nCur = 0;
	LONG nTotal = 0;
	Glue_GetPlayPos(mGlue, &nCur, &nTotal);
	return nCur;
}


LONG CfsNVRPluginCtrl::GetTotalFrames(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	LONG nCur = 0;
	LONG nTotal = 0;
	Glue_GetPlayPos(mGlue, &nCur, &nTotal);
	return nTotal;
}


SHORT CfsNVRPluginCtrl::SetWndChannelSelect(SHORT nWndIndex, SHORT nChannel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WRITELOG("SetWndChannelSelect  [%d][%d]", nWndIndex, nChannel);
	return Glue_SetWndChannelSelect(mGlue, nWndIndex, nChannel);
}


SHORT CfsNVRPluginCtrl::GetWndChannelSelect(SHORT nWndIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SHORT index = Glue_GetWndChannelSelect(mGlue, nWndIndex);
	//WRITELOG("GetWndChannelSelect  [%d][%d]", nWndIndex, index);
	return index;
}


SHORT CfsNVRPluginCtrl::GetWndChannelCount(SHORT nWndIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SHORT count = Glue_GetWndChannelCount(mGlue, nWndIndex);
	WRITELOG("Glue_GetWndChannelCount  [%d][%d]", nWndIndex, count);
	return count;
}


LRESULT CfsNVRPluginCtrl::OnEventCallBack(WPARAM wParam, LPARAM lParam)
{
	switch ((UINT)wParam)
	{
	case MSG_TWOWND_SWAP:
		{
			PTwoWndSwapInfo pInfo = (PTwoWndSwapInfo)lParam;
			if(pInfo)
			{
				OnTwoWndSwap(pInfo->newWnd, pInfo->oldWnd);
				WRITELOG("OnTwoWndSwap  [%d][%d]", pInfo->newWnd, pInfo->oldWnd);
				delete pInfo;
				pInfo = NULL;
			}
		}
	default:
		break;
	}
	return S_OK;
}

#endif