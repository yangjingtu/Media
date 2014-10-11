#include "stdafx.h"
#include "gluelib.h"
#include "Glue.h"
HANDLE Glue_InitializeGlue()
{
	CGlue* glue = new CGlue();
	if (glue)
	{
		return (HANDLE)glue;
	}
	return NULL;
}

void Glue_DeleteGlue(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->OnDestroy();
		delete glue;
		glue = NULL;
	}
}

void Glue_RstPlugin(HANDLE hGlue, CreateVarRiant vrcreate, DeleteVarRiant vrdel, Int32ToVarRiant cvint, StringToVarRiant cvstring, InvokeJavaScriptFunc func, void* data)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->RegisterPlugin(vrcreate, vrdel, cvint, cvstring, func, data);
	}
}

int Glue_OnCreate(HANDLE hGlue, HWND hParent, HINSTANCE hInstance)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->OnCreate(hParent, hInstance);
		return GLUE_OK;
	}	
	return GLUE_HANDLE_ERR;
}

int	Glue_OnSize(HANDLE hGlue, int w, int h)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->OnSize(w, h);
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

int Glue_OnDestroy(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->OnDestroy();
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

int Glue_SetProperty_str(HANDLE hGlue, char* name, char* value)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->SetPropertyOfStr(name, value);
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

int Glue_SetProperty_Long(HANDLE hGlue, char* name, long value)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->SetPropertyOfLong(name, value);
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

int Glue_GetNextPage(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		WRITELOG("Get Next Page Begin");
		glue->GetNextPage();
		WRITELOG("Get Next Page End");
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

int Glue_GetPrevPage(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if (glue)
	{
		glue->GetPrevPage();
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

int Glue_FullScreen(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		WRITELOG("Full Screen Begin");
		glue->EnterOrExitFullScreen();
		WRITELOG("Full Screen End");
		return GLUE_OK;
	}
	return GLUE_HANDLE_ERR;
}

bool Glue_OpenMedia(HANDLE hGlue, const char* mediaPath, short nType)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->OpenMedia(mediaPath, nType);
	}
	return false;
}

bool Glue_CloseMedia(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->CloseMedia();
	}
	return false;
}

bool Glue_Play(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Play();
	}
	return false;
}

bool Glue_Pause(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Pause();
	}
	return false;
}

bool Glue_Stop(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Stop();
	}
	return false;
}

bool Glue_Forward(HANDLE hGlue, short nSpeed)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Forward(nSpeed);
	}
	return false;
}

bool Glue_Reverse(HANDLE hGlue, short nSpeed)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Reverse(nSpeed);
	}
	return false;
}

void Glue_PreFrame(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->PreFrame();
	}
}

void Glue_NextFrame(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->NextFrame();
	}
}

bool Glue_Seek(HANDLE hGlue, long pos, long minVal, long maxVal)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Seek(pos, minVal, maxVal);
	}
	return false;
}

short  Glue_Snap(HANDLE hGlue, const char* savePath)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->Snap(savePath);
	}
	return -1;
}

bool Glue_SetSelectWnd(HANDLE hGlue, short nWndIndex)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->SetSelectWnd(nWndIndex) == 0 ? true : false;
	}
	return false;
}

short Glue_SetWndChannels(HANDLE hGlue, short nWndId, short nChannels)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->SetWndChannels( nWndId,  nChannels);
	}
	return 0;
}

void Glue_Exit(HANDLE hGlue)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		glue->Exit();
	}
}

void Glue_GetPlayPos(HANDLE hGlue, LPLONG curPos, LPLONG totalFrames)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		glue->GetPlayPos(curPos, totalFrames);
	}
}

short Glue_SetWndChannelSelect(HANDLE hGlue, short nWndIndex, short nChannel)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->SetWndChannelSelect(nWndIndex, nChannel);
	}
	return -1;
}

short Glue_GetWndChannelSelect(HANDLE hGlue, short nWndIndex)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->GetWndChannelSelect(nWndIndex);
	}
	return -1;
}

short Glue_GetWndChannelCount(HANDLE hGlue, short nWndIndex)
{
	CGlue* glue = static_cast<CGlue*>(hGlue);
	if(glue)
	{
		return glue->GetWndChannelCount(nWndIndex);
	}
	return -1;
}