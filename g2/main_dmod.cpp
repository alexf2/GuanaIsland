//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main_dmod.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainDmod *MainDmod = NULL;
//---------------------------------------------------------------------------
__fastcall TMainDmod::TMainDmod(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
