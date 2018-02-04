#include "stdafx.h"

void TFieldsMgr::LoadFromControl( bool bOne, int id )
	{   
	  TVEC_FieldBindAP::iterator it( m_vec.begin() );
			TVEC_FieldBindAP::iterator it2( m_vec.end() );
			for( ; it != it2; ++it )
				 if( bOne == false || (bOne == true && id == (*it)->m_iIDCtrl) )
						 (*it)->LoadFromControl();
	}
void TFieldsMgr::LoadToControl( bool bOne, int id )
	{
	  TVEC_FieldBindAP::iterator it( m_vec.begin() );
			TVEC_FieldBindAP::iterator it2( m_vec.end() );
			for( ; it != it2; ++it )
				 if( bOne == false || (bOne == true && id == (*it)->m_iIDCtrl) )
						 (*it)->LoadToControl();
	}

void TFieldsMgr::Fetch( bool bOne, int id )
	{
	  TVEC_FieldBindAP::iterator it( m_vec.begin() );
			TVEC_FieldBindAP::iterator it2( m_vec.end() );
			for( ; it != it2; ++it )
				 if( bOne == false || (bOne == true && id == (*it)->m_iIDCtrl) )
						 (*it)->Fetch();
	}
int TFieldsMgr::Store( bool bNoCheck, bool bOne, int id )
	{
	  TVEC_FieldBindAP::iterator it( m_vec.begin() );
			TVEC_FieldBindAP::iterator it2( m_vec.end() );
			int iCntStored = 0;
			for( ; it != it2; ++it )
				 if( bOne == false || (bOne == true && id == (*it)->m_iIDCtrl) )
						{
						  if( (*it)->Store(bNoCheck) )  ++iCntStored;
						}

			return  iCntStored;
	}
void TFieldsMgr::Clear( bool bOne, int id )
	{
	  TVEC_FieldBindAP::iterator it( m_vec.begin() );
			TVEC_FieldBindAP::iterator it2( m_vec.end() );
			for( ; it != it2; ++it )
				 if( bOne == false || (bOne == true && id == (*it)->m_iIDCtrl) )
						 (*it)->Clear();
	}


void TFieldBind_Int::LoadFromControl()
	{
	  CString str;
	  m_ed->GetWindowText( str );
			str.TrimRight();

			if( str.IsEmpty() ) 
				{
				  m_bChanged = m_bNull ? false:true; 
				  m_bNull = true; return;
				}

			LPTSTR lp = NULL;
			unsigned long ulRes = strtoul( (LPCTSTR)str, &lp, 10 );
			if( lp - (LPCTSTR)str != str.GetLength() ) 
				{
				  m_ed->SetFocus();
						CString s2; s2.Format( _T("Bad value in field: %s"), (LPCTSTR)m_csFieldName );
				  throw exception( (LPCTSTR)s2 );
				}

			m_bChanged = m_bNull || ulRes != m_iValue ? true:false;
			m_iValue = ulRes; m_bNull = false;
	}
void TFieldBind_Int::LoadToControl()
	{
	  CString str;
			if( !m_bNull ) str.Format( _T("%u"), m_iValue );
			m_ed->SetWindowText( str );
	}
void TFieldBind_Int::Fetch()
	{
	  m_bChanged = false;

   if( m_pRec->BOF == VARIANT_TRUE || m_pRec->EndOfFile == VARIANT_TRUE )
				 Clear();
			else
				{
						_variant_t vvt( m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value );
						if( vvt.vt == VT_NULL ) m_bNull = true;
						else m_bNull = false, m_iValue = (long)vvt; 
				}
	}
bool TFieldBind_Int::Store( bool bNoCheck )
	{
   if( m_bRdOnly ) return false;

	  if( bNoCheck || m_bChanged )
				{
						try {
							  m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value = _variant_t( (long)m_iValue );
							}
						catch( _com_error& e )
							{
							  m_ed->SetFocus();
									throw e;
							}
      
				  m_bChanged = false;
						return true;
				}
			return false;
	}
void TFieldBind_Int::Clear()
	{
	  m_bNull = true;
	}



void TFieldBind_Str::LoadFromControl()
	{
	  CString str;
	  m_ed->GetWindowText( str );
			str.TrimRight();

			if( str.IsEmpty() ) 
				{
				  m_bChanged = m_bNull ? false:true; 
				  m_bNull = true; return;
				}

			m_bChanged = m_bNull || m_sValue != str ? true:false;
			m_sValue = str; m_bNull = false;
	}
void TFieldBind_Str::LoadToControl()
	{	  
			if( !m_bNull ) m_ed->SetWindowText( m_sValue );
			else m_ed->SetWindowText( _T("") );
	}
void TFieldBind_Str::Fetch()
	{
	  m_bChanged = false;

			if( m_pRec->BOF == VARIANT_TRUE || m_pRec->EndOfFile == VARIANT_TRUE )
				 Clear();
			else
				{
						_variant_t vvt( m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value );
						if( vvt.vt == VT_NULL ) m_bNull = true;
						else m_bNull = false, m_sValue = (LPCTSTR)(_bstr_t)vvt; 
				}
	}
bool TFieldBind_Str::Store( bool bNoCheck )
	{
   if( m_bRdOnly ) return false;

	  if( bNoCheck || m_bChanged )
				{
						try {
							  m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value = _variant_t( (LPCTSTR)m_sValue );
							}
						catch( _com_error& e )
							{
							  m_ed->SetFocus();
									throw e;
							}
      
				  m_bChanged = false;
						return true;
				}
			return false;
	}
void TFieldBind_Str::Clear()
	{
	  m_bNull = true, m_sValue.Empty();
	}



void TFieldBind_Money::LoadFromControl()
	{
	  COleCurrency m_currTmp;
			CString str;
	  m_ed->GetWindowText( str );
			str.TrimRight();

			if( str.IsEmpty() ) { Clear(); return; }
						
			if( !m_currTmp.ParseCurrency(str) )
				{
						m_ed->SetFocus();
						CString s2; s2.Format( _T("Bad value in field: %s"), (LPCTSTR)m_csFieldName );
						throw exception( (LPCTSTR)s2 );
				}

			m_bNull = false;						
			m_bChanged = m_currTmp != m_currValue ? true:false;
			m_currValue = m_currTmp;
	}
void TFieldBind_Money::LoadToControl()
	{	  
	  CString str;
	  if( !m_bNull ) str = m_currValue.Format();

			m_ed->SetWindowText( str );
	}
void TFieldBind_Money::Fetch()
	{   
	  m_bChanged = false;

			if( m_pRec->BOF == VARIANT_TRUE || m_pRec->EndOfFile == VARIANT_TRUE )
				 Clear();
			else
				{
						_variant_t vvt( m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value );
						if( vvt.vt == VT_NULL ) m_bNull = true, m_currValue.SetCurrency( 0, 0 );
						else m_bNull = false, m_currValue = vvt; 
				}
	}
bool TFieldBind_Money::Store( bool bNoCheck )
	{
   if( m_bRdOnly ) return false;

	  if( bNoCheck || m_bChanged )
				{
						try {
							  m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value = 
										  (CURRENCY)m_currValue;
							}
						catch( _com_error& e )
							{
							  m_ed->SetFocus();
									throw e;
							}
      
				  m_bChanged = false;
						return true;
				}
			return false;
	}
void TFieldBind_Money::Clear()
	{
	  m_currValue.SetCurrency( 0, 0 );
			m_bNull = true;
	}



void TFieldBind_Date::LoadFromControl()
	{
   if( m_bRdOnly ) return;

	  COleDateTime m_dateTmp;
	  m_date->GetTime( m_dateTmp );

			m_dateTmp.SetDateTime( m_dateTmp.GetYear(), 
				 m_dateTmp.GetMonth(), m_dateTmp.GetDay(), 0, 0, 0 );

			m_bNull = false;			

			m_bChanged = m_odtValue != m_dateTmp ? true:false;
			m_odtValue = m_dateTmp;
	}
void TFieldBind_Date::LoadToControl()
	{	  
			m_date->SetTime( m_odtValue );
	}
void TFieldBind_Date::Fetch()
	{
	  m_bChanged = false;

			if( m_pRec->BOF == VARIANT_TRUE || m_pRec->EndOfFile == VARIANT_TRUE )
				 Clear();
			else
				{
						_variant_t vvt( m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value );
						m_odtValue = vvt, m_bNull = false;		
				}
	}
bool TFieldBind_Date::Store( bool bNoCheck )
	{
   if( m_bRdOnly ) return false;

	  if( bNoCheck || m_bChanged )
				{
						try {
							  m_pRec->Fields->GetItem((LPCTSTR)m_csFieldName)->Value = 
										_variant_t( (DATE)m_odtValue );
							}
						catch( _com_error& e )
							{
							  m_date->SetFocus();
									throw e;
							}
      
				  m_bChanged = false;
						return true;
				}
			return false;
	}
void TFieldBind_Date::Clear()
	{
	  m_odtValue = COleDateTime::GetCurrentTime();
	}
