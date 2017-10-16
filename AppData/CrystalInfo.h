#ifndef _CRYSTALINFO_20141226_
#define _CRYSTALINFO_20141226_

#pragma once

class CCrystalInfo : public ICrystalInfo
{
public:
    CCrystalInfo(void);
    virtual ~CCrystalInfo(void);

public:
    virtual LATTICE_TYPE    GetLatticeType();
    virtual void    SetLatticeType(LATTICE_TYPE enLatticeType);

    virtual FLOAT   GetA();
    virtual void    SetA(FLOAT fA);

    virtual FLOAT   GetB();
    virtual void    SetB(FLOAT fB);

    virtual FLOAT   GetC();
    virtual void    SetC(FLOAT fC);

    virtual FLOAT   GetAlpha();
    virtual void    SetAlpha(FLOAT fAlpha);

    virtual FLOAT   GetBeta();
    virtual void    SetBeta(FLOAT fBeta);

    virtual FLOAT   GetGamma();
    virtual void    SetGamma(FLOAT fGamma);

    virtual FLOAT   GetV();
    virtual void    SetV(FLOAT fV);

    virtual vector<CString>&    GetAttachList();
    virtual INT     GetAttachCount();

    virtual LPCTSTR GetAttach(INT nIndex);
    virtual BOOL    SetAttach(INT nIndex, LPCTSTR pszAttach, __out CString& strFilePath);

    virtual vector<CString>&    GetRemarkList();
    virtual INT     GetRemarkCount();

    virtual LPCTSTR GetRemark(INT nIndex);
    virtual BOOL    SetRemark(INT nIndex, LPCTSTR pszRemark);

private:
    INT                 m_nId;

    LATTICE_TYPE        m_enLatticeType;
    FLOAT               m_fA;
    FLOAT               m_fB;
    FLOAT               m_fC;
    FLOAT               m_fAlpha;
    FLOAT               m_fBeta;
    FLOAT               m_fGamma;
    FLOAT               m_fV;

    vector<CString>     m_vctAttach;
    vector<CString>     m_vctRemark;
};

#endif