//{{AFX_INCLUDES()
#include "data_combo\datacombo.h"
//}}AFX_INCLUDES
#if !defined(AFX_TBILLADD_H__7A0D63D6_EB02_11D2_ADAB_00504E02C39D__INCLUDED_)
#define AFX_TBILLADD_H__7A0D63D6_EB02_11D2_ADAB_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TBillAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TBillAdd dialog

class TBillAdd : public CDialog
{
// Construction
public:
	TBillAdd( LPCTSTR lpUsr, CDataGrid *pWGrid, 
		        _RecordsetPtr& rBill, CAdodc& rAdc, 
										CDataGrid *pWGrid_Room,
										_RecordsetPtr& rRoom, _ConnectionPtr& rConn,
										CPoint& rp, CWnd* pParent = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(TBillAdd)
	enum { IDD = IDD_TBILLADD_DIALOG };
	CDateTimeCtrl	m_dtcDateTo;
	CDateTimeCtrl	m_dtcDateFrom;
	CDateTimeCtrl	m_dtcDate;
	CDataCombo	m_dcRoom;
	COleDateTime	m_odtDate;
	COleDateTime	m_odtFrom;
	COleDateTime	m_odtTo;
	UINT	m_uiBillNo;
	CString	m_csComments;
	CString	m_csDetails;
	CString	m_csFolio;
	CString	m_csGI;
	UINT	m_uiNoOfGuests;
	//}}AFX_DATA
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TBillAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	 _RecordsetPtr& m_rBill;
		CAdodc& m_rAdc;
		CPoint& m_rPt;
		CDataGrid *m_pWGrid, *m_pWGrid_Room;

		CFont m_fntDFLT;

		_RecordsetPtr& m_rRoom;
		_ConnectionPtr& m_rConn;

		CString m_csUsr;

		list<TAP_COXBtn> m_lstCOX;
		CBrush hbrBkEdit;

		bool MakePost();
		void ChecheckEmpty( CString& rS, LPCTSTR lpMsg, int iCtl );

		void OnKeyPressDatacombo1( short FAR* KeyAscii );

	// Generated message map functions
	//{{AFX_MSG(TBillAdd)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBILLADD_H__7A0D63D6_EB02_11D2_ADAB_00504E02C39D__INCLUDED_)
