#pragma once
#include "stdafx.h"
class CAWnd
{
public:
	CAWnd(void);
	~CAWnd(void);
	
	HWND GetHWND() const;
	operator HWND() const;

	bool RegisterWindowClass(LPCTSTR pstrClassName, HINSTANCE hInstance);
	HWND Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HINSTANCE hInstance, HMENU hMenu = NULL);
	HWND Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, HINSTANCE hInstance, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);

	void ShowWindow(bool bShow = true, bool bTakeFocus = true);
protected:
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual UINT GetClassStyle() const;
public:
	HWND m_hWnd;
};