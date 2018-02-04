#include "stdafx.h"


BEGIN_MESSAGE_MAP( TLookUp, CDialog )	
	ON_WM_CREATE()	
	ON_WM_DESTROY()	
	ON_WM_SHOWWINDOW()	
	ON_WM_ACTIVATE()	
	//ON_LBN_DBCLICK( IDC_LIST, OnLbSel )	
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( MyLB, CListBox )	
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void MyLB::OnLButtonUp( UINT nFlags, CPoint point )
	{
	  CListBox::OnLButtonUp( nFlags, point );

			((TLookUp*)m_pL)->OnLbSel();
			//m_rL.OnLbSel();
	}

void TLookUp::OnDestroy() 
 {    
			CDialog::OnDestroy();			
 }


void TLookUp::OnLbSel()
	{
	  if( m_bHandle == false ) return;			

			//EndModalLoop( IDOK );
		 OnOK();
	}

TLookUp::~TLookUp()
	{
	}

void TLookUp::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
	{
	  CDialog::OnActivate( nState, pWndOther, bMinimized );
			if( m_bPassed == false && nState == WA_INACTIVE )
			  OnCancel(); 
	}


void TLookUp::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );               

   DDX_Control( pDX, IDC_LIST, m_lBox );					
 }


void TLookUp::ReFill()
	{   
			if( m_bUnboundMode == false )
				{
						m_lBox.ResetContent();
						m_lst.clear();

						try {
									MyCOMChk( m_pRecLook->MoveFirst() );
									while( m_pRecLook->EndOfFile == VARIANT_FALSE )
										{
												TAP_LookSlot ap( new TLookSlot() );

												ap->key = (long)m_pRecLook->Fields->GetItem( (LPCTSTR)m_sKey )->Value;	 
												ap->val = (LPCTSTR)(_bstr_t)m_pRecLook->Fields->GetItem( (LPCTSTR)m_sFieldLook )->Value;

												m_lst.push_back( ap );
												m_lBox.SetItemData( m_lBox.AddString((LPCSTR)ap->val), (DWORD)ap.get() );										 

												m_pRecLook->MoveNext();									
										}

									MyCOMChk( m_pRecLook->MoveFirst() );
							}
						catch( _com_error &e )
							{
									ShowErr( this, e );	
									return;
							}
				}
			else
				{
				  m_lBox.ResetContent();

						TLST_LookSlot::iterator it( m_lst.begin() );
						TLST_LookSlot::iterator itEnd( m_lst.end() );
						
						for( ; it != itEnd; ++it )						
			     m_lBox.SetItemData( m_lBox.AddString((LPCSTR)(*it)->val), (DWORD)(*it).get() );										 						
				}
	}

void TLookUp::Ass( _RecordsetPtr pRecMain, _RecordsetPtr pRecLook,
	 LPCTSTR lpKey, LPCTSTR lpFieldLook, LPCTSTR lpFieldReplicat )
	{
			m_pRecMain = pRecMain; m_pRecLook = pRecLook;
			m_sKey = lpKey; 
			m_sFieldLook = lpFieldLook;
			m_sFieldReplicat = lpFieldReplicat;			
	}

void TLookUp::Ass( _RecordsetPtr pRecMain, LPCTSTR lpFieldReplicat,
			 LPCTSTR* lpArr )
	{
   m_bUnboundMode = true;

	  m_pRecMain = pRecMain;
			m_sFieldReplicat = lpFieldReplicat;			

			for( ; *lpArr; ++lpArr )
				{
						TAP_LookSlot ap( new TLookSlot() );
						
						ap->val = *lpArr;

						m_lst.push_back( ap );						
				}
	}

void TLookUp::OnShowWindow( BOOL bShow, UINT nStatus )
	{	  
	  CDialog::OnShowWindow( bShow, nStatus );
			m_bPassed = false; 

			if( bShow == TRUE )
				{
      CWnd edW; edW.Attach( (HWND)m_pGr->GetHWndEditor() );
						CRect r; edW.GetWindowRect( &r );
						edW.Detach();
						//m_pGr->ClientToScreen( &r );
						//GetParent()->ScreenToClient( &r );
      
						SetWindowPos( NULL, r.left, r.bottom, r.Width(), 150, 
							             SWP_NOZORDER );						
						GetClientRect( &r );
						GetDlgItem( IDC_LIST )->SetWindowPos( NULL, 
							 r.left, r.top, r.right, r.bottom, SWP_NOZORDER );

						bool bFnd = false;
						unsigned int i = 0;
						if( m_bUnboundMode == false ) 
							{
									long lSeek;
									if( m_pRecMain->BOF || m_pRecMain->EndOfFile ) lSeek = 0;
									else lSeek = m_pRecMain->Fields->GetItem( (LPCTSTR)m_sKey )->Value;	 

									/*if( lSeek == 0 )
										{
            GetParent()->MessageBox( _T("Insert possible only after updating parent record"),
													 _T("Warning"), MB_OK|MB_ICONEXCLAMATION );
												PostMessage( WM_CLOSE, 0, 0 );
												return;
										}*/

									TLST_LookSlot::iterator it( m_lst.begin() );
									TLST_LookSlot::iterator itEnd( m_lst.end() );
									
									for( ; it != itEnd; ++it, ++i )
											if( (*it)->key == lSeek ) { bFnd = true; break; }
							}
						else
							{
							  CString str;
									if( m_pRecMain->BOF || m_pRecMain->EndOfFile ) str = _T("");
									else 
										{
										  _variant_t vt( m_pRecMain->Fields->GetItem( (LPCTSTR)m_sFieldReplicat )->Value );
												if( vt.vt == VT_NULL )
													{													  
               if( GetParent() )
																 GetParent()->MessageBox( _T("Cann't edit field - needed primary key"), _T("Error"), MB_ICONSTOP|MB_OK );
															else ::MessageBox( NULL, _T("Cann't edit field - needed primary key"), _T("Error"), MB_ICONSTOP|MB_OK );
															OnCancel();
															return;
													}
										  str = (LPCTSTR)(_bstr_t)vt;	 
										}

									TLST_LookSlot::iterator it( m_lst.begin() );
									TLST_LookSlot::iterator itEnd( m_lst.end() );
									
									for( ; it != itEnd; ++it, ++i )
											if( (*it)->val == str ) { bFnd = true; break; }
							}

						m_bHandle = false;
						m_lBox.SetCurSel( bFnd == true ? i:0 );
						m_bHandle = true;
				}			
	}

int TLookUp::OnCreate( LPCREATESTRUCT lpC )
 {    
   if( CDialog::OnCreate(lpC) == -1 ) return -1;    					

   return 0;
 }
void TLookUp::OnCancel()
	{ 
	  m_bPassed = true;

	  CDialog::OnCancel();
	  //EndModalLoop( IDCANCEL );
	}
static CColumn FinItem( CDataGrid& rG, LPCTSTR lpNameData )
	{
	  CColumns clms( rG.GetColumns() );
			for( long i = clms.GetCount() - 1; i >= 0; --i )
				 if( clms.GetItem(_variant_t(i)).GetDataField().CompareNoCase(lpNameData) == 0 )
						 return clms.GetItem(_variant_t(i));

			return CColumn();
	}
void TLookUp::OnOK()
 {   	  
   m_bPassed = true;

   long lSel = m_lBox.GetCurSel();
			if( lSel == LB_ERR ) OnCancel();

			try {				  				  
					 TLookSlot *p = (TLookSlot*)m_lBox.GetItemData( lSel );
						if( m_bUnboundMode == false ) 
							{
									m_pRecMain->Fields->GetItem( (LPCTSTR)m_sKey )->Value = (long)p->key;
									m_pRecMain->Fields->GetItem( (LPCTSTR)m_sFieldReplicat )->Value = (LPCTSTR)p->val;
							}
						else
							 m_pRecMain->Fields->GetItem( (LPCTSTR)m_sFieldReplicat )->Value = (LPCTSTR)p->val;

						//m_pGr->SetEditActive( TRUE ); 
						//FinItem( *m_pGr, m_sFieldReplicat ).SetText( (LPCTSTR)p->val );						
      m_pGr->SetCurrentCellModified( TRUE );
						//m_pGr->SetEditActive( FALSE ); 
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

   CDialog::OnOK();
 }

BOOL TLookUp::OnInitDialog()
 {  
   CDialog::OnInitDialog();			

			UpdateData( FALSE );			
   ReFill();			
			
   return FALSE;
 }

