#if !defined(_CNCT_DLG_)
#define _CNCT_DLG_


class TCnctDlg: public CDialog
 {
public:
  TCnctDlg( CWnd *pPar ):
    CDialog( IDD_DIALOG_CONNECT, pPar )	
   { 
			  m_iSecurity = 0;
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  virtual void OnOK();

      
  int m_iSecurity;
  CString m_csServer, m_cs_DBName, m_csUser, m_csPWD;		

private:      

	 void CheckState();

		afx_msg void OnNT();
		afx_msg void OnUs();
		afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
protected:
	afx_msg void OnBtnRecreate();

  DECLARE_MESSAGE_MAP()
 };


#endif
