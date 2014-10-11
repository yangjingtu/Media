#pragma once
#include "stdafx.h"
#include "Audio.h"
#include "NPWnd.h"
#define IN_WAVEHDRS		5
class CWaveIn
{
public:
	CWaveIn(void);
	~CWaveIn(void);
	
	BOOL OpenRec();
	BOOL StartRec();
	BOOL CloseDev();
	BOOL SetRecParam(PARAMTYPE type, LPVOID pData );
	BOOL StopRec();
	void SetCallback(RECDATA fun, LPVOID pUser);
	static void CALLBACK waveOutProc( HWAVEIN hwo, UINT uMsg, DWORD dwInstance, DWORD lParam, DWORD wParam );

protected:
	static LRESULT CALLBACK AudioProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void SendData(int type, PWAVEHDR pWavHdr, BOOL bEnding);
	void ReleaseBuf();

private:
	PBYTE		pBuffer[IN_WAVEHDRS];
	PWAVEHDR	pWaveHdr[IN_WAVEHDRS];
	unsigned char* pSaveBuffer;

	HWAVEIN		m_hWaveIn;
	WAVEFORMATEX	wavformex;

	RECDATA		m_recdata;
	LPVOID		m_pUser;

	int			m_dataHeadSize;
	int			m_iWaveHdrSize;

	bool		m_bStop;
	CRITICAL_SECTION m_Section;

	CAWnd		m_myWnd;
	WNDPROC		m_OldWndProc;
};
