//////////////////////////////////////////////////////////////////////////
//配置文件基类
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "IniFile.h"
#include <string>
using namespace std;

#define CONFIG Config::Instance()

class Config
{
private:
	Config(void);
	virtual ~Config(void);
public:
	static Config& Instance();

	virtual void Init();

	wstring GetMediaPath();
	bool IsShowLog();
	bool IsShowWnd();

protected:
	virtual wstring GetConfigPath();

protected:
	wstring GetAppPath();

private:
	CIniFile* m_pIni;
};

