#include <WinSock2.h>
#include "PlayFile.h"
#include "../../wellNet/FileMng/AviObj.h"

CPlayFile::CPlayFile(CDDrawView* ddraw, short* nWndId)
	: CPlayHelper(ddraw, nWndId)
{
	Init();
}

CPlayFile::~CPlayFile(void)
{
	UnInit();
}

void CPlayFile::Init()
{
	m_videotype = 0;
	mhThread = NULL;
	miThreadID = 0;
	m_bThreadRun = false;

	m_nReverseSpeed = 1;
	m_nForwardSpeed = 1;

	m_pFileObj = new CAviObj;
}

void CPlayFile::UnInit()
{
	if(mhThread)
	{
		CloseHandle(mhThread);
		mhThread = NULL;
	}

	if(m_pFileObj)
	{
		delete m_pFileObj;
		m_pFileObj = NULL;
	}
}


int CPlayFile::openAVIFile(TCHAR* fileName)
{
	string strPath("");
//	strPath += "D:\\well\\00000000\\RECORD\\20140912\\20140912_152221.avi";
	strPath += fileName;
	return m_pFileObj->Open(strPath.c_str(), AVI_FILEMODE_READ);
}

int CPlayFile::closeAVIFile()
{
	return m_pFileObj->Close();
}

void CPlayFile::PlayLocalAvi(const char* fileName)
{
	if( 0 == openAVIFile((char*)fileName) )
	{
		m_bThreadRun = true;
		if (mhThread == NULL)
		{
			mhThread = CreateThread(NULL, NULL, RecordPlay, this, NULL, &miThreadID);
		}
		CloseHandle(mhThread);
		mhThread = NULL;
	}
}

DWORD WINAPI RecordPlay(LPVOID lParam)
{
	CPlayFile* self = (CPlayFile*)lParam;
	if(self)
	{
		return self->_RecordPlay();
	}
	return -1;
}

DWORD CPlayFile::_RecordPlay()
{	
	int ret = 0;
	char* frameBuffer = new char[MAX_FRAME_LEN];
	int frameLen = MAX_FRAME_LEN - sizeof(Frame_t);
	Frame_t *_frame = (Frame_t*)frameBuffer;
	while( m_bThreadRun )
	{
		_frame->DataLen = frameLen;
		ret = m_pFileObj->GetFrame(_frame);
		if( ret != 0 ) 
		{
			break;
		}

		if(_frame->FrameNo == 0)
		{
			if (m_hDecoder)
			{
				if (m_videotype != DCD_UNKNOW)
				{
					FS_UnRegister_Decoder(m_hDecoder, m_videotype);
				}
				FS_Release_Decoder(m_hDecoder);
			}
			m_hDecoder = FS_Create_Decoder();

			if (_frame->EncodeType == ENC_H264)
			{
				FS_Register_Decoder(m_hDecoder, DCD_H264, &CPlayHelper::VedioDecoderCallback, this);
				m_videotype = DCD_H264;
			}
			else
			{
				FS_Register_Decoder(m_hDecoder, DCD_MJPEG, &CPlayHelper::VedioDecoderCallback, this);
				m_videotype = DCD_MJPEG;
			}
		}

		CPlayHelper::_frameFunc(_frame, this);
	}

	if(frameBuffer)
	{
		delete[] frameBuffer;
		frameBuffer = 0;
	}
	return 0;
}

bool CPlayFile::Open(const char* url, short nType)
{
	PlayLocalAvi(url);
	return true;
}

bool CPlayFile::Close()
{
	closeAVIFile();
	m_bThreadRun = false;

	m_isPause = false;
	m_isPlayFrame = false;
	m_isNextFrame = false;
	m_isSnap = false;

	return CPlayHelper::Close();
}

bool CPlayFile::Reverse(short nSpeed)
{
	SetReverseSpeed(nSpeed);
	return false;
}

bool CPlayFile::Forward(short nSpeed)
{
	SetForwardSpeed(nSpeed);
	return false;
}

bool CPlayFile::PreFrame()
{
	return false;
}

bool CPlayFile::Seek(long pos, long min, long max)
{
	m_pFileObj->Seek(pos);
	return true;
}