//---------------------------------------------------------------------------
#ifndef main_dmodH
#define main_dmodH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TMainDmod : public TDataModule
{
__published:	// IDE-managed Components
    TDatabase *DB1;
private:	// User declarations
public:		// User declarations
    __fastcall TMainDmod(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainDmod *MainDmod;
//---------------------------------------------------------------------------
#endif
