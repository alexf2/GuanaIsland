// ReportSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "ReportSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportSelectDlg dialog


CReportSelectDlg::CReportSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportSelectDlg)
	m_iReportType = 0;
	//}}AFX_DATA_INIT
}


void CReportSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportSelectDlg)
	DDX_Radio(pDX, IDC_RADIO_ORDINAL, m_iReportType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CReportSelectDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportSelectDlg message handlers
