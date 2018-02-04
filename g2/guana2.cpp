//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("guana2.res");
USEFORM("guana2_mf.cpp", Form1);
USEFORM("path_imp.cpp", FPathImp);
USEFORM("dm1.cpp", DMod1); /* TDataModule: DesignClass */
USEFORM("dm2.cpp", DMod2); /* TDataModule: DesignClass */
USEFORM("nform.cpp", NavForm);
USEFORM("main_dmod.cpp", MainDmod); /* TDataModule: DesignClass */
USEFORM("helper.cpp", FHelper);
USEFORM("rpt_ord.cpp", QuickReport_Ordinal); /* TQuickRep: DesignClass */
USEFORM("rpt_sum.cpp", QuickReport_Sum); /* TQuickRep: DesignClass */
USEUNIT("myexc.cpp");
USEFORM("SElDate.cpp", FSelDate);
//---------------------------------------------------------------------------
#include <dbtables.hpp>
#include <memory>
using namespace std;
#include "path_imp.h"

void CheckVersion()
 {
   bool bFl = false;
   try {
     auto_ptr<TDatabase> apBase( new TDatabase(NULL) );
     apBase->AliasName = "GUANA2";
     apBase->DatabaseName = "DB01";
     apBase->KeepConnection = true;
     apBase->LoginPrompt = false;
     apBase->Name = "DB01";
     apBase->Connected = true;

     auto_ptr<TQuery> apQ( new TQuery(NULL) );
     apQ->DatabaseName = apBase.get()->DatabaseName;
     apQ->SQL->Add( "select count(*) as VCNT from MyVersion where MYVERSION=106" );

     apQ->Open();
     bFl = apQ->FieldByName("VCNT")->AsInteger == 1 ? true:false;
     apQ->Close();

     apBase->Connected = false;;
   }
   catch( Exception *e )
    {
      throw Exception( Format("Cann't check version of GUANA.MDB. You have old version or incorrect DSN installed.\nThe error: [%s]",
       ARRAYOFCONST((e->Message.c_str())) ) );
    }
   if( !bFl ) throw Exception( "You have incorrect version GUANA.MDB. Required version is V1.06" );
 }

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->HintPause = 1000;

        /*if( Session->IsAlias("GUANA2") == false )
         {
           auto_ptr<TFPathImp> app( (FPathImp = new TFPathImp(NULL)) );

           if( FPathImp->ShowModal() != IDOK ) return 0;

           auto_ptr<TStringList> ap( new TStringList() );
           try {
              ap->Add( Format("DATABASE NAME=%s", ARRAYOFCONST(( FPathImp->Edit1->Text.c_str()  ))) );
              ap->Add("USER NAME=");
              Session->AddAlias( "GUANA2", "MSACCESS", ap.get() );
              Session->SaveConfigFile();
            }
            catch( Exception &e )
             {
               MessageBox( NULL, Format("Cann't connect to DB: [%s]", ARRAYOFCONST((e.Message.c_str()))).c_str(), "Error", MB_OK|MB_ICONSTOP );
               return 0;
             }
         }*/

        CheckVersion();

        Application->CreateForm(__classid(TForm1), &Form1);
        Application->CreateForm(__classid(TFPathImp), &FPathImp);
        Application->CreateForm(__classid(TMainDmod), &MainDmod);
        Application->CreateForm(__classid(TDMod2), &DMod2);
        Application->CreateForm(__classid(TDMod1), &DMod1);
        Application->CreateForm(__classid(TQuickReport_Ordinal), &QuickReport_Ordinal);
        Application->CreateForm(__classid(TQuickReport_Sum), &QuickReport_Sum);
        Application->CreateForm(__classid(TFSelDate), &FSelDate);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
