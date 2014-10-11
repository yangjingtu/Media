/***********************************************************************************************************************
Copyright:  
FileName: DebugLog.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef  COMMON_DEBUG_LOG_H_
#define  COMMON_DEBUG_LOG_H_
#include <stdio.h>
#include "BaseType.h"
#include "CommDef.h"

/* data type */
typedef enum DEBUG_LEVEL 
{
	DL_INFO    = 0,
	DL_WARNING = 1,
	DL_ERROR   = 2,
	DL_MAX
}DEBUG_LEVEL_E;

typedef enum DEBUG_MODULE
{
	DM_SRV = 0,
	DM_MAX
}DEBUG_MODULE_E;

/*Function declaration list*/
//extern S8 g_ModuleList[DM_MAX+1][20];
//extern S8 g_LevelList[DL_MAX+1][20];


#ifdef  USE_DEBUG_LOG
	#define DEBUG_INFO      printf
	#define DEBUG_WARNING   printf 
	#define DEBUG_ERROR     printf 
	#define RUN_DEBUG       printf("file[%s]:func[%s]:line[%d]\n", __FILE__, __FUNCTION__, __LINE__);
#else
	#define DEBUG_INFO
    #define DEBUG_WARNING
    #define DEBUG_ERROR
#endif


#endif /*end COMMON_DEBUG_LOG_H_*/