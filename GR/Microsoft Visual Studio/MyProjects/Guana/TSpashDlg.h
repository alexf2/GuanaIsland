#if !defined(AFX_TSPASHDLG_H__8BBB99A7_E670_11D2_AD9D_00504E02C39D__INCLUDED_)
#define AFX_TSPASHDLG_H__8BBB99A7_E670_11D2_AD9D_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TSpashDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TSpashDlg dialog


class TSpashDlg : public CDialog
{
// Construction
public:
	TSpashDlg(CWnd* pParent = NULL);   // standard constructor

	CProgressCtrl m_pr;	
	CMutex m_mtx, m_mtx2, m_mtx3;
	CEvent m_evSta;
	bool m_bChange;
	long m_bCancel, m_bInit;		
	CString m_csTtl;

	bool NotCancel()
		{
		  CSingleLock lock( &m_mtx3, TRUE );		  
		  return !m_bCancel;
		}
	bool IsInitialized()
		{
		  CSingleLock lock( &m_mtx2, TRUE );		  
		  return m_bInit;
		}
	void SetCancel()
		{
		  CSingleLock lock( &m_mtx3, TRUE );		  
				m_bCancel = true;		  
		}
	void SetTttl( LPCTSTR lpS )
		{
		  //while( !IsInitialized() );
    CSingleLock lock1( &m_evSta );
				lock1.Lock();

		  CSingleLock lock( &m_mtx, TRUE );
				m_bChange = true;
				m_csTtl = lpS;				
		}
	void SetInit()
		{
		  CSingleLock lock( &m_mtx2, TRUE );		  
		  m_bInit = true;
				m_evSta.SetEvent();
		}

// Dialog Data
	//{{AFX_DATA(TSpashDlg)
	enum { IDD = IDD_TSPASHDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TSpashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:  
	// Generated message map functions
	//{{AFX_MSG(TSpashDlg)
	virtual BOOL OnInitDialog();	
	//}}AFX_MSG

 virtual void OnCancel();
	virtual void OnOk();

	DECLARE_MESSAGE_MAP()
};

bool StartSplash( TSpashDlg* rpD );
UINT WorkFunction( LPVOID );

class TMyThrd: public CWinThread
 {

DECLARE_DYNCREATE( TMyThrd )

public: 
	 TMyThrd(): CWinThread()
			{
			  m_pDlg = NULL, iRng = 0;
			}
  TMyThrd( AFX_THREADPROC pfnThreadProc, LPVOID pParam, TSpashDlg* pDlg ):
	   CWinThread( pfnThreadProc, pParam ),
				m_pDlg( pDlg ), iRng( 0 )
	  {	 
	  }

  virtual ~TMyThrd();  
  virtual BOOL OnIdle( LONG lCount );
		virtual BOOL InitInstance();

protected:  
	  TSpashDlg* m_pDlg;
			int iRng;
 };


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSPASHDLG_H__8BBB99A7_E670_11D2_AD9D_00504E02C39D__INCLUDED_)
