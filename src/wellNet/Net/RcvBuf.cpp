/***********************************************************************************************************************
Copyright:  
FileName: RcvBuf.cpp   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/28/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "stdafx.h"
#include "RcvBuf.h"

CRcvBuf::CRcvBuf()
{
	m_DataLen  = 0;
	m_BufSize  = 0;
	m_ParsePos = NULL;
	m_Buf      = NULL;
	m_Start    = NULL;
	m_WrPos    = NULL;
}


CRcvBuf::~CRcvBuf()
{

}


Void CRcvBuf::Init(S32 hSize, S32 BufSize)
{
	if (hSize + 64 >= BufSize) 
	{
		exit(-1);
	}

	m_Buf = new S8[BufSize];
	if (NULL == m_Buf) 
	{
		exit(-1);
	}
	m_DataLen = 0;
	m_BufSize = BufSize - hSize;
	m_Start   = m_Buf + hSize;
	m_WrPos   = m_Start;
}

Void CRcvBuf::UnInit()
{
	if (m_Buf) 
	{
		delete []m_Buf;
		m_Buf = NULL;
	}
}


Void CRcvBuf::Clear()
{
	m_DataLen  = 0;
	m_WrPos    = m_Start;
	m_ParsePos = NULL;
}


S32 CRcvBuf::AddDataLen(S32 AddLen)
{
	if ((m_Start + m_BufSize - m_WrPos) < AddLen) 
	{
		return MW_FAILED;
	}

	m_DataLen += AddLen;
	m_WrPos   += AddLen;
	if (m_WrPos < m_Start || m_WrPos > m_Start + m_BufSize || m_DataLen < 0  || m_DataLen > m_BufSize) 
	{
		Clear();
	}
	m_Start[m_DataLen] = 0;
	
	return m_DataLen;
}


S32 CRcvBuf::GetFreeLen()
{
	S32 FreeSize = (m_Start + m_BufSize - m_WrPos);

	if (FreeSize > m_BufSize - 64) 
	{
		FreeSize = m_BufSize - 64;
	}

	return FreeSize;
}


S8 *CRcvBuf::GetWrPos()
{
	return m_WrPos;
}


S8 *CRcvBuf::FindKey(const S8* Key, S32 Len)
{
	S8 *Pos = NULL;
	if ( Key && Len < m_DataLen ) 
	{
		m_ParsePos = NULL;
		Pos = ::strstr(m_Start, Key, m_DataLen);
		if (NULL == Pos) 
		{
			memmove( m_Start, m_Start + (m_DataLen - ( Len  -1 ))  , Len - 1);
			m_DataLen = Len - 1;
			m_WrPos = m_Start + m_DataLen;
		} 
		else if(Pos != m_Start) 
		{
			m_DataLen -= (Pos - m_Start );
			m_WrPos   -= (Pos - m_Start );
			memmove( m_Start, Pos , m_DataLen );
			Pos = m_Start;
		} 
		else 
		{
			Pos = m_Start;
		}

		if (m_WrPos < m_Start || m_WrPos > m_Start + m_BufSize || m_DataLen < 0  || m_DataLen > m_BufSize) 
		{
			Clear();
		}
		return Pos;
	} 
	else 
	{
		return NULL;
	}
}



S32 CRcvBuf::GetDataLen()
{
	return m_DataLen;
}


S32 CRcvBuf::Update()
{
	if (m_ParsePos) 
	{
		if ((m_DataLen - (m_ParsePos - m_Start) ) >= 0) 
		{
			memmove( m_Start, m_ParsePos , m_DataLen - (m_ParsePos - m_Start) );
			m_DataLen -= (m_ParsePos - m_Start);
			m_WrPos   -= (m_ParsePos - m_Start);
		} 
		else 
		{
			Clear();
		}

		if (m_WrPos < m_Start || m_WrPos > m_Start + m_BufSize || m_DataLen < 0  || m_DataLen > m_BufSize) 
		{
			Clear();
		}
	}
	m_ParsePos = NULL;

	return MW_SUCC;
}


Void CRcvBuf::SetParsePos(S8* Pos)
{
	m_ParsePos = Pos;
}


Bool CRcvBuf::CheckDataLen(S32 Len)
{
	if (m_BufSize - Len < 64) 
	{
		return false;
	} 
	else 
	{
		return true;
	}
}


S8* CRcvBuf::GetStartPos()
{
	return m_Start;
}

