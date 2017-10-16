#include "stdafx.h"
#include "Util.h"
#include <time.h>
#include <assert.h>
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

CStringW Util::Encoding::ConvertUTF8ToUnicode( LPCSTR pszStr )
{
    if(!pszStr)
        return L"";

    // 预转换，计算需要的空间
    INT wstrlen = ::MultiByteToWideChar(CP_UTF8, NULL, pszStr, strlen(pszStr), NULL, 0);
    if(wstrlen == 0)
    {
        return  L"";
    }
    ++wstrlen;

    WCHAR *wp = new WCHAR[wstrlen + 8];
    ::ZeroMemory(wp, (wstrlen + 8)*sizeof(WCHAR));

    // 转换
    INT ret = ::MultiByteToWideChar(CP_UTF8, NULL, pszStr, strlen(pszStr), wp, wstrlen);
    if(ret != 0)
    {
        CStringW s = wp;
        delete[] wp;
        return s ;
    }

    // 保存并返回
    delete[] wp;
    return L"";
}

CStringA Util::Encoding::ConvertUnicodeToUTF8( LPCWSTR pszStr )
{
    if (!pszStr)
        return "";

    int len = wcslen(pszStr);
    CStringA sr;
    UINT uLimit = 0x7fffffff;
    if (uLimit <= 0 || len == 0)
    {
        return sr;
    }
    UINT utf8len = 0;
    for (int i=0; i<len; ++i)
    {
        int marki = i;
        unsigned short us = pszStr[i];
        if (us == 0)
        {
            utf8len += 2;
        }
        else if (us <= 0x7f)
        {
            utf8len += 1;
        }
        else if (us <= 0x7ff)
        {
            utf8len += 2;
        }
        else if (0xd800 <= us && us <= 0xdbff && i+1<len)
        {
            unsigned short ul = pszStr[i+1];
            if (0xdc00 <= ul && ul <= 0xdfff)
            {
                ++i;
                utf8len += 4;
            }
            else
            {
                utf8len += 3;
            }
        }
        else
        {
            utf8len += 3;
        }
        if (utf8len > uLimit)
        {
            len = marki;
            break;
        }
    }

    PBYTE pc = (PBYTE)sr.GetBuffer(utf8len+1);
    for (int i=0; i<len; ++i)
    {
        unsigned short us = pszStr[i];
        if (us == 0)
        {
            *pc ++ = 0xc0;
            *pc ++ = 0x80;
        }
        else if (us <= 0x7f)
        {
            *pc ++ = (char)us;
        }
        else if (us <= 0x7ff)
        {
            *pc ++ = 0xc0 | (us >> 6);
            *pc ++ = 0x80 | (us & 0x3f);
        }
        else if(0xd800 <= us && us <= 0xdbff && i+1<len)
        {
            unsigned short ul = pszStr[i+1];
            if (0xdc00 <= ul && ul <= 0xdfff)
            {
                ++i;
                UINT bc = (us-0xD800)*0x400 + (ul-0xDC00) + 0x10000;
                *pc ++ = (BYTE)(0xf0 | ((bc >> 18) & 0x07));
                *pc ++ = (BYTE)(0x80 | ((bc >> 12) & 0x3f));
                *pc ++ = (BYTE)(0x80 | ((bc >>  6) & 0x3f));
                *pc ++ = (BYTE)(0x80 | ((bc      ) & 0x3f));
            }
            else
            {
                *pc ++ = (BYTE) (0xe0 | ((us >> 12) & 0x0f));
                *pc ++ = (BYTE) (0x80 | ((us >>  6) & 0x3f));
                *pc ++ = (BYTE) (0x80 | ((us      ) & 0x3f));
            }
        }
        else
        {
            *pc ++ = (BYTE) (0xe0 | ((us >> 12) & 0x0f));
            *pc ++ = (BYTE) (0x80 | ((us >>  6) & 0x3f));
            *pc ++ = (BYTE) (0x80 | ((us      ) & 0x3f));
        }
    }
    * pc++ = 0;
    sr.ReleaseBuffer();
    return sr;
}

CStringA Util::Encoding::ConvertUnicodeToAnsi( LPCWSTR pszStr )
{
    if(!pszStr)
        return "";

    LCID lcid = GetThreadLocale();
    SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC));

    USES_CONVERSION;
    CStringA strRet = W2A(pszStr);

    SetThreadLocale(lcid);

    return strRet;
}

CStringW Util::Encoding::ConvertAnsiToUnicode( LPCSTR pszStr )
{
    if(!pszStr)
        return L"";

    LCID lcid = GetThreadLocale();
    SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC));

    USES_CONVERSION;
    CStringW strRet = A2W(pszStr);

    SetThreadLocale(lcid);

    return strRet;
}

CString Util::CmdLine::GetCfgPath( INT argc, TCHAR* argv[] )
{
    TCHAR szCfgPath[MAX_PATH] = {0};
    if(argc >= 2)
    {
        _tcscpy_s(szCfgPath, MAX_PATH, argv[1]);
    }
    else
    {
        GetModuleFileName(NULL, szCfgPath, MAX_PATH);
        PathRemoveFileSpec(szCfgPath);
        PathAddBackslash(szCfgPath);
        PathAppend(szCfgPath, _T("config.ini"));
    }

    return szCfgPath;
}

INT Util::String::SplitString( const CString& s, const CString& regex, std::vector<CString>* pVec )
{
    if(!pVec)
    {
        return -1;
    }

    CString str = s;
    if(str.IsEmpty())
    {
        return -1;
    }

    while (TRUE)
    {
        int index = str.Find (regex) ;
        if (index == -1) 
        {
            pVec->push_back(str);
            break;
        }
        else
        {
            pVec->push_back(str.Left(index));
            str = str.Mid (index + regex.GetLength());
        }
    }

    return pVec->size();
}

CString Util::String::ToStr( DWORD dwValue )
{
    CString strRet;
    strRet.Format(_T("%d"), dwValue);

    return strRet;
}

CString Util::String::ToStr( time_t tmValue )
{
    TCHAR szValue[65] = {0};
    _i64tot(tmValue, szValue, 10);
    szValue[64] = _T('\0');

    return szValue;
}

time_t Util::String::ToTimet( LPCTSTR pszValue )
{
    if(!pszValue)
        return 0;

    time_t _tm = _ttoi64(pszValue);
    return _tm;
}

BOOL Util::File::ReadFileToBuffer( LPCTSTR pszFile, PBYTE* lpFileBuf, DWORD& dwFileSize )
{
    if(!pszFile || !lpFileBuf)
    {
        return FALSE;
    }

    HANDLE hFile = ::CreateFile(pszFile, GENERIC_READ , FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    do
    {
        dwFileSize = ::GetFileSize(hFile, NULL);
        if(dwFileSize == 0xFFFFFFFF)
        {
            break;
        }
        
        *lpFileBuf = new BYTE[dwFileSize + 1];
        if(*lpFileBuf == NULL)
        {
            break;
        }

        ZeroMemory(*lpFileBuf, dwFileSize + 1);

        DWORD dwRead = 0;
        if(!::ReadFile(hFile, *lpFileBuf, dwFileSize, &dwRead, NULL) || dwFileSize != dwRead)
        {
            delete[] *lpFileBuf;
            *lpFileBuf = NULL;

            break;
        }

        bRet = TRUE;
    } while (FALSE);

    ::CloseHandle(hFile);
    hFile = NULL;

    return bRet;
}

BOOL Util::File::WriteBufferToFile( LPCTSTR pszFile, PBYTE lpFileBuf, DWORD dwFileBufSize )
{
    if(!pszFile || !lpFileBuf)
    {
        return FALSE;
    }

    HANDLE hFile = ::CreateFile(pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;

    do 
    {
        DWORD dwWrite = 0;
        bRet = ::WriteFile(hFile, lpFileBuf, dwFileBufSize, &dwWrite, NULL);
    } while (FALSE);

    ::CloseHandle(hFile);
    hFile = NULL;

    return bRet;
}

BOOL Util::File::IsUnicodeFile( LPCTSTR pszFile )
{
    if(!pszFile)
        return FALSE;

    HANDLE hFile = ::CreateFile(pszFile, GENERIC_READ , FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;

    do 
    {
        WORD wFlag = 0;
        DWORD dwRead = 0;
        if(!::ReadFile(hFile, (LPVOID)&wFlag, sizeof(WORD), &dwRead, NULL) || sizeof(WORD) != dwRead)
        {
            break;
        }

        if(wFlag == 0xFEFF)
            bRet = TRUE;
    } while (FALSE);

    ::CloseHandle(hFile);
    hFile = NULL;

    return bRet;
}

BOOL Util::File::DelDir( LPCTSTR pszDir )
{
    if(!pszDir)
    {
        return FALSE;
    }

    if(!PathFileExists(pszDir))
    {
        return TRUE;
    }

    BOOL bRet = FALSE;

    TCHAR szSearch[MAX_PATH] = {0};
    _tcscpy_s(szSearch, MAX_PATH, pszDir);
    PathAddBackslash(szSearch);
    PathAppend(szSearch, _T("*.*"));

    TCHAR szTmp[MAX_PATH] = {0};

    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(szSearch, &data);
    while(hFind != INVALID_HANDLE_VALUE)
    {
        do 
        {
            if(_tcscmp(data.cFileName, _T(".")) == 0 || _tcscmp(data.cFileName, _T("..")) == 0)
            {
                break;
            }

            _tcscpy_s(szTmp, MAX_PATH, pszDir);
            PathAddBackslash(szTmp);
            PathAppend(szTmp, data.cFileName);

            if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                bRet = DelDir(szTmp);
            }
            else
            {
                bRet = DeleteFile(szTmp);
            }
        } while (FALSE);

        if(!FindNextFile(hFind, &data))
        {
            break;
        }
    }
    FindClose(hFind);

    bRet = RemoveDirectory(pszDir);
    return bRet;
}

BOOL Util::File::CollectFiles( LPCTSTR pszDir, LPCTSTR pszExt, vector<CString>& vctFile )
{
    if(!pszDir || !pszExt)
        return FALSE;

    TCHAR szSearch[MAX_PATH] = {0};
    _tcscpy_s(szSearch, MAX_PATH, pszDir);
    ::PathAddBackslash(szSearch);
    ::PathAppend(szSearch, pszExt);

    TCHAR szResult[MAX_PATH] = {0};
    WIN32_FIND_DATA data;

    HANDLE hFind = ::FindFirstFile(szSearch, &data);
    while(hFind != INVALID_HANDLE_VALUE)
    {
        _tcscpy_s(szResult, MAX_PATH, pszDir);
        ::PathAddBackslash(szResult);
        ::PathAppend(szResult, data.cFileName);
        vctFile.push_back(szResult);

        if(!::FindNextFile(hFind, &data))
        {
            break;
        }
    }
    ::FindClose(hFind);

    return TRUE;
}

BOOL Util::File::MkDir( LPCTSTR pszDir )
{
    if(!pszDir)
        return FALSE;

    TCHAR szTmp[MAX_PATH] = {0};
    _tcscpy_s(szTmp, MAX_PATH, pszDir);
    ::PathAddBackslash(szTmp);

    USES_CONVERSION;
    return ::MakeSureDirectoryPathExists(T2A(szTmp));
}

CString Util::File::DirCat( LPCTSTR pszDir, LPCTSTR pszFile )
{
    if(!pszDir && !pszFile)
        return _T("");
    else if(!pszDir)
        return pszFile;
    else if(!pszFile)
        return pszDir;

    TCHAR szTmp[MAX_PATH] = {0};
    _tcscpy_s(szTmp, MAX_PATH, pszDir);
    ::PathAddBackslash(szTmp);
    ::PathAppend(szTmp, pszFile);

    return szTmp;
}

CString Util::Date::GetDateString()
{
    time_t llTime = time(NULL);

    CString strResult;
    struct tm * pTime = _localtime64(&llTime);
    if(pTime)
    {	
        strResult.Format(_T( "%04d-%02d-%02d" ), 1900 + pTime->tm_year, pTime->tm_mon+1, pTime->tm_mday);
    }

    return strResult;
}

namespace Util
{
    namespace Descode
    {
#define BASE_XOR_VALUE	(0x75127602)
#define BASE_PLUS_VALUE	(0x00990011)

        // sizeof(pOutput) >= dwInputLen * 2
        void SimpleFormEncode( char* pInput, DWORD dwInputLen, char* pOutput )
        {
            char cHigh, cLow;

            for( DWORD i = 0; i < dwInputLen; i++ )
            {
                cHigh = (BYTE)pInput[i] >> 4;
                if( cHigh >= 10 )
                    *pOutput++ = ( cHigh - 10 ) + 'A';
                else
                    *pOutput++ = cHigh + '0';

                cLow = pInput[i] & 0x0F;
                if( cLow >= 10 )
                    *pOutput++ = ( cLow - 10 ) + 'A';
                else
                    *pOutput++ = cLow + '0';
            }
        }

        // pInput 和 pOutput 可以相同
        void SimpelFormDecode( char* pInput, DWORD dwInputLen, char* pOutput )
        {
            char cHigh, cLow;

            assert( dwInputLen % 2 == 0 );

            for( DWORD i = 0; i < dwInputLen; i += 2 )
            {
                // cHigh
                if( pInput[i] >= 'A' )
                    cHigh = pInput[i] - 'A' + 10;
                else
                    cHigh = pInput[i] - '0';

                // cLow
                if( pInput[i + 1] >= 'A' )
                    cLow = pInput[i + 1] - 'A' + 10;
                else
                    cLow = pInput[i + 1] - '0';

                *pOutput++ = cLow | ( (BYTE)cHigh << 4 );
            }
        }

        //	加密字符串。
        //	strInput为输入字符串，dwKey为一个密钥
        //	转换成功返回一个字符串指针，该字符串必须由调用者free() !!!!!!!!
        //	转换失败返回NULL
        //	算法对加密后的串做BASE64编码
        char* EncryptString(char* strInput, DWORD dwKey)
        {
            char* strDup;
            int nLen;
            nLen=strlen(strInput);

            strDup=(char*)malloc((nLen+1)*sizeof(char));
            if(strDup==NULL)
                return NULL;

            int i;
            srand(dwKey);
            for(i=0;i<nLen;i++)
                strDup[i] = (char)(strInput[i] ^ ((char)(rand()+BASE_PLUS_VALUE)) ^ BASE_XOR_VALUE);

            int nFormLen = nLen * 2 + 1;
            char* pForm = (char*)malloc( nFormLen );
            if( NULL != pForm )
            {
                SimpleFormEncode( strDup, nLen, pForm );
                pForm[ nLen * 2 ] = '\0';
            }

            free(strDup);

            return pForm;
        }

        //	对strEncrypt的密文进行解码，该密文必须由EncryptString加密的
        //	如果失败，返回NULL
        //	成功，返回一块指向原文字符串的缓冲区，由调用者free() !!!!!!!
        void DecryptString(char* strEncrypt, DWORD dwKey)
        {
            int nDecodeLen;

            nDecodeLen = strlen(strEncrypt);
            SimpelFormDecode( strEncrypt, nDecodeLen, strEncrypt );

            nDecodeLen /= 2;
            srand(dwKey);
            for( int i = 0; i < nDecodeLen; i++ )
                strEncrypt[i] ^= ((char)(rand()+BASE_PLUS_VALUE)) ^ BASE_XOR_VALUE;
            strEncrypt[nDecodeLen] = '\0';
        }

        DWORD GetVolumeNumber()
        {
            TCHAR tzVolumeNameBuffer[MAX_PATH+2]={};
            TCHAR tzFileSystemNameBuffer[MAX_PATH+2]={};
            DWORD dwVolumeNameSize = MAX_PATH+1;
            DWORD dwFileSystemNameSize = MAX_PATH+1;

            DWORD dwMaximumComponentLength=0;
            DWORD dwFileSystemFlags=0;

            DWORD dwVolumeSerialNumber = 0;
            if(!GetVolumeInformation(_T("C:\\"),tzVolumeNameBuffer,dwVolumeNameSize,
                &dwVolumeSerialNumber,&dwMaximumComponentLength,&dwFileSystemFlags,
                tzFileSystemNameBuffer,dwFileSystemNameSize) || !dwVolumeSerialNumber)
                dwVolumeSerialNumber = 1985;
            return dwVolumeSerialNumber;
        }

        LPSTR EncryptPassword(LPCTSTR lpszSrc)
        {
            LPSTR lpstrPassword = NULL;
            static DWORD dwKey = GetVolumeNumber();
            if(lpszSrc)
            {
                USES_CONVERSION;
                CHAR*	psData = T2A((LPTSTR)lpszSrc);
                lpstrPassword = EncryptString(psData, dwKey);
            }

            return lpstrPassword;
        }

        LPSTR DecryptPassword(LPCTSTR lpszSrc)
        {
            LPSTR lpstrPassword = NULL;
            static DWORD dwKey = GetVolumeNumber();
            int nSrcLen = _tcslen(lpszSrc);
            if(lpszSrc && nSrcLen%2 == 0)
            {
                TCHAR* szTmp = new TCHAR[nSrcLen + 1];
                _tcscpy_s(szTmp, nSrcLen + 1, lpszSrc);
                szTmp[nSrcLen] = 0;

                USES_CONVERSION;
                CHAR* psData = T2A(szTmp);
                DecryptString(psData,dwKey);
                if(psData)
                {
                    int nLength = strlen(psData);
                    if(nLength>0 && nLength<100)
                    {
                        lpstrPassword = (CHAR*)malloc((nLength+1)*sizeof(CHAR));
                        if(lpstrPassword)
                        {
                            memset(lpstrPassword , 0 , (nLength+1)*sizeof(CHAR));
                            _snprintf_s(lpstrPassword , nLength + 1, _TRUNCATE, "%s" , psData);
                        }
                    }
                }

                delete[] szTmp;
                szTmp = NULL;
            }	
            return lpstrPassword;
        }
    }
}

LPTSTR Util::Descode::DecryptPassword( LPCTSTR lpszSrc, LPTSTR lpszDes, INT nMax )
{
    if ( lpszDes )
    {
        LPSTR pszPass = DecryptPassword( lpszSrc );
        if ( pszPass )
        {
            USES_CONVERSION;
            _sntprintf_s( lpszDes, nMax-1, _TRUNCATE, _T("%s"), A2T( pszPass ) );
            free( pszPass );
        }
    }
    return lpszDes;
}

LPTSTR Util::Descode::EncryptPassword( LPCTSTR lpszSrc, LPTSTR lpszDes, INT nMax )
{
    if ( lpszDes )
    {
        LPSTR pszPass = EncryptPassword( lpszSrc );
        if ( pszPass )
        {
            USES_CONVERSION;
            _sntprintf_s( lpszDes, nMax-1, _TRUNCATE, _T("%s"), A2T(pszPass) );
            free( pszPass );
        }
    }
    return lpszDes;
}
