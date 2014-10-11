#include "StdAfx.h"
#include "Decoder.h"
#include "CriLock.h"
#include "../AACCodec/AACDecodeImpl.h"
#pragma comment(lib, "../../lib/AACCodec.lib")

CDecoder::CDecoder(void)
{
	_cvt = new MediaConvert;
	memset(&_frame, 0x0, sizeof(FRAME_PER));
	_height = 0;
	_width = 0;
	m_audioBuf = NULL;
	m_audioBufSize = 0;

	m_bResetDecord = false;
	InitializeCriticalSection(&m_cri);
	
	memset(&m_wfx, 0x0, sizeof(WAVEFORMATEX));
	m_nChannels = 0;
	m_wBitsPerSample = 0;
	m_nSamplesPerSec = 0;

	m_pAac = CAACDecoder::CreateDecoder();
}

CDecoder::~CDecoder(void)
{
	if (_cvt)
	{
		if (_cvt->IsVideoDevOpen()) {
			_cvt->VideoDecClose();
		}
		delete _cvt;
		_cvt = NULL;
	}

	if (_frame.frame)
	{
		free(_frame.frame);
		_frame.frame = NULL;
	}

	CAACDecoder::DeleteDecoder(m_pAac);

	DeleteCriticalSection(&m_cri);
}

void CDecoder::UnRegisterDecoder(int type)
{
	CCriLock lock(m_cri);

	switch (type)
	{
	case DCD_IMAADPCM:
		break;
	case DCD_H264:
		if (_frame.frame)
		{
			free(_frame.frame);
			_frame.frame = NULL;
		}
		memset(&_frame, 0x0, sizeof(FRAME_PER));
		m_videoCb = NULL;
		_width = _height = 0;
		break;
	case DCD_MJPEG:
		if (m_audioBuf)
		{
			free(m_audioBuf);
			m_audioBuf = NULL;
		}
		m_audioCb = NULL;
		m_audioBufSize = 0;
		break;
	case DCD_G726:
		break;
#ifdef USE_HSSDK
	case DCD_HIG726:
		if (m_audioBuf)
		{
			free(m_audioBuf);
			m_audioBuf = NULL;
		}
		m_audioCb = NULL;
		m_audioBufSize = 0;
		break;
#endif
	case DCD_AAC:
		if(m_pAac)
		{
			m_pAac->Uninitialize();
		}
		m_audioCb = NULL;
		m_audioBufSize = 0;

		memset(&m_wfx, 0x0, sizeof(WAVEFORMATEX));
		m_nChannels = 0;
		m_wBitsPerSample = 0;
		m_nSamplesPerSec = 0;
		break;
	}

}

bool CDecoder::ResetDecoder()
{
	m_bResetDecord = true;
	return true;
}

bool CDecoder::SetDecoder(int type, FSDECORD_CB cb, void* userdata)
{
	CCriLock lock(m_cri);

	m_userdata = userdata;

	switch (type)
	{
	case DCD_IMAADPCM:
		m_audioCb = cb;
		assert(_cvt);
		_cvt->ImaAdpcmInitDecode(0, 0);
		GetPCMData = &CDecoder::ADPCM2PCM;
		break;
	case DCD_H264:
		m_videoCb = cb;
		GetYUV420Data = &CDecoder::H2642YUV420;
		break;
	case DCD_MJPEG:
		m_videoCb = cb;
		GetYUV420Data = &CDecoder::MJPG2YUV420;
		break;
	case DCD_G726:
		m_audioCb = cb;
		GetPCMData = &CDecoder::G7262PCM;
		m_audioBufSize = 9600;
		m_audioBuf = (char*)malloc(m_audioBufSize);
		break;
		
	case DCD_PCM:
		{
			m_audioCb = cb;
			GetPCMData = &CDecoder::PCM2PCM;
			m_audioBufSize = 9600;
			m_audioBuf = (char*)malloc(m_audioBufSize);
		}
		break;
	case DCD_AAC:
		{
			m_audioCb = cb;
			GetPCMData = &CDecoder::AAC2PCM;
			if(m_pAac == NULL)
			{
				m_pAac = CAACDecoder::CreateDecoder();
			}
			assert(m_pAac != NULL);
			m_pAac->Initialize();
		}
		break;
	}

	return true;
	
}

bool CDecoder::SetResolution(int nWidth, int nHeight)
{
	_width = nWidth;
	_height = nHeight;
	return true;
}

char* CDecoder::H2642YUV420(char* data, int lenght)
{
	CCriLock lock(m_cri);

	if (m_bResetDecord)
	{
		m_bResetDecord = false;
		_width = _height = 0;
		_cvt->VideoInit();
	}

	int type = data[4] & 0x1f;	//解析SPS帧 7-sps  8-pps 5-I   1-P
	if (0x67 == data[4])//解析SPS帧 0x67 sps  0x68 pps 0x65 I
	{
		MediaConvert::ParseSPS((unsigned char*)&(data[5]), &_width, &_height);
	}

	if (_width != _cvt->VideoWidth() || _height != _cvt->VideoHeight()) //图像大小发生改变
	{
		//重启解码器
		if (_cvt->IsVideoDevOpen()) {
			_cvt->VideoDecClose();
		}

		_cvt->VideoDecOpen(CODEC_ID_H264, _width, _height, PIX_FMT_YUV420P);
		//填充要解码的帧头格式
		if (_frame.frame) {
			free(_frame.frame);
		}
		_frame.length = _cvt->VideoByteCount();
		_frame.frame = (char *)malloc(_frame.length);
		_frame.width = _width;
		_frame.height = _height;
		assert(_frame.frame);

		if (m_videoCb)
		{
			(*m_videoCb)(REBOOTDECODER, (void*)&_frame, m_userdata);
		}
	}

	if (_cvt->VideoWidth() != 0 && _cvt->VideoHeight() !=0 &&
		_cvt->VideoDecFrame((uint8_t*)data, lenght, (uint8_t*)_frame.frame)) {	//解码
			return (char*)_frame.frame;

	}
	return NULL;
}

char* CDecoder::MJPG2YUV420(char* data, int lenght)
{
	CCriLock lock(m_cri);

	MediaConvert::ParseJpeg((const uint8_t*)data, lenght, &_width, &_height);
	if (_width != _cvt->VideoWidth() || _height != _cvt->VideoHeight()) //图像大小发生改变
	{
		//重启解码器
		if (_cvt->IsVideoDevOpen()) {
			_cvt->VideoDecClose();
		}

		//解码库支持能力
		//if(!_cvt->VideoDecOpen(CODEC_ID_MJPEG, _width, _height, PIX_FMT_YUV420P)) return NULL;
		_cvt->VideoDecOpen(CODEC_ID_MJPEG, _width, _height, PIX_FMT_YUV420P);
		//填充要解码的帧头格式
		if (_frame.frame) {
			free(_frame.frame);
		}
		_frame.length = _cvt->VideoByteCount();
		_frame.frame = (char *)malloc(_frame.length);
		_frame.width = _width;
		_frame.height = _height;
		assert(_frame.frame);

		if (m_videoCb)
		{
			(*m_videoCb)(REBOOTDECODER, (void*)&_frame, m_userdata);
		}
	}

	if (_cvt->VideoWidth() != 0 && _cvt->VideoHeight() !=0 &&
		_cvt->VideoDecFrame((uint8_t*)data, lenght, (uint8_t*)_frame.frame)) {	//解码
			return (char*)_frame.frame;
	}
	return NULL;
}

char* CDecoder::ADPCM2PCM(char* data, int lenght, int& newLength)
{
	CCriLock lock(m_cri);

	newLength = lenght*4;
	if (newLength > m_audioBufSize)
	{
		free(m_audioBuf);
		m_audioBufSize = newLength;
		m_audioBuf = (char*)malloc(m_audioBufSize);
	}
	_cvt->ImaAdpcmDecode((unsigned char*)data, lenght, (unsigned char*)m_audioBuf);
	return m_audioBuf;
}

char* CDecoder::G7262PCM(char* data, int lenght, int& newLength)
{
	CCriLock lock(m_cri);

	newLength = lenght*8;
	if (newLength > m_audioBufSize)
	{
		free(m_audioBuf);
		m_audioBufSize = newLength;
		m_audioBuf = (char*)malloc(m_audioBufSize);
	}
	g726_Decode(data, (unsigned char*)m_audioBuf);
	return m_audioBuf;
}

char* CDecoder::PCM2PCM(char* data, int lenght, int& newLength)
{
	CCriLock lock(m_cri);

	if (m_audioBufSize < lenght)
	{
		free(m_audioBuf);
		m_audioBufSize = lenght;
		m_audioBuf = (char*)malloc(m_audioBufSize);
	}
	memcpy(m_audioBuf, data, lenght);
	newLength = lenght;
	return m_audioBuf;
}

char* CDecoder::GetYUV420Buffer(char* data, int lenght)
{
	CCriLock lock(m_cri);

	return (this->*GetYUV420Data)(data, lenght);
}

char* CDecoder::GetPCMBuffer(char* data, int lenght, int& newLength)
{
	CCriLock lock(m_cri);

	return (this->*GetPCMData)(data, lenght, newLength);
}

char* CDecoder::AAC2PCM(char* data, int length, int& newLength)
{
	assert(m_pAac != NULL);
	CCriLock lock(m_cri);

	if(m_bResetDecord)
	{
		m_bResetDecord = false;

		memset(&m_wfx, 0x0, sizeof(WAVEFORMATEX));
		m_nChannels = 0;
		m_wBitsPerSample = 0;
		m_nSamplesPerSec = 0;
	}
	
	if( (m_nChannels != m_wfx.nChannels || m_wBitsPerSample != m_wfx.wBitsPerSample || m_nSamplesPerSec != m_wfx.nSamplesPerSec)
		|| (m_nChannels == 0 && m_wBitsPerSample == 0 && m_nSamplesPerSec == 0) )
	{
		//重启解码器
		m_pAac->Initialize();
		bool bRlt = m_pAac->StartDecode((BYTE*)data, length);
		if(bRlt)
		{
			m_pAac->GetOutputBufferFormat(m_wfx);
			m_nChannels = m_wfx.nChannels;
			m_wBitsPerSample = m_wfx.wBitsPerSample;
			m_nSamplesPerSec = m_wfx.nSamplesPerSec;

			if(m_audioCb)
			{
				(*m_audioCb)(REBOOTDECODER, (void*)&m_wfx, m_userdata);
			}
		}
	}

	m_pAac->DecodeBuffer((BYTE*)data, length, (BYTE**)&m_audioBuf, &m_audioBufSize);
	newLength = m_audioBufSize;
	m_pAac->GetOutputBufferFormat(m_wfx);

	return m_audioBuf;
}