#ifndef _ICRYSTALINFO_H_20141226_
#define _ICRYSTALINFO_H_20141226_

#pragma once

typedef enum
{
    LATTICE_TYPE_NONE               = 0,

    LATTICE_TYPE_P,
    LATTICE_TYPE_A,
    LATTICE_TYPE_B,
    LATTICE_TYPE_C,
    LATTICE_TYPE_F,
    LATTICE_TYPE_I,
    LATTICE_TYPE_R,
}LATTICE_TYPE;

typedef enum
{
    REMARK_TYPE_NONE                = -1,

    REMARK_TYPE_1,
    REMARK_TYPE_2,
    REMARK_TYPE_3,
    REMARK_TYPE_4,
    REMARK_TYPE_5,

    REMARK_TYPE_MAX,
}REMARK_TYPE;

typedef enum
{
    ATTACH_TYPE_NONE                = -1,
    ATTACH_TYPE_MAX                 = 9999,
}ATTACH_TYPE;

class ICrystalInfo
{
public:
    virtual LATTICE_TYPE    GetLatticeType() PURE;
    virtual void    SetLatticeType(LATTICE_TYPE enLatticeType) PURE;

    virtual FLOAT   GetA() PURE;
    virtual void    SetA(FLOAT fA) PURE;

    virtual FLOAT   GetB() PURE;
    virtual void    SetB(FLOAT fB) PURE;

    virtual FLOAT   GetC() PURE;
    virtual void    SetC(FLOAT fC) PURE;

    virtual FLOAT   GetAlpha() PURE;
    virtual void    SetAlpha(FLOAT fAlpha) PURE;

    virtual FLOAT   GetBeta() PURE;
    virtual void    SetBeta(FLOAT fBeta) PURE;

    virtual FLOAT   GetGamma() PURE;
    virtual void    SetGamma(FLOAT fGamma) PURE;

    virtual FLOAT   GetV() PURE;
    virtual void    SetV(FLOAT fV) PURE;

    virtual vector<CString>&    GetAttachList() PURE;
    virtual INT     GetAttachCount() PURE;

    virtual LPCTSTR GetAttach(INT nIndex) PURE;
    virtual BOOL    SetAttach(INT nIndex, LPCTSTR pszAttach, __out CString& strFilePath) PURE;

    virtual vector<CString>&    GetRemarkList() PURE;
    virtual INT     GetRemarkCount() PURE;

    virtual LPCTSTR GetRemark(INT nIndex) PURE;
    virtual BOOL    SetRemark(INT nIndex, LPCTSTR pszRemark) PURE;
};

class CCrystalInfoFactory
{
public:
    static ICrystalInfo* CreateCrystalInfo();
};

#endif