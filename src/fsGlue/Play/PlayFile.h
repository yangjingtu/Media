#pragma once
#include "../../wellMedia/CommDataDef.h"
#include "PlayHelper.h"
#include "../../wellNet/FileMng/FileObj.h"


#define  MAX_FRAME_LEN (1920*1080 + sizeof(Frame_t))

DWORD WINAPI RecordPlay(LPVOID lParam);

class CPlayFile : public CPlayHelper
{
	friend DWORD WINAPI RecordPlay(LPVOID lParam);
public:
	CPlayFile(CDDrawView* ddraw, short* nWndId);
	virtual ~CPlayFile(void);

	virtual bool Open(const char* url, short nType);
	virtual bool Close();

	virtual bool Reverse(short nSpeed);
	virtual bool Forward(short nSpeed);
	virtual bool PreFrame();
	virtual bool Seek(long pos, long min, long max);

protected:
	void PlayLocalAvi(const char* fileName);

	DWORD _RecordPlay();

private:
	void Init();
	void UnInit();

	int openAVIFile(TCHAR* fileName);
	int closeAVIFile();

private:
	int m_videotype;
	HANDLE			mhThread;
	DWORD			miThreadID;

	bool			m_bThreadRun;

	CFileObj*		m_pFileObj;
};

