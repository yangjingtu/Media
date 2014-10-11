#include "WavePlay.h"

#include <mmsystem.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE  8192
#define BLOCK_COUNT 20

#define BLOCK_MAX 8192


//回调函数
//如果选择窗口接受回调信息, 可能会发送到窗口的消息有:
//MM_WOM_OPEN  = $3BB;
// MM_WOM_CLOSE = $3BC;
// MM_WOM_DONE  = $3BD;

// 如果选择函数接受回调信息, 可能会发送给函数的消息有:
// WOM_OPEN  = MM_WOM_OPEN;
// WOM_CLOSE = MM_WOM_CLOSE;
// WOM_DONE  = MM_WOM_DONE;
void CALLBACK callback_waveOutProc( HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ) 
{ 
    /*
     * 忽略打开关闭设备操作
     */
    if(uMsg != WOM_DONE)
        return;
	CWavePlay* pThis=(CWavePlay*)dwInstance;
	if (NULL==pThis)
	{
		return;
	}
	pThis->WaveMsg(hwo,uMsg,dwParam1,dwParam2);
	return;
} 

CWavePlay::CWavePlay(void)
{
	m_bPalyState = false; 
	ZeroMemory(&m_wavformex,sizeof(WAVEFORMATEX)); 
	_waveBlocks          = NULL;
	_nWaveFreeBlockCount = 0;
	_nWaveCurrentBlock   = 0;

	InitializeCriticalSection(&_waveCriticalSection);
}

//关闭线程，释放资源
CWavePlay::~CWavePlay(void)
{
	CloseDevice();
	DeleteCriticalSection(&_waveCriticalSection);
}

bool CWavePlay::CloseDevice()
{
	ZeroMemory(&m_wavformex,sizeof(WAVEFORMATEX)); 

	 while(_nWaveFreeBlockCount != 0 && _nWaveFreeBlockCount < BLOCK_COUNT)
        Sleep(10);

    /*
     * unprepare any blocks that are still prepared
     */
    for(int i = 0; i < _nWaveFreeBlockCount; i++)
	{
        if(_waveBlocks && _waveBlocks[i].dwFlags & WHDR_PREPARED)
		{
            waveOutUnprepareHeader(m_hWaveOut, &_waveBlocks[i], sizeof(WAVEHDR));
		}
	}
	if(_waveBlocks)
	{
		freeBlocks(_waveBlocks);
		_waveBlocks = NULL;
	}
	waveOutClose(m_hWaveOut);
	m_bPalyState = false; 
	return true;
}

bool CWavePlay::Start(PWAVEFORMATEX pWavHead) 
{ 
	if (m_bPalyState) 
	{
		return false;
	}
	if (NULL==pWavHead)
	{
		return false;
	}
	//CloseDevice();
	_waveBlocks = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
	_nWaveFreeBlockCount = BLOCK_COUNT;
	_nWaveCurrentBlock   = 0;
	memcpy(&m_wavformex,pWavHead,sizeof(WAVEFORMATEX));
	m_wavformex.nBlockAlign     = (m_wavformex.wBitsPerSample * m_wavformex.nChannels) >> 3;
	m_wavformex.nAvgBytesPerSec = m_wavformex.nBlockAlign * m_wavformex.nSamplesPerSec;

	if(::waveOutOpen (0,0,&m_wavformex,0,0,WAVE_FORMAT_QUERY)) //WAVE_FORMAT_QUERY = $0001;{只是判断设备是否支持给定的格式, 并不打开}
	{ 
		//TRACE_ERR2("wave设备初始化失败～"); 
		return false; 
	} 

	//lphWaveOut: PHWaveOut;   {用于返回设备句柄的指针; 如果 dwFlags=WAVE_FORMAT_QUERY, 这里应是 nil}
	//uDeviceID: UINT;         {设备ID; 可以指定为: WAVE_MAPPER, 这样函数会根据给定的波形格式选择合适的设备}
	//lpFormat: PWaveFormatEx; {TWaveFormat 结构的指针; TWaveFormat 包含要申请的波形格式}
	//dwCallback: DWORD        {回调函数地址或窗口句柄; 若不使用回调机制, 设为 nil}
	//dwInstance: DWORD        {给回调函数的实例数据; 不用于窗口}
	//dwFlags: DWORD           {打开选项}
	if(waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_wavformex, (DWORD_PTR)callback_waveOutProc, (DWORD)this, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) 
	{
		 //TRACE_ERR2("wave设备打开失败～"); 
		 return false;
	}
	m_bPalyState    = true;
	_nWaveCurrentBlock   = 0;
	return true;
} 


void CWavePlay::WaveMsg(HWAVEOUT hwo, UINT uMsg, DWORD dwParam1, DWORD dwParam2) 
{
	EnterCriticalSection(&_waveCriticalSection);
	_nWaveFreeBlockCount++;
	LeaveCriticalSection(&_waveCriticalSection);
	return;
}

void CWavePlay::PlayAudio(LPSTR data, int size)
{
	if (!m_bPalyState) 
		return ; 

    WAVEHDR* current;
    int remain;

    current = &_waveBlocks[_nWaveCurrentBlock];
	if(current == NULL)
		return;
    
    while(size > 0) {
        /* 
         * 首先确定使用的header 是 unprepared
         */
        if(current->dwFlags & WHDR_PREPARED) 
            waveOutUnprepareHeader(m_hWaveOut, current, sizeof(WAVEHDR));

        if(size < (int)(BLOCK_SIZE - current->dwUser)) {
            memcpy(current->lpData + current->dwUser, data, size);
            current->dwUser += size;
            break;
        }

        remain = BLOCK_SIZE - current->dwUser;
        memcpy(current->lpData + current->dwUser, data, remain);
        size -= remain;
        data += remain;
        current->dwBufferLength = BLOCK_SIZE;
       
        waveOutPrepareHeader(m_hWaveOut, current, sizeof(WAVEHDR));
        waveOutWrite(m_hWaveOut, current, sizeof(WAVEHDR));
        
        EnterCriticalSection(&_waveCriticalSection);
        _nWaveFreeBlockCount--;
        LeaveCriticalSection(&_waveCriticalSection);
        
        /*
         * 等待free一个block
         */
        while(!_nWaveFreeBlockCount)
            Sleep(10);

        /*
         * 指向下一个block
         */
        _nWaveCurrentBlock++;
        _nWaveCurrentBlock %= BLOCK_COUNT;

        current = &_waveBlocks[_nWaveCurrentBlock];
        current->dwUser = 0;
    }
}


WAVEHDR* CWavePlay::allocateBlocks(int size, int count)
{
	unsigned char* buffer;
	WAVEHDR* blocks;
	DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;

	if((buffer = (unsigned char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, totalBufferSize)) == NULL) 
	{
		fprintf(stderr, "Memory allocation error\n");
		ExitProcess(1);
	}
	blocks = (WAVEHDR*)buffer;
	buffer += sizeof(WAVEHDR) * count;
	for(int i = 0; i < count; i++) {
		blocks[i].dwBufferLength = size;
		blocks[i].lpData = (LPSTR)buffer;
		buffer += size;
	}

	return blocks;
}

void CWavePlay::freeBlocks(WAVEHDR* blockArray)
{
	HeapFree(GetProcessHeap(), 0, blockArray);
}

BOOL CWavePlay::OpenDev(int nSamplesPerSec, int nChannels, int nBitPerSample)
{
	WAVEFORMATEX wfmt;
	wfmt.nSamplesPerSec = nSamplesPerSec; //44100;
	wfmt.wBitsPerSample = nBitPerSample; //16
	wfmt.nChannels = nChannels; //2
	wfmt.cbSize = 0;
	wfmt.wFormatTag = WAVE_FORMAT_PCM;
	wfmt.nBlockAlign = (wfmt.wBitsPerSample * wfmt.nChannels) >> 3;
	wfmt.nAvgBytesPerSec = wfmt.nBlockAlign * wfmt.nSamplesPerSec;

	return Start(&wfmt);
}

BOOL CWavePlay::PlayData(unsigned char* buf, int size)
{
	PlayAudio((LPSTR)buf, size);
	return TRUE;
}

BOOL CWavePlay::CloseDev()
{
	return CloseDevice();
}