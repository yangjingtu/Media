#pragma once
#include "stdafx.h"
#include "g726/g726.h"
#ifdef USE_HSSDK
#include "hssdk/hi_voice_api.h"
#pragma comment(lib, "lib_VoiceEngine_dll.lib")
#endif

class CEncoder
{
public:
	CEncoder(void);
	~CEncoder(void);

	bool SetEncoder(int type);
	void UnRegisterEncoder(int type);

	char* GetAUBuffer(char* data, int lenght, int& newLength);
protected:
	char* (CEncoder::*GetAUData)(char* data, int lenght, int& newLength);

	char* PCM2ADPCM(char* data, int lenght, int& newLength);
	char* PCM2G726(char* data, int lenght, int& newLength);

	void ImaAdpcmInitEncode(int sample, int index);
	void ImaAdpcmEncode(const unsigned char * raw, int len, unsigned char * encoded);

#ifdef USE_HSSDK
	char* PCM2HIG726(char* data, int lenght, int& newLength);
#endif

private:
	int		m_enBufLength;
	char*	m_enBuf;
	int		_en_sample;
	int		_en_index;

#ifdef USE_HSSDK
		hiVOICE_G726_STATE_S g_sG726DecState;
#endif
};
