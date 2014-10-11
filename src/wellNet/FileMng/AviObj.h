/***********************************************************************************************************************
Copyright:  
FileName: AviObj.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef FILEMNG_AVIOBJ_H_
#define FILEMNG_AVIOBJ_H_
#include <string>
#include "FileObj.h"
using namespace std;

class WELL_SDK CAviObj : public CFileObj
{
public:
	CAviObj();
	virtual ~CAviObj();

public:
	virtual S32 Open(const char* Name, S32 Mode);
	virtual S32 Close();
	virtual S32 SetRdMode(S8 Mode);
	virtual S32 GetFrame(Frame_t *pFrame);
	virtual S32 Seek(S32 FrameNo);
	virtual S32 GetFrames();
	virtual S32 GetIntVal();
	virtual S32 GetVHead(VideoHead_t &vHead);
	
public:
	Bool              m_bOpened;
	S32               m_vCurFrameNo;
	S64               m_vLastPts;
	HANDLE            m_fHandle;
	S32               m_vStreamId;
	S32               m_aStreamId;
	S8                m_RdMode;            //0:all; 1:key
	AviMainHeader     m_AviMainHeader;
	AviStreamHeader   m_vAviStreamHeader;
	AviStreamHeader   m_aAviStreamHeader;
	GmAviStreamFormat m_vStreamFormat;
	GmAviStreamFormat m_aStreamFormat;
};

#endif /*FILEMNG_AVIOBJ_H_*/