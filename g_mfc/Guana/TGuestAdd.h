//{{AFX_INCLUDES()
#include "data_combo\datacombo.h"
//}}AFX_INCLUDES
#if !defined(AFX_TGUESTADD_H__4AA60D01_EAAD_11D2_ADAA_00504E02C39D__INCLUDED_)
#define AFX_TGUESTADD_H__4AA60D01_EAAD_11D2_ADAA_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TGuestAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TGuestAdd dialog

class TGuestAdd : public CDialog
{
// Construction
public:
	TGuestAdd( CDataGrid *pWGrid, _RecordsetPtr& rGuest, CAdodc& rAdc, CPoint& rp, CWnd* pParent = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(TGuestAdd)
	enum { IDD = IDD_TGUESTADD_DIALOG };
	CComboBox	m_cbnState;
	CDataCombo	m_dcCat;
	CString	m_edFName;
	CString	m_edLName;
	COleCurrency	m_edDeposit;
	CString	m_edCity;
	CString	m_edAddr;
	UINT	m_uiPost;
	CString	m_csState;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TGuestAdd)
	public:
	virtual BOOL DestroyWindow();	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  _RecordsetPtr& m_rGuest;
		CAdodc& m_rAdc;
		CPoint& m_rPt;
		CDataGrid *m_pWGrid;

		list<TAP_COXBtn> m_lstCOX;
		CBrush hbrBkEdit;

		bool MakePost();
		void ChecheckEmpty( CString& rS, LPCTSTR lpMsg, int iCtl );

		void OnKeyPressDatacombo1( short FAR* KeyAscii );

	// Generated message map functions
	//{{AFX_MSG(TGuestAdd)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnOk2();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TGUESTADD_H__4AA60D01_EAAD_11D2_ADAA_00504E02C39D__INCLUDED_)
