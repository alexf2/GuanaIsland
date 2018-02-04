#include "stdafx.h"

void TMyEdit::OnSetFocus( CWnd* pOldWnd )
	{
	  CWnd::OnSetFocus( pOldWnd );
			if( GetParent() )
	    reinterpret_cast<TMyProperty*>(AfxGetApp()->m_pMainWnd)->CallNavAss( this );
	}

BEGIN_MESSAGE_MAP( TMyEdit, CEdit )
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void TMyDateTime::OnSetFocus( CWnd* pOldWnd )
	{
	  CWnd::OnSetFocus( pOldWnd );
			if( GetParent() )
	    reinterpret_cast<TMyProperty*>(AfxGetApp()->m_pMainWnd)->CallNavAss( this );
	}

BEGIN_MESSAGE_MAP( TMyDateTime, CDateTimeCtrl )
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

