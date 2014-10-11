#include "Encoder.h"
static int index_adjust[8] = {-1,-1,-1,-1,2,4,6,8};

static int step_table[89] = 
{
	7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,
	50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,
	408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,
	2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,
	10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767
};

CEncoder::CEncoder(void)
{
	m_enBufLength = 0;
	m_enBuf = NULL;
}

CEncoder::~CEncoder(void)
{
}

bool CEncoder::SetEncoder(int type)
{
	switch (type)
	{
	case DCD_ENCODE_IMAADPCM:
		m_enBufLength = 160;
		m_enBuf = (char*)malloc(m_enBufLength);
		ImaAdpcmInitEncode(0, 0);
		GetAUData = &CEncoder::PCM2ADPCM;
		break;
	case DCD_ENCODE_G726:
		m_enBufLength = 120;
		m_enBuf = (char*)malloc(m_enBufLength);
		GetAUData = &CEncoder::PCM2G726;
		break;
#ifdef USE_HSSDK
	case DCD_ENCODE_HIG726:

		HI_RESULT Ret;
		Ret =HI_VOICE_EncReset (&g_sG726DecState, MEDIA_G726_16KBPS);
		if (Ret != 0)
			return false;

		m_enBufLength = 140;
		m_enBuf = (char*)malloc(m_enBufLength);
		GetAUData = &CEncoder::PCM2HIG726;
		break;
#endif
	}
	return true;
}

void CEncoder::UnRegisterEncoder(int type)
{
	switch (type)
	{
	case DCD_ENCODE_IMAADPCM:
	case DCD_ENCODE_G726:
		m_enBufLength = 0;
		if (m_enBuf)
		{
			free(m_enBuf);
			m_enBuf = NULL;
		}
		break;
#ifdef USE_HSSDK
	case DCD_ENCODE_HIG726:
		m_enBufLength = 0;
		if (m_enBuf)
		{
			free(m_enBuf);
			m_enBuf = NULL;
		}
		break;
#endif
	}
}

void CEncoder::ImaAdpcmInitEncode(int sample, int index)
{
	_en_sample = sample;
	_en_index = index;
}

void CEncoder::ImaAdpcmEncode(const unsigned char * raw, int len, unsigned char * encoded)
{
	short * pcm = (short *)raw;
	int cur_sample;
	int i;
	int delta;
	int sb;
	int code;
	len >>= 1;

	for (i = 0;i < len;i ++)
	{
		cur_sample = pcm[i]; // 得到当前的采样数据
		delta = cur_sample - _en_sample; // 计算出和上一个的增量
		if ( delta < 0 )
		{
			delta = -delta;
			sb = 8;
		}
		else 
		{
			sb = 0;
		}	// sb 保存的是符号位
		code = 4 * delta / step_table[_en_index];	// 根据 steptable[]得到一个 0-7 的值
		if (code>7) 
			code=7;	// 它描述了声音强度的变化量

		delta = (step_table[_en_index] * code) / 4 + step_table[_en_index] / 8;	// 后面加的一项是为了减少误差
		if (sb) 
			delta = -delta;
		_en_sample += delta;
		if (_en_sample > 32767)
			_en_sample = 32767;
		else if (_en_sample < -32768)
			_en_sample = -32768;

		_en_index += index_adjust[code];
		if (_en_index < 0) 
			_en_index = 0;
		else if (_en_index > 88) 
			_en_index = 88;

		if (i & 0x01)
			encoded[i >> 1] |= code | sb;
		else
			encoded[i >> 1] = (code | sb) << 4;
	}
}

char* CEncoder::GetAUBuffer(char* data, int lenght, int& newLength)
{
	return (this->*GetAUData)(data, lenght, newLength);
}

char* CEncoder::PCM2ADPCM(char* data, int lenght, int& newLength)
{
	//assert(lenght == 640);
	ImaAdpcmEncode((unsigned char*)data, lenght, (unsigned char*)m_enBuf);
	newLength = 160;//lenght/4;
	return m_enBuf;
}

char* CEncoder::PCM2G726(char* data, int lenght, int& newLength)
{
	assert(lenght == 960);
	g726_Encode((unsigned char*)data, m_enBuf);
	newLength = 120;
	return m_enBuf;
}

#ifdef USE_HSSDK
char* CEncoder::PCM2HIG726(char* data, int lenght, int& newLength)
{
	assert(lenght == 960);	//320个包一组 共 3 个包 , 编码后 单个包 大小  44
// 	for(int i=0; i<3; i++)
// 		HI_VOICE_EncodeFrame(&g_sG726DecState, (short*)data + 160*i, (short*)(m_enBuf + 44*i), 160);
// 	newLength = 132;

	HI_VOICE_EncodeFrame(&g_sG726DecState, (short*)data, (short*)m_enBuf, 480);
	newLength = 124;
	return m_enBuf;
}
#endif