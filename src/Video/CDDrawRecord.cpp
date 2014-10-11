#include "Video.h"
#include <math.h>
CDDrawRecord::CDDrawRecord()
: m_iSelectWndID(1)
, m_iWndNum(4)
{
	memset(&m_iSelectRect, 0, sizeof(RECT));
	memset(lpDDSView, NULL, sizeof(LPDIRECTDRAWSURFACE7));
	memset(m_hThread, NULL, sizeof(HANDLE));
	memset(m_iThreadID, 0, sizeof(DWORD));
}

CDDrawRecord::~CDDrawRecord()
{
	for (int i=0; i<4; i++)
	{
		PostThreadMessage(m_iThreadID[i], WM_QUIT, NULL, NULL);
		if (WaitForSingleObject(m_hThread[i], 300) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThread[i], 0);
		}
		CloseHandle(m_hThread[i]);
	}

	for (int i=0; i<4; i++)
	{
		SafeRelease(lpDDSView[i]);
	}
}

void CDDrawRecord::DDsfDrawToDDsf(LPDIRECTDRAWSURFACE7 lpDDsfd, RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt)
{
	HRESULT hr = lpDDsfd->Blt(&dst, lpDDSfr, &rt, DDBLT_WAIT, NULL);
}

void CDDrawRecord::DDsfDrawToDDBack(RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt)
{
	DDsfDrawToDDsf(lpDDSBack, dst, lpDDSfr, rt);
}

void CDDrawRecord::DrawBorder()
{
	HDC hDC;
	RECT rt = {0};
	int bs = sqrt((double)m_iWndNum);
	lpDDSBack->GetDC(&hDC);
	for (int i=0; i<=2; i++)
	{
		rt.top = m_height*i/bs;
		rt.left = 0;
		rt.bottom = m_height*i/bs;
		rt.right = m_widht;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);

		rt.top = 0;
		rt.left = m_widht*i/bs;
		rt.bottom = m_height;
		rt.right = m_widht*i/bs;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
	}
	if (m_iSelectWndID != 0)
	{
		int x = (m_iSelectWndID - 1) / bs;
		int y = (m_iSelectWndID - 1) % bs;
		if (x >= 0 && y >= 0)
		{
			rt.left = y * m_widht / bs - 1;
			rt.right = (y + 1) * m_widht / bs + 1;
			rt.top = x * m_height / bs - 1;
			rt.bottom = (x + 1) * m_height / bs + 1;
			CGdiEngine::DrawRect(hDC, rt, 2, 0xFFFF0000);
			m_iSelectRect = rt;
		}	
	}
	lpDDSBack->ReleaseDC(hDC);
}

void CDDrawRecord::DrawSelectBorder(POINT pt)
{
	RECT rt = {0};
	GetWindowRect(m_hWnd, &rt);
	int bs = sqrt((double)m_iWndNum);
	int WndWidth = (rt.right - rt.left) / bs;
	int WndHeight = (rt.bottom - rt.top) / bs;
	pt.x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
	pt.y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

	rt.left = m_widht * (pt.x - 1) / bs - 1;
	rt.right = m_widht * pt.x / bs + 1;
	rt.top = m_height * (pt.y - 1) / bs - 1;
	rt.bottom = m_height * pt.y / bs + 1;

	m_iSelectWndID = (pt.y-1) * bs + pt.x;

	HDC hDC;
	lpDDSBack->GetDC(&hDC);
	CGdiEngine::DrawRect(hDC, m_iSelectRect, 2, 0xFFFFFFFF);
	CGdiEngine::DrawRect(hDC, rt, 2, 0xFFFF0000);
	lpDDSBack->ReleaseDC(hDC);
	m_iSelectRect = rt;
}

void CDDrawRecord::Flip()
{
	RECT dst;
	GetWindowRect(m_hWnd, &dst);
	HRESULT hr = lpDDSBack->Blt(NULL, lpDDSBack,NULL, DDBLT_WAIT, NULL);
	hr = lpDDSFront->Blt(&dst, lpDDSBack, NULL, DDBLT_WAIT, NULL);
	if(hr != DD_OK) {
		if (hr == DDERR_SURFACELOST){
			lpDD->RestoreAllSurfaces();
		}
	}
}

LPDIRECTDRAWSURFACE7 CDDrawRecord::PlayYUV(BYTE* y, BYTE* u, BYTE* v, int iWnd, LPDIRECTDRAWSURFACE7 lpDDSView, RECT SurfaceRt, RECT& dst)
{
	GetWndRect(iWnd, dst);
	return CDDraw::PlayYUV(y, u, v, iWnd, lpDDSView, SurfaceRt);
}

void CDDrawRecord::GetWndRect(int iWnd, RECT& dst)
{
	iWnd --;
	int bs = sqrt((double)m_iWndNum);
	int x = iWnd % bs;
	int y = iWnd / bs;

	dst.left = m_widht * x / bs + 2;
	dst.right = m_widht * (x + 1) / bs - 2;
	dst.top = m_height * y / bs + 2;
	dst.bottom = m_height * (y + 1) / bs - 2;
}

void CDDrawRecord::ClearScreen(int iWnd, DWORD color)
{
	RECT dst;
	GetWndRect(iWnd, dst);
	DrawRect(color, &dst, lpDDSBack);
}

void CDDrawRecord::ClearScreen(DWORD color)
{
	DrawRect(color, NULL, lpDDSBack);
}

int  CDDrawRecord::GetSelectWndID()
{
	return m_iSelectWndID;
}