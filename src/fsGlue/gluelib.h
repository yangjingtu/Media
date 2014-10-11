#ifndef GLUE_LIB_H
#define GLUE_LIB_H

typedef void  (*InvokeJavaScriptFunc)(char* fun, void* argv, int argc);
typedef void  (*Int32ToVarRiant)(int data, void* arg, int index, int lenght);
typedef void  (*StringToVarRiant)(char* data, void* arg, int index, int lenght);
typedef void* (*CreateVarRiant)(int lenght);
typedef void  (*DeleteVarRiant)(void* data);
typedef enum{
	ACTIVEX_PLUGIN,
	NPAPI_PLUGIN
}CluePlugin;
typedef enum{
	GLUE_HANDLE_ERR = -100,
	GLUE_OK = 0
}GlueHandle;

#ifdef __cplusplus
extern "C" {
#endif
	HANDLE Glue_InitializeGlue();

	void Glue_RstPlugin(HANDLE hGlue, CreateVarRiant vrcreate, DeleteVarRiant vrdel, Int32ToVarRiant cvint, StringToVarRiant cvstring, InvokeJavaScriptFunc func, void* data);

	int Glue_OnCreate(HANDLE hGlue, HWND hParent, HINSTANCE hInstance);
	int	Glue_OnSize(HANDLE hGlue, int w, int h);
	int Glue_OnDestroy(HANDLE hGlue);

	int Glue_GetNextPage(HANDLE hGlue);
	int Glue_GetPrevPage(HANDLE hGlue);

	int Glue_SetProperty_str(HANDLE hGlue, char* name, char* value);
	int Glue_SetProperty_Long(HANDLE hGlue, char* name, long value);

	int Glue_FullScreen(HANDLE hGlue);

	void Glue_DeleteGlue(HANDLE hGlue);

	bool Glue_OpenMedia(HANDLE hGlue, const char* mediaPath, short nType);
	bool Glue_CloseMedia(HANDLE hGlue);
	bool Glue_Play(HANDLE hGlue);
	bool Glue_Pause(HANDLE hGlue);
	bool Glue_Stop(HANDLE hGlue);
	bool Glue_Forward(HANDLE hGlue, short nSpeed);
	bool Glue_Reverse(HANDLE hGlue, short nSpeed);
	void Glue_PreFrame(HANDLE hGlue);
	void Glue_NextFrame(HANDLE hGlue);
	bool Glue_Seek(HANDLE hGlue, long pos, long minVal, long maxVal);

	short Glue_Snap(HANDLE hGlue, const char* savePath);

	bool Glue_SetSelectWnd(HANDLE hGlue, short nWndIndex);
	short Glue_SetWndChannels(HANDLE hGlue,short nWndId, short nChannels);

	void Glue_Exit(HANDLE hGlue);
	void Glue_GetPlayPos(HANDLE hGlue, LPLONG curPos, LPLONG totalFrames);

	short Glue_SetWndChannelSelect(HANDLE hGlue, short nWndIndex, short nChannel);
	short Glue_GetWndChannelSelect(HANDLE hGlue, short nWndIndex);
	short Glue_GetWndChannelCount(HANDLE hGlue, short nWndIndex);
#ifdef __cplusplus
}
#endif

#endif // end GUARD_LIBTEST_H