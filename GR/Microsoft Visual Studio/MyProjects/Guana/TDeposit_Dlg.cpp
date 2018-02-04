// TDeposit_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "limits.h"
#include "float.h"
#include "guana.h"
#include "TDeposit_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TDeposit_Dlg dialog


TDeposit_Dlg::TDeposit_Dlg( COleCurrency& cr, CWnd* pParent /*=NULL*/)
	: CDialog(TDeposit_Dlg::IDD, pParent)
{
  //LPTSTR lp;
  //m_dDeposit = strtod( (LPTSTR)(LPCTSTR)str, &lp );
	//{{AFX_DATA_INIT(TDeposit_Dlg)
	m_ocValue = COleCurrency(0, 0);
	//}}AFX_DATA_INIT

 	if( cr.GetStatus() == COleCurrency::valid )
	   m_ocDeposit = m_ocValue = cr;
	 else m_ocDeposit = m_ocValue = COleCurrency(0, 0);
}


void TDeposit_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TDeposit_Dlg)
	DDX_Text(pDX, IDC_EDIT_DEPOS, m_ocValue);
	//}}AFX_DATA_MAP
	 /*if( pDX->m_bSaveAndValidate == FALSE ) m_dRead = m_dDeposit;
	 DDX_Text( pDX, IDC_EDIT_DEPOS, m_dRead );
		DDV_MinMaxDouble( pDX, m_dRead, 0, DBL_MAX );	 */
}


BEGIN_MESSAGE_MAP(TDeposit_Dlg, CDialog)
	//{{AFX_MSG_MAP(TDeposit_Dlg)
	ON_BN_CLICKED(IDC_BUTTON_SUB, OnButtonSub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TDeposit_Dlg message handlers

void TDeposit_Dlg::OnOK() 
{
	if( !UpdateData(TRUE) ) return;

	//m_dDeposit += m_dRead;
	COleCurrency key( m_ocDeposit );
	m_ocDeposit += m_ocValue;
	if( CheckDep(m_ocDeposit) == false )
		{
		  m_ocDeposit = key; return;
		}

	CDialog::OnOK();
}

bool TDeposit_Dlg::CheckDep( COleCurrency& cr )
	{
	  COleCurrency cr0( 0, 0 );

			if( cr < cr0 )
				{
				  MessageBox( _T("Deposit must been positive value"), _T("Error"), MB_OK|MB_ICONEXCLAMATION );
				  return false;
				}
			return true;
	}

void TDeposit_Dlg::OnCancel() 
{	
	CDialog::OnCancel();
}

void TDeposit_Dlg::OnButtonSub() 
{
	 if( !UpdateData(TRUE) ) return;

		//m_dDeposit -= m_dRead;
		COleCurrency key( m_ocDeposit );
		m_ocDeposit -= m_ocValue;
		if( CheckDep(m_ocDeposit) == false )
		{
		  m_ocDeposit = key; return;
		}

		CDialog::OnOK();
}

BOOL TDeposit_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  static TBtnInf iIds[] =
				{
						{ IDOK, _T("Add to deposit") }, 
						{ IDCANCEL, _T("Nothing to do") },
						{ IDC_BUTTON_SUB, _T("Substract to deposit") },						
						{ 0, NULL },
				};
		MakeDefCOXBtns( iIds, this, m_lstCOX );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
