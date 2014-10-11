#pragma once
#include "PlayHelper.h"

class CNetOBjBase;

class CPlayNet : public CPlayHelper
{
public:
	CPlayNet(CDDrawView* ddraw, short* nWndId);
	virtual ~CPlayNet(void);

	virtual bool Open(const char* url, short nType);
	virtual bool Close();
	virtual bool Forward(short nSpeed);
	virtual bool Seek(long pos, long min, long max);
	virtual bool Reverse(short nSpeed);
	virtual bool NextFrame();
	virtual bool Pause();

private:
	void Init();
	void UnInit();

	void SkipIFrame(bool isSkip);

	void InitServPort();
private:
	CNetOBjBase* m_pNet;
	ADP::RspStreamOpen_t m_NetRlt;
	bool m_isSkipIFrame;

	int m_Ports[5];
};

