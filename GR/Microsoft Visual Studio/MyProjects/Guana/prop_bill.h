#if !defined(_PROP_BILL_)
#define _PROP_BILL_

#include "..\..\..\SEAGATE SOFTWARE\CRYSTAL REPORTS\peplus.h"

class TPropPage_Bill: public CPropertyPage
{

 friend class TMyProperty;
	friend class TPropPage_Time;

	DECLARE_DYNCREATE( TPropPage_Bill )

public:
	TPropPage_Bill();
	~TPropPage_Bill();

	enum { IDD = IDD_DIALOG_BILL };


	bool MakeUpdate_Rec();

protected:
	vector<TSD> m_vecSrc;
	void InitDScrs();

	list<TAP_COXBtn> m_lstCOX;

	virtual void DoDataExchange( CDataExchange* pDX ); 	

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();	

	void SetTxtProp();
 BOOL RegisterClasses();

	TMyDataGrid m_GridGuest, m_GridBill, 
			 m_GridAddChares, m_GridPyments, m_GridRoomUsed;

	/*TMyMSMask m_maskBillNo, m_maskFolio, //m_maskDate,
		       m_maskGI, m_maskDetails, 
									m_mask_TotalAddt, m_mask_Payments, m_mask_TotalRooms,
									m_mask_TaxServ, m_mask_Total, m_mask_Deposit,
									m_mask_BDue, m_mask_SubTotal;
	 TMyDTPicker m_pickDate;*/

 TMyEdit m_edBillNo, m_edFolio, 
		       m_edGI, m_edDetails, 
									m_ed_TotalAddt, m_ed_Payments, m_ed_TotalRooms,
									m_ed_TaxServ, m_ed_Total, m_ed_Deposit,
									m_ed_BDue, m_ed_SubTotal;
	TMyDateTime m_dt_Date;

	CAdodc m_ADODCGuest, m_ADODCBill, 
			 m_ADODCAddChares, m_ADODCPyments, m_ADODCRoomUsed;

	CAdodc m_ADODCCatGuest, m_ADODCRoom, m_ADODCCatPay;

	auto_ptr<TLookUp> m_apLUP_GuestCat, m_apLUP_CA,
		 m_apLUP_ACNAMe, m_apLUP_RoomName;		
	
	list<TAP_COXText> m_lstAPT;

	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL OnSetActive();

	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );

	//virtual BOOL OnKillActive();
	//virtual BOOL OnSetActive( );
	
	afx_msg void OnBeforeColEdit_Guest( short ColIndex, short KeyAscii, short FAR* Cancel );
	afx_msg void OnBeforeColEdit_AddCh( short ColIndex, short KeyAscii, short FAR* Cancel );
	afx_msg void OnBeforeColEdit_RoomsAss( short ColIndex, short KeyAscii, short FAR* Cancel );

	afx_msg void OnOnAddNewDatagrid_Bill();
	afx_msg void OnBeforeInsertDatagrid_Bill( short FAR* Cancel );

	afx_msg void OnOnAddNewDatagrid_AddCh();
	afx_msg void OnBeforeInsertDatagrid_AddCh( short FAR* Cancel );
	afx_msg void OnOnAddNewDatagrid_Paymts();

	afx_msg void OnOnAddNewDatagrid_Payment();
	afx_msg void OnBeforeInsertDatagrid_Payment( short FAR* Cancel );

	afx_msg void OnOnAddNewDatagrid_Rooms();
	afx_msg void OnBeforeInsertDatagrid_Rooms( short FAR* Cancel );

	afx_msg void OnErrorDatagrid_Guest( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_Bill( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_AddCh( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_Pay( short DataError, short FAR* Response );
	afx_msg void OnErrorDatagrid_Rooms( short DataError, short FAR* Response );

	void HandleGridError( short DataError, short FAR* Response );

protected:	
	void CheckAvaliableChilds();

	auto_ptr<TSearchGuest> m_apSearchGuest;

	TZoomer m_tzZoom;
	void CalcZoomRect( CRect& );

	//COXBitmapButton m_btnPayEnrol, m_btnMoneyBack,
		 //m_btnSearch, m_btnComments, m_btnPrt;

	void Resync_Bill();

	void Enrollment( bool bFl );
	void MakeRefresh( _RecordsetPtr& rc );

	void SetBtnsState();

	afx_msg void OnBeforeUpdate_Guest( short FAR* Cancel );
	afx_msg void OnBeforeColUpdate_Guest( short ColIndex, VARIANT FAR* OldValue, short FAR* Cancel );
	afx_msg void OnBeforeDelete_Guest( short FAR* Cancel );

	afx_msg void OnBeforeDelete_Bill( short FAR* Cancel );
	afx_msg void OnBeforeDelete_AddCharges( short FAR* Cancel );
	afx_msg void OnBeforeDelete_RoomsAlloc( short FAR* Cancel );

	afx_msg void OnButtonClick_Guest( short ColIndex );
	afx_msg void OnButtonClick_AddCharges( short ColIndex );
	afx_msg void OnButtonClick_RoomsUsed( short ColIndex );
	
	afx_msg void OnSort_Guest_Nam();
	afx_msg void OnSort_Guest_City();
	afx_msg void OnSort_Guest_Category();
	afx_msg void OnSort_Guest_SizeDeposit();

	afx_msg void OnSort_Bill_Date();
	afx_msg void OnSort_Bill_Folio();
	afx_msg void OnSort_Bill_TotalNights();
	afx_msg void OnSort_Bill_BalanceDue();

	afx_msg void OnSearcGuest_Clicked();
	afx_msg void OnComment_Clicked();
	afx_msg void OnPayEnrol_Clicked();
	afx_msg void OnMoneyBack_Clicked();
	afx_msg void OnPrint_Rpt();
	afx_msg void OnWizard_New();		

	afx_msg void OnMoveCompleteADODCGuest( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnMoveCompleteADODCBill( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );	
	afx_msg void OnMoveCompleteADODCAddChares( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnMoveCompleteADODCPyments( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnMoveCompleteADODCRoomUsed( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );

	afx_msg void OnError_Guest( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay );
	afx_msg void OnError_Bill( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay );
	afx_msg void OnError_AddCh( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay );
	afx_msg void OnError_Payments( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay );
	afx_msg void OnError_RoomsAlloc( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay );


	afx_msg void OnWillMoveAdodc_Bill( long adReason, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnRecordChangeComplete_Bill( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnRecordChangeComplete_Guest( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnRecordsetChangeComplete_Bill(long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnWillChangeRecordset_Bill( long adReason, long FAR* adStatus, LPDISPATCH pRecordset );
 afx_msg void OnWillChangeRecord_Bill( long adReason, long cRecords, long FAR* adStatus, LPDISPATCH pRecordset );

	afx_msg void OnRecordsetChangeComplete_AddCh( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
 afx_msg void OnRecordsetChangeComplete_Paymts( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset ); 
 afx_msg void OnRecordsetChangeComplete_Rooms( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );


	afx_msg void OnRecordChangeComplete_AddCh( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
 afx_msg void OnRecordChangeComplete_Rooms( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
	afx_msg void OnRecordChangeComplete_Paymts( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );

	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );	

	afx_msg void OnNewGuest();
	afx_msg void OnNewBill();	
	
	afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );

	void ReportErr( short DataError, short FAR* Response, int idGrid );
 void ReportErr_ADC( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay, int idADC );

	enum EN_Guest_Sorts { G_Name, G_City, G_Category, G_SizeDeposit };
	EN_Guest_Sorts m_enGuestSrt;
	CString QGuest();   		

	enum EN_Bill_Sorts { B_Date, B_Folio, B_TotalN, B_Balance };
	EN_Bill_Sorts m_enBillSrt;
	CString QBill();

	void ChangeCommand_( int iCmd );	  //0 - Guest, 1 - Bill

	void RemoveRecord_( _RecordsetPtr pRSKey, LPCTSTR lpKey, 
																					_CommandPtr pCmd, LPCTSTR lpParam );

	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual void OnCancel();

	void MakeAddRecommendedCategorues();
	void UpdateCurrGuestName();

	CFont m_fntPM, m_fntUSR;
	void SetupFnts();
	

	_CommandPtr m_pCmd_RemoveGuest, 
		 m_pCmd_Bill, m_pCmd_AddCharges, m_pCmd_RoomsAlloc;

	bool m_bFlWillAdd;

	HWND m_hwModal;

	TFieldsMgr m_mgrBind;
	CBrush hbrBkEdit, hbrBkEdit2;

	CPoint m_ptComments, m_ptAddGuest, m_ptAddBill;

	void MkVisible( CDataGrid*, bool );

	CRPEJob* m_pCRNormalJob;
	CRPEJob* m_pCRPrettyJob;

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};


#endif
