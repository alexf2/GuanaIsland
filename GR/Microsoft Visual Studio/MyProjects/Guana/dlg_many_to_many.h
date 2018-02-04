#if !defined(_MANY_TO_MANY_)
#define _MANY_TO_MANY_


class TManyToMany: public CDialog
 {
public: 
  TManyToMany( CWnd *pPar, CPoint& rp ):
    CDialog( IDD_DIALOG_MANY_TO_MANY, pPar ),
				m_rPt( rp ),
				m_bFlTrans( false ),
				m_bR1Open( false ), m_bR2Open( false )
   { 			  
					m_pCnct = NULL; m_iKeyVal = 0; pMM = NULL;
   }
		virtual ~TManyToMany();

		void Ass( 
			 _ConnectionPtr pCnct, 
				LPCTSTR lpT1_Name,
				LPCTSTR lpT2_Name,
				LPCTSTR lpTLnk_Name,
				LPCTSTR lpKey1_Name,
				LPCTSTR lpKey2_Name,

				LPCTSTR lpTitl1,
				LPCTSTR lpTitl2,
				LPCTSTR lpTitlMain,

				int iKeyVal,
				TClmDef* pCDef )
			{
			  m_pCnct = pCnct;
					ms_lpT1_Name = lpT1_Name;
					ms_lpT2_Name = lpT2_Name;
					ms_lpTLnk_Name = lpTLnk_Name;
					ms_lpKey1_Name = lpKey1_Name;
					ms_lpKey2_Name = lpKey2_Name;

					ms_lpTitl1 = lpTitl1;
					ms_lpTitl2 = lpTitl2;
					ms_lpTitlMain = lpTitlMain;

					m_iKeyVal = iKeyVal;

					pMM = pCDef;
			}

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
		int OnCreate( LPCREATESTRUCT lpC );

  virtual void OnOK();
		virtual void OnCancel();		

		virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
		afx_msg void OnDestroy();

		void OnRem();
		void OnAdd();
		
private:      
  bool m_bFlTrans;
		void ClosePendingTrans( bool bFl = false );

		_ConnectionPtr m_pCnct;

		CString ms_lpT1_Name;
		CString ms_lpT2_Name;
		CString ms_lpTLnk_Name;
		CString ms_lpKey1_Name, ms_lpKey2_Name;

		CString ms_lpTitl1;
		CString ms_lpTitl2;
		CString ms_lpTitlMain;

		int m_iKeyVal;

		CDataGrid m_gr1, m_gr2;
		CAdodc m_dac1, m_dac2;

		CString GetQ1();
		CString GetQ2();

		CPoint &m_rPt;

		_RecordsetPtr m_pRec1, m_pRec2;
		_CommandPtr m_pCmd1, m_pCmd2;

		_CommandPtr m_pCmdAdd, m_pCmdRemov;

		TClmDef* pMM;

		static TToolTipData m_vTD[ 3 ];

		//typedef auto_ptr<COXBitmapButton> TAP_COXBtn;
		list<TAP_COXBtn> m_lstCOX;

		bool m_bR1Open, m_bR2Open;

  DECLARE_MESSAGE_MAP()
 };

#endif
