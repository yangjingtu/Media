#pragma once
#include "stdafx.h"
// fsNVRPluginCtrl.h : CfsNVRPluginCtrl ActiveX 控件类的声明。


// CfsNVRPluginCtrl : 有关实现的信息，请参阅 fsNVRPluginCtrl.cpp。

#define MAXCHANNEL	9
#define TIMERPOLLING  1

class CfsNVRPluginCtrl : public COleControl
{
	DECLARE_DYNCREATE(CfsNVRPluginCtrl)

// 构造函数
public:
	CfsNVRPluginCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CfsNVRPluginCtrl();

	DECLARE_OLECREATE_EX(CfsNVRPluginCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CfsNVRPluginCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CfsNVRPluginCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CfsNVRPluginCtrl)		// 类型名称和杂项状态

	LRESULT OnEventCallBack(WPARAM wParam, LPARAM lParam);

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		eventidOnTwoWndSwap = 1L,
		dispidGetWndChannelCount = 25L,
		dispidGetWndChannelSelect = 24L,
		dispidSetWndChannelSelect = 23L,
		dispidGetTotalFrames = 22L,
		dispidGetCurPos = 21L,
		dispidGetPlayPos = 20L,
		dispidExit = 19L,
		dispidSetWndChannels = 18L,
		dispidSetSelectWnd = 17L,
		dispidSnap = 16L,
		dispidSeek = 15L,
		dispidNextFrame = 14L,
		dispidPreFrame = 13L,
		dispidReverse = 12L,
		dispidForward = 11L,
		dispidStop = 10L,
		dispidPause = 9L,
		dispidPlay = 8L,
		dispidCloseMedia = 7L,
		dispidOpenMedia = 6L,
		dispidFullScreen = 5L,
		dispidPolling = 4L,
		dispidChangePage = 3L,
		dispidSetWndStyle = 2L,
		dispidtest = 1L
	};
protected:
	void test(void);//用于测试
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual void OnSetClientSite();

protected:
	int InvokeJavaScriptFunc(OLECHAR FAR* bstrMember, VARIANT* argv, int argc, bool npass=true);

private:
	IDispatch*		  m_pScript;
	HANDLE			  mGlue;
	bool			  m_bIsPolling;

protected:
 	void SetWndStyle(LONG style, LONG CHNum);
	void ChangePage(LONG bNext);
	void Polling(void);
	void FullScreen(void);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	SHORT OpenMedia(LPCTSTR bstrPath, SHORT nType);
	SHORT CloseMedia(SHORT nWndIndex);
	VARIANT_BOOL Play(void);
	VARIANT_BOOL Pause(void);
	VARIANT_BOOL Stop(void);
	VARIANT_BOOL Forward(SHORT nSpeed);
	VARIANT_BOOL Reverse(SHORT nSpeed);
	void PreFrame(void);
	void NextFrame(void);
	VARIANT_BOOL Seek(LONG lPos, LONG minVal, LONG maxVal);
	SHORT Snap(LPCTSTR savePath);
	VARIANT_BOOL SetSelectWnd(SHORT nWndIndex);
	SHORT SetWndChannels(SHORT nWndId, SHORT nChannels);
	SHORT Exit(void);
	void GetPlayPos(LONG* curPos, LONG* nTotalFrames);
	LONG GetCurPos(void);
	LONG GetTotalFrames(void);
	SHORT SetWndChannelSelect(SHORT nWndIndex, SHORT nChannel);
	SHORT GetWndChannelSelect(SHORT nWndIndex);
	SHORT GetWndChannelCount(SHORT nWndIndex);

	void OnTwoWndSwap(LONG newWndId, LONG oldWndId)
	{
		FireEvent(eventidOnTwoWndSwap, EVENT_PARAM(VTS_I4 VTS_I4), newWndId, oldWndId);
	}
};

