#include "stdafx.h"
#include "CRHelpers.h"
#include "guana.h"
#include "..\..\..\Seagate Software\Crystal Reports\crdates.h"

#include "HELP\\guana.hh"

static LPCTSTR lpPassword_for_removeAll = _T("VOLF2");

#define BYCATEGORY_REPORT _T("ReportByCategory.rpt")

#define CNotMyProperty TMyProperty
#define COM_VERIFY     MyCOMChk
#define ComErrorMsgBox ShowErr

IMPLEMENT_DYNCREATE( TPropPage_Time, CPropertyPage )

TPropPage_Time::TPropPage_Time(): 
  CPropertyPage( TPropPage_Time::IDD ),
		m_vecSrc( 1 ),
		m_bOpened( false ), bOpenedFind( false ), m_bLockLst( true )
 {
   m_psp.dwFlags &= ~PSP_HASHELP;	     

			m_bFlAll = true;
	  m_iSrt = 1;
	  m_iFltCat = 1;
												
	  //m_od1 = COleDateTime::GetCurrentTime();
			//m_od2 = COleDateTime::GetCurrentTime();   

			try {
					 m_od1.SetDate(1990, 01, 01);
					 m_od2.SetDate(1999, 01, 01);
				}
			catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();
				}

			/*m_od1.SetDateTime( m_od1.GetYear(), 
				         m_od1.GetMonth() - 1, m_od1.GetDay(), 0, 0, 0 );
			m_od2.SetDateTime( m_od2.GetYear(), 
				         m_od2.GetMonth() + 1, m_od2.GetDay(), 0, 0, 0 );*/
 }

TPropPage_Time::~TPropPage_Time()
 { 
	  dlSplash.SetCancel();
 }


void TPropPage_Time::DoDataExchange( CDataExchange* pDX )
 {
			CPropertyPage::DoDataExchange(pDX);			

			TSubclassCtrls subCtrls[] =
				{
						{ IDC_DATAGRID_GB, &m_GridLst },
						{ IDC_DATALIST_CAT, &m_dtaLst },
						{ IDC_DATETIMEPICKER1, &m_tm1 },
						{ IDC_DATETIMEPICKER2, &m_tm2 },

						{ IDC_DC_GB, &m_ADODCLst },
						{ IDC_DC_CATEGORY_G, &m_ADODCCat }, 

						{ IDC_RADIO_ALL, &m_bAll },
						{ IDC_RADIO_SEL, &m_bSel },						
						{ 0, NULL }
				};

			MakeSubcass( subCtrls, this, pDX );  		
 }

void TPropPage_Time::SetTxtProp()
	{
   TLds iLstIds[] =
				{
						{ &m_GridLst, IDC_STATIC_GB }, 						
						{ NULL, 0 }
				};

			SetupTextProp( iLstIds, this, m_lstAPT );							
	}

BOOL TPropPage_Time::PreTranslateMessage( MSG* pMsg )
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
										}
									else if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
										{
										  //if( PreTranslateMessage(pMsg) ) return TRUE;
												if( pMsg->message == WM_KEYUP ) ::MessageBeep( -1 );
												return TRUE;
										}
							}
						else if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
							{
							  //if( PreTranslateMessage(pMsg) ) return TRUE;
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


BOOL TPropPage_Time::OnInitDialog()
 {   
   UpdateData( FALSE );   

			((CButton*)GetDlgItem( IDC_RADIO_DATE ))->SetCheck( 1 );
			((CButton*)GetDlgItem( IDC_RADIO_ALL ))->SetCheck( 1 );

			TMyProperty *pPar = (TMyProperty*)GetParent();
			pPar->m_mapGRID_DC.insert( pair<LPVOID,LPVOID>(&m_GridLst, &m_ADODCLst) );
			

			TGridInitData tArr[] = 
				{ 
						{ &m_GridLst, RGB(255,255,220) },						
						{ NULL, 0 }
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
		
		dlSplash.SetCancel();
		
		static TBtnInf iIds[] =
				{
						{ IDC_BUTTON_REPORT, _T("Preview and print report") }, 						
						{ IDC_BUTTON_REFILL, _T("Refill list") }, 						
						{ IDC_BUTTON_JUMP, _T("Jump to details for selected record") },
						{ IDC_BUTTON_REMOVE, _T("Remove all bills in current time interval") },
						{ 0, NULL },
				};

		MakeDefCOXBtns( iIds, this, m_lstCOX );
		m_lstCOX.back()->SetTextColor( RGB(255,0,0) );						
				
		m_lstCOX.front()->EnableWindow( m_bFlAll == false );

		m_tm1.SetTime( m_od1 );
		m_tm2.SetTime( m_od2 );

		m_dtaLst.SetLocked( m_bFlAll );

		m_dtaLst.SetForeColor( m_bFlAll ? RGB(192, 192, 192):RGB(64, 128, 128) );
				
		pPar->CallNavAss( &m_GridLst, true );				
		m_GridLst.PostMessage( WM_SETFOCUS, 0, 0 );

		try {
	   m_pCRJob = CRPEngine::GetEngine()->OpenJob( BYCATEGORY_REPORT );
	   SetLogOnInfoTo( m_pCRJob, ((CGuanaApp*)AfxGetApp())->GetLogOnInfo() );
			}
		catch( CException *pE )
			{
			  pE->Delete();
			}
		catch( _com_error &e )
	  {
	    ShowErr( this, e );					
	  }	  

		 
			m_od1.SetDate(
				  GetProfileValueInt(_T("page_time"), _T("year1"), 1990),
						GetProfileValueInt(_T("page_time"), _T("month1"), 1),
						GetProfileValueInt(_T("page_time"), _T("day1"), 1)
					);
			m_od2.SetDate(
				  GetProfileValueInt(_T("page_time"), _T("year2"), 1999),
						GetProfileValueInt(_T("page_time"), _T("month2"), 1),
						GetProfileValueInt(_T("page_time"), _T("day2"), 1)
					);		 		

			float fHigh = GetProfileValueFloat( _T("page_time"), _T("cel_high_gb"), -1 );
			if( fHigh != -1 ) m_GridLst.SetRowHeight( fHigh );

			m_tm1.SetTime( m_od1 );
		 m_tm2.SetTime( m_od2 );

			m_tm1.SetFormat( _T("M/d/yyy") );
			m_tm2.SetFormat( _T("M/d/yyy") );

			CColumns clms1( m_GridLst.GetColumns() );
			clms1.GetItem( _variant_t(7L) ).SetNumberFormat( _T("M/d/yyyy") );

			try {
					m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
					m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

					MakeRefresh();			
				}
			catch( _com_error &e )
	   {
	     ShowErr( this, e );					
	   }	  

   return TRUE;
 }

void TPropPage_Time::OnShowWindow( BOOL bShow, UINT nStatus )
	{
	  static bool m_bFlFirst = true;
			
	  CPropertyPage::OnShowWindow( bShow, nStatus );

			if( m_bFlFirst == true )
				{
				  TMyProperty *pPar = (TMyProperty*)GetParent();
						pPar->ResetLastFocus();
				  pPar->CallNavAss( pPar->p_wLastFocus ? (CDataGrid*)pPar->p_wLastFocus:&m_GridLst, true );				
				}
			m_bFlFirst = false;
	}

CString TPropPage_Time::GetQ( CString* pFltOut )
	{

			CString str( _T("select g.ID_GUEST, g.ID_CAT, g.__NAMECAT, g.FNAME, g.LNAME, "
				      "b.BILL_NO, b.ID_GUEST, b.DATE_, b.FOLIO, "
										"b.HEARD_OF_GI, b.DETAILS, b._TOTAL_ADDITIONAL, "
										"b._SUBTOTAL, b._TOTAL_PAYMENTS,  b._TOTAL, "
										"b._BALANCE_DUE, b._TOTAL_GUEST, b._TOTAL_NIGHTS, "
										"b._TOTAL_ROOMSPAY, b._DEPOSIT_RECIEVED, b._TAX_AND_SERVICE "
				"from "
				 "GuestBill b INNER JOIN Guest g ON b.ID_GUEST=g.ID_GUEST "
					  "where ") );

			CString sT1 = m_od1.Format( VAR_DATEVALUEONLY );
			CString sT2 = m_od2.Format( VAR_DATEVALUEONLY );
			CString flt;
			if( m_bFlAll ) 
				  flt.Format( _T(" b.DATE_ >= '%s' and b.DATE_ <= '%s' "), (LPCTSTR)sT1, (LPCTSTR)sT2 );
			else 
				flt.Format( _T(" b.DATE_ >= '%s' and b.DATE_ <= '%s' and g.ID_CAT = %u "),
				  (LPCTSTR)sT1, (LPCTSTR)sT2, m_iFltCat );

			CString srt;
			if( m_iSrt == 0 ) srt = _T(" order by FNAME, LNAME");
			else srt = _T(" order by DATE_ ");

			if( pFltOut ) *pFltOut = flt;

			return str + flt + srt;	
	}


void TPropPage_Time::InitDScrs()
	{
   TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );

	  
			
			TBindRecs brArr[] =
				{
					{ &m_ADODCLst, &m_GridLst, _T("select g.ID_GUEST, g.ID_CAT, g.__NAMECAT, g.FNAME, g.LNAME, "
				      "b.BILL_NO, b.ID_GUEST, b.DATE_, b.FOLIO, "
										"b.HEARD_OF_GI, b.DETAILS, b._TOTAL_ADDITIONAL, "
										"b._SUBTOTAL, b._TOTAL_PAYMENTS,  b._TOTAL, "
										"b._BALANCE_DUE, b._TOTAL_GUEST, b._TOTAL_NIGHTS, "
										"b._TOTAL_ROOMSPAY, b._DEPOSIT_RECIEVED, b._TAX_AND_SERVICE "
				"from "
				 "GuestBill b INNER JOIN Guest g ON b.ID_GUEST=g.ID_GUEST "
					  "where b.DATE_ >= '1.1.1990' and b.DATE_ <= '1.1.1999' order by b.DATE_"), adCmdText,
							{
									{ _T("__NAMECAT"), _T("Category"), 0, 0, 7, NULL },
									{ _T("FNAME"), _T("First Name"), 0, 0, 10, NULL },
									{ _T("LNAME"), _T("Last Name"), 0, 0, 10, NULL },
									{ _T("BILL_NO"), _T("Bill No"), 0, 0, 5, NULL },
									{ _T("DATE_"), _T("Date"), 2, 0, 10, _TM_FDATE_ },
									{ _T("FOLIO"), _T("Folio"), 0, 0, 7, NULL },
									{ _T("HEARD_OF_GI"), _T("Heard of GI"), 0, 0, 15, NULL },
									{ _T("DETAILS"), _T("Details"), 0, 0, 20, NULL },
									{ _T("_TOTAL_ADDITIONAL"), _T("Total additional$"), 0, 0, 5, _TM_MONEY_ },
									{ _T("_SUBTOTAL"), _T("Subtotal$"), 0, 0, 5, _TM_MONEY_ },
									{ _T("_TOTAL_PAYMENTS"), _T("Total payments$"), 0, 0, 5, _TM_MONEY_ },
									{ _T("_TOTAL"), _T("Total$"), 0, 0, 5, _TM_MONEY_ },
									{ _T("_BALANCE_DUE"), _T("Ballance due$"), 0, 0, 5, _TM_MONEY_ },									
									{ _T("_TOTAL_GUEST"), _T("Total guests"), 0, 0, 5, NULL },
									{ _T("_TOTAL_NIGHTS"), _T("Total nights"), 0, 0, 5, NULL },
									{ NULL, NULL, 0, 0, NULL }
							}},     
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
						m_vecSrc[i].m_pRec->PutRefSource( m_vecSrc[i].m_pCmd );


						if( i == 0 )
							{							  
							  /*CString sT = m_od1.Format(VAR_DATEVALUEONLY);
									//sT = _T("'") + sT + _T("'");

							  m_vecSrc[i].m_pCmd->Parameters->Append(
										  m_vecSrc[i].m_pCmd->CreateParameter(
												   _T("date1"), adBSTR, adParamInput, 
															10, _variant_t( _bstr_t(sT) )
												 ) 
										);								

									sT = m_od2.Format(VAR_DATEVALUEONLY);
									//sT = _T("'") + sT + _T("'");

									m_vecSrc[i].m_pCmd->Parameters->Append(
										  m_vecSrc[i].m_pCmd->CreateParameter(
												   _T("date2"), adBSTR, adParamInput, 
															10, _variant_t( _bstr_t(sT) )
												 ) 
										);
									
									m_vecSrc[i].m_pCmd->Parameters->Append(
										  m_vecSrc[i].m_pCmd->CreateParameter(
												   _T("id_cat"), adInteger, adParamInput, 
															-1, _variant_t((long)m_iFltCat)
												 ) 
										);*/
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

			
			MyCOMChk( m_pCmd_Lst.CreateInstance(_T("ADODB.Command")) );
			MyCOMChk( m_pRec_Lst.CreateInstance(_T("ADODB.Recordset")) );			
			MyCOMChk( m_pCmdFind.CreateInstance(_T("ADODB.Command")) );
			MyCOMChk( m_pRecFind.CreateInstance(_T("ADODB.Recordset")) );			
						
			m_pCmd_Lst->ActiveConnection = pPar->m_pConnection;
			m_pCmd_Lst->CommandType = adCmdText;
			m_pCmd_Lst->CommandText = _T("select * from CategoryGuest");

			m_pRec_Lst->CursorLocation = adUseClient;
			m_pRec_Lst->CursorType = adOpenKeyset;
			m_pRec_Lst->LockType = adLockOptimistic;
			m_pRec_Lst->PutRefSource( m_pCmd_Lst );

			m_pCmd_Lst->Prepared = VARIANT_TRUE;

			_variant_t vNull;
			vNull.vt = VT_ERROR, vNull.scode = DISP_E_PARAMNOTFOUND;
			m_pRec_Lst->Open( vNull, vNull,
					adOpenKeyset, adLockOptimistic, adCmdUnknown );
			m_bOpened = true;
						
			m_ADODCCat.SetRefRecordset( m_pRec_Lst );

			m_dtaLst.SetListField( _T("NAMECAT") );
			m_dtaLst.SetRefRowSource( m_ADODCCat.GetControlUnknown() );						

			//m_dtaLst.BindDefaultProperty( 0x2, VT_BSTR, _T("ID_CAT"), &m_ADODCCat );


			m_pCmdFind->ActiveConnection = pPar->m_pConnection;
			m_pCmdFind->CommandType = adCmdText;
			m_pCmdFind->CommandText = 
				  _T("select * from CategoryGuest where NAMECAT=?");

			m_pCmdFind->Parameters->Append(
										  m_pCmdFind->CreateParameter(
												   _T("namecat"), adBSTR, adParamInput, 
															20, _variant_t(_bstr_t(_T("*")))
												 ) 
										);
			//m_pCmdFind->Parameters->GetItem( _T("namecat") )->PutValue( 
					//								_variant_t(_bstr_t(_T("ttt"))) );

			m_pRecFind->CursorLocation = adUseClient;
			m_pRecFind->CursorType = adOpenKeyset;
			m_pRecFind->LockType = adLockOptimistic;
			m_pRecFind->PutRefSource( m_pCmdFind );

			m_pCmdFind->Prepared = VARIANT_TRUE;
			
			m_pRecFind->Open( vNull, vNull,
					adOpenKeyset, adLockOptimistic, adCmdUnknown );
			bOpenedFind = true;			

			m_bLockLst = false;
	}


void TPropPage_Time::OnDestroy()
 {   
   dlSplash.SetCancel();

   try {
					if( m_bOpened ) m_pRec_Lst->Close();
					m_bOpened = false;
					if( bOpenedFind ) m_pRecFind->Close();
					bOpenedFind = false;	
					
					SetProfileValueInt( _T("page_time"), _T("year1"), m_od1.GetYear() );
					SetProfileValueInt( _T("page_time"), _T("month1"), m_od1.GetMonth() );
					SetProfileValueInt( _T("page_time"), _T("day1"), m_od1.GetDay() );

					SetProfileValueInt( _T("page_time"), _T("year2"), m_od2.GetYear() );
					SetProfileValueInt( _T("page_time"), _T("month2"), m_od2.GetMonth() );
					SetProfileValueInt( _T("page_time"), _T("day2"), m_od2.GetDay() );

					SetProfileValueFloat( _T("page_time"), _T("cel_high_gb"), m_GridLst.GetRowHeight() );
			}
		catch( _com_error &e )
	  {
	    ShowErr( this, e );					
	  }	  

   CPropertyPage::OnDestroy();
 }

void TPropPage_Time::RefillLst()
	{
	  C_Recordset rs( m_ADODCCat.GetRecordset() );
			_variant_t vt( rs.GetBookmark() );
			long lPos = rs.GetAbsolutePosition();

			m_dtaLst.ReFill();

			long lCou = rs.GetRecordCount();
			 if( lPos < lCou )
						rs.SetBookmark( vt );
				else if( lCou ) rs.MoveLast();
	}

BOOL TPropPage_Time::OnSetActive()
	{
	  try {
     //m_vecSrc[0].m_pRec->Requery( adOptionUnspecified );										
				}
			catch( _com_error &e )
				{
						ShowErr( this, e );						
				}						
			catch( CException *pE )
				{
				  pE->Delete();
				}
	  return CPropertyPage::OnSetActive();
	}


BEGIN_MESSAGE_MAP( TPropPage_Time, CPropertyPage )
	//{{AFX_MSG_MAP(TPropPageL3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	 ON_WM_DESTROY()
	 ON_WM_SHOWWINDOW()
	 ON_BN_CLICKED( IDC_RADIO_FNAM, OnFnam_Clicked )
	 ON_BN_CLICKED( IDC_RADIO_DATE, OnDate_Clicked )
	 ON_BN_CLICKED( IDC_RADIO_ALL, OnAll_Clicked )
		ON_BN_CLICKED( IDC_RADIO_SEL, OnSel_Clicked )		

		ON_BN_CLICKED( IDC_BUTTON_REPORT, OnPrintRep_Clicked )

		ON_BN_CLICKED( IDC_BUTTON_REFILL, OnRefill_Clicked )		

		ON_NOTIFY( DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDTNotify1 )
		ON_NOTIFY( DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnDTNotify1 )

		ON_NOTIFY( DTN_CLOSEUP, IDC_DATETIMEPICKER1, OnDTNotify1 )
		ON_NOTIFY( DTN_CLOSEUP, IDC_DATETIMEPICKER2, OnDTNotify1 )

		ON_BN_CLICKED( IDC_BUTTON_JUMP, OnJump_Clicked )
		ON_BN_CLICKED( IDC_BUTTON_REMOVE, OnRemove_Clicked )		

		ON_WM_HELPINFO()

END_MESSAGE_MAP()

void TPropPage_Time::Refresh_Time( CDateTimeCtrl& rC, COleDateTime& rT )
	{
	  COleDateTime tmp;
	  rC.GetTime( tmp );
			if( tmp.GetStatus() != COleDateTime::valid )
				{
				  CString str; 
						str.Format( _T("Date is invalid. Reset to previous date %s"), (LPCTSTR)rT.Format() );
				  MessageBox( str, _T("Error"), MB_OK|MB_ICONEXCLAMATION );
						rC.SetTime( rT );

						return;
				}

			rT = tmp;
		
			m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
			m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

			MakeRefresh();			
	}

void TPropPage_Time::OnDTNotify1( NMHDR *pHdr, LRESULT* pRes )
 {
	  if( pHdr->code == DTN_DATETIMECHANGE )
				{
						if( pHdr->idFrom == IDC_DATETIMEPICKER1 )
							{
									if( m_tm1.GetMonthCalCtrl() != NULL ) return;							
									Refresh_Time( m_tm1, m_od1 );									
							}
						else
							{
									if( m_tm2.GetMonthCalCtrl() != NULL ) return;																	
									Refresh_Time( m_tm2, m_od2 );
							}						
			}
			else if( pHdr->code == DTN_CLOSEUP )
				 Refresh_Time( 
					  pHdr->idFrom == IDC_DATETIMEPICKER1 ? m_tm1:m_tm2,
							pHdr->idFrom == IDC_DATETIMEPICKER1 ? m_od1:m_od2
						);
 }

void TPropPage_Time::MakeRefresh()
	{	 
	   try {
						if( m_vecSrc[0].m_pRec->RecordCount == 0 )  
								m_vecSrc[0].m_pRec->Requery( adOptionUnspecified );			
						else
							{
									_variant_t vt( m_vecSrc[0].m_pRec->Bookmark );
										m_vecSrc[0].m_pRec->Requery( adOptionUnspecified );			
										if( m_vecSrc[0].m_pRec->RecordCount != 0 ) 
													m_vecSrc[0].m_pRec->Bookmark = vt;
							}
				}
				catch( _com_error &e )
					{
							ShowErr( this, e );		
							//return FALSE;
					}	
	}

void TPropPage_Time::OnFnam_Clicked()
	{
	  m_iSrt = 0;
			m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
			m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

   MakeRefresh();			
	}
void TPropPage_Time::OnDate_Clicked()
	{
	  m_iSrt = 1;			
			m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
			m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

			MakeRefresh();			
	}
void TPropPage_Time::OnAll_Clicked()
	{
	  m_bFlAll = true;			
			m_lstCOX.front()->EnableWindow( m_bFlAll == false );
	  m_dtaLst.SetLocked( TRUE );
			m_dtaLst.SetForeColor( RGB(192, 192, 192) );

			m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
			m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

			MakeRefresh();			
	}
void TPropPage_Time::OnSel_Clicked()
	{
	  m_bFlAll = false;
			m_lstCOX.front()->EnableWindow( m_bFlAll == false );
	  m_dtaLst.SetLocked( FALSE );
			m_iFltCat = GetCategory();
			m_dtaLst.SetForeColor( RGB(64, 128, 128) );

			m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
			m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

			MakeRefresh();			
	}

int TPropPage_Time::GetCategory()
	{ 
   if( m_bLockLst ) return 0;

	  try {
						CString str = m_dtaLst.GetText();

						m_pCmdFind->Parameters->GetItem( _T("namecat") )->PutValue( 
													_variant_t(_bstr_t((LPCTSTR)str)) );

						m_pRecFind->Requery( adOptionUnspecified );										
      
						if( 
							   m_pRecFind->BOF == VARIANT_TRUE || m_pRecFind->EndOfFile == VARIANT_TRUE || 
										m_pRecFind->AbsolutePosition == adPosUnknown || 
							   m_pRecFind->RecordCount == 0 ) return 0;
						else
						  return (long)m_pRecFind->Fields->GetItem( _T("ID_CAT") )->Value;
				}
			catch( _com_error &e )
	   {
	     ShowErr( this, e );		
					 //return FALSE;
	   }	

			return 0;
	}

void TPropPage_Time::OnRefill_Clicked()
	{
	  RefillLst();
	}

void TPropPage_Time::OnMoveCompleteADODC_Cat( long adReason, LPDISPATCH pError, long FAR* adStatus, LPDISPATCH pRecordset )
	{	  
	}

BEGIN_EVENTSINK_MAP( TPropPage_Time, CPropertyPage ) 
  ON_EVENT( TPropPage_Time, IDC_DC_CATEGORY_G, 201 /* MoveComplete */, OnMoveCompleteADODC_Cat, VTS_I4 VTS_DISPATCH VTS_PI4 VTS_DISPATCH ) 		
		ON_EVENT( TPropPage_Time, IDC_DATALIST_CAT, 1 /* Change */, OnChangeDatacombo_Cat, VTS_NONE )
END_EVENTSINK_MAP()


void TPropPage_Time::OnChangeDatacombo_Cat() 
	{
	  m_iFltCat = GetCategory(),
			m_vecSrc[0].m_pCmd->CommandText = (LPCTSTR)GetQ();
			m_vecSrc[0].m_pCmd->Prepared = VARIANT_TRUE;

			MakeRefresh();			
	}


void TPropPage_Time::OnPrintRep_Clicked()
 {
			UpdateData(TRUE);
			m_pCRJob->DiscardSavedData();
			double idCat = m_iFltCat;
			CRDateTime dateFrom = {
				CRYearMonthDayToDate(m_od1.GetYear(), m_od1.GetMonth(), m_od1.GetDay()),
				0 };
			CRDateTime dateTo = {
				CRYearMonthDayToDate(m_od2.GetYear(), m_od2.GetMonth(), m_od2.GetDay()),
				0 };
			short nParameterFields = m_pCRJob->GetNParameterFields();
			while (nParameterFields--) {
				CRPEParameterFieldInfo pfi;
				m_pCRJob->GetNthParameterField(nParameterFields, &pfi);
				if (*(pfi.m_ReportName) != _T('\0')) // !_tcscmp(pfi.m_ReportName, _T(""))
					continue;
				if (_tcscmp(pfi.m_Name, _T("@CategoryId")) == 0) {
					ASSERT(pfi.m_ValueType == PEP_PF_NUMBER);
					*(double*)pfi.m_CurrentValue = idCat;
					pfi.m_CurrentValueSet = TRUE;
					VERIFY(m_pCRJob->SetNthParameterField(nParameterFields, &pfi));
					continue;
				}
				if (_tcscmp(pfi.m_Name, _T("@DateFrom")) == 0) {
					ASSERT(pfi.m_ValueType == PEP_PF_DATETIME);
					*(CRDateTime*)pfi.m_CurrentValue = dateFrom;
					pfi.m_CurrentValueSet = TRUE;
					VERIFY(m_pCRJob->SetNthParameterField(nParameterFields, &pfi));
					continue;
				}
				if (_tcscmp(pfi.m_Name, _T("@DateTo")) == 0) {
					ASSERT(pfi.m_ValueType == PEP_PF_DATETIME);
					*(CRDateTime*)pfi.m_CurrentValue = dateTo;
					pfi.m_CurrentValueSet = TRUE;
					VERIFY(m_pCRJob->SetNthParameterField(nParameterFields, &pfi));
					continue;
				}
				//ASSERT(FALSE);
			}
			CRPEWindowOptions wo(PEP_UNCHANGED, PEP_UNCHANGED, PEP_UNCHANGED,
				FALSE, PEP_UNCHANGED, PEP_UNCHANGED, PEP_UNCHANGED, 
				PEP_UNCHANGED, PEP_UNCHANGED, PEP_UNCHANGED, TRUE, PEP_UNCHANGED);
			m_pCRJob->SetWindowOptions(&wo);
			m_pCRJob->OutputToWindow(_T("Bills Report Preview"), CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, (CWnd*)NULL);
			m_pCRJob->Start();
 }


BOOL TPropPage_Time::OnApply()
	{
	  //MessageBeep( -1 );
	  return TRUE;
	}

void TPropPage_Time::OnCancel()
	{
	  //::MessageBeep( -1 );
	}

void TPropPage_Time::OnRemove_Clicked()
	{
   if( m_vecSrc[0].m_pRec->BOF == VARIANT_TRUE || 
				   m_vecSrc[0].m_pRec->EndOfFile == VARIANT_TRUE || 										
							m_vecSrc[0].m_pRec->RecordCount == 0
					)
				{
				  CString str; 
						str.Format( _T("Nothing to remove - no bills in %s - %s"),
							 (LPCTSTR)m_od1.Format(VAR_DATEVALUEONLY), (LPCTSTR)m_od2.Format(VAR_DATEVALUEONLY) );
      MessageBox( str, _T("Warning"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}

	  TLoginDlg dialogPwd( this );

			int iCntTry = 0;
			while( 1 )
				{
			   if( dialogPwd.DoModal() != IDOK ) return;
						if( dialogPwd.csPwd.CompareNoCase(lpPassword_for_removeAll) == 0 ) break;
						if( iCntTry >= 5 ) 
							{
							  MessageBox( _T("Access to removing is denied"),
										           _T("Information"), MB_OK|MB_ICONINFORMATION );
									return;
							}
						MessageBox( _T("Wrong password. Try again."), 
							           _T("Information"), MB_OK|MB_ICONINFORMATION );
						++iCntTry;
				}
			
			try {
				 TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );
				 _CommandPtr pC;
					MyCOMChk( pC.CreateInstance(_T("ADODB.Command")) );
								
					pC->ActiveConnection = pPar->m_pConnection;
					pC->CommandType = adCmdStoredProc;
					pC->CommandText = _T("GuestBill_delcascade_time");
					pC->Prepared = VARIANT_TRUE;										

					pC->Parameters->Append(
								pC->CreateParameter(
											_T("d1"), adDBDate, adParamInput, 
											-1, _variant_t((DATE)m_od1, VT_DATE)
											
									) 
						);
					pC->Parameters->Append(
								pC->CreateParameter(
											_T("d1"), adDBDate, adParamInput, 
											-1, _variant_t((DATE)m_od2, VT_DATE)
											
									) 
						);

					_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
					pC->Execute( &vtEmpty, &vtEmpty, -1 );
					m_vecSrc[0].m_pRec->Requery( adOptionUnspecified );
				}
			catch( _com_error &e )
	   {
	     ShowErr( this, e );					
	   }	
	}

void TPropPage_Time::OnJump_Clicked()
	{	  
	  if( m_vecSrc[0].m_pRec->BOF == VARIANT_TRUE || 
				   m_vecSrc[0].m_pRec->EndOfFile == VARIANT_TRUE || 										
							m_vecSrc[0].m_pRec->RecordCount == 0
					)
				{
      MessageBox( _T("Cann't jump. Selected record needed"), _T("Warning"), MB_OK|MB_ICONEXCLAMATION );
						return;
				}

			TMyProperty *pPar = reinterpret_cast<TMyProperty*>( GetParent() );
			TPropPage_Bill *p = dynamic_cast<TPropPage_Bill*>( pPar->GetPage(0) );
			if( !p ) return;

			try {
						_variant_t vvtGuest( m_vecSrc[ 0 ].m_pRec->Fields->GetItem(_T("ID_GUEST"))->Value );
						_variant_t vvtBill( m_vecSrc[ 0 ].m_pRec->Fields->GetItem(_T("BILL_NO"))->Value );

						p->m_vecSrc[ 0 ].m_pRec->MoveFirst();

						CString str; str.Format( _T("ID_GUEST = %lu"), (long)vvtGuest );
						if( FAILED(p->m_vecSrc[ 0 ].m_pRec->Find( _bstr_t((LPCTSTR)str), 0L, adSearchForward, _variant_t(0L))) )
							{
									MessageBox( _T("Cann't find this record"), _T("Error"), MB_OK|MB_ICONSTOP );
									return;
							}						
						
						if( !(p->m_vecSrc[ 1 ].m_pRec->BOF == VARIANT_TRUE || 
										p->m_vecSrc[ 1 ].m_pRec->EndOfFile == VARIANT_TRUE || 										
										p->m_vecSrc[ 1 ].m_pRec->RecordCount == 0)
								)
							{
							  //p->m_vecSrc[1].m_pCmd->Parameters->GetItem( _T("ID_GUEST") )->PutValue( vvtGuest );
									//p->m_vecSrc[ 1 ].m_pRec->Requery( adOptionUnspecified );
									str.Format( _T("BILL_NO = %lu"), (long)vvtBill );
									p->m_vecSrc[ 1 ].m_pRec->Find( _bstr_t((LPCTSTR)str), 0L, adSearchForward, _variant_t(0L) );
							}
   
						pPar->SetActivePage( 0 );
				}
			catch( _com_error &e )
	   {
	     ShowErr( this, e );					
	   }	
	}

BOOL TPropPage_Time::OnHelpInfo( HELPINFO* lpHelpInfo )
	{
	  if( lpHelpInfo->iContextType != HELPINFO_WINDOW )
	    return Default();

			static pair<int, TCtxBnd> arrInit[] =
				{
				  pair<int, TCtxBnd>( IDC_RADIO_FNAM, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),
						pair<int, TCtxBnd>( IDC_RADIO_DATE, TCtxBnd(IDH_Sorte, HELP_CONTEXTPOPUP) ),

						pair<int, TCtxBnd>( IDC_BUTTON_REMOVE, TCtxBnd(IDH_ListBills, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_RADIO_ALL, TCtxBnd(IDH_ListBills, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_RADIO_SEL, TCtxBnd(IDH_ListBills, HELP_CONTEXT) ),
						pair<int, TCtxBnd>( IDC_BUTTON_JUMP, TCtxBnd(IDH_ListBills, HELP_CONTEXT) )
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
