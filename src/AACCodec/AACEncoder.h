#pragma once

#include "..\..\include\faac\faac.h"
//#include "F:\software\03-code\ffmpeg\lib\faac\include\faaccfg.h"
#include <windows.h>
#include <MMSystem.h>

#define AACDECODEC

#ifdef AACDECODEC
#define AACDECODEC_API __declspec(dllexport)
#else
#define AACDECODEC_API __declspec(dllimport)
#endif

typedef void (CALLBACK *AACENCODECALLBACK)(LPBYTE lpEncBuffer, int nByteCount, int nSampleCount, LPVOID lpUserdata);
struct AACENCODEPARAM
{
	enum MPEGVersion{_MPEG2 = 1, _MPEG4 = 2};
	enum AACType{Main = 1, Low = 2, _SSR = 3, Lip = 4};
	enum OutputFormat{Raw = 0, Adts = 1};

	AACENCODEPARAM()
		: eMpegVersion(_MPEG4)
		, eAACType(Low)
		, nBitRate(128000)
		, nChannel(2)
		, nSamplingFreq(48000)
		, nQuantizer(100)
		, eOutputFormat(Adts)
		, nBitsPerSample(16)
	{
	}

	MPEGVersion		eMpegVersion;      // 1:mpeg 2 , 0:mpeg 4
	AACType			eAACType;          // AAC object type 
	int				nBitRate;          // bitrate 
	int				nChannel;          // channel
	int				nSamplingFreq;     // frequency  
	int				nQuantizer;        // Quantizer quality 
	OutputFormat	eOutputFormat;     // Bitstream output format (0 = Raw; 1 = ADTS)
	int				nBitsPerSample;    //
};

class AACDECODEC_API CAACEncoder
{
public:
	CAACEncoder(void);
	virtual ~CAACEncoder(void);

public:
	static CAACEncoder*		CreateEncoder();
	static void				DeleteEncoder(CAACEncoder* decoder);

public:
	virtual BOOL			Initialize(void) = 0;
	virtual BOOL			SetInputFormat(const WAVEFORMATEX& wfx) = 0;
	virtual BOOL			SetOutpurForamt(const AACENCODEPARAM& aep) = 0;
	virtual BOOL			SetCallback(AACENCODECALLBACK lpCallback, LPVOID lpUserdata) = 0;
	virtual BOOL			Start() = 0;
	virtual BOOL			EncodeBuffer(const BYTE* const lpData, const int nSize) = 0;
	virtual BOOL			Stop() = 0;
};
