/***********************************************************************************************************************
Copyright:  
FileName: Common.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_
#include "BaseType.h"
#include "DataType.h"
#include "CommDef.h"
#include "DebugLog.h"
#include "CommFunc.h"
#include "MsgMapT.hpp"
#include "Frame.h"
#include "Trash.h"
#ifdef RUN_WIN32
	#include "Win32Def.h"
#elif
    Error "Please choose platfrom"
#endif

#define _EXPORTING

#ifdef _EXPORTING
#define WELL_SDK __declspec(dllexport)
#else
#define WELL_SDK __declspec(dllimport)
#endif

#pragma comment(lib,"Ws2_32.lib")

#endif  /*COMMON_COMMON_H_*/