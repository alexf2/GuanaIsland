// TSearchGuest.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "TSearchGuest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TSearchGuest dialog


TSearchGuest::TSearchGuest( _ConnectionPtr &rConn, _RecordsetPtr& rRec, CWnd* pParent /*=NULL*/)
	: CDialog(TSearchGuest::IDD, pParent),
	  m_rRec( rRec ),
			m_rConn( rConn ),
			m_bFlRestart( true ),
			m_bInit( false )
{
	//{{AFX_DATA_INIT(TSearchGuest)
	m_csFName = _T("%");
	m_csLName = _T("A_%");
	//}}AFX_DATA_INIT
}


void TSearchGuest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TSearchGuest)
	DDX_Control(pDX, IDC_EDIT_LNAME, m_edLName);
	DDX_Control(pDX, IDC_EDIT_FNAME, m_edFName);
	DDX_Text(pDX, IDC_EDIT_FNAME, m_csFName);
	DDX_Text(pDX, IDC_EDIT_LNAME, m_csLName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TSearchGuest, CDialog)
	//{{AFX_MSG_MAP(TSearchGuest)
	ON_WM_SHOWWINDOW()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()	
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TSearchGuest message handlers

BOOL TSearchGuest::OnInitDialog() 
{
	 CDialog::OnInitDialog();
	
		static TBtnInf iIds[] =
					{
							{ IDOK, _T("Go to next record") }, 						
							{ IDCANCEL, _T("Close search") }, 													
							{ 0, NULL },
					};

		MakeDefCOXBtns( iIds, this, m_lstCOX );

		try {
					MyCOMChk( m_pCmdQ.CreateInstance(_T("ADODB.Command")) );
					MyCOMChk( m_pRecQ.CreateInstance(_T("ADODB.Recordset")) );			
					m_bInit = true;
					
					m_pCmdQ->ActiveConnection = m_rConn;
					m_pCmdQ->CommandType = adCmdText; //where FNAME like ? and LNAME like ?
					m_pCmdQ->CommandText = _T("select ID_GUEST from Guest where FNAME like ? and LNAME like ?");						
					m_pCmdQ->Prepared = VARIANT_TRUE;
					
					m_pRecQ->CursorLocation = adUseClient;
					m_pRecQ->CursorType = adOpenKeyset;
					m_pRecQ->LockType = adLockOptimistic;
					//m_vecSrc[0].m_pRec->PutRefActiveConnection( pPar->m_pConnection );
					m_pRecQ->PutRefSource( m_pCmdQ );

					
					m_pCmdQ->Parameters->Append(
								m_pCmdQ->CreateParameter(
											_T("FNAME"), adBSTR, adParamInput, 
											-1, _variant_t(_bstr_t(_T("%")))
									) 
						);
					m_pCmdQ->Parameters->Append(
								m_pCmdQ->CreateParameter(
											_T("LNAME"), adBSTR, adParamInput, 
											-1, _variant_t(_bstr_t(_T("%")))
									) 
						);
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );						
				}						
	
	 return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void TSearchGuest::OnCancel()
	{
	  CDialog::OnCancel();
	}
void TSearchGuest::OnOK()
	{
   if( m_rRec->RecordCount == 0 )
				{
      MessageBox( _T("Cann't search - the recordset is empty"), _T("Warning"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}
			

			try {
						CString csFName;
						m_edFName.GetWindowText( csFName );
						CString csLName;
						m_edLName.GetWindowText( csLName );

						if( m_pRecQ->State == adStateClosed )
							{
							  m_pCmdQ->Parameters->GetItem( _T("FNAME") )->PutValue( 
										_bstr_t((LPCTSTR)csFName)
								   );			
									m_pCmdQ->Parameters->GetItem( _T("LNAME") )->PutValue( 
										_bstr_t((LPCTSTR)csLName)
								   );			

         _variant_t vNull;
						   vNull.vt = VT_ERROR, vNull.scode = DISP_E_PARAMNOTFOUND;
						   m_pRecQ->Open( vNull, vNull, adOpenKeyset, adLockOptimistic, adCmdUnknown );

									m_rRec->MoveFirst();
							}
						else
						if( m_bFlRestart || csFName != m_csFName || csLName != m_csLName )
							{
							  m_pCmdQ->Parameters->GetItem( _T("FNAME") )->PutValue( 
										_bstr_t((LPCTSTR)csFName)
								   );			
									m_pCmdQ->Parameters->GetItem( _T("LNAME") )->PutValue( 
										_bstr_t((LPCTSTR)csLName)
								   );			
									m_pRecQ->Requery( adOptionUnspecified );
								 m_rRec->MoveFirst();
							}

						m_bFlRestart = false;

						m_csFName = csFName, m_csLName = csLName;

						if( m_pRecQ->RecordCount == 0 )
							{
         MessageBox( _T("No records found"), _T("Info"), MB_OK|MB_ICONINFORMATION );
									return;
							}

						if( m_pRecQ->EndOfFile == VARIANT_TRUE )				
							{
									if( MessageBox(_T("End of record. Restart ?"), _T("Q"), MB_YESNO|MB_ICONQUESTION) == IDNO ) return;
									else m_rRec->MoveFirst(), m_pRecQ->MoveFirst();
							}

						
						CString str; 
						//str.Format( _T("FNAME=%s and LNAME=%s"), (LPCTSTR)csFName, (LPCTSTR)csLName );
						str.Format( _T("ID_GUEST=%lu"), (long)m_pRecQ->Fields->GetItem( _T("ID_GUEST") )->Value );

						m_rRec->MoveFirst();
						if( FAILED(m_rRec->Find( _bstr_t((LPCTSTR)str), 0L, adSearchForward, _variant_t(0L))) )
							{
							  MessageBox( _T("Cann't find this record"), _T("Info"), MB_OK|MB_ICONINFORMATION );
									return;
							}
						m_pRecQ->MoveNext();
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );	
				}
	}


void TSearchGuest::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	 CDialog::OnShowWindow( bShow, nStatus );

		if( bShow == TRUE ) m_bFlRestart = true;			
		try {
			  if( m_bInit && bShow == FALSE && m_pRecQ->State == adStateOpen ) m_pRecQ->Close();
				}
		catch( _com_error &e )
			{
					ShowErr( this, e );						
			}						
}

void TSearchGuest::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
	{
	  CDialog::OnActivate( nState, pWndOther, bMinimized );

			if( nState != WA_INACTIVE ) return;

			if( pWndOther && pWndOther->m_hWnd != GetParent()->m_hWnd ) return;

			m_bFlRestart = true;
			try {
			  if( m_bInit && m_pRecQ->State == adStateOpen ) m_pRecQ->Close();
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );						
				}						
	}

void TSearchGuest::OnKillFocus(CWnd* pNewWnd) 
 {
			CDialog::OnKillFocus(pNewWnd);					
 }

void TSearchGuest::OnDestroy() 
 {
	  CDialog::OnDestroy();

			try {
			  if( m_bInit && m_pRecQ->State == adStateOpen ) m_pRecQ->Close();
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );						
				}						
 }

