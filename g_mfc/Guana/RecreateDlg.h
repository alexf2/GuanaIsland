#if !defined(AFX_RECREATEDLG_H__9BB27373_E284_11D2_A4C6_204C4F4F5020__INCLUDED_)
#define AFX_RECREATEDLG_H__9BB27373_E284_11D2_A4C6_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecreateDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CRecreateDlg dialog

class CRecreateDlg : public CDialog
{
// Construction
public:
	CRecreateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecreateDlg)
	enum { IDD = IDD_DIALOG_RECREATE };
	DWORD	m_dwDataSize;
	DWORD	m_dwLogSize;
	CString	m_strPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecreateDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECREATEDLG_H__9BB27373_E284_11D2_A4C6_204C4F4F5020__INCLUDED_)
