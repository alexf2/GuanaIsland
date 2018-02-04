//---------------------------------------------------------------------------
#ifndef SElDateH
#define SElDateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ToolEdit.hpp"
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFSelDate : public TForm
{
__published:	// IDE-managed Components
    TDateEdit *DateEdit1;
    void __fastcall DateEdit1AcceptDate(TObject *Sender, TDateTime &ADate,
          bool &Action);
    void __fastcall DateEdit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFSelDate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFSelDate *FSelDate;
//---------------------------------------------------------------------------
#endif
