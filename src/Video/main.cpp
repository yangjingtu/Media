#include "Video.h"
HANDLE g_hDllModuleV =	NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hDllModuleV = hModule;
		//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);//REALTIME_PRIORITY_CLASS
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		g_hDllModuleV = NULL;
		break;
	}
	return TRUE;
}

CDDraw::CDDraw(void)
: m_height(0)
, m_widht(0)
{
	lpDD = NULL;
	lpDDSBack = NULL;
	lpDDSFront = NULL;
	lpClipper = NULL;
}

CDDraw::~CDDraw(void)
{
	SafeRelease(lpDDSBack);
	SafeRelease(lpDDSFront);
	SafeRelease(lpDD);
}

// 恢复丢失的DDraw对象
HRESULT CDDraw::RestoreDDraw(void)
{
	HRESULT ddrval; 
	if(lpDDSFront)
	{
		ddrval = lpDDSFront->Restore();
		if(ddrval==DD_OK)
		{
			if(lpDDSBack)
			{
				ddrval = lpDDSBack->Restore();
			}
		}
	}
	return ddrval;
}

//********************************************
//功能：清屏
//参数：目标表面，颜色值
void CDDraw::Clrscr(LPDIRECTDRAWSURFACE7 surf,WORD color )
{
	DDBLTFX ddbltfx;
	ddbltfx.dwSize=sizeof(ddbltfx);
	ddbltfx.dwFillColor=color;
	surf->Blt(NULL,NULL,NULL,DDBLT_COLORFILL,&ddbltfx);
}

void CDDraw::DrawRect(DWORD color, LPRECT rt, LPDIRECTDRAWSURFACE7 lpDDSO){
	DDBLTFX ddbltfx; 
	ddbltfx.dwSize = sizeof(ddbltfx); 
	ddbltfx.dwFillColor = color;
	lpDDSO->Blt(rt, NULL, NULL, DDBLT_COLORFILL| DDBLT_WAIT, &ddbltfx); 
}

void CDDraw::DrawRect(DWORD color, int sx, int sy, int ex, int ey, LPDIRECTDRAWSURFACE7 lpDDSO){
	RECT rt;
	rt.left = sx;
	rt.top = sy;
	rt.right = ex;
	rt.bottom = ey;
	DrawRect(color, &rt, lpDDSO);
}

bool CDDraw::InitDDraw7(HWND hWnd, int width, int height)
{
	DWORD dwFlags;
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;//表面结构
	DDSCAPS2             ddscaps;
	ddrval = DirectDrawCreateEx(NULL,(void **)&lpDD,IID_IDirectDraw7,NULL);//创建DD7对象
	if (ddrval != DD_OK)
	{
		return 0;
	}
	//设置协作级别为全屏模式
	dwFlags = DDSCL_NORMAL;;
	lpDD->SetCooperativeLevel(hWnd, dwFlags);	

	// 创建主表面	
	ZeroMemory( &ddsd , sizeof(ddsd) );
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	ddrval=lpDD->CreateSurface( &ddsd, &lpDDSFront, NULL );
	if(ddrval != DD_OK)
	{
		return(false);
	}

	//创建后表面
	ZeroMemory( &ddscaps , sizeof(ddscaps) );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
	if(ddrval != DD_OK)
	{
		return(false);
	}

	//
	m_hWnd = hWnd;
	m_height = height;
	m_widht = width;
	DrawRect(0x00000000, 0, 0, width, height, lpDDSBack);
	return true;
}

LPDIRECTDRAWSURFACE7 CDDraw::CreatEmptySurface(int w , int h)
{
	DDSURFACEDESC2	        ddsd;
	LPDIRECTDRAWSURFACE7   lpDDSO;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; 
	ddsd.dwWidth = w; 
	ddsd.dwHeight = h;

	if (FAILED(lpDD->CreateSurface(&ddsd,&lpDDSO,NULL)))
	{
		return NULL;
	}
	return lpDDSO;
}

LPDIRECTDRAWSURFACE7 CDDraw::CreatOverlaySurface(int w , int h)
{
	DDSURFACEDESC2	        ddsd;
	LPDIRECTDRAWSURFACE7   lpDDSO;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = w; 
	ddsd.dwHeight = h;

	int i = 0;
	HRESULT   hRet;
	do 
	{
		ddsd.ddpfPixelFormat = ddpfOverlayFormats[i];
		hRet = lpDD->CreateSurface(&ddsd, &lpDDSO, NULL);
		
	} while (hRet != DD_OK && (++i < PF_TABLE_SIZE));

	if (hRet != DD_OK)
	{
		return NULL;
	}
	return lpDDSO;
}

void CDDraw::ReleaseDDraw(void)
{
	if (lpDD != NULL)
	{

		if (lpDDSBack != NULL)
		{
			lpDDSBack->Release();
			lpDDSBack = NULL;
		}
		if (lpDDSFront != NULL)
		{
			lpDDSFront->Release();
			lpDDSFront = NULL;
		}

		lpDD->Release();
		lpDD = NULL;
	}
}

bool CDDraw::CreateClipper(HWND hWnd){
	//创建Clipper裁剪器
	lpDD->CreateClipper(0, &lpClipper, NULL);
	lpClipper->SetHWnd(0, hWnd);
	HRESULT ddrval = lpDDSFront->SetClipper(lpClipper);
	if(ddrval != DD_OK)
	{
		return(false);
	}
	return true;
}

int CDDraw::SetDDSPointColor(LPDIRECTDRAWSURFACE7	lpDDSO, DWORD color){
	HRESULT ddrval;
	DDSURFACEDESC2 LockedSurface;
	ZeroMemory(&LockedSurface, sizeof(DDSURFACEDESC2));
	LockedSurface.dwSize   =   sizeof(LockedSurface);
	LPDWORD LockedSurfaceMemory;

	ddrval = lpDDSO->Lock(NULL, &LockedSurface, DDLOCK_WAIT |  DDLOCK_SURFACEMEMORYPTR, NULL);  
	if(ddrval != DD_OK)
	{
		return 1;
	}

	LockedSurfaceMemory = (LPDWORD)LockedSurface.lpSurface;//得到表面指针

	*(LockedSurfaceMemory)=color;
	ddrval = lpDDSO->Unlock(NULL);
	if(ddrval != DD_OK){
		return 2;
	}
	return 0;
}

void CDDraw::DrawText(LPDIRECTDRAWSURFACE7 lpDDSo,int x, int y, LPCTSTR str, COLORREF textcolor, int fontheight, int fontwidth)
{		
	HDC hdc;
	HFONT font;
	font = CreateFont(fontheight, fontwidth, 0, 0, FW_NORMAL, FALSE, FALSE,FALSE,ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Garamond");

	lpDDSo->GetDC(&hdc);
	SelectObject(hdc , font);   
	//SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(0,0,0));
	SetTextColor(hdc, textcolor); 
	TextOut(hdc, x, y, str, strlen(str));
	DeleteObject(font);
	lpDDSo->ReleaseDC(hdc);	
}

LPDIRECTDRAWSURFACE7 CDDraw::MyLoadBitmap(HBITMAP hbm,//位图文件名
										  int W,//宽和高
										  int H)
{
	HDC						hMemDC;
	HDC						hSODC;
	DDSURFACEDESC2	        ddsd;
	LPDIRECTDRAWSURFACE7   lpDDSO;

	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; 
	ddsd.dwWidth = W; 
	ddsd.dwHeight = H;

	if (FAILED(lpDD->CreateSurface(&ddsd,&lpDDSO,NULL)))
	{
		return NULL;
	}

	hMemDC=CreateCompatibleDC(NULL);
	if (hbm == NULL)
	{
		return NULL;
	}
	SelectObject(hMemDC,hbm);

	lpDDSO->GetDC(&hSODC);
	BOOL b = BitBlt(hSODC,0,0,W,H,hMemDC,0,0,SRCCOPY);//把位图复制到离屏页面

	DeleteDC(hMemDC);
	lpDDSO->ReleaseDC(hSODC);

	MySetColorKey(lpDDSO);
	return lpDDSO;
}

void CDDraw::MySetColorKey(LPDIRECTDRAWSURFACE7 lpDDSO)
{
	DDCOLORKEY ddck;
	ddck.dwColorSpaceHighValue = 0;
	ddck.dwColorSpaceLowValue = 0;//设置关键色
	lpDDSO->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

LPDIRECTDRAWSURFACE7 CDDraw::PlayYUV(BYTE* y, BYTE* u, BYTE* v, int iWnd, LPDIRECTDRAWSURFACE7 lpDDSView, RECT SurfaceRt){
	if(iWnd < 0) return NULL;
	if(!lpDDSView) return NULL;
	DDSURFACEDESC2 LockedSurface;
	ZeroMemory(&LockedSurface, sizeof(DDSURFACEDESC2));
	LockedSurface.dwSize   =   sizeof(LockedSurface);

	HRESULT hRet = lpDDSView->Lock(NULL, &LockedSurface, DDLOCK_WAIT, NULL);  
	if (FAILED(hRet))
		return false;

	LPBYTE lpSurf = (LPBYTE)LockedSurface.lpSurface;

	if(lpSurf) {
		int i;
		// fill Y data
		for(i = 0; i < SurfaceRt.bottom; i++)
		{
			memcpy(lpSurf, y, SurfaceRt.right);
			y += SurfaceRt.right;
			lpSurf += LockedSurface.lPitch;
		}

		// fill V data
		for(i = 0; i < SurfaceRt.bottom / 2; i++)
		{
			memcpy(lpSurf, v, SurfaceRt.right / 2);
			v += SurfaceRt.right / 2;
			lpSurf += LockedSurface.lPitch / 2;
		}

		// fill U data
		for(i = 0; i < SurfaceRt.bottom / 2; i++)
		{
			memcpy(lpSurf, u, SurfaceRt.right / 2);
			u += SurfaceRt.right / 2;
			lpSurf += LockedSurface.lPitch / 2;
		}
	}

	lpDDSView->Unlock(NULL);
	return lpDDSView;
}
