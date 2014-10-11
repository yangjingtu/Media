/***********************************************************************************************************************
Copyright:  
FileName: Cond.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef THREAD_COND_H_
#define THREAD_COND_H_
#include "../Common/Common.h"
#include "Lock.h"

class CCond
{
public:
	CCond();
	virtual ~CCond();

public:
	Void Wait(CLock &Lock);
	Void Wake();

protected:
	cdHandle_t  m_Handle;
};

#endif /*THREAD_COND_H_*/