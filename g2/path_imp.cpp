//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "path_imp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFPathImp *FPathImp;
//---------------------------------------------------------------------------
__fastcall TFPathImp::TFPathImp(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFPathImp::BitBtn1Click( TObject *Sender )
{
  HANDLE hf = ::CreateFile( Edit1->Text.c_str(), GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
  if( hf ==  INVALID_HANDLE_VALUE )
   {
     AnsiString str = Format( "File '%s' not found", ARRAYOFCONST((Edit1->Text.c_str())) );
     Application->MessageBox( str.c_str(), "Error", MB_OK + MB_ICONSTOP );
     return;
   }
  else ::CloseHandle( hf );
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

