// guanaDlg.h : header file
//

#if !defined(AFX_GUANADLG_H__ECC8011C_D845_11D2_AD75_00504E02C39D__INCLUDED_)
#define AFX_GUANADLG_H__ECC8011C_D845_11D2_AD75_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/////////////////////////////////////////////////////////////////////////////
// CGuanaDlg dialog

class TFineTab: public CTabCtrl
{
  DECLARE_DYNAMIC( TFineTab ) 	 
public:
  TFineTab(): CTabCtrl()
	{
	}

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );    
};

class TMyProperty : public CPropertySheet
{
	DECLARE_DYNAMIC(TMyProperty)

// Construction
public:
	TMyProperty(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	TMyProperty(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	
	virtual ~TMyProperty();

	
	void AddControlPages();	
	//BOOL Create( CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)Ö1, DWORD dwExStyle = 0 );			
 

	void CallNavAss( CWnd*, bool bFlDirectly = false );
	CString GetCaptionFor( CDataGrid* );
	void ResetLastFocus()
		{
		  p_wLastFocus = NULL;
		}

	
	_ConnectionPtr m_pConnection;
	bool m_bFlConnOpen;
	TMyDBNav m_nav;

	CWnd *p_wLastFocus;

	CAdodc* GetADC_for_Grid( CDataGrid* );
	map<LPVOID, LPVOID> m_mapGRID_DC;
	map<LPVOID, CString> m_mapCapt;

protected:
	 HICON m_hIcon;
		
		COXBitmapButton m_btnExit;		

		bool InitDataSrc();


	 // Generated message map functions
	 //{{AFX_MSG(CGENWDlg)
	 virtual BOOL OnInitDialog();
	 afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	 afx_msg HCURSOR OnQueryDragIcon();	
  //afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	 //}}AFX_MSG

	 afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
		afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
		afx_msg void OnDestroy();

		afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );
		
  void MakeExitBtn( CWnd* );		

		virtual BOOL OnApply();
		virtual void OnCancel();
		virtual BOOL PreTranslateMessage( MSG* pMsg );
		virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

		LONG OnInitNavigator( UINT wPar, LONG lPar );		
		
	// Generated message map functions
protected:
  TPropPage_Bill m_pagBill;
  TPropPage_Sys  m_pagSys;
  TPropPage_Time m_pagTime;

  TFineTab m_ftWrap;

  auto_ptr<CToolTipCtrl> m_apToolTip;		
		
  DECLARE_MESSAGE_MAP()	
};

extern UINT globl_iMsgIniNav;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUANADLG_H__ECC8011C_D845_11D2_AD75_00504E02C39D__INCLUDED_)
