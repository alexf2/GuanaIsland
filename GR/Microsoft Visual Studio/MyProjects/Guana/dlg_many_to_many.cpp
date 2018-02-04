#include "stdafx.h"


BEGIN_MESSAGE_MAP( TManyToMany, CDialog )	
	ON_WM_CREATE()	
	ON_WM_DESTROY()
	ON_BN_CLICKED( IDC_BUTTON_REM, OnRem )
	ON_BN_CLICKED( IDC_BUTTON_ADD, OnAdd )
END_MESSAGE_MAP()


void TManyToMany::OnDestroy() 
 {    
			CDialog::OnDestroy();			

			ClosePendingTrans( false );			
 }


TToolTipData TManyToMany::m_vTD[ 3 ] =
	{
	  TToolTipData( IDC_BUTTON_REM, _T("Remove selected categoryes") ),
			TToolTipData( IDC_BUTTON_ADD, _T("Add selected categoryes") ),
			TToolTipData( 0, _T("") )			
	};

void TManyToMany::OnRem()
	{
	
	  try {
						if( m_pRec1->BOF == VARIANT_TRUE || 
							   m_pRec1->EndOfFile == VARIANT_TRUE ) 
							{
									MessageBox( _T("Cann't remove - no records"), _T("Info"), MB_OK|MB_ICONEXCLAMATION );
									return;
							}

						m_pCmdRemov->Parameters->GetItem( _T("id_1") )->PutValue( _variant_t((long)m_iKeyVal) );
						m_pCmdRemov->Parameters->GetItem( _T("id_2") )->PutValue( 
									m_pRec1->Fields->GetItem( (LPCTSTR)ms_lpKey2_Name )->Value
							);
						_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );

						m_pCmdRemov->Execute( &vtEmpty, &vtEmpty, adCmdText );						
			 }
			catch( _com_error &e )
	   {
	     ShowErr( this, e );	
	   }
			m_pRec1->Requery( adOptionUnspecified );
			m_pRec2->Requery( adOptionUnspecified );
	}
void TManyToMany::OnAdd()
	{
	    try {
						if( m_pRec2->BOF == VARIANT_TRUE || 
							   m_pRec2->EndOfFile == VARIANT_TRUE
								)
							{
									MessageBox( _T("Cann't add - no records"), _T("Info"), MB_OK|MB_ICONEXCLAMATION );
									return;
							}						

						m_pCmdAdd->Parameters->GetItem( _T("id_1") )->PutValue( _variant_t((long)m_iKeyVal) );
						m_pCmdAdd->Parameters->GetItem( _T("id_2") )->PutValue( 
									m_pRec2->Fields->GetItem( (LPCTSTR)ms_lpKey2_Name )->Value
							);
						_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );

						m_pCmdAdd->Execute( &vtEmpty, &vtEmpty, adCmdText );						
			 }
			catch( _com_error &e )
	   {
	     ShowErr( this, e );	
	   }
			m_pRec1->Requery( adOptionUnspecified );
			m_pRec2->Requery( adOptionUnspecified );
	}

void TManyToMany::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );               

   DDX_Control( pDX, IDC_DATAGRID_T1, m_gr1 );
			DDX_Control( pDX, IDC_DATAGRID_T2, m_gr2 );

			DDX_Control( pDX, IDC_DC_T1, m_dac1 );
			DDX_Control( pDX, IDC_DC_T2, m_dac2 );			
 }

CString TManyToMany::GetQ1()
	{
	  CString str;
			str.Format( _T("select * from %s where %s in (select %s from %s where %s.%s=%u)"), 
				  (LPCTSTR)ms_lpT2_Name,  
						(LPCTSTR)ms_lpKey2_Name,

						(LPCTSTR)ms_lpKey2_Name,
						(LPCTSTR)ms_lpTLnk_Name,

						(LPCTSTR)ms_lpTLnk_Name,
						(LPCTSTR)ms_lpKey1_Name, m_iKeyVal);
			return str;
	}
CString TManyToMany::GetQ2()
	{
	  CString str;
			str.Format( _T("select * from %s where not( %s in (select %s from %s where %s.%s=%u))"), 
				  (LPCTSTR)ms_lpT2_Name,  
						(LPCTSTR)ms_lpKey2_Name,

						(LPCTSTR)ms_lpKey2_Name,
						(LPCTSTR)ms_lpTLnk_Name,

						(LPCTSTR)ms_lpTLnk_Name,
						(LPCTSTR)ms_lpKey1_Name, m_iKeyVal);
			return str;
	}

TManyToMany::~TManyToMany()
	{
	  try {
					if( m_bR1Open )
							m_pRec1->Close();

					if( m_bR2Open )
							m_pRec2->Close();				
				}
			catch( _com_error &e )
				{
				  ShowErr( this, e );	
				}
	}

int TManyToMany::OnCreate( LPCREATESTRUCT lpC )
{    
  ASSERT( m_pCnct != NULL );
  if( CDialog::OnCreate(lpC) == -1 ) return -1;

	 //EnableToolTips( TRUE );
		//EnableTrackingToolTips( TRUE );

		//UpdateData( FALSE );

		try {
		  //if( InitDataSrc() == false ) return -1;   
			 MyCOMChk( m_pCmd1.CreateInstance(_T("ADODB.Command")) );
				MyCOMChk( m_pRec1.CreateInstance(_T("ADODB.Recordset")) );			
				MyCOMChk( m_pCmd2.CreateInstance(_T("ADODB.Command")) );
				MyCOMChk( m_pRec2.CreateInstance(_T("ADODB.Recordset")) );			

				m_pCnct->BeginTrans();
				m_bFlTrans = true;

				m_pCmd1->ActiveConnection = m_pCnct;
				m_pCmd1->CommandType = adCmdText;
				m_pCmd1->CommandText = (LPCTSTR)GetQ1();						

				m_pCmd2->ActiveConnection = m_pCnct;
				m_pCmd2->CommandType = adCmdText;
				m_pCmd2->CommandText = (LPCTSTR)GetQ2();						

				m_pRec1->CursorLocation = adUseClient;
				m_pRec1->CursorType = adOpenKeyset;
				m_pRec1->LockType = adLockOptimistic;				
				m_pRec1->PutRefSource( m_pCmd1 );

				VARIANT_BOOL vBool;
				VarBoolFromUI1( TRUE, &vBool );
				m_pCmd1->Prepared = vBool;

				_variant_t vNull;
				vNull.vt = VT_ERROR, vNull.scode = DISP_E_PARAMNOTFOUND;
				m_pRec1->Open( vNull, vNull,
						adOpenKeyset, adLockOptimistic, adCmdUnknown );
				m_bR1Open = true;

				m_pRec2->CursorLocation = adUseClient;
				m_pRec2->CursorType = adOpenKeyset;
				m_pRec2->LockType = adLockOptimistic;				
				m_pRec2->PutRefSource( m_pCmd2 );

				
				m_pCmd2->Prepared = vBool;

				m_pRec2->Open( vNull, vNull,
						adOpenKeyset, adLockOptimistic, adCmdUnknown );				
				m_bR2Open = true;

				
				CString tStr;
				tStr.Format( _T("insert into %s (%s, %s) values(?, ?)"),
					 (LPCTSTR)ms_lpTLnk_Name, (LPCTSTR)ms_lpKey1_Name, (LPCTSTR)ms_lpKey2_Name );

				MyCOMChk( m_pCmdAdd.CreateInstance(_T("ADODB.Command")) );			
				MyCOMChk( m_pCmdRemov.CreateInstance(_T("ADODB.Command")) );			
				m_pCmdAdd->ActiveConnection = m_pCnct;
				m_pCmdAdd->CommandType = adCmdText;
				m_pCmdAdd->CommandText = (LPCTSTR)tStr;
    m_pCmdAdd->Parameters->Append( 
					 m_pCmdAdd->CreateParameter( 
						  _T("id_1"), adInteger, adParamInput, 
								sizeof(int), _variant_t(0L) ) 
						);
				m_pCmdAdd->Parameters->Append( 
					 m_pCmdAdd->CreateParameter( 
						  _T("id_2"), adInteger, adParamInput, 
								sizeof(int), _variant_t(0L) ) 
						);

				m_pCmdRemov->ActiveConnection = m_pCnct;
				m_pCmdRemov->CommandType = adCmdText;

				tStr.Format( _T("delete from %s where %s=? and %s=?"),
					 (LPCTSTR)ms_lpTLnk_Name, (LPCTSTR)ms_lpKey1_Name, (LPCTSTR)ms_lpKey2_Name );
				m_pCmdRemov->CommandText = (LPCTSTR)tStr;

				m_pCmdRemov->Parameters->Append( 
					 m_pCmdRemov->CreateParameter( 
						  _T("id_1"), adInteger, adParamInput, 
								sizeof(int), _variant_t(0L) ) 
						);
				m_pCmdRemov->Parameters->Append( 
					 m_pCmdRemov->CreateParameter( 
						  _T("id_2"), adInteger, adParamInput, 
								sizeof(int), _variant_t(0L) ) 
						);

				m_pCmdAdd->Prepared = vBool, m_pCmdRemov->Prepared = vBool;
			}
		catch( _com_error &e )
	  {
	    ShowErr( this, e );	

					return -1;
	  }
    
  return 0;
}


int TManyToMany::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
 {
   return FindToolTip( m_vTD, m_hWnd, point, pTI );   
 }

void TManyToMany::ClosePendingTrans( bool bFl )
	{
	  if( m_bFlTrans )
				{
				  if( bFl == true )  m_pCnct->CommitTrans();
						else m_pCnct->RollbackTrans();

					 m_bFlTrans = false;				
				}
	}

void TManyToMany::OnCancel()
	{
   ClosePendingTrans( false );

	  CDialog::OnCancel();
	}
void TManyToMany::OnOK()
 {   
	  CRect r; GetWindowRect( r );
   m_rPt.x = r.left; m_rPt.y = r.top;

			ClosePendingTrans( true );

   CDialog::OnOK();
 }



BOOL TManyToMany::OnInitDialog()
 {  
   CDialog::OnInitDialog();			

			UpdateData( FALSE );
			
			static TBtnInf iIds[] =
				{
						{ IDOK, _T("Close dialog and commit all changes") }, 
						{ IDCANCEL, _T("Close dialog and discard all changes") },
						{ IDC_BUTTON_REM, _T("Remove selected presented records") },
						{ IDC_BUTTON_ADD, _T("Add selected avaliable") },
						{ 0, NULL },
				};
			MakeDefCOXBtns( iIds, this, m_lstCOX );
				

			m_dac1.SetRefRecordset( m_pRec1 );
			m_gr1.SetRefDataSource( m_dac1.GetControlUnknown() );						

			m_dac2.SetRefRecordset( m_pRec2 );
			m_gr2.SetRefDataSource( m_dac2.GetControlUnknown() );						

			m_gr1.SetBackColor( RGB(255,255,220) );
			m_gr2.SetBackColor( RGB(255,255,220) );
			m_gr1.GetHeadFont().SetBold( TRUE );
			m_gr2.GetHeadFont().SetBold( TRUE );

			SetupGridClms( m_gr1, pMM );
			SetupGridClms( m_gr2, pMM );
 
   CRect r; GetDesktopWindow()->GetWindowRect( r );
   if( !r.PtInRect(m_rPt) )
     CenterWindow( GetParent() );
   else
	  SetWindowPos( NULL, m_rPt.x, m_rPt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );

			SetWindowText( ms_lpTitlMain );
			m_gr1.SetCaption( ms_lpTitl1 );
			m_gr2.SetCaption( ms_lpTitl2 );
			

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_DATAGRID_T1 );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

