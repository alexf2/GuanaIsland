// guana.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "guana.h"
#include "guanaDlg.h"
#include <odbcinst.h>
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TSpashDlg dlSplash;

/////////////////////////////////////////////////////////////////////////////
// CGuanaApp

BEGIN_MESSAGE_MAP(CGuanaApp, CWinApp)
	//{{AFX_MSG_MAP(CGuanaApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG	

	ON_COMMAND( ID_HELP_FINDER, CWinApp::OnHelpFinder )
	ON_COMMAND( ID_HELP, CWinApp::OnHelp )
	ON_COMMAND( ID_CONTEXT_HELP, CWinApp::OnContextHelp )
	ON_COMMAND( ID_DEFAULT_HELP, CWinApp::OnHelpFinder )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuanaApp construction

CGuanaApp::CGuanaApp(): m_apMainDlg( NULL )  
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGuanaApp object

CGuanaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGuanaApp initialization

BOOL CGuanaApp::InitInstance()
 {
   setlocale( LC_ALL, _T("") );
			AfxEnableControlContainer();						

			//if( !SUCCEEDED(::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED)) )
   if( !SUCCEEDED(::CoInitialize(NULL)) )
				{
						::MessageBox( NULL, "Error initializing COM", "Error", MB_OK|MB_ICONSTOP );
						return FALSE;
				}


			// Standard initialization
			// If you are not using these features and wish to reduce the size
			//  of your final executable, you should remove from the following
			//  the specific initialization routines you do not need.

		#ifdef _AFXDLL
			 Enable3dControls();			// Call this when using MFC in a shared DLL
		#else
			 Enable3dControlsStatic();	// Call this when linking to MFC statically
		#endif

   SetRegistryKey( _T("AC Guana Bill") );

	// AS: Getting DB connect info
	// To Alex: Here is a right place for calling this dialog.
	//          I've corrected your code to don't call this dialog
	//          twice. See guanaDlg.cpp
	TCnctDlg ConnectionDlg( NULL );
	ConnectionDlg.m_iSecurity = GetProfileValueInt( _T("connect"), _T("security"), 0 );
	ConnectionDlg.m_csServer = GetProfileValueStr( _T("connect"), _T("server"), _T("SERVER") );
	ConnectionDlg.m_cs_DBName = GetProfileValueStr( _T("connect"), _T("db"), _T("guana_bill") );
	ConnectionDlg.m_csUser = GetProfileValueStr( _T("connect"), _T("user"), _T("sa") );   
	
	//ConnectionDlg.Create( IDD_DIALOG_CONNECT );
	//ConnectionDlg.SetWindowPos( &CWnd::wndTop, 0,0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	//dlSplash.SetTttl( _T("Getting connect parameters...") );
	if (ConnectionDlg.DoModal() != IDOK)
		return FALSE;

	StartSplash( &dlSplash );
	//dlSplash.SetTttl( _T("Initializing application...") );
	

	SetProfileValueInt( _T("connect"), _T("security"), ConnectionDlg.m_iSecurity );
	SetProfileValueStr( _T("connect"), _T("server"), ConnectionDlg.m_csServer );
	SetProfileValueStr( _T("connect"), _T("db"), ConnectionDlg.m_cs_DBName );
	SetProfileValueStr( _T("connect"), _T("user"), ConnectionDlg.m_csUser );

 dlSplash.SetTttl( _T("Initializing crystal reports...") );

	TCHAR pszAttributes[256];
	wsprintf(pszAttributes, _T("DSN=GuanaTmp%cServer=%s%cDatabase=%s%cTrusted_Connection=%s%c"),
		0, LPCTSTR(ConnectionDlg.m_csServer), 0, 
		LPCTSTR(ConnectionDlg.m_cs_DBName), 0, 
		ConnectionDlg.m_iSecurity ? _T("no") : _T("yes"), 0);
	SQLConfigDataSource(NULL, ODBC_ADD_DSN, _T("SQL Server"), pszAttributes);

	if (!m_CREngine.Open())
		return FALSE;

	_tcscpy(m_LogOnInfo.m_serverName, _T("GuanaTmp"));
	if (ConnectionDlg.m_iSecurity) {
		// Use normal connection
		_tcscpy(m_LogOnInfo.m_userID, LPCTSTR(ConnectionDlg.m_csUser));
		_tcscpy(m_LogOnInfo.m_password, LPCTSTR(ConnectionDlg.m_csPWD));
	}
	_tcscpy(m_LogOnInfo.m_databaseName, LPCTSTR(ConnectionDlg.m_cs_DBName));

	m_CREngine.LogOnServer(_T("pdsmon.dll"), &m_LogOnInfo);

	// AS: End fixes

	dlSplash.SetTttl( _T("Opening connection to DB...") );

			m_apMainDlg = auto_ptr<TMyProperty>( new TMyProperty(_T("Guana Island bill v1.02")) );
			m_pMainWnd = m_apMainDlg.get();
			int nResponse = m_apMainDlg->DoModal();
			if (nResponse == IDOK)
			 {
				 // TODO: Place code here to handle when the dialog is
				 //  dismissed with OK
			 }
			else if (nResponse == IDCANCEL)
			 {
				 // TODO: Place code here to handle when the dialog is
				 //  dismissed with Cancel
			 }

			m_apMainDlg = auto_ptr<TMyProperty>( NULL );

			::CoUninitialize();

			// Since the dialog has been closed, return FALSE so that we exit the
			//  application, rather than start the application's message pump.
			return FALSE;
 }

int CGuanaApp::ExitInstance() 
{	
  dlSplash.SetCancel();

		// AS: Begin
		if (m_CREngine.GetEngineStatus() == CRPEngine::engineOpen) {
			while (!m_CREngine.CanClose()) ::Sleep(100);
			m_CREngine.LogOffServer(_T("pdsmon.dll"), &m_LogOnInfo);
			m_CREngine.Close();
		}

		SQLConfigDataSource(NULL, ODBC_REMOVE_DSN, _T("SQL Server"), _T("DSN=GuanaTmp\0"));
		// AS: End				

			return CWinApp::ExitInstance();
}

const CRPELogOnInfo* CGuanaApp::GetLogOnInfo() const
{
	return &m_LogOnInfo;
}

CGuanaApp::~CGuanaApp()
	{	  
	}

void CGuanaApp::WinHelp( DWORD dwData, UINT nCmd )
 {
   /*switch( dwData ) 
				{
						case 131200: case 262146:
								//dwData = IDH_Contents;
								break;	 
						case 387073: case 131201:
								//dwData = IDH_Interface;
								break;	 
						case 66236: case 66238: case 66239: case 66237: 
								//dwData = IDH_BrigEditor;
					break;
							};*/
    
   CWinApp::WinHelp( dwData, nCmd );    
 }
