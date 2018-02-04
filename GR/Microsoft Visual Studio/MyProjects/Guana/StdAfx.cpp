// stdafx.cpp : source file that includes just the standard includes
//	guana.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <ocidl.h>

LPCTSTR _TM_FDATE_ = _T("M/d/yyyy");
LPCTSTR _TM_MONEY_ = _T("#,##0.00;(#,##0.00)");

LPCTSTR arrST[] =
	{
			_T("AL"), _T("AK"), _T("AZ"), _T("AR"), _T("CA"), _T("CO"), 
			_T("CT"), _T("DE"), _T("FL"), _T("GA"), _T("HI"), _T("ID"), 
			_T("IL"), _T("IN"), _T("IA"), _T("KS"), _T("KY"), _T("LA"), 
			_T("ME"), _T("MD"),_T("MA"), _T("MI"), _T("MN"), _T("MS"), 
			_T("MO"), _T("MT"), _T("NE"), _T("NV"), _T("NH"), _T("NJ"),
			_T("NM"), _T("NY"), _T("NC"), _T("ND"), _T("OH"), _T("OK"), 
			_T("OR"), _T("PA"), _T("PR"), _T("RI"), _T("SC"), _T("SD"), 
			_T("TN"), _T("TX"), _T("UT"), _T("VT"), _T("VA"), _T("WA"), 
			_T("WV"), _T("WI"), _T("WY"), NULL
	};

void SetupGridClms( CDataGrid& rG, TClmDef* pcd )
 {   
			CDC dc;
			dc.CreateCompatibleDC(&CWindowDC(CWnd::FromHandle(HWND(rG.GetHWnd()))));

			HFONT hFont;
			
			//IFontPtr pIFontCell = IDispatchPtr(rG.GetFont().m_lpDispatch);
			IFontPtr pIFontCell = rG.GetFont().m_lpDispatch;
		/*	pIFontCell->get_hFont(&hFont);
			pIFontCell->AddRefHfont(hFont);
			CFont* pCellFont = CFont::FromHandle(hFont);*/

			IFontPtr pIFontHead = IDispatchPtr(rG.GetHeadFont().m_lpDispatch);
			pIFontHead->get_hFont(&hFont);
			pIFontHead->AddRefHfont(hFont);
			CFont* pHeadFont = CFont::FromHandle(hFont);
			
			CFont *pFOld = (CFont*)dc.SelectObject( pHeadFont );

			TEXTMETRICOLE tm;
			pIFontCell->QueryTextMetrics(&tm);

			CColumns Columns = rG.GetColumns();
			long nColumns = Columns.GetCount();
			while( nColumns-- ) 
				{
						CColumn Column = Columns.GetItem(_variant_t(nColumns));
						CString strDataField = Column.GetDataField();

						bool bFinded = false;
						for( TClmDef* p = pcd; p->m_sName; ++p )
							if( strDataField == p->m_sName ) 
								{
										Column.SetCaption( p->m_sDispl );
										Column.SetAlignment( p->m_lAln );														
										Column.SetButton( p->m_bF&1 );
										Column.SetLocked( p->m_bF&2 );

										CSize szTxt( dc.GetTextExtent(Column.GetCaption()) );
										Column.SetWidth( max(szTxt.cx, tm.tmAveCharWidth*(p->m_bWidth)) );
										if( p->m_lpFormat )
										  Column.SetNumberFormat( p->m_lpFormat );

										bFinded = true;
										break;
							}
						if( !bFinded ) Column.SetVisible( FALSE );
			 }

			dc.SelectObject( pFOld );
		//	pIFontCell->ReleaseHfont(HFONT(*pCellFont));
			pIFontHead->ReleaseHfont( HFONT(*pHeadFont) );

			rG.HoldFields();
 }

int FindToolTip( TToolTipData* pD, HWND hwnd, CPoint point, TOOLINFO* pTI )
	{
   TToolTipData *pDta = NULL;
   CRect r;
   HWND hw;
   for( ; pD->m_uiId; ++pD )
				{
						hw = ::GetDlgItem( hwnd, pD->m_uiId );
						if( !hw ) continue;
						::GetWindowRect( hw, &r );
						CPoint pp1( r.left, r.top ), pp2( r.right, r.bottom );
						::ScreenToClient( hwnd, &pp1 ), ::ScreenToClient( hwnd, &pp2 );
						r.left = pp1.x; r.top = pp1.y;
						r.right = pp2.x; r.bottom = pp2.y;
						if( r.PtInRect(point) == TRUE ) 
							{
									pDta = pD;
						   break;
							}
				}   

   if( !pDta ) return -1;

   //pTI->uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   if( pTI )
				{ 
      pTI->uFlags = TTF_IDISHWND;
      pTI->hwnd = hwnd;
      pTI->uId = (UINT)hw;
      pTI->rect = r;
      //pTI->hinst = AfxGetResourceHandle();
      pTI->hinst = 0;
      //pTI->lpszText = MAKEINTRESOURCE( m_uiCommand );
      pTI->lpszText = _strdup( (LPCSTR)(pDta->m_csTxt) );
				}
   else return ::GetDlgCtrlID( hw );

   return 1;
	}

void MakeSubcass( TSubclassCtrls* pDta, CDialog *pDlg, CDataExchange* pDX )
	{
	  for( ; pDta->id; ++pDta )
				  DDX_Control( pDX, pDta->id, *pDta->pWnd );
	}

void SetupTextProp( TLds* pLds, CWnd* pWndp, list<TAP_COXText>& rLst  )
	{
	  TMyProperty *pPar_ = reinterpret_cast<TMyProperty*>( pWndp->GetParent() );

			for( TLds* p = pLds; p->pGr; ++p )
				{
						CString str;
						pWndp->GetDlgItemText( p->id, str );
						CWnd *pW = pWndp->GetDlgItem( p->id );
						CRect r;						
						pW->GetWindowRect( &r );
						pWndp->ScreenToClient( &r );
						pW->DestroyWindow();			
						int iH = r.Height();
						r.InflateRect( 0, 2 );

						TAP_COXText ap( new COXStaticText() ); 						

						
						pPar_->m_mapCapt.insert( pair<LPVOID, CString>(p->pGr, str) );
						
						ap->Create( str, WS_VISIBLE, r, pWndp, p->id );
						
						ap->SetTextColor( RGB(0, 0, 255) );
						ap->SetBkColor( RGB(192, 192, 192) );			
						ap->SetFontHeight( iH );
						ap->SetFontAttr( OX_BOLD_FONT );
						ap->SetCharSet( ANSI_CHARSET );
						ap->SetFontName( _T("Arial") );			

						ap->SetEmboss( TRUE, TRUE );
						//ap->Set3Doffset( 1 );
						//m_statTxt_Rooms.SetRaisedEdge( TRUE );

						ap->SetGraphicsMode( GM_COMPATIBLE, TRUE );

						rLst.push_back( ap );
				}
	}

void MakeDefCOXBtns( TBtnInf* pI, CWnd* pWndp, list<TAP_COXBtn>& rLst )
	{
	  CFont *pFnt = CFont::FromHandle((HFONT)::GetStockObject(SYSTEM_FIXED_FONT));

			for( ; pI->lp; ++pI )
				{
				  TAP_COXBtn ap( new COXBitmapButton() );

						pWndp->GetDlgItem( pI->id )->SetFont( pFnt );		

						ap->SubclassDlgItem( pI->id, pWndp );		
						ap->ModifyStyle( 0, BS_OWNERDRAW );
						ap->SetToolTipText( pI->lp );
						ap->SetToolTip( TRUE );
						ap->SetTrackLook( TRUE );		
						//ap->SizeToContent();
						ap->SetTextColor( RGB(0,0,255) );				

			   rLst.push_back( ap );
				}
	}


int GetProfileValueInt( LPCTSTR lpszSection, LPCTSTR lpszItem, int iDef )
 {
			int nValue = AfxGetApp()->GetProfileInt( lpszSection, lpszItem, -1 );
			if( nValue == -1 )
				{
						nValue = iDef;
						AfxGetApp()->WriteProfileInt( lpszSection, lpszItem, iDef );
				}
			return nValue;
 }

float GetProfileValueFloat( LPCTSTR lpszSection, LPCTSTR lpszItem, float fDef )
 {
			CString sValue = AfxGetApp()->GetProfileString( lpszSection, lpszItem, _T("__NOT DEFINED__") );
			if( sValue == _T("__NOT DEFINED__") )
				{
						sValue.Format( _T("%f"), fDef );
						AfxGetApp()->WriteProfileString( lpszSection, lpszItem, sValue );
				}
			LPTSTR lpTmp;
			return strtod( (LPCTSTR)sValue, &lpTmp );
 }

CString GetProfileValueStr( LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpDef )
 {
			CString sValue = AfxGetApp()->GetProfileString( lpszSection, lpszItem, _T("__NOT DEFINED__") );
			if( sValue == _T("__NOT DEFINED__") )
				{
						sValue = lpDef;
						AfxGetApp()->WriteProfileString( lpszSection, lpszItem, sValue );
				}
			return sValue;
 }


void SetProfileValueInt( LPCTSTR lpszSection, LPCTSTR lpszItem, int iVal )
	{
	  if( AfxGetApp()->GetProfileInt(lpszSection, lpszItem, -1) != iVal )
				  AfxGetApp()->WriteProfileInt( lpszSection, lpszItem, iVal );
	}
void SetProfileValueStr( LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpVal )
	{
	  if( AfxGetApp()->GetProfileString(lpszSection, lpszItem, _T("__NOT DEFINED__")) != lpVal )
				  AfxGetApp()->WriteProfileString( lpszSection, lpszItem, lpVal );
	}

void SetProfileValueFloat( LPCTSTR lpszSection, LPCTSTR lpszItem, float fVal )
	{
	  CString sValue = AfxGetApp()->GetProfileString( lpszSection, lpszItem, _T("__NOT DEFINED__") );			
			sValue.Format( _T("%f"), fVal );

	  if( AfxGetApp()->GetProfileString(lpszSection, lpszItem, _T("__NOT DEFINED__")) != sValue )
				  AfxGetApp()->WriteProfileString( lpszSection, lpszItem, sValue );
	}

void TZoomer::Zoom( CDataGrid *pGrid, CRect& rectNew )
	{
	  m_pGrid = pGrid;
			CWnd *pWPriv = pGrid->GetWindow( GW_HWNDPREV );

	  m_hwAfter = pWPriv ? pWPriv->m_hWnd:NULL;
			CRect r; pGrid->GetWindowRect( &r );
			pGrid->GetParent()->ScreenToClient( &r );

   x = r.left, y = r.top, cx = r.Width(), cy = r.Height();

			//m_pGrid->ShowWindow( SW_HIDE );
			//HWND pWW = ((TMyDataGrid*)m_pGrid)->GetCtrlWnd();

			COleControlSite* pWW = ((TMyDataGrid*)m_pGrid)->GetCtrlWnd();

			//pWW->ModifyStyle( 0, WS_OVERLAPPED, 0 );
			pWW->SetWindowPos( &CWnd::wndTop, rectNew.left, rectNew.top, 
			                rectNew.right, rectNew.bottom, 0 );

			//::MoveWindow( pWW->m_hWnd, rectNew.left, rectNew.top, 
			  //            rectNew.right, rectNew.bottom, TRUE );

			
			//m_pGrid->ShowWindow( SW_SHOW );

			m_bFl = true;
	}
void TZoomer::Restore()
	{
	   //m_pGrid->ShowWindow( SW_HIDE );
	  COleControlSite* pWW = ((TMyDataGrid*)m_pGrid)->GetCtrlWnd();
			//pWW->ModifyStyle( WS_OVERLAPPED, 0, 0 );
	  pWW->SetWindowPos( CWnd::FromHandle(m_hwAfter), x, y, 
			                cx, cy, 0 );
			//::MoveWindow( (HWND)m_pGrid->GetHWnd(), x, y, 
			  //              cx, cy, TRUE );
			
			//m_pGrid->ShowWindow( SW_SHOW );
			m_bFl = false;
	}

void MakeUpdate( CDataGrid *pG, CWnd *pW )
	{
	  try {
				  if( pG->GetDataChanged() == TRUE || pG->GetEditActive() )
							{
									if( pG->GetEditActive() ) 
										 pG->SetEditActive( FALSE ), pG->SetEditActive( TRUE );

									IDispatchPtr dspPtr( pG->GetDataSource() );
									COleDispatchDriver drv( dspPtr );

									LPDISPATCH pDispatch;
									drv.InvokeHelper( 0x18, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL );
									C_Recordset rc( pDispatch );

									_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
									rc.Update( vtEmpty, vtEmpty );
							}
						else
								pW->MessageBox( _T("No updates - data was not changed."),  _T("Information"), MB_OK|MB_ICONINFORMATION );
				}
			catch( _com_error &e )
				{
						ShowErr( pW, e );																		
						pG->SetFocus();
				}			
			catch( CException *pE )
				{
						pE->ReportError();
						pE->Delete();
						pG->SetFocus();
				}
	}


void CancelDSrec( CDataGrid* m_pWGrid, CAdodc *m_pWAdc )
	{
	  if( m_pWGrid == NULL ) return;

			m_pWGrid->SetEditActive( FALSE );																														
			m_pWGrid->SetDataChanged( FALSE );

			C_Recordset rs( m_pWAdc->GetRecordset() );

			rs.CancelUpdate();

			if( rs.GetBof() == FALSE && rs.GetEof() == FALSE && rs.GetRecordCount() )				
				try {
						if( rs.GetAbsolutePosition() == rs.GetRecordCount() );							
				}
			catch( CException *pE )
				{
				  pE->Delete();
						rs.MoveLast();
				}
	}
