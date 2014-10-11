#pragma once
#include "Audio.h"

#define OUT_WAVEHDRS 10
class CWaveOut
{
public:
	CWaveOut(void);
	virtual ~CWaveOut(void);

	HWAVEOUT GetHandle();
	
	void SetLastMMError(MMRESULT mmr);
	MMRESULT GetLastMMError();
	

	static void CALLBACK waveOutProc( HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD lParam, DWORD wParam );
	virtual BOOL OpenDev();
	virtual BOOL PlayData(unsigned char* buf, int size);
	virtual BOOL CloseDev();
	virtual BOOL SetPlayParam( PARAMTYPE type, LPVOID pData );
	
	operator HWAVEOUT() const;

protected:
	void ReleaseBuf();

protected:

	MMRESULT m_mmr;

	HWAVEOUT m_hWaveOut;
	WAVEFORMATEX	m_wavformex;
	CRITICAL_SECTION  m_critical;

private:
	PWAVEHDR	pWaveOutHdr[OUT_WAVEHDRS];
	PBYTE		pWaveOutBuf[OUT_WAVEHDRS];
	bool		m_bPause;
	int			decode_sample;
	int			decode_index;
	int			m_iWaveHdrSize;
};
