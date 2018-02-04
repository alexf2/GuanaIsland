#if !defined(_PROP_TECH_)
#define _PROP_TECH_

class TPropPage_Sys: public CPropertyPage
{

 friend class TMyProperty;

	DECLARE_DYNCREATE( TPropPage_Sys )

public:
	TPropPage_Sys();
	~TPropPage_Sys();

	enum { IDD = IDD_DIALOG_TECH };

protected:

 list<TAP_COXBtn> m_lstCOX;

 vector<TSD> m_vecSrc;
	void InitDScrs();

	virtual void DoDataExchange( CDataExchange* pDX ); 	

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();	

 BOOL RegisterClasses();
	void SetTxtProp();

	TMyDataGrid m_GridRoom, m_GridBillCat, 
			 m_GridGuestCat, m_GridRecommendedBillCat;

	CAdodc m_ADODCRoom, m_ADODCBillCat,
			 m_ADODCGuestCat, m_ADODCRecommendedBillCat;

	
	list<TAP_COXText> m_lstAPT;	 

	virtual BOOL PreTranslateMessage( MSG* pMsg );
	//BOOL SaveFocusControl();
 //afx_msg void OnSetFocus( CWnd* pOldWnd );


	virtual BOOL OnKillActive();
	//virtual BOOL OnSetActive( );

	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );

	afx_msg void OnErrorDatagrid_Room( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_BillCat( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_GuestCat( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_RecomBillCat( short DataError, short FAR* Response );

	void ReportErr( short DataError, short FAR* Response, int idGrid );

	afx_msg void OnMoveCompleteAdodc_GuestCat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnMoveCompleteAdodc_Room( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnMoveCompleteAdodc_BillCat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnMoveCompleteAdodc_RecommendedCat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );

	afx_msg void OnBeforeColEdit_Room(short ColIndex, short KeyAscii, short FAR* Cancel);
	afx_msg void OnBeforeColEdit_BillCat(short ColIndex, short KeyAscii, short FAR* Cancel);
	afx_msg void OnBeforeColEdit_GuestCat(short ColIndex, short KeyAscii, short FAR* Cancel);
	afx_msg void OnBeforeColEdit_RecomBillCat(short ColIndex, short KeyAscii, short FAR* Cancel);

 afx_msg void OnAfterColEdit_Room(short ColIndex);
	afx_msg void OnAfterColEdit_BillCat(short ColIndex);
	afx_msg void OnAfterColEdit_GuestCat(short ColIndex);
	afx_msg void OnAfterColEdit_RecomBillCat(short ColIndex);

	afx_msg void OnError_Room( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay );

	afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );

	void ReportErr_ADC( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay, int idADC );	
	

protected:	
	
	virtual BOOL OnApply(); 
	virtual void OnCancel();

	bool MakeUpdate_Rec();

	COXBitmapButton m_btnEditRecBill;
	afx_msg void OnEditRecBill_Clicked();
	afx_msg void OnSort_Nam();
	afx_msg void OnSort_Rate();

	CPoint m_ptEditRecBill_Key;

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};



#endif
