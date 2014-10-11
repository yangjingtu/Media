#pragma once
#include "stdafx.h"

class CWavePlay
{
public:
	CWavePlay(void);
	virtual ~CWavePlay(void);

	BOOL OpenDev(int nSamplesPerSec, int nChannels, int nBitPerSample);
	BOOL PlayData(unsigned char* buf, int size);
	BOOL CloseDev();

protected:
	bool Start(PWAVEFORMATEX pWavHead);
	void PlayAudio(LPSTR data, int size);
public:
	void WaveMsg(HWAVEOUT hwo, UINT uMsg, DWORD dwParam1, DWORD dwParam2);

private:
	bool m_bPalyState;
private:
	CRITICAL_SECTION _waveCriticalSection;
	HWAVEOUT m_hWaveOut;
	WAVEFORMATEX	m_wavformex;
	WAVEHDR*         _waveBlocks;	
	int              _nWaveCurrentBlock;
	volatile int     _nWaveFreeBlockCount;	
protected:
	bool CloseDevice();
	WAVEHDR* allocateBlocks(int size, int count);
	void freeBlocks(WAVEHDR* blockArray);
};

