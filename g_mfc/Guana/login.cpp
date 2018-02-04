#include "stdafx.h"

void TLoginDlg::DoDataExchange(CDataExchange* pDX)
 {
	  CDialog::DoDataExchange( pDX );	

			DDX_Text( pDX, IDC_EDIT_PASSW, csPwd );
 }

BEGIN_MESSAGE_MAP( TLoginDlg, CDialog )
END_MESSAGE_MAP()




