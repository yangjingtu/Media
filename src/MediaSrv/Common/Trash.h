/***********************************************************************************************************************
Copyright:  
FileName: Trash.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_TRASH_H_
#define COMMON_TRASH_H_
#include <list>
#include "BaseType.h"
#include "DebugLog.h"
#include "CommFunc.h"
using namespace std;

class CTrash 
{
public:
	CTrash();
	virtual ~CTrash();
	//virtual Void Destroy();

public:
	static void init();
	static tProcType_t TrashProc(tProcPara_t Param);
	static Bool m_bInit;
	static list<CTrash*> m_ObjQ;

public:
	virtual Void Delete();

public:
	Bool m_aLive;
	Bool m_bDeletEd;
	S32  m_NotALiveCnt;
};

#endif /*COMMON_TRASH_H_*/