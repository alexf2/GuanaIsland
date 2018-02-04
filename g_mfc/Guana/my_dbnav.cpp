#include "stdafx.h"

#include "my_dbnav.h"

TCreBmpDta TMyDBNav::bmps[] =
	{
			{ IDB_BITMAP_BEGIN, _T("begin") },
			{ IDB_BITMAP_PRIV, _T("priv") },
			{ IDB_BITMAP_NEXT, _T("next") },
			{ IDB_BITMAP_END, _T("end") },
			{ IDB_BITMAP_ADD, _T("add new record") },
			{ IDB_BITMAP_DEL, _T("remove current record") },
			{ IDB_BITMAP_EDIT, _T("edit current record (F2)") },
			{ IDB_BITMAP_OK, _T("post edit (F12)") },
			{ IDB_BITMAP_CANCEL, _T("cancel edit (ESC)") },
			{ IDB_BITMAP_REFRESH, _T("refresh recordset") },
			{ 0, NULL }
	};

CPoint TMyDBNav::GetBtnSize()
	{ 
	  return CPoint( BMPNAV_WIDTH, BMPNAV_HEIGHT );
	}

CPoint TMyDBNav::GetRecSize()
	{
	  CPoint pt( GetBtnSize() );
			int i = 0;
			for( TCreBmpDta* p = bmps; p->id; ++p, ++i );

	  return CPoint( pt.x * i, pt.y );
	}

int TMyDBNav::OnCreate( LPCREATESTRUCT lpC )
	{
	  //COXBitmapButton::m_ptImageOffset = CPoint( 0, 0 );

	  if( CStatic::OnCreate(lpC) == -1 ) return -1;
							
			int i = 0;
			CRect r; GetClientRect( &r ); r.left = r.top = 0;
			CPoint pt( GetBtnSize() );
			r.right = pt.x, r.bottom = pt.y;
			
			POINT ptOffs;  ptOffs.x = r.Width(), ptOffs.y = 0;
			for( TCreBmpDta* p = bmps; p->id; ++p, ++i )
				{
				  TAP_COXBtn ap( new COXBitmapButton() );
						ap->m_ptImageOffset = CPoint( 0, 0 );

						m_map.insert( pair<int, COXBitmapButton*>(IDC_NAVBTN_STA +  i, ap.get()) );

						ap->Create( _T(""), BS_OWNERDRAW|WS_VISIBLE|WS_CHILD, r, this, IDC_NAVBTN_STA +  i );
						r.OffsetRect( ptOffs );						

				  VERIFY( ap->LoadBitmap(MAKEINTRESOURCE(p->id), TRUE, RGB(192,192,192)) );
     	ap->SetToolTip( TRUE );
						ap->SetToolTipText( p->lp );
						ap->SetTrackLook( TRUE );						

			   m_vecBtn[ i ] = ap;
				}


			pt = GetRecSize();
			SetWindowPos( NULL, 0, 0, pt.x, pt.y, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );

			return 0;
	}

CDataGrid* TMyDBNav::GetGridFor( CAdodc* pWAdc )
	{
	  CWnd* pPar = m_hWnd ? GetParent():NULL;
			if( !pPar ) return NULL;

			TMyProperty* pProp = reinterpret_cast<TMyProperty*>( pPar );

			map<LPVOID, LPVOID>::iterator it( pProp->m_mapGRID_DC.begin() );
			map<LPVOID, LPVOID>::iterator itEnd( pProp->m_mapGRID_DC.end() );

			for( ; it != itEnd; ++it )
				 if( it->second == (LPVOID)pWAdc &&
						   dynamic_cast<CDataGrid*>((CWnd*)it->first) != NULL )
						 return dynamic_cast<CDataGrid*>((CWnd*)it->first);

   ASSERT( FALSE );
			return NULL;				
	}

void TMyDBNav::Assign( CWnd* pWGrid, CAdodc* pWAdc )
	{
	  m_pWGrid = GetGridFor( pWAdc );
		 m_pWAdc = pWAdc;

			if( m_hWnd == NULL ) return;

			TestState( pWAdc );

			CWnd* pPar = GetParent();
			if( pPar )
				{
				  CString str;
						pPar->GetWindowText( str );

						//CString strC( reinterpret_cast<TMyProperty*>(pPar)->GetCaptionFor(pWGrid) );
						CString strC( pWAdc->GetCaption() );
						//if( strC.IsEmpty() ) strC = pWGrid->GetCaption();

						int iInd = str.Find( _T(" # "), 0 );
						if( iInd == -1 ) str += CString(_T(" # ")) + strC;
						else str = str.Left( iInd  ) + CString(_T(" # ")) + strC;

						pPar->SetWindowText( str );
				}
	}

BOOL TMyDBNav::OnEraseBkgnd( CDC *pDC )
 {
   //CRect r; GetClientRect( r ); 
   //pDC->FillSolidRect( r, RGB(255,255,255) ); 
   return TRUE;   
 }


void TMyDBNav::TestState( CWnd* pW )
	{	  
			if( m_pWAdc != pW && m_pWGrid != pW ) return;
			ASSERT( m_pWAdc != NULL && m_pWGrid != NULL );

			C_Recordset rec( m_pWAdc->GetRecordset() );
			long lPos = rec.GetBof();

			SetBtnState( lPos != TRUE, IDC_NAVBTN_BEGIN );
			SetBtnState( lPos != TRUE, IDC_NAVBTN_PRIV );

			lPos = rec.GetEof();
			SetBtnState( lPos != TRUE, IDC_NAVBTN_NEXT );
			SetBtnState( lPos != TRUE, IDC_NAVBTN_END );

			bool bEmpty = rec.GetBof() && rec.GetEof();
			bool bEmpty2 = rec.GetBof() || rec.GetEof();

   lPos = bEmpty || bEmpty2 ? FALSE:rec.GetEditMode();
			BOOL bFA = m_pWGrid->GetAllowAddNew();
			
			SetBtnState( bFA /*lPos == adEditNone && bFA*/, IDC_NAVBTN_ADD );
			SetBtnState( lPos == adEditNone && bFA, IDC_NAVBTN_REMOV );			

			SetBtnState( false, IDC_NAVBTN_EDIT );

			SetBtnState( true/*lPos == adEditInProgress*/, IDC_NAVBTN_UPDATE );
			SetBtnState( true/*lPos == adEditInProgress*/, IDC_NAVBTN_CANCEL );

			SetBtnState( lPos == adEditNone, IDC_NAVBTN_REFRESH );						
	}

void TMyDBNav::SetBtnState( bool bFl, int id )
	{
	  if( m_hWnd == NULL ) return;

			map<int, COXBitmapButton*>::iterator it = 
			  m_map.find( id );
			if( it == m_map.end() ) return;	  			

			BOOL bFlw = it->second->IsWindowEnabled();
			if(  bFl != bool(bFlw) ) it->second->EnableWindow( bFl );  
	}

void TMyDBNav::GridEdit( CWnd* pW, bool bFl )
	{	  
			if( m_pWAdc != pW && m_pWGrid != pW ) return;
			ASSERT( m_pWAdc != NULL );			

			TestState( pW );
			/*SetBtnState( !bFl, IDC_NAVBTN_ADD );
			SetBtnState( !bFl, IDC_NAVBTN_REMOV );			

			SetBtnState( !bFl, IDC_NAVBTN_EDIT );

			SetBtnState( bFl, IDC_NAVBTN_UPDATE );
			SetBtnState( bFl, IDC_NAVBTN_CANCEL );

			SetBtnState( !bFl, IDC_NAVBTN_REFRESH );						*/
	}

void TMyDBNav::OnNav_Begin()
	{
	  if( m_pWAdc == NULL ) return;

			m_pWAdc->GetRecordset().MoveFirst();
	}
void TMyDBNav::OnNav_Priv()
	{
	  if( m_pWAdc == NULL || m_pWAdc->GetRecordset().GetBof() ) return;

			m_pWAdc->GetRecordset().MovePrevious();
	}
void TMyDBNav::OnNav_Next()
	{
	  if( m_pWAdc == NULL || m_pWAdc->GetRecordset().GetEof() ) return;

			m_pWAdc->GetRecordset().MoveNext();
	}
void TMyDBNav::OnNav_End()
	{
	  if( m_pWAdc == NULL ) return;

			m_pWAdc->GetRecordset().MoveLast();
	}
void TMyDBNav::OnNav_Add()
	{
	  if( m_pWGrid == NULL ) return;

			//m_pWAdc->GetRecordset().MoveFirst();
			_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
			m_pWAdc->GetRecordset().AddNew( vtEmpty, vtEmpty );

			/*if( m_pWAdc->GetParent() )
				{
				  CWnd *pW = m_pWAdc->GetParent();
						if( dynamic_cast<TPropPage_Bill*>(pW)	)
							 dynamic_cast<TPropPage_Bill*>(pW)->CallAddNew( m_pWAdc );
				}*/
	}
void TMyDBNav::OnNav_Remov()
	{
	  if( m_pWAdc == NULL ) return;

			C_Recordset rs( m_pWAdc->GetRecordset() );

			if( rs.GetBof() || rs.GetEof() || rs.GetRecordCount() == 0 )				 
				{
				  if( GetParent() )
							 GetParent()->MessageBox( _T("Cann't remove - no records"), _T("Warning"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}

			_variant_t vt( rs.GetBookmark() );
			long lPos = rs.GetAbsolutePosition();
			try {
		    rs.Delete( adAffectCurrent );
			 }
			catch( COleDispatchException *pE )
				{
				  if( pE->m_scError != -2147217842 ) //manual cancel delete record
				    pE->ReportError();
				  pE->Delete();

						rs.Requery( adOptionUnspecified );							
						long lCou = rs.GetRecordCount();
			   if( lPos < lCou )
						  rs.SetBookmark( vt );
						else if( lCou ) rs.MoveLast();
				}
			catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();

						rs.Requery( adOptionUnspecified );													
						long lCou = rs.GetRecordCount();
			   if( lPos < lCou )
						  rs.SetBookmark( vt );
						else if( lCou ) rs.MoveLast();
				}									
	}
void TMyDBNav::OnNav_Edit()
	{
	  if( m_pWGrid == NULL ) return;
			
			m_pWGrid->SetEditActive( TRUE );			
	}
void TMyDBNav::OnNav_Update()
	{
	  if( m_pWGrid == NULL ) return;

			//m_pWGrid->SetEditActive( FALSE );
			_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
			m_pWAdc->GetRecordset().Update( vtEmpty, vtEmpty );
	}


void TMyDBNav::OnNav_Cancel()
	{
	  CancelDSrec( m_pWGrid, m_pWAdc );
	}
void TMyDBNav::OnNav_Refresh()
	{
	  if( m_pWAdc  == NULL ) return;												

			try {
						C_Recordset rs( m_pWAdc->GetRecordset() );
						_variant_t vt( rs.GetBookmark() );
						rs.Requery( adOptionUnspecified );			
						if( rs.GetRecordCount() )
						  rs.SetBookmark( vt );
				}
			catch( CException *pE )
				{
				  pE->Delete();
				}
	}

BEGIN_MESSAGE_MAP( TMyDBNav, CStatic )
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	ON_BN_CLICKED( IDC_NAVBTN_BEGIN, OnNav_Begin )
	ON_BN_CLICKED( IDC_NAVBTN_PRIV, OnNav_Priv )
	ON_BN_CLICKED( IDC_NAVBTN_NEXT, OnNav_Next )
	ON_BN_CLICKED( IDC_NAVBTN_END, OnNav_End )
	ON_BN_CLICKED( IDC_NAVBTN_ADD, OnNav_Add )
	ON_BN_CLICKED( IDC_NAVBTN_REMOV, OnNav_Remov )
	ON_BN_CLICKED( IDC_NAVBTN_EDIT, OnNav_Edit )
	ON_BN_CLICKED( IDC_NAVBTN_UPDATE, OnNav_Update )
	ON_BN_CLICKED( IDC_NAVBTN_CANCEL, OnNav_Cancel )
	ON_BN_CLICKED( IDC_NAVBTN_REFRESH, OnNav_Refresh )

END_MESSAGE_MAP()

