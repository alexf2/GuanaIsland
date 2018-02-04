//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "nform.h"
#include "myexc.h"
#include "dm1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TNavForm *NavForm;

void __fastcall TNavForm::Loaded(void)
{
  TForm::Loaded();
  Visible = False;
  //Position = poDefault;
  //BorderIcons = TBorderIcons();
  //BorderStyle = bsNone;
  HandleNeeded();
  //SetBounds(0, 0, Width, Height);
}

void __fastcall TNavForm::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams( Params );
  if (dynamic_cast<TForm*>(Owner))
    Params.WndParent = dynamic_cast<TForm*>(Owner)->Handle;
  else
    ShowMessage("No Cast in ChildForm CreateParams");
  Params.Style &= ~WS_CHILD;// | WS_CLIPSIBLINGS;
  //Params.X = 0;
  //Params.Y = 0;
}


//---------------------------------------------------------------------------
__fastcall TNavForm::TNavForm(TComponent* Owner)
    : TForm(Owner)
{
  m_pGrid = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TNavForm::DBNavigator1BeforeAction(TObject *Sender,
      TNavigateBtn Button)
{
  TQuery *pQ;
  if( Button == nbRefresh && (pQ=dynamic_cast<TQuery*>(DBNavigator1->DataSource->DataSet)) )
   {
     MyRefr( pQ );
     throw TNothingException();
   }
}
//---------------------------------------------------------------------------

