#include "stdafx.h"
#include "Log.h"
#include <time.h>
#include <direct.h>

namespace lqs
{
	CLog& GetLogger()
	{
		static CLog log;
		return log;
	}

	CLog::CLog(void)
		: m_strFilePrefix("FOSCAM")
		, m_strFileType(".txt")
		, m_iIdleFile(0)
		, m_bWriteFileError(false)
		, m_bFileOpened(false)
	{
		SetPath();
		SetLogFileSize();
		SetIndex(GetLastFileName());
		if (CreateNewLogFile())
		{
			m_bFileOpened = true;
//#ifdef WIN32
			InitializeCriticalSection(&m_csLogFile);
//#else
//			pthread_mutex_init(&m_csLogFile, NULL);
//#endif
		}
	}

	CLog::~CLog(void)
	{
		if (m_bFileOpened)
		{
			fclose(m_pCurrentFile);
			m_pCurrentFile = NULL;
//#ifdef WIN32
			DeleteCriticalSection(&m_csLogFile);
//#else
//			pthread_mutex_destroy(&m_csLogFile);
//#endif
		}
	}

	void CLog::SetPath()
	{
		char szBuff[MAX_PATH] = {0};

		std::string strFilePath;
		GetModuleFileName(NULL, szBuff, MAX_PATH - 1);
		strrchr(szBuff, '\\')[1] = '\0';
		strcat(szBuff, "LOGS\\");
		//m_strModulePath = szBuff;
		//mkdir(szBuff);//#include <direct.h>
		m_strModulePath = "D:\\LOGSTTT\\";
		mkdir("D:\\LOGSTTT");//#include <direct.h>
	}

	std::string CLog::GetNewLogFileName(time_t time)
	{
		char timeBuf[201];
		char index[4];
		itoa(m_iIdleFile++ % 100, index, 10);
		strftime(timeBuf, 200, "_%Y-%m-%d_%H-%M-%S_", localtime(&time));
		return m_strModulePath + m_strFilePrefix + timeBuf + index + m_strFileType;
	}

	bool CLog::CreateNewLogFile()
	{
		std::string filename = GetNewLogFileName(time(NULL));
		if (m_pCurrentFile != NULL)
		{
			fclose(m_pCurrentFile);
			m_pCurrentFile = NULL;
		}

		m_pCurrentFile = fopen(filename.c_str(), "w");//if not exist, create it

		if (m_pCurrentFile)
		{
			m_strLastFileName = filename;
			m_iFileSize = 0;
		}

		WirteLastFileName();

		return m_pCurrentFile != NULL;
	}

	void CLog::Log(const char *format, ...)
	{
//#ifdef WIN32
		EnterCriticalSection(&m_csLogFile);
//#else
//		pthread_mutex_lock(&m_csLogFile);
//#endif
		va_list args;
		va_start(args, format);
		Log_va_list(m_lLogLevel, format, args);
		va_end(args);
//#ifdef WIN32
		LeaveCriticalSection(&m_csLogFile);
//#else
//		pthread_mutex_unlock(&m_csLogFile);
//#endif
	}

	void CLog::Log_va_list(LOG_LEVEL logLevel, const char *format, va_list vargs)
	{
		if (!m_bFileOpened || m_iFileSize > m_iMaxFileSize)
		{
			CreateNewLogFile();
		}

		if (m_bFileOpened)
		{
			int iWritten = __Log(m_pCurrentFile, logLevel, format, vargs);
			if (iWritten < 0)
			{
				m_bWriteFileError = true;
			}
			else
			{
				m_iFileSize += iWritten;
			}
		}
	}

	int CLog::_Log(FILE *fp, LOG_LEVEL logLevel, const char* format, ...)
	{
		int iCount = 0;
		va_list args;
		va_start(args, format);
		iCount = __Log(fp, logLevel, format, args);
		va_end(args);

		return iCount;
	}

	int CLog::__Log(FILE *fp, LOG_LEVEL logLevel, const char* format, va_list args)
	{
		int numWritten = 0;
		bool writtenError = false;

		char timeBuf[201] = {0};
		time_t tt = time(NULL);
		int iSize = (int)strftime(timeBuf, 200, "%Y%m%d_%H:%M:%S******", localtime(&tt));
		int iWritten = (int)fwrite(timeBuf, 1, iSize, fp);
		if (iSize != iWritten)
		{
			writtenError = true;
		}

		numWritten += iWritten;

		iWritten = vfprintf(fp, format, args);
		if (iWritten < 0)
		{
			writtenError = true;
		}
		else
		{
			numWritten += iWritten;
		}

		if (fputs("\n", fp) == EOF)
		{
			writtenError = true;
		}
		numWritten += 2;

		fflush(fp);

		if (writtenError)
		{
			numWritten = -1;
		}

		return numWritten;
	}

	std::string CLog::GetLastFileName()
	{
		m_strLastFileName = "";
		int len = m_strLastFileName.length();
		std::string file = m_strModulePath + "LastFile.ini";
		FILE *pFile = fopen(file.c_str(), "r");
		if (pFile == NULL)
		{
			return m_strLastFileName;
		}

		char buf[MAX_PATH] = {0};
		fread(buf, 1, MAX_PATH, pFile);
		fclose(pFile);
		m_strLastFileName = buf;

		return m_strLastFileName;
	}

	void CLog::WirteLastFileName()
	{
		int len = m_strLastFileName.length();
		std::string file = m_strModulePath + "LastFile.ini";
		FILE *pFile = fopen(file.c_str(), "w");
		if (pFile != NULL)
		{
			fwrite(m_strLastFileName.c_str(), 1, len, pFile);
			fclose(pFile);
		}
	}

	void CLog::SetIndex(std::string lastFile)
	{
		int iStart = lastFile.rfind('_');
		int iEnd = lastFile.rfind('.');
		m_iIdleFile = atoi(lastFile.substr(iStart + 1, iEnd - iStart - 1).c_str()) + 1;
	}
}