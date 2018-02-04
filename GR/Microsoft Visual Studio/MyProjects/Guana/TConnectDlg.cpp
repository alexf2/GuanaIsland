// TConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "TConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TConnectDlg dialog


TConnectDlg::TConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TConnectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TConnectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TConnectDlg, CDialog)
	//{{AFX_MSG_MAP(TConnectDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TConnectDlg message handlers
