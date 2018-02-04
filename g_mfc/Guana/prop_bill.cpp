#include "stdafx.h"
#include "oledberr.h"
#include "guana.h"
#include "CRHelpers.h"
#include "ReportSelectDlg.h"

#include "HELP\\guana.hh"

#define GUESTBILL_REPORT _T("GuestBill.rpt")
#define PRETTYGUESTBILL_REPORT _T("PrettyGuestBill.rpt")

#define CNotMyProperty TMyProperty
#define COM_VERIFY     MyCOMChk
#define ComErrorMsgBox ShowErr

IMPLEMENT_DYNCREATE( TPropPage_Bill, CPropertyPage )

TPropPage_Bill::TPropPage_Bill(): 
  CPropertyPage( TPropPage_Bill::IDD ),
		m_vecSrc( 8 ),
		m_enGuestSrt( TPropPage_Bill::G_Name ),
	 m_enBillSrt( TPropPage_Bill::B_Date ),
		m_apLUP_GuestCat( NULL ),
		m_apLUP_CA( NULL ),
		m_apLUP_ACNAMe( NULL ),
		m_apLUP_RoomName( NULL ),
		hbrBkEdit( RGB(18,194,143) ),
		hbrBkEdit2( RGB(240,245,255) ),
		m_ptComments( -1, -1 ), m_ptAddGuest( -1, -1 ), m_ptAddBill( -1, -1 ),
		m_bFlWillAdd( false ),
		m_apSearchGuest( NULL ),
		m_hwModal( NULL )
 {
   m_psp.dwFlags &= ~PSP_HASHELP;	     
 }

TPropPage_Bill::~TPropPage_Bill()
 { 
	  dlSplash.SetCancel();
 }

CString TPropPage_Bill::QGuest()
	{
	  LPCTSTR lpSrt = _T("");

			switch( m_enGuestSrt )
				{
				  case G_Name:
							 lpSrt = _T("FNAME, LNAME"); break;
						case G_City:
							 lpSrt = _T("CITY"); break; 
						case G_Category:
							 lpSrt = _T("__NAMECAT"); break;
						case G_SizeDeposit:
							 lpSrt = _T("DEPOSIT"); break;
				};

			return CString(_T("select "
						"__NAMECAT, ID_GUEST, ID_CAT,  FNAME, LNAME, DEPOSIT, CITY, STATE, ADDRESS, POST "
						"from guest")) + 
				CString(_T(" order by ")) + CString(lpSrt);
	}
CString TPropPage_Bill::QBill()
	{
	  LPCTSTR lpSrt = _T("");
			switch( m_enBillSrt )
				{
				  case B_Date:
							 lpSrt = _T("DATE_"); break;
						case B_Folio:
							 lpSrt = _T("FOLIO"); break; 
						case B_TotalN:
							 lpSrt = _T("_TOTAL"); break;
						case B_Balance:
							 lpSrt = _T("_BALANCE_DUE"); break;
				};

			return CString(_T("select "
					" * from guestbill  "
					" where ID_GUEST=? ")) + 
				CString(_T(" order by ")) + CString(lpSrt);
	}

void TPropPage_Bill::DoDataExchange( CDataExchange* pDX )
	{
	  CPropertyPage::DoDataExchange(pDX);

			TSubclassCtrls subCtrls[] =
				{
						{ IDC_DATAGRID_GUEST, &m_GridGuest },
						{ IDC_DATAGRID_BILL, &m_GridBill },
						{ IDC_DATAGRID_ADDCHARGES, &m_GridAddChares },
						{ IDC_DATAGRID_PAYMENTS, &m_GridPyments },
						{ IDC_DATAGRID_ROOMSALLOC, &m_GridRoomUsed },

						{ IDC_DC_GUEST, &m_ADODCGuest },
						{ IDC_DC_BILL, &m_ADODCBill },
						{ IDC_DC_ADDCHARGES, &m_ADODCAddChares },
						{ IDC_DC_PAYMENTS, &m_ADODCPyments },
						{ IDC_DC_ROOMSALLOC, &m_ADODCRoomUsed },

						/*{ IDC_MASKEDBOX_BILLNO, &m_maskBillNo },
						{ IDC_MASKEDBOX_FOLIO, &m_maskFolio },
						{ IDC_MASKEDBOX_GI, &m_maskGI },
						{ IDC_MASKEDBOX_DETAILS, &m_maskDetails },
						{ IDC_MASKEDBOX_TOTALADD, &m_mask_TotalAddt },

						{ IDC_MASKEDBOX_PAYMENTS, &m_mask_Payments },
						{ IDC_MASKEDBOX_TOTALROOMS, &m_mask_TotalRooms },
						{ IDC_MASKEDBOX_TAXANDSERV, &m_mask_TaxServ },
						{ IDC_MASKEDBOX_TOTAL, &m_mask_Total },
						{ IDC_MASKEDBOX_DEPOSIT, &m_mask_Deposit },
						{ IDC_MASKEDBOX_BALANCEDUE, &m_mask_BDue },
						{ IDC_MASKEDBOX_SUBTOTAL, &m_mask_SubTotal },						

						{ IDC_DTPICKER_DATE, &m_pickDate },*/

						{ IDC_EDIT_BILLNO, &m_edBillNo },
						{ IDC_EDIT_FOLIO, &m_edFolio },
						{ IDC_EDIT_GI, &m_edGI },
						{ IDC_EDIT_DETAILS, &m_edDetails },
						{ IDC_EDIT_TOTALADD, &m_ed_TotalAddt },

						{ IDC_EDIT_PAYMENTS, &m_ed_Payments },
						{ IDC_EDIT_TOTALROOMS, &m_ed_TotalRooms },
						{ IDC_EDIT_TAXANDSERV, &m_ed_TaxServ },
						{ IDC_EDIT_TOTAL, &m_ed_Total },
						{ IDC_EDIT_DEPOSIT, &m_ed_Deposit },
						{ IDC_EDIT_BALANCEDUE, &m_ed_BDue },
						{ IDC_EDIT_SUBTOTAL, &m_ed_SubTotal },						

						{ IDC_PICK_DATE, &m_dt_Date },						

						{ IDC_DC_CATEGORY_G, &m_ADODCCatGuest },
						{ IDC_DC_ROOM, &m_ADODCRoom },
						{ IDC_DC_CATPAY, &m_ADODCCatPay },

						{ 0, NULL }
				};

			MakeSubcass( subCtrls, this, pDX );  		
	}

void TPropPage_Bill::SetTxtProp()
	{
   TLds iLstIds[] =
				{
						{ &m_GridGuest, IDC_STATIC_GUEST }, 
						{ &m_GridBill, IDC_STATIC_BILL },
						//{ &m_GridAddChares, IDC_DATAGRID_ADDCHARGES }, 
						//{ &m_GridPyments, IDC_DATAGRID_PAYMENTS }, 
						//{ &m_GridRoomUsed, IDC_DATAGRID_ROOMSALLOC }, 
						{ NULL, 0 }
				};

			SetupTextProp( iLstIds, this, m_lstAPT );							
	}


BOOL TPropPage_Bill::PreTranslateMessage( MSG* pMsg )
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

															return TRUE;
													}
												else if( pMsg->wParam == VK_F2  && pG->GetAllowUpdate() )
													{
               if( pMsg->message == WM_KEYDOWN ) return TRUE;

													  pG->SetSelLength( 0 ), 
													  pG->SetEditActive( TRUE ); 
															return TRUE;
													}
												else if( pMsg->wParam == VK_F5 && pMsg->message == WM_KEYUP )
													{												
															if( m_tzZoom.IsZoomed() )																				
																	MkVisible( pG, true ),
																	m_tzZoom.Restore();
															else
																{
																		CRect rcZ; CalcZoomRect( rcZ );
																		MkVisible( pG, false );
																		m_tzZoom.Zoom( pG, rcZ );
																}														
													}
												else if( pMsg->wParam == VK_F12 && pMsg->message == WM_KEYUP )
													{													  
															MakeUpdate( pG, this );
													}
										}								
									else if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN  )
										{
												if( pMsg->message == WM_KEYUP ) ::MessageBeep( -1 );
												return TRUE;
										}
							}
						else if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
							{
							  if( pMsg->message == WM_KEYUP ) ::MessageBeep( -1 );
							  return TRUE;
							}
				}
				

	  if( CPropertyPage::PreTranslateMessage(pMsg) )
		   return TRUE;

			// don't translate dialog messages when in Shift+F1 help mode
			/*CWinApp* pFrameWnd = AfxGetApp();
			if( pFrameWnd != NULL && pFrameWnd->m_bHelpMode )
				 return FALSE;*/

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


BOOL TPropPage_Bill::OnInitDialog()
 {   
    UpdateData( FALSE );  
				
				dlSplash.SetTttl( _T("Opening data sources...") );

				((CButton*)GetDlgItem( IDC_RADIO_FNAM ))->SetCheck( 1 );
				((CButton*)GetDlgItem( IDC_RADIO_DATE ))->SetCheck( 1 );

				TMyProperty *pPar = (TMyProperty*)GetParent();
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridGuest, &m_ADODCGuest) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridBill, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridAddChares, &m_ADODCAddChares) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridPyments, &m_ADODCPyments) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridRoomUsed, &m_ADODCRoomUsed) );

				/*pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_maskBillNo, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_maskFolio, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_maskGI, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_maskDetails, &m_ADODCBill) );				

				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_pickDate, &m_ADODCBill) );				*/

				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_edBillNo, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_edFolio, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_edGI, &m_ADODCBill) );
				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_edDetails, &m_ADODCBill) );				

				pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_dt_Date, &m_ADODCBill) );				

				CFont *pSysFnt = CFont::FromHandle( (HFONT)GetStockObject(OEM_FIXED_FONT) );

				m_ed_TotalAddt.SetFont( pSysFnt );
				m_ed_Payments.SetFont( pSysFnt );
				m_ed_TotalRooms.SetFont( pSysFnt );
				m_ed_TaxServ.SetFont( pSysFnt );
				m_ed_Total.SetFont( pSysFnt );
				m_ed_Deposit.SetFont( pSysFnt );
				m_ed_BDue.SetFont( pSysFnt );
				m_ed_SubTotal.SetFont( pSysFnt );
				
				TGridInitData tArr[] = 
					{ 
							{ &m_GridGuest, RGB(240,245,255) },
							{ 	&m_GridBill, RGB(240,245,255) }, 
							{ 	&m_GridAddChares, RGB(240,245,255) }, 
							{ 	&m_GridPyments, RGB(240,245,255) }, 
							{ 	&m_GridRoomUsed, RGB(240,245,255) }, 
							{ 	NULL, 0 }
					}; 
				for( TGridInitData* p = tArr; p->m_pGr; ++p )
					{
							p->m_pGr->SetAllowArrows( TRUE ); 
							p->m_pGr->SetTabAction( 0 );
							p->m_pGr->SetBackColor( p->m_clr );						
							p->m_pGr->GetHeadFont().SetBold( TRUE );
					}

				SetTxtProp();

				try {
					InitDScrs();
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						//return FALSE;
				}						
			

			static TBtnInf iIds[] =
				{
						{ IDC_BUTTON_SEARCH, _T("Search guest") }, 
						{ IDC_BUTTON_PAYENROL, _T("Make enrolment") },
						{ IDC_BUTTON_MONEYBACK, _T("Money back") },
						{ IDC_BUTTON_COMMENT, _T("View/edit comments for this bill") },
						{ IDC_BUTTON_PRT2, _T("Print selected bill") },
						{ IDC_BUTTON_WIZ, _T("Add new guest or bill") },
						{ 0, NULL },
				};

			MakeDefCOXBtns( iIds, this, m_lstCOX );

			SetupFnts();

			float fHigh = GetProfileValueFloat( _T("page_bill"), _T("cel_high_guest"), -1 );
			if( fHigh != -1 ) m_GridGuest.SetRowHeight( fHigh );
			fHigh = GetProfileValueFloat( _T("page_bill"), _T("cel_high_bill"), -1 );
			if( fHigh != -1 ) m_GridBill.SetRowHeight( fHigh );
			fHigh = GetProfileValueFloat( _T("page_bill"), _T("cel_high_addch"), -1 );
			if( fHigh != -1 ) m_GridAddChares.SetRowHeight( fHigh );
			fHigh = GetProfileValueFloat( _T("page_bill"), _T("cel_high_payments"), -1 );
			if( fHigh != -1 ) m_GridPyments.SetRowHeight( fHigh );
			fHigh = GetProfileValueFloat( _T("page_bill"), _T("cel_high_rooms"), -1 );
			if( fHigh != -1 ) m_GridRoomUsed.SetRowHeight( fHigh );


			COXBitmapButton *pB = (COXBitmapButton*)GetDlgItem( IDC_BUTTON_PRT2 );
			VERIFY( pB->LoadBitmap(IDB_BITMAP_PRT, TRUE, RGB(192,192,192)) );

			pB = (COXBitmapButton*)GetDlgItem( IDC_BUTTON_WIZ );
			VERIFY( pB->LoadBitmap(IDB_BITMAP_WIZ, TRUE, RGB(192,192,192)) );


			/*CColumns clms1( m_GridBill.GetColumns() );
			clms1.GetItem( _variant_t(2L) ).SetNumberFormat( _T("M/d/yyyy") );			
			CColumns clms2( m_GridRoomUsed.GetColumns() );
			clms2.GetItem( _variant_t(3L) ).SetNumberFormat( _T("M/d/yyyy") );
			clms2.GetItem( _variant_t(4L) ).SetNumberFormat( _T("M/d/yyyy") );
			clms2.GetItem( _variant_t(8L) ).SetNumberFormat( _T("$#,##0.00;($#,##0.00)") );
			clms2.GetItem( _variant_t(9L) ).SetNumberFormat( _T("$#,##0.00;($#,##0.00)") );
			CColumns clms3( m_GridAddChares.GetColumns() );
			clms3.GetItem( _variant_t(5L) ).SetNumberFormat( _T("M/d/yyyy") );			*/

			m_dt_Date.SetFormat( _TM_FDATE_ );

			pPar->CallNavAss( &m_GridGuest, true );				
			m_GridGuest.PostMessage( WM_SETFOCUS, 0, 0 );

			UpdateCurrGuestName();
			
			m_pCRNormalJob = CRPEngine::GetEngine()->OpenJob(GUESTBILL_REPORT);
			SetLogOnInfoTo(m_pCRNormalJob, ((CGuanaApp*)AfxGetApp())->GetLogOnInfo());
			
			m_pCRPrettyJob = CRPEngine::GetEngine()->OpenJob(PRETTYGUESTBILL_REPORT);
			SetLogOnInfoTo(m_pCRPrettyJob, ((CGuanaApp*)AfxGetApp())->GetLogOnInfo());

   return TRUE;
 }

void TPropPage_Bill::SetupFnts()
	{
	  static LOGFONT lf = { -12, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
		   OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		   VARIABLE_PITCH | FF_DONTCARE, _T("MS Sans Serif")
	  };

	  m_fntPM.CreateFontIndirect( &lf );
			m_fntUSR.CreateFontIndirect( &lf );

			GetDlgItem( IDC_STATIC_M1 )->SetFont( &m_fntPM );
			GetDlgItem( IDC_STATIC_P1 )->SetFont( &m_fntPM );
			GetDlgItem( IDC_STATIC_EQ1 )->SetFont( &m_fntPM );
			GetDlgItem( IDC_STATIC_P2 )->SetFont( &m_fntPM );
			GetDlgItem( IDC_STATIC_EQ2 )->SetFont( &m_fntPM );
			GetDlgItem( IDC_STATIC_FRAME )->SetFont( &m_fntUSR );			
	}


void TPropPage_Bill::OnShowWindow( BOOL bShow, UINT nStatus )
	{	  			
   dlSplash.SetCancel();

	  CPropertyPage::OnShowWindow( bShow, nStatus );			
	}


void TPropPage_Bill::InitDScrs()
	{
   TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

	  /*TMaskEdInint meArr[] = {
						{ &m_maskBillNo, &m_ADODCBill, VT_INT, _T("BILL_NO") },
						{ &m_maskFolio, &m_ADODCBill, VT_BSTR, _T("FOLIO") },			
						{ &m_maskGI, &m_ADODCBill, VT_BSTR, _T("HEARD_OF_GI") },
						{ &m_maskDetails, &m_ADODCBill, VT_BSTR, _T("DETAILS") },
						{ &m_mask_TotalAddt, &m_ADODCBill, VT_CY, _T("_TOTAL_ADDITIONAL") },
						{ &m_mask_Payments, &m_ADODCBill, VT_CY, _T("_TOTAL_PAYMENTS") },
						{ &m_mask_TotalRooms, &m_ADODCBill, VT_CY, _T("_TOTAL_ROOMSPAY") },
						{ &m_mask_TaxServ, &m_ADODCBill, VT_CY, _T("_TAX_AND_SERVICE") },
						{ &m_mask_Total, &m_ADODCBill, VT_CY, _T("_TOTAL") },
						{ &m_mask_Deposit, &m_ADODCBill, VT_CY, _T("_DEPOSIT_RECIEVED") },
						{ &m_mask_BDue, &m_ADODCBill, VT_CY, _T("_BALANCE_DUE") },
					 { &m_mask_SubTotal, &m_ADODCBill, VT_CY, _T("_SUBTOTAL") },						
						{ NULL, NULL, 0, NULL }						
				};			*/


			TBindRecs brArr[] =
				{
					{ &m_ADODCGuest, &m_GridGuest, _T("select "
						"__NAMECAT, ID_GUEST, ID_CAT, FNAME, LNAME, ADDRESS, CITY, STATE, POST, DEPOSIT "
						"from guest "
						" order by FNAME, LNAME"), adCmdText,
								{ 
								   {_T("__NAMECAT"), _T("Category"), 0, 1, 15, NULL},
											{_T("DEPOSIT"), _T("Deposit $"), 2, 0, 7, _TM_MONEY_},
											{_T("FNAME"), _T("First Name"), 0, 0, 10, NULL},
											{_T("LNAME"), _T("Last Name"), 0, 0, 10, NULL},
											{_T("ADDRESS"), _T("Address"), 0, 0, 15, NULL},
											{_T("CITY"), _T("City"), 0, 0, 10, NULL},
											{_T("STATE"), _T("State"), 0, 1, 10, NULL},											
											{_T("POST"), _T("Post"), 0, 0, 10, NULL},
											{NULL, NULL, 0, 0, NULL}
								} },
     { &m_ADODCBill, &m_GridBill, _T("select "
					" * from guestbill  "
					" where ID_GUEST=? order by DATE_"), adCmdText,
								{
								   { _T("BILL_NO"), _T("Bill no (r)"), 1, 2, 5, NULL }, 
											{ _T("DATE_"), _T("Date"), 2, 0, 10, _TM_FDATE_ }, 
											{ _T("FOLIO"), _T("Folio"), 0, 0, 10, NULL },																						
											{ _T("HEARD_OF_GI"), _T("Heard of GI"), 0, 0, 20, NULL },																						
											{ _T("DETAILS"), _T("Details"), 0, 0, 30, NULL },																																	
											{ NULL, NULL, 0, 0, NULL }
								}	},
					{ &m_ADODCAddChares, &m_GridAddChares, _T("select "
					" ID_AC, __NAME, BILL_NO, ID_BILL, AMOUNT, MKDATE " 
					" from  AdditionalCharge where BILL_NO = ? order by MKDATE"), adCmdText,
								{
											{ _T("__NAME"), _T("Name"), 0, 1, 15, NULL },
											{ _T("AMOUNT"), _T("Amount $"), 0, 0, 5, _TM_MONEY_ },
											{ _T("MKDATE"), _T("Date (o)"), 0, 0, 10, _TM_FDATE_ },
											{ NULL, NULL, 0, 0, NULL }
								} },
					{ &m_ADODCPyments, &m_GridPyments, 
					  _T("select * from  Payments where BILL_NO = ?"), adCmdText,
								{
											{ _T("DESCRIPTION"), _T("Description"), 0, 0, 40 }, 
											{ _T("AMOUNT"), _T("Amount $"), 0, 0, 5, _TM_MONEY_ },											
											{ NULL, NULL, 0, 0, NULL }
								} },
					{ &m_ADODCRoomUsed, &m_GridRoomUsed, 
					  _T("select ID_DB, BILL_NO, NO_OF_GUESTS, DATE_IN, "
							   "DATE_OUT, ID_ROOM, __NAME, _NIGHTS, _CHARGE, "
										"_RATE from  GuestBillData where BILL_NO = ? order by DATE_IN, DATE_OUT"), adCmdText,
								{
											{ _T("__NAME"), _T("Name room"), 0, 1, 15, NULL }, 
											{ _T("NO_OF_GUESTS"), _T("No. of Guests"), 0, 0, 5, NULL }, 
											{ _T("DATE_IN"), _T("Date from"), 2, 0, 10, _TM_FDATE_ }, 
											{ _T("DATE_OUT"), _T("Date to"), 2, 0, 10, _TM_FDATE_ }, 
											{ _T("_NIGHTS"), _T("Nights (r)"), 0, 2, 5, NULL }, 
											{ _T("_CHARGE"), _T("Charge $ (r)"), 0, 2, 5, _TM_MONEY_ }, 
											{ _T("_RATE"), _T("Rooms rate $ (r)"), 0, 2, 5, _TM_MONEY_ }, 
											{ NULL, NULL, 0, 0, NULL }
								} },
						{ &m_ADODCCatGuest, NULL, 
					  _T("select * from  CategoryGuest"), adCmdText,
								{											
											{ NULL, NULL, 0, 0, NULL }
								} },
							{ &m_ADODCRoom, NULL, 
					  _T("select * from  Room"), adCmdText,
								{											
											{ NULL, NULL, 0, 0, NULL }
								} },
							{ &m_ADODCCatPay, NULL, 
					  _T("select * from  BillCategory"), adCmdText,
								{											
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
						

						m_vecSrc[i].m_pRec->CursorLocation = adUseClient;
						m_vecSrc[i].m_pRec->CursorType = adOpenKeyset;
						m_vecSrc[i].m_pRec->LockType = adLockOptimistic;
						//m_vecSrc[0].m_pRec->PutRefActiveConnection( pPar->m_pConnection );
						m_vecSrc[i].m_pRec->PutRefSource( m_vecSrc[i].m_pCmd );


						if( i == 1 )
							{
							  m_vecSrc[i].m_pCmd->Parameters->Append(
										  m_vecSrc[i].m_pCmd->CreateParameter(
												   _T("ID_GUEST"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);

									VARIANT_BOOL vBoolTr;
						   VarBoolFromUI1( TRUE, &vBoolTr );
									_variant_t vTmp;
									if( m_vecSrc[0].m_pRec->BOF == vBoolTr ||
				         m_vecSrc[0].m_pRec->EndOfFile == vBoolTr ) 
												vTmp = 0L;
									else
           vTmp = ChkZero_LONG_V(m_vecSrc[0].m_pRec->Fields->GetItem( _T("ID_GUEST") )->Value);
									
									m_vecSrc[i].m_pCmd->Parameters->GetItem( _T("ID_GUEST") )->PutValue( 
										vTmp.vt == VT_NULL ? _variant_t(0L):vTmp 	 );
							}
						else if( i == 2 || i == 3 || i == 4 )
							{
							  m_vecSrc[i].m_pCmd->Parameters->Append(
										  m_vecSrc[i].m_pCmd->CreateParameter(
												   _T("BILL_NO"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);

									VARIANT_BOOL vBoolTr;
						   VarBoolFromUI1( TRUE, &vBoolTr );
									_variant_t vTmp;
									if( m_vecSrc[1].m_pRec->BOF == vBoolTr ||
				         m_vecSrc[1].m_pRec->EndOfFile == vBoolTr ) 
												vTmp = 0L;
									else
										 vTmp = ChkZero_LONG(m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value);
									
									m_vecSrc[i].m_pCmd->Parameters->GetItem( _T("BILL_NO") )->PutValue( 
										 	vTmp.vt == VT_NULL ? _variant_t(0L):vTmp );
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

						if( p->m_pGr )
							{
						   p->m_pGr->SetRefDataSource( p->m_pDC->GetControlUnknown() );						
						   SetupGridClms( *p->m_pGr, p->m_cd );
							}
				}

			//for( TMaskEdInint* pp = meArr; pp->m_pMask; ++pp )			
     //pp->m_pMask->BindDefaultProperty( 0x16, pp->m_vt, pp->m_lpName, pp->m_pADC );
								
			//m_pickDate.BindDefaultProperty( 0x14, VT_DATE, _T("DATE_"), &m_ADODCBill );

			
			MyCOMChk( m_pCmd_RemoveGuest.CreateInstance(_T("ADODB.Command")) );				
						
			m_pCmd_RemoveGuest->ActiveConnection = pPar->m_pConnection;
			m_pCmd_RemoveGuest->CommandType = adCmdStoredProc;
			m_pCmd_RemoveGuest->CommandText = _T("Guest_delcascade");									
			m_pCmd_RemoveGuest->Parameters->Append(
										  m_pCmd_RemoveGuest->CreateParameter(
												   _T("id_guest"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);
			VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
			m_pCmd_RemoveGuest->Prepared = vBool;


			MyCOMChk( m_pCmd_Bill.CreateInstance(_T("ADODB.Command")) );				
						
			m_pCmd_Bill->ActiveConnection = pPar->m_pConnection;
			m_pCmd_Bill->CommandType = adCmdStoredProc;
			m_pCmd_Bill->CommandText = _T("GuestBill_delcascade");									
			m_pCmd_Bill->Parameters->Append(
										  m_pCmd_Bill->CreateParameter(
												   _T("bill_no"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);
			
			m_pCmd_Bill->Prepared = vBool;

			MyCOMChk( m_pCmd_AddCharges.CreateInstance(_T("ADODB.Command")) );				
						
			m_pCmd_AddCharges->ActiveConnection = pPar->m_pConnection;
			m_pCmd_AddCharges->CommandType = adCmdText;
			m_pCmd_AddCharges->CommandText = 
				 _T("delete AdditionalCharge where ID_AC = ?");									
			m_pCmd_AddCharges->Parameters->Append(
										  m_pCmd_AddCharges->CreateParameter(
												   _T("id_ac"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);
			/*m_pCmd_AddCharges->Parameters->Append(
										  m_pCmd_AddCharges->CreateParameter(
												   _T("id_bill"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);*/
			
			m_pCmd_AddCharges->Prepared = vBool;

			MyCOMChk( m_pCmd_RoomsAlloc.CreateInstance(_T("ADODB.Command")) );				
						
			m_pCmd_RoomsAlloc->ActiveConnection = pPar->m_pConnection;
			m_pCmd_RoomsAlloc->CommandType = adCmdText;
			m_pCmd_RoomsAlloc->CommandText = _T("delete GuestBillData where ID_DB = ?");									
			m_pCmd_RoomsAlloc->Parameters->Append(
										  m_pCmd_RoomsAlloc->CreateParameter(
												   _T("id_db"), adInteger, adParamInput, 
															sizeof(int), _variant_t(0L)
												 ) 
										);
			
			m_pCmd_RoomsAlloc->Prepared = vBool;						

   auto_ptr<TFieldBind_Int> apIB1( new TFieldBind_Int() );
			apIB1->Bind( m_vecSrc[1].m_pRec, _T("BILL_NO"), IDC_EDIT_BILLNO, &m_edBillNo );
					 
			auto_ptr<TFieldBind_Str> apIB2( new TFieldBind_Str() );
			apIB2->Bind( m_vecSrc[1].m_pRec, _T("FOLIO"), IDC_EDIT_FOLIO, &m_edFolio );

			auto_ptr<TFieldBind_Date> apIB3( new TFieldBind_Date() );
			apIB3->Bind( m_vecSrc[1].m_pRec, _T("DATE_"), IDC_PICK_DATE, &m_dt_Date );

			auto_ptr<TFieldBind_Str> apIB4( new TFieldBind_Str() );
			apIB4->Bind( m_vecSrc[1].m_pRec, _T("HEARD_OF_GI"), IDC_EDIT_GI, &m_edGI );

			auto_ptr<TFieldBind_Str> apIB5( new TFieldBind_Str() );
			apIB5->Bind( m_vecSrc[1].m_pRec, _T("DETAILS"), IDC_EDIT_DETAILS, &m_edDetails );

			auto_ptr<TFieldBind_Money> apIB6( new TFieldBind_Money() );
			apIB6->Bind( m_vecSrc[1].m_pRec, _T("_TOTAL_ADDITIONAL"), IDC_EDIT_TOTALADD, &m_ed_TotalAddt, true );

			auto_ptr<TFieldBind_Money> apIB7( new TFieldBind_Money() );
			apIB7->Bind( m_vecSrc[1].m_pRec, _T("_TOTAL_PAYMENTS"), IDC_EDIT_PAYMENTS, &m_ed_Payments, true );

			auto_ptr<TFieldBind_Money> apIB8( new TFieldBind_Money() );
			apIB8->Bind( m_vecSrc[1].m_pRec, _T("_TOTAL_ROOMSPAY"), IDC_EDIT_TOTALROOMS, &m_ed_TotalRooms, true );

			auto_ptr<TFieldBind_Money> apIB9( new TFieldBind_Money() );
			apIB9->Bind( m_vecSrc[1].m_pRec, _T("_TAX_AND_SERVICE"), IDC_EDIT_TAXANDSERV, &m_ed_TaxServ, true );

			auto_ptr<TFieldBind_Money> apIB10( new TFieldBind_Money() );
			apIB10->Bind( m_vecSrc[1].m_pRec, _T("_TOTAL"), IDC_EDIT_TOTAL, &m_ed_Total, true );

			auto_ptr<TFieldBind_Money> apIB11( new TFieldBind_Money() );
			apIB11->Bind( m_vecSrc[1].m_pRec, _T("_DEPOSIT_RECIEVED"), IDC_EDIT_DEPOSIT, &m_ed_Deposit, true );

			auto_ptr<TFieldBind_Money> apIB12( new TFieldBind_Money() );
			apIB12->Bind( m_vecSrc[1].m_pRec, _T("_BALANCE_DUE"), IDC_EDIT_BALANCEDUE, &m_ed_BDue, true );

			auto_ptr<TFieldBind_Money> apIB13( new TFieldBind_Money() );
			apIB13->Bind( m_vecSrc[1].m_pRec, _T("_SUBTOTAL"), IDC_EDIT_SUBTOTAL, &m_ed_SubTotal, true );
			
   
			m_mgrBind, (TAP_FieldBind&)apIB1, (TAP_FieldBind&)apIB2, (TAP_FieldBind&)apIB3, 
				 (TAP_FieldBind&)apIB4, (TAP_FieldBind&)apIB5, (TAP_FieldBind&)apIB6, 
				 (TAP_FieldBind&)apIB7, (TAP_FieldBind&)apIB8, (TAP_FieldBind&)apIB9, 
					(TAP_FieldBind&)apIB10, (TAP_FieldBind&)apIB11, (TAP_FieldBind&)apIB12, (TAP_FieldBind&)apIB13;
	}

BOOL TPropPage_Bill::OnSetActive()
	{

   auto_ptr<TLookUp>* arr[] =
				{
				  &m_apLUP_GuestCat, &m_apLUP_CA,
		    &m_apLUP_ACNAMe, &m_apLUP_RoomName, NULL
				};

			try {
					for( int i = 5; i <= 7; ++i )
				   m_vecSrc[ i ].m_pRec->Requery( adOptionUnspecified );
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );						
				}						

   for( auto_ptr<TLookUp>** p = arr; *p; ++p )
				 if( (*p)->get() != NULL ) (**p)->ReFill();

	  return CPropertyPage::OnSetActive();
	}

void TPropPage_Bill::OnDestroy()
 {      
			SetProfileValueFloat( _T("page_bill"), _T("cel_high_guest"), m_GridGuest.GetRowHeight() );
			SetProfileValueFloat( _T("page_bill"), _T("cel_high_bill"), m_GridBill.GetRowHeight() );
			SetProfileValueFloat( _T("page_bill"), _T("cel_high_addch"), m_GridAddChares.GetRowHeight() );
			SetProfileValueFloat( _T("page_bill"), _T("cel_high_payments"), m_GridPyments.GetRowHeight() );
			SetProfileValueFloat( _T("page_bill"), _T("cel_high_rooms"), m_GridRoomUsed.GetRowHeight() );			

   CPropertyPage::OnDestroy();

			dlSplash.SetCancel();

   delete m_apLUP_GuestCat.release();

			delete m_apLUP_CA.release();
		 delete m_apLUP_ACNAMe.release();
		 delete m_apLUP_RoomName.release();

			hbrBkEdit.DeleteObject();
   hbrBkEdit2.DeleteObject();
			m_fntPM.DeleteObject(); m_fntUSR.DeleteObject();
 }

void TPropPage_Bill::OnSort_Guest_Nam()
	{
	  m_enGuestSrt = G_Name;
			ChangeCommand_( 0 );
	}
void TPropPage_Bill::OnSort_Guest_City()
 {
	  m_enGuestSrt = G_City;
			ChangeCommand_( 0 );
	}
void TPropPage_Bill::OnSort_Guest_Category()
 {
	  m_enGuestSrt = G_Category; 
			ChangeCommand_( 0 );
	}
void TPropPage_Bill::OnSort_Guest_SizeDeposit()
 {
	  m_enGuestSrt = G_SizeDeposit;
			ChangeCommand_( 0 );
	}

void TPropPage_Bill::OnSort_Bill_Date()
 {
	  m_enBillSrt = B_Date;
			ChangeCommand_( 1 );
	}
void TPropPage_Bill::OnSort_Bill_Folio()
 {
	  m_enBillSrt = B_Folio;
			ChangeCommand_( 1 );
	}
void TPropPage_Bill::OnSort_Bill_TotalNights()
 {
	  m_enBillSrt = B_TotalN;
			ChangeCommand_( 1 );
	}
void TPropPage_Bill::OnSort_Bill_BalanceDue()
 {
	  m_enBillSrt = B_Balance;
			ChangeCommand_( 1 );
	}

struct TCmdCallInf
	{
	  CString (TPropPage_Bill::* pFn)();
	};
void TPropPage_Bill::ChangeCommand_( int iCmd )
	{
   static TCmdCallInf arrFunc[] = 
				{
						{ &TPropPage_Bill::QGuest },
      { &TPropPage_Bill::QBill } 
				};

	  m_vecSrc[ iCmd ].m_pCmd->CommandText = (LPCTSTR)(this->*(arrFunc[ iCmd ].pFn))();
			VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
			m_vecSrc[ iCmd ].m_pCmd->Prepared = vBool;

			m_vecSrc[ iCmd ].m_pRec->Requery( adOptionUnspecified );
	}

void TPropPage_Bill::OnSearcGuest_Clicked()
 {	  
   TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

			if( m_apSearchGuest.get() == NULL )
				{
				  m_apSearchGuest = auto_ptr<TSearchGuest>( 
							 new TSearchGuest(pPar->m_pConnection, m_vecSrc[0].m_pRec, this) );
						m_apSearchGuest->Create( IDD_DLG_SEARCH_GUEST, this );
				}

			m_apSearchGuest->ShowWindow( SW_SHOW );			

	}
void TPropPage_Bill::OnComment_Clicked()
 {	  
	  TCommentsDlg dlg( this, m_ptComments, m_vecSrc[1].m_pRec, &m_GridBill );
			dlg.DoModal();				
	}
void TPropPage_Bill::Enrollment( bool bFl )
	{
   if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ||
							m_vecSrc[1].m_pRec->RecordCount == 0
					) 
				{
      MessageBox( _T("Cann't pay enroll - bill not avaliable"), _T("Error"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}

   unsigned long lID_GUEST = 
				 (long)m_vecSrc[1].m_pRec->Fields->GetItem(_T("ID_GUEST"))->Value;

   bool bOpened = false;
			_RecordsetPtr pRec;

	  try {					
					TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

					_CommandPtr pC; 
					MyCOMChk( pC.CreateInstance(_T("ADODB.Command")) );
								
					pC->ActiveConnection = pPar->m_pConnection;
					pC->CommandType = adCmdStoredProc;
					pC->CommandText = _T("PayEnroll");
					pC->Prepared = VARIANT_TRUE;
					

					pC->Parameters->Append(
								pC->CreateParameter(
											_T("bill_no"), adInteger, adParamInput, 
											-1, 
											m_vecSrc[1].m_pRec->Fields->GetItem(_T("BILL_NO"))->Value
									) 
						);
					pC->Parameters->Append(
								pC->CreateParameter(
											_T("flag"), adInteger, adParamInput, 
											-1, bFl ? _variant_t(1L):_variant_t(0L)
									) 
						);

					_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
					pC->Execute( &vtEmpty, &vtEmpty, -1 );

					//MakeRefresh( m_vecSrc[ 1 ].m_pRec );						
					try {
								m_vecSrc[ 1 ].m_pRec->Resync( adAffectCurrent, adResyncAllValues );
								m_vecSrc[ 0 ].m_pRec->Resync( adAffectCurrent, adResyncAllValues );
								m_mgrBind.Fetch();
						  m_mgrBind.LoadToControl();
								SetBtnsState();
						}
					catch( _com_error &e )
						{
								ShowErr( this, e );
						}			

					CString str;
					str.Format( _T("select DEPOSIT, FNAME, LNAME from Guest where ID_GUEST=%u"), lID_GUEST );
					pC->CommandType = adCmdText;
					pC->CommandText = (LPCTSTR)str;
					pC->Prepared = VARIANT_TRUE;

					
					MyCOMChk( pRec.CreateInstance(_T("ADODB.Recordset")) );													

					pRec->CursorLocation = adUseClient;
					pRec->CursorType = adOpenKeyset;
					pRec->LockType = adLockOptimistic;					
					pRec->PutRefSource( pC );

					_variant_t vNull;
					vNull.vt = VT_ERROR, vNull.scode = DISP_E_PARAMNOTFOUND;
					pRec->Open( vNull, vNull,
							adOpenKeyset, adLockOptimistic, adCmdUnknown );
					bOpened = true;

					if( bFl )
							str.Format( _T("The payments done. Rest on guest's [%s %s] deposit: %s$"),
										(LPCTSTR)(_bstr_t)pRec->Fields->GetItem( _T("FNAME") )->Value,
										(LPCTSTR)(_bstr_t)pRec->Fields->GetItem( _T("LNAME") )->Value,
										(LPCTSTR)COleCurrency(pRec->Fields->GetItem( _T("DEPOSIT") )->Value).Format()
									);
					else
						 str.Format( _T("Reimburse done. Guest's [%s %s] deposit: %s$"),
										(LPCTSTR)(_bstr_t)pRec->Fields->GetItem( _T("FNAME") )->Value,
										(LPCTSTR)(_bstr_t)pRec->Fields->GetItem( _T("LNAME") )->Value,
										(LPCTSTR)COleCurrency(pRec->Fields->GetItem( _T("DEPOSIT") )->Value).Format()
									);

     MessageBox( str, _T("Info"), MB_OK|MB_ICONINFORMATION );
				}
			catch( _com_error &e )
				{				  
						ShowErr( this, e );
						if( bOpened ) pRec->Close();
						MakeRefresh( m_vecSrc[ 1 ].m_pRec );						
						return;
				}						

			if( bOpened ) pRec->Close();
	}
void TPropPage_Bill::MakeRefresh( _RecordsetPtr& rc )
	{
	  _variant_t vt( rc->Bookmark );
				rc->Requery( adOptionUnspecified );			
				if( rc->RecordCount != 0 ) rc->Bookmark = vt;
	}
void TPropPage_Bill::OnPayEnrol_Clicked()
 {
   Enrollment( true );
	}
void TPropPage_Bill::OnMoneyBack_Clicked()
 {
	  Enrollment( false );
	}

struct TMenuEntry
 {
   UINT    m_Attr;
   UINT    m_ID;
   LPCTSTR m_Txt;
 };
static TMenuEntry mnuCfgLink[] =
 {
			{ MF_STRING | MF_ENABLED, ID_CMD_GUEST, "&Guest" },
			{ MF_STRING | MF_ENABLED, ID_CMD_BILL, "&Bill" }  ,
			{ 0, 0, NULL }
 };
static void FillMenu( CMenu& PopMenu, TMenuEntry* pE )
 {
   for( ; pE->m_Txt; pE++ )
     PopMenu.AppendMenu( pE->m_Attr, pE->m_ID, pE->m_Txt );
 }
void TPropPage_Bill::OnWizard_New()
	{
	  CMenu	PopMenu;
			BOOL	bExists;			

			CRect r;
			GetDlgItem( IDC_BUTTON_WIZ )->GetWindowRect( &r );
			//ScreenToClient( &r );

			CPoint	ClientPoint( r.right, r.bottom );

			try
				{
						bExists = PopMenu.CreatePopupMenu();
						if (!bExists)
							AfxThrowUserException();				
																		
						FillMenu( PopMenu, mnuCfgLink );
																				
						if( !PopMenu.TrackPopupMenu(TPM_RIGHTALIGN,
							   ClientPoint.x, ClientPoint.y, this) 
	 						)
							AfxThrowUserException();
				}
			catch( CUserException* pEx )
				{
						AfxMessageBox( _T("Cann't create popup menu") );	
						pEx->Delete();
				} 

			//if( bExists )
					//PopMenu.DestroyMenu();
	}
void TPropPage_Bill::OnNewGuest()
	{
	  TGuestAdd dlg( &m_GridGuest, m_vecSrc[0].m_pRec, m_ADODCCatGuest, m_ptAddGuest, this );
			if( dlg.DoModal() == IDOK2 ) OnNewBill();
	}
void TPropPage_Bill::OnNewBill()
	{
   if( m_vecSrc[0].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[0].m_pRec->EndOfFile == VARIANT_TRUE ||
							m_vecSrc[0].m_pRec->RecordCount == 0
					) 
				{
      MessageBox( _T("Cann't add new bill - parent guest missing"), _T("Error"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}

   TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

   CString str;
			str.Format( _T("%s %s"), 
				 (LPCTSTR)(_bstr_t)m_vecSrc[0].m_pRec->Fields->GetItem( _T("FNAME") )->Value, 
					(LPCTSTR)(_bstr_t)m_vecSrc[0].m_pRec->Fields->GetItem( _T("LNAME") )->Value
				);

	  TBillAdd dlg( str, &m_GridBill, m_vecSrc[1].m_pRec,
				 m_ADODCRoom, &m_GridRoomUsed, m_vecSrc[4].m_pRec,
					pPar->m_pConnection,
					m_ptAddBill, this );

			dlg.DoModal();				
	}
void TPropPage_Bill::OnPrint_Rpt()
{
	CReportSelectDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CRPEJob* pCRJob = (dlg.m_iReportType == 0) ? m_pCRNormalJob : m_pCRPrettyJob;
	pCRJob->DiscardSavedData();
	double dBillNo = m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value;
	{  // Setting BillNo parameter
		short nParameterFields = pCRJob->GetNParameterFields();
		while (nParameterFields--) {
			CRPEParameterFieldInfo pfi;
			pCRJob->GetNthParameterField(nParameterFields, &pfi);
			if (*(pfi.m_ReportName) == _T('\0')) { //_tcscmp(pfi.m_ReportName, _T(""))
				ASSERT(_tcscmp(pfi.m_Name, _T("@BillNo")) == 0);
				ASSERT(pfi.m_ValueType == PEP_PF_NUMBER);
				*(double*)pfi.m_CurrentValue = dBillNo;
				pfi.m_CurrentValueSet = TRUE;
				VERIFY(pCRJob->SetNthParameterField(nParameterFields, &pfi));
				break;
			}
		}
	}
	CRPEWindowOptions wo(PEP_UNCHANGED, PEP_UNCHANGED, PEP_UNCHANGED,
		FALSE, PEP_UNCHANGED, PEP_UNCHANGED, PEP_UNCHANGED, 
		PEP_UNCHANGED, PEP_UNCHANGED, PEP_UNCHANGED, TRUE, PEP_UNCHANGED);
	pCRJob->SetWindowOptions(&wo);
	pCRJob->OutputToWindow(_T("Guest Bill Report Preview"), CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, (CWnd*)NULL);
	pCRJob->Start();
}

BEGIN_MESSAGE_MAP( TPropPage_Bill, CPropertyPage )
	//{{AFX_MSG_MAP(TPropPageL3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED( IDC_BUTTON_SEARCH, OnSearcGuest_Clicked )
	ON_BN_CLICKED( IDC_BUTTON_COMMENT, OnComment_Clicked )
	ON_BN_CLICKED( IDC_BUTTON_PAYENROL, OnPayEnrol_Clicked )
	ON_BN_CLICKED( IDC_BUTTON_MONEYBACK, OnMoneyBack_Clicked )
	ON_BN_CLICKED( IDC_BUTTON_PRT2, OnPrint_Rpt )
	ON_BN_CLICKED( IDC_BUTTON_WIZ, OnWizard_New )
	

	ON_BN_CLICKED( IDC_RADIO_FNAM, OnSort_Guest_Nam )
	ON_BN_CLICKED( IDC_RADIO_CITY, OnSort_Guest_City )
	ON_BN_CLICKED( IDC_RADIO_CAT, OnSort_Guest_Category )
	ON_BN_CLICKED( IDC_RADIO_DEPOSIT, OnSort_Guest_SizeDeposit )

	ON_BN_CLICKED( IDC_RADIO_DATE, OnSort_Bill_Date )
	ON_BN_CLICKED( IDC_RADIO_FOLIO, OnSort_Bill_Folio )
	ON_BN_CLICKED( IDC_RADIO_NIGHTS, OnSort_Bill_TotalNights )
	ON_BN_CLICKED( IDC_RADIO_BALANCEDUE, OnSort_Bill_BalanceDue )

	ON_WM_CTLCOLOR()

	ON_COMMAND( ID_CMD_GUEST, OnNewGuest )
	ON_COMMAND( ID_CMD_BILL, OnNewBill )

	//ON_MESSAGE( WM_HELPHITTEST, OnCommandHelp )
 //ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )

	/*ON_COMMAND( ID_HELP_FINDER, CPropertyPage::OnHelpFinder )
	ON_COMMAND( ID_HELP, CPropertyPage::OnHelp )
	ON_COMMAND( ID_DEFAULT_HELP, CPropertyPage::OnHelpFinder )*/

	ON_WM_HELPINFO()

END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP( TPropPage_Bill, CPropertyPage ) 
	
	ON_EVENT(TPropPage_Bill, IDC_DC_GUEST, 201 /* MoveComplete */, OnMoveCompleteADODCGuest, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)
	ON_EVENT(TPropPage_Bill, IDC_DC_BILL, 201 /* MoveComplete */, OnMoveCompleteADODCBill, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)

	ON_EVENT(TPropPage_Bill, IDC_DC_ADDCHARGES, 201 /* MoveComplete */, OnMoveCompleteADODCAddChares, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)
	ON_EVENT(TPropPage_Bill, IDC_DC_PAYMENTS, 201 /* MoveComplete */, OnMoveCompleteADODCPyments, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)
	ON_EVENT(TPropPage_Bill, IDC_DC_ROOMSALLOC, 201 /* MoveComplete */, OnMoveCompleteADODCRoomUsed, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH)

	//ON_EVENT( TPropPage_Sys, IDC_DC_ROOM, 211 /* Error */, OnError_Room, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )

	ON_EVENT(TPropPage_Bill, IDC_DC_BILL, 211 /* Error */, OnError_Bill, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )
	ON_EVENT(TPropPage_Bill, IDC_DC_ADDCHARGES, 211 /* Error */, OnError_AddCh, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )
	ON_EVENT(TPropPage_Bill, IDC_DC_PAYMENTS, 211 /* Error */, OnError_Payments, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )
	ON_EVENT(TPropPage_Bill, IDC_DC_ROOMSALLOC, 211 /* Error */, OnError_RoomsAlloc, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )
	ON_EVENT(TPropPage_Bill, IDC_DC_GUEST, 211 /* Error */, OnError_Guest, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL )

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 210 /* ButtonClick */, OnButtonClick_Guest, VTS_I2 )
 ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ADDCHARGES, 210 /* ButtonClick */, OnButtonClick_AddCharges, VTS_I2 )
 ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 210 /* ButtonClick */, OnButtonClick_RoomsUsed, VTS_I2 )

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 209 /* BeforeUpdate */, OnBeforeUpdate_Guest, VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 206 /* BeforeColUpdate */, OnBeforeColUpdate_Guest, VTS_I2 VTS_PVARIANT VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 207 /* BeforeDelete */, OnBeforeDelete_Guest, VTS_PI2 )

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ADDCHARGES, 207 /* BeforeDelete */, OnBeforeDelete_AddCharges, VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_BILL, 207 /* BeforeDelete */, OnBeforeDelete_Bill, VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 207 /* BeforeDelete */, OnBeforeDelete_RoomsAlloc, VTS_PI2 )		

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 215 /* Error */, OnErrorDatagrid_Guest, VTS_I2 VTS_PI2 )	

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_BILL, 215 /* Error */, OnErrorDatagrid_Bill, VTS_I2 VTS_PI2 )	
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ADDCHARGES, 215 /* Error */, OnErrorDatagrid_AddCh, VTS_I2 VTS_PI2 )	
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_PAYMENTS, 215 /* Error */, OnErrorDatagrid_Pay, VTS_I2 VTS_PI2 )	
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 215 /* Error */, OnErrorDatagrid_Rooms, VTS_I2 VTS_PI2 )	
	

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 205 /* BeforeColEdit */, OnBeforeColEdit_Guest, VTS_I2 VTS_I2 VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ADDCHARGES, 205 /* BeforeColEdit */, OnBeforeColEdit_AddCh, VTS_I2 VTS_I2 VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 205 /* BeforeColEdit */, OnBeforeColEdit_RoomsAss, VTS_I2 VTS_I2 VTS_PI2 )	

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_BILL, 217 /* OnAddNew */, OnOnAddNewDatagrid_Bill, VTS_NONE )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_BILL, 208 /* BeforeInsert */, OnBeforeInsertDatagrid_Bill, VTS_PI2 )

	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ADDCHARGES, 217 /* OnAddNew */, OnOnAddNewDatagrid_AddCh, VTS_NONE )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_PAYMENTS, 217 /* OnAddNew */, OnOnAddNewDatagrid_Payment, VTS_NONE )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 217 /* OnAddNew */, OnOnAddNewDatagrid_Rooms, VTS_NONE )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 217 /* OnAddNew */, OnOnAddNewDatagrid_Paymts, VTS_NONE )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_GUEST, 217 /* OnAddNew */, OnOnAddNewDatagrid_Guest, VTS_NONE )
	
	
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ADDCHARGES, 208 /* BeforeInsert */, OnBeforeInsertDatagrid_AddCh, VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_PAYMENTS, 208 /* BeforeInsert */, OnBeforeInsertDatagrid_Payment, VTS_PI2 )
	ON_EVENT( TPropPage_Bill, IDC_DATAGRID_ROOMSALLOC, 208 /* BeforeInsert */, OnBeforeInsertDatagrid_Rooms, VTS_PI2 )		

	
	ON_EVENT( TPropPage_Bill, IDC_DC_GUEST, 205 /* RecordChangeComplete */, OnRecordChangeComplete_Guest, VTS_I4 VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )
	ON_EVENT( TPropPage_Bill, IDC_DC_BILL, 200 /* WillMove */, OnWillMoveAdodc_Bill, VTS_I4 VTS_PI4 VTS_DISPATCH )
	ON_EVENT( TPropPage_Bill, IDC_DC_BILL, 205 /* RecordChangeComplete */, OnRecordChangeComplete_Bill, VTS_I4 VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )
 ON_EVENT( TPropPage_Bill, IDC_DC_BILL, 207 /* RecordsetChangeComplete */, OnRecordsetChangeComplete_Bill, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )
	ON_EVENT( TPropPage_Bill, IDC_DC_BILL, 206 /* WillChangeRecordset */, OnWillChangeRecordset_Bill, VTS_I4 VTS_PI4 VTS_DISPATCH)
 ON_EVENT( TPropPage_Bill, IDC_DC_BILL, 204 /* WillChangeRecord */, OnWillChangeRecord_Bill, VTS_I4 VTS_I4 VTS_PI4 VTS_DISPATCH)

 ON_EVENT( TPropPage_Bill, IDC_DC_ADDCHARGES, 207 /* RecordsetChangeComplete */, OnRecordsetChangeComplete_AddCh, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )
	ON_EVENT( TPropPage_Bill, IDC_DC_PAYMENTS, 207 /* RecordsetChangeComplete */, OnRecordsetChangeComplete_Paymts, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )
	ON_EVENT( TPropPage_Bill, IDC_DC_ROOMSALLOC, 207 /* RecordsetChangeComplete */, OnRecordsetChangeComplete_Rooms, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )

	
	ON_EVENT( TPropPage_Bill, IDC_DC_ADDCHARGES, 205 /* RecordChangeComplete */, OnRecordChangeComplete_AddCh, VTS_I4 VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )
	ON_EVENT( TPropPage_Bill, IDC_DC_ROOMSALLOC, 205 /* RecordChangeComplete */, OnRecordChangeComplete_Rooms, VTS_I4 VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )	
	ON_EVENT( TPropPage_Bill, IDC_DC_PAYMENTS, 205 /* RecordChangeComplete */, OnRecordChangeComplete_Paymts, VTS_I4 VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH )			
	
END_EVENTSINK_MAP()

void TPropPage_Bill::UpdateCurrGuestName()
	{
	  CString sTmp;

   if( m_vecSrc[0].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[0].m_pRec->EndOfFile == VARIANT_TRUE ||
							m_vecSrc[0].m_pRec->AbsolutePosition == adPosUnknown ||
							m_vecSrc[0].m_pRec->RecordCount == 0
					) 		
     sTmp.Format( _T("Details of bill for: \"\"") );
			else		 
			  sTmp.Format( _T("Details of bill for: \"%s %s\""), 
					  (LPCTSTR)(_bstr_t)m_vecSrc[0].m_pRec->Fields->GetItem( _T("FNAME") )->Value,
					  (LPCTSTR)(_bstr_t)m_vecSrc[0].m_pRec->Fields->GetItem( _T("LNAME") )->Value );

   GetDlgItem( IDC_STATIC_FRAME )->SetWindowText( sTmp );
	}

void TPropPage_Bill::OnMoveCompleteADODCGuest( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );			

			if( m_vecSrc[1].m_pCmd == NULL || m_vecSrc[0].m_pRec == NULL ) return;
				
			try {
				
						if( m_vecSrc[0].m_pRec->BOF == vBool ||
										m_vecSrc[0].m_pRec->EndOfFile == vBool ) 				
								m_vecSrc[1].m_pCmd->Parameters->GetItem( _T("ID_GUEST") )->PutValue( 
										_variant_t(0L) );
						else
							{
									long lMod = m_vecSrc[0].m_pRec->EditMode;
									
									m_vecSrc[1].m_pCmd->Parameters->GetItem( _T("ID_GUEST") )->PutValue( 
													lMod != adEditNone ? 
													_variant_t(0L):
													ChkZero_LONG(m_vecSrc[0].m_pRec->Fields->GetItem( _T("ID_GUEST") )->Value)	 
											);			

									UpdateCurrGuestName();         
							}

												
						TLST_ClmDef2 lstKeep;
						m_vecSrc[1].m_pRec->Requery( adOptionUnspecified );			

						CWnd* pW = GetParent();
						if( !pW ) return;

						reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridGuest );
				}
			catch( _com_error &e )
				{
				  if( _com_error::WCodeToHRESULT(e.WCode()) != DB_E_DELETEDROW )
						  ShowErr( this, e );
						return;
				}
	}

void TPropPage_Bill::OnMoveCompleteADODCBill( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );


			try {
						if( m_vecSrc[1].m_pCmd == NULL || 
        
										m_vecSrc[2].m_pCmd == NULL || 
										m_vecSrc[3].m_pCmd == NULL || 
										m_vecSrc[4].m_pCmd == NULL ||

										m_vecSrc[1].m_pRec == NULL || 
										m_vecSrc[2].m_pRec == NULL || 
										m_vecSrc[3].m_pRec == NULL || 
										m_vecSrc[4].m_pRec == NULL ) return;
						
						_variant_t vvSet;

						if( m_vecSrc[1].m_pRec->BOF == vBool ||
										m_vecSrc[1].m_pRec->EndOfFile == vBool )
								vvSet = 0L;
						else
							{      
									long lMod = m_vecSrc[1].m_pRec->EditMode;
									vvSet = (lMod != adEditNone ? 
													_variant_t(0L):
													ChkZero_LONG(m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value));
							}
						
						m_vecSrc[2].m_pCmd->Parameters->GetItem( _T("BILL_NO") )->PutValue( 
										vvSet
								);			
						m_vecSrc[3].m_pCmd->Parameters->GetItem( _T("BILL_NO") )->PutValue( 
										vvSet
								);			
						m_vecSrc[4].m_pCmd->Parameters->GetItem( _T("BILL_NO") )->PutValue( 
										vvSet
								);			
   

						TLST_ClmDef2 lstKeep;
						m_vecSrc[2].m_pRec->Requery( adOptionUnspecified );			
						m_vecSrc[3].m_pRec->Requery( adOptionUnspecified );			
						m_vecSrc[4].m_pRec->Requery( adOptionUnspecified );			

						CWnd* pW = GetParent();
						if( pW ) 
						   reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridBill );
						
      /*iStt = m_vecSrc[1].m_pRec->Status;
						iSiz = m_vecSrc[1].m_pRec->RecordCount;

						tt1 = m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE;
						tt2 = m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE;
						tt3 = m_vecSrc[1].m_pRec->AbsolutePosition == adPosUnknown;
						tt4 = m_vecSrc[1].m_pRec->State;*/
						/*if( 
							   m_vecSrc[1].m_pRec->BOF != VARIANT_TRUE &&
										m_vecSrc[1].m_pRec->EndOfFile != VARIANT_TRUE
								)*/

						SetBtnsState();

						if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							   m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ||
										m_vecSrc[1].m_pRec->RecordCount == 0
								)
								m_mgrBind.Clear(),
						  m_mgrBind.LoadToControl();
						else
						  m_mgrBind.Fetch(),
						  m_mgrBind.LoadToControl();
				}
			catch( _com_error &e )
				{
				  HRESULT hrr = _com_error::WCodeToHRESULT(e.WCode());
				  if( _com_error::WCodeToHRESULT(e.WCode()) != DB_E_DELETEDROW &&
							   _com_error::WCodeToHRESULT(e.WCode()) != -2147220992
								)
						  ShowErr( this, e );
						CheckAvaliableChilds();
						return;
				}
			catch( exception& e )
				{
				  MessageBox( e.what(), _T("Error"), MB_OK|MB_ICONSTOP );
				}
			CheckAvaliableChilds();
	}

void TPropPage_Bill::CheckAvaliableChilds()
	{
	  bool bFl = m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							       m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ||
										    m_vecSrc[1].m_pRec->RecordCount == 0;
			bFl = !bFl;
			CDataGrid* arrGr[] =
				{
				  &m_GridAddChares, &m_GridPyments, &m_GridRoomUsed, NULL
				};
			for( CDataGrid** p = arrGr; *p; ++p )
				 if( (*p)->GetEnabled() != bFl ) 
						{
						  //(*p)->EnableWindow( bFl );
						  (*p)->SetEnabled( bFl );
								//(*p)->
								(*p)->SetBackColor( bFl ? RGB(240,245,255):RGB(192,192,192) );
						}
	}

void TPropPage_Bill::SetBtnsState()
	{
	  static int arr[] =
				{
				  IDC_BUTTON_PAYENROL, IDC_BUTTON_MONEYBACK,
						IDC_BUTTON_COMMENT, IDC_BUTTON_PRT2, 
						0
				};

			bool bFl1, bFl2;

			try {
				  if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							   m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ||
										m_vecSrc[1].m_pRec->RecordCount == 0 ||
										m_vecSrc[1].m_pRec->AbsolutePosition == adPosUnknown
								) bFl1 = false, bFl2 = false;
						else
							{
							  long lMod = m_vecSrc[1].m_pRec->EditMode;
							  _variant_t vvt = (lMod != adEditNone ? 
													_variant_t(-1L):
													ChkZero_LONG(m_vecSrc[1].m_pRec->Fields->GetItem( _T("_PAY_ENROLMENT") )->Value));

										bFl2 = ((long)vvt) == -1L ? false:true;
										bFl1 = ((long)vvt) == 0L ? true:false;
							}
						
					 ::EnableWindow( ::GetDlgItem(m_hWnd, IDC_BUTTON_PAYENROL), bFl1 );
						::EnableWindow( ::GetDlgItem(m_hWnd, IDC_BUTTON_MONEYBACK), !bFl1 && bFl2 );

						::EnableWindow( ::GetDlgItem(m_hWnd, IDC_BUTTON_COMMENT), bFl2 );
						::EnableWindow( ::GetDlgItem(m_hWnd, IDC_BUTTON_PRT2), bFl2 );
				}
			catch( _com_error &e )
				{				 
						ShowErr( this, e );
						return;
				}	  
	}

void TPropPage_Bill::OnMoveCompleteADODCAddChares( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridAddChares );
	}

void TPropPage_Bill::OnMoveCompleteADODCPyments( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridPyments );
	}

void TPropPage_Bill::OnMoveCompleteADODCRoomUsed( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  CWnd* pW = GetParent();
			if( !pW ) return;

			reinterpret_cast<TMyProperty*>(pW)->m_nav.TestState( &m_GridRoomUsed );
	}


void TPropPage_Bill::OnError_Guest( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay )
 {	
   ReportErr_ADC( ErrorNumber, Description, Scode, Source, HelpFile, HelpContext, fCancelDisplay, IDC_DC_GUEST );			
 }
void TPropPage_Bill::OnError_Bill( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay )
 {	
   ReportErr_ADC( ErrorNumber, Description, Scode, Source, HelpFile, HelpContext, fCancelDisplay, IDC_DC_BILL );			
 }
void TPropPage_Bill::OnError_AddCh( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay )
 {	
   ReportErr_ADC( ErrorNumber, Description, Scode, Source, HelpFile, HelpContext, fCancelDisplay, IDC_DC_ADDCHARGES );			
 }
void TPropPage_Bill::OnError_Payments( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay )
 {	
   ReportErr_ADC( ErrorNumber, Description, Scode, Source, HelpFile, HelpContext, fCancelDisplay, IDC_DC_PAYMENTS );			
 }
void TPropPage_Bill::OnError_RoomsAlloc( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay )
 {	
   ReportErr_ADC( ErrorNumber, Description, Scode, Source, HelpFile, HelpContext, fCancelDisplay, IDC_DC_ROOMSALLOC );			
 }

void TPropPage_Bill::ReportErr_ADC( long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay, int idADC )
	{
	  CAdodc *pcdADC = reinterpret_cast<CAdodc*>( GetDlgItem(idADC) );
			_bstr_t bs( *Description, false );
			MessageBox( (LPCTSTR)bs, _T("Error"), MB_OK|MB_ICONEXCLAMATION );
			*fCancelDisplay = TRUE;
	}



void TPropPage_Bill::OnButtonClick_AddCharges( short ColIndex )
	{
		 //m_apLUP_ACNAMe( NULL )

			TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

			if( ColIndex == 1 )
				{
						if( m_apLUP_ACNAMe.get() == NULL )
							{
									m_apLUP_ACNAMe = auto_ptr<TLookUp>( new TLookUp(this, &m_GridAddChares, pPar->m_pConnection) );
									m_apLUP_ACNAMe->Ass( m_vecSrc[2].m_pRec, m_vecSrc[7].m_pRec,
											_T("ID_BILL"), _T("NAME"), _T("__NAME") );				
									m_apLUP_ACNAMe->Create( IDD_DIALOG_LOOKUP, this );
							}
						else
								m_apLUP_ACNAMe->Ass( m_vecSrc[2].m_pRec, m_vecSrc[7].m_pRec,
										_T("ID_BILL"), _T("NAME"), _T("__NAME") );				

						m_hwModal = m_apLUP_ACNAMe->m_hWnd;
						m_apLUP_ACNAMe->ShowWindow( SW_SHOW );			
				}
	}
void TPropPage_Bill::OnButtonClick_RoomsUsed( short ColIndex )
	{	  
			TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

			if( ColIndex == 6 ) 
				{
						if( m_apLUP_RoomName.get() == NULL )
							{
									m_apLUP_RoomName = auto_ptr<TLookUp>( new TLookUp(this, &m_GridRoomUsed, pPar->m_pConnection) );
									m_apLUP_RoomName->Ass( m_vecSrc[4].m_pRec, m_vecSrc[6].m_pRec,
											_T("ID_ROOM"), _T("NAME"), _T("__NAME") );				
									m_apLUP_RoomName->Create( IDD_DIALOG_LOOKUP, this );
							}
						else
								m_apLUP_RoomName->Ass( m_vecSrc[4].m_pRec, m_vecSrc[6].m_pRec,
										_T("ID_ROOM"), _T("NAME"), _T("__NAME") );				

						m_apLUP_RoomName->ShowWindow( SW_SHOW );			
				}
	}
void TPropPage_Bill::OnButtonClick_Guest( short ColIndex ) 
 {
	 	//dts->GetColumns().GetItem( _variant_t(ColIndex) ).SetText( _T("Test") );

   /*static CDataList* dta = NULL;
			if( !dta ) dta = new CDataList();
			
			CRect r( 100, 100, 200, 400 );
			dta->Create( _T("Wnds"), WS_POPUP|WS_VISIBLE, r, this, -1 );				

			dta->SetWindowPos( &wndTop, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );

			dta->SetRefRowSource( m_ADODCCatGuest.GetControlUnknown() );

   dta->SetListField( _T("NAMECAT") );
			dta->SetBoundColumn( _T("ID_CAT") );

			dta->BindDefaultProperty( 0x2, VT_INT, _T("ID_CAT"), &m_ADODCGuest );
   dta->Refresh(); 

			m_GridGuest.GetColumns().GetItem( _variant_t(ColIndex) ).SetText( _T("Test2") );*/


	  TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

			if( ColIndex == 0 )
				{
						if( m_apLUP_GuestCat.get() == NULL )
							{
									m_apLUP_GuestCat = auto_ptr<TLookUp>( new TLookUp(this, &m_GridGuest, pPar->m_pConnection) );
									m_apLUP_GuestCat->Ass( m_vecSrc[0].m_pRec, m_vecSrc[5].m_pRec,
											_T("ID_CAT"), _T("NAMECAT"), _T("__NAMECAT") );				
									m_apLUP_GuestCat->Create( IDD_DIALOG_LOOKUP, this );
							}
						else
								m_apLUP_GuestCat->Ass( m_vecSrc[0].m_pRec, m_vecSrc[5].m_pRec,
										_T("ID_CAT"), _T("NAMECAT"), _T("__NAMECAT") );				

					 m_apLUP_GuestCat->ShowWindow( SW_SHOW );								
				}
			else if( ColIndex == 7 )
				{				  

      if( m_apLUP_CA.get() == NULL )
							{
									m_apLUP_CA = auto_ptr<TLookUp>( new TLookUp(this, &m_GridGuest, pPar->m_pConnection) );
									m_apLUP_CA->Ass( m_vecSrc[0].m_pRec, _T("STATE"), arrST );				
									m_apLUP_CA->Create( IDD_DIALOG_LOOKUP, this );
							}
						
					 m_apLUP_CA->ShowWindow( SW_SHOW );												  
				}
 }


struct Ts2s
	{
	  LPCTSTR lpParam, lpField;
	};
void TPropPage_Bill::OnBeforeUpdate_Guest( short FAR* Cancel ) 
 {		
	  //*Cancel = TRUE;	  
   /*CColumns clms( m_GridGuest.GetColumns() );
			CString str;
			bool bFilled = false;
			for( long i = clms.GetCount() - 1; i >= 0; --i )
				{
				  CColumn cl( clms.GetItem(_variant_t(i)) );
				  if( cl.GetVisible() )
							{
							  str = cl.GetText(); str.TrimRight();
									if( !str.IsEmpty() )
										{
										  bFilled = true; break;
										}
							}
				}
			if( bFilled == false ) *Cancel = TRUE;*/
 }
void TPropPage_Bill::OnBeforeColUpdate_Guest( short ColIndex, VARIANT FAR* OldValue, short FAR* Cancel ) 
 {	
 }
void TPropPage_Bill::RemoveRecord_( _RecordsetPtr pRSKey, LPCTSTR lpKey, 
																																			 _CommandPtr pCmd, LPCTSTR lpParam )
	{
	  VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
	  if( pRSKey->BOF == vBool ||
				   pRSKey->EndOfFile == vBool ) return;			

	  _variant_t vvSet =
							pRSKey->Fields->GetItem( lpKey )->Value;

			pCmd->Parameters->GetItem( lpParam )->PutValue( 
					  vvSet
					);			   

			_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
				try {					
					//m_GridGuest.SetEditActive( FALSE );																														
			  //if( m_GridGuest.GetDataChanged() ) return;
					//m_vecSrc[0].m_pRec->CancelUpdate();  
					pCmd->Execute( &vtEmpty, &vtEmpty, -1 );
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return;
				}			   
	}
void TPropPage_Bill::OnBeforeDelete_Guest( short FAR* Cancel ) 
	{
   *Cancel = TRUE;
			if( MessageBox(_T("Bills will deleted for current guest. Continue ?"), _T("Warning"), MB_YESNO|MB_ICONEXCLAMATION) == IDYES )
			  RemoveRecord_( m_vecSrc[0].m_pRec, _T("ID_GUEST"), m_pCmd_RemoveGuest, _T("id_guest") );	  
	}
void TPropPage_Bill::OnBeforeDelete_Bill( short FAR* Cancel )
	{
	  *Cancel = TRUE;
			RemoveRecord_( m_vecSrc[1].m_pRec, _T("BILL_NO"), m_pCmd_Bill, _T("bill_no") );	  
	}
void TPropPage_Bill::OnBeforeDelete_AddCharges( short FAR* Cancel )
	{
	  *Cancel = TRUE;
			VARIANT_BOOL vBool;
			VarBoolFromUI1( TRUE, &vBool );
	  if( m_vecSrc[2].m_pRec->BOF == vBool ||
				   m_vecSrc[2].m_pRec->EndOfFile == vBool ) return;			

	  _variant_t vvSet =
							m_vecSrc[2].m_pRec->Fields->GetItem( _T("ID_AC") )->Value;
			//_variant_t vvSet2 =
					//		m_vecSrc[2].m_pRec->Fields->GetItem( _T("ID_BILL") )->Value;

			m_pCmd_AddCharges->Parameters->GetItem( _T("id_ac") )->PutValue( 
					  vvSet
					);
			//m_pCmd_AddCharges->Parameters->GetItem( _T("id_bill") )->PutValue( 
					//  vvSet2
					//);

			_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
				try {										
					m_pCmd_AddCharges->Execute( &vtEmpty, &vtEmpty, -1 );
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return;
				}			   
	}
void TPropPage_Bill::OnBeforeDelete_RoomsAlloc( short FAR* Cancel )
	{
	  *Cancel = TRUE;
			RemoveRecord_( m_vecSrc[4].m_pRec, _T("ID_DB"), m_pCmd_RoomsAlloc, _T("id_db") );
	}

void TPropPage_Bill::HandleGridError( short DataError, short FAR* Response )
	{
	  if( DataError == 7011 ) //cancel update
				 *Response = FALSE;
			else if( DataError == 7007 )
				{
				  *Response = FALSE;
						MessageBox( _T("Incorrect data inputed (bad format or size). Changes canceled."), _T("Error"), MB_OK|MB_ICONSTOP );
				}
	}

void TPropPage_Bill::OnErrorDatagrid_Guest( short DataError, short FAR* Response ) 
 {
	  HandleGridError( DataError, Response );	  
 }
void TPropPage_Bill::OnErrorDatagrid_Bill( short DataError, short FAR* Response ) 
 {
	  HandleGridError( DataError, Response );	  
 }
void TPropPage_Bill::OnErrorDatagrid_AddCh( short DataError, short FAR* Response ) 
 {
	  HandleGridError( DataError, Response );	  
 }
void TPropPage_Bill::OnErrorDatagrid_Pay( short DataError, short FAR* Response ) 
 {
	  HandleGridError( DataError, Response );	  
 }
void TPropPage_Bill::OnErrorDatagrid_Rooms( short DataError, short FAR* Response ) 
 {
	  HandleGridError( DataError, Response );	  
 }

void TPropPage_Bill::OnBeforeColEdit_Guest( short ColIndex, short KeyAscii, short FAR* Cancel )
 {
	  if(	ColIndex == 0 /*|| ColIndex == 7*/ )
				 *Cancel = TRUE, MessageBeep( -1 );
			else
			if( ColIndex == 9 )
				{
      *Cancel = TRUE;
						TDeposit_Dlg dlg( COleCurrency(m_GridGuest.GetColumns().GetItem(_variant_t(ColIndex)).GetValue()), this );						
						if( dlg.DoModal() == IDOK )
							{
							  //CString str; str.Format( _T("%1.2f"), dlg.m_dDeposit );
							  m_GridGuest.GetColumns().GetItem(_variant_t(ColIndex)).SetValue( 
										 _variant_t( (CURRENCY)dlg.m_ocDeposit ) );
							}
				}
 }

void TPropPage_Bill::OnBeforeColEdit_AddCh( short ColIndex, short KeyAscii, short FAR* Cancel )
 {
	  if(	ColIndex == 1 )
				 *Cancel = TRUE, MessageBeep( -1 );
 }

void TPropPage_Bill::OnBeforeColEdit_RoomsAss( short ColIndex, short KeyAscii, short FAR* Cancel )
 {
	  if(	ColIndex == 6 )
				 *Cancel = TRUE, MessageBeep( -1 );
 }

void TPropPage_Bill::SetEmp()
	{
	  //now its make trigger

   /*_CommandPtr cmd;
			_RecordsetPtr pRec;
			bool bOpened = false;
			TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

	  try {					 
					 MyCOMChk( cmd.CreateInstance(_T("ADODB.Command")) );						
						
						cmd->ActiveConnection = pPar->m_pConnection;
						cmd->CommandType = adCmdText;

						_variant_t vtAff;
						_variant_t vtNull( DISP_E_PARAMNOTFOUND, VT_ERROR );

						cmd->CommandText = _T("select US_NAME = user_name()");
						pRec = cmd->Execute( &vtAff, &vtNull, adCmdText );
					 bOpened = true;

						_variant_t vvtUsr = pRec->Fields->GetItem( _T("US_NAME") )->Value;
						pRec->Close(); bOpened = false;

						CString str;
						str.Format( _T("select NAME from Operator where user_ LIKE '%s'"), (LPCTSTR)(_bstr_t)vvtUsr );
						cmd->CommandText = (LPCTSTR)str;																						
      
						pRec = cmd->Execute( &vtAff, &vtNull, adCmdText );
					 bOpened = true;

					 _variant_t vvtName = pRec->Fields->GetItem( _T("NAME") )->Value;

						pRec->Close(); bOpened = false;

						
						str.Format( _T("select ID_EMP from Emplyee where FULL_NAME LIKE '%s'"), (LPCTSTR)(_bstr_t)vvtName );
						cmd->CommandText = (LPCTSTR)str;
						pRec = cmd->Execute( &vtAff, &vtNull, adCmdText );
					 bOpened = true;

					 m_vecSrc[1].m_pRec->Fields->GetItem( _T("ID_EMP") )->Value = 		
								 pRec->Fields->GetItem( _T("ID_EMP") )->Value;
				}
			catch( _com_error &e )
				{
				  if( bOpened ) pRec->Close();
						ShowErr( this, e );
						return;
				}

			if( bOpened ) pRec->Close();*/
	}

void TPropPage_Bill::OnOnAddNewDatagrid_Bill() 
 {			  
   SetEmp();
	//m_vecSrc[1].m_pRec->Fields->GetItem( _T("ID_EMP") )->Value = 1L;

   TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

			_CommandPtr pCmdEnt;
			_RecordsetPtr pRec;
			bool bROpen = false;

			try {
					_variant_t vTmp;
					if( m_vecSrc[0].m_pRec->BOF == VARIANT_TRUE ||
									m_vecSrc[0].m_pRec->EndOfFile == VARIANT_TRUE ) 
								vTmp = 0L;
					else
							vTmp = ChkZero_LONG_V(m_vecSrc[0].m_pRec->Fields->GetItem( _T("ID_GUEST") )->Value);

			  m_vecSrc[1].m_pRec->Fields->GetItem( _T("ID_GUEST") )->Value = vTmp;
					

					COleDateTime currDta;					
					currDta = COleDateTime::GetCurrentTime();
					//currDta.SetTime( 0, 0, 0 );

					
     /*MyCOMChk( pCmdEnt.CreateInstance(_T("ADODB.Command")) );
					pCmdEnt->ActiveConnection = pPar->m_pConnection;
					pCmdEnt->CommandType = adCmdText;
					pCmdEnt->CommandText = _T("SELECT @@identity as adID");						
					
					_variant_t vtAff;
					_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
					pRec = pCmdEnt->Execute( &vtAff, &vtEmpty, adCmdText );
					bROpen = true;

					_variant_t vID( pRec->Fields->GetItem( _T("adID") )->Value );

					m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value = 
						 vID;*/
						
					m_vecSrc[1].m_pRec->Fields->GetItem( _T("DATE_") )->Value = 
						_variant_t( (DATE)currDta, VT_DATE  );						
					
					m_vecSrc[1].m_pRec->Fields->GetItem( _T("HEARD_OF_GI") )->Value = 
						_variant_t( _bstr_t(_T("NOT ENTERED"))  );
					m_vecSrc[1].m_pRec->Fields->GetItem( _T("DETAILS") )->Value = 
						_variant_t( _bstr_t(_T("NOT ENTERED"))  );
					m_vecSrc[1].m_pRec->Fields->GetItem( _T("COMMENTS") )->Value = 
						_variant_t( _bstr_t(_T("NOT ENTERED"))  );

					//m_GridBill.Refresh();
					
					//m_vecSrc[1].m_pRec->Fields->GetItem( _T("DATE_") )->Value = _variant_t( (DATE)currDta );
					//m_GridBill.GetColumns().GetItem(_variant_t(2L)).SetText( currDta.Format() );
					//CString sss = m_GridBill.GetColumns().GetItem(_variant_t(2L)).GetText();
					//int aa=12;

					//m_vecSrc[1].m_pRec->Resync( adAffectCurrent, adResyncAllValues );
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						if( bROpen ) pRec->Close();
						return;
				}			

			if( bROpen ) pRec->Close();
 }


void TPropPage_Bill::OnBeforeInsertDatagrid_Bill( short FAR* Cancel ) 
 {
	  if( m_vecSrc[0].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[0].m_pRec->EndOfFile == VARIANT_TRUE 
					) 
			*Cancel = TRUE,
   MessageBox( _T("Cann't add new record - missing parent record (guest)"), _T("Information"), MB_OK|MB_ICONINFORMATION );
 }

void TPropPage_Bill::OnOnAddNewDatagrid_Paymts() 
	{
	  try {
					_variant_t vTmp;
					if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
									m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ) 
								vTmp = 0L;
					else
							vTmp = ChkZero_LONG_V(m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value);

			  m_vecSrc[3].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value = vTmp;
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return;
				}			
	}
void TPropPage_Bill::OnOnAddNewDatagrid_Guest()
	{
   
	}
void TPropPage_Bill::OnOnAddNewDatagrid_AddCh() 
	{
	  try {
					_variant_t vTmp;
					if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
									m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ) 
								vTmp = 0L;
					else
							vTmp = ChkZero_LONG_V(m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value);

			  m_vecSrc[2].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value = vTmp;
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return;
				}			
	}
void TPropPage_Bill::OnBeforeInsertDatagrid_AddCh( short FAR* Cancel ) 
	{
	  if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE 
					)
				*Cancel = TRUE,
				MessageBox( _T("Cann't add new record - missing parent record (bill)"), _T("Information"), MB_OK|MB_ICONINFORMATION );
	}

void TPropPage_Bill::OnOnAddNewDatagrid_Payment()
	{
	  try {
					_variant_t vTmp;
					if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
									m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ) 
								vTmp = 0L;
					else
							vTmp = ChkZero_LONG_V(m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value);

			  m_vecSrc[3].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value = vTmp;
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return;
				}			
	}
void TPropPage_Bill::OnBeforeInsertDatagrid_Payment( short FAR* Cancel ) 
	{
	  if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE 
					)
			*Cancel = TRUE,
			MessageBox( _T("Cann't add new record - missing parent record (bill)"), _T("Information"), MB_OK|MB_ICONINFORMATION );
	}

void TPropPage_Bill::OnOnAddNewDatagrid_Rooms() 
	{
	  try {
					_variant_t vTmp;
					if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
									m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ) 
								vTmp = 0L;
					else
							vTmp = ChkZero_LONG_V(m_vecSrc[1].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value);

			  m_vecSrc[4].m_pRec->Fields->GetItem( _T("BILL_NO") )->Value = vTmp;

					COleDateTime currDta;					
					currDta = COleDateTime::GetCurrentTime();

					COleDateTimeSpan span; span.SetDateTimeSpan( 3, 0, 0, 0 );
					
					m_vecSrc[4].m_pRec->Fields->GetItem( _T("DATE_IN") )->Value = 
						_variant_t( (DATE)currDta, VT_DATE  );						
					currDta += span;
					m_vecSrc[4].m_pRec->Fields->GetItem( _T("DATE_OUT") )->Value = 
						_variant_t( (DATE)currDta, VT_DATE  );						
					m_vecSrc[4].m_pRec->Fields->GetItem( _T("NO_OF_GUESTS") )->Value = 
						_variant_t( 2L  );						
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );
						return;
				}			
	}
void TPropPage_Bill::OnBeforeInsertDatagrid_Rooms( short FAR* Cancel ) 
	{
	  if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ||
							m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE 
					) 
			*Cancel = TRUE,
			MessageBox( _T("Cann't add new record - missing parent record (bill)"), _T("Information"), MB_OK|MB_ICONINFORMATION );
	}


void TPropPage_Bill::OnWillMoveAdodc_Bill( long adReason, long FAR* adStatus, LPDISPATCH pRecordset )
	{
	  /*try {
				 if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ) return;
					if( m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ) return;
					//if( *adStatus == adRecDBDeleted ) return;					  
					
					long ss1 = m_vecSrc[1].m_pRec->AbsolutePosition;
					long ss = m_vecSrc[1].m_pRec->Status;
					if( ss == adRecDBDeleted ) return;
						
	    m_mgrBind.LoadFromControl();
			  int iStored = m_mgrBind.Store();														
					if( iStored ) m_GridBill.SetDataChanged( TRUE );
					//if( iStored ) m_vecSrc[1].m_pRec->
				}
			catch( _com_error &e )
				{
				  HRESULT hr = _com_error::WCodeToHRESULT( e.WCode() );
				  if( hr != DB_E_DELETEDROW && hr != -2146825267 && hr != -2147220992 )//-2146825267
							{
						   ShowErr( this, e );
						   *adStatus = adStatusCancel;
							}
						return;
				}
			catch( exception& e )
				{
				  MessageBox( e.what(), _T("Error"), MB_OK|MB_ICONSTOP );
						*adStatus = adStatusCancel;
						return;
				}*/
			
	}

void TPropPage_Bill::Resync_Bill()
	{
	  try {
				  m_vecSrc[ 1 ].m_pRec->Resync( adAffectCurrent, adResyncAllValues );
						m_mgrBind.Fetch();
						m_mgrBind.LoadToControl();
						//SetBtnsState();
				}
	  catch( _com_error &e )
				{
						ShowErr( this, e );						
				}				  
	}

void TPropPage_Bill::MakeAddRecommendedCategorues()
	{
	  _CommandPtr cmd;
			bool bOpened = false;
			TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

	  try {
	     _variant_t vvtBILL_NO( m_vecSrc[1].m_pRec->Fields->GetItem(_T("BILL_NO"))->Value );
						_variant_t vvtID_GUEST( m_vecSrc[1].m_pRec->Fields->GetItem(_T("ID_GUEST"))->Value );

					 MyCOMChk( cmd.CreateInstance(_T("ADODB.Command")) );						
						
						cmd->ActiveConnection = pPar->m_pConnection;
						cmd->CommandType = adCmdStoredProc;
						cmd->CommandText = _T("AddCategoryes");				
						
						cmd->Parameters->Append(
										 cmd->CreateParameter(
												   _T("bill_no"), adInteger, adParamInput, 
															-1, vvtBILL_NO )
												 );
						cmd->Parameters->Append(
										 cmd->CreateParameter(
												   _T("id_guest"), adInteger, adParamInput, 
															-1, vvtID_GUEST )
												 );

		    _variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
					 cmd->Execute( &vtEmpty, &vtEmpty, -1 ); 			

						m_vecSrc[2].m_pCmd->Parameters->GetItem( _T("BILL_NO") )->PutValue( 
										vvtBILL_NO
								);			
						m_vecSrc[ 2 ].m_pRec->Requery( adOptionUnspecified );
				}
			catch( _com_error &e )
				{						
				}			
	}


void TPropPage_Bill::OnRecordChangeComplete_Guest( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
	  if( adReason == adRsnUpdate && *adStatus == adStatusOK ) 
     UpdateCurrGuestName();	  
 }

void TPropPage_Bill::OnRecordChangeComplete_Bill( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
	  if( adReason == adRsnUndoAddNew ) m_bFlWillAdd = false;
			if( adReason == adRsnUpdate ) SetBtnsState();
	  if( *adStatus == adStatusErrorsOccurred )
				{
				  //m_bFlWillAdd = false;
				  *adStatus = adStatusCancel; return;
				}
			
	  if( adReason == adRsnAddNew ) OnOnAddNewDatagrid_Bill(), m_bFlWillAdd = true;
			if( adReason == adRsnUndoAddNew ) m_bFlWillAdd = false;
			if( adReason == adRsnUpdate && m_bFlWillAdd )
				{
				  m_bFlWillAdd = false;
						MakeAddRecommendedCategorues();
				}			
 }
void TPropPage_Bill::OnRecordChangeComplete_AddCh( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
	  if( *adStatus == adStatusErrorsOccurred && adReason != adRsnDelete )
				{				  
				  *adStatus = adStatusCancel; return;
				}
	  if( adReason == adRsnAddNew ) 
				{
				  OnOnAddNewDatagrid_AddCh();
				}
   if( adReason == adRsnUpdate || adReason == adRsnDelete ) Resync_Bill();				
 }
void TPropPage_Bill::OnRecordChangeComplete_Paymts( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
	  if( *adStatus == adStatusErrorsOccurred  && adReason != adRsnDelete )
				{
				  *adStatus = adStatusCancel; return;
				}
			if( adReason == adRsnAddNew ) 
				{
				  OnOnAddNewDatagrid_Paymts();
				}
   if( adReason == adRsnUpdate || adReason == adRsnDelete ) Resync_Bill();
 }
void TPropPage_Bill::OnRecordChangeComplete_Rooms( long adReason, long cRecords, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
   if( *adStatus == adStatusErrorsOccurred  && adReason != adRsnDelete )
				{
				  *adStatus = adStatusCancel; return;
				}

	  if( adReason == adRsnAddNew ) 
				{
				  OnOnAddNewDatagrid_Rooms();
				}
			if( adReason == adRsnUpdate || adReason == adRsnDelete ) Resync_Bill();				
			if( adReason == adRsnUpdate )
				{
						try {
									m_vecSrc[ 4 ].m_pRec->Resync( adAffectCurrent, adResyncAllValues );									
							}
						catch( _com_error &e )
							{
									ShowErr( this, e );						
							}				  
				}
 }

void TPropPage_Bill::OnRecordsetChangeComplete_Bill(long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset) 
 {		  
 }

void TPropPage_Bill::OnRecordsetChangeComplete_AddCh(long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset) 
 {
	  if( *adStatus == adStatusErrorsOccurred )
				{
				  *adStatus = adStatusCancel; return;
				}
   if( adReason == adRsnDelete ) Resync_Bill();				
 }
void TPropPage_Bill::OnRecordsetChangeComplete_Paymts(long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset) 
 {		 
	  if( *adStatus == adStatusErrorsOccurred )
				{
				  *adStatus = adStatusCancel; return;
				}
   if( adReason == adRsnDelete ) Resync_Bill();				
 }
void TPropPage_Bill::OnRecordsetChangeComplete_Rooms(long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset) 
 {		 
	  if( *adStatus == adStatusErrorsOccurred )
				{
				  *adStatus = adStatusCancel; return;
				}
   if( adReason == adRsnDelete ) Resync_Bill();				
 }


void TPropPage_Bill::OnWillChangeRecordset_Bill( long adReason, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
 }
void TPropPage_Bill::OnWillChangeRecord_Bill( long adReason, long cRecords, long FAR* adStatus, LPDISPATCH pRecordset ) 
 {	
   //if( adReason != adRsnUpdate ) return;
			

	  /*try {
				 if( m_vecSrc[1].m_pRec->BOF == VARIANT_TRUE ) return;
					if( m_vecSrc[1].m_pRec->EndOfFile == VARIANT_TRUE ) return;
					//if( *adStatus == adRecDBDeleted ) return;					  
					
					//long ss1 = m_vecSrc[1].m_pRec->AbsolutePosition;
					//long ss = m_vecSrc[1].m_pRec->Status;
					//if( ss == adRecDBDeleted ) return;
						
	    m_mgrBind.LoadFromControl();
			  int iStored = m_mgrBind.Store();														
					//if( iStored ) m_GridBill.SetDataChanged( TRUE );
					if( iStored ) m_vecSrc[1].m_pRec->Update();
				}
			catch( _com_error &e )
				{
				  HRESULT hr = _com_error::WCodeToHRESULT( e.WCode() );
				  if( hr != DB_E_DELETEDROW && hr != -2146825267 && hr != -2147220992 )//-2146825267
							{
						   ShowErr( this, e );
						   *adStatus = adStatusCancel;
							}
						return;
				}
			catch( exception& e )
				{
				  MessageBox( e.what(), _T("Error"), MB_OK|MB_ICONSTOP );
						*adStatus = adStatusCancel;
						return;
				}*/			
 }

HBRUSH TPropPage_Bill::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   static int arr[] = 
				{
				  IDC_EDIT_TOTALADD,
						IDC_EDIT_PAYMENTS, 
						IDC_EDIT_TOTALROOMS,
						IDC_EDIT_TAXANDSERV,
						IDC_EDIT_TOTAL, 
						IDC_EDIT_DEPOSIT,
						IDC_EDIT_BALANCEDUE, 
						IDC_EDIT_SUBTOTAL, 
						0
				};
			static int arr2[] = 
				{
				  IDC_EDIT_BILLNO,
						IDC_EDIT_FOLIO,
						IDC_EDIT_GI,
						IDC_EDIT_DETAILS,
						IDC_PICK_DATE,
						0
				};

   HBRUSH hbr = CPropertyPage::OnCtlColor( pDC, pWnd, nCtlColor );
			
   
   int id = pWnd->GetDlgCtrlID();
   for( int* p = arr; *p; ++p )
				 if( *p == id )
						{
	       pDC->SetTextColor( RGB(0,0,230) );
								pDC->SetBkColor( RGB(18,194,143) );
								return hbrBkEdit;
						}
			for( p = arr2; *p; ++p )
				if( *p == id )
					{
	      //pDC->SetTextColor( RGB(0,0,230) );
							pDC->SetBkColor( RGB(240,245,255) );
							return hbrBkEdit2;
					}
		 if( id == IDC_STATIC_FRAME )
				   pDC->SetTextColor( RGB(200,20,20) );			

   return hbr;
 }

BOOL TPropPage_Bill::OnApply()
	{
	  //MessageBeep( -1 );
	  return MakeUpdate_Rec();
	}

BOOL TPropPage_Bill::OnKillActive()
	{
   //CPropertyPage::OnKillActive();
	  return MakeUpdate_Rec();
	}


void TPropPage_Bill::OnCancel()
	{
	  //::MessageBeep( -1 );
	}

bool TPropPage_Bill::MakeUpdate_Rec()
	{   
   if( IsWindowVisible() == FALSE ) return true;

   CDataGrid* arr[] =
				{
				  &m_GridGuest, &m_GridBill, 
			   &m_GridAddChares, &m_GridPyments, &m_GridRoomUsed , NULL
				};

	  int i = 0;
			CDataGrid **p = arr;
	  try {
					_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
					for( ; i < 5; ++i, ++p )
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



void TPropPage_Bill::CalcZoomRect( CRect& r )
	{   
   CRect rEd;
			GetDlgItem( IDC_EDIT_TOTALADD )->GetWindowRect( &rEd );

			CRect rFr;
			GetDlgItem( IDC_STATIC_FRAME )->GetWindowRect( &rFr );

			ScreenToClient( &rEd ), ScreenToClient( &rFr );

   r.left = rFr.left, r.right = rFr.Width();
			r.top = 5, r.bottom = rEd.top - 10;
	}

/*struct TEnumWDta
	{	  
			bool bFl;
			int iCmd;
			int id;
	};

BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam )
	{
   static int iInit1[] = { 
				  IDC_EDIT_TOTALADD, IDC_EDIT_PAYMENTS, IDC_EDIT_TOTALROOMS, 
						IDC_EDIT_TAXANDSERV, IDC_EDIT_TOTAL, IDC_EDIT_BALANCEDUE, 
						IDC_EDIT_DEPOSIT, IDC_EDIT_SUBTOTAL, IDC_BUTTON_COMMENT, 
						IDC_BUTTON_PAYENROL, IDC_BUTTON_MONEYBACK, IDC_STATIC_1, 
						IDC_STATIC_M1, IDC_STATIC_2, IDC_STATIC_P1, 
						IDC_STATIC_3, IDC_STATIC_P2, IDC_STATIC_4, 
						IDC_STATIC_EQ2, IDC_STATIC_5, IDC_STATIC_6, 
						IDC_STATIC_7, IDC_STATIC_EQ1, IDC_STATIC_8, IDC_STATIC_9 };
			
			static int iInit2[] = { 
				  IDC_DC_GUEST,
						IDC_DC_BILL,
						IDC_DC_ADDCHARGES,
						IDC_DC_PAYMENTS,
						IDC_DC_ROOMSALLOC,
						IDC_DC_CATEGORY_G,
						IDC_DC_ROOM,
						IDC_DC_CATPAY
						 };

	  static set<int> sVisible( iInit1, iInit1 + sizeof(iInit1)/sizeof(int)  );
			static set<int> sNoAction( iInit2, iInit2 + sizeof(iInit2)/sizeof(int)  );


	  TEnumWDta *p = (TEnumWDta*)lParam;

			int idC = ::GetDlgCtrlID( hwnd );

			if( idC != p->id &&
							sNoAction.find(idC) == sNoAction.end() &&
							sVisible.find(idC) == sVisible.end() &&
					  ::IsWindowVisible(hwnd) != p->bFl 
					)
					::ShowWindow( hwnd, p->iCmd );

			return TRUE;
	}*/

void TPropPage_Bill::MkVisible( CDataGrid* pG, bool bFl )
	{
	  /*TEnumWDta dta;
			dta.bFl = bFl, dta.iCmd = bFl ? SW_SHOWNA:SW_HIDE;
			dta.id = pG->GetDlgCtrlID();

			::EnumChildWindows( m_hWnd, EnumChildProc, (LPARAM)&dta );*/

			static int iInit1[] = { 
				  IDC_RADIO_FNAM,
						IDC_RADIO_CITY, IDC_RADIO_CAT, IDC_RADIO_DEPOSIT, IDC_BUTTON_SEARCH,
						IDC_STATIC_GUEST, IDC_RADIO_DATE, IDC_RADIO_FOLIO, IDC_RADIO_NIGHTS,
						IDC_STATIC_BILL, IDC_RADIO_BALANCEDUE, IDC_BUTTON_PRT2, IDC_BUTTON_WIZ,
						IDC_STATIC_FRAME, IDC_EDIT_BILLNO, IDC_EDIT_FOLIO, IDC_PICK_DATE,
						IDC_EDIT_GI, IDC_EDIT_DETAILS, IDC_STATIC_20, IDC_STATIC_21, 
						IDC_STATIC_22, IDC_STATIC_23, IDC_STATIC_24, 0
						 };

			int iCmd = bFl ? SW_SHOWNA:SW_HIDE;

	  for( int* p = iInit1; *p; ++p )
				::ShowWindow( ::GetDlgItem(m_hWnd,*p), iCmd );

	}


BOOL TPropPage_Bill::OnHelpInfo( HELPINFO* lpHelpInfo )
	{
	  if( lpHelpInfo->iContextType != HELPINFO_WINDOW )
	    return Default();

			static pair<int, TCtxBnd> arrInit[] =
				{
				  pair<int, TCtxBnd>( IDC_EDIT_BILLNO, TCtxBnd(IDH_Dubl, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_FOLIO, TCtxBnd(IDH_Dubl, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_PICK_DATE, TCtxBnd(IDH_Dubl, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_GI, TCtxBnd(IDH_Dubl, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_DETAILS, TCtxBnd(IDH_Dubl, HELP_CONTEXTPOPUP) ),

						pair<int, TCtxBnd>( IDC_EDIT_TOTALADD, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_TAXANDSERV, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_PAYMENTS, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_TOTAL, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_TOTALROOMS, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_DEPOSIT, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_BALANCEDUE, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_EDIT_SUBTOTAL, TCtxBnd(IDH_Calc, HELP_CONTEXTPOPUP) ),

						pair<int, TCtxBnd>( IDC_STATIC_20, TCtxBnd(IDH_MainWindow, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_STATIC_22, TCtxBnd(IDH_MainWindow, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_STATIC_23, TCtxBnd(IDH_MainWindow, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_STATIC_24, TCtxBnd(IDH_MainWindow, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_STATIC_21, TCtxBnd(IDH_MainWindow, HELP_CONTEXT) ),

						pair<int, TCtxBnd>( IDC_RADIO_FNAM, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_CITY, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_CAT, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_DEPOSIT, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_DATE, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_FOLIO, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_NIGHTS, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_BALANCEDUE, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),

						pair<int, TCtxBnd>( IDC_BUTTON_WIZ, TCtxBnd(IDH_Wizard, HELP_CONTEXTPOPUP) ),

						pair<int, TCtxBnd>( IDC_BUTTON_PAYENROL, TCtxBnd(IDH_Enrol, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_BUTTON_MONEYBACK, TCtxBnd(IDH_Enrol, HELP_CONTEXT) ),

						pair<int, TCtxBnd>( IDC_BUTTON_COMMENT, TCtxBnd(IDH_Comments, HELP_CONTEXTPOPUP) )
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
