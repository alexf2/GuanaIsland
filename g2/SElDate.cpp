//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SElDate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ToolEdit"
#pragma resource "*.dfm"
TFSelDate *FSelDate = NULL;
//---------------------------------------------------------------------------
__fastcall TFSelDate::TFSelDate(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFSelDate::DateEdit1AcceptDate(TObject *Sender,
      TDateTime &ADate, bool &Action)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFSelDate::DateEdit1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if( Key == VK_RETURN ) ModalResult = mrOk;
  else if( Key == VK_ESCAPE ) ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFSelDate::FormShow(TObject *Sender)
{
  DateEdit1->DoClick();
}
//---------------------------------------------------------------------------
void __fastcall TFSelDate::FormResize(TObject *Sender)
{
  SetWindowPos( DateEdit1->Handle, NULL, 0, 0, ClientWidth, ClientHeight,
                SWP_NOZORDER|SWP_NOACTIVATE );
}
//---------------------------------------------------------------------------
