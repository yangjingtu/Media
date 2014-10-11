#include "stdafx.h"

CWaveOut::CWaveOut(void)
{
	m_hWaveOut = NULL;
	m_bPause = false;
	m_mmr = 0;
	m_iWaveHdrSize = 640;
	for (int i=0; i<OUT_WAVEHDRS; i++)
	{
		pWaveOutHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
		ZeroMemory(pWaveOutHdr[i], sizeof(PWAVEHDR));
		pWaveOutHdr[i]->dwUser = -1;
		pWaveOutBuf[i] = NULL;
	}
	decode_sample = decode_index = 0;
	InitializeCriticalSection(&m_critical);
}

CWaveOut::~CWaveOut(void)
{
	for (int i=0; i<OUT_WAVEHDRS; i++)
	{
		if (pWaveOutHdr[i])
		{
			free(pWaveOutHdr[i]);
			pWaveOutHdr[i] = NULL;
		}
		if (pWaveOutBuf[i])
		{
			free(pWaveOutBuf[i]);
			pWaveOutBuf[i] = NULL;
		}
	}
	DeleteCriticalSection(&m_critical);
}

/*------------------------------------------------------------------------------*/
void waveOutErrorMsg(MMRESULT mmr,char* szTitle)
{
	char txt[MAXERRORLENGTH];
	waveOutGetErrorText(mmr,txt,MAXERRORLENGTH);
	MessageBox(0,txt,szTitle,MB_OK|MB_ICONWARNING);
}

/*------------------------------------------------------------------------------*/
void CALLBACK CWaveOut::waveOutProc( HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD lParam, DWORD wParam )
{
	CWaveOut* pThis=(CWaveOut*)dwInstance;

	if(WOM_DONE == uMsg)    //播放完成
	{ 
		WAVEHDR* pWaveHdr = (WAVEHDR*)lParam;
		//EnterCriticalSection(&pThis->m_critical);
		if (pWaveHdr)   
		{
			//OutputDebugStr("waveOutUnprepareHeader Enter\n");
			//if (!waveOutUnprepareHeader(hwo, pWaveHdr, sizeof(WAVEHDR)))
			{
				//OutputDebugStr("waveOutUnprepareHeader Exit---\n");
				pWaveHdr->dwUser = -1;   
			}
		} 
		/*int i = -1;
		for(i = 0; i < OUT_WAVEHDRS; i++)   
		{   
			if((pThis->pWaveOutHdr[i])->dwUser != -1)   
			{   
				break;   
			}   
		}   
		if(i == OUT_WAVEHDRS)   
		{   
			OutputDebugStr("waveOutPause ----------------\n");
			waveOutPause(hwo);
			pThis->m_bPause = TRUE;
		} */
		//LeaveCriticalSection(&pThis->m_critical);
	}
	return ;
}

/*------------------------------------------------------------------------------*/
//打开声音播放设备
BOOL CWaveOut::OpenDev()
{
	AUDIODEBUG("open waveform audio for output");
	m_wavformex.wFormatTag		=	WAVE_FORMAT_PCM;
	m_wavformex.nChannels			=	1;
	m_wavformex.nSamplesPerSec	=	8000;
	m_wavformex.nAvgBytesPerSec	=	16000;
	m_wavformex.nBlockAlign		=	2;
	m_wavformex.wBitsPerSample	=	16;
	m_wavformex.cbSize			=	0;
	for (int i=0; i<OUT_WAVEHDRS; i++)
	{
		if (pWaveOutBuf[i] == NULL)
		{
			pWaveOutBuf[i] = (PBYTE)malloc(m_iWaveHdrSize);
			ZeroMemory(pWaveOutBuf[i], m_iWaveHdrSize);
			pWaveOutHdr[i]->dwBufferLength = m_iWaveHdrSize;
		}		
	}
	AUDIODEBUG("waveOutOpen test");
	if(m_mmr = waveOutOpen (0,WAVE_MAPPER,&m_wavformex,0,0,WAVE_FORMAT_QUERY))
	{
		AUDIODEBUG("waveOutOpen test err");
		ReleaseBuf();
		return FALSE;
	}
	AUDIODEBUG("waveOutOpen");
	if (m_mmr = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_wavformex, (DWORD)&CWaveOut::waveOutProc, (DWORD)this, CALLBACK_FUNCTION))
	{
		AUDIODEBUG("waveOutOpen err");
		ReleaseBuf();
		return FALSE;
	}
	return TRUE;
}

void CWaveOut::ReleaseBuf()
{
	for (int i=0; i<OUT_WAVEHDRS; i++)
	{
		if (pWaveOutBuf[i])
		{
			free(pWaveOutBuf[i]);
			pWaveOutBuf[i] = NULL;
		}		
	}
}

BOOL CWaveOut::PlayData(unsigned char* buf, int size)
{
	EnterCriticalSection(&m_critical);
	int iIndex = -1; 
	for(iIndex = 0; iIndex < OUT_WAVEHDRS; iIndex++)   
	{   
		if(pWaveOutHdr[iIndex]->dwUser == -1)   
		{
			m_mmr = waveOutUnprepareHeader(m_hWaveOut, pWaveOutHdr[iIndex], sizeof(WAVEHDR));
			if (m_mmr && MMSYSERR_INVALPARAM != m_mmr)
			{
				goto RET;
			}
			break;
		}   
	}
	if (iIndex < OUT_WAVEHDRS) {
		if (pWaveOutHdr[iIndex]->dwBufferLength != size)
		{
			free(pWaveOutBuf[iIndex]);
			pWaveOutBuf[iIndex] = (PBYTE)malloc(size);
		}
		memcpy(pWaveOutBuf[iIndex], buf, size);
		ZeroMemory(pWaveOutHdr[iIndex], sizeof(WAVEHDR));
		pWaveOutHdr[iIndex]->dwUser = iIndex;
		pWaveOutHdr[iIndex]->lpData = (LPSTR)pWaveOutBuf[iIndex];
		pWaveOutHdr[iIndex]->dwBufferLength = size;
		MMRESULT m_mmr=waveOutPrepareHeader(m_hWaveOut, pWaveOutHdr[iIndex], sizeof(WAVEHDR));
		DWORD i = GetLastError();
		if (m_mmr)
		{
			pWaveOutHdr[iIndex]->dwUser = -1;
			goto RET;
		}

		m_mmr=waveOutWrite(m_hWaveOut, pWaveOutHdr[iIndex], sizeof(WAVEHDR));
		if (m_mmr)
		{
			pWaveOutHdr[iIndex]->dwUser = -1;
			goto RET;
		}
		/*if(m_bPause == TRUE)   
		{
			OutputDebugStr("waveOutRestart Enter\n");
			waveOutRestart(m_hWaveOut); 
			OutputDebugStr("waveOutRestart Exit\n");
			m_bPause = FALSE;   
		}*/
		LeaveCriticalSection(&m_critical);
		return TRUE;
	}
RET:
	LeaveCriticalSection(&m_critical);
	return FALSE;
}


/*------------------------------------------------------------------------------*/
CWaveOut::operator HWAVEOUT() const
{
	return m_hWaveOut;
}

/*------------------------------------------------------------------------------*/

MMRESULT CWaveOut::GetLastMMError()
{
	return m_mmr;
}
/*------------------------------------------------------------------------------*/
void CWaveOut::SetLastMMError(MMRESULT mmr)
{
	m_mmr=mmr;
}
/*------------------------------------------------------------------------------*/
BOOL CWaveOut::CloseDev()
{
	EnterCriticalSection(&m_critical);
	waveOutReset(m_hWaveOut);
	for (int i=0; i<OUT_WAVEHDRS; i++)
	{
		waveOutUnprepareHeader(m_hWaveOut, pWaveOutHdr[i], sizeof(WAVEHDR));
		if (pWaveOutBuf[i])
		{
			free(pWaveOutBuf[i]);
			pWaveOutBuf[i] = NULL;
		}
		pWaveOutHdr[i]->dwUser = OUT_WAVEHDRS;
	}
	m_mmr = waveOutClose (m_hWaveOut) ;
	LeaveCriticalSection(&m_critical);
	return TRUE;
}

/*------------------------------------------------------------------------------*/
HWAVEOUT CWaveOut::GetHandle()
{
	return m_hWaveOut;
}

BOOL CWaveOut::SetPlayParam( PARAMTYPE type, LPVOID pData )
{
	assert(pData != NULL);
	if (PARAMTYPE::SETDATAHEADSIZE == type)
	{
	}else if (PARAMTYPE::SETWAVEFORMATEX == type)
	{
		memcpy(&m_wavformex, pData, sizeof(WAVEFORMATEX));
	}else if (PARAMTYPE::SETWAVEHDRSIZE == type)
	{
		m_iWaveHdrSize = *(int*)pData;
	}else{
		return FALSE;
	}
	return TRUE;
}
