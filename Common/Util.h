#ifndef _UTIL_H_20131029_
#define _UTIL_H_20131029_

#pragma once

#include <vector>
using namespace std;

namespace Util
{
    namespace Encoding
    {
        CStringW ConvertUTF8ToUnicode(LPCSTR pszStr);
        CStringA ConvertUnicodeToUTF8(LPCWSTR pszStr);

        CStringA ConvertUnicodeToAnsi(LPCWSTR pszStr);
        CStringW ConvertAnsiToUnicode(LPCSTR pszStr);
    }

    namespace CmdLine
    {
        CString GetCfgPath(INT argc, TCHAR* argv[]);
    }

    namespace String
    {
        INT SplitString(const CString& s, const CString& regex, std::vector<CString>* pVec);
        CString ToStr(DWORD dwValue);

        CString ToStr(time_t tmValue);
        time_t ToTimet(LPCTSTR pszValue);
    }

    namespace File
    {
        BOOL ReadFileToBuffer(LPCTSTR pszFile, PBYTE* lpFileBuf, DWORD& dwFileBufSize);
        BOOL WriteBufferToFile(LPCTSTR pszFile, PBYTE lpFileBuf, DWORD dwFileBufSize);
        BOOL IsUnicodeFile(LPCTSTR pszFile);
        BOOL CollectFiles(LPCTSTR pszDir, LPCTSTR pszExt, vector<CString>& vctFile);
        BOOL DelDir(LPCTSTR pszDir);
        BOOL MkDir(LPCTSTR pszDir);

        CString DirCat(LPCTSTR pszDir, LPCTSTR pszFile);
    }

    namespace Date
    {
        CString GetDateString();
    }

    // º”√‹£¨Ω‚√‹
    namespace Descode
    {
        LPTSTR DecryptPassword(LPCTSTR lpszSrc, LPTSTR lpszDes, INT nMax);
        LPTSTR EncryptPassword(LPCTSTR lpszSrc, LPTSTR lpszDes, INT nMax);
    }
}

#endif