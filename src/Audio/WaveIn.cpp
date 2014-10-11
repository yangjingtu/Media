#include "WaveIn.h"
#include <WindowsX.h>
CWaveIn::CWaveIn(void)
{
	m_dataHeadSize = 0;
	m_iWaveHdrSize = 960;	//G726
	m_recdata = NULL;
	m_pUser = NULL;
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		pWaveHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
		ZeroMemory(pWaveHdr[i], sizeof(PWAVEHDR));
		pWaveHdr[i]->dwUser = -1;
		pBuffer[i] = NULL;
	}

	pSaveBuffer = NULL;
	InitializeCriticalSection(&m_Section);
	m_bStop = false;
}

CWaveIn::~CWaveIn(void)
{
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		if (pWaveHdr[i])
		{
			free(pWaveHdr[i]);
			pWaveHdr[i] = NULL;
		}
	}
	DeleteCriticalSection(&m_Section);
}

BOOL CWaveIn::StopRec()
{
	if (m_hWaveIn == NULL) return FALSE;
	EnterCriticalSection(&m_Section);
	if (m_bStop){
		LeaveCriticalSection(&m_Section);
		return TRUE;
	} 
	m_bStop = true;
	LeaveCriticalSection(&m_Section);
	
	waveInStop(m_hWaveIn);
	waveInReset(m_hWaveIn);
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		waveInUnprepareHeader(m_hWaveIn, pWaveHdr[i], sizeof(WAVEHDR));
	}
	return TRUE;
}

BOOL CWaveIn::CloseDev()
{
	if (m_hWaveIn == NULL) return FALSE;
	EnterCriticalSection(&m_Section);
	if (!m_bStop)
	{
		StopRec();
	}
	LeaveCriticalSection(&m_Section);

	waveInReset(m_hWaveIn);
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		//waveInUnprepareHeader(m_hWaveIn, pWaveHdr[i], sizeof(WAVEHDR));
		if (pBuffer[i])
		{
			free(pBuffer[i]);
			pBuffer[i] = NULL;
		}
	}
	if (pSaveBuffer)
	{
		delete []pSaveBuffer;
		pSaveBuffer = NULL;
	}
	
	SetWindowLongPtr(m_myWnd.GetHWND(), GWLP_USERDATA, (LONG_PTR)NULL);
	SubclassWindow(m_myWnd.GetHWND(), (WNDPROC)m_OldWndProc);

	waveInClose(m_hWaveIn);
	if (IsWindow(m_myWnd.GetHWND()))
	{
		::SendMessage(m_myWnd.GetHWND(), WM_QUIT, NULL, NULL);
	}
	return TRUE;
}

void CWaveIn::ReleaseBuf()
{
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		if (pBuffer[i] == NULL)
		{
			pBuffer[i] = (PBYTE)malloc(m_iWaveHdrSize);
			ZeroMemory(pBuffer[i], m_iWaveHdrSize);
		}		
	}
}

LRESULT CALLBACK CWaveIn::AudioProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CWaveIn* pwaveIn = (CWaveIn*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (pwaveIn)
	{
		switch (msg)
		{
		case MM_WIM_DATA:
			{
				//OutputDebugStr("waveOutProc + \n");

				EnterCriticalSection(&pwaveIn->m_Section);
				if(!pwaveIn->m_bStop){

					//OutputDebugStr("waveOutProc\n");

					pwaveIn->SendData(0, (PWAVEHDR)lParam,(BOOL)wParam);
					memset(((PWAVEHDR)lParam)->lpData, 0, pwaveIn->m_iWaveHdrSize);

					DWORD i = GetLastError();
					char buf[100];
					wsprintf(buf, "waveInAddBuffer +%d :err: %d\n", ((PWAVEHDR)lParam)->dwUser, i);
					//OutputDebugStr(buf);
					//waveInUnprepareHeader((HWAVEIN)wParam,((PWAVEHDR)lParam),sizeof(WAVEHDR));
					//waveInPrepareHeader((HWAVEIN)wParam,((PWAVEHDR)lParam),sizeof(WAVEHDR));

					waveInAddBuffer(pwaveIn->m_hWaveIn, ((PWAVEHDR)lParam), sizeof(WAVEHDR));
					//OutputDebugStr("waveInAddBuffer -\n");

					LeaveCriticalSection(&pwaveIn->m_Section);
				}else
					LeaveCriticalSection(&pwaveIn->m_Section);

				//OutputDebugStr("waveOutProc - \n");
			}
			break;
		case MM_WIM_OPEN:
			break;
		case MM_WIM_CLOSE://只在拔除mic时处理
			{
				if (hWnd == NULL) break;
				EnterCriticalSection(&pwaveIn->m_Section);
				if (!pwaveIn->m_bStop)
				{
					pwaveIn->SendData(1, (PWAVEHDR)NULL,(BOOL)wParam);
					pwaveIn->m_bStop = true;
					for (int i=0; i<IN_WAVEHDRS; i++)
					{
						waveInUnprepareHeader(pwaveIn->m_hWaveIn, pwaveIn->pWaveHdr[i], sizeof(WAVEHDR));
					}
				}
				LeaveCriticalSection(&pwaveIn->m_Section);
			}
			break;
// 		case WM_NCDESTROY:
// 			{
// 				LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
// 				pwaveIn->m_myWnd.m_hWnd = NULL;
// 				return lRes;
// 			}
// 			break;
		default:
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CWaveIn::OpenRec()
{
	AUDIODEBUG("OpenDev --- Create HDR");
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		if (pBuffer[i] == NULL)
		{
			pBuffer[i] = (PBYTE)malloc(m_iWaveHdrSize);
			ZeroMemory(pBuffer[i], m_iWaveHdrSize);
		}		
	}
	if (pSaveBuffer == NULL)
	{
		pSaveBuffer = new unsigned char[m_iWaveHdrSize + m_dataHeadSize];
		ZeroMemory(pSaveBuffer, m_iWaveHdrSize + m_dataHeadSize);
	}
	//Open waveformat audio for input
	wavformex.wFormatTag		=	WAVE_FORMAT_PCM;
	wavformex.nChannels			=	1;
	wavformex.nSamplesPerSec	=	8000;
	wavformex.nAvgBytesPerSec	=	16000;
	wavformex.nBlockAlign		=	2;
	wavformex.wBitsPerSample	=	16;
	wavformex.cbSize			=	0;
	//Open the device for recording
	AUDIODEBUG("waveInOpen");
	extern HANDLE g_hDllModule;
	m_myWnd.Create(NULL, "static", "audio", WS_MINIMIZE, 0, (HINSTANCE)g_hDllModule);
	SetWindowLongPtr(m_myWnd.GetHWND(), GWLP_USERDATA, (LONG_PTR)this);
	m_OldWndProc = SubclassWindow(m_myWnd.GetHWND(), (WNDPROC)AudioProc);
	m_myWnd.ShowWindow(false, false);
	MMRESULT mrt = waveInOpen(&m_hWaveIn, WAVE_MAPPER, &wavformex, (DWORD)m_myWnd.GetHWND(), NULL, CALLBACK_WINDOW);
	//使用 CALLBACK_FUNCTION ，当mic拔除时，收不到 MM_MIN_CLOSE 消息。原因不明， 改用 CALLBACK_WINDOW
	//MMRESULT mrt = waveInOpen(&m_hWaveIn, WAVE_MAPPER, &wavformex, (DWORD)&CWaveIn::waveOutProc, (DWORD)this, CALLBACK_FUNCTION);
	if(mrt)
	{
		int ret = GetLastError();
		AUDIODEBUG("waveInOpen Err : %d", ret);
		ReleaseBuf();
		AUDIODEBUG("waveInOpen Err -- ReleaseBuf");
		goto RET;
	}

	AUDIODEBUG("PrepareHeader");
	//为录音设备准备缓存//////////////////////////////////////
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		pWaveHdr[i]->lpData =			(LPTSTR)pBuffer[i];
		pWaveHdr[i]->dwBufferLength = m_iWaveHdrSize;
		pWaveHdr[i]->dwBytesRecorded = 0;
		pWaveHdr[i]->dwFlags =		0;;
		pWaveHdr[i]->dwUser  =		i;
		pWaveHdr[i]->dwLoops =		1;
		pWaveHdr[i]->lpNext  =		NULL;
		pWaveHdr[i]->reserved =		0;
	}

// 	for (int i=0; i<IN_WAVEHDRS; i++)
// 	{
// 		waveInPrepareHeader(m_hWaveIn, pWaveHdr[i], sizeof(WAVEHDR));
// 	}

	//给录音设备添加缓存
// 	for (int i=0; i<IN_WAVEHDRS; i++)
// 	{
// 		waveInAddBuffer(m_hWaveIn, pWaveHdr[i], sizeof(WAVEHDR));
// 	}

	return TRUE;

RET:
	return FALSE;
}

BOOL CWaveIn::StartRec()
{
	//开始录音
	if (m_hWaveIn == NULL)
	{
		OpenRec();
	}
	if (m_hWaveIn == NULL) return FALSE;
	for (int i=0; i<IN_WAVEHDRS; i++)
	{
		waveInPrepareHeader(m_hWaveIn, pWaveHdr[i], sizeof(WAVEHDR));
		waveInAddBuffer(m_hWaveIn, pWaveHdr[i], sizeof(WAVEHDR));
	}
	EnterCriticalSection(&m_Section);
	m_bStop = false;
	waveInStart(m_hWaveIn);
	LeaveCriticalSection(&m_Section);
	return 0;
}

void CALLBACK CWaveIn::waveOutProc( HWAVEIN hwo, UINT uMsg, DWORD dwInstance, DWORD lParam, DWORD wParam )
{
	CWaveIn* pwaveIn = (CWaveIn*)dwInstance;
	if ( MM_WIM_DATA == uMsg )
	{
// 		WAVEHDR* pWH=(WAVEHDR*)lParam;
// 		waveInUnprepareHeader((HWAVEIN)wParam,pWH,sizeof(WAVEHDR));
// 
// 		pwaveIn->SendData((PWAVEHDR)lParam,(BOOL)wParam);
// 		memset(((PWAVEHDR)lParam)->lpData, 0, pwaveIn->m_iWaveHdrSize);
// 
// 		waveInPrepareHeader((HWAVEIN)wParam,pWH,sizeof(WAVEHDR));
// 		waveInAddBuffer((HWAVEIN)wParam,pWH,sizeof(WAVEHDR));
		//OutputDebugStr("waveOutProc + \n");

		EnterCriticalSection(&pwaveIn->m_Section);
		if(!pwaveIn->m_bStop){
			
			//OutputDebugStr("waveOutProc\n");			
			pwaveIn->SendData(0, (PWAVEHDR)lParam,(BOOL)wParam);
			memset(((PWAVEHDR)lParam)->lpData, 0, pwaveIn->m_iWaveHdrSize);

			DWORD i = GetLastError();
			char buf[100];
			wsprintf(buf, "waveInAddBuffer +%d :err: %d\n", ((PWAVEHDR)lParam)->dwUser, i);
			//OutputDebugStr(buf);
			//waveInUnprepareHeader((HWAVEIN)wParam,((PWAVEHDR)lParam),sizeof(WAVEHDR));
			//waveInPrepareHeader((HWAVEIN)wParam,((PWAVEHDR)lParam),sizeof(WAVEHDR));
			
			waveInAddBuffer(hwo, ((PWAVEHDR)lParam), sizeof(WAVEHDR));
			//OutputDebugStr("waveInAddBuffer -\n");

			LeaveCriticalSection(&pwaveIn->m_Section);
		}else
			LeaveCriticalSection(&pwaveIn->m_Section);

		//OutputDebugStr("waveOutProc - \n");
		
	}else if(MM_WIM_CLOSE == uMsg){
		EnterCriticalSection(&pwaveIn->m_Section);
		if (!pwaveIn->m_bStop)
		{
			pwaveIn->m_bStop = true;
			for (int i=0; i<IN_WAVEHDRS; i++)
			{
				waveInUnprepareHeader(hwo, pwaveIn->pWaveHdr[i], sizeof(WAVEHDR));
			}
		}
		LeaveCriticalSection(&pwaveIn->m_Section);
	}else{
		int a=0;
		a++;
	}	
}

void CWaveIn::SendData(int type, PWAVEHDR pWavHdr, BOOL bEnding)
{
	if (m_recdata)
	{
		if (pWavHdr)
		{
			(*m_recdata)(type, (unsigned char*)pWavHdr->lpData, m_dataHeadSize + m_iWaveHdrSize, m_pUser);
		}else{
			(*m_recdata)(type, NULL, 0, m_pUser);
		}
	}
}

void CWaveIn::SetCallback(RECDATA fun, LPVOID pUser)
{
	m_recdata = fun;
	m_pUser = pUser;
}

BOOL CWaveIn::SetRecParam( PARAMTYPE type, LPVOID pData )
{
	assert(pData != NULL);
	if (PARAMTYPE::SETDATAHEADSIZE == type)
	{
		m_dataHeadSize = *(int*)pData;
	}else if (PARAMTYPE::SETWAVEFORMATEX == type)
	{
		memcpy(&wavformex, pData, sizeof(WAVEFORMATEX));
	}else if (PARAMTYPE::SETWAVEHDRSIZE == type)
	{
		m_iWaveHdrSize = *(int*)pData;
	}else{
		return FALSE;
	}
	return TRUE;
}