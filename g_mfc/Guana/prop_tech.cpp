#include "stdafx.h"

#include "HELP\\guana.hh"

IMPLEMENT_DYNCREATE( TPropPage_Sys, CPropertyPage )

TPropPage_Sys::TPropPage_Sys() : 
  CPropertyPage( TPropPage_Sys::IDD ),
		m_ptEditRecBill_Key( -1, -1 ),
		m_vecSrc( 4 )
 {
   m_psp.dwFlags &= ~PSP_HASHELP;	   
 }

TPropPage_Sys::~TPropPage_Sys()
 { 
 }


void TPropPage_Sys::DoDataExchange( CDataExchange* pDX )
 {
			CPropertyPage::DoDataExchange(pDX);

			TSubclassCtrls subCtrls[] =
				{
						{ IDC_DATAGRID_ROOM, &m_GridRoom },
						{ IDC_DATAGRID_BILLCAT, &m_GridBillCat },
						{ IDC_DATAGRID_GUESTCAT, &m_GridGuestCat },
						{ IDC_DATAGRID_RECOMMBILLCAT, &m_GridRecommendedBillCat },

						{ IDC_DC_ROOM, &m_ADODCRoom },
						{ IDC_DC_BILLCAT, &m_ADODCBillCat },
						{ IDC_DC_GUESTCAT, &m_ADODCGuestCat },
						{ IDC_DC_RECOMMBILLCAT, &m_ADODCRecommendedBillCat },
						{ 0, NULL }
				};

			MakeSubcass( subCtrls, this, pDX );  		
 }



void TPropPage_Sys::SetTxtProp()
	{
   TLds iLstIds[] =
				{
						{ &m_GridRoom, IDC_STATIC_ROOM }, 
						{ &m_GridBillCat, IDC_STATIC_LCAT },
						{ &m_GridGuestCat, IDC_STATIC_ALLGCAT }, 
						{ &m_GridRecommendedBillCat, IDC_STATIC_SELGCAT }, 
						{ NULL, 0 }
				};

			SetupTextProp( iLstIds, this, m_lstAPT );							
	}

BOOL TPropPage_Sys::PreTranslateMessage( MSG* pMsg )
 {

   if( pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN )
				{
				  CWnd *pW = GetFocus();
						if( pW )
							{
							  CDataGrid *pG = dynamic_cast<CDataGrid*>( pW );
									if( !pG && pW->GetParent() ) 
										 pG = dynamic_cast<CDataGrid*>( pW->GetParent() );

									if( pG )									 
									 {										            
										  if( pMsg->wParam == VK_ESCAPE )
													{
													  if( pMsg->message == WM_KEYDOWN ) return TRUE;
															
															CancelDSrec( pG, ((TMyProperty*)GetParent())->GetADC_for_Grid(pG) );
															//pG->SetEditActive( FALSE );																														
															//pG->SetDataChanged( FALSE );
															return TRUE;
													}
												else if( pMsg->wParam == VK_RETURN )
													{
               if( pMsg->message == WM_KEYDOWN ) return TRUE;
               
															if( pG->GetEditActive() )
                 pG->SetEditActive( FALSE ); 

															//pG->SetSelStart( 0 );
													  //pG->SetSelLength( pG->GetText().GetLength() );																												  
															return TRUE;
													}
												else if( pMsg->wParam == VK_F2  && pG->GetAllowUpdate() )
													{
               if( pMsg->message == WM_KEYDOWN ) return TRUE;

													  pG->SetSelLength( 0 ), 
													  pG->SetEditActive( TRUE ); 
															return TRUE;
													}
												else if( pMsg->wParam == VK_F12 && pMsg->message == WM_KEYUP )
													{													  
															MakeUpdate( pG, this );
													}
										}
								else if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
									{
											if( pMsg->message == WM_KEYUP ) ::MessageBeep( -1 );
											return TRUE;
									}
							}
						else if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN  )
							{
							  if( pMsg->message == WM_KEYUP ) ::MessageBeep( -1 );
							  return TRUE;
							}
				}
				

	  if( CPropertyPage::PreTranslateMessage(pMsg) )
		   return TRUE;

			// don't translate dialog messages when in Shift+F1 help mode
			CFrameWnd* pFrameWnd = GetTopLevelFrame();
			if( pFrameWnd != NULL && pFrameWnd->m_bHelpMode )
				 return FALSE;

			// since 'IsDialogMessage' will eat frame window accelerators,
			//   we call all frame windows' PreTranslateMessage first

			/*pFrameWnd = GetParentFrame();   // start with first parent frame
			while( pFrameWnd != NULL )
			 {
						// allow owner & frames to translate before IsDialogMessage does
						if( pFrameWnd->PreTranslateMessage(pMsg) )
							 return TRUE;

						// try parent frames until there are no parent frames
						pFrameWnd = pFrameWnd->GetParentFrame();
			 }*/

			// filter both messages to dialog and from children
			return PreTranslateInput( pMsg );
	}

BOOL TPropPage_Sys::OnInitDialog()
 {   
   UpdateData( FALSE );   

			((CButton*)GetDlgItem( IDC_RADIO_NAM ))->SetCheck( 1 );

			TMyProperty *pPar = (TMyProperty*)GetParent();
			pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridRoom, &m_ADODCRoom) );
			pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridBillCat, &m_ADODCBillCat) );
			pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridGuestCat, &m_ADODCGuestCat) );
			pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridRecommendedBillCat, &m_ADODCRecommendedBillCat) );

			TGridInitData tArr[] = 
				{ 
						{ &m_GridRoom, RGB(240,245,255) },
						{ 	&m_GridBillCat, RGB(240,245,255) }, 
						{ 	&m_GridGuestCat, RGB(240,245,255) }, 
						{ 	&m_GridRecommendedBillCat, RGB(255,255,220) }, 
						{ 	NULL, 0 }
				}; 
			for( TGridInitData* p = tArr; p->m_pGr; ++p )
				{
						p->m_pGr->SetAllowArrows( TRUE ); 
						p->m_pGr->SetTabAction( 0 );
						p->m_pGr->SetBackColor( p->m_clr );						
						p->m_pGr->GetHeadFont().SetBold( TRUE );																								

						//(*p)->SetParent( ((CPropertySheet*)GetParent())->GetTabControl() );
				}

			SetTxtProp();

			try {
		  InitDScrs();
			}
		catch( _com_error &e )
	  {
	    ShowErr( this, e );
					//PostQuitMessage( 0 );
					//return FALSE;
	  }			

		m_btnEditRecBill.m_ptImageOffset = CPoint( 0, 0 );								

		CRect r; m_GridRecommendedBillCat.GetWindowRect( &r );
		ScreenToClient( &r ); 
		r.bottom = r.top, r.left = r.right - BMPNAV_WIDTH - 1,
		r.top = r.bottom - BMPNAV_HEIGHT - 1;
		m_btnEditRecBill.Create( _T(""), BS_OWNERDRAW|WS_VISIBLE|WS_CHILD, r, this, IDC_BTN_EDITREC );
				

		VERIFY( m_btnEditRecBill.LoadBitmap(IDB_BITMAP_EDIT, TRUE, RGB(192,192,192)) );
  m_btnEditRecBill.SetToolTip( TRUE );
		m_btnEditRecBill.SetToolTipText( _T("Edit list") );
		m_btnEditRecBill.SetTrackLook( TRUE );						

		 //((TMyProperty*)GetParent())->m_nav.Assign( &m_GridRoom, &m_ADODCRoom );
		
		//m_GridRoom.SetFocus();
		//pPar->CallNavAss( &m_GridRoom, true );				
		m_GridRoom.PostMessage( WM_SETFOCUS, 0, 0 );


		float fHigh = GetProfileValueFloat( _T("page_tech"), _T("cel_high_rooms"), -1 );
		if( fHigh != -1 ) m_GridRoom.SetRowHeight( fHigh );

		fHigh = GetProfileValueFloat( _T("page_tech"), _T("cel_high_paycats"), -1 );
		if( fHigh != -1 ) m_GridBillCat.SetRowHeight( fHigh );
		fHigh = GetProfileValueFloat( _T("page_tech"), _T("cel_high_guestcats"), -1 );
		if( fHigh != -1 ) m_GridGuestCat.SetRowHeight( fHigh );
		fHigh = GetProfileValueFloat( _T("page_tech"), _T("cel_high_recommcats"), -1 );
		if( fHigh != -1 ) m_GridRecommendedBillCat.SetRowHeight( fHigh );
		
			
   return TRUE;
 }

void TPropPage_Sys::OnShowWindow( BOOL bShow, UINT nStatus )
	{
	  static bool m_bFlFirst = true;
			
	  CPropertyPage::OnShowWindow( bShow, nStatus );

			if( m_bFlFirst == true )
				{
				  TMyProperty *pPar = (TMyProperty*)GetParent();
						pPar->ResetLastFocus();
				  pPar->CallNavAss( pPar->p_wLastFocus ? (CDataGrid*)pPar->p_wLastFocus:&m_GridRoom, true );				
				}
			m_bFlFirst = false;
	}



void TPropPage_Sys::InitDScrs()
	{
   TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

	  //m_vecSrc.assign( 4 );
			TBindRecs brArr[] =
				{
					{ &m_ADODCRoom, &m_GridRoom, _T("select * from ROOM order by NAME"), adCmdText,
							{
									{ _T("NAME"), _T("Name"), 0, 0, 20, NULL },
									{ _T("RATE"), _T("Rate $"), 0, 0, 7, _TM_MONEY_ },
									{ NULL, NULL, 0, 0, NULL }
							}},
     { &m_ADODCBillCat, &m_GridBillCat, _T("select * from BillCategory order by NAME"), adCmdText,
							{
									{ _T("NAME"), _T("Name"), 0, 0, 20, NULL }, 
									{ NULL, NULL, 0, 0, NULL }
							}},
					{ &m_ADODCGuestCat, &m_GridGuestCat, _T("select * from CategoryGuest  order by NAMECAT"), adCmdText,
							{
									{ _T("NAMECAT"), _T("Name"), 0, 0, 20, NULL }, 
									{ NULL, NULL, 0, 0, NULL }
							}},
					{ &m_ADODCRecommendedBillCat, &m_GridRecommendedBillCat, 
					  _T("select b.ID_BILL, b.NAME  "
          "from CategoryGuest c INNER JOIN CategoryGuest_BillCategory cb ON c.ID_CAT = cb.ID_CAT "
          "INNER JOIN BillCategory b ON b.ID_BILL = cb.ID_BILL "
	         "where c.ID_CAT = ?  order by b.NAME"), adCmdText,
							{
									{ _T("NAME"), _T("Name"), 0, 0, 20, NULL }, 
									{ NULL, NULL, 0, 0, NULL }
							} },
					{ NULL, NULL, NULL, adCmdStoredProc }
				};

			int i = 0;
			for( TBindRecs *p = brArr; p->m_pDC; ++p, ++i )
				{							
						MyCOMChk( m_vecSrc[i].m_pCmd.CreateInstance(_T("ADODB.Command")) );
						MyCOMChk( m_vecSrc[i].m_pRec.CreateInstance(_T("ADODB.Recordset")) );			
						
						m_vecSrc[i].m_pCmd->ActiveConnection = pPar->m_pConnection;
						m_vecSrc[i].m_pCmd->CommandType = p->m_iCmdTyp;
						m_vecSrc[i].m_pCmd->CommandText = p->m_sConn;						
						

						//m_vecSrc[i].m_pRec->MaxRecords = 5;
						m_vecSrc[i].m_pRec->CursorLocation = adUseClient;
						m_vecSrc[i].m_pRec->CursorType = adOpenKeyset;
						m_vecSrc[i].m_pRec->LockType = adLockOptimistic;
						//m_vecSrc[0].m_pRec->PutRefActiveConnection( pPar->m_pConnection );
						m_vecSrc[i].m_pRec->PutRefSource( m_vecSrc[i].m_pCmd );


						if( i == 3 )
							{
							  m_vecSrc[i].m_pCmd->Parameters->Append(
										  m_vecSrc[i].m_pCmd->CreateParameter(
												   _T("id_cat2"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);

									VARIANT_BOOL vBoolTr;
						   VarBoolFromUI1( TRUE, &vBoolTr );
									_variant_t vTmp;
									if( m_vecSrc[2].m_pRec->BOF == vBoolTr ||
				         m_vecSrc[2].m_pRec->EndOfFile == vBoolTr ) 
												vTmp = 0L;
									else
										 vTmp = m_vecSrc[2].m_pRec->Fields->GetItem( _T("ID_CAT") )->Value;

									m_vecSrc[i].m_pCmd->Parameters->GetItem( _T("id_cat2") )->PutValue( 
										 vTmp.vt == VT_NULL ? _variant_t(0L):vTmp	 );
							}

						VARIANT_BOOL vBool;
						VarBoolFromUI1( TRUE, &vBool );
						m_vecSrc[i].m_pCmd->Prepared = vBool;

						_variant_t vNull;
						vNull.vt = VT_ERROR, vNull.scode = DISP_E_PARAMNOTFOUND;
						m_vecSrc[i].m_pRec->Open( vNull, vNull,
								adOpenKeyset, adLockOptimistic, adCmdUnknown );
						m_vecSrc[i].m_bOpened = true;
						
						
						p->m_pDC->SetRefRecordset( m_vecSrc[i].m_pRec );

						p->m_pGr->SetRefDataSource( p->m_pDC->GetControlUnknown() );						

						SetupGridClms( *p->m_pGr, p->m_cd );
				}
	}

void TPropPage_Sys::OnDestroy()
 {   

   SetProfileValueFloat( _T("page_tech"), _T("cel_high_rooms"), m_GridRoom.GetRowHeight() );
			SetProfileValueFloat( _T("page_tech"), _T("cel_high_paycats"), m_GridBillCat.GetRowHeight() );
			SetProfileValueFloat( _T("page_tech"), _T("cel_high_guestcats"), m_GridGuestCat.GetRowHeight() );
			SetProfileValueFloat( _T("page_tech"), _T("cel_high_recommcats"), m_GridRecommendedBillCat.GetRowHeight() );

   CPropertyPage::OnDestroy();
 }


BEGIN_MESSAGE_MAP( TPropPage_Sys, CPropertyPage )
	//{{AFX_MSG_MAP(TPropPageL3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED( IDC_BTN_EDITREC, OnEditRecBill_Clicked )
	ON_BN_CLICKED( IDC_RADIO_NAM, OnSort_Nam )
	ON_BN_CLICKED( IDC_RADIO_RATE, OnSort_Rate )	

	ON_WM_HELPINFO()

END_MESSAGE_MAP()

void TPropPage_Sys::OnSort_Nam()
	{
	  m_vecSrc[0].m_pCmd->CommandText = _T("select * from Room order by NAME");
			VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
			m_vecSrc[0].m_pCmd->Prepared = vBool;

			m_vecSrc[0].m_pRec->Requery( adOptionUnspecified );
	}
void TPropPage_Sys::OnSort_Rate()
	{
	  m_vecSrc[0].m_pCmd->CommandText = _T("select * from Room order by RATE");
			VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
			m_vecSrc[0].m_pCmd->Prepared = vBool;

			m_vecSrc[0].m_pRec->Requery( adOptionUnspecified );
	}

void TPropPage_Sys::OnEditRecBill_Clicked()
	{
	  if( m_vecSrc[2].m_pRec->AbsolutePosition == adPosBOF ||
				   m_vecSrc[2].m_pRec->AbsolutePosition == adPosEOF ) 
				{
      MessageBox( _T("Cann't edit"), _T("Info"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}
								
			int id_cat = (long)(m_vecSrc[2].m_pRec->Fields->GetItem( _T("ID_CAT") )->Value);
			CString sName( (LPCTSTR)(_bstr_t)(m_vecSrc[2].m_pRec->Fields->GetItem( _T("NAMECAT") )->Value) );
   
			TClmDef tcDef[] = 
				{ 
						{ _T("NAME"), _T("Name of category"), 0, 0, 20 },
						{ NULL, NULL }
				};

			TMyProperty* pPar = (TMyProperty*)GetParent();
			TManyToMany dlg( this, m_ptEditRecBill_Key );
			dlg.Ass( pPar->m_pConnection, 
				 _T("CategoryGuest"), _T("BillCategory"), 
					_T("CategoryGuest_BillCategory"),

					_T("ID_CAT"), _T("ID_BILL"),

					_T("Assigned pay categoryes"), _T("Avaliable pay categoryes"),
					sName, id_cat, tcDef );						

			dlg.DoModal();
			m_vecSrc[3].m_pRec->Requery( adOptionUnspecified );
	}

void TPropPage_Sys::OnErrorDatagrid_Room( short DataError, short FAR* Response )
	{
	  ReportErr( DataError, Response, IDC_DATAGRID_ROOM );
	}
void TPropPage_Sys::OnErrorDatagrid_BillCat( short DataError, short FAR* Response )
	{
	  ReportErr( DataError, Response, IDC_DATAGRID_ROOM );
	}
void TPropPage_Sys::OnErrorDatagrid_GuestCat( short DataError, short FAR* Response )
	{
	  ReportErr( DataError, Response, IDC_DATAGRID_BILLCAT );
	}
void TPropPage_Sys::OnErrorDatagrid_RecomBillCat( short DataError, short FAR* Response )
	{
	  ReportErr( DataError, Response, IDC_DATAGRID_GUESTCAT );
	}

void TPropPage_Sys::ReportErr( short DataError, short FAR* Response, int idGrid )
	{
	  CDataGrid *pcdGr = reinterpret_cast<CDataGrid*>( GetDlgItem(idGrid) );
			MessageBox( (LPCTSTR)pcdGr->GetErrorText(), "Error", MB_OK|MB_ICONEXCLAMATION );
			*Response = 0;
	}

void KeepGridClms( CDataGrid& rGrid, TLST_ClmDef2& rLst )
	{
	  CColumns clms( rGrid.GetColumns() );
			rLst.assign( clms.GetCount() );
			for( int i = clms.GetCount() - 1; i >= 0; --i )
				{
      _variant_t vIt( (long)i );  
						if( clms.GetItem(vIt).GetVisible() == FALSE ) continue;

				  rLst[ i ].Ass( clms.GetItem(vIt).GetDataField(), 
					                clms.GetItem(vIt).GetCaption(), clms.GetItem(vIt).GetAlignment() 
							 											);
				}
	}

void RestoreGridClms( CDataGrid& rGrid, TLST_ClmDef2& rLst )
	{
	  CColumns clms( rGrid.GetColumns() );
			VARIANT_BOOL vBool;
			VarBoolFromUI1( FALSE, &vBool );
			
			for( int i = clms.GetCount() - 1; i >= 0; --i )
				{
				  _variant_t vIt( (long)i );  

				  CColumn clm( clms.GetItem(vIt) );

						CString tStr( clm.GetDataField() );
						bool bFl = false;
						for( int j = rLst.size() - 1; j >= 0; --j )
							 if( rLst[j].m_sName == tStr )
									{
									  bFl = true;
											clm.SetCaption( rLst[j].m_sDispl );
											clm.SetAlignment( rLst[j].m_lAln );
											break;
									}

						if( bFl == false ) clm.SetVisible( vBool );
				}
	}

void TPropPage_Sys::OnMoveCompleteAdodc_GuestCat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
   VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
			
			if( m_vecSrc[3].m_pCmd == NULL || m_vecSrc[2].m_pRec == NULL ) return;
				
			if( m_vecSrc[2].m_pRec->BOF == vBool ||
				   m_vecSrc[2].m_pRec->EndOfFile == vBool ) return;			

			long lMod = m_vecSrc[2].m_pRec->EditMode;

			//int id_cat = (long)(m_vecSrc[2].m_pRec->Fields->GetItem( _T("ID_CAT") )->Value);
			//if( m_vecSrc[2].m_pRec->Status != adRecNew )
			m_vecSrc[3].m_pCmd->Parameters->GetItem( _T("id_cat2") )->PutValue( 
					  lMod != adEditNone ? 
							_variant_t(0L):
							m_vecSrc[2].m_pRec->Fields->GetItem( _T("ID_CAT") )->Value	 
					);			

   /*CString str;
			str.Format( _T("select b.ID_BILL, b.NAME  "
          "from CategoryGuest c INNER JOIN CategoryGuest_BillCategory cb ON c.ID_CAT = cb.ID_CAT "
          "INNER JOIN BillCategory b ON b.ID_BILL = cb.ID_BILL "
	         "where c.ID_CAT = %u"), id_cat );

			m_vecSrc[3].m_pCmd->CommandText = (LPCTSTR)str;			
			
			VarBoolFromUI1( FALSE, &vBool );
			m_vecSrc[3].m_pCmd->Prepared = vBool;*/


   TLST_ClmDef2 lstKeep;
			//KeepGridClms( m_GridRecommendedBillCat, lstKeep );
			//m_vecSrc[3].m_pRec->Resync( adAffectAll, adResyncAllValues );
			m_vecSrc[3].m_pRec->Requery( adOptionUnspecified );			
			//RestoreGridClms( m_GridRecommendedBillCat, lstKeep );
			//m_GridGuestCat.GetColumns().Remove( _variant_t((long)0) );

			CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridGuestCat );
	}


void TPropPage_Sys::OnMoveCompleteAdodc_Room( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridRoom );
	}
void TPropPage_Sys::OnMoveCompleteAdodc_BillCat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridBillCat );
	}
void TPropPage_Sys::OnMoveCompleteAdodc_RecommendedCat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridRecommendedBillCat );
	}

BEGIN_EVENTSINK_MAP( TPropPage_Sys, CPropertyPage ) 

	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_ROOM, 215 /* Error */, OnErrorDatagrid_Room, VTS_I2 VTS_PI2 )
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_BILLCAT, 215 /* Error */, OnErrorDatagrid_BillCat, VTS_I2 VTS_PI2 )
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_GUESTCAT, 215 /* Error */, OnErrorDatagrid_GuestCat, VTS_I2 VTS_PI2 )
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_RECOMMBILLCAT, 215 /* Error */, OnErrorDatagrid_RecomBillCat, VTS_I2 VTS_PI2 )

	ON_EVENT(TPropPage_Sys, IDC_DC_GUESTCAT, 201 /* MoveComplete */, OnMoveCompleteAdodc_GuestCat, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)
	ON_EVENT(TPropPage_Sys, IDC_DC_ROOM, 201 /* MoveComplete */, OnMoveCompleteAdodc_Room, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)
	ON_EVENT(TPropPage_Sys, IDC_DC_BILLCAT, 201 /* MoveComplete */, OnMoveCompleteAdodc_BillCat, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)
	ON_EVENT(TPropPage_Sys, IDC_DC_RECOMMBILLCAT, 201 /* MoveComplete */, OnMoveCompleteAdodc_RecommendedCat, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)

	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_ROOM, 205 /* BeforeColEdit */, OnBeforeColEdit_Room, VTS_I2 VTS_I2 VTS_PI2)
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_BILLCAT, 205 /* BeforeColEdit */, OnBeforeColEdit_BillCat, VTS_I2 VTS_I2 VTS_PI2)
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_GUESTCAT, 205 /* BeforeColEdit */, OnBeforeColEdit_GuestCat, VTS_I2 VTS_I2 VTS_PI2)
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_RECOMMBILLCAT, 205 /* BeforeColEdit */, OnBeforeColEdit_RecomBillCat, VTS_I2 VTS_I2 VTS_PI2)

	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_ROOM, 200 /* AfterColEdit */, OnAfterColEdit_Room, VTS_I2)
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_BILLCAT, 200 /* AfterColEdit */, OnAfterColEdit_BillCat, VTS_I2)
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_GUESTCAT, 200 /* AfterColEdit */, OnAfterColEdit_GuestCat, VTS_I2)
	ON_EVENT(TPropPage_Sys, IDC_DATAGRID_RECOMMBILLCAT, 200 /* AfterColEdit */, OnAfterColEdit_RecomBillCat, VTS_I2)

	ON_EVENT( TPropPage_Sys, IDC_DC_ROOM, 211 /* Error */, OnError_Room, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )
	
END_EVENTSINK_MAP()


void TPropPage_Sys::OnBeforeColEdit_Room(short ColIndex, short KeyAscii, short FAR* Cancel)
	{	  
			CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridRoom, true );
	}
void TPropPage_Sys::OnBeforeColEdit_BillCat(short ColIndex, short KeyAscii, short FAR* Cancel)
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridBillCat, true );
	}
void TPropPage_Sys::OnBeforeColEdit_GuestCat(short ColIndex, short KeyAscii, short FAR* Cancel)
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridGuestCat, true );
	}
void TPropPage_Sys::OnBeforeColEdit_RecomBillCat(short ColIndex, short KeyAscii, short FAR* Cancel)
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridRecommendedBillCat, true );
	}

void TPropPage_Sys::OnAfterColEdit_Room(short ColIndex)
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridRoom, false );
	}
void TPropPage_Sys::OnAfterColEdit_BillCat(short ColIndex)
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridBillCat, false );
	}
void TPropPage_Sys::OnAfterColEdit_GuestCat(short ColIndex)
	{ 
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridGuestCat, false );
	}
void TPropPage_Sys::OnAfterColEdit_RecomBillCat(short ColIndex)
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.GridEdit( &m_GridBillCat, false );
	}


void TPropPage_Sys::OnError_Room( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay )
 {	
   ReportErr_ADC( ErrorNumber, Description, Scode, Source, HelpFile, HelpContext, fCancelDisplay, IDC_DC_ROOM );			
 }

void TPropPage_Sys::ReportErr_ADC( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay, int idADC )
	{
	  CAdodc *pcdADC = reinterpret_cast<CAdodc*>( GetDlgItem(idADC) );
			_bstr_t bs( *Description, false );
			MessageBox( (LPCTSTR)bs, _T("Error"), MB_OK|MB_ICONEXCLAMATION );
			*fCancelDisplay = TRUE;
	}


BOOL TPropPage_Sys::OnApply()
	{
	  //MessageBeep( -1 );	  
	  return MakeUpdate_Rec();
	}

void TPropPage_Sys::OnCancel()
	{
	  //::MessageBeep( -1 );
	}

bool TPropPage_Sys::MakeUpdate_Rec()
	{   
   CDataGrid* arr[] =
				{
				  &m_GridRoom, &m_GridBillCat, &m_GridGuestCat, NULL
				};

	  int i = 0;
			CDataGrid **p = arr;
	  try {
					_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
					for( ; i < 3; ++i, ++p )
						 if( m_vecSrc[ i ].m_pRec->RecordCount )
								{
								  PositionEnum pss = m_vecSrc[ i ].m_pRec->GetAbsolutePosition();

										if(  pss != adPosUnknown && pss != adPosBOF && pss != adPosEOF &&								  
								      ((*p)->GetEditActive() || (*p)->GetDataChanged()) 
												)
						    (*p)->SetEditActive( FALSE ),
							   m_vecSrc[ i ].m_pRec->Update( &vtEmpty, &vtEmpty );						
								}
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
      arr[ i ]->SetFocus();
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

BOOL TPropPage_Sys::OnKillActive()
	{
   //CPropertyPage::OnKillActive();
	  return MakeUpdate_Rec();
	}

BOOL TPropPage_Sys::OnHelpInfo( HELPINFO* lpHelpInfo )
	{
	  if( lpHelpInfo->iContextType != HELPINFO_WINDOW )
	    return Default();

			static pair<int, TCtxBnd> arrInit[] =
				{
				  pair<int, TCtxBnd>( IDC_RADIO_NAM, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_RATE, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) )						
				};

			static map<int, TCtxBnd> mapCtxBnd;

			if( mapCtxBnd.empty() )
				 copy( arrInit, 
					      arrInit + sizeof(arrInit)/sizeof(pair<int, TCtxBnd>),
											inserter( mapCtxBnd, mapCtxBnd.begin() )
									);
			
			map<int, TCtxBnd>::iterator it = mapCtxBnd.find( lpHelpInfo->iCtrlId );

			if( it != mapCtxBnd.end() )
				   AfxGetApp()->WinHelp( it->second.ctx, it->second.uCmd );
			else
				 AfxGetApp()->WinHelp( IDH_MainWindow, HELP_CONTEXT );			
			
			return TRUE;

	}
