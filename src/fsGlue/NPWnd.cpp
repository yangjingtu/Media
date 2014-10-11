#include "stdafx.h"

CNPWnd::CNPWnd() : m_hWnd(NULL), m_hParentWnd(NULL)
{
}

CNPWnd::~CNPWnd(void)
{
}

UINT CNPWnd::GetClassStyle() const
{
	return CS_DBLCLKS;
}

HWND CNPWnd::GetHWND() const 
{ 
	return m_hWnd; 
}

CNPWnd::operator HWND() const
{
	return m_hWnd;
}

HWND CNPWnd::Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HINSTANCE hInstance, HMENU hMenu)
{
	return Create(hwndParent, pstrClassName, pstrName, dwStyle, dwExStyle, hInstance, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND CNPWnd::Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, HINSTANCE hInstance, int x, int y, int cx, int cy, HMENU hMenu)
{
	m_hParentWnd = hwndParent;
	if(!RegisterWindowClass(pstrClassName, hInstance) ) return NULL;
	m_hWnd = ::CreateWindowEx(dwExStyle, pstrClassName, pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, hInstance, this);
	ASSERT(m_hWnd!=NULL);
	m_hInstance = hInstance;
	return m_hWnd;
}

void CNPWnd::Close(UINT nRet)
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	PostMessage(m_hWnd, WM_CLOSE, (WPARAM)nRet, 0L);
}

void CNPWnd::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

bool CNPWnd::RegisterWindowClass(LPCTSTR pstrClassName, HINSTANCE hInstance)
{
	WNDCLASS wc = { 0 };
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CNPWnd::__WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = pstrClassName;
	ATOM ret = ::RegisterClass(&wc);
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LRESULT CALLBACK CNPWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CNPWnd* pWnd = NULL;

	switch (uMsg)
	{

	ON_CASE_CNPWND(WM_PAINT, OnPaint)
	ON_CASE_CNPWND(WM_MOUSEMOVE, OnMouseMove)
	ON_CASE_CNPWND(WM_LBUTTONDBLCLK, OnLButtonDBClick)
	ON_CASE_CNPWND(WM_LBUTTONDOWN, OnLButtonDown)
	ON_CASE_CNPWND(WM_LBUTTONUP, OnLButtonUp)
	ON_CASE_CNPWND(WM_RBUTTONDOWN, OnRButtonDown)
	ON_CASE_CNPWND(WM_RBUTTONUP, OnRButtonUp)
	ON_CASE_CNPWND(WM_KEYDOWN, OnKeyDown)
	ON_CASE_CNPWND(WM_MOUSEWHEEL, OnMouseWheel)
	ON_CASE_CNPWND(WM_SIZE, OnSize)
	ON_CASE_METHOD(WM_DESTROY, OnDestroy)
	
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void CALLBACK CNPWnd::_TimerProc(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{
	CNPWnd* p = (CNPWnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if(p == NULL)
	{
		return;
	}
		
	p->OnTimer(hwnd, message, iTimerId, dwTime);
}

void CNPWnd::ClearBg()
{
	HDC hDC = GetDC(m_hWnd);
	//HBRUSH hBrush = (HBRUSH)GetClassLong(m_hWnd, GCL_HBRBACKGROUND);
	SelectObject(hDC, GetStockBrush(BLACK_BRUSH));

	RECT rt;
	GetClientRect(m_hWnd, &rt);
	Rectangle(hDC, rt.left, rt.top, rt.right, rt.bottom);
}

LRESULT CNPWnd::OnPaint(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnLButtonDBClick(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CNPWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	int w = LOWORD(lParam);
	int h = HIWORD(lParam);
	return S_OK;
}

void CNPWnd::OnTimer(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{

}

void CNPWnd::OnDestroy()
{

}

