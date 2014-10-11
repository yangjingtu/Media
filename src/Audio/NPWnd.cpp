#include "NPWnd.h"

CAWnd::CAWnd() : m_hWnd(NULL)
{
}

CAWnd::~CAWnd(void)
{
}

UINT CAWnd::GetClassStyle() const
{
	return 0;
}

HWND CAWnd::GetHWND() const 
{ 
	return m_hWnd; 
}

CAWnd::operator HWND() const
{
	return m_hWnd;
}

HWND CAWnd::Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HINSTANCE hInstance, HMENU hMenu)
{
	return Create(hwndParent, pstrClassName, pstrName, dwStyle, dwExStyle, hInstance, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND CAWnd::Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, HINSTANCE hInstance, int x, int y, int cx, int cy, HMENU hMenu)
{
	if(!RegisterWindowClass(pstrClassName, hInstance) ) return NULL;
	m_hWnd = ::CreateWindowEx(dwExStyle, pstrClassName, pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, hInstance, this);
	assert(m_hWnd!=NULL);
	return m_hWnd;
}

void CAWnd::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
	assert(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

bool CAWnd::RegisterWindowClass(LPCTSTR pstrClassName, HINSTANCE hInstance)
{
	WNDCLASS wc = { 0 };
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CAWnd::__WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = pstrClassName;
	ATOM ret = ::RegisterClass(&wc);
	assert(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LRESULT CALLBACK CAWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}