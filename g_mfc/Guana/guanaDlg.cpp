// guanaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "guanaDlg.h"
#include "resource.h"

#include "HELP\\guana.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(TMyProperty, CPropertySheet)
IMPLEMENT_DYNAMIC( TFineTab, CTabCtrl )


void TFineTab::DrawItem( LPDRAWITEMSTRUCT lpDr )
{
  TCITEM tc;	 
  //GetItem( lpDr->itemID, &tc );

  CDC dc; dc.Attach( lpDr->hDC );
  dc.FillSolidRect( &lpDr->rcItem, /*::GetSysColor(COLOR_BTNFACE)*/RGB(255,0,0) );
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


TMyProperty::TMyProperty(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
	 p_wLastFocus( NULL ),
		m_bFlConnOpen( false )
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  AddControlPages();
}

TMyProperty::TMyProperty(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	 p_wLastFocus( NULL ),
		m_bFlConnOpen( false )
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME_I);
  m_psh.dwFlags |= PSP_USEHICON | PSH_NOAPPLYNOW;
  m_psh.dwFlags &= ~PSH_HASHELP;
  m_psh.hIcon = m_hIcon;  

		globl_iMsgIniNav = ::RegisterWindowMessage( "WM_GB_ININAV" );

  AddControlPages();
}

TMyProperty::~TMyProperty()
 {
   dlSplash.SetCancel();

   try {
					 m_pagBill.m_vecSrc.clear();
						m_pagSys.m_vecSrc.clear();
						m_pagTime.m_vecSrc.clear();
				}
			catch( _com_error &e )
				{
						//ShowErr( this, e );					
				}	  

			try {
					if( m_bFlConnOpen )
							m_pConnection->Close();
				}
			catch( _com_error &e )
				{
						//ShowErr( this, e );					
				}	  
			dlSplash.SetCancel();
 }


void TMyProperty::AddControlPages()
 {   
   AddPage( &m_pagBill );
   AddPage( &m_pagSys );
   AddPage( &m_pagTime );   
 }

/////////////////////////////////////////////////////////////////////////////
// CGENWDlg message handlers

void TMyProperty::OnShowWindow( BOOL bShow, UINT nStatus )
 { 	  
   CPropertySheet::OnShowWindow( bShow, nStatus );
			
			//ResetLastFocus();
		 //CallNavAss( &m_pagBill.m_GridBill, true );				

			PostMessage( globl_iMsgIniNav, 0, 0 );
 }

void TMyProperty::MakeExitBtn( CWnd* pWnd )
 {
   pWnd->ModifyStyleEx( 0, WS_EX_STATICEDGE );
 	 
   CRect r, rBtn;
   GetClientRect( r ); pWnd->GetWindowRect( rBtn );
   ScreenToClient( rBtn );
   int iX = float(rBtn.Width()) * 1.5;
   pWnd->SetWindowText( "E&xit" );
   rBtn.left = (r.Width() - iX) / 2; rBtn.right = rBtn.left + iX;
   pWnd->MoveWindow( rBtn );   
	   //DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 );
 }

HBRUSH TMyProperty::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CPropertySheet::OnCtlColor( pDC, pWnd, nCtlColor );
   
   int id = pWnd->GetDlgCtrlID();
   if( id == IDCANCEL )
     pDC->SetTextColor( RGB(0,0,255) );   

   return hbr;
} 

BOOL TMyProperty::PreCreateWindow( CREATESTRUCT& cs )
	{
   BOOL bFl = CPropertySheet::PreCreateWindow( cs );
   if( !bFl ) return FALSE;

			cs.dwExStyle |= WS_EX_CONTEXTHELP;

	  return TRUE;
	}

BOOL TMyProperty::OnInitDialog()
{    		
  CPropertySheet::OnInitDialog();				
				

  ModifyStyleEx( 0, WS_EX_CONTEXTHELP );

		CWnd *pOK = GetDlgItem( IDCANCEL );
		CWnd *pCancel = GetDlgItem( IDOK );
		if( pOK ) pOK->DestroyWindow();
		pCancel->ModifyStyleEx( 0, WS_EX_STATICEDGE );
		pCancel->SetWindowText( "E&xit" );
					//MakeExitBtn( pCancel );	
		
		pCancel->SetFont( CFont::FromHandle((HFONT)::GetStockObject(SYSTEM_FIXED_FONT)) );		

		m_ftWrap.SubclassWindow( GetTabControl()->m_hWnd );

		GetTabControl()->ModifyStyleEx( 0, WS_EX_STATICEDGE );
		GetTabControl()->ModifyStyle( 0, TCS_TOOLTIPS|TCS_SINGLELINE|TCS_RAGGEDRIGHT|TCS_BOTTOM );
		SetActivePage( 1 ), SetActivePage( 0 );
		
		CRect r, r2; GetClientRect( r ); //r.right++; r.bottom++;
		GetTabControl()->MoveWindow( r );       	    
		
		GetTabControl()->GetItemRect( GetPageCount() - 1, r );	
		CSize sz; sz.cx = r.Width(); //sz.cy = r.Height();
		pCancel->GetWindowRect( r2 ); sz.cy = r2.Height() * 1.3;		

					int iBrd = float(sz.cy) * 0.2;

  
		CPoint ptNavSz( m_nav.GetRecSize() );
		int iSkip = ptNavSz.x + 20;

		GetClientRect( r2 );
		pCancel->SetWindowPos(  &wndTop,
			r.right + iSkip,//4*iBrd,
			r2.bottom - sz.cy, 
			r2.Width() - r.right - iSkip - 5, 
			sz.cy - iBrd, SWP_NOACTIVATE );

		CRect rTmp; pCancel->GetWindowRect( rTmp ); ScreenToClient( rTmp );
		int iTop = rTmp.top + (rTmp.Height() - ptNavSz.y) / 2; 
		CRect rVav( r.right + 10, iTop, 0, 0 ); 
		rVav.right = rVav.left + ptNavSz.x;
		rVav.bottom = rVav.top + ptNavSz.y;
		m_nav.Create( _T(""), WS_CHILD|WS_VISIBLE, rVav, this, 900 );

		

		GetTabControl()->SetItemSize( sz );			
		SetActivePage( 1 ), SetActivePage( 0 );

		m_apToolTip = auto_ptr<CToolTipCtrl>( new CToolTipCtrl() );    
		m_apToolTip->Create( this );
		GetTabControl()->SetToolTips( m_apToolTip.get() );

		CRect rr;
		GetTabControl()->GetItemRect( 0, &rr );	
		m_apToolTip->AddTool( 
			GetTabControl(), "tool 1", &rr, 1 );

		GetTabControl()->GetItemRect( 1, &rr );
		m_apToolTip->AddTool( 
			GetTabControl(), "tool 2", &rr, 2 );

		GetTabControl()->GetItemRect( 2, &rr );
		m_apToolTip->AddTool( 
			GetTabControl(), "tool 3", &rr, 3 );
		

		// Add "About..." menu item to system menu.

		// IDM_ABOUTBOX must be in the system command range.
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		//SetWindowLong( GetSafeHwnd(), GWL_STYLE, 
				//(GetWindowLong(GetSafeHwnd(), GWL_STYLE) & ~WS_POPUP & ~WS_CHILD) | WS_MINIMIZEBOX | WS_OVERLAPPED );

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		 {
					CString strAboutMenu;
					strAboutMenu.LoadString(IDS_ABOUTBOX);
					if (!strAboutMenu.IsEmpty())
						{
								pSysMenu->AppendMenu(MF_SEPARATOR);
								pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
						}
		 }

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		
		// TODO: Add extra initialization here	    	
				
		m_btnExit.SubclassDlgItem( IDOK, this );		
		m_btnExit.ModifyStyle( 0, BS_OWNERDRAW );
		m_btnExit.SetToolTipText( _T("Close application") );
		m_btnExit.SetToolTip( TRUE );
		m_btnExit.SetTrackLook( TRUE );		
		m_btnExit.SetTextColor( RGB(0,0,255) );								

		SetActivePage( GetProfileValueInt(_T("mainwindow"), _T("activepage"), 0) );

		return TRUE;  // return TRUE  unless you set the focus to a control
}


void TMyProperty::OnDestroy() 
 {    			
   dlSplash.SetCancel();

   if( GetTabControl() ) 
				 SetProfileValueInt( _T("mainwindow"), _T("activepage"), GetActiveIndex() );
			CPropertySheet::OnDestroy();			
 }


/*class TExitExc: public CException
	{
public:
	  TExitExc( LPCTSTR lp ): 
						m_csErr( lp ), 
						CException( TRUE )
				{				 
				}

			virtual BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL )
				{
				  int iCpn = min( m_csErr.GetLength(), nMaxError - 1 );
				  _tcsncpy( lpszError, (LPCTSTR)m_csErr, iCpn );
						*(lpszError + iCpn) = 0;
						return TRUE;
				}
private:
	 CString m_csErr;
	};*/


bool TMyProperty::InitDataSrc()
	{	
	  // AS: This dialog was called already.
			//     Don't call it twice.
			/*
   TCnctDlg dlg( NULL );

			dlg.m_iSecurity = GetProfileValueInt( _T("connect"), _T("security"), 0 );
			dlg.m_csServer = GetProfileValueStr( _T("connect"), _T("server"), _T("SERVER") );
			dlg.m_cs_DBName = GetProfileValueStr( _T("connect"), _T("db"), _T("guana_bill") );
			dlg.m_csUser = GetProfileValueStr( _T("connect"), _T("user"), _T("sa") );   

			if( dlg.DoModal() != IDOK ) 
				{
				  //PostQuitMessage( 0 );
				  //throw new TExitExc( _T("Connection aborted by user") );
				  return false;
				}
			

			SetProfileValueInt( _T("connect"), _T("security"), dlg.m_iSecurity );
			SetProfileValueStr( _T("connect"), _T("server"), dlg.m_csServer );
			SetProfileValueStr( _T("connect"), _T("db"), dlg.m_cs_DBName );
			SetProfileValueStr( _T("connect"), _T("user"), dlg.m_csUser );*/

			MyCOMChk( m_pConnection.CreateInstance(_T("ADODB.Connection")) );
			m_pConnection->CursorLocation = adUseServer;
			m_pConnection->IsolationLevel = adXactSerializable;
			
			int iSecurity = GetProfileValueInt( _T("connect"), _T("security"), 0 );
			CString csServer = GetProfileValueStr( _T("connect"), _T("server"), _T("SERVER") );
			CString csDBName = GetProfileValueStr( _T("connect"), _T("db"), _T("guana_bill") );
			CString csUser = GetProfileValueStr( _T("connect"), _T("user"), _T("sa") );

			CString sCnct;
			if( iSecurity )
			  sCnct.Format( _T("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s"), 
					  (LPCTSTR)((CGuanaApp*)AfxGetApp())->m_csPWD, (LPCTSTR)csUser, (LPCTSTR)csDBName, (LPCTSTR)csServer );
			else
				 sCnct.Format( _T("Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=%s;Data Source=%s"), 
					  (LPCTSTR)csDBName, (LPCTSTR)csServer );			

			MyCOMChk( m_pConnection->Open( (LPCTSTR)sCnct,	_T(""), _T(""),
				  //(LPCTSTR)(dlg.m_csPWD.IsEmpty() ? _T(""):(dlg.m_iSecurity ? dlg.m_csPWD:_T(""))), 
				  adConnectUnspecified) );						   
			m_bFlConnOpen = true;

			return true;
	}

void TMyProperty::OnSysCommand(UINT nID, LPARAM lParam)
 {
			if ((nID & 0xFFF0) == IDM_ABOUTBOX)
			 {
				  CAboutDlg dlgAbout;
				  dlgAbout.DoModal();
			 }
			else
			 {
				  CPropertySheet::OnSysCommand(nID, lParam);
			 }
 }

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR TMyProperty::OnQueryDragIcon()
 {
   return (HCURSOR) m_hIcon;
 }

int TMyProperty::OnCreate( LPCREATESTRUCT lpC )
{  
  if( CPropertySheet::OnCreate(lpC) == -1 ) return -1;

  // AS: Will be done in InitDialog		
		bool bFlOpen = false;
	 try {
		  if( InitDataSrc() == true ) bFlOpen = true;
			}
		catch( _com_error &e )
	  {
	    ShowErr( this, e );						
	  }		

		if( bFlOpen == false )
			{
     MessageBox( _T("Connection to DB not open"), _T("Information"), MB_OK|MB_ICONINFORMATION );
					return -1;
			}
    
  return 0;
}

CString TMyProperty::GetCaptionFor( CDataGrid* pGr )
	{
	  map<LPVOID, CString>::iterator it = 
				 m_mapCapt.find( (LPVOID)pGr );

			if( it == m_mapCapt.end() ) return _T("");
			else return it->second;
	}

CAdodc* TMyProperty::GetADC_for_Grid( CDataGrid* pG )
	{
	  return (CAdodc*)m_mapGRID_DC.find( pG )->second;
	}

void TMyProperty::CallNavAss( CWnd* pDataControl, bool bFlDirectly )
	{
	  if( bFlDirectly == false && (LPVOID)p_wLastFocus == (LPVOID)pDataControl ) return;

			p_wLastFocus = pDataControl;
			map<LPVOID, LPVOID>::iterator it = 
				 m_mapGRID_DC.find( pDataControl );
			if( it == m_mapGRID_DC.end() ) return;

			m_nav.Assign( (CWnd*)it->first, (CAdodc*)it->second );
	}

extern UINT globl_iMsgIniNav = 0;

BEGIN_MESSAGE_MAP( TMyProperty, CPropertySheet )
	//{{AFX_MSG_MAP(TMyProperty)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
    //ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( globl_iMsgIniNav, OnInitNavigator )
	ON_WM_DESTROY()

	ON_WM_HELPINFO()

	/*ON_COMMAND( ID_HELP_FINDER, CPropertySheet::OnHelpFinder )
	ON_COMMAND( ID_HELP, CPropertySheet::OnHelp )	
	ON_COMMAND( ID_DEFAULT_HELP, CPropertySheet::OnHelpFinder )*/

END_MESSAGE_MAP()

LONG TMyProperty::OnInitNavigator( UINT, LONG )
	{
	  ResetLastFocus();
   switch( GetActiveIndex() )
				{
				  case 0:
							{
							  CallNavAss( &m_pagBill.m_GridGuest, true );
									try {
									   m_pagBill.m_mgrBind.Fetch(),
						      m_pagBill.m_mgrBind.LoadToControl();
										}
									catch( _com_error &e )
										{
										}
									catch( CException *pE )
										{
										  pE->Delete();
										}
							}
						break;
						case 1:
							 CallNavAss( &m_pagSys.m_GridRoom, true );
							 break;
						case 2:
							 CallNavAss( &m_pagTime.m_GridLst, true );
							 break;
				};

	  return 0;
	}

BOOL TMyProperty::OnApply()
	{
	  MessageBeep( -1 );
	  return FALSE;
	}

void TMyProperty::OnCancel()
	{
	  ::MessageBeep( -1 );
	}

BOOL TMyProperty::PreTranslateMessage( MSG* pMsg )
	{  
	  if( pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN )
				{
				  if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
								{
									 if( pMsg->message == WM_KEYUP ) ::MessageBeep( -1 );
										return TRUE;
							}
				}

			return CPropertySheet::PreTranslateMessage( pMsg );
	}

BOOL TMyProperty::OnHelpInfo( HELPINFO* lpHelpInfo )
	{
	  if( lpHelpInfo->iContextType != HELPINFO_WINDOW )
	    return Default();
			
			//lpHelpInfo->dwContextId = IDH_MainWindow;
			AfxGetApp()->WinHelp( IDH_MainWindow, HELP_CONTEXT );

			return TRUE;
	}
