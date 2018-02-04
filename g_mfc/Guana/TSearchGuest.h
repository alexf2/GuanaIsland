#if !defined(AFX_TSEARCHGUEST_H__ADB21A78_E4E0_11D2_AD96_00504E02C39D__INCLUDED_)
#define AFX_TSEARCHGUEST_H__ADB21A78_E4E0_11D2_AD96_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TSearchGuest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TSearchGuest dialog

class TSearchGuest : public CDialog
{
// Construction
public:
	TSearchGuest( _ConnectionPtr &rConn,  _RecordsetPtr& rRec, CWnd* pParent );   // standard constructor

// Dialog Data
	//{{AFX_DATA(TSearchGuest)
	enum { IDD = IDD_DLG_SEARCH_GUEST };
	CEdit	m_edLName;
	CEdit	m_edFName;
	CString	m_csFName;
	CString	m_csLName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TSearchGuest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  _RecordsetPtr& m_rRec;
		_ConnectionPtr& m_rConn;

  _RecordsetPtr m_pRecQ;
		_CommandPtr m_pCmdQ;

		bool m_bFlRestart, m_bInit;
		list<TAP_COXBtn> m_lstCOX;

		virtual void OnCancel();
		virtual void OnOK();

	// Generated message map functions
	//{{AFX_MSG(TSearchGuest)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();	
	//}}AFX_MSG

	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSEARCHGUEST_H__ADB21A78_E4E0_11D2_AD96_00504E02C39D__INCLUDED_)
