#ifndef _LOG_H
#define _LOG_H

#include <Windows.h>
#include <shlobj.h>

#include <iostream>
#include <string>

/*******************************Example***************************************/
//1.#include "Log.h"															
//2.WRITELOG("out of memory, File=%s, Line=%d", __FILE__, __LINE__)  
/******************************************************************************/                                                      

#define WRITELOG lqs::GetLogger().Log
#define MAXRECORDSIZE 10000
#define MAXFILESIZE 1000
#define MAXNUMLENGTH 20
#define MAXCONTENTLENGTH 300

namespace lqs
{
	
	class CLog
	{
	public:
		enum LOG_LEVEL
		{
			NORMAL_LOG,
			DEBUG_LOG,
			TRACE_LOG,
		};

	public:
		CLog(void);
		~CLog(void);
		void Log(const char *format, ...);

		void PrintOutput(const char* format, ...);
	protected:
		void SetLogLevel(LOG_LEVEL logLevel = DEBUG_LOG)
		{
			m_lLogLevel = logLevel;
		}
		void Log_va_list(LOG_LEVEL logLevel, const char* format, va_list vargs);
		void SetPath();
		int _Log(FILE *fp, LOG_LEVEL logLevel, const char* format, ...);
		int __Log(FILE *fp, LOG_LEVEL logLevel, const char* format, va_list args);
		std::string GetNewLogFileName(time_t time);
		bool CreateNewLogFile();

		void SetConfig();
		void CreateDefaultConfigFile();
		void WriteFileIndex();
		void UpdateConfigFileIndex();
		void GetFileIndex();
		void CreateIndexFile();
		void RemoveLogFile(char* path);

	private:
		std::string m_strModulePath;
		std::string m_strFilePrefix;
		std::string m_strFileType;
		std::string m_strLastFileName;

		
		int m_iFileSize;
		int m_iIdleFile;

		int m_iRecordNum;
		int m_iMaxRecordSize;
		int m_iMaxRecordFileSize;
		int m_iFileIndex;


		bool m_bWriteFileError;
		LOG_LEVEL m_lLogLevel;
		FILE *m_pCurrentFile;
		bool m_bFileOpened;
		CRITICAL_SECTION m_csLogFile;

		bool m_bConsole;
	};

	extern CLog& GetLogger();
}

#endif