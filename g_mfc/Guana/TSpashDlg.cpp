// TSpashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guana.h"
#include "TSpashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TSpashDlg dialog

IMPLEMENT_DYNCREATE( TMyThrd, CWinThread )


BOOL TMyThrd::OnIdle( LONG lCount )
	{
	  CWinThread::OnIdle( lCount );

			try {										  						
						if( m_pDlg->NotCancel() )
							{
									Sleep( 200 );

									CSingleLock lock( &m_pDlg->m_mtx, TRUE );
         if( m_pDlg->m_bChange )
										 m_pDlg->SetDlgItemText( IDC_STATIC_TTL, m_pDlg->m_csTtl ),
				       m_pDlg->m_pr.SetPos( 0 ),
											m_pDlg->m_bChange = false;

									lock.Unlock();
									//pD->UpdateWindow();

									m_pDlg->m_pr.SetPos( m_pDlg->m_pr.GetPos() >= iRng ? 0:m_pDlg->m_pr.GetPos() + 10 );									
							}
						else
							{
						   m_pDlg->EndDialog( IDOK ),
						   m_pDlg->DestroyWindow();
									PostQuitMessage( 0 );

									return FALSE;
							}
				}
			catch( _com_error &e )
				{				  
						ShowErr( NULL, e );
				}		
			catch( CException *pE )
				{
				  pE->ReportError();
				  pE->Delete();
				}
			if( !m_pDlg->IsInitialized() ) m_pDlg->SetInit();

			return TRUE;
	}
TMyThrd::~TMyThrd()
	{
	}
BOOL TMyThrd::InitInstance()
	{
	  CWinThread::InitInstance();

	  m_pDlg->Create( IDD_TSPASHDLG_DIALOG );
			m_pDlg->ShowWindow( SW_SHOW );
			

			CRect r; m_pDlg->m_pr.GetClientRect( &r );
			iRng = r.Width();
			m_pDlg->m_pr.SetRange( 0, iRng );
			m_pDlg->m_pr.SetStep( 10 );

			m_pDlg->SetInit();

			return TRUE;
	}

/*struct TThrdDta
	{
	  CWinThread *p;
	};

UINT WorkFunction( LPVOID pp  )
	{	  
   ((TThrdDta*)pp)->p->Run();	  	  
			delete (TThrdDta*)pp;
			return 0;
	}*/

bool StartSplash( TSpashDlg* rpD )
	{
	  //TThrdDta* dta = new TThrdDta();			
			CWinThread *p = new TMyThrd( NULL, 0, rpD );			
			//dta->p = p;
			return p->CreateThread();
	}



TSpashDlg::TSpashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TSpashDlg::IDD, pParent), 
	m_mtx( FALSE ), m_bCancel( 0 ), m_bInit( 0 ),
	m_mtx2( FALSE ), m_mtx3( FALSE ),
	m_bChange( false ), m_evSta( FALSE, TRUE )
{
	//{{AFX_DATA_INIT(TSpashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TSpashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TSpashDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	 DDX_Control( pDX, IDC_PROGRESS1, m_pr );
}


BEGIN_MESSAGE_MAP(TSpashDlg, CDialog)
	//{{AFX_MSG_MAP(TSpashDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TSpashDlg message handlers

BOOL TSpashDlg::OnInitDialog() 
 {
			CDialog::OnInitDialog();
			
			CFont *pSysFnt = CFont::FromHandle( (HFONT)GetStockObject(OEM_FIXED_FONT) );	
			GetDlgItem( IDC_STATIC_TTL )->SetFont( pSysFnt );

			//if( bShow )
			SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE ),
			SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE );					

			return TRUE;  // return TRUE unless you set the focus to a control
																	// EXCEPTION: OCX Property Pages should return FALSE
 }


void TSpashDlg::OnCancel()
	{
	}
void TSpashDlg::OnOk()
	{
	}
