#include "StdAfx.h"
#include "NPWndProc.h"
#include "../wellMedia/CommDataDef.h"

extern HWND g_hwndPluginCtrl;

#define  CALL_PLAYOBJ_METHOD(method) \
	CPlayHelper* pObj = GetSelectPlayObj();\
	if(pObj) { return pObj->method(); }

#define CALL_PLAYOBJ_METHOD_PARAM(method, param) \
	CPlayHelper* pObj = GetSelectPlayObj();\
	if(pObj) { return pObj->method(param); }

CNPWndProc::CNPWndProc(void)
{
	m_pGlue = NULL;
	m_isSelect = false;
	m_nChannelNo = 0;
	
	m_nWndStyle = 0;
	m_nOldWnd = 1;
	m_nOldStyle = 0;
	m_nOldPage = 0;

	m_bMouseDown = false;

	m_nSelectWndId = 1;
	m_nWndCount = 1;
}

CNPWndProc::~CNPWndProc(void)
{
	m_PlayMgr.UnInit();

	CloseMedia();
}

HWND CNPWndProc::Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HINSTANCE hInstance, HMENU hMenu)
{
	HWND hWnd = CNPWnd::Create(hwndParent, pstrClassName, pstrName,dwStyle, dwExStyle, rc, hInstance, hMenu);

	m_PlayMgr.Init(hWnd);

	return hWnd;
}

LRESULT CNPWndProc::OnPaint(WPARAM wParam, LPARAM lParam)
{
#if MULIT_WND
	PAINTSTRUCT ps;
	HDC hdc;
	char chrChannelNo[2] = {0};
	itoa(m_nChannelNo, chrChannelNo, 10);
	hdc = BeginPaint(m_hWnd, &ps);
	if (m_isSelect)
	{
		DrawBorder(m_pGlue->GetBoderPen());

		TextOut(hdc, 20, 20, chrChannelNo, 1);
	}else{
		DrawBorder(m_pGlue->GetBoderPen2());
		TextOut(hdc, 20, 20, chrChannelNo, 1);
	}

	EndPaint(m_hWnd, &ps);
#endif
	return S_OK;
}

LRESULT CNPWndProc::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWndProc::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{

#if MULIT_WND
	SetFocus(m_hWnd);
	m_pGlue->SetSelectedChanl(this);
	m_pGlue->DrawBorderAll(this);
#endif

	m_bMouseDown = true;
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	RECT rt = {0};
	GetWindowRect(m_hWnd, &rt);
	if (pt.x >= 0 && pt.y >= 0 && pt.x <= rt.right - rt.left && pt.y <= rt.bottom - rt.top)
	{
		GetDDrawView().DrawSelectBorder(pt);
		m_nSelectWndId = GetDDrawView().GetSelectWndID();
	}

	return S_OK;
}

LRESULT CNPWndProc::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (m_bMouseDown)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_bMouseDown = false;
		int newID = GetDDrawView().GetWndID(pt);
		int oldID = GetDDrawView().GetSelectWndID();
		if (newID != oldID)
		{
			m_PlayMgr.ChangeWnd(newID, oldID);
			m_PlayMgr.GetDDrawView().DrawSelectBorder(newID);
			m_PlayMgr.PostMsg(CLEARSCREEN, 100, lParam);

			//交換窗口時回調告訴瀏覽器
			PTwoWndSwapInfo pInfo = new TwoWndSwapInfo;
			pInfo->newWnd = newID;
			pInfo->oldWnd = oldID;
			CB_EVENT( MSG_TWOWND_SWAP, pInfo);

			//交換窗口后選擇窗口也髮生了改變
			m_nSelectWndId = newID;
		}
		else
		{

		}
	}
	return S_OK;
}

LRESULT CNPWndProc::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_pGlue->IsParentFullScreen())
	{
		m_pGlue->EnterOrExitFullScreen();
	}
	return S_OK;
}

LRESULT CNPWndProc::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWndProc::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);		//  为鼠标x坐标
	int y = HIWORD(lParam);		//  为鼠标y坐标

	return S_OK;
}

LRESULT CNPWndProc::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		{
			if(m_pGlue->IsChildFullScreen())
			{
				m_pGlue->EnterOrExitFullScreenForSigChanl(GetHWND());
			}
			if(m_pGlue->IsParentFullScreen())
			{
				m_pGlue->EnterOrExitFullScreen();
			}
		}
		break;
	}
	return S_OK;
}

LRESULT CNPWndProc::OnLButtonDBClick(WPARAM wParam, LPARAM lParam)
{

#if MULIT_WND
	SetFocus(m_hWnd);
	m_pGlue->SetSelectedChanl(this);
	m_pGlue->DrawBorderAll(this);
	m_pGlue->EnterOrExitFullScreenForSigChanl(m_hWnd);
#endif

	int wnd, style;
	GetDDrawView().GetWndStyle(wnd, style);
	if (wnd == 1 && style == 0)
	{
		GetDDrawView().SetWndStyle(m_nOldWnd, m_nOldStyle);
		GetDDrawView().ShowPage(m_nOldPage);
		GetDDrawView().ReSetSCRect();
		m_PlayMgr.PostMsg(CLEARSCREEN, NULL, NULL);
	}else{
		m_nOldPage = GetDDrawView().GetNowPage();
		m_nOldWnd = wnd;
		m_nOldStyle = style;
		int iWnd = GetDDrawView().GetSelectWndID();
		GetDDrawView().SetWndStyle(1, 0);
		GetDDrawView().ShowPage(iWnd - 1);
		GetDDrawView().ReSetSCRect();
		m_PlayMgr.PostMsg(CLEARSCREEN, NULL, NULL);
	}

	return S_OK;
}

void CNPWndProc::OnTimer(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{
	switch (iTimerId)
	{
	default:
		break;
	}

}

void CNPWndProc::OnDestroy()
{
	//关闭所有的窗口(索引从1开始)
	for (int i = 1; i <= m_nWndCount; ++i)
	{
		CPlayHelper* pObj = m_PlayMgr.FindPlayObj(m_nSelectWndId);
		if(pObj)
		{
			BOOL bRlt = pObj->Close();
		}
	}
}

void CNPWndProc::DrawBorder(HPEN hPen)
{
	RECT rc;
	HDC hDC = ::GetDC(m_hWnd);
	::GetClientRect(m_hWnd, &rc);
	SelectObject(hDC, hPen);
	::SelectObject(hDC, ::GetStockObject(BLACK_BRUSH/*HOLLOW_BRUSH*/));
	Rectangle(hDC, rc.left+1, rc.top+1, rc.right-1, rc.bottom-1);

	char chrChannelNo[2] = {0};
	itoa(m_nChannelNo, chrChannelNo, 10);
	TextOut(hDC, 20, 20, chrChannelNo, 1);
	ReleaseDC(m_hWnd, hDC);
}

CPlayHelper* CNPWndProc::GetSelectPlayObj()
{
	return m_PlayMgr.FindPlayObj(m_nSelectWndId);
}

bool CNPWndProc::RemoveSelectPlayObj()
{
	m_PlayMgr.RemovePlayObj(m_nSelectWndId);
	return true;
}

CDDrawView& CNPWndProc::GetDDrawView()
{
		return m_PlayMgr.GetDDrawView();
}

//////////////////////////////////////////////////////////////////////////
//Operate
//////////////////////////////////////////////////////////////////////////
BOOL CNPWndProc::OpenMedia(const char* mediaPath, short nType)
{
	CPlayHelper* pObj = GetSelectPlayObj();
	if(pObj == NULL)
	{
		pObj = m_PlayMgr.CreatePlayObj(EM_NET_FILE, m_nSelectWndId);
		//pObj = m_PlayMgr.CreatePlayObj(EM_LOCAL_FILE, m_nSelectWndId);

		if(pObj)
		{
			return pObj->Open(mediaPath, nType);
		}
	}
	return TRUE;
}

BOOL CNPWndProc::CloseMedia()
{
	CPlayHelper* pObj = GetSelectPlayObj();
	if(pObj)
	{
		BOOL bRlt = pObj->Close();
		if(bRlt)
		{
			RemoveSelectPlayObj();
			m_PlayMgr.PostMsg(CLEARSCREEN, NULL, NULL);
		}
		return bRlt;
	}
	return TRUE;
}

BOOL CNPWndProc::Play()
{
	CALL_PLAYOBJ_METHOD(Play)
	return TRUE;
}

BOOL CNPWndProc::Pause()
{
	CALL_PLAYOBJ_METHOD(Pause)
	return FALSE;
}

BOOL CNPWndProc::Stop()
{
	return CloseMedia();
	//CALL_PLAYOBJ_METHOD(Stop)
	//return FALSE;
}

BOOL CNPWndProc::Reverse(short nSpeed)
{
	CALL_PLAYOBJ_METHOD_PARAM(Reverse, nSpeed)
	return FALSE;
}

BOOL CNPWndProc::Forward(short nSpeed)
{
	CALL_PLAYOBJ_METHOD_PARAM(Forward, nSpeed)
	return FALSE;
}

BOOL CNPWndProc::PreFrame()
{
	CALL_PLAYOBJ_METHOD(PreFrame)
	return FALSE;
}

BOOL CNPWndProc::NextFrame()
{
	CALL_PLAYOBJ_METHOD(NextFrame)
	return FALSE;
}

BOOL CNPWndProc::Seek(long pos, long min, long max)
{
	CPlayHelper* pObj = GetSelectPlayObj();
	if(pObj)
	{
		return pObj->Seek(pos, min, max);
	}
	return FALSE;
}

short CNPWndProc::Snap(const char* savePath)
{
	CALL_PLAYOBJ_METHOD_PARAM(Snap, savePath)
	return -1;
}

void CNPWndProc::SetChannels(short nChannels)
{
	if(nChannels <= 0)
		m_nWndCount = 1;
	else
		m_nWndCount = nChannels;
	m_PlayMgr.GetDDrawView().SetWndStyle(m_nWndCount, 0);
	m_PlayMgr.PostMsg(CLEARSCREEN, NULL, NULL);
}

void CNPWndProc::GetPlayPos(long* curPos, long* totalFrames)
{
	CPlayHelper* pObj = GetSelectPlayObj();
	if(pObj)
	{
		pObj->GetPlayPos(curPos, totalFrames);
	}
	else
	{
		*curPos = 0;
		*totalFrames = 0;
	}
}

void CNPWndProc::SetSelectChannel(short nChannel)
{
	if(nChannel >= 1 && nChannel <= m_nWndCount)
	{
		m_nSelectWndId = nChannel;
		GetDDrawView().SetSelectWnd(nChannel);
		GetDDrawView().DrawBorder();
	}
}

void CNPWndProc::Exit()
{
	m_PlayMgr.RemoveAll();
}