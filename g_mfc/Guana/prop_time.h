#if !defined(_PROP_TIME_)
#define _PROP_TIME_

#include "..\..\..\SEAGATE SOFTWARE\CRYSTAL REPORTS\peplus.h"


class TPropPage_Time: public CPropertyPage
 {

   friend class TMyProperty;

	  DECLARE_DYNCREATE( TPropPage_Time )

public:
		TPropPage_Time();
		~TPropPage_Time();

		enum { IDD = IDD_DIALOG_TIME };

protected:
		vector<TSD> m_vecSrc;
		void InitDScrs();

		virtual void DoDataExchange( CDataExchange* pDX ); 	

		virtual BOOL OnInitDialog();
		afx_msg void OnDestroy();	

		BOOL RegisterClasses();
		void SetTxtProp();

		TMyDataGrid m_GridLst;	
		CAdodc m_ADODCLst, m_ADODCCat;

		_RecordsetPtr m_pRec_Lst, m_pRecFind;
			bool m_bOpened, bOpenedFind;
		_CommandPtr m_pCmd_Lst, m_pCmdFind;
		//CDataCombo m_dtaLst;
		TMyDataCombo m_dtaLst;

		CDateTimeCtrl m_tm1, m_tm2;		
		CButton m_bAll, m_bSel;
		
		list<TAP_COXText> m_lstAPT;	 

		virtual BOOL PreTranslateMessage( MSG* pMsg );
		//BOOL SaveFocusControl();
		//afx_msg void OnSetFocus( CWnd* pOldWnd );

		//virtual BOOL OnKillActive();
		//virtual BOOL OnSetActive( );

		afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );

		virtual BOOL OnSetActive();
		void Refresh_Time( CDateTimeCtrl& rC, COleDateTime& rT );

		list<TAP_COXBtn> m_lstCOX;

protected:	
		CRPEJob* m_pCRJob; // AS: Crystal Reports print job

		bool m_bFlAll;
		int m_iSrt;//0 - name, 1 - date
		int m_iFltCat;
		COleDateTime m_od1, m_od2;

		void MakeRefresh();

		int GetCategory();

		CString GetQ( CString* pFltOut = NULL );
		void RefreshGrid();

		void OnDTNotify1( NMHDR *pHdr, LRESULT * pRes );

		afx_msg void OnFnam_Clicked();
		afx_msg void OnDate_Clicked(); 
		afx_msg void OnAll_Clicked(); 
		afx_msg void OnSel_Clicked(); 
		afx_msg void OnRefill_Clicked();

		afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );

		afx_msg void OnPrintRep_Clicked();

		void OnMoveCompleteADODC_Cat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset );
		void OnChangeDatacombo_Cat();

		void RefillLst();

		virtual BOOL OnApply();
		virtual void OnCancel();
		void OnJump_Clicked();
		void OnRemove_Clicked();

		bool m_bLockLst;

		DECLARE_MESSAGE_MAP()
		DECLARE_EVENTSINK_MAP()
 };



#endif
