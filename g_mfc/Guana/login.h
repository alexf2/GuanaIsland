#if !defined(_DLG_LOGIN_)
#define _DLG_LOGIN_


class TLoginDlg: public CDialog
 {
public:
	 TLoginDlg( CWnd* pParent ):
				 CDialog( IDD_DIALOG_LOGIN, pParent )
						{
						}


  CString csPwd;

protected:
	 virtual void DoDataExchange( CDataExchange* pDX );
	
protected:	
	  DECLARE_MESSAGE_MAP()
 };


#endif
