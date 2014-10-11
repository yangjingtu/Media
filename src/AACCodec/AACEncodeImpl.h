#pragma once
#include "aacencoder.h"

class CAACEncodeImpl : public CAACEncoder
{
public:
	CAACEncodeImpl(void);
	virtual ~CAACEncodeImpl(void);

public:
	BOOL			Initialize(void);
	BOOL			SetInputFormat(const WAVEFORMATEX& wfx);
	BOOL			SetOutpurForamt(const AACENCODEPARAM& aep);
	BOOL			SetCallback(AACENCODECALLBACK lpCallback, LPVOID lpUserdata);
	BOOL			Start();
	BOOL			EncodeBuffer(const BYTE* const lpData, const int nSize);
	BOOL			Stop();

protected:
	void			Uninitialize();
	BOOL				Requantitize(WAVEFORMATEX* pSrcWfx, BYTE* pSrcBuf, int nSrcBufLen, WAVEFORMATEX* pDstWfx, BYTE** pDstBuf, int& nDstBufLen);

private:
	WAVEFORMATEX		m_wfx;
	AACENCODEPARAM		m_aep;

	AACENCODECALLBACK	m_lpCallback;
	LPVOID				m_lpUserdata;

	DWORD				m_dwSampleCountIn;
	DWORD				m_dwFrameCountOut;
	int					m_nFrmSampleCount;
	int					m_nStreamSize;

	faacEncHandle		m_pFaacEnc;
	BYTE*				m_pInBuffer;
	BYTE*				m_pOutBuffer;
	BYTE*				m_pLeaveBuffer;
	int					m_nLeaveSize;
};
