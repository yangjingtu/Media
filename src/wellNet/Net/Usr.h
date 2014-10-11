/***********************************************************************************************************************
Copyright:  
FileName: Usr.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_USR_H_
#define NET_USR_H_
#include <string>
#include <map>
#include "../Common/Common.h"
#include "../Thread/Cond.h"
#include "../Thread/Lock.h"
#include "../Thread/ThreadPoolT.hpp"
using namespace std;

class CNetObj;

class WELL_SDK CSession
{
public:
	CSession();
	virtual ~CSession();
public:
	CNetObj  *m_pObj;
};

class WELL_SDK CGroup
{
public:
	CGroup();
	virtual ~CGroup();
public:
	map<Socket_t, CSession>       m_SessionMap;
};

class WELL_SDK CCrowd
{
public:
	CCrowd();
	virtual ~CCrowd();
public:
	Void AddSession(CNetObj *pObj);
	Void DelSession(CNetObj *pObj);
public:
	map<S64, CGroup>              m_GroupMap;
};


#endif /*NET_USR_H_*/