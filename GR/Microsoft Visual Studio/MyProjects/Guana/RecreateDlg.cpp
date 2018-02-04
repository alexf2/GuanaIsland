// RecreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "RecreateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecreateDlg dialog


CRecreateDlg::CRecreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecreateDlg)
	m_dwDataSize = 8;
	m_dwLogSize = 4;
	m_strPath = _T("C:\\MSSQL7\\Data\\");
	//}}AFX_DATA_INIT
}


void CRecreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecreateDlg)
	DDX_Text(pDX, IDC_EDIT_DSIZE, m_dwDataSize);
	DDV_MinMaxDWord(pDX, m_dwDataSize, 1, 10240);
	DDX_Text(pDX, IDC_EDIT_LSIZE, m_dwLogSize);
	DDV_MinMaxDWord(pDX, m_dwLogSize, 1, 10240);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecreateDlg, CDialog)
	//{{AFX_MSG_MAP(CRecreateDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecreateDlg message handlers

HBRUSH CRecreateDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor ) 
 {
			HBRUSH hbr = CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
   
   int id = pWnd->GetDlgCtrlID();
   if( id == IDOK )
     pDC->SetTextColor( RGB(255,0,0) );   

   return hbr;

 }

BOOL CRecreateDlg::OnInitDialog() 
 {
			CDialog::OnInitDialog();
				
			CFont *pSysFnt = CFont::FromHandle( (HFONT)GetStockObject(OEM_FIXED_FONT) );
			GetDlgItem( IDOK )->SetFont( pSysFnt );
			
			return TRUE;  // return TRUE unless you set the focus to a control
																	// EXCEPTION: OCX Property Pages should return FALSE
 }
