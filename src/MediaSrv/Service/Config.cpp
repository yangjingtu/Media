#include "Config.h"
#include <tchar.h>
#include <string>
using namespace std;

Config::Config(void)
	: m_pIni(NULL)
{
	Init();
}


Config::~Config(void)
{
	if(m_pIni)
	{
		delete m_pIni;
		m_pIni = NULL;
	}
}

wstring Config::GetAppPath()
{
	TCHAR path[MAX_PATH];   
	GetModuleFileName(NULL,path,MAX_PATH);  
	wstring strP = path;
	strP = strP.substr(0, strP.rfind('\\'));
	return strP;
}

void Config::Init()
{
	m_pIni = new CIniFile(GetConfigPath().c_str());
	if(m_pIni)
	{
		TCHAR tmp[20] = {0};
		m_pIni->GetProfileString(_T("MEDIA"), _T("LOG"), tmp);
		if(_tcslen(tmp) == 0)
		{
			m_pIni->SetProfileString(_T("MEDIA"), _T("LOG"), _T("0"));
			m_pIni->SetProfileString(_T("MEDIA"), _T("SHOW"), _T("0"));
		}
	}
}


wstring Config::GetConfigPath()
{
	wstring strConfigPath = GetAppPath();
	strConfigPath += _T("\\dvr_cfg.ini");
	return strConfigPath;
}

Config& Config::Instance()
{
	static Config cf;
	return cf;
}

wstring Config::GetMediaPath()
{
	if(m_pIni)
	{
		TCHAR path[MAX_PATH] = {0};
		m_pIni->GetProfileString(_T("FILE"), _T("Path"), path);
		return path;
	}
	return _T("");
}

//是否显示日志
bool Config::IsShowLog()
{
	if(m_pIni)
	{
		int nLOG = m_pIni->GetProfileInt(_T("MEDIA"), _T("LOG"));
		return nLOG ? true : false;
	}
	return false;
}

//是否显示窗口本身
bool Config::IsShowWnd()
{
	if(m_pIni)
	{
		int nLOG = m_pIni->GetProfileInt(_T("MEDIA"), _T("SHOW"));
		return nLOG ? true : false;
	}
	return false;
}