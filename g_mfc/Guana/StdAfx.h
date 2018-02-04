// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__ECC8011E_D845_11D2_AD75_00504E02C39D__INCLUDED_)
#define AFX_STDAFX_H__ECC8011E_D845_11D2_AD75_00504E02C39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdtctl.h>
#include <afxpriv.h>

#include "memory"
#include "list"
#include "vector"
#include "map"
#include "set"
using namespace std;


#define INITGUID
// AS: Alex, don't use absolute paths, pls
#import "e:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")
#include <stdio.h>
#include "icrsint.h"


#include "OX\\OXBitmapButton.h"
#include "OX\\OXBitmapButtonres.h"
#include "OX\\OXDIB.h"
#include "OX\\OXStaticText.h"

#include "ADO_DSRC\\adodc.h"
#include "ADO_GRID\\datagrid.h"
#include "ADO_GRID\\font1.h"
#include "ADO_GRID\\columns.h"
#include "ADO_GRID\\column.h"
#include "ADO_DSRC\\_recordset.h"
#include "MASK_EDIT\\msmask.h"
#include "PICKER\\dtpicker.h"
#include "DATA_LIST\\datalist.h"
#include "DATA_COMBO\\datacombo.h"


#include "resource.h"

#include "com_chk.h"
#include "my_dbnav.h"
#include "lookup.h"
#include "my_edit.h"
#include "fld_binder.h"



#pragma pack( push, 1 )

class TSD
	{
public:
	  TSD(): m_bOpened( false )
				{
				}
			TSD( const TSD& rTDS )
				{
				  this->operator=( rTDS );
				}
			TSD& operator=( const TSD& rTDS )
				{
				  m_pRec = rTDS.m_pRec;
						m_pCmd = rTDS.m_pCmd;
						m_bOpened = rTDS.m_bOpened;
						return *this;
				}
			~TSD()
				{
				  if( m_bOpened )
							{
							  m_bOpened = false;
									try {
							    m_pRec->Close();
										}
									catch( _com_error &e )
										{
										}
							}
				}

			_RecordsetPtr m_pRec;
			_CommandPtr m_pCmd;
			bool m_bOpened;
	};

struct TSubclassCtrls
	{
	  UINT id;
			CWnd *pWnd;
	};
void MakeSubcass( TSubclassCtrls* pDta, CDialog *pDlg, CDataExchange* pDX );

struct TLds
	{
	  CDataGrid *pGr;
			long id;
	};
typedef auto_ptr<COXStaticText> TAP_COXText;
void SetupTextProp( TLds*, CWnd*, list<TAP_COXText>& );


struct TBtnInf
	{
	  UINT id;
			LPCTSTR lp;
	};
typedef auto_ptr<COXBitmapButton> TAP_COXBtn;
void MakeDefCOXBtns( TBtnInf*, CWnd*, list<TAP_COXBtn>& );

struct TClmDef
	{
	  LPCTSTR    m_sName, m_sDispl;
			long       m_lAln;
			BYTE       m_bF; //001b - button, 010b - lock, 011b - button+lock
			BYTE       m_bWidth;
			LPCTSTR    m_lpFormat;
	};

struct TMaskEdInint
	{
	  CMSMask  *m_pMask;
			CAdodc   *m_pADC;
			VARTYPE  m_vt;
			LPCTSTR  m_lpName;
	};



void SetupGridClms( CDataGrid&, TClmDef* );

struct TClmDef2
	{
	  CString    m_sName, m_sDispl;
			long       m_lAln;

			TClmDef2()
				{
				}
			TClmDef2( const TClmDef2& rCD )
				{
				  this->operator=( rCD );
				}			
			TClmDef2& operator=( const TClmDef2& rCD )
				{
				  m_sName = rCD.m_sName, m_sDispl = rCD.m_sDispl;
						m_lAln = rCD.m_lAln;
				  return *this;
				}

			void Ass( CString &sName, CString &sDispl, long  lAln )
				{
				  m_sName = sName, m_sDispl = sDispl;
						m_lAln = lAln;
				}
	};

typedef vector<TClmDef2> TLST_ClmDef2;

void KeepGridClms( CDataGrid& rGrid, TLST_ClmDef2& rLst );
void RestoreGridClms( CDataGrid& rGrid, TLST_ClmDef2& rLst );

struct TBindRecs
	{
	  CAdodc          *m_pDC;
			CDataGrid       *m_pGr;
			LPCTSTR         m_sConn;
			CommandTypeEnum m_iCmdTyp;

			TClmDef         m_cd[20];
	};

struct TGridInitData
	{
	  CDataGrid *m_pGr;
			COLORREF m_clr;
	};

struct  TToolTipData
 {
   TToolTipData() { m_uiId = 0; }
   TToolTipData( UINT id, LPCSTR lp ): m_csTxt( lp )
				{
						m_uiId = id;
				}

   UINT m_uiId;
   CString m_csTxt;
 };

#pragma pack( pop )

int FindToolTip( TToolTipData*, HWND, CPoint point, TOOLINFO* pTI );

inline long ChkZero_LONG( _variant_t vt )
	{
	  return vt.vt == VT_NULL ? 0L:(long)vt;
	}
inline _variant_t ChkZero_LONG_V( _variant_t vt )
	{
	  return vt.vt == VT_NULL ? _variant_t(0L):vt;
	}

class THoldDir
	{
public:
	  THoldDir( LPCTSTR lpNew )
				{				  
				  try {
								LPTSTR lp = m_csOldPtch.GetBuffer( _MAX_PATH + 1 );
								*lp = 0;
								DWORD dw = ::GetCurrentDirectory( _MAX_PATH, lp );
								m_csOldPtch.ReleaseBuffer();

								if( dw != 0 )
								  dw = ::SetCurrentDirectory( lpNew ),
								  m_bOK = true;
							}
						catch( CException *pE )
							{
							  m_bOK = false;
							  pE->Delete();
							}
				}
			~THoldDir()
				{
				  if( m_bOK ) ::SetCurrentDirectory( (LPCTSTR)m_csOldPtch );
				}
protected:
	 CString m_csOldPtch;
		bool m_bOK;
	};

class TZoomer
	{
public:
	 TZoomer():
					m_pGrid( NULL ),	m_hwAfter( (HWND)0 ), 
					x( 0 ), y( 0 ), cx( 0 ), cy( 0 ), m_bFl( false )
			{
			}

		void Zoom( CDataGrid *pGrid, CRect& rectNew );
		void Restore();

		bool IsZoomed() const
			{
			  return m_bFl;
			}  

protected:	 
	 CDataGrid *m_pGrid;
		HWND m_hwAfter;
		int x, y, cx, cy;
		bool m_bFl;
	};

struct TCtxBnd
	{   
   TCtxBnd()
				{
				}
			TCtxBnd( int ctx_, UINT uCmd_ )
				{
				  ctx = ctx_, uCmd = uCmd_; 
				}

	  int ctx;
			UINT uCmd;
	};

int GetProfileValueInt( LPCTSTR lpszSection, LPCTSTR lpszItem, int iDef = 0 );
float GetProfileValueFloat( LPCTSTR lpszSection, LPCTSTR lpszItem, float iDef = -1 );
CString GetProfileValueStr( LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpDef = _T("") );

void SetProfileValueInt( LPCTSTR lpszSection, LPCTSTR lpszItem, int iVal );
void SetProfileValueFloat( LPCTSTR lpszSection, LPCTSTR lpszItem, float fVal );
void SetProfileValueStr( LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpVal );

void MakeUpdate( CDataGrid *pG, CWnd *pW );

void CancelDSrec( CDataGrid* m_pWGrid, CAdodc *m_pWAdc );

extern LPCTSTR _TM_FDATE_;
extern LPCTSTR _TM_MONEY_;
extern LPCTSTR arrST[];

#include "dlg_many_to_many.h"
#include "dlg_comm.h"
#include "login.h"
#include "TSearchGuest.h"
#include "TSpashDlg.h"
#include "TDeposit_Dlg.h"
#include "TGuestAdd.h"
#include "TBillAdd.h"

#include "cnct_dlg.h"
#include "prop_tech.h"
#include "prop_bill.h"
#include "prop_time.h"
#include "guanaDlg.h"




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ECC8011E_D845_11D2_AD75_00504E02C39D__INCLUDED_)
