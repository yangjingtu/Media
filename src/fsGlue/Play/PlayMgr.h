#pragma once
#include "../../Video/Video.h"
#include "../../Audio/Audio.h"
#include <vector>
using namespace std;

#define CLEARSCREEN	WM_USER + 101

class CWndPlayView;
class CPlayHelper;
class CDDrawWnd;

typedef vector<CPlayHelper*>  VecPlayObj;
typedef VecPlayObj::iterator	VecPlayObjItor;

class CPlayMgr
{
public:
	CPlayMgr();
	~CPlayMgr(void);

	bool Init(HWND hWnd);
	void UnInit();

	CDDrawView& GetDDrawView() { return m_playView; }

	void PostMsg(UINT,  WPARAM, LPARAM);

	CPlayHelper* CreatePlayObj(int nType, short nWndId = -1);
	CPlayHelper* FindPlayObj(const POINT& pt);
	CPlayHelper* FindPlayObj(short nWndId);
	void RemovePlayObj(short nWndId);

	void ChangeWnd(int wnd1, int wnd2);

	void InitAudio(PWAVEFORMATEX pWfx = NULL);
	void ReleaseAudio();
	void CloseAudioPlay();
	bool AudioPlay(unsigned char* buf, int size,  int type);
	void ResetAudio(PWAVEFORMATEX pWfx);

	void RemoveAll();
private:
	bool AddPlayObj(CPlayHelper* pObj);
	void RemovePlayObj(CPlayHelper* pObj);

	static DWORD WINAPI Run(LPVOID param);
	DWORD RunFunc();

private:
	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	HWND GetHwnd() const { return m_hWnd; }

private:
	HWND m_hWnd;						//ddraw显示的窗口
	CDDrawView m_playView;		//ddraw对象类
	CDDrawWnd* m_ddrawWnd;		//窗口管理对象

	bool m_isInit;

	//////////////////////////////////////////////////////////////////////////
	//IO线程，定时把后表面翻转显示
	bool m_bIORun;						
	HANDLE m_hIOThread;
	DWORD m_dwThreadId;
	//////////////////////////////////////////////////////////////////////////

	VecPlayObj m_vWnd;	//窗口对应的播放设备列表

	CRITICAL_SECTION	m_cAudio;
	HAUDIOOUT	m_hWaveOut;
	WAVEFORMATEX m_wfx;
	bool m_bInitAudio;
};

