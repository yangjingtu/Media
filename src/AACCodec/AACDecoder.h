#pragma once
#include <MMSystem.h>

#define AACDECODEC

#ifdef AACDECODEC
#define AACDECODEC_API __declspec(dllexport)
#else
#define AACDECODEC_API __declspec(dllimport)
#endif

class AACDECODEC_API CAACDecoder
{
protected:
	CAACDecoder(void);
	virtual ~CAACDecoder(void);

public:
	static CAACDecoder*		CreateDecoder();
	static void				DeleteDecoder(CAACDecoder* decoder);

public:
	virtual BOOL			Initialize() = 0;
	virtual BOOL			StartDecode(BYTE *pBuffer, int nBufLen) = 0; 
	virtual BOOL			GetOutputBufferFormat(WAVEFORMATEX& wfx) = 0;
	virtual BOOL			DecodeBuffer(BYTE *pBufferIn, int nBufLenIn, BYTE **pBufferOut, int *nBufLenOut) = 0;	
	virtual BOOL			Uninitialize() = 0;
};
