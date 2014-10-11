#pragma once
#include <Windows.h>
#include <ddraw.h>
#include <assert.h>
#pragma comment(lib, "../../lib/ddraw/ddraw.lib")
#pragma comment(lib, "../../lib/ddraw/dxguid.lib")

#define _EXPORTING

#ifdef	_EXPORTING
#define VIDEO_SDK  __declspec(dllexport)
#else
#define VIDEO_SDK __declspec(dllimport)
#endif

#define SafeRelease(pobj) if(pobj){pobj->Release();pobj=NULL;}
static DDPIXELFORMAT ddpfOverlayFormats[] = {
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','V','1','2'),0,0,0,0,0},  // YV12
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('U','Y','V','Y'),0,0,0,0,0},  // UYVY    
};
#define PF_TABLE_SIZE (sizeof(ddpfOverlayFormats) / sizeof(ddpfOverlayFormats[0]))

class CGdiEngine
{
public:
	CGdiEngine(void);
	~CGdiEngine(void);

public:
	static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
	static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
	static void DrawRoundRect(HDC hDC, const RECT& rc, int nSize, int width, int height, DWORD dwPenColor);
};

class VIDEO_SDK CDDraw
{
public:
	CDDraw(void);
	virtual ~CDDraw(void);
	virtual bool InitDDraw7(HWND hWnd, int width, int height);
	virtual LPDIRECTDRAWSURFACE7 CreatEmptySurface(int w , int h);
	virtual LPDIRECTDRAWSURFACE7 CreatOverlaySurface(int w , int h);
	virtual void ReleaseDDraw(void);
	virtual HRESULT RestoreDDraw(void);
	virtual int SetDDSPointColor(LPDIRECTDRAWSURFACE7	lpDDSO, DWORD color);
	virtual bool CreateClipper(HWND hWnd);
	virtual void DrawText(LPDIRECTDRAWSURFACE7 lpDDSo, int x, int y, LPCTSTR str, COLORREF textcolor = RGB(255, 255, 255), int fontheight = 16, int fontwidth = 8);
	virtual void Clrscr(LPDIRECTDRAWSURFACE7,WORD=0);
	virtual void DrawRect(DWORD color, LPRECT rt, LPDIRECTDRAWSURFACE7 lpDDSO);
	virtual void DrawRect(DWORD color, int sx, int sy, int ex, int ey, LPDIRECTDRAWSURFACE7 lpDDSO);
	virtual LPDIRECTDRAWSURFACE7 MyLoadBitmap(HBITMAP hbm, int W,	int H);
	virtual void MySetColorKey(LPDIRECTDRAWSURFACE7 lpDDSO);
	
	virtual LPDIRECTDRAWSURFACE7 PlayYUV(BYTE* y, BYTE* u, BYTE* v, int iWnd, LPDIRECTDRAWSURFACE7 lpDDSView, RECT SurfaceRt);
	virtual void Flip() = 0;
public:
	HWND m_hWnd;
	int m_height;
	int m_widht;
	LPDIRECTDRAW7       lpDD;
	LPDIRECTDRAWSURFACE7	lpDDSFront;
	LPDIRECTDRAWSURFACE7    lpDDSBack;
	LPDIRECTDRAWCLIPPER		 lpClipper;
};

class VIDEO_SDK CDDrawView :
	public CDDraw
{
public:
	CDDrawView(void);
	~CDDrawView(void);
	LPDIRECTDRAWSURFACE7 PlayYUV(BYTE* y, BYTE* u, BYTE* v, int iWnd, LPDIRECTDRAWSURFACE7 lpDDSView, RECT SurfaceRt, RECT& dst);

public:
	void DDsfDrawToDDsf(LPDIRECTDRAWSURFACE7 lpDDsfd, RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt);
	void DDsfDrawToDDBack(RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt);
	void SetWndStyle(int num, int style);
	void GetWndStyle(int &num, int &style);
	void DrawBorder();
	void DrawSelectBorder(POINT pt);
	void DrawSelectBorder(int wndid);
	void GetWndRect(int iWnd, RECT& dst);
	int  GetSelectWndID();
	void SetSelectWnd(int nWndId);
	int  GetWndID(POINT pt);
	void ClearScreen(int iWnd, DWORD color = 0x0);
	void ClearScreen(DWORD color = 0x0);
	void ShowPage(int iPage);
	int  GetNowPage();
	void ReSetSCRect();
	void Flip();

protected:
	

private:
	int	m_iWndNum;
	int m_iWndStyle;
	int m_iSelectWndID;	//从1开始计数
	RECT m_iSelectRect;
	int	m_iPage;		//从0开始计数
};

class VIDEO_SDK CDDrawRecord :
	public CDDraw
{
public:
	CDDrawRecord(void);
	~CDDrawRecord(void);
	LPDIRECTDRAWSURFACE7 PlayYUV(BYTE* y, BYTE* u, BYTE* v, int iWnd, LPDIRECTDRAWSURFACE7 lpDDSView, RECT SurfaceRt, RECT& dst);

public:
	void DDsfDrawToDDsf(LPDIRECTDRAWSURFACE7 lpDDsfd, RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt);
	void DDsfDrawToDDBack(RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt);
	void GetWndRect(int iWnd, RECT& dst);
	void DrawBorder();
	void DrawSelectBorder(POINT pt);
	int  GetSelectWndID();

	void Flip();
	void ClearScreen(int iWnd, DWORD color = 0x0);
	void ClearScreen(DWORD color = 0x0);

protected:


private:
	LPDIRECTDRAWSURFACE7    lpDDSView[4];
	HANDLE					m_hThread[4];
	DWORD					m_iThreadID[4];
	int						m_iSelectWndID;
	int						m_iWndNum;
	RECT					m_iSelectRect;
	
};

