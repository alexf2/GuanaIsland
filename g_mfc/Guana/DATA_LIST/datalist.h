#if !defined(AFX_DATALIST_H__65EF0669_DD00_11D2_AD81_00504E02C39D__INCLUDED_)
#define AFX_DATALIST_H__65EF0669_DD00_11D2_AD81_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPicture5;
class COleFont5;

/////////////////////////////////////////////////////////////////////////////
// CDataList wrapper class

class CDataList : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDataList)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xf0d2f219, 0xccb0, 0x11d0, { 0xa3, 0x16, 0x0, 0xaa, 0x0, 0x68, 0x8b, 0x10 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetBoundText();
	void SetBoundText(LPCTSTR lpszNewValue);
	CString GetBoundColumn();
	void SetBoundColumn(LPCTSTR lpszNewValue);
	long GetMousePointer();
	void SetMousePointer(long nNewValue);
	CPicture5 GetMouseIcon();
	void SetMouseIcon(LPDISPATCH newValue);
	void SetRefMouseIcon(LPDISPATCH newValue);
	BOOL GetIntegralHeight();
	void SetIntegralHeight(BOOL bNewValue);
	long GetMatchEntry();
	void SetMatchEntry(long nNewValue);
	VARIANT GetSelectedItem();
	short GetVisibleCount();
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	CString GetListField();
	void SetListField(LPCTSTR lpszNewValue);
	COleFont5 GetFont();
	void SetRefFont(LPDISPATCH newValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	unsigned long GetBackColor();
	void SetBackColor(unsigned long newValue);
	unsigned long GetForeColor();
	void SetForeColor(unsigned long newValue);
	BOOL GetMatchedWithList();
	long GetAppearance();
	void SetAppearance(long nNewValue);
	BOOL GetRightToLeft();
	void SetRightToLeft(BOOL bNewValue);
	long GetHwnd();
	LPUNKNOWN GetRowSource();
	void SetRefRowSource(LPUNKNOWN newValue);
	CString GetRowMember();
	void SetRowMember(LPCTSTR lpszNewValue);
	long GetOLEDragMode();
	void SetOLEDragMode(long nNewValue);
	long GetOLEDropMode();
	void SetOLEDropMode(long nNewValue);
	void ReFill();
	VARIANT GetVisibleItems(short nIndex);
	void Refresh();
	void OLEDrag();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALIST_H__65EF0669_DD00_11D2_AD81_00504E02C39D__INCLUDED_)
