#pragma once
#include "NPWndProc.h"
#include "gluelib.h"

#define MAXCHANNEL	1

//是否有多箇窗口， 主要由MAXCHANNEL決定
#define MULIT_WND  (MAXCHANNEL > 1 ? 1 : 0)

#define NPMSG_PARAM	\
	CNPWnd* pWnd, WPARAM wParam, LPARAM lParam

#define DECLARE_ONMSG(func) \
	LRESULT func(NPMSG_PARAM);

class CGlue
{
public:
	CGlue(void);
	~CGlue(void);

public:
	void RegisterPlugin(CreateVarRiant vrcreate, DeleteVarRiant vrdel, Int32ToVarRiant cvint, StringToVarRiant cvstring, InvokeJavaScriptFunc func, void* data);
	void OnCreate(HWND hParent, HINSTANCE hInstance);
	void OnSize(int w, int h);
	void OnDestroy();

	void SetPropertyOfStr(char* name, char* value);
	void SetPropertyOfLong(char* name, long value);

	void GetNextPage();
	void GetPrevPage();
	void EnterOrExitFullScreen();
	void EnterOrExitFullScreenForSigChanl(HWND hWnd);
	void SetSelectedChanl(CNPWndProc* pWnd);
	CNPWndProc GetSelectedChanl();

	bool OpenMedia(const char* mediaPath, short nType);
	bool CloseMedia();
	bool Play();
	bool Pause();
	bool Stop();
	bool Forward(short nSpeed);
	bool Reverse(short nSpeed);
	void PreFrame();
	void NextFrame();
	bool Seek(long pos, long minVal, long maxVal);
	short Snap(const char* savePath);
	short SetSelectWnd(short nWndIndex);
	short SetWndChannels(short nWndId, short nChannels);
	void Exit();
	void GetPlayPos(long* curPos, long* totalFrames);

	short SetWndChannelSelect(short nWndIndex, short nChannel);
	short GetWndChannelSelect(short nWndIndex);
	short GetWndChannelCount(short nWndIndex);


	bool IsParentFullScreen() const { return m_bIsParentFullScreen; }
	bool IsChildFullScreen() const { return m_bIsChildFullScreen; }
	bool IsPolling() const { return m_bIsPolling; }
	void DrawBorderAll(CNPWndProc* pWnd);

	HPEN GetBoderPen() const { return m_hBorderPen; }
	HPEN GetBoderPen2() const { return m_hBorderPen2; }

protected://消息响应
	DECLARE_ONMSG(OnLButtonDBClick)
	DECLARE_ONMSG(OnLButtonDown)
	DECLARE_ONMSG(OnLButtonUp)
	DECLARE_ONMSG(OnRButtonDown)
	DECLARE_ONMSG(OnRButtonUp)
	DECLARE_ONMSG(OnMouseMove)
	DECLARE_ONMSG(OnMouseWheel)
	DECLARE_ONMSG(OnKeyDown)
	DECLARE_ONMSG(OnPaint)
	DECLARE_ONMSG(OnSize)

protected:
	static LRESULT CALLBACK NPWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void DrawBorderSingle(CNPWndProc* pWnd);

	void FullScreen();

private:
	CNPWndProc		  m_VideoWnd[MAXCHANNEL];	//控件最大支持9路设备
	int				  m_iMaxWnd;		//当前设备最大通道支持能力,m_iMaxWnd <= 9;取值 1,4,9
	short			  m_iCurMaxWnd;		//当前通道状态;取值 1,4,6,8,9
	short			  m_iCurSelChannel;	//当前选中通道
	short			  m_iWndStyle;		//当前Wnd表现形式
	short			  m_iFstChannel;    //每个多路窗口的第一个窗口序号
	HWND			  m_hParentWnd;
	HWND			  m_hSuperWnd;

	HINSTANCE		  m_hInstance;
	HPEN			  m_hBorderPen;
	HPEN			  m_hBorderPen2;
	BOOL			  m_bActScrProtection;
	bool			  m_bIsParentFullScreen;
	bool			  m_bIsChildFullScreen;
	bool			  m_bIsPolling;
	RECT			  m_rcWnd;
	RECT			  m_rcParentWnd;
	RECT			  m_rcChildWnd;
	RECT			  m_rcParentWndPos;
	RECT			  m_rcChildWndPos;

	CreateVarRiant			m_pVrCreate;
	DeleteVarRiant			m_pVrDelete;
	Int32ToVarRiant			m_pCvInt;
	StringToVarRiant		m_pCvString;
	InvokeJavaScriptFunc	m_jsCall;
	void*					m_pUserData;

	short			 m_nSelectWnd;
};
