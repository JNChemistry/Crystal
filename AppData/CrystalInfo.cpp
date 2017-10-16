#include "StdAfx.h"
#include "CrystalInfo.h"

ICrystalInfo* CCrystalInfoFactory::CreateCrystalInfo()
{
    return new CCrystalInfo();
}

//////////////////////////////////////////////////////////////////////////

CCrystalInfo::CCrystalInfo(void)
    : m_nId(0)
    , m_enLatticeType(LATTICE_TYPE_NONE)
    , m_fA(0)
    , m_fB(0)
    , m_fC(0)
    , m_fAlpha(0)
    , m_fBeta(0)
    , m_fGamma(0)
    , m_fV(0)
{
    m_vctRemark.resize(REMARK_TYPE_MAX);
}

CCrystalInfo::~CCrystalInfo(void)
{
}

LATTICE_TYPE CCrystalInfo::GetLatticeType()
{
    return m_enLatticeType;
}

void CCrystalInfo::SetLatticeType( LATTICE_TYPE enLatticeType )
{
    m_enLatticeType = enLatticeType;
}

FLOAT CCrystalInfo::GetA()
{
    return m_fA;
}

void CCrystalInfo::SetA( FLOAT fA )
{
    m_fA = fA;
}

FLOAT CCrystalInfo::GetB()
{
    return m_fB;
}

void CCrystalInfo::SetB( FLOAT fB )
{
    m_fB = fB;
}

FLOAT CCrystalInfo::GetC()
{
    return m_fC;
}

void CCrystalInfo::SetC( FLOAT fC )
{
    m_fC = fC;
}

FLOAT CCrystalInfo::GetAlpha()
{
    return m_fAlpha;
}

void CCrystalInfo::SetAlpha( FLOAT fAlpha )
{
    m_fAlpha = fAlpha;
}

FLOAT CCrystalInfo::GetBeta()
{
    return m_fBeta;
}

void CCrystalInfo::SetBeta( FLOAT fBeta )
{
    m_fBeta = fBeta;
}

FLOAT CCrystalInfo::GetGamma()
{
    return m_fGamma;
}

void CCrystalInfo::SetGamma( FLOAT fGamma )
{
    m_fGamma = fGamma;
}

FLOAT CCrystalInfo::GetV()
{
    return m_fV;
}

void CCrystalInfo::SetV( FLOAT fV )
{
    m_fV = fV;
}

vector<CString>& CCrystalInfo::GetAttachList()
{
    return m_vctAttach;
}

INT CCrystalInfo::GetAttachCount()
{
    return (INT)m_vctAttach.size();
}

LPCTSTR CCrystalInfo::GetAttach( INT nIndex )
{
    if(nIndex >= 0 && nIndex < (INT)m_vctAttach.size())
        return m_vctAttach[nIndex];

    return NULL;
}

BOOL CCrystalInfo::SetAttach( INT nIndex, LPCTSTR pszAttach, __out CString& strFilePath )
{
    if(!pszAttach || !::PathFileExists(pszAttach))
        return FALSE;

    BOOL bRet = FALSE;
    CString strSrcFileName = ::PathFindFileName(pszAttach);

    // TODO
    if(nIndex <= ATTACH_TYPE_NONE || nIndex >= (INT)m_vctAttach.size())
    {

    }
    else
    {
        
    }

    return bRet;
}

vector<CString>& CCrystalInfo::GetRemarkList()
{
    return m_vctRemark;
}

INT CCrystalInfo::GetRemarkCount()
{
    return (INT)m_vctRemark.size();
}

LPCTSTR CCrystalInfo::GetRemark( INT nIndex )
{
    if(nIndex <= REMARK_TYPE_NONE || nIndex >= REMARK_TYPE_MAX || nIndex >= (INT)m_vctRemark.size())
        return NULL;

    return m_vctRemark[nIndex];
}

BOOL CCrystalInfo::SetRemark( INT nIndex, LPCTSTR pszRemark )
{
    if(nIndex <= REMARK_TYPE_NONE || nIndex >= REMARK_TYPE_MAX || nIndex >= (INT)m_vctRemark.size())
    {
        return FALSE;
    }

    m_vctRemark[nIndex] = pszRemark;

    return TRUE;
}
