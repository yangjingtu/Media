#include "StdAfx.h"
#include "AACEncodeImpl.h"

CAACEncodeImpl::CAACEncodeImpl(void)
	: m_wfx()
	, m_aep()
	
	, m_lpCallback(NULL)
	, m_lpUserdata(NULL)

	, m_dwSampleCountIn(0)
	, m_dwFrameCountOut(0)
	, m_nFrmSampleCount(0)
	, m_nStreamSize(0)

	, m_pFaacEnc(NULL)
	, m_pInBuffer(NULL)
	, m_pOutBuffer(NULL)
	, m_pLeaveBuffer(NULL)
	, m_nLeaveSize(0)
{
}

CAACEncodeImpl::~CAACEncodeImpl(void)
{
	Uninitialize();
}

BOOL CAACEncodeImpl::Initialize()
{
	Uninitialize();
	memset(&m_wfx, 0, sizeof(WAVEFORMATEX));

	return TRUE;
}

BOOL CAACEncodeImpl::SetInputFormat(const WAVEFORMATEX& wfx)
{
	m_wfx					= wfx;
	m_wfx.nBlockAlign		= m_wfx.wBitsPerSample * m_wfx.nChannels / 8;

	switch(m_wfx.wBitsPerSample)
	{
	case 16:
	//case 24:
	case 32:
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL CAACEncodeImpl::SetOutpurForamt(const AACENCODEPARAM& aep)
{
	m_aep					= aep;
	return TRUE;
}

BOOL CAACEncodeImpl::SetCallback(AACENCODECALLBACK lpCallback, LPVOID lpUserdata)
{
	if(lpCallback == NULL)return FALSE;

	m_lpCallback			= lpCallback;
	m_lpUserdata			= lpUserdata;

	return TRUE;
}

BOOL CAACEncodeImpl::Start()
{
	if(m_wfx.wBitsPerSample != m_aep.nBitsPerSample)return FALSE;
	if(m_wfx.nSamplesPerSec != m_aep.nSamplingFreq)return FALSE;
	if(m_wfx.nChannels != m_aep.nChannel)return FALSE;

	m_dwSampleCountIn		= 0;
	m_dwFrameCountOut		= 0;

	unsigned long			inputSamples(0);
	unsigned long			maxOutputBytes(0);

	m_pFaacEnc				= faacEncOpen(m_aep.nSamplingFreq, m_aep.nChannel, &inputSamples, &maxOutputBytes);
	if (m_pFaacEnc == NULL) return  FALSE;

	faacEncConfigurationPtr config(faacEncGetCurrentConfiguration(m_pFaacEnc));
	
	if(config->version != FAAC_CFG_VERSION) 
	{
		faacEncClose(m_pFaacEnc);
		return FALSE;
	}

	config->aacObjectType	= m_aep.eAACType;
	config->mpegVersion		= m_aep.eMpegVersion;
	config->bitRate			= m_aep.nBitRate / m_aep.nChannel;	//bits/second per channel
	config->allowMidside	= 1;
	config->useLfe			= 1;
	config->bandWidth		= m_aep.nSamplingFreq / 2;					//采样频率是带宽的2倍
	config->outputFormat	= m_aep.eOutputFormat;						//默认为ADTS
	switch(m_aep.nBitsPerSample) 
	{
	case 16:
		config->inputFormat = FAAC_INPUT_16BIT;
		break;
	//case 24:   
	//	config->inputFormat = FAAC_INPUT_24BIT; //faac 不支持 24bits
	//	break;
	case 32:
		config->inputFormat = FAAC_INPUT_32BIT;
		break;
	default:
		return FALSE;
	}

	int						nInputSize(inputSamples * m_aep.nBitsPerSample / 8);
	int						nRet(faacEncSetConfiguration(m_pFaacEnc,config));
	if(nRet == 0)return FALSE;

	m_nFrmSampleCount		= inputSamples;
	m_nStreamSize			= maxOutputBytes;

	m_pLeaveBuffer			= new BYTE[nInputSize];
	m_pInBuffer				= new BYTE[nInputSize];
	m_pOutBuffer			= new BYTE[maxOutputBytes];
	m_nLeaveSize			= 0;

	return nRet;
}

BOOL CAACEncodeImpl::EncodeBuffer(const BYTE* const lpData, const int nSize)
{
	if(lpData == NULL || nSize <= 0)return FALSE;
	if(m_pFaacEnc == NULL)return FALSE;

	int							nFrameSize(m_nFrmSampleCount * m_aep.nBitsPerSample / 8);
	int							count((nSize + m_nLeaveSize) / nFrameSize);
	int							nCurPos(0);

	if (count == 0)
	{
		memcpy(m_pLeaveBuffer + m_nLeaveSize, lpData, nSize);
		m_nLeaveSize			+= nSize;
		return TRUE;
	}

	m_dwSampleCountIn			+= nSize / (m_wfx.wBitsPerSample / 8);
	while(count-- > 0)
	{
		//拷贝数据
		if (m_nLeaveSize > 0)
		{
			memcpy(m_pInBuffer, m_pLeaveBuffer, m_nLeaveSize);
			memcpy(m_pInBuffer + m_nLeaveSize, lpData + nCurPos, nFrameSize - m_nLeaveSize);
			nCurPos				+= (nFrameSize - m_nLeaveSize);
			m_nLeaveSize		= 0;
		}
		else
		{
			memcpy(m_pInBuffer, lpData + nCurPos, nFrameSize);
			nCurPos				+= nFrameSize;
		}

		int						nBytes(faacEncEncode(m_pFaacEnc, (int32_t*)m_pInBuffer, m_nFrmSampleCount, m_pOutBuffer, m_nStreamSize));

		if (nBytes > 0)
		{
			if(m_lpCallback)m_lpCallback(m_pOutBuffer, nBytes, m_nFrmSampleCount, m_lpUserdata);
			m_dwFrameCountOut++;
		}
	}

	m_nLeaveSize				= nSize - nCurPos;
	if(m_nLeaveSize)memcpy(m_pLeaveBuffer, lpData + nCurPos, m_nLeaveSize);

	return TRUE;
}

BOOL CAACEncodeImpl::Stop()
{
	DWORD						dwFrameCountIn((m_dwSampleCountIn + m_nFrmSampleCount - 1 ) / m_nFrmSampleCount);
	
	if(m_pFaacEnc == NULL)return FALSE;
	while (m_dwFrameCountOut < dwFrameCountIn)
	{
		int						nFrameSize(m_nFrmSampleCount * m_aep.nBitsPerSample / 8);
		
		memset(m_pInBuffer, 0, nFrameSize);
		if(m_nLeaveSize > 0)
		{
			memcpy(m_pInBuffer, m_pLeaveBuffer, m_nLeaveSize);
			m_nLeaveSize		= 0;
		}

		int						nBytes(faacEncEncode(m_pFaacEnc, (int32_t*)m_pInBuffer, m_nFrmSampleCount, m_pOutBuffer, m_nStreamSize));
		if (nBytes > 0)
		{
			if(m_lpCallback)m_lpCallback(m_pOutBuffer, nBytes, m_nFrmSampleCount, m_lpUserdata);
			m_dwFrameCountOut++;
		}
		else
		{
			break;
		}
	}

	if (m_pFaacEnc != NULL)
	{
		faacEncClose(m_pFaacEnc);
		m_pFaacEnc = NULL;
	}

	return TRUE;
}

void CAACEncodeImpl::Uninitialize()
{
	if (m_pFaacEnc)faacEncClose(m_pFaacEnc);
	if (m_pInBuffer)delete[] m_pInBuffer;
	if (m_pOutBuffer)delete[] m_pOutBuffer;
	if (m_pLeaveBuffer)delete[] m_pLeaveBuffer;

	m_pFaacEnc = NULL;
	m_pInBuffer				= NULL;
	m_pOutBuffer			= NULL;
	m_pLeaveBuffer			= NULL;
}

BOOL CAACEncodeImpl::Requantitize(WAVEFORMATEX* pSrcWfx, BYTE* pSrcBuf, int nSrcBufLen, WAVEFORMATEX* pDstWfx, BYTE** pDstBuf, int& nDstBufLen)
{
	int						nOutLen;
	int						nCount;
	int						nBlock;

	if (pDstWfx->wBitsPerSample == 32)
	{
		if (pSrcWfx->wBitsPerSample == 16)
		{
			nOutLen			= nSrcBufLen * 2;
			nCount			= nSrcBufLen / 2; 
			nBlock			= 2;
		}
		if (pSrcWfx->wBitsPerSample == 24 || pSrcWfx->wBitsPerSample == 20)
		{
			nOutLen				= nSrcBufLen / 3 * 4;
			nCount				= nSrcBufLen / 3;
			nBlock				= 3;
		}

		if(*pDstBuf == NULL)
		{
			*pDstBuf			= new BYTE[nOutLen];
		}

		memset(*pDstBuf, 0, nOutLen);

		for (int i = 0; i < nCount; i++)
		{
			memcpy(*pDstBuf + i * 4 + 4 - nBlock, pSrcBuf + i * nBlock, nBlock);
		}

		nDstBufLen = nOutLen;
		return TRUE;
	}

	if (pDstWfx->wBitsPerSample == 16)
	{
		if (pSrcWfx->wBitsPerSample == 32)
		{
			nOutLen				= nSrcBufLen / 2;
			nCount				= nSrcBufLen / 4; 
			nBlock				= 4;
		}
		if (pSrcWfx->wBitsPerSample == 24 || pSrcWfx->wBitsPerSample == 20)
		{
			nOutLen				= nSrcBufLen / 3 * 2;
			nCount				= nSrcBufLen / 3;
			nBlock				= 3;
		}

		if(*pDstBuf == NULL)
		{
			*pDstBuf			= new BYTE[nOutLen];
		}

		memset(*pDstBuf, 0, nOutLen);

		for (int i = 0; i < nCount; i++)
		{
			memcpy(*pDstBuf + i * 2, pSrcBuf + i * nBlock + nBlock - 2, 2);
		}

		nDstBufLen				= nOutLen;
		return TRUE;
	}

	return FALSE;
}
