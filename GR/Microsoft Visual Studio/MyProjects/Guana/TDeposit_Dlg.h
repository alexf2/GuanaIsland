#if !defined(AFX_TDEPOSIT_DLG_H__8BBB99AB_E670_11D2_AD9D_00504E02C39D__INCLUDED_)
#define AFX_TDEPOSIT_DLG_H__8BBB99AB_E670_11D2_AD9D_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDeposit_Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TDeposit_Dlg dialog

class TDeposit_Dlg : public CDialog
{
// Construction
public:
	TDeposit_Dlg( COleCurrency&, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TDeposit_Dlg)
	enum { IDD = IDD_TDEPOSIT_DLG_DIALOG };
	COleCurrency	m_ocValue;
	//}}AFX_DATA

	//double m_dDeposit, m_dRead;
	COleCurrency m_ocDeposit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TDeposit_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
 list<TAP_COXBtn> m_lstCOX; 
	// Generated message map functions
	//{{AFX_MSG(TDeposit_Dlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonSub();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

 bool CheckDep( COleCurrency& );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDEPOSIT_DLG_H__8BBB99AB_E670_11D2_AD9D_00504E02C39D__INCLUDED_)
