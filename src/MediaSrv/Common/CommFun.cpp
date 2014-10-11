/***********************************************************************************************************************
Copyright:  
FileName: CommFun.cpp   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "CommFunc.h"

/***********************************************************************************************************************
Function:  CreateNormalThread
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
tHandle_t CreateNormalThread(tThreadProc_p Proc, tProcPara_t Param, S32 StackSize)
{
	tHandle_t  Ret; 
	CREATE_NORMAL_THREAD(Proc, Param, StackSize, Ret);
	if (!THREAD_HANDLE_OK(Ret))
	{
		DEBUG_ERROR("Create thread err\n");
	}
	
	return Ret;
}

/***********************************************************************************************************************
Function:  CancelNormalThread
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Void CancelNormalThread(tHandle_t Handle)
{
	CANCEL_NORMAL_THREAD(Handle);
}

/***********************************************************************************************************************
Function:  strstr
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S8 *strstr(const S8* src, const S8* dst, S32 srcLen)
{
	if ( *dst )
	{
		while ( srcLen-- > 0 )
		{
			for(S32 n = 0; * (src + n) == *(dst + n); n++)
			{
				if ( !*(dst + n + 1) )
				{
					return (char *) src;
				}
			}
			src++;
		}
		return NULL;
	}
	else
	{
		return (char *)src;
	}
}
