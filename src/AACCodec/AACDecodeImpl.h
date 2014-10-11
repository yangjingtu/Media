#pragma once
#include "aacdecoder.h"
#include "..\..\include\faad\neaacdec.h"
#pragma comment(lib, "..\\..\\lib\\faad\\libfaad.lib")
#pragma comment(lib, "..\\..\\lib\\faac\\libfaac.lib")


typedef struct AAC_DECODEINFO
{//aac解码重要信息，其中前三项可以外部配置，其余的从aac文件中获取
	unsigned char iAACType;				// AAC object type 
	unsigned char iBitsPerSample;		//
	unsigned long iSampFreq;			// frequency 

	unsigned char iMpegVersion;			// 1:mpeg 2 , 0:mpeg 4
	unsigned char iChannel;				// channel number
	unsigned char iHeaderType;			// 0 = Raw; 1 = ADIF; 2 = ADTS
	unsigned long iDataRate;			// Datarate 

}*LPAAC_DECODEINFO;

class AACDECODEC_API CAACDecodeImpl : public CAACDecoder
{
public:
	CAACDecodeImpl(void);
	virtual ~CAACDecodeImpl(void);

public:
	virtual BOOL			Initialize();
	virtual BOOL			StartDecode(BYTE *pBuffer, int nBufLen); 
	virtual BOOL			GetOutputBufferFormat(WAVEFORMATEX& wfx);
	virtual BOOL			DecodeBuffer(BYTE *pBufferIn, int nBufLenIn, BYTE **pBufferOut, int *nBufLenOut);	
	virtual BOOL			Uninitialize();

private:
	NeAACDecHandle				m_hFaadDec;
	LPAAC_DECODEINFO			m_lpInfo;//解码重要信息
	BYTE*						m_pInLeftBuffer;
	BYTE*						m_pOutBuffer;
	int							m_nOutBUfferLen;
	long						m_lStreamSize;
	long						m_lSampleRate;
};
