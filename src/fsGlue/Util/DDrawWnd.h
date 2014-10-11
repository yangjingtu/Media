#pragma once
#include "../stdafx.h"
#include <map>
using namespace std;
#include "CriLock.h"

class CPlayHelper;

#define URLLEN		128
#define STRLEN		32
#define MACLEN		20
#define UIDLEN		128
#define UIDMINLEN	20

typedef struct tagDeviceInfo{
	int		id;
	TCHAR	ip[URLLEN+1];
	int		webPort;
	TCHAR	user[STRLEN+1];
	TCHAR	pwd[STRLEN+1];
	int		type;
	TCHAR	mac[MACLEN+1];
	CPlayHelper* pPlay;
}DeviceInfo, lPDeviceInfo;

#define WND_MAX 65

class CDDrawWnd
{
public:
	CDDrawWnd(void);
	~CDDrawWnd(void);

public:
	short& GetWndID(short Id);
	short& GetEmptyWndID();
	short& GetErrorWndID();
	void ChangeWnd(short wnd1, short wnd2);
	void CancelWndId(short wndId);

	bool IsWndIdValid(short wndId);

	void ClearWndId();

private:
	void InitWndID();

private:
	//////////////////////////////////////////////////////////////////////////
	//´°¿ÚID
	typedef struct tagDDrawWndInfo
	{
		short wnd;
		bool isUse;
	}DDrawWndInfo, *PDDrawWndInfo;

	DDrawWndInfo m_wndInfo[WND_MAX];
	short m_iWndCount;
	//////////////////////////////////////////////////////////////////////////

	DECLARE_LOCK
};

