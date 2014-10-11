#include "StdAfx.h"
#include "Glue.h"
#include "Play/PlayMgr.h"
#include "Util/DDrawWnd.h"

#define ON_NPWND_MSG_PROC(Glue, pWnd, lParam, wParam, func) \
	Glue->func(pWnd, lParam, wParam)

#define ON_CASE_MSG_PROC(uMsg, func, Glue, pWnd, lParam, wParam) \
	case uMsg: \
		return ON_NPWND_MSG_PROC(Glue, pWnd, lParam, wParam, func);

HWND g_hwndPluginCtrl;

CGlue::CGlue(void)
{
	m_iMaxWnd = MAXCHANNEL;
	m_iCurMaxWnd = MAXCHANNEL;
	m_iWndStyle = 0;
	m_iCurSelChannel = 0;
	m_iFstChannel = 0;

	m_hParentWnd = NULL;
	m_hSuperWnd = NULL;

	m_hInstance = NULL;
	m_hBorderPen = m_hBorderPen2 = NULL;
	m_bIsParentFullScreen = false;
	m_bIsChildFullScreen = false;
	m_bIsPolling = false;
	m_nSelectWnd = 0;
}

CGlue::~CGlue(void)
{
	if (m_hBorderPen)
	{
		DeletePen(m_hBorderPen);
		m_hBorderPen = NULL;
	}

	if (m_hBorderPen2)
	{
		DeletePen(m_hBorderPen2);
		m_hBorderPen2 = NULL;
	}
}

void CGlue::RegisterPlugin(CreateVarRiant vrcreate, DeleteVarRiant vrdel, Int32ToVarRiant cvint, StringToVarRiant cvstring, InvokeJavaScriptFunc func, void* data)
{
	ASSERT(func && data && cvint && cvstring && vrcreate && vrdel);
	m_jsCall = func;
	m_pUserData = data;
	m_pCvInt = cvint;
	m_pCvString = cvstring;
	m_pVrCreate = vrcreate;
	m_pVrDelete = vrdel;
}

void CGlue::OnCreate(HWND hParent, HINSTANCE hInstance)
{
	ASSERT(m_hInstance == NULL);	//OnCreate只允许被调用一次

	m_hParentWnd = hParent;
	m_hInstance = hInstance;
	g_hwndPluginCtrl = hParent;
	
	RECT rc = {0};
	for (int i = 0; i < m_iMaxWnd; i++)
	{
		char chrNum[10] = {0};
		itoa(i, chrNum, 10);

		m_VideoWnd[i].m_pGlue = this;
		m_VideoWnd[i].m_nChannelNo = i;
		m_VideoWnd[i].Create(hParent, "static", chrNum/*"VideoWnd"*/, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW, rc, hInstance);
		SetWindowLongPtr(m_VideoWnd[i].GetHWND(), GWLP_USERDATA, (LONG_PTR)&m_VideoWnd[i]);
		SetClassLong(m_VideoWnd[i].GetHWND(), GCL_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));
		SubclassWindow(m_VideoWnd[i].GetHWND(), (WNDPROC)NPWndProc);
	}

	m_hBorderPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)/*RGB(250, 225, 60)*/);
	m_hBorderPen2 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	m_VideoWnd[0].m_isSelect = true;
}

void CGlue::OnSize(int w, int h)
{
	switch (m_iMaxWnd)
	{
	case 1:
		{
			::SetWindowPos(m_VideoWnd[0].GetHWND(), NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
			DrawBorderSingle(&m_VideoWnd[0]);
		}
		break;
	case 4:
		{
			switch(m_iCurMaxWnd){
			case 1:
				if(m_iFstChannel > 3)
				{
					m_iFstChannel = 0;
				}
				if(m_iFstChannel < 0)
				{
					m_iFstChannel = 3;
				}
				::SetWindowPos(m_VideoWnd[m_iFstChannel].GetHWND(), NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
				DrawBorderSingle(&m_VideoWnd[m_iFstChannel]);
				for(int i = 0; i < m_iFstChannel; i++)
				{
					::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
				}
				for(int i = m_iFstChannel + 1; i < m_iMaxWnd; i++)
				{
					::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
				}
				break;
			case 4:
				{
					int x = w / 2;
					int y = h / 2;
					for (int i=0; i<2; i++)
					{
						for (int j=0; j<2; j++)
						{
							::SetWindowPos(m_VideoWnd[i*2+j].GetHWND(), NULL, x*j, y*i, x, y, SWP_NOZORDER | SWP_NOACTIVATE);
							DrawBorderSingle(&m_VideoWnd[i*2+j]);
						}
					}
				}
				break;
			}
		}
		break;
	case 9:
		{
			switch(m_iCurMaxWnd){
			case 1:
				if(m_iFstChannel > 8)
				{
					m_iFstChannel = 0;
				}
				if(m_iFstChannel < 0)
				{
					m_iFstChannel = 8;
				}
				::SetWindowPos(m_VideoWnd[m_iFstChannel].GetHWND(), NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
				DrawBorderSingle(&m_VideoWnd[m_iFstChannel]);

				//隐藏其他窗口
				for (int i=0; i<m_iFstChannel; i++)
				{
					::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
				}

				for (int i=m_iFstChannel+1; i<MAXCHANNEL; i++)
				{
					::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
				}
				break;
			case 4:
				{
					if(m_iFstChannel >= 8)
					{
						if(m_iFstChannel == 12)
						{
							m_iFstChannel = 0;
						}else
						{
							m_iFstChannel = 8;
						}
					}else if(m_iFstChannel >= 4 && m_iFstChannel < 8)
					{
						m_iFstChannel = 4;
					}else if(m_iFstChannel >= 0 && m_iFstChannel < 4)
					{
						m_iFstChannel = 0;
					}else
					{
						m_iFstChannel = 8;
					}
					int x = w / 2;
					int y = h / 2;
					for (int i=0; i<2; i++)
					{
						for (int j=0; j<2; j++)
						{
							if((m_iFstChannel + i*2+j) < m_iMaxWnd)
							{
								::SetWindowPos(m_VideoWnd[m_iFstChannel + i*2+j].GetHWND(), NULL, x*j, y*i, x, y, SWP_NOZORDER | SWP_NOACTIVATE);
								DrawBorderSingle(&m_VideoWnd[m_iFstChannel + i*2+j]);
							}
						}
					}

					//隐藏其他窗口
					for (int i=0; i<m_iFstChannel; i++)
					{
						::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
					}

					for (int i=m_iFstChannel + 4; i<MAXCHANNEL; i++)
					{
						::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}				
				break;
			case 6:
				{
					RECT rt[6] = {0};
					if(m_iFstChannel >= 6)
					{
						if(m_iFstChannel == 12)
						{
							m_iFstChannel = 0;
						}else
						{
							m_iFstChannel = 6;
						}
					}else if(m_iFstChannel >= 0 && m_iFstChannel < 6)
					{
						m_iFstChannel = 0;
					}else
					{
						m_iFstChannel = 6;
					}
					int x = w / 3;
					int y = h / 3;

					rt[0].left = rt[0].top = 0;
					rt[0].right = x * 2;
					rt[0].bottom = y * 2;

					for (int j=0; j<2; j++)
					{
						rt[j+1].left = x*2;
						rt[j+1].right = x*3;

						rt[j+1].top = y*j;
						rt[j+1].bottom = y + rt[j+1].top;
					}

					for (int i=0; i<3; i++)
					{
						rt[i+3].top = y*2;
						rt[i+3].bottom = y*3;

						rt[i+3].left = x*i;
						rt[i+3].right = x + rt[i+3].left;
					}

					for (int i=0; i<6; i++)
					{
						if((m_iFstChannel+i) < m_iMaxWnd)
						{
							::SetWindowPos(m_VideoWnd[m_iFstChannel+i].GetHWND(), NULL, rt[i].left, rt[i].top, rt[i].right - rt[i].left, rt[i].bottom - rt[i].top, SWP_NOZORDER | SWP_NOACTIVATE);
							DrawBorderSingle(&m_VideoWnd[m_iFstChannel + i]);
						}
					}

					//隐藏其他窗口
					for (int i=0; i<m_iFstChannel; i++)
					{
						::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
					}

					for (int i=m_iFstChannel + 6; i<MAXCHANNEL; i++)
					{
						::SetWindowPos(m_VideoWnd[i].GetHWND(), NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
				break;
			case 8:
				break;
			default:
			case 9:
				{
					int x = w / 3;
					int y = h / 3;
					for (int i=0; i<3; i++)
					{
						for (int j=0; j<3; j++)
						{
							::SetWindowPos(m_VideoWnd[i*3+j].GetHWND(), NULL, x*j, y*i, x, y, SWP_NOZORDER | SWP_NOACTIVATE);
							DrawBorderSingle(&m_VideoWnd[i*3+j]);
						}
					}
				}
				break;
			}
		}
		break;
	}
}

void CGlue::OnDestroy()
{
	
}

void CGlue::SetPropertyOfStr(char* name, char* value)
{
	if(strcmp("MaxChannel", name) == 0) m_iMaxWnd = atoi(value);
	else if (strcmp("CurChannel", name) == 0) m_iCurMaxWnd = atoi(value);
	else if (strcmp("CurStyle", name) == 0)	m_iWndStyle = atoi(value);
}

void CGlue::SetPropertyOfLong(char* name, long value)
{
	if(strcmp("MaxChannel", name) == 0) m_iMaxWnd = value;
	else if (strcmp("CurChannel", name) == 0) m_iCurMaxWnd = value;
	else if (strcmp("CurStyle", name) == 0)	m_iWndStyle = value;
}

void CGlue::GetNextPage()
{
	//m_iCurSelChannel += m_iCurMaxWnd;  //该值未复位 会一直往下加
	m_iFstChannel += m_iCurMaxWnd;

	WRITELOG("Current Selected Channel %d", m_iFstChannel);

	RECT rt;
	::GetClientRect(m_hParentWnd, &rt);
	OnSize(rt.right - rt.left, rt.bottom - rt.top);
}

void CGlue::GetPrevPage()
{
	//m_iCurSelChannel -= m_iCurMaxWnd;  //该值未复位 会一直往下减
	m_iFstChannel -= m_iCurMaxWnd;

	RECT rt;
	::GetClientRect(m_hParentWnd, &rt);
	OnSize(rt.right - rt.left, rt.bottom - rt.top);
}

void CGlue::DrawBorderAll(CNPWndProc* pWnd)
{
	for (int i=0; i<m_iMaxWnd; i++)
	{
		m_VideoWnd[i].m_isSelect = false;
		m_VideoWnd[i].DrawBorder(m_hBorderPen2);
	}
	pWnd->m_isSelect = true;
	pWnd->DrawBorder(m_hBorderPen);
}

void CGlue::DrawBorderSingle(CNPWndProc* pWnd)
{
	if(pWnd->m_isSelect == true)
	{
		pWnd->DrawBorder(m_hBorderPen);
	}
	else
	{
		pWnd->DrawBorder(m_hBorderPen2);
	}
}

void CGlue::FullScreen()
{
	HWND hWnd = m_VideoWnd[0].GetHWND();
	LONG style = ::GetWindowLong(hWnd, GWL_STYLE);
	if(!m_bIsParentFullScreen)
	{
		HWND hWndTray = FindWindow("Shell_TrayWnd", NULL);
		::SetWindowPos(hWndTray, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);

		m_hSuperWnd = ::GetParent(hWnd);
		::ShowWindow(m_hSuperWnd, SW_HIDE);
		::SetParent(hWnd, ::GetDesktopWindow());
		style &= ~(WS_DLGFRAME | WS_THICKFRAME);
		SetWindowLong(hWnd, GWL_STYLE, style);
		::ShowWindow(hWnd, SW_SHOWMAXIMIZED);

		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, nScreenWidth, nScreenHeight, SWP_FRAMECHANGED);
	}
	else
	{
		HWND hWndTray = FindWindow("Shell_TrayWnd", NULL);
		::SetWindowPos(hWndTray, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW);

		style |= WS_DLGFRAME | WS_THICKFRAME;
		SetWindowLong(hWnd, GWL_STYLE, style);
		::SetParent(hWnd, m_hSuperWnd);
		::ShowWindow(m_hSuperWnd, SW_SHOW);
	}
	m_bIsParentFullScreen = !m_bIsParentFullScreen;
}

//////////////////////////////////////////////////////////////////////////
//EnterOrExitFullScreen全屏之后hWnd無效，會中斷程序
//modified by yjt 2014-09-18 替換爲FullScreen函數
//////////////////////////////////////////////////////////////////////////
void CGlue::EnterOrExitFullScreen()
{
	FullScreen();
	return;

	if(m_hSuperWnd == NULL)
	{
		SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &m_bActScrProtection, 0); //获取屏保信息
		if(m_bActScrProtection)
		{
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, 0);		//禁用屏保
		}
		GetClientRect(m_hParentWnd, &m_rcParentWnd);
		DWORD style = 0x16000000;
		SetWindowLong(m_hParentWnd, GWL_STYLE, style);
		m_hSuperWnd = SetParent(m_hParentWnd, NULL);
		m_bIsParentFullScreen = true;
		SetWindowPos(m_hParentWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
		
		//不能SetFocus： 因为SetFocus在播放时会消毁窗口？
		//SetFocus(GetSelectedChanl().GetHWND());
	}
	else
	{
		if(m_bActScrProtection)
		{
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, 0);	//启用屏保
		}
		DWORD style = 0x52000000;
		SetWindowLong(m_hParentWnd, GWL_STYLE, style);
		SetParent(m_hParentWnd, m_hSuperWnd);
		m_hSuperWnd = NULL;
		m_bIsParentFullScreen = false;
		SetWindowPos(m_hParentWnd, NULL, m_rcParentWnd.left, m_rcParentWnd.top, m_rcParentWnd.right, m_rcParentWnd.bottom, SWP_SHOWWINDOW);
	}
}

void CGlue::EnterOrExitFullScreenForSigChanl(HWND hWnd)
{
	if(GetParent(hWnd) == m_hParentWnd)
	{
		SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &m_bActScrProtection, 0); //获取屏保信息
		if(m_bActScrProtection)
		{
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, 0);		//禁用屏保
		}
		GetClientRect(hWnd, &m_rcChildWnd);
		GetWindowRect(hWnd, &m_rcChildWndPos);
		GetWindowRect(m_hParentWnd, &m_rcParentWndPos);
		DWORD style = 0x16000000;
		SetWindowLong(hWnd, GWL_STYLE, style);
		SetParent(hWnd, NULL);
		m_bIsChildFullScreen = true;
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
		//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 500, 500, SWP_SHOWWINDOW);
		SetFocus(hWnd);
	}
	else
	{
		if(m_bActScrProtection)
		{
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, 0);	//启用屏保
		}
		DWORD style = 0x52000000;
		SetWindowLong(hWnd, GWL_STYLE, style);
		SetParent(hWnd, m_hParentWnd);
		m_bIsChildFullScreen = false;
		SetWindowPos(hWnd, NULL, m_rcChildWndPos.left - m_rcParentWndPos.left, m_rcChildWndPos.top - m_rcParentWndPos.top, m_rcChildWnd.right, m_rcChildWnd.bottom, SWP_SHOWWINDOW);
	}
}

void CGlue::SetSelectedChanl(CNPWndProc* pWnd)
{
	for(int i = 0; i < m_iMaxWnd; i++)
	{
		if(m_VideoWnd[i] == *pWnd)
		{
			pWnd->m_isSelect = true;
			m_nSelectWnd = i;
		}
		else
		{
			m_VideoWnd[i].m_isSelect = false;
		}
	}
}

CNPWndProc CGlue::GetSelectedChanl()
{
	for(int i = 0; i < m_iMaxWnd; i++)
	{
		if(m_VideoWnd[i].m_isSelect == true)
		{
			return m_VideoWnd[i];
		}
	}
	return m_VideoWnd[0];
}

LRESULT CALLBACK CGlue::NPWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CNPWnd* pWnd = (CNPWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	CGlue*	pGlue = static_cast<CGlue*>(pWnd->m_pGlue);

	switch (uMsg)
	{
		ON_CASE_MSG_PROC(WM_MOUSEWHEEL, OnMouseWheel, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_LBUTTONDOWN, OnLButtonDown, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_LBUTTONUP, OnLButtonUp, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_RBUTTONDOWN, OnRButtonDown, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_RBUTTONUP, OnRButtonUp, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_MOUSEMOVE, OnMouseMove, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_KEYDOWN, OnKeyDown, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_LBUTTONDBLCLK, OnLButtonDBClick, pGlue, pWnd, wParam, lParam)
		//ON_CASE_MSG_PROC(WM_PAINT, OnPaint, pGlue, pWnd, wParam, lParam)
		ON_CASE_MSG_PROC(WM_SIZE, OnSize, pGlue, pWnd, wParam, lParam)
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CGlue::OnPaint(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnPaint(wParam, lParam);
}

LRESULT CGlue::OnLButtonDBClick(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnLButtonDBClick(wParam, lParam);
}

LRESULT CGlue::OnMouseWheel(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnMouseWheel(wParam, lParam);
}

LRESULT CGlue::OnLButtonDown(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnLButtonDown(wParam, lParam);
}

LRESULT CGlue::OnLButtonUp(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnLButtonUp(wParam, lParam);
}

LRESULT CGlue::OnRButtonDown(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnRButtonDown(wParam, lParam);
}

LRESULT CGlue::OnRButtonUp(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnRButtonUp(wParam, lParam);
}

LRESULT CGlue::OnMouseMove(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnMouseMove(wParam, lParam);
}

LRESULT CGlue::OnKeyDown(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnKeyDown(wParam, lParam);
}

LRESULT CGlue::OnSize(CNPWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	return pWnd->OnSize(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
//Operate
//////////////////////////////////////////////////////////////////////////
bool CGlue::OpenMedia(const char* mediaPath, short nType)
{
	return m_VideoWnd[m_nSelectWnd].OpenMedia(mediaPath, nType);
}

bool CGlue::CloseMedia()
{
	return m_VideoWnd[m_nSelectWnd].CloseMedia();
}

bool CGlue::Play()
{
	return m_VideoWnd[m_nSelectWnd].Play();
}

bool CGlue::Pause()
{
	return m_VideoWnd[m_nSelectWnd].Pause();
}

bool CGlue::Stop()
{
	return m_VideoWnd[m_nSelectWnd].Stop();
}

bool CGlue::Forward(short nSpeed)
{
	return m_VideoWnd[m_nSelectWnd].Forward(nSpeed);
}

bool CGlue::Reverse(short nSpeed)
{
	return m_VideoWnd[m_nSelectWnd].Reverse(nSpeed);
}

void CGlue::PreFrame()
{
	m_VideoWnd[m_nSelectWnd].PreFrame();
}

void CGlue::NextFrame()
{
	m_VideoWnd[m_nSelectWnd].NextFrame();
}

bool CGlue::Seek(long pos, long minVal, long maxVal)
{
	return m_VideoWnd[m_nSelectWnd].Seek(pos, minVal, maxVal);
}

short CGlue::Snap(const char* savePath)
{
	return m_VideoWnd[m_nSelectWnd].Snap(savePath);
}

short CGlue::SetSelectWnd(short nWndIndex)
{
	if(nWndIndex < 0 || nWndIndex > m_iMaxWnd)
	{
		return -1;
	}

	for(int i = 0; i < m_iMaxWnd; i++)
	{
		if( i == nWndIndex )
		{
			m_nSelectWnd = i;
			m_VideoWnd[i].m_isSelect = true;
		}
		else
		{
			m_VideoWnd[i].m_isSelect = false;
		}
	}

	return 0;
}

short CGlue::SetWndChannels(short nWndId, short nChannels)
{
	if(nWndId < 0 || nWndId > m_iMaxWnd)
	{
		return -1;
	}
	m_VideoWnd[nWndId].SetChannels(nChannels);
	return 0;
}

void CGlue::Exit()
{
	for (int i = 0; i < m_iMaxWnd; ++i)
	{
		m_VideoWnd[i].Exit();
	}
}

void CGlue::GetPlayPos(long* curPos, long* totalFrames)
{
	m_VideoWnd[m_nSelectWnd].GetPlayPos(curPos, totalFrames);
}

short CGlue::SetWndChannelSelect(short nWndIndex, short nChannel)
{
	if(nWndIndex < 0 || nWndIndex > m_iMaxWnd)
		return -1;

	m_VideoWnd[nWndIndex].SetSelectChannel(nChannel);
	return 0;
}

short CGlue::GetWndChannelSelect(short nWndIndex)
{
	if(nWndIndex < 0 || nWndIndex > m_iMaxWnd)
		return -1;

	return m_VideoWnd[nWndIndex].GetDDrawView().GetSelectWndID();
}

short CGlue::GetWndChannelCount(short nWndIndex)
{
	if(nWndIndex < 0 || nWndIndex > m_iMaxWnd)
		return -1;

	return m_VideoWnd[nWndIndex].GetChannels();
}