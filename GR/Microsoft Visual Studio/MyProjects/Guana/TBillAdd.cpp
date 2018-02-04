// TBillAdd.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "ADO_DSRC\\field.h"
#include "ADO_DSRC\\fields.h"

#include "TBillAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TBillAdd dialog


TBillAdd::TBillAdd( LPCTSTR lpUsr, CDataGrid *pWGrid, 
																				_RecordsetPtr& rBill, CAdodc& rAdc, 
																				CDataGrid *pWGrid_Room,
																				_RecordsetPtr& rRoom, _ConnectionPtr& rConn,
																				CPoint& rp, CWnd* pParent
																		)
	: CDialog( TBillAdd::IDD, pParent ),
	  m_rBill( rBill ), m_rAdc( rAdc ),
			m_rPt( rp ),
   m_pWGrid( pWGrid ),
			hbrBkEdit( RGB(240,245,255) ),
			m_csUsr( lpUsr ),
			m_pWGrid_Room( pWGrid_Room ), m_rRoom( rRoom ), m_rConn( rConn )
{
	//{{AFX_DATA_INIT(TBillAdd)
	m_odtDate = COleDateTime::GetCurrentTime();
	m_odtFrom = COleDateTime::GetCurrentTime();
	m_odtTo = COleDateTime::GetCurrentTime();
	m_uiBillNo = 0;
	m_csComments = _T("");
	m_csDetails = _T("NOT ENTERED");
	m_csFolio = _T("F1");
	m_csGI = _T("NOT ENTERED");
	m_uiNoOfGuests = 2;
	//}}AFX_DATA_INIT
	COleDateTimeSpan span; span.SetDateTimeSpan( 3, 0, 0, 0 );
	m_odtTo += span;
}

void TBillAdd::ChecheckEmpty( CString& rS, LPCTSTR lpMsg, int iCtl )
	{
	  if( !rS.IsEmpty() ) return;

			MessageBox( lpMsg, _T("Error"), MB_OK|MB_ICONEXCLAMATION );
			PostMessage( WM_NEXTDLGCTL, (WPARAM)::GetDlgItem(m_hWnd, iCtl), 1 ),

			throw exception( _T("CHECKING") );
	}


void TBillAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TBillAdd)
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATETO, m_dtcDateTo);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATEFROM, m_dtcDateFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_dtcDate);
	DDX_Control(pDX, IDC_DATACOMBO_ROOM, m_dcRoom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATE, m_odtDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATEFROM, m_odtFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATETO, m_odtTo);
	DDX_Text(pDX, IDC_EDIT_BILLNO, m_uiBillNo);
	DDV_MinMaxUInt(pDX, m_uiBillNo, 0, UINT_MAX);
	DDX_Text(pDX, IDC_EDIT_COMMENTS, m_csComments);
	DDV_MaxChars(pDX, m_csComments, 256);
	DDX_Text(pDX, IDC_EDIT_DETAILS, m_csDetails);
	DDV_MaxChars(pDX, m_csDetails, 64);
	DDX_Text(pDX, IDC_EDIT_FOLIO, m_csFolio);
	DDV_MaxChars(pDX, m_csFolio, 15);
	DDX_Text(pDX, IDC_EDIT_GI, m_csGI);
	DDV_MaxChars(pDX, m_csGI, 35);
	DDX_Text(pDX, IDC_EDIT_NOOFGUESTS, m_uiNoOfGuests);
	DDV_MinMaxUInt(pDX, m_uiNoOfGuests, 1, 200);
	//}}AFX_DATA_MAP
	if( pDX->m_bSaveAndValidate == TRUE )
		{ 
		  if( m_odtTo < m_odtFrom )						
						 MessageBox( _T("Needing Date To >= Date From"), _T("Error"), MB_OK|MB_ICONEXCLAMATION ),
					  PostMessage( WM_NEXTDLGCTL, (WPARAM)::GetDlgItem(m_hWnd, IDC_DATETIMEPICKER_DATETO), 1 ),
							pDX->Fail();

				if( m_odtDate != m_odtFrom )						
						 MessageBox( _T("Needing Date = Date From"), _T("Error"), MB_OK|MB_ICONEXCLAMATION ),
					  PostMessage( WM_NEXTDLGCTL, (WPARAM)::GetDlgItem(m_hWnd, IDC_DATETIMEPICKER_DATEFROM), 1 ),
							pDX->Fail();

				try {
						 ChecheckEmpty( m_csDetails, _T("Missing Details"), IDC_EDIT_DETAILS );
						 ChecheckEmpty( m_csFolio, _T("Missing Folio"), IDC_EDIT_FOLIO );
						 ChecheckEmpty( m_csGI, _T("Missing Heard of GI"), IDC_EDIT_GI );						 
					}
				catch( exception& e )
					{
					  pDX->Fail();
					}
		}
}


BEGIN_MESSAGE_MAP(TBillAdd, CDialog)
	//{{AFX_MSG_MAP(TBillAdd)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TBillAdd message handlers

bool TBillAdd::MakePost()
	{
	  if( !UpdateData(TRUE) ) return false;

			CRect r; GetWindowRect( r );
   m_rPt.x = r.left; m_rPt.y = r.top;

			try {
      m_rConn->BeginTrans();

						//m_rBill->Fields->GetItem( _T("BILL_NO") )->Value = _variant_t( (long)m_uiBillNo );
						m_rBill->Fields->GetItem( _T("DATE_") )->Value = _variant_t( (DATE)m_odtDate, VT_DATE );
						m_rBill->Fields->GetItem( _T("FOLIO") )->Value = _bstr_t( (LPCTSTR)m_csFolio );
						
						m_rBill->Fields->GetItem( _T("HEARD_OF_GI") )->Value = _bstr_t( (LPCTSTR)m_csGI );
						m_rBill->Fields->GetItem( _T("DETAILS") )->Value = _bstr_t( (LPCTSTR)m_csDetails );

						if( !m_csComments.IsEmpty() )
						  m_rBill->Fields->GetItem( _T("COMMENTS") )->Value = _bstr_t( (LPCTSTR)m_csComments );
						else
							{
							  _variant_t vvt; vvt.vt = VT_NULL;
							  m_rBill->Fields->GetItem( _T("COMMENTS") )->Value = vvt;																		
							}
						
						_variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
						m_rBill->Update( vtEmpty, vtEmpty );

						
			   m_rRoom->AddNew( vtEmpty, vtEmpty );

						m_rRoom->Fields->GetItem( _T("NO_OF_GUESTS") )->Value = _variant_t( (long)m_uiNoOfGuests );
						m_rRoom->Fields->GetItem( _T("DATE_IN") )->Value = _variant_t( (DATE)m_odtFrom, VT_DATE );
						m_rRoom->Fields->GetItem( _T("DATE_OUT") )->Value = _variant_t( (DATE)m_odtTo, VT_DATE );						


						C_Recordset rs( m_rAdc.GetRecordset() );
						CString str;
						str.Format( _T("NAME='%s'"), (LPCTSTR)m_dcRoom.GetText() );
						rs.MoveFirst();
						rs.Find( _bstr_t((LPCTSTR)str), 0L, adSearchForward, _variant_t(0L) );

						m_rRoom->Fields->GetItem( _T("__NAME") )->Value =
							 rs.GetFields().GetItem( _variant_t(_T("NAME")) ).GetValue();						  
						m_rRoom->Fields->GetItem( _T("ID_ROOM") )->Value = 
								rs.GetFields().GetItem( _variant_t(_T("ID_ROOM")) ).GetValue();						 

						m_rRoom->Update( vtEmpty, vtEmpty );
										
						m_pWGrid->SetFocus();
						m_pWGrid->SetEditActive( TRUE );

						m_rConn->CommitTrans();
				}
			catch( _com_error &e )
				{				 
						ShowErr( this, e );
						m_rConn->RollbackTrans();
						return false;
				}		
			catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();
						m_rConn->RollbackTrans();
						return false;
				}

			return true;
	}


void TBillAdd::OnCancel() 
{
	try {
			  m_pWGrid->SetEditActive( FALSE );																														
			  m_pWGrid->SetDataChanged( FALSE );			  	    					
					//m_rBill->Delete( adAffectCurrent );
					m_rBill->CancelUpdate();

					if( m_rBill->RecordCount != 0 )
					  m_rBill->MoveLast();					
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

void TBillAdd::OnOK() 
{
	 if( !MakePost() ) return;
	
	 CDialog::OnOK();
}

int TBillAdd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		  return -1;

	 try {	 
			 _variant_t vtEmpty( DISP_E_PARAMNOTFOUND, VT_ERROR );
			 m_rBill->AddNew( vtEmpty, vtEmpty );
			}
		catch( _com_error &e )
				{
						ShowErr( this, e );
						return -1;
				}			   
	
	// TODO: Add your specialized creation code here
	
	 return 0;
}

HBRUSH TBillAdd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
			static int arr[] = 
							{									
									IDC_EDIT_BILLNO, 
									IDC_DATETIMEPICKER_DATE,
									IDC_EDIT_FOLIO,
									IDC_EDIT_GI, 
									IDC_EDIT_NOOFGUESTS,
									IDC_DATETIMEPICKER_DATEFROM, 
									IDC_DATETIMEPICKER_DATETO, 
									IDC_EDIT_DETAILS,
									IDC_EDIT_COMMENTS,
									0
							};
						      
   int id = pWnd->GetDlgCtrlID();
   for( int* p = arr; *p; ++p )
				 if( *p == id )
						{
	       pDC->SetTextColor( RGB(64,128,128) );
								pDC->SetBkColor( RGB(240,245,255) );
								return hbrBkEdit;
						}
			
   return hbr;		
}

BOOL TBillAdd::OnInitDialog() 
{
	 CDialog::OnInitDialog();
	
	 CRect r; GetDesktopWindow()->GetWindowRect( r );
		if( !r.PtInRect(m_rPt) )
				CenterWindow( GetParent() );
		else
		  SetWindowPos( NULL, m_rPt.x, m_rPt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );

		static TBtnInf iIds[] =
				{
						{ IDOK, _T("Add new bill") }, 
						{ IDCANCEL, _T("Cancel adding bill") },
						{ 0, NULL }
				};
		MakeDefCOXBtns( iIds, this, m_lstCOX );

		CString str; str.Format( _T("Adding new bill for guest: \"%s\""), (LPCTSTR)m_csUsr );
		SetWindowText( str );
		
		m_dtcDateTo.SetFormat( _TM_FDATE_ );
		m_dtcDateFrom.SetFormat( _TM_FDATE_ );
		m_dtcDate.SetFormat( _TM_FDATE_ );		

		static LOGFONT lf = { -12, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
		   OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		   VARIABLE_PITCH | FF_DONTCARE, _T("MS Sans Serif")
	  };

	 m_fntDFLT.CreateFontIndirect( &lf );

		static int iArrID[] = {
			  IDC_EDIT_BILLNO, IDC_DATETIMEPICKER_DATE, IDC_EDIT_FOLIO,
					IDC_EDIT_GI, IDC_EDIT_NOOFGUESTS, IDC_DATETIMEPICKER_DATEFROM, 
					IDC_DATETIMEPICKER_DATETO, IDC_EDIT_DETAILS, 
					IDC_EDIT_COMMENTS, 0
			};
		for( int* p = iArrID; *p; ++p )
			::SendMessage( ::GetDlgItem(m_hWnd, *p), WM_SETFONT, (WPARAM)(HFONT)m_fntDFLT, 0 );

		try {
					m_dcRoom.SetListField( _T("NAME") );
					m_dcRoom.SetRefRowSource( m_rAdc.GetControlUnknown() );								  
					m_rAdc.GetRecordset().MoveFirst();
					m_dcRoom.SetText( 
						  _bstr_t(m_rAdc.GetRecordset().GetFields().GetItem( _variant_t(_T("NAME")) ).GetValue())
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

void TBillAdd::OnDestroy() 
{
	 CDialog::OnDestroy();
	
  hbrBkEdit.DeleteObject();
		m_fntDFLT.DeleteObject();
}

BEGIN_EVENTSINK_MAP( TBillAdd, CDialog ) 
	
 ON_EVENT( TBillAdd, IDC_DATACOMBO_ROOM, -603 /* KeyPress */, OnKeyPressDatacombo1, VTS_PI2 )	

END_EVENTSINK_MAP()

void TBillAdd::OnKeyPressDatacombo1( short FAR* KeyAscii ) 
 {
	  *KeyAscii = 0;
	  ::MessageBeep( -1 );
 }
