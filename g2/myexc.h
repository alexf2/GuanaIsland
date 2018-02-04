//---------------------------------------------------------------------------
#ifndef myexcH
#define myexcH

class TNothingException: public Exception
 {
public:
  __fastcall TNothingException(): Exception( "NOTHING" )
   {
   }
  __fastcall virtual ~TNothingException(void) { }
 };
//---------------------------------------------------------------------------
#endif
