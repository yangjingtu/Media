#pragma once
#include "stdafx.h"

#include "npwnd.h"
#include "../wellMedia/CommDataDef.h"
#include "Play/PlayNet.h"
#include "Play/PlayFile.h"
#include "Play/PlayMgr.h"

class CNPWndProc :
	public CNPWnd
{
	friend class CGlue;
public:
	CNPWndProc(void);
	~CNPWndProc(void);

	virtual HWND Create(HWND hwndParent, LPCTSTR pstrClassName, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HINSTANCE hInstance, HMENU hMenu = NULL);

	virtual void OnDestroy();
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

	void DrawBorder(HPEN hPen);

	CDDrawView& GetDDrawView() ;

public://设备操作方法
	BOOL OpenMedia(const char* mediaPath, short nType);
	BOOL CloseMedia();

	BOOL Play();
	BOOL Pause();
	BOOL Stop();
	BOOL Reverse(short nSpeed);
	BOOL Forward(short nSpeed);
	BOOL PreFrame();
	BOOL NextFrame();
	BOOL Seek(long pos, long min, long max);
	short Snap(const char* savePath);
	void GetPlayPos(long* curPos, long* totalFrames);

	void SetSelectChannel(short nChannel);
	void SetChannels(short nChannels);
	short GetChannels() const { return m_nWndCount; }

	void Exit();

protected:
	BOOL OpenVideo(const char* url);
	BOOL CloseVideo();
	BOOL OpenAudio();
	BOOL CloseAudio();

private:
	CPlayHelper* GetSelectPlayObj();
	bool RemoveSelectPlayObj();

private:
	//////////////////////////////////////////////////////////////////////////
	//外层多窗口控制
	bool	m_isSelect;
	int		m_nChannelNo;
	//////////////////////////////////////////////////////////////////////////

	CPlayMgr m_PlayMgr;
	//////////////////////////////////////////////////////////////////////////
	//DDraw表面多窗口控制
	short m_nWndStyle;
	short m_nOldWnd;
	short m_nOldStyle;
	short m_nOldPage;

	bool m_bMouseDown;

	short m_nSelectWndId;

	short m_nWndCount;
	//////////////////////////////////////////////////////////////////////////
};
