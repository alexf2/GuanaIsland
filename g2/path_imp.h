//---------------------------------------------------------------------------
#ifndef path_impH
#define path_impH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFPathImp : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TBitBtn *BitBtn1;
    TEdit *Edit1;
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFPathImp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFPathImp *FPathImp;
//---------------------------------------------------------------------------
#endif
