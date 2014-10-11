/***********************************************************************************************************************
Copyright:  
FileName: Trash.cpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "AviObj.h"

CAviObj::CAviObj()
{
	m_fHandle     = NULL;
	m_bOpened     = false;
	m_vCurFrameNo = 0;
	m_vStreamId   = 0;
	m_aStreamId   = 0;
	m_vLastPts    = 0;
	m_RdMode      = 0;
}

CAviObj::~CAviObj()
{
	Close();
}

S32 CAviObj::Open(string Name, S32 Mode)
{
	S32 rVal;
	if (m_bOpened)
	{
		DEBUG_ERROR("The File obj is opened \n");
		return MW_FAILED;
	}
	
	m_fHandle = GMAVIOpen((S8 *)Name.c_str(), Mode,  0);
	if (!m_fHandle)
	{
		DEBUG_ERROR("GMAVIOpen open filed [%s]\n", Name.c_str());
		return MW_FAILED;
	}

	rVal = GMAVIGetAviMainHeader(m_fHandle, &m_AviMainHeader);
	if (GMSTS_OK != rVal)
	{
		DEBUG_ERROR("GMAVIGetAviMainHeader Failed [%d]\n", rVal);
		return MW_FAILED;
	}
	S32 StreamNum; 
	rVal = GMAVIGetStreamHeaderNum(m_fHandle, &StreamNum);
	if (GMSTS_OK != rVal)
	{
		DEBUG_ERROR("GMAVIGetStreamHeaderNum Failed [%d]\n", rVal);
		return MW_FAILED;
	}
	DEBUG_INFO("GMAVIGetStreamHeaderNum Num:%d", StreamNum);
	
	S32 StreamId;
	AviStreamHeader   StreamHeader;
	GmAviStreamFormat StreamFormat;
	for (S32 i = 0; i < StreamNum; i++)
	{
		rVal = GMAVIGetStreamHeader(m_fHandle, i + 1, &StreamHeader, &StreamFormat, &StreamId);
		if (GMSTS_OK != rVal)
		{
			DEBUG_ERROR("GMAVIGetStreamHeader[%d] Failed [%d]\n",i, rVal);
			continue;
		}
		S32 FccType;
		memcpy(&FccType, StreamHeader.fccType, sizeof(S32));
		if(FccType == GM_MAKE_FOURCC('v','i','d','s'))
		{
			m_vStreamId = StreamId;
			memcpy(&m_vAviStreamHeader, &StreamHeader, sizeof(m_vAviStreamHeader));
			memcpy(&m_vStreamFormat, &StreamFormat, sizeof(m_vStreamFormat));
		}
		else if(FccType == GM_MAKE_FOURCC('a','u','d','s'))
		{
			m_aStreamId = StreamId;
			memcpy(&m_aAviStreamHeader, &StreamHeader, sizeof(m_aAviStreamHeader));
			memcpy(&m_aStreamFormat, &StreamFormat, sizeof(m_aStreamFormat));
		} 
		else
		{
			DEBUG_ERROR("GMAVIGetStreamHeader[%d] unkown stream [%d]\n", i);
			continue;
		}
	}
	m_bOpened = true;
	GMAVIReset(m_fHandle, GMAVI_SECTION_STREAM_DATA);

	DEBUG_INFO("[%s][%d] Frames:%d FrameIntVal:%d \n", __FILE__, __LINE__,                                     \
				m_AviMainHeader.dwTotalFrames,                                                                 \
				m_AviMainHeader.dwMicroSecPerFrame / 1000                                                      \
			   );

	return MW_SUCC;
}

S32 CAviObj::Close()
{
	m_fHandle     = NULL;
	m_bOpened     = false;
	m_vCurFrameNo = 0;
	m_vStreamId   = 0;
	m_aStreamId   = 0;
	m_vLastPts    = 0;
	m_RdMode      = 0;
	if (m_fHandle)
	{
		GMAVIClose(m_fHandle);
	}
	
	return MW_SUCC;
}

S32 CAviObj::SetRdMode(S8 Mode)
{
	m_RdMode = Mode;
	return MW_SUCC;
}

S32 CAviObj::GetFrame(Frame_t *pFrame)
{
	if (!m_bOpened || !pFrame)
	{
		DEBUG_ERROR("<GetFrame> File not open or param err\n");
		return MW_FAILED;
	}
	S32  StreamId = 0;
	S32  IndxFlag = 0;
	S32  Pos      = 0;
	S32  NewPos   = 0;

	do {
		S32 rVal = GMAVIGetStreamDataAndIndex(m_fHandle, &StreamId,                \
			(U8*)pFrame->Data, &pFrame->DataLen, &IndxFlag,                        \
			NULL, NULL, 0,                                                         \
			0, &Pos, &NewPos                                                       \
			);
		if (GMSTS_OK != rVal)
		{
			DEBUG_ERROR("GMAVIGetStreamDataAndIndex failed rVal:%d\n", rVal);
			if (rVal == GMSTS_END_OF_DATA)
			{
				rVal = FILE_END_OF_DATA;
			}
			return rVal;
		}

		if(StreamId == m_vStreamId)
		{
			m_vCurFrameNo++;
			pFrame->Pts               = (m_vLastPts += m_AviMainHeader.dwMicroSecPerFrame / 1000);
		}

	} while(m_RdMode && (!IndxFlag || StreamId == m_aStreamId));

	pFrame->FrameTag = FRAME_TAG;
	pFrame->Id         = 0;
	pFrame->FrameNo    = m_vCurFrameNo; 
	pFrame->Time       = GET_TICKS(); 
	if (StreamId == m_vStreamId)
	{
		//m_vCurFrameNo++;
		switch (m_vStreamFormat.video_format.bmiHeader.biCompression)
		{
		case GMAVI_TYPE_H264:
			pFrame->EncodeType = ENC_H264;
			pFrame->FrameType  = IndxFlag == 1 ?  FRAME_I : FRAME_P;
			break;
		case GMAVI_TYPE_MPEG4:
			pFrame->EncodeType = ENC_MP4;
			pFrame->FrameType  = FRAME_M;
			break;
		case GMAVI_TYPE_MJPEG:
			pFrame->EncodeType = ENC_MJ;
			pFrame->FrameType  = FRAME_M;
			break;
		default:
			pFrame->EncodeType = ENC_H264;
			pFrame->FrameType  = IndxFlag == 1 ?  FRAME_I : FRAME_P;
			break;
		}
		//pFrame->Pts               = (m_vLastPts += m_AviMainHeader.dwMicroSecPerFrame / 1000);
		pFrame->Video.VBitRate    = m_AviMainHeader.dwMaxBytesPerSec * 8;
		pFrame->Video.VFrameRate  = m_vAviStreamHeader.dwRate /m_vAviStreamHeader.dwScale;
		pFrame->Video.VWidth      = m_AviMainHeader.dwWidth;
		pFrame->Video.VHeight     = m_AviMainHeader.dwHeight;
	}
	else if(StreamId == m_aStreamId)
	{
		pFrame->EncodeType = ENC_AAC;
		pFrame->FrameType  = FRAME_A;
		pFrame->Pts        = m_vLastPts;  
		pFrame->Audio.SampleRate    = m_aAviStreamHeader.dwRate /m_aAviStreamHeader.dwScale;
		pFrame->Audio.BitsPerSample = m_aStreamFormat.audio_format.wBitsPerSample;
	}
	else
	{
		DEBUG_ERROR("GMAVIGetStreamDataAndIndex StreamId:%d err\n", StreamId);
		return MW_FAILED;
	}

	return MW_SUCC;
}

S32 CAviObj::Seek(S32 FrameNo)
{
	if (!m_bOpened)
	{
		DEBUG_ERROR("[%s][%d]File not open \n", __FILE__, __LINE__);
		return MW_FAILED;
	}

	S32 Offset = 0;
	S32 rVal = GMAVISeekToFrame(m_fHandle, FrameNo, &Offset);
	if (GMSTS_OK != rVal)
	{
		DEBUG_ERROR("GMAVISeekToFrame failed rVal:%d\n", rVal);
		return MW_FAILED;
	}
	
	return (m_vCurFrameNo = FrameNo + Offset);
}

S32 CAviObj::GetFrames()
{
	return m_AviMainHeader.dwTotalFrames;
}

S32 CAviObj::GetIntVal()
{
	return m_AviMainHeader.dwMicroSecPerFrame / 1000;
}

S32 CAviObj::GetVHead(VideoHead_t &vHead)
{
	vHead.VBitRate   = m_AviMainHeader.dwMaxBytesPerSec * 8;
	vHead.VFrameRate = m_vAviStreamHeader.dwRate /m_vAviStreamHeader.dwScale;
	vHead.VWidth     = m_AviMainHeader.dwWidth;
	vHead.VHeight    = m_AviMainHeader.dwHeight;
	
	return MW_SUCC;
}