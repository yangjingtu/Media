#include "DDrawWnd.h"
#include "../Play/PlayHelper.h"

CDDrawWnd::CDDrawWnd(void)
{
	InitWndID();
	INIT_LOCK;
}


CDDrawWnd::~CDDrawWnd(void)
{
	UNINIT_LOCK;
}

void CDDrawWnd::InitWndID()
{
	for (int i = 0; i < WND_MAX; ++i)
	{
		m_wndInfo[i].isUse = false;
		m_wndInfo[i].wnd = i + 1;
	}
	m_iWndCount = 0;
}

void CDDrawWnd::ClearWndId()
{
	for (int i = 0; i < WND_MAX; ++i)
	{
		m_wndInfo[i].isUse = false;
		m_wndInfo[i].wnd = 0;
	}
	m_iWndCount = 0;
}

short& CDDrawWnd::GetErrorWndID()
{
	LOCK;
	return m_wndInfo[WND_MAX - 1].wnd;
}

short& CDDrawWnd::GetWndID(short Id)
{
	LOCK;
	for (int i = 0; i < WND_MAX; ++i)
	{
		if(m_wndInfo[i].wnd == Id)
		{
			if(m_wndInfo[i].isUse == false)
			{
				m_wndInfo[i].isUse = true;
				m_iWndCount++;
				return m_wndInfo[i].wnd;
			}
		}
	}
	return m_wndInfo[WND_MAX].wnd;
}

short& CDDrawWnd::GetEmptyWndID()
{
	LOCK;
	for (int i = 0; i < WND_MAX; ++i)
	{
		if(m_wndInfo[i].isUse == false)
		{
			m_wndInfo[i].isUse = true;
			m_iWndCount++;
			return m_wndInfo[i].wnd;
		}
	}
	return m_wndInfo[WND_MAX].wnd;
}

void CDDrawWnd::ChangeWnd(short wnd1, short wnd2)
{
	LOCK;
	int i = 0;
	for( ; i < WND_MAX; ++i)
	{
		if(m_wndInfo[i].wnd == wnd1)
		{
			break;
		}
	}
	if( WND_MAX == i)
	{
		i = wnd1 - 1;
		m_wndInfo[i].wnd = wnd1;
	}

	int j = 0;
	for ( ; j < WND_MAX; ++j)
	{
		if(m_wndInfo[j].wnd == wnd2)
		{
			break;
		}
	}
	if(j == WND_MAX)
	{
		j = wnd2 - 1;
		m_wndInfo[j].wnd = wnd2;
	}

	short tmp = m_wndInfo[i].wnd;
	m_wndInfo[i].wnd = m_wndInfo[j].wnd;
	m_wndInfo[j].wnd = tmp;
}

void CDDrawWnd::CancelWndId(short wndId)
{
	LOCK;
	for (int i = 0; i < WND_MAX; ++i)
	{
		if(m_wndInfo[i].wnd == wndId)
		{
			m_wndInfo[i].isUse = false;
			m_iWndCount--;
			break;
		}
	}
	if(m_iWndCount == 0)
	{
		for (int i = 0 ; i < WND_MAX; ++i)
		{
			m_wndInfo[i].wnd = i + 1;
		}
	}
}

bool CDDrawWnd::IsWndIdValid(short wndId)
{
	return (wndId > 0 && wndId < WND_MAX);
}
