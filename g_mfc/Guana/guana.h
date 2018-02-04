// guana.h : main header file for the GUANA application
//

#if !defined(AFX_GUANA_H__ECC8011A_D845_11D2_AD75_00504E02C39D__INCLUDED_)
#define AFX_GUANA_H__ECC8011A_D845_11D2_AD75_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "..\..\..\SEAGATE SOFTWARE\CRYSTAL REPORTS\peplus.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CGuanaApp:
// See guana.cpp for the implementation of this class
//

extern TSpashDlg dlSplash;

class CGuanaApp : public CWinApp
 {
public:
	 const CRPELogOnInfo* GetLogOnInfo() const;

	 CGuanaApp();
		virtual ~CGuanaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuanaApp)
public:
	  virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

   virtual int ExitInstance();
			virtual void WinHelp( DWORD dwData, UINT nCmd = HELP_CONTEXT );

	  auto_ptr<TMyProperty> m_apMainDlg;

			CString m_csPWD;

// Implementation

	//{{AFX_MSG(CGuanaApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CRPELogOnInfo m_LogOnInfo;
	CRPEngine m_CREngine;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUANA_H__ECC8011A_D845_11D2_AD75_00504E02C39D__INCLUDED_)
