#pragma once
#include "../../Video/Video.h"
#include "../../AVDecoder/avdecodersdk.h"
#include "../../AUEncoder/avEncodersdk.h"
#include "../../wellMedia/CommDataDef.h"
#include "../../wellNet/Common/Frame.h"
#include "../../wellNet/Adapter/AdapterBase.h"
// #include "../../wellNet/wellNetSdk.h"
// typedef long long S64;
// typedef int S32;

#define RECT_CMP(rt1, rt2) (rt1.bottom == rt2.bottom && rt1.left == rt2.left && rt1.right == rt2.right && rt1.top == rt2.top)
#define RECT_SIZE_CMP(rt, sz) ( (rt.right - rt.left > sz) && (rt.bottom - rt.top > sz) )

typedef enum _EM_FILE_TYPE{
	EM_LOCAL_FILE = 0,
	EM_NET_FILE
}EM_FILE_TYPE;

class CPlayMgr;

class CPlayHelper
{
public:
	CPlayHelper(CDDrawView* ddraw, short* nWndid);
	virtual ~CPlayHelper(void);

	virtual bool Open(const char* url, short nType);
	virtual bool Close();

	virtual bool Play();
	virtual bool Stop();
	virtual bool Reverse(short nSpeed);
	virtual bool Forward(short nSpeed);
	virtual bool PreFrame();
	virtual bool NextFrame();
	virtual bool Pause();
	virtual bool Seek(long pos, long min, long max);
	virtual bool Snap(const char* savePath);
	virtual void GetPlayPos(long* curPos, long* nTotalFrames);

	short GetWndId();

	void SetParent(CPlayMgr* pParent);
protected:
	void PlayAudio(char* pcmdata, int len);
	bool PlayVideo(char* enVideoData, int len);

	virtual int GetVedioDecoderID() { return DCD_H264; }
	virtual int GetAudioDecoderID()	{ return DCD_AAC; }
	virtual int GetAudioEncoderID() { return DCD_ENCODE_G726; }
	
	virtual void SetResolution(int nWidth, int nHeight);

protected:
	static S32 frameFunc(Frame_t* pFram, Void* Param);
	static S32 msgCallFunc(const ADP::Msg_t& msg, Void* Param);
	static void VedioDecoderCallback(int msg, void *param, void *userdata);
	static void AudioDecoderCallback(int msg, void *param, void *userdata);

	S32 _frameFunc(Frame_t* pFram, Void* Param);
	S32 _msgCallFunc(const ADP::Msg_t& msg, Void* Param);

	//播放速度控制
	void SpeedCtrl(S64 nPts, DWORD dwStart);

	void SetReverseSpeed(short nSpeed);
	void SetForwardSpeed(short nSpeed);
	void ReSetPlaySpeed();
	void PauseResume();
	void PlayFrame();

	bool IsPlayingVideo() { return m_isPlayingVideo | m_isPlayFrame; }
	bool IsPlayingAudio();
	bool IsPlaying() {return m_isPlayingVideo | m_isPlayFrame | m_isPlayingAudio; }

private:
	void Init();
	void UnInit();

	void ResetParams();

protected:
	HAVDECODER m_hDecoder;									//解码对象
	
	//////////////////////////////////////////////////////////////////////////
	//指定父窗口的CDDrawView对象
	CDDrawView&  m_ddraw;

	//视频显示相关参数
	short* m_nWndId;													//窗口ID
	LPDIRECTDRAWSURFACE7 m_pDDYUV;		
	LPDIRECTDRAWSURFACE7 m_pDDYUVBack;

	RECT m_YUVRect;
	RECT m_DestRt;
	RECT m_rMid;
	//////////////////////////////////////////////////////////////////////////
	short	m_nMediaType;
	string m_strUrl;

	bool m_isOpen;
	bool m_isPause;	
	bool m_isPlayingVideo;
	bool m_isPlayingAudio;

	bool m_isPlayFrame;
	bool m_isNextFrame;

	bool m_isSnap;
	string m_strSaveSnapPath;

	int	 m_nReverseSpeed;
	int	 m_nForwardSpeed;

	S64 m_nPts;							//速度控制
	//////////////////////////////////////////////////////////////////////////
	CRITICAL_SECTION m_cri;

	CPlayMgr* m_pParent;

	long m_nTotalFrame;			//總幀數
	long m_nCurFrame;				//噹前幀號
};

