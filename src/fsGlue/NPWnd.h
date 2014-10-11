#pragma once

#define ON_CASE_CNPWND(msg, func) \
	case msg:\
	{\
	CNPWnd* p = (CNPWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);\
	if(p == NULL){ break; }\
	p->func(wParam, lParam);\
	break;\
	}

#define ON_CASE_METHOD(msg, func) \
	case msg:\
	{\
	CNPWnd* p = (CNPWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);\
	if(p == NULL){ break; }\
	p->func();\
	break;\
	}

#define SET_TIMER(hwnd, id, elapse) SetTimer(hwnd, id, elapse, CNPWnd::_TimerProc)


class CGlue;

class CNPWnd
{
	friend class CGlue;
public:
	CNPWnd(void);
	virtual ~CNPWnd(void);
	
	HWND GetHWND() const;
	operator HWND() const;

	void ClearBg();

	bool RegisterWindowClass(LPCTSTR pstrClassName, HINSTANCE hInstance);
	virtual HWND Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HINSTANCE hInstance, HMENU hMenu = NULL);
	virtual HWND Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, HINSTANCE hInstance, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	void Close(UINT nRet);
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	virtual void OnDestroy();
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime);

	virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDBClick(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam);

protected:
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual UINT GetClassStyle() const;

	static void CALLBACK _TimerProc(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime);

protected:
	HWND m_hWnd;
	HWND m_hParentWnd;
	HMODULE m_hInstance;

	CGlue*	m_pGlue;
};