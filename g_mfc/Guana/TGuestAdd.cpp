// TGuestAdd.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "ADO_DSRC\\field.h"
#include "ADO_DSRC\\fields.h"
#include "TGuestAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TGuestAdd dialog


TGuestAdd::TGuestAdd( CDataGrid *pWGrid, _RecordsetPtr& rGuest, CAdodc& rAdc, CPoint& rp, CWnd* pParent )
	: CDialog( TGuestAdd::IDD, pParent ),
	  m_rGuest( rGuest ), m_rAdc( rAdc ),
			m_rPt( rp ),
   m_pWGrid( pWGrid ),
			hbrBkEdit( RGB(240,245,255) )
{
	//{{AFX_DATA_INIT(TGuestAdd)
	m_edFName = _T("");
	m_edLName = _T("");
	m_edDeposit = COleCurrency(0, 0);
	m_edCity = _T("");
	m_edAddr = _T("");
	m_uiPost = 0;
	m_csState = _T("");
	//}}AFX_DATA_INIT
}

void TGuestAdd::ChecheckEmpty( CString& rS, LPCTSTR lpMsg, int iCtl )
	{
	  if( !rS.IsEmpty() ) return;

			MessageBox( lpMsg, _T("Error"), MB_OK|MB_ICONEXCLAMATION );
			PostMessage( WM_NEXTDLGCTL, (WPARAM)::GetDlgItem(m_hWnd, iCtl), 1 ),

			throw exception( _T("CHECKING") );
	}

void TGuestAdd::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TGuestAdd)
	DDX_Control(pDX, IDC_COMBO_STATE, m_cbnState);
	DDX_Control(pDX, IDC_DATACOMBO_CAT, m_dcCat);
	DDX_Text(pDX, IDC_EDIT_FNAME, m_edFName);
	DDX_Text(pDX, IDC_EDIT_LNAME, m_edLName);
	DDX_Text(pDX, IDC_EDIT_DEPOSIT, m_edDeposit);
	DDX_Text(pDX, IDC_EDIT_CITY, m_edCity);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_edAddr);
	DDX_Text(pDX, IDC_EDIT_POST, m_uiPost);	
	DDV_MinMaxUInt( pDX, m_uiPost, 0, UINT_MAX );
	DDX_CBString(pDX, IDC_COMBO_STATE, m_csState);
	DDV_MaxChars(pDX, m_csState, 20);
	//}}AFX_DATA_MAP
	if( pDX->m_bSaveAndValidate == TRUE )
		{
    DDV_MaxChars( pDX, m_edFName, 30 );
				DDV_MaxChars( pDX, m_edLName, 30 );
    DDV_MaxChars( pDX, m_edCity, 20 );
				DDV_MaxChars( pDX, m_edAddr, 50 );
				//DDV_MaxChars( pDX, m_edPost, 50 );				
				if( m_edDeposit.GetStatus() != COleCurrency::valid || 
					   m_edDeposit < COleCurrency(0, 0)
						)
						 MessageBox( _T("Bad value of deposit"), _T("Error"), MB_OK|MB_ICONEXCLAMATION ),
					  PostMessage( WM_NEXTDLGCTL, (WPARAM)::GetDlgItem(m_hWnd, IDC_EDIT_DEPOSIT), 1 ),
							pDX->Fail();

				try {
						 ChecheckEmpty( m_edFName, _T("Missing first name"), IDC_EDIT_FNAME );
						 ChecheckEmpty( m_edLName, _T("Missing last name"), IDC_EDIT_LNAME );
						 ChecheckEmpty( m_edCity, _T("Missing city"), IDC_EDIT_CITY );
						 ChecheckEmpty( m_edAddr, _T("Missing address"), IDC_EDIT_ADDRESS );				
					}
				catch( exception& e )
					{
					  pDX->Fail();
					}
		}
}


BEGIN_MESSAGE_MAP(TGuestAdd, CDialog)
	//{{AFX_MSG_MAP(TGuestAdd)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TGuestAdd message handlers

BOOL TGuestAdd::OnInitDialog() 
{
		CDialog::OnInitDialog();
		
		CRect r; GetDesktopWindow()->GetWindowRect( r );
		if( !r.PtInRect(m_rPt) )
				CenterWindow( GetParent() );
		else
		  SetWindowPos( NULL, m_rPt.x, m_rPt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );

  for( LPCTSTR *p = arrST; *p; ++p )
		  m_cbnState.InsertString( -1, *p );		
		m_cbnState.SetCurSel( 0 );		

		m_cbnState.GetWindowText( m_csState );

		static TBtnInf iIds[] =
				{
						{ IDOK, _T("Add new guest") }, 
						{ IDCANCEL, _T("Cancel adding guest") },
						{ IDOK2, _T("Add new guest and bill for this guest") },						
						{ 0, NULL }
				};
			MakeDefCOXBtns( iIds, this, m_lstCOX );

		try {
					m_dcCat.SetListField( _T("NAMECAT") );
					m_dcCat.SetRefRowSource( m_rAdc.GetControlUnknown() );								  
					m_rAdc.GetRecordset().MoveFirst();
					m_dcCat.SetText( 
						  _bstr_t(m_rAdc.GetRecordset().GetFields().GetItem( _variant_t(_T("NAMECAT")) ).GetValue())
						);					
			}
		 catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );						
				}		
		
		
		return FALSE;  // return TRUE unless you set the focus to a control
																// EXCEPTION: OCX Property Pages should return FALSE
}

void TGuestAdd::OnCancel() 
{
  try {
			  m_pWGrid->SetEditActive( FALSE );																														
			  m_pWGrid->SetDataChanged( FALSE );			  	    					
					//m_rGuest->Delete( adAffectCurrent );
					m_rGuest->CancelUpdate();

					if( m_rGuest->RecordCount != 0 )
					  m_rGuest->MoveLast();					
			}
		catch( _com_error &e )
				{
						ShowErr( this, e );
						//return;
				}
		catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();
				}		
	
	 CDialog::OnCancel();
}

bool TGuestAdd::MakePost()
	{
	  if( !UpdateData(TRUE) ) return false;

			CRect r; GetWindowRect( r );
   m_rPt.x = r.left; m_rPt.y = r.top;

			try {
						m_rGuest->Fields->GetItem( _T("FNAME") )->Value = _bstr_t( (LPCTSTR)m_edFName );
						m_rGuest->Fields->GetItem( _T("LNAME") )->Value = _bstr_t( (LPCTSTR)m_edLName );
						m_rGuest->Fields->GetItem( _T("CITY") )->Value = _bstr_t( (LPCTSTR)m_edCity );
						//CString str; m_cbnState.GetWindowText( str );
						m_rGuest->Fields->GetItem( _T("STATE") )->Value = _bstr_t( (LPCTSTR)m_csState );
						m_rGuest->Fields->GetItem( _T("ADDRESS") )->Value = _bstr_t( (LPCTSTR)m_edAddr );
						m_rGuest->Fields->GetItem( _T("POST") )->Value = _variant_t( (long)m_uiPost );
						m_rGuest->Fields->GetItem( _T("DEPOSIT") )->Value = (CURRENCY)m_edDeposit;

						CString str;
						C_Recordset rs( m_rAdc.GetRecordset() );
						str.Format( _T("NAMECAT='%s'"), (LPCTSTR)m_dcCat.GetText() );
						rs.MoveFirst();
						rs.Find( _bstr_t((LPCTSTR)str), 0L, adSearchForward, _variant_t(0L) );

						m_rGuest->Fields->GetItem( _T("__NAMECAT") )->Value =
							 rs.GetFields().GetItem( _variant_t(_T("NAMECAT")) ).GetValue();						  
						m_rGuest->Fields->GetItem( _T("ID_CAT") )->Value = 
								rs.GetFields().GetItem( _variant_t(_T("ID_CAT")) ).GetValue();						 
						

						_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
						m_rGuest->Update( vtEmpty, vtEmpty );

						m_pWGrid->SetFocus();
						m_pWGrid->SetEditActive( TRUE );
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return false;
				}		
			catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();
						return false;
				}

			return true;
	}

void TGuestAdd::OnOK() 
{
  if( !MakePost() ) return;
	 	
 	CDialog::OnOK();
}

void TGuestAdd::OnOk2() 
{
	 if( !MakePost() ) return;	

		EndDialog( IDOK2 );
}

BOOL TGuestAdd::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}


int TGuestAdd::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	 if (CDialog::OnCreate(lpCreateStruct) == -1)
		  return -1;

	 try {	 
			 _variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
			 m_rGuest->AddNew( vtEmpty, vtEmpty );
			}
		catch( _com_error &e )
				{
						ShowErr( this, e );
						return -1;
				}			   
	
	// TODO: Add your specialized creation code here
	
	 return 0;
}

HBRUSH TGuestAdd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
			static int arr[] = 
							{									
									IDC_EDIT_FNAME, 
									IDC_EDIT_LNAME,
									IDC_EDIT_DEPOSIT,
									IDC_EDIT_ADDRESS, 
									IDC_EDIT_CITY,
									IDC_COMBO_STATE, 
									IDC_EDIT_POST, 
									0
							};
						      
   int id = pWnd->GetDlgCtrlID();
   for( int* p = arr; *p; ++p )
				 if( *p == id )
						{
	       //pDC->SetTextColor( RGB(0,0,230) );
								pDC->SetBkColor( RGB(240,245,255) );
								return hbrBkEdit;
						}
			
   return hbr;		
}

void TGuestAdd::OnDestroy() 
{
	 CDialog::OnDestroy();
	
  hbrBkEdit.DeleteObject();
}


BEGIN_EVENTSINK_MAP( TGuestAdd, CDialog ) 
	
 ON_EVENT( TGuestAdd, IDC_DATACOMBO_CAT, -603 /* KeyPress */, OnKeyPressDatacombo1, VTS_PI2 )	

END_EVENTSINK_MAP()

void TGuestAdd::OnKeyPressDatacombo1( short FAR* KeyAscii ) 
 {
	  *KeyAscii = 0;
	  ::MessageBeep( -1 );
 }