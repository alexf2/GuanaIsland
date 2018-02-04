#if !defined(AFX_TCONNECTDLG_H__502FC416_D9E2_11D2_AD79_00504E02C39D__INCLUDED_)
#define AFX_TCONNECTDLG_H__502FC416_D9E2_11D2_AD79_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TConnectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TConnectDlg dialog

class TConnectDlg : public CDialog
{
// Construction
public:
	TConnectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TConnectDlg)
	enum { IDD = IDD_DIALOG_CONNECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	 afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );

	// Generated message map functions
	//{{AFX_MSG(TConnectDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCONNECTDLG_H__502FC416_D9E2_11D2_AD79_00504E02C39D__INCLUDED_)
