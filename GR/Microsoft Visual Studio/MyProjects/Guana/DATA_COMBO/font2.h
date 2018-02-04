#if !defined(AFX_FONT2_H__65EF0667_DD00_11D2_AD81_00504E02C39D__INCLUDED_)
#define AFX_FONT2_H__65EF0667_DD00_11D2_AD81_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// COleFont2 wrapper class

class COleFont2 : public COleDispatchDriver
{
public:
	COleFont2() {}		// Calls COleDispatchDriver default constructor
	COleFont2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COleFont2(const COleFont2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CY GetSize();
	void SetSize(const CY&);
	BOOL GetBold();
	void SetBold(BOOL);
	BOOL GetItalic();
	void SetItalic(BOOL);
	BOOL GetUnderline();
	void SetUnderline(BOOL);
	BOOL GetStrikethrough();
	void SetStrikethrough(BOOL);
	short GetWeight();
	void SetWeight(short);
	short GetCharset();
	void SetCharset(short);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONT2_H__65EF0667_DD00_11D2_AD81_00504E02C39D__INCLUDED_)