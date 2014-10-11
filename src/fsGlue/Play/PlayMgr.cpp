#include <WinSock2.h>
#include "PlayMgr.h"
#include "PlayHelper.h"
#include "../Util/DDrawWnd.h"

CPlayMgr::CPlayMgr(void)
	: m_hWnd(NULL)
	, m_isInit(false)
	, m_dwThreadId(0)
	, m_ddrawWnd(NULL)
	, m_hWaveOut(NULL)
	, m_bInitAudio(false)
{

}

CPlayMgr::~CPlayMgr(void)
{
	if(m_isInit)
	{
		UnInit();
	}
}

bool CPlayMgr::Init(HWND hWnd)
{
	if(hWnd == NULL)
		return false;

	m_hWnd = hWnd;

	WRITELOG("InitDDraw7");
	m_playView.InitDDraw7(m_hWnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	m_playView.CreateClipper(m_hWnd);
	m_playView.SetWndStyle(10, 1);
	m_playView.ShowPage(0);
	m_playView.DrawSelectBorder(1);
	m_playView.DrawBorder();

	m_isInit = true;
	m_ddrawWnd = new CDDrawWnd();

	::InitializeCriticalSection(&m_cAudio);
	WRITELOG("InitAudio");
	InitAudio();
	
	m_hIOThread = CreateThread(NULL, NULL, CPlayMgr::Run, this, NULL, &m_dwThreadId);
	m_bIORun = true;

	return true;
}

void CPlayMgr::UnInit()
{ 
	RemoveAll();

	if(m_ddrawWnd)
	{
		m_ddrawWnd->ClearWndId();
		delete m_ddrawWnd;
		m_ddrawWnd = NULL;
	}

	WRITELOG("WaitForSingleObject IO Thread");
	m_bIORun = false;
	if(WaitForSingleObject(m_hIOThread, 5) == WAIT_TIMEOUT)
	{
		::TerminateThread(m_hIOThread, 0);
	}
	WRITELOG("WaitForSingleObject IO Thread END");
	CloseHandle(m_hIOThread);
	m_hIOThread = NULL;
	m_dwThreadId = 0;

	if(m_isInit)
	{
		m_playView.ClearScreen();
		m_playView.ReleaseDDraw();
		WRITELOG("ReleaseDDraw");
	}
		
	ReleaseAudio();
	WRITELOG("ReleaseAudio");
	::DeleteCriticalSection(&m_cAudio);

	m_isInit = false;
}

DWORD WINAPI CPlayMgr::Run(LPVOID param)
{
	CPlayMgr* self = (CPlayMgr*)param;
	if(self)
	{
		return self->RunFunc();
	}
	return 0;
}

DWORD CPlayMgr::RunFunc()
{
	MSG msg;

	int iTimes = 0;

	while (m_bIORun)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
 			case CLEARSCREEN:
 				{
					m_playView.ClearScreen();
					m_playView.DrawBorder();
					m_playView.Flip();
					if (msg.wParam == 100)
					{
						POINT pt = { LOWORD(msg.lParam), HIWORD(msg.lParam) };
						m_playView.DrawSelectBorder(pt);
					}
					else
					{
						int i = m_playView.GetSelectWndID();
						if (i > 0)
						{
							m_playView.DrawSelectBorder(i);
						}
					}

					iTimes = 1;
 					break;
 				}
			default:
				break;
			}
		}

		if(iTimes == 0)
			m_playView.Flip();
		Sleep(20);

		if( iTimes == 1)
		{
			Sleep(200);

			m_playView.ClearScreen();
			m_playView.DrawBorder();
			m_playView.Flip();

			iTimes = 0;
		}
	}
	return 0;
}

void CPlayMgr::PostMsg(UINT msg, WPARAM wparam, LPARAM lparam)
{
	PostThreadMessage(m_dwThreadId, msg, wparam, lparam);
}

//////////////////////////////////////////////////////////////////////////
CPlayHelper* CPlayMgr::FindPlayObj(const POINT& pt)
{
	int nWndId = m_playView.GetWndID(pt);
	return FindPlayObj(nWndId);
}

CPlayHelper* CPlayMgr::FindPlayObj(short nWndId)
{
	if(m_dwThreadId == NULL)
	{
		return NULL;
	}

	if(m_ddrawWnd->IsWndIdValid(nWndId))
	{
		VecPlayObjItor it = m_vWnd.begin();
		for ( ; it != m_vWnd.end(); ++it)
		{
			if((*it) && (*it)->GetWndId() == nWndId)
			{
				return (*it);
			}
		}
	}
	return NULL;
}

bool CPlayMgr::AddPlayObj(CPlayHelper* pObj)
{
	if(pObj == NULL || m_ddrawWnd == NULL)
	{
		return false;
	}

	if(!m_ddrawWnd->IsWndIdValid(pObj->GetWndId()))
	{
		return false;
	}

	bool bAdd = true;
	VecPlayObjItor it = m_vWnd.begin();
	for ( ; it != m_vWnd.end(); ++it)
	{
		if((*it) == pObj)
		{
			bAdd = false;
			break;
		}
		if((*it)->GetWndId() == pObj->GetWndId())
		{
			bAdd = false;
			break;
		}
	}

	if(bAdd)
	{
		m_vWnd.push_back(pObj);
	}

	return bAdd;
}

CPlayHelper* CPlayMgr::CreatePlayObj(int nType, short nWndId)
{
	CPlayHelper* pObj = NULL;
	if(m_ddrawWnd == NULL)
	{
		return pObj;
	}

	short& wndIdTmp = (nWndId == -1 ? m_ddrawWnd->GetEmptyWndID() : m_ddrawWnd->GetWndID(nWndId));
	switch (nType)
	{
	case EM_LOCAL_FILE:
		pObj = new CPlayFile(&m_playView,&wndIdTmp);
		break;
	case EM_NET_FILE:
		pObj = new CPlayNet(&m_playView,&wndIdTmp);
		break;
	default:
		break;
	}

	if(pObj)
	{
		pObj->SetParent(this);
		AddPlayObj(pObj);
		WRITELOG("AddPlayObj");
	}
	return pObj;
}

void CPlayMgr::RemovePlayObj(CPlayHelper* pObj)
{
	if(pObj == NULL)
	{
		return;
	}

	if(m_ddrawWnd->IsWndIdValid(pObj->GetWndId()))
	{
		m_ddrawWnd->CancelWndId(pObj->GetWndId());
	}

	VecPlayObjItor it = m_vWnd.begin();
	for ( ; it != m_vWnd.end(); ++it)
	{
		if((*it) == pObj)
		{
			pObj->Close();
			delete pObj;
			pObj = NULL;

			m_vWnd.erase(it);
			WRITELOG("RemovePlayObj");
			break;
		}
	}
}

void CPlayMgr::RemoveAll()
{
	VecPlayObjItor it = m_vWnd.begin();
	for( ; it != m_vWnd.end(); ++it)
	{
		if((*it))
		{
			(*it)->Close();
			(*it) = NULL;
		}
	}
	m_vWnd.clear();
}

void CPlayMgr::RemovePlayObj(short nWndId)
{
	CPlayHelper* pObj = FindPlayObj(nWndId);
	RemovePlayObj(pObj);
}

void CPlayMgr::ChangeWnd(int wnd1, int wnd2)
{
	if(m_ddrawWnd)
	{
		m_ddrawWnd->ChangeWnd(wnd1, wnd2);
	}
}
//////////////////////////////////////////////////////////////////////////
void CPlayMgr::InitAudio(PWAVEFORMATEX pWfx)
{
	EnterCriticalSection(&m_cAudio);
	if( m_hWaveOut == NULL)
	{
		if(pWfx)
		{
			m_hWaveOut = CreatePlayDev2();
			m_wfx = *pWfx;
			m_bInitAudio = OpenPlayDev2(m_hWaveOut, pWfx->nSamplesPerSec, pWfx->nChannels, pWfx->wBitsPerSample);
		}
		else
		{
			memset(&m_wfx, 0x0, sizeof(WAVEFORMATEX));
		}
	}
	LeaveCriticalSection(&m_cAudio);
}

void CPlayMgr::ReleaseAudio()
{
	if(!m_bInitAudio)
	{
		return;
	}
	EnterCriticalSection(&m_cAudio);
	if (m_hWaveOut)
	{
		m_bInitAudio = ClosePlayDev2(m_hWaveOut);
		m_hWaveOut = NULL;
	}
	memset(&m_wfx, 0x0, sizeof(WAVEFORMATEX));
	LeaveCriticalSection(&m_cAudio);
}

bool CPlayMgr::AudioPlay(unsigned char* buf, int size,  int type)
{
	if(!m_bInitAudio)
		return false;
	EnterCriticalSection(&m_cAudio);
	int iwnd = m_playView.GetSelectWndID();
	if (type == iwnd)
	{
		bool b = WriteData2(m_hWaveOut, buf, size);
	}
	LeaveCriticalSection(&m_cAudio);
	return true;
}

void CPlayMgr::CloseAudioPlay()
{
	if(!m_bInitAudio)
		return;
	EnterCriticalSection(&m_cAudio);
	if (m_hWaveOut != NULL)
	{
		m_bInitAudio = ClosePlayDev2(m_hWaveOut);
		m_hWaveOut = NULL;
	}
	LeaveCriticalSection(&m_cAudio);
}

void CPlayMgr::ResetAudio(PWAVEFORMATEX pWfx)
{
	if(pWfx && (pWfx->nChannels != m_wfx.nChannels || 
		pWfx->nSamplesPerSec != m_wfx.nSamplesPerSec || 
		pWfx->wBitsPerSample != m_wfx.wBitsPerSample))
	{
		CloseAudioPlay();
		InitAudio(pWfx);
	}
}
