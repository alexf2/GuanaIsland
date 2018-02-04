#include "stdafx.h"

#include <atlbase.h>



void MyCOMChk( HRESULT hr )
 {
   if( FAILED(hr) ) 
	{
	  IErrorInfo *pE; 
	  if( SUCCEEDED(GetErrorInfo(0, &pE)) )
	    _com_raise_error( hr, pE );
	  else _com_raise_error( hr );
	}
 }

void ShowErr( CWnd *pW, _com_error &e )
 {
   CString str;
   _bstr_t bSrc( e.Source() ), bDescr( e.Description() );

   str.Format( _T("ErrMess: [%s]\nErrSrc: [%s]\nErrDescr: [%s]"), 
			e.ErrorMessage(), 
			bSrc.length() ? (LPCTSTR)bSrc:_T("null"),
			bDescr.length() ? (LPCTSTR)bDescr:_T("null") );

					CString str2;
					IErrorInfo *pIE = e.ErrorInfo();
					if( pIE )
						{
								pIE->Release();
								CComPtr<IErrorInfo> cpIE( pIE ); 
								/*CComPtr<IMyErrorInfo> cpMy;
								HRESULT hr = cpIE.QueryInterface( &cpMy );
								if( SUCCEEDED(hr) && cpMy->HaveExtra() == S_OK )
									{
											TVexExtraData dta;
											memset( &dta, 0, sizeof(TVexExtraData) );
											cpMy->GetExtInfo( &dta );

											USES_CONVERSION;
											
														str2.Format( "\nVEX=%d; SubCode=%d; Name1=%s; Name2=%s", 
													dta.m_vex, dta.m_iSubCode,
													dta.m_bsN1 ? OLE2CT(dta.m_bsN1):"Null",
													dta.m_bsN2 ? OLE2CT(dta.m_bsN2):"Null" );

											::SysFreeString( dta.m_bsN1 );
											::SysFreeString( dta.m_bsN2 );
									}*/
						}

					::MessageBox( pW ? pW->m_hWnd:NULL, (LPCTSTR)(str+str2), "Error", MB_OK|MB_ICONSTOP );
 }
