/***********************************************************************************************************************
Copyright:  
FileName: CommFun.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_COMM_FUNC_H_
#define COMMON_COMM_FUNC_H_
#include "BaseType.h"
#include "CommDef.h"
#include "DebugLog.h"
#ifdef RUN_WIN32
#include "Win32Def.h"
#endif

//Function declaration list
tHandle_t CreateNormalThread(tThreadProc_p Proc, tProcPara_t Param, S32 StackSize = 3 * 1024 * 1024);
Void CancelNormalThread(tHandle_t Handle);
S8 *strstr(const S8* src, const S8* dst, S32 srcLen);

#endif  /*COMMON_COMM_FUNC_H_*/