#include "stdafx.h"


BEGIN_MESSAGE_MAP( TCommentsDlg, CDialog )	
	ON_WM_CREATE()	
	//ON_WM_DESTROY()	
END_MESSAGE_MAP()


/*void TCommentsDlg::OnDestroy() 
 {    
			CDialog::OnDestroy();						
 }*/

void TCommentsDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );               

   DDX_Control( pDX, IDC_EDIT_TXT, m_Ed );						
			DDX_Text( pDX, IDC_EDIT_TXT, str );
 }


TCommentsDlg::~TCommentsDlg()
	{
	}

int TCommentsDlg::OnCreate( LPCREATESTRUCT lpC )
{    
  if( CDialog::OnCreate(lpC) == -1 ) return -1;

		//UpdateData( FALSE );

		try {		  
			  if( m_rRec->BOF == VARIANT_TRUE ||
							  m_rRec->EndOfFile == VARIANT_TRUE ||
									m_rRec->RecordCount == 0 ||
									m_rRec->AbsolutePosition == adPosUnknown 
							)
						{
						  MessageBox( _T("Cann't edit"), _T("Error"), MB_OK|MB_ICONSTOP );
								return -1;
						}
					_variant_t vvt( m_rRec->Fields->GetItem(_T("COMMENTS"))->Value );
					if( vvt.vt != VT_NULL ) str = (LPCTSTR)(_bstr_t)vvt;
					else str.Empty();
			}
		catch( _com_error &e )
	  {
	    ShowErr( this, e );	
					return -1;
	  }
    
  return 0;
}


void TCommentsDlg::OnCancel()
	{
	  CDialog::OnCancel();
	}
void TCommentsDlg::OnOK()
 {   
   CDialog::OnOK();

	  CRect r; GetWindowRect( r );
   m_rPt.x = r.left; m_rPt.y = r.top;

			try {
				  str.TrimRight();
						_variant_t vvt;
						
						if( str.IsEmpty() ) vvt.vt = VT_NULL;
						else vvt = (LPCTSTR)str;

						if( str.GetLength() > 255 )
							{
							  MessageBox( _T("Data truncatec to 255 symbols"), _T("Warning"), 
										MB_OK|MB_ICONEXCLAMATION );
									str = str.Left( 255 );
							}

			   m_rRec->Fields->GetItem(_T("COMMENTS"))->Value = vvt;
						m_pG->SetDataChanged( TRUE );
				}
			catch( _com_error &e )
	   {
	     ShowErr( this, e );					
	   }			   
 }


BOOL TCommentsDlg::OnInitDialog()
 {  
   CDialog::OnInitDialog();			

			UpdateData( FALSE );

			//m_Ed.SetWindowText( str );
			
			static TBtnInf iIds[] =
				{
						{ IDOK, _T("Close dialog and commit all changes") }, 
						{ IDCANCEL, _T("Close dialog and discard all changes") },				
						{ 0, NULL },
				};
			MakeDefCOXBtns( iIds, this, m_lstCOX );							
 
   CRect r; GetDesktopWindow()->GetWindowRect( r );
   if( !r.PtInRect(m_rPt) )
     CenterWindow( GetParent() );
   else
	  SetWindowPos( NULL, m_rPt.x, m_rPt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );

						
   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_TXT );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

