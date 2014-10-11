/***********************************************************************************************************************
Copyright:  
FileName: Lock.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef THREAD_LOCK_H_
#define THREAD_LOCK_H_

#include "AutoDo.h"

class __declspec(dllexport) CLock : public CAutoDo
{
public:
	CLock();
	virtual ~CLock();

public:
	virtual Void Inc();
	virtual Void Dec();
	Void Lock();
	Void Unlock();

public:
	lkHandle_t &GetHandle();

protected:
	lkHandle_t m_Handle;
};

#endif /*THREAD_LOCK_H_*/