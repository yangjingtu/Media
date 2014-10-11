#include <WinSock2.h>
#include "PlayHelper.h"
#include "../Util/CriLock.h"
#include "../Image/Jpeg.h"
#include "../Image/Jpeg.h"

#ifdef _DEBUG
void XdPrint(_TCHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_TCHAR buf[256];
	_vstprintf(buf, format, args);
	OutputDebugString(buf);
	va_end(args);
}
#else
void XdPrint(_TCHAR* format, ...){}
#endif

CPlayHelper::CPlayHelper(CDDrawView* ddraw,  short* nWndid )
	: m_ddraw(*ddraw)
	, m_nWndId(nWndid)
{
	Init();
}

CPlayHelper::~CPlayHelper(void)
{
	UnInit();
}

void CPlayHelper::Init()
{
	INIT_LOCK;
	m_pDDYUV = NULL;
	m_pDDYUVBack = NULL;
	memset(&m_YUVRect, 0x0, sizeof(RECT));
	memset(&m_DestRt, 0x0, sizeof(RECT));
	memset(&m_rMid, 0x0, sizeof(RECT));

	//先创建和注册解码器,再打开网络
	m_hDecoder = FS_Create_Decoder();
	assert(m_hDecoder != NULL);	
	FS_Register_Decoder(m_hDecoder, GetVedioDecoderID(), &CPlayHelper::VedioDecoderCallback, this);
	FS_Register_Decoder(m_hDecoder, GetAudioDecoderID(), &CPlayHelper::AudioDecoderCallback, this);
	WRITELOG("FS_Register_Decoder");

	ResetParams();

	ReSetPlaySpeed();
	m_nPts = 0;

}

void CPlayHelper::UnInit()
{
	ResetParams();
	Sleep(50);

	if(m_hDecoder)
	{
		FS_UnRegister_Decoder(m_hDecoder, GetVedioDecoderID());
		FS_UnRegister_Decoder(m_hDecoder, GetAudioDecoderID());
		FS_Release_Decoder(m_hDecoder);
		m_hDecoder = NULL;
		WRITELOG("FS_Release_Decoder");
	}

	SafeRelease(m_pDDYUV);
	memset(&m_YUVRect, 0x0, sizeof(RECT));

	UNINIT_LOCK;
}

void CPlayHelper::SetResolution(int nWidth, int nHeight)
{
	FS_Reset_Decoder(m_hDecoder, nWidth, nHeight);
}

void CPlayHelper::ResetParams()
{
	LOCK;
	m_isOpen = false;
	m_isPause = false;
	m_isPlayFrame = false;
	m_isNextFrame = false;
	m_isPlayingVideo = false;
	m_isPlayingAudio = false;
	m_isSnap = false;
	m_nMediaType = EM_MEDIA_UNKNOW;
	m_nReverseSpeed = 1;
	m_nForwardSpeed = 1;
	m_nTotalFrame = 0;
	m_nCurFrame = 0;
}

short CPlayHelper::GetWndId()
{
	return (*m_nWndId);
}

bool CPlayHelper::PlayVideo(char* enVideoData, int len)
{
	if(m_hDecoder == NULL)
		return false;

	BYTE* yuv420 = (BYTE*)FS_GetYUV420Data(m_hDecoder, enVideoData, len);
	if(!yuv420 || m_YUVRect.bottom == 0 || m_YUVRect.right == 0 )
	{
		return false;
	}

	RECT rt = {0};
	RECT rc = {0};

	if(m_ddraw.PlayYUV(yuv420, yuv420 + m_YUVRect.bottom * m_YUVRect.right,  yuv420 + m_YUVRect.bottom * m_YUVRect.right * 5 / 4, (*m_nWndId), m_pDDYUV, m_YUVRect, rt))
	{
		if( !RECT_CMP(m_DestRt, rt) && RECT_SIZE_CMP(rt, 50))
		{
			SafeRelease(m_pDDYUVBack);
			int width = rt.right - rt.left;
			int height = rt.bottom - rt.top;
			m_pDDYUVBack = m_ddraw.CreatEmptySurface(width, height);
			//center
			m_rMid.top = m_rMid.left = 0;
			m_rMid.right = m_YUVRect.right;
			m_rMid.bottom = m_YUVRect.bottom;
			float bw = (float)width / m_YUVRect.right;//m_playView->m_widht/*1280*/;
			float bh = (float)height / m_YUVRect.bottom;//m_playView->m_height/*720*/;
			if (bw < bh)
			{
				int temp = bw * m_YUVRect.bottom;//m_playView->m_height;
				m_rMid.top = (height - temp)/2;
				m_rMid.left = 0;
				m_rMid.bottom = m_rMid.top + temp;
				m_rMid.right = width;
			}else{
				int temp = bh * m_YUVRect.right;//m_playView->m_widht;
				m_rMid.left = (width - temp)/2;
				m_rMid.top = 0;
				m_rMid.bottom =  height;
				m_rMid.right = m_rMid.left + temp;
			}
		}

		if(m_pDDYUVBack)
		{
			m_ddraw.Clrscr(m_pDDYUVBack);
			m_pDDYUVBack->Blt(&m_rMid, m_pDDYUV, NULL, DDBLT_WAIT, NULL);
			rc.bottom = rt.bottom - rt.top;
			rc.right = rt.right - rt.left;
			m_ddraw.DDsfDrawToDDBack(rt, m_pDDYUVBack, rc);
		}
	}

	//抓图
	if( m_isSnap )
	{
		Yuv420ToJpeg(yuv420, yuv420 + m_YUVRect.bottom * m_YUVRect.right,  yuv420 + m_YUVRect.bottom * m_YUVRect.right * 5 / 4, 
			m_YUVRect.right - m_YUVRect.left, m_YUVRect.bottom - m_YUVRect.top, m_strSaveSnapPath.c_str());

		//保存图像
		m_isSnap = false;
	}

	return true;
}

void CPlayHelper::PlayAudio(char* pcmdata, int len)
{
	if(pcmdata && len != 0 && m_nWndId)
	{
		m_pParent->AudioPlay((unsigned char*)pcmdata, len , *m_nWndId);
	}
}

void CPlayHelper::VedioDecoderCallback(int msg, void *param, void *userdata)
{
	CPlayHelper* self = (CPlayHelper*)userdata;
	switch (msg)
	{
	case REBOOTDECODER:
		{
			LPFRAME_P frame = (LPFRAME_P)param;
			if (self->m_YUVRect.bottom != frame->width && self->m_YUVRect.right != frame->height)
			{
				SafeRelease(self->m_pDDYUV);
				self->m_pDDYUV = self->m_ddraw.CreatOverlaySurface(frame->width, frame->height);
			}
			self->m_YUVRect.bottom  = frame->height;
			self->m_YUVRect.right = frame->width;
		}
		break;
	}
}

void CPlayHelper::AudioDecoderCallback(int msg, void *param, void *userdata)
{
	CPlayHelper* self = (CPlayHelper*)userdata;
	switch (msg)
	{
	case REBOOTDECODER:
		{
			PWAVEFORMATEX pWfx = (PWAVEFORMATEX)param;
			assert(pWfx != NULL);
			self->m_pParent->ResetAudio(pWfx);
		}
		break;
	}
}

S32 CPlayHelper::frameFunc(Frame_t* pFram, Void* Param)
{
	CPlayHelper* self = (CPlayHelper*)Param;
	if(!self || self->m_hDecoder == NULL)
		return -1;

	return self->_frameFunc(pFram, Param);
}

S32 CPlayHelper::msgCallFunc(const ADP::Msg_t& msg, Void* Param)
{
	CPlayHelper* self = (CPlayHelper*)Param;
	if(self)
	{
		return self->_msgCallFunc(msg, Param);
	}
	return 0;
}

S32 CPlayHelper::_frameFunc(Frame_t* pFram, Void* Param)
{
	while (!IsPlaying())
	{
		//Open后，堵住，点play后开始
		Sleep(3);	
	}

	//暂停和恢复
	PauseResume();

	//按帧播放PlayFrame();
	PlayFrame();

	m_nCurFrame = pFram->FrameNo;
	if(pFram->FrameType == FRAME_I || pFram->FrameType == FRAME_P)
	{
		if(IsPlayingVideo())
		{
			DWORD dwStart = GetTickCount();
			PlayVideo(pFram->Data, pFram->DataLen);
			if(!m_isPlayFrame)
			{
				SpeedCtrl(pFram->Pts - m_nPts, dwStart);
			}
		}
		m_nPts = pFram->Pts;
	}
	else if(pFram->FrameType == FRAME_A)
	{
		if(IsPlayingAudio())
		{
			char* audioBuf = NULL;
			int iAudioLen = 0;
			audioBuf = FS_GetPCMData(m_hDecoder, (char*)pFram->Data, pFram->DataLen, iAudioLen); 

			PlayAudio(audioBuf, iAudioLen);
		}
	}

	return 0;
}

S32 CPlayHelper::_msgCallFunc(const ADP::Msg_t& msg, Void* Param)
{
	switch (msg.Type)
	{
	default:
		break;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//Operate
//////////////////////////////////////////////////////////////////////////

bool CPlayHelper::Open(const char* url, short nType)
{
	LOCK;
	m_strUrl = url;
	m_nMediaType = nType;
	if(m_nMediaType == EM_MEDIA_UNKNOW)
	{
		if(m_strUrl.find("avi") != string::npos || m_strUrl.find("AVI") != string::npos)
		{
			m_nMediaType = EM_AV;
		}
		else if( m_strUrl.find("wav") != string::npos || m_strUrl.find("WAV") != string::npos)
		{
			m_nMediaType = EM_AUDIO;
		}
		else
		{
			m_nMediaType = EM_VIDEO;
		}
	}

	m_isOpen = true;

	return true;
}

bool CPlayHelper::Close()
{
	LOCK;
	if(!m_isOpen)
		return false;

	ResetParams();

	m_ddraw.DrawBorder();
	m_ddraw.ClearScreen();
	m_ddraw.Flip();

	return true;
}

bool CPlayHelper::Play()
{
	LOCK;
	if(m_isOpen == false)
		return false;

	if(m_nMediaType == EM_VIDEO)
	{
		m_isPlayingVideo = true;
	}
	else if(m_nMediaType == EM_AUDIO)
	{
		m_isPlayingAudio = true;
	}
	else if(m_nMediaType == EM_AV)
	{
		m_isPlayingAudio = true;
		m_isPlayingVideo = true;
	}
	else
	{
		m_isPlayingAudio = true;
		m_isPlayingVideo = true;
	}

	return true;
}

bool CPlayHelper::Stop()
{
	LOCK;
	return Close();
}

bool CPlayHelper::Reverse(short nSpeed)
{
	LOCK;
	SetReverseSpeed(nSpeed);
	return true;
}

bool CPlayHelper::Forward(short nSpeed)
{
	LOCK;
	SetForwardSpeed(nSpeed);
	return true;
}

bool CPlayHelper::PreFrame()
{	
	LOCK;
	m_isPlayFrame = true;
	m_isNextFrame = true;
	return true;
}

bool CPlayHelper::NextFrame()
{
	LOCK;
	m_isPlayFrame = true;
	m_isNextFrame = true;
	m_isPause = false;
	return true;
}
 
bool CPlayHelper::Pause()
{
	LOCK;
	m_isPause = !m_isPause;

	//从暂停状态变成恢复状态时，恢复正常播放
	if(m_isPause == false)
	{
		m_isPlayFrame = false;
	}
	
	ReSetPlaySpeed();

	return true;
}

bool CPlayHelper::Seek(long pos, long min, long max)
{
	return false;
}

bool CPlayHelper::Snap(const char* savePath)
{
	LOCK;
	m_isSnap = true;
	m_strSaveSnapPath = savePath;
	return true;
}

void CPlayHelper::SetReverseSpeed(short nSpeed)
{
	m_nReverseSpeed = nSpeed;
	m_nForwardSpeed = 1;
}

void CPlayHelper::SetForwardSpeed(short nSpeed)
{
	m_nForwardSpeed = nSpeed;
	m_nReverseSpeed = 1;
}

void CPlayHelper::ReSetPlaySpeed()
{
	m_nForwardSpeed = 1;
	m_nReverseSpeed = 1;
}

/************************************************************************/
//功能： 播放速度控制
//参数：
//		nOffPts [IN]		--		此帧和上一帧的间隔时间（ms)
//		dwStart [IN]		--		开始播放前的时间
/************************************************************************/
void CPlayHelper::SpeedCtrl(S64 nOffPts, DWORD dwStart)
{
	DWORD dwEnd = GetTickCount();
	XdPrint("nOffpts: %d, pts: %d\r\n", nOffPts,  dwEnd - dwStart);
	dwEnd = nOffPts - (dwEnd - dwStart);
	if(m_nForwardSpeed > 1)
	{
		//快放
		dwEnd = dwEnd / m_nForwardSpeed;
	}
	if(m_nReverseSpeed > 1)
	{
		//慢放
		dwEnd = dwEnd * m_nReverseSpeed;
	}

	if(dwEnd > 5 && dwEnd < 1000)	//过滤第一帧
	{
		Sleep(dwEnd);
	}
}

void CPlayHelper::PauseResume()
{
	if( m_isPause)
	{
		while( m_isPause)
		{
			Sleep(1);
		}
	}
}

void CPlayHelper::PlayFrame()
{
	if( m_isPlayFrame )
	{
		while (!m_isNextFrame)
		{
			if(!m_isPlayFrame)
			{
				break;
			}
			Sleep(3);
		}
		m_isNextFrame = false;
	}
}

void CPlayHelper::SetParent(CPlayMgr* pParent)
{
	assert(pParent);
	m_pParent = pParent;
}

bool CPlayHelper::IsPlayingAudio()
{
	short iwnd = m_pParent->GetDDrawView().GetSelectWndID();
	return (*m_nWndId == iwnd && m_isPlayingAudio);
}

void CPlayHelper::GetPlayPos(long* curPos, long* nTotalFrames)
{
	*curPos = m_nCurFrame;
	*nTotalFrames = m_nTotalFrame;
}