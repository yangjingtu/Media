#include "StdAfx.h"
#include "AACDecodeImpl.h"

CAACDecodeImpl::CAACDecodeImpl(void)
	: m_hFaadDec(NULL)
	, m_lpInfo(NULL)
	, m_pInLeftBuffer(NULL)
	, m_pOutBuffer(NULL)
	, m_nOutBUfferLen(0)
	, m_lStreamSize(0)
	, m_lSampleRate(0)
{
}

CAACDecodeImpl::~CAACDecodeImpl(void)
{
	Uninitialize();
}

BOOL CAACDecodeImpl::Initialize()
{
	Uninitialize();
	return TRUE;
}

BOOL CAACDecodeImpl::StartDecode(BYTE *pBuffer, int nBufLen)
{
	if (pBuffer == NULL)
	{
		//TRACE("Input buffer error!\n");
		return FALSE;
	}

	// Check if decoder has the needed capabilities
	unsigned long				cap(NeAACDecGetCapabilities());

	// Open the library
	m_hFaadDec					= NeAACDecOpen();
	if (m_hFaadDec == NULL)
	{
		//TRACE("open aac decode error!\n");
		return FALSE;
	}

	// Get the current config
	NeAACDecConfigurationPtr	pAacDecConf(NeAACDecGetCurrentConfiguration(m_hFaadDec));

	if (pAacDecConf == NULL)
	{
		//TRACE("get configuration error!\n");
		return FALSE;
	}

// 	pAacDecConf->downMatrix			= 0;
// 	pAacDecConf->useOldADTSFormat	= 1;
// 	NeAACDecSetConfiguration(m_hFaadDec, pAacDecConf);

	unsigned long					iSampleRate(44100);
	unsigned char					iChannelNum(1);
	unsigned char					iHeaderType(0);
	unsigned char					iMpegVersion(0);
	long							iByteInitConsume(0); //初始化用到的字节数
	long							iDataRate(0);//数据率

	iByteInitConsume				= NeAACDecInit(m_hFaadDec, pBuffer, nBufLen, &iSampleRate, &iChannelNum);
	if (iByteInitConsume < 0)
	{
		//TRACE("aac decode initial error!\n");
		return FALSE;
	}

	m_lpInfo						= new AAC_DECODEINFO;
	m_lpInfo->iAACType				= pAacDecConf->defObjectType;
	if (pAacDecConf->outputFormat == FAAD_FMT_16BIT)
		m_lpInfo->iBitsPerSample	= 16;
	else if (pAacDecConf->outputFormat == FAAD_FMT_24BIT)
		m_lpInfo->iBitsPerSample	= 24;
	else if (pAacDecConf->outputFormat == FAAD_FMT_32BIT)
		m_lpInfo->iBitsPerSample	= 32;

	m_lpInfo->iSampFreq				= pAacDecConf->defSampleRate;
	m_lpInfo->iChannel				=  iChannelNum;
	m_lpInfo->iDataRate				= m_lpInfo->iSampFreq * m_lpInfo->iBitsPerSample * m_lpInfo->iChannel / 8;
	m_lpInfo->iHeaderType			= iHeaderType;
	m_lpInfo->iMpegVersion			= iMpegVersion;
	iDataRate						= m_lpInfo->iDataRate;	


	NeAACDecFrameInfo				hFrameInfo;
	unsigned long					iLenDecodeOut(0);

	// Decode the frame in pBuffer
	m_pOutBuffer					=(BYTE*)NeAACDecDecode(m_hFaadDec, &hFrameInfo, pBuffer, nBufLen);
	m_lSampleRate					= hFrameInfo.samplerate;
	m_nOutBUfferLen					= hFrameInfo.samples * m_lpInfo->iChannel;

	return TRUE;
}

BOOL CAACDecodeImpl::GetOutputBufferFormat(WAVEFORMATEX& wfx)
{
	if(m_lpInfo == NULL)return FALSE;

	wfx.wFormatTag					= 1;
	wfx.nChannels					= m_lpInfo->iChannel;
	wfx.wBitsPerSample				= m_lpInfo->iBitsPerSample;
	wfx.nSamplesPerSec				= m_lSampleRate;
	wfx.nBlockAlign					= wfx.wBitsPerSample * wfx.nChannels / 8;
	wfx.nAvgBytesPerSec				=  wfx.nChannels *  wfx.nSamplesPerSec * wfx.wBitsPerSample / 8;
	wfx.cbSize						= 0;

	return TRUE;
}

BOOL CAACDecodeImpl::DecodeBuffer(BYTE *pBufferIn, int nBufLenIn, BYTE **pBufferOut, int *nBufLenOut)
{
	if ((nBufLenIn == NULL) || (nBufLenIn < 0))
	{
		//TRACE("set input buffer error!\n");
		return FALSE;
	}

	NeAACDecFrameInfo			hFrameInfo;
	unsigned long				iLenDecodeOut(0);

	m_pOutBuffer				= (BYTE*)NeAACDecDecode(m_hFaadDec, &hFrameInfo, pBufferIn, nBufLenIn);
	m_nOutBUfferLen				= hFrameInfo.samples * m_lpInfo->iChannel;
	m_lSampleRate				= hFrameInfo.samplerate;

	if (hFrameInfo.error != 0)return FALSE;
	
	m_lStreamSize				+= iLenDecodeOut;
	*pBufferOut					= m_pOutBuffer;
	*nBufLenOut					= m_nOutBUfferLen;

	return TRUE;
}

BOOL CAACDecodeImpl::Uninitialize()
{
	if (m_hFaadDec != NULL)
	{	
		NeAACDecClose(m_hFaadDec);
		m_hFaadDec				= NULL;	
	}
	m_nOutBUfferLen				= 0;
	if (m_pInLeftBuffer != NULL) 
	{
		delete[] m_pInLeftBuffer;
		m_pInLeftBuffer			= NULL;
	}
	if (m_lpInfo != NULL)
	{
		delete m_lpInfo;
		m_lpInfo				= NULL;
	}

	return 1;
}
