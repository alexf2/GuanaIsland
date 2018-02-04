#if !defined(_DLG_COMMENTS_)
#define _DLG_COMMENTS_


class TCommentsDlg: public CDialog
 {
public: 
  TCommentsDlg( CWnd *pPar, CPoint& rp, _RecordsetPtr& rRec, CDataGrid *pG ):
    CDialog( IDD_DIALOG_COMMENTS, pPar ),
				m_rPt( rp ),
				m_rRec( rRec ),
				m_pG( pG )
   { 			  					
   }
		virtual ~TCommentsDlg();		

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
		int OnCreate( LPCREATESTRUCT lpC );

  virtual void OnOK();
		virtual void OnCancel();		
		//afx_msg void OnDestroy();		
				
private:      
  _RecordsetPtr& m_rRec;  				
		CPoint &m_rPt;				
		CString str;

		CDataGrid *m_pG;

  list<TAP_COXBtn> m_lstCOX;

		CEdit m_Ed;

  DECLARE_MESSAGE_MAP()
 };

#endif
