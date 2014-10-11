/***********************************************************************************************************************
Copyright:  
FileName: MsgMapT.hpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/2/9   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_MSGMAP_H_
#define COMMON_MSGMAP_H_
#include  <map>
using namespace std;
#include "../Thread/Lock.h"

class CLock;

template<typename T1, typename T2>
class CMsgMapT
{
public:
	CMsgMapT(){};
	~CMsgMapT(){};

public:
	S32 Put(T1 t1, T2 &t2);
	S32 Get(T1 t1, T2 &t2, S32 Timeout);

protected:
	CLock         m_Lock;
	map<T1, T2>   m_MsgMap;
};

template<typename T1, typename T2>
S32 CMsgMapT<T1, T2>::Put(T1 t1, T2 &t2)
{
	CAutoDoT<CLock> Auto(m_Lock);
	if (m_MsgMap.size() > 512)
	{
		m_MsgMap.erase(m_MsgMap.begin());
	}
	m_MsgMap[t1] = t2;

	return MW_SUCC;
}

template<typename T1, typename T2>
S32 CMsgMapT<T1, T2>::Get(T1 t1, T2 &t2, S32 mSec)
{
	S32 Times = 300;
	S32 Slp   = mSec / Times;
	do
	{
		do 
		{
			CAutoDoT<CLock> Auto(m_Lock);
			map<T1, T2>::iterator it = m_MsgMap.find(t1);
			if (it != m_MsgMap.end())
			{
				t2 = it->second;
				m_MsgMap.erase(it);
				return MW_SUCC;
			}
		} while (0);
		mSleep(Slp);
	} while (--Times > 0);

	return MW_FAILED;
}

#endif /*COMMON_MSGMAP_H_*/