#include "Video.h"
#include <math.h>
CDDrawView::CDDrawView(void)
: m_iWndNum(0)
, m_iWndStyle(0)
, m_iSelectWndID(1)
, m_iPage(0)
{
	memset(&m_iSelectRect, 0, sizeof(RECT));
}

CDDrawView::~CDDrawView(void)
{
	
}

void CDDrawView::GetWndRect(int iWnd, RECT& dst)
{
	iWnd --;
	if(m_iWndStyle == 0){	//规则窗口
		if (m_iWndNum == 1)
		{
			if (m_iSelectWndID == iWnd+m_iPage+1)
			{
				dst.left =  2;
				dst.right = m_widht - 2;
				dst.top =  2;
				dst.bottom = m_height - 2;
			}
		}else{
			int bs = sqrt((double)m_iWndNum);
			int x = iWnd % bs;
			int y = iWnd / bs;

			dst.left = m_widht * x / bs + 2;
			dst.right = m_widht * (x + 1) / bs - 2;
			dst.top = m_height * y / bs + 2;
			dst.bottom = m_height * (y + 1) / bs - 2;
		}
	}else if(m_iWndStyle == 1){
		int bs = m_iWndNum / 2;
		int bsd = bs - 1;
		int x = iWnd % bs;
		int y = iWnd / bs;
		if( y == 0 ){
			if (iWnd == 0)
			{
				dst.left = 2;
				dst.right = m_widht * bsd / bs - 2;
				dst.top = 2;
				dst.bottom = m_height * bsd / bs - 2;
			}else{
				y = x - 1;
				x = bs - 1;
				dst.left = m_widht * x / bs + 2;
				dst.right = m_widht * (x + 1) / bs - 2;
				dst.top = m_height * y / bs + 2;
				dst.bottom = m_height * (y + 1) / bs - 2;							
			}
		}else{
			y  = bs - 1;
			dst.left = m_widht * x / bs + 2;
			dst.right = m_widht * (x + 1) / bs - 2;
			dst.top = m_height * y / bs + 2;
			dst.bottom = m_height * (y + 1) / bs - 2;
		}
	}else if (m_iWndStyle == 2)
	{
		iWnd -= 3;
		int wndNum = m_iWndNum - 3;
		if (iWnd > 0)
		{			
			int bs = wndNum / 2;
			int x = iWnd % bs;
			int y = iWnd / bs;
			if( y == 0 ){
				if (iWnd == 0)
				{
					dst.left = 2;
					dst.right = m_widht * 2 / bs - 2;
					dst.top = 2;
					dst.bottom = m_height * 2 / bs - 2;
				}else{
					y = x - 1;
					x = bs - 1;
					dst.left = m_widht * x / bs + 2;
					dst.right = m_widht * (x + 1) / bs - 2;
					dst.top = m_height * y / bs + 2;
					dst.bottom = m_height * (y + 1) / bs - 2;							
				}
			}else{
				y  = bs - 1;
				dst.left = m_widht * x / bs + 2;
				dst.right = m_widht * (x + 1) / bs - 2;
				dst.top = m_height * y / bs + 2;
				dst.bottom = m_height * (y + 1) / bs - 2;
			}
		}else{
			{
				iWnd += 3;
				int bs = wndNum / 2;
				int bsd = bs - 1;
				int x = iWnd % 2;
				int y = iWnd / 2;

				int width = m_widht*bsd / bs;
				int height = m_height*bsd / bs;
				
				dst.left = width * x / 2 + 2;
				dst.right = width * (x + 1) / 2 - 2;
				dst.top = height * y / 2 + 2;
				dst.bottom = height * (y + 1) / 2 - 2;					
			}
		}
	}
}

LPDIRECTDRAWSURFACE7 CDDrawView::PlayYUV(BYTE* y, BYTE* u, BYTE* v, int iWnd, LPDIRECTDRAWSURFACE7 lpDDSView, RECT SurfaceRt, RECT& dst)
{
	iWnd -= m_iPage*m_iWndNum;
	if (iWnd <= 0 || iWnd > m_iWndNum)
	{
		return NULL;
	}
	GetWndRect(iWnd, dst);
	return CDDraw::PlayYUV(y, u, v, iWnd, lpDDSView, SurfaceRt);
}

void CDDrawView::SetWndStyle(int num, int style)
{
	assert(style>=0 && style<=5);
	m_iWndNum = num;
	m_iWndStyle = style;
}

void CDDrawView::GetWndStyle(int &num, int &style)
{
	num = m_iWndNum;
	style = m_iWndStyle;
}

void CDDrawView::DrawBorder()
{
	HDC hDC;
	lpDDSBack->GetDC(&hDC);
	if(hDC == NULL)
		return;

	int num = m_iWndNum;
	int style = m_iWndStyle;
	RECT rt = {0};

	if (style == 0)
	{
		int wndNum = sqrt((double)num);
		for (int i=0; i<=wndNum; i++)
		{
			rt.top = m_height*i/wndNum;
			rt.left = 0;
			rt.bottom = m_height*i/wndNum;
			rt.right = m_widht;
			CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);

			rt.top = 0;
			rt.left = m_widht*i/wndNum;
			rt.bottom = m_height;
			rt.right = m_widht*i/wndNum;
			CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		}
		if (m_iSelectWndID != 0)
		{
			int x = (m_iSelectWndID - 1 - m_iPage * m_iWndNum) / wndNum;
			int y = (m_iSelectWndID - 1 - m_iPage * m_iWndNum) % wndNum;
			if (x >= 0 && y >= 0)
			{
				rt.left = y * m_widht / wndNum - 1;
				rt.right = (y + 1) * m_widht / wndNum + 1;
				rt.top = x * m_height / wndNum - 1;
				rt.bottom = (x + 1) * m_height / wndNum + 1;
				if (m_iPage * m_iWndNum <= m_iSelectWndID && (m_iPage + 1) * m_iWndNum >= m_iSelectWndID )
				{
					CGdiEngine::DrawRect(hDC, rt, 2, 0xFFFF0000);
					m_iSelectRect = rt;
				}
			}	
		}
	}else if (style == 1)
	{
		assert(num % 2 == 0);
		int wndNum = num / 2;
		int bs = wndNum - 1;
		rt.bottom = m_height;
		rt.top = rt.left = 0;
		rt.right = 1;
		//竖线
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.right = rt.left = m_widht*bs/wndNum;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.right = rt.left = m_widht;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = m_height*bs/wndNum;
		for(int i=1; i<bs; i++){
			rt.left = rt.right = m_widht*i/wndNum;
			CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		}

		//横线
		rt.top = rt.bottom = rt.left = 0;
		rt.right = m_widht;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = rt.bottom = m_height*bs/wndNum;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = rt.bottom = m_height;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.left = m_widht*bs/wndNum;

		for(int i=1; i<bs; i++){
			rt.top = rt.bottom = m_height*i/wndNum;
			CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		}
	}else if (style == 2)
	{
		num -= 3;
		assert(num % 2 == 0);
		int wndNum = num / 2;
		int bs = wndNum - 1;
		int bsd = bs / 2;
		rt.bottom = m_height;
		rt.top = rt.left = 0;
		rt.right = 1;

		//竖线
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.right = rt.left = m_widht*bs/wndNum;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.right = rt.left = m_widht;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = m_height*bs/wndNum;
		int ibs = bs / 2 * 2;
		for(int i=1; i<bs; i++){
				rt.left = rt.right = m_widht*i/wndNum;
				CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);			
		}

		//横线
		rt.top = rt.bottom = rt.left = 0;
		rt.right = m_widht;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = rt.bottom = m_height*bs/wndNum;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = rt.bottom = m_height;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.left = m_widht*bs/wndNum;

		for(int i=1; i<bs; i++){
				rt.top = rt.bottom = m_height*i/wndNum;
				CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		}

		int width = m_widht*bs / wndNum;
		int height = m_height*bs / wndNum;
		rt.left = rt.right = width / 2;
		rt.top = 0;
		rt.bottom = height ;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
		rt.top = rt.bottom = height / 2;
		rt.left = 0;
		rt.right = width;
		CGdiEngine::DrawLine(hDC, rt, 2, 0xFFFFFFFF);
	}	
	lpDDSBack->ReleaseDC(hDC);
}

void CDDrawView::Flip()
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

void CDDrawView::DrawSelectBorder(int wndid)
{
	HDC hDC;
	lpDDSBack->GetDC(&hDC);
	if(hDC == NULL)
		return;

	RECT rt = {0};
	wndid--;
	GetWindowRect(m_hWnd, &rt);
	if(m_iWndStyle == 0){	//规则窗口
		int bs = sqrt((double)m_iWndNum);
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		int x = wndid % bs;
		int y = wndid / bs;

		rt.left = m_widht * x / bs - 1;
		rt.right = m_widht * (x+1) / bs + 1;
		rt.top = m_height * y / bs - 1;
		rt.bottom = m_height * (y+1) / bs + 1;

		m_iSelectWndID = wndid+1;
	}else if(m_iWndStyle == 1)	//不规则窗口
	{
		int bs = m_iWndNum / 2 + m_iWndNum % 2;
		int bsd = bs - 1;
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;

		if (wndid == 0)
		{
			rt.left = 0;
			rt.right = m_widht * bsd / bs + 1;
			rt.top = 0 - 1;
			rt.bottom = m_height * bsd / bs + 1;
			m_iSelectWndID = 1 + m_iPage * m_iWndNum;
		}else if(wndid < bs){
			rt.left = m_widht * bsd / bs -1;
			rt.right = m_widht + 1;
			rt.top = m_height * (wndid-1) / bs;
			rt.bottom = m_height * wndid / bs + 1;
			m_iSelectWndID = wndid+1;
		}else{
			int x = wndid - bs;
			rt.left = m_widht * x / bs -1;
			rt.right = m_widht * (x+1) / bs + 1;
			rt.top = m_height * bsd / bs - 1;
			rt.bottom = m_height + 1;
			m_iSelectWndID = wndid+1;
		}
	}else if (m_iWndStyle == 2)
	{
// 		int iWndNum = m_iWndNum - 3;
// 		int bs = iWndNum / 2 + iWndNum % 2;
// 		int bsd = bs - 1;
// 		int WndWidth = (rt.right - rt.left) / bs;
// 		int WndHeight = (rt.bottom - rt.top) / bs;
// 		int x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
// 		int y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);
// 
// 		if ((x >= 1 && x <= bsd && y != bs) | (y >= 1 && y <= bsd && x != bs))
// 		{
// 
// 			WndWidth = (rt.right - rt.left) * bsd / bs / 2;
// 			WndHeight = (rt.bottom - rt.top) * bsd / bs / 2;
// 
// 			x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
// 			y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);
// 
// 			WndWidth = m_widht * bsd / bs / 2;
// 			WndHeight = m_height * bsd / bs / 2;
// 
// 			rt.bottom = WndHeight * y + 1;
// 			rt.left = WndWidth * (x - 1);
// 			rt.right = WndWidth * x + 1;
// 			rt.top = WndHeight * (y - 1) - 1;
// 
// 			m_iSelectWndID = (y-1) * 2 + x + m_iPage * m_iWndNum;
// 		}else {
// 			rt.right = m_widht * x / bs + 1;
// 			rt.top = m_height * (y - 1) / bs - 1;
// 			rt.bottom = m_height * y / bs + 1;
// 			if (pt.x == 1)
// 			{
// 				rt.left = m_widht * (x - 1) / bs;
// 			}else{
// 				rt.left = m_widht * (x - 1) / bs - 1;
// 			}
// 
// 			if (y == bs)
// 			{
// 				m_iSelectWndID = bs + 3 + x + m_iPage * m_iWndNum;
// 			}else{
// 				m_iSelectWndID = y + 4 + m_iPage * m_iWndNum;
// 			}
// 		}
	}

	CGdiEngine::DrawRect(hDC, m_iSelectRect, 2, 0xFFFFFFFF);
	CGdiEngine::DrawRect(hDC, rt, 2, 0xFFFF0000);
	lpDDSBack->ReleaseDC(hDC);
	m_iSelectRect = rt;
}

void CDDrawView::DrawSelectBorder(POINT pt)
{
	HDC hDC;
	lpDDSBack->GetDC(&hDC);
	if(hDC == NULL)
		return;

	RECT rt = {0};
	GetWindowRect(m_hWnd, &rt);
	if(m_iWndStyle == 0){	//规则窗口
		int bs = sqrt((double)m_iWndNum);
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		pt.x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
		pt.y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

		rt.left = m_widht * (pt.x - 1) / bs - 1;
		rt.right = m_widht * pt.x / bs + 1;
		rt.top = m_height * (pt.y - 1) / bs - 1;
		rt.bottom = m_height * pt.y / bs + 1;

		m_iSelectWndID = (pt.y-1) * bs + pt.x + m_iPage * m_iWndNum;
	}else if(m_iWndStyle == 1)	//不规则窗口
	{
		int bs = m_iWndNum / 2 + m_iWndNum % 2;
		int bsd = bs - 1;
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		pt.x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
		pt.y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);
	
		if ((pt.x >= 1 && pt.x <= bsd && pt.y != bs) | (pt.y >= 1 && pt.y <= bsd && pt.x != bs))
		{
			rt.left = 0;
			rt.right = m_widht * bsd / bs + 1;
			rt.top = 0;
			rt.bottom = m_height * bsd / bs + 1;
			m_iSelectWndID = 1 + m_iPage * m_iWndNum;
		}else {
			rt.right = m_widht * pt.x / bs + 1;
			rt.top = m_height * (pt.y - 1) / bs;
			rt.bottom = m_height * pt.y / bs + 1;
			if (pt.x == 1)
			{
				rt.left = m_widht * (pt.x - 1) / bs;
			}else{
				rt.left = m_widht * (pt.x - 1) / bs - 1;
			}

			if (pt.y == bs)
			{
				m_iSelectWndID = bs + pt.x + m_iPage * m_iWndNum;
			}else{
				m_iSelectWndID = pt.y + 1 + m_iPage * m_iWndNum;
			}
		}		
	}else if (m_iWndStyle == 2)
	{
		int iWndNum = m_iWndNum - 3;
		int bs = iWndNum / 2 + iWndNum % 2;
		int bsd = bs - 1;
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		int x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
		int y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

		if ((x >= 1 && x <= bsd && y != bs) | (y >= 1 && y <= bsd && x != bs))
		{
			
			WndWidth = (rt.right - rt.left) * bsd / bs / 2;
			WndHeight = (rt.bottom - rt.top) * bsd / bs / 2;

			x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
			y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

			WndWidth = m_widht * bsd / bs / 2;
			WndHeight = m_height * bsd / bs / 2;

			rt.bottom = WndHeight * y + 1;
			rt.left = WndWidth * (x - 1);
			rt.right = WndWidth * x + 1;
			rt.top = WndHeight * (y - 1) - 1;

			m_iSelectWndID = (y-1) * 2 + x + m_iPage * m_iWndNum;
		}else {
			rt.right = m_widht * x / bs + 1;
			rt.top = m_height * (y - 1) / bs - 1;
			rt.bottom = m_height * y / bs + 1;
			if (pt.x == 1)
			{
				rt.left = m_widht * (x - 1) / bs;
			}else{
				rt.left = m_widht * (x - 1) / bs - 1;
			}

			if (y == bs)
			{
				m_iSelectWndID = bs + 3 + x + m_iPage * m_iWndNum;
			}else{
				m_iSelectWndID = y + 4 + m_iPage * m_iWndNum;
			}
		}
	}

	CGdiEngine::DrawRect(hDC, m_iSelectRect, 2, 0xFFFFFFFF);
	CGdiEngine::DrawRect(hDC, rt, 2, 0xFFFF0000);
	lpDDSBack->ReleaseDC(hDC);
	m_iSelectRect = rt;
}

int CDDrawView::GetWndID(POINT pt)
{
	RECT rt = {0};
	GetWindowRect(m_hWnd, &rt);
	if(m_iWndStyle == 0){	//规则窗口
		int bs = sqrt((double)m_iWndNum);
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		pt.x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
		pt.y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);
		return (pt.y-1) * bs + pt.x + m_iPage * m_iWndNum;
	}else if(m_iWndStyle == 1)	//不规则窗口
	{
		int bs = m_iWndNum / 2 + m_iWndNum % 2;
		int bsd = bs - 1;
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		pt.x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
		pt.y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

		if ((pt.x >= 1 && pt.x <= bsd && pt.y != bs) | (pt.y >= 1 && pt.y <= bsd && pt.x != bs))
		{
			return 1;
		}else {
			if (pt.y == bs)
			{
				return bs + pt.x + m_iPage * m_iWndNum;
			}else{
				return pt.y + 1 + m_iPage * m_iWndNum;
			}
		}		
	}else if (m_iWndStyle == 2)
	{
		int iWndNum = m_iWndNum - 3;
		int bs = iWndNum / 2 + iWndNum % 2;
		int bsd = bs - 1;
		int WndWidth = (rt.right - rt.left) / bs;
		int WndHeight = (rt.bottom - rt.top) / bs;
		int x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
		int y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

		if ((x >= 1 && x <= bsd && y != bs) | (y >= 1 && y <= bsd && x != bs))
		{

			WndWidth = (rt.right - rt.left) * bsd / bs / 2;
			WndHeight = (rt.bottom - rt.top) * bsd / bs / 2;

			x = pt.x / WndWidth + (pt.x % WndWidth == 0 ? 0 : 1);
			y = pt.y / WndHeight + (pt.y % WndHeight == 0 ? 0 : 1);

			return (y-1) * 2 + x;
		}else {
			if (y == bs)
			{
				return bs + 3 + x + m_iPage * m_iWndNum;
			}else{
				return y + 4 + m_iPage * m_iWndNum;
			}
		}
	}
}

int CDDrawView::GetSelectWndID()
{
	return m_iSelectWndID;
}

void CDDrawView::SetSelectWnd(int nWndId)
{
	if(nWndId < 1 || nWndId > m_iWndNum)
		return;

	m_iSelectWndID = nWndId;
}

void CDDrawView::DDsfDrawToDDsf(LPDIRECTDRAWSURFACE7 lpDDsfd, RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt)
{
	HRESULT hr = lpDDsfd->Blt(&dst, lpDDSfr, &rt, DDBLT_WAIT, NULL);
}

void CDDrawView::DDsfDrawToDDBack(RECT& dst, LPDIRECTDRAWSURFACE7 lpDDSfr, RECT& rt)
{
	DDsfDrawToDDsf(lpDDSBack, dst, lpDDSfr, rt);
}

void CDDrawView::ClearScreen(int iWnd, DWORD color)
{
	RECT dst;
	assert(iWnd>0);
	GetWndRect(iWnd, dst);
	DrawRect(color, &dst, lpDDSBack);
}

void CDDrawView::ClearScreen(DWORD color)
{
	DrawRect(color, NULL, lpDDSBack);
}

void CDDrawView::ShowPage(int iPage)
{
	m_iPage = iPage;
}

int CDDrawView::GetNowPage()
{
	return m_iPage;
}

void CDDrawView::ReSetSCRect()
{
	memset(&m_iSelectRect, 0, sizeof(RECT));
}