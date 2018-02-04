#include "stdafx.h"
#include "RecreateDlg.h"

//
extern TSpashDlg dlSplash;

BEGIN_MESSAGE_MAP( TCnctDlg, CDialog )
 ON_BN_CLICKED( IDC_RADIO_WINNT, OnNT )
	ON_BN_CLICKED( IDC_RADIO3, OnUs )
	ON_BN_CLICKED(IDC_BTN_RECREATE, OnBtnRecreate)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


//################
static LPCTSTR lpPassword_for_createDB = _T("VOLF");
//################


void TCnctDlg::OnNT()
	{
	  CheckState();
	}
void TCnctDlg::OnUs()
	{
	  CheckState();
	}

void TCnctDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );            

   DDX_Radio( pDX, IDC_RADIO_WINNT, m_iSecurity );
        
   DDX_Text( pDX, IDC_EDIT_SERVER, m_csServer );
			DDX_Text( pDX, IDC_EDIT_DBNAME, m_cs_DBName );
			DDX_Text( pDX, IDC_EDIT_US, m_csUser );
			DDX_Text( pDX, IDC_EDIT_PWD, m_csPWD );   			
 }

void TCnctDlg::CheckState()
	{
	  BOOL bFl = reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_WINNT))->GetCheck() == 0;

			GetDlgItem(IDC_EDIT_US)->EnableWindow( bFl ),
			GetDlgItem(IDC_EDIT_PWD)->EnableWindow( bFl ),
			GetDlgItem(IDC_STATIC_USER)->EnableWindow( bFl ),
			GetDlgItem(IDC_STATIC_PWD)->EnableWindow( bFl );
	}

void TCnctDlg::OnOK()
 {   
   CDialog::OnOK();
 }

void TCnctDlg::OnShowWindow( BOOL bShow, UINT nStatus )
	{	  			
	  CDialog::OnShowWindow( bShow, nStatus );

			if( bShow )
			  SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE ),
					SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE );					
	}


BOOL TCnctDlg::OnInitDialog()
 {  
   CDialog::OnInitDialog();
 
   CenterWindow( GetParent() );
			CheckState();					

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_SERVER );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }
//################


void TCnctDlg::OnBtnRecreate() 
 {
	  //################
   TLoginDlg dialogPwd( this );

			int iCntTry = 0;
			while( 1 )
				{
			   if( dialogPwd.DoModal() != IDOK ) return;
						if( dialogPwd.csPwd.CompareNoCase(lpPassword_for_createDB) == 0 ) break;
						if( iCntTry >= 5 ) 
							{
							  MessageBox( _T("Access denied"), _T("Information"), MB_OK|MB_ICONINFORMATION );
									return;
							}
						MessageBox( _T("Wrong password. Try again."), 
							           _T("Information"), MB_OK|MB_ICONINFORMATION );
						++iCntTry;
				}
			//################

			CRecreateDlg dlg;
			if (dlg.DoModal() == IDCANCEL)
				return;
			if (dlg.m_strPath[dlg.m_strPath.GetLength()-1] != _T('\\'))
				dlg.m_strPath += _T('\\');


    
	//################
			CString csStartPath;
	  LPTSTR lpBuf = csStartPath.GetBuffer( _MAX_PATH );    
	  ::GetModuleFileName( AfxGetApp()->m_hInstance, lpBuf, _MAX_PATH );
   
   TCHAR drive[ _MAX_DRIVE ];
   TCHAR dir[ _MAX_DIR ];
   TCHAR fname[ _MAX_FNAME ];
   TCHAR ext[ _MAX_EXT ];

			_splitpath( lpBuf, drive, dir, fname, ext );
			_makepath( lpBuf, drive, dir, _T(""), _T("") );	  	 

			csStartPath.ReleaseBuffer();

	  THoldDir hldDir( csStartPath );
	//################

			UpdateData(TRUE);
			HANDLE hFileOut;
			hFileOut = ::CreateFile(
				_T("Guana.sql"),
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFileOut == INVALID_HANDLE_VALUE)
				return;
			DWORD dwBytesWritten;
			char szHead0[] = 
				"USE master\r\n"
				"GO\r\n"
				"if exists (select name from master..sysdatabases where name = \'";
			::WriteFile(hFileOut, szHead0, sizeof(szHead0)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead1[] = 
				"\') DROP DATABASE ";
			::WriteFile(hFileOut, szHead1, sizeof(szHead1)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead2[] = 
				"\r\n"
				"\r\n"
				"if exists (select name from master..sysdevices where name = \'";
			::WriteFile(hFileOut, szHead2, sizeof(szHead2)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead3[] = 
				"_log')\r\n"
				"  EXEC sp_dropdevice ";
			::WriteFile(hFileOut, szHead3, sizeof(szHead3)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead4[] = 
				"_log, DELFILE\r\n"
				"if exists (select name from master..sysdevices where name = \'";
			::WriteFile(hFileOut, szHead4, sizeof(szHead4)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead5[] = 
				"\')\r\n"
				"  EXEC sp_dropdevice ";
			::WriteFile(hFileOut, szHead5, sizeof(szHead5)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead6[] = 
				", DELFILE\r\n"
				"GO\r\n"
				"\r\n"
				"USE master\r\n"
				"GO\r\n"
				"CREATE DATABASE ";
			::WriteFile(hFileOut, szHead6, sizeof(szHead6)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead7[] = 
				"\r\n"
				"	ON (name=\'";
			::WriteFile(hFileOut, szHead7, sizeof(szHead7)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead8[] = 
				"\', filename=\'";
			::WriteFile(hFileOut, szHead8, sizeof(szHead8)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(dlg.m_strPath), dlg.m_strPath.GetLength(), &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHead9[] = 
				".dat\', size=";
			::WriteFile(hFileOut, szHead9, sizeof(szHead9)-1, &dwBytesWritten, NULL);
			CString str;
			str.Format("%u", dlg.m_dwDataSize);
			::WriteFile(hFileOut, LPCSTR(str), str.GetLength(), &dwBytesWritten, NULL);
			char szHeadA[] = 
				"MB)\r\n"
				"		  LOG ON (name=\'";
			::WriteFile(hFileOut, szHeadA, sizeof(szHeadA)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHeadB[] = 
				"_log\', filename=\'";
			::WriteFile(hFileOut, szHeadB, sizeof(szHeadB)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(dlg.m_strPath), dlg.m_strPath.GetLength(), &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHeadC[] = 
				"_log.dat\', size=";
			::WriteFile(hFileOut, szHeadC, sizeof(szHeadC)-1, &dwBytesWritten, NULL);
			str.Format("%u", dlg.m_dwLogSize);
			::WriteFile(hFileOut, LPCSTR(str), str.GetLength(), &dwBytesWritten, NULL);
			char szHeadD[] = 
				"MB)\r\n"
				"GO\r\n"
				"USE ";
			::WriteFile(hFileOut, szHeadD, sizeof(szHeadD)-1, &dwBytesWritten, NULL);
			::WriteFile(hFileOut, LPCSTR(m_cs_DBName), m_cs_DBName.GetLength(), &dwBytesWritten, NULL);
			char szHeadE[] = 
				"\r\n"
				"go\r\n";
			::WriteFile(hFileOut, szHeadE, sizeof(szHeadE)-1, &dwBytesWritten, NULL);


			HANDLE hFileIn;
			hFileIn = ::CreateFile(
				_T("Guana.sql.tail"),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFileIn == INVALID_HANDLE_VALUE) {
				::CloseHandle(hFileOut);
				return;
			}

			char szBuffer[4096];
			DWORD dwBytesReaded;

			::ReadFile(hFileIn, szBuffer, sizeof(szBuffer), &dwBytesReaded, NULL);
			while (dwBytesReaded != 0) {
				::WriteFile(hFileOut, szBuffer, dwBytesReaded, &dwBytesWritten, NULL);
				::ReadFile(hFileIn, szBuffer, sizeof(szBuffer), &dwBytesReaded, NULL);
	}

	::CloseHandle(hFileIn);
	::CloseHandle(hFileOut);

	STARTUPINFO si = {
		sizeof(STARTUPINFO),
		NULL,
		NULL,
		NULL,
		0, 0,
		0, 0,
		0, 0,
		0,
		0,
		0,
		0,
		NULL,
		NULL,
		NULL,
		NULL
	};
	TCHAR szCmdLine[256];
	if (m_iSecurity)
		// Normal connection
		wsprintf(szCmdLine, _T("isql.exe -S %s -i Guana.sql -b -n -U %s -P %s"), LPCTSTR(m_csServer), LPCTSTR(m_csUser), LPCTSTR(m_csPWD));
	else
		// Trusted connection
		wsprintf(szCmdLine, _T("isql.exe -S %s -i Guana.sql -b -n -E"), LPCTSTR(m_csServer));
	PROCESS_INFORMATION pi;
	if (!::CreateProcess(
		NULL, //_T("isql.exe"), 
		szCmdLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)) {
		::MessageBox(NULL, "Cann't start Isql.exe", "Error!", MB_OK | MB_ICONERROR);
		return;
	}
	DWORD dwError = ::GetLastError();
	while (::WaitForSingleObject(pi.hProcess, 500) == WAIT_TIMEOUT);
	DWORD dwExitCode;
	::GetExitCodeProcess(pi.hProcess, &dwExitCode);
	if (dwExitCode == 1)
		::MessageBox(NULL, "Error executing Isql.exe", "Error!", MB_OK | MB_ICONERROR);
}
