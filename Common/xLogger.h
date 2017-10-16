#pragma once
#include <shlobj.h>
#include <strsafe.h>
#include <atltime.h>
/************************************************************************
<<<<<<<   xLogʹ��˵��   >>>>>>
[���÷���]
1����ENABLE_XLOG�궨��
2��XLOG��֧�ֹ�������log��XLOG_NOFILTER�ǲ������˵�log��XLOG_DBG��debug�²���Ч��log
3��CxAutoLogger������ڹ���������ֱ��log�������ڼ�⺯�����������������ں�����
 
[�鿴log����]
1�������г��������ļ����½�xLogConfig.ini�ļ�
2����Ҫ����xlog����Ľ����section���У����磺
[360Desktop.exe]
filter=filter list
logDirName=360Desktop
log=2

���У�
filter��ʾ���˹ؼ��֣������д��ֻ����ʾ�ùؼ��ֵ�log������ؼ����á�|���ָ����������д���򲻹����κιؼ���
logDirName��ʾlog���������ļ����������ļ���·���ļ�������������%appdata%\{logDirName}\Logs���������Ĭ����360Desktop
log��ʾ���log���ͣ�1���������dbgview�У�2���������log�ļ���,���ɵ�log�ļ���Ϊlog����Ľ�����

************************************************************************/


//----------------xLog�꿪��--------------------
#define ENABLE_XLOG		//����궨��ȥ������ص�����xLog
//-----------------------------------------------

enum LOG_TYPE
{
	BIT_ENABLE_OUTPUT_LOG = 1,
	BIT_ENABLE_FILE_LOG	= (1<<1),
	BIT_ENABLE_PFM_LOG	= (1<<2),
};


class CxLogger
{
	CxLogger()
	{
		m_dwStartTickCount = GetTickCount();
		TCHAR szExeFile[MAX_PATH] = {0};
		GetModuleFileName(NULL, szExeFile, MAX_PATH);

		CString strExeName = PathFindFileName(szExeFile);

		CString strCfgFile = L"c:\\test\\xLogConfig.ini";
		if(!PathFileExists(strCfgFile))
		{
			PathAppend(szExeFile, _T("..\\xLogConfig.ini"));
			strCfgFile = szExeFile;
		}
		
		//read ini file
		UINT uLogMask = GetPrivateProfileInt(strExeName, _T("log"), 0, strCfgFile);
		GetPrivateProfileString(strExeName, _T("filter"), _T(""), m_strFilters.GetBuffer(MAX_PATH), MAX_PATH, strCfgFile);
		m_strFilters.ReleaseBuffer();

		CString strLogDirName;
		GetPrivateProfileString(strExeName, _T("logDirName"), _T("AutoAnalyze"), strLogDirName.GetBuffer(MAX_PATH), MAX_PATH, strCfgFile);
		strLogDirName.ReleaseBuffer();		
		if(strLogDirName.IsEmpty())
			strLogDirName = _T("AutoAnalyze");

		m_bEnableFileLog = uLogMask & BIT_ENABLE_FILE_LOG;
		m_bEnableOutLog = uLogMask & BIT_ENABLE_OUTPUT_LOG;
		m_bEnablePFMLog = uLogMask & BIT_ENABLE_PFM_LOG;
		m_hLogFile = NULL;

		if (m_bEnableFileLog)
		{
			TCHAR szLogPath[_MAX_PATH]={0}; 
			//SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szLogPath); 
            GetModuleFileName(NULL, szLogPath, MAX_PATH);
            PathRemoveFileSpec(szLogPath);
			PathAppend(szLogPath, _T("\\logs"));
			SHCreateDirectory(NULL, szLogPath);
			PathAppend(szLogPath, strExeName + _T(".log"));

			m_hLogFile = CreateFile (szLogPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == m_hLogFile)
			{
				m_bEnableFileLog = FALSE;
				return;
			}
			SetFilePointer (m_hLogFile, 0, NULL, FILE_END) ;
		}
	}
public:
	/// ��ȡȫ�ֶ���
	static CxLogger* GetInstance()  { return &s_logger; }

	virtual ~CxLogger()
	{
		if (m_bEnableFileLog)
			DumpFileLog();
	}

	void DumpFileLog()
	{
		if (m_hLogFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hLogFile);
			m_hLogFile = NULL;
		}
	}

	/// ��ӡ������Ϣ
	void PrintLog (LPCTSTR szFilter, LPCTSTR szLog, ...)
	{
		if (m_bEnableOutLog || m_bEnableFileLog)
		{
			if (m_strFilters.GetLength() > 0 && m_strFilters.Find(szFilter) < 0)
			{
				return;
			}
			
			TCHAR szText[0x1000] = {};

			va_list args;
			va_start (args, szLog);
			StringCchVPrintf (szText, 0x1000, szLog, args);						
			va_end(args);

            CTime curTime = CTime::GetCurrentTime();
            CString strCurTime = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));

			CString strLog;
			strLog.Format(_T("%s [%s] %s\r\n"), strCurTime, szFilter, szText);

			if (m_bEnableOutLog)
				OutputDebugString(strLog);

			if (m_bEnableFileLog && (m_hLogFile != INVALID_HANDLE_VALUE))
			{
				DWORD   dwWrittenLen = 0;
				WriteFile(m_hLogFile, strLog, strLog.GetLength() * sizeof(TCHAR), &dwWrittenLen, NULL);
			}
		}

	}

	/// ��ӡ������Ϣ
	void PrintLog2 (LPCTSTR szFilter, LPCTSTR szLog, ...)
	{
		if (m_bEnableOutLog || m_bEnableFileLog)
		{
			if (m_strFilters.GetLength() > 0 && m_strFilters.Find(szFilter) < 0)
			{
				return;
			}

			TCHAR szText[0x1000] = {};

			va_list args;
			va_start (args, szLog);
			StringCchVPrintf (szText, 0x1000, szLog, args);						
			va_end(args);

			CTime curTime = CTime::GetCurrentTime();
			CString strCurTime = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			CString strLog;
			strLog.Format(_T("[%s] %s [%u] [%u] %s\r\n"), szFilter, strCurTime, m_dwStartTickCount,GetTickCount(),szText);

			if (m_bEnableOutLog)
				OutputDebugString(strLog);

			if (m_bEnableFileLog && (m_hLogFile != INVALID_HANDLE_VALUE))
			{
				DWORD   dwWrittenLen = 0;
				WriteFile(m_hLogFile, strLog, strLog.GetLength() * sizeof(TCHAR), &dwWrittenLen, NULL);
			} 
		}
	}

public:
	BOOL		m_bEnablePFMLog;
private:
	BOOL		m_bEnableFileLog;
	BOOL		m_bEnableOutLog;

	HANDLE		m_hLogFile;
	CString		m_strFilters;
	static CxLogger s_logger;
	DWORD		m_dwStartTickCount;
};

__declspec(selectany) CxLogger CxLogger::s_logger;


//-----------------------------------------------------------------------------------------

#ifdef ENABLE_XLOG

	#define XLOG(filter, log, ...)  {CxLogger::GetInstance()->PrintLog(filter, log, __VA_ARGS__);}
	#define XLOG_NOFILTER(log, ...)  {CxLogger::GetInstance()->PrintLog(_T(""), log, __VA_ARGS__);}
	#define XLOG2(filter, log, ...)  {CxLogger::GetInstance()->PrintLog2(filter, log, __VA_ARGS__);}
	#define XLOG_PFM(log, ...)  if( CxLogger::GetInstance()->m_bEnablePFMLog) XLOG2(_T("Performance"), log, __VA_ARGS__)
	#ifdef DEBUG
		#define XLOG_DBG XLOG
	#else
		#define XLOG_DBG(filter, str, ...) 
	#endif

#else

	#define XLOG(filter, str, ...)  
	#define XLOG_NOFILTER(log, ...)
	#define XLOG_DBG(filter, str, ...) 

#endif // ENABLE_XLOG


class CxAutoLogger
{
public:
	CxAutoLogger(const CString& strFilter, const CString& strLog)
	{
		XLOG(strFilter, L"Begin - %s", strLog);

		m_strFilter = strFilter;
		m_strLog = strLog;
		m_dwBeginTickCnt = GetTickCount();
	}

	~CxAutoLogger()
	{
		XLOG(m_strFilter, L"End - %s [%u ms]", m_strLog, GetTickCount() - m_dwBeginTickCnt);
	}

protected:
	CString m_strLog;
	CString m_strFilter;
	DWORD	m_dwBeginTickCnt;
};