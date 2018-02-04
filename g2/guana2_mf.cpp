//---------------------------------------------------------------------------
#include <vcl.h>
#include <SysUtils.hpp>
#pragma hdrstop

#include "guana2_mf.h"
#include "dm1.h"
#include "dm2.h"
#include "nform.h"
#include "main_dmod.h"
#include "helper.h"
#include "rpt_sum.h"
#include "myexc.h"
#include "seldate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "RxGrdCpt"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma link "RXCtrls"
#pragma link "RxLookup"
#pragma link "ToolEdit"
#pragma link "CurrEdit"
#pragma link "RxDBComb"
#pragma resource "*.dfm"
TForm1 *Form1;

void __fastcall TForm1::AppException( TObject *Sender, Sysutils::Exception *E )
{
  if( !dynamic_cast<TNothingException*>(E) )
    Application->ShowException( E );
}

void InputDF( TRxDBGrid* pG )
 {
   if( pG->SelectedField->IsNull )
     FSelDate->DateEdit1->Date = TDateTime::CurrentDate();
   else
     FSelDate->DateEdit1->Date = pG->SelectedField->AsDateTime;

   //RECT r;
   POINT ptt[ 2 ] =
    {
      { pG->InplaceEditor->Left, pG->InplaceEditor->Top },
      { pG->InplaceEditor->Left + pG->InplaceEditor->Width, pG->InplaceEditor->Top + pG->InplaceEditor->Height }
    };
   MapWindowPoints( pG->InplaceEditor->Parent ? pG->InplaceEditor->Parent->Handle:0,
     FSelDate->Parent ? FSelDate->Parent->Handle:0, ptt, 2 );

   SetWindowPos( FSelDate->Handle, NULL, ptt[0].x, ptt[0].y,
     pG->InplaceEditor->Width, pG->InplaceEditor->Height, SWP_NOZORDER );

   if( FSelDate->ShowModal() == IDOK )
    {
      if( pG->DataSource->DataSet->State != dsEdit ) pG->DataSource->DataSet->Edit();
      pG->SelectedField->AsDateTime = FSelDate->DateEdit1->Date;
      try {
        if( pG->InplaceEditor->CanFocus() ) pG->InplaceEditor->SetFocus();
       }
      catch( Exception *pE )
       {
       }
    }
 }

void TBndCtrl::CheckState()
 {
   if( !m_pSrc ) return;
   bool bFlag = !(m_pSrc->State == dsInactive || m_pSrc->State == dsInsert ||
     m_pSrc->DataSet->Bof && m_pSrc->DataSet->Eof );
   if( m_bState == bFlag ) return;

   m_bState = bFlag;
   list<TCtrlEntry>::iterator itSta( m_s.begin() );
   list<TCtrlEntry>::iterator itEnd( m_s.end() );
   for( ; itSta != itEnd; ++itSta )
     if( m_bState != (*itSta).p->Enabled )
      {
        if( m_bState == true )
           (*itSta).p->Enabled = true,
            SetClr( (*itSta).p, (*itSta).key );
        else
          (*itSta).p->Enabled = false, (*itSta).key = GetClr( (*itSta).p ),
          SetClr( (*itSta).p, clLtGray );
      }
 }

TColor GetClr( TControl* p )
 {
   TDBGrid *pG = dynamic_cast<TDBGrid*>( p );
   if( pG ) return pG->Color;

   TDBComboBox *pC = dynamic_cast<TDBComboBox*>( p );
   if( pC ) return pC->Color;

   TDBEdit *pE = dynamic_cast<TDBEdit*>( p );
   if( pE ) return pE->Color;

   TRxDBLookupCombo *pL = dynamic_cast<TRxDBLookupCombo*>( p );
   if( pL ) return pL->Color;

   TDBDateEdit *pD = dynamic_cast<TDBDateEdit*>( p );
   if( pD ) return pD->Color;

   TRxDBCalcEdit *pCC = dynamic_cast<TRxDBCalcEdit*>( p );
   if( pCC ) return pCC->Color;

   TDBMemo *pMM = dynamic_cast<TDBMemo*>( p );
   if( pMM ) return pMM->Color;

   return 0;
 }
void SetClr( TControl* p, TColor clr )
 {
   if( clr == 0 ) return;

   TDBGrid *pG = dynamic_cast<TDBGrid*>( p );
   if( pG ) { pG->Color = clr; return; }

   TDBComboBox *pC = dynamic_cast<TDBComboBox*>( p );
   if( pC ) { pC->Color = clr; return; }

   TDBEdit *pE = dynamic_cast<TDBEdit*>( p );
   if( pE ) { pE->Color = clr; return; }

   TRxDBLookupCombo *pL = dynamic_cast<TRxDBLookupCombo*>( p );
   if( pL ) { pL->Color = clr; return; }

   TDBDateEdit *pD = dynamic_cast<TDBDateEdit*>( p );
   if( pD ) { pD->Color = clr; return; }

   TRxDBCalcEdit *pCC = dynamic_cast<TRxDBCalcEdit*>( p );
   if( pCC ) { pCC->Color = clr; return; }

   TDBMemo *pMM = dynamic_cast<TDBMemo*>( p );
   if( pMM ) { pMM->Color = clr; return; }
 }


void AssElipsisProp( TDBGrid* pG, AnsiString& rS )
 {
   for( int i = pG->Columns->Count - 1; i >= 0; --i )
    { //AnsiString pp = pG->Columns->Items[i]->FieldName;
      if( pG->Columns->Items[i]->FieldName == rS )
       {
         pG->Columns->Items[i]->ButtonStyle = cbsEllipsis;
         return;
       }
    }
 }

void AssRdProp( TDBGrid* pG, AnsiString& rS, bool bFl )
 {
   for( int i = pG->Columns->Count - 1; i >= 0; --i )
    { //AnsiString pp = pG->Columns->Items[i]->FieldName;
      if( pG->Columns->Items[i]->FieldName == rS )
       {
         pG->Columns->Items[i]->ReadOnly = bFl;
         return;
       }
    }
 }


void SetupFldClr( TField *Field, TColor &Background, TFont* rF, bool bSel )
 {
   /*if( rF && rF->Charset != RUSSIAN_CHARSET )
     rF->Charset = RUSSIAN_CHARSET,
     MessageBox( NULL, "hhh", "kkk", MB_OK );*/
   if( bSel ) return;
   if( Field->Tag == 3 ) Background = clInfoBk;//calc
   else if( Field->Tag == 1 ) Background = clCream; //null
   else if( Field->Tag == 2 ) Background = clMoneyGreen; //money
   else if( Field->Tag == 5 ) rF->Color = clGray; //money
 }

void __fastcall TForm1::MyIdleHandler( TObject *Sender, bool &Done )
 {
   Done = true;

   if( NavForm->BndGrid && IsWindowVisible(NavForm->Handle) != IsWindowVisible(NavForm->BndGrid->Handle) )
    {
      bool bFl = IsWindowVisible( NavForm->BndGrid->Handle );
      SetWindowPos( NavForm->Handle, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|(bFl ? SWP_SHOWWINDOW:SWP_HIDEWINDOW) ),
      NavForm->Visible = bFl;
    }
   else if( !NavForm->BndGrid )
     NavForm->Visible = false,
     SetWindowPos( NavForm->Handle, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_HIDEWINDOW );

   if( FHelper->BndCtrl && IsWindowVisible(FHelper->Handle) != IsWindowVisible(FHelper->BndCtrl->Handle) )
    {
      bool bFl = IsWindowVisible( FHelper->BndCtrl->Handle );
      SetWindowPos( FHelper->Handle, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|(bFl ? SWP_SHOWWINDOW:SWP_HIDEWINDOW) ),
      FHelper->Visible = bFl;
      if( bFl == false ) FHelper->BndCtrl = NULL;
    }
   else if( !FHelper->BndCtrl )
     FHelper->Visible = false,
     SetWindowPos( FHelper->Handle, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_HIDEWINDOW );

   if( DMod1 && DMod2 && m_bndGuest.Empty() )
    {
      m_bndGuest.Bnd( DMod1->srGuest );
      m_bndGuest, DBDateEdit1, DBEdit_Folio,
        DBEdit_GI, DBEdit_Details, RxDBGrid_MRooms, RxDBGrid_MAddCh,
        RxDBGrid_MPayments, DBMemo_Comments, RxDBGrid_Bills,

        DBEdit_TotalRooms, DBEdit_TaxServ, DBEdit_Subtotal,
        DBEdit_DepositReceived, DBEdit_TotalAdd, DBEdit_Payments,
        DBEdit_Total, DBEdit_Balance, DBEdit_BillNo;

      m_bndBill.Bnd( DMod1->srBill );
      m_bndBill, RxDBGrid_Rooms, RxDBGrid_AddCharges, RxDBGrid_Payments,
        RxDBGrid_MRooms, RxDBGrid_MAddCh, RxDBGrid_MPayments;

    }
   m_bndGuest.CheckState();
   m_bndBill.CheckState();

   if( FHelper ) FHelper->CheckState();
 }


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner), m_pLastGrid( NULL ), m_bFirstShow( true )
{
  memset( &m_arrLastFocus, 0, sizeof(m_arrLastFocus) );

  m_pNavForm = NavForm  = new TNavForm( this );
  FHelper = new TFHelper( this );
  Application->OnIdle = MyIdleHandler;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RxDBGrid_RoomEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //DBNavigator1->DataSource = dynamic_cast<TDBGrid*>(Sender)->DataSource;
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_Room,
    NavForm->Caption = "List of all rooms";

  NavForm->DBNavigator1->ConfirmDelete = false;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_GuestCatEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_GuestCat,
    NavForm->Caption = "All guests categoryes";

  NavForm->DBNavigator1->ConfirmDelete = false;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_BillsCatEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_BillsCat,
    NavForm->Caption = "All bills categoryes";
  NavForm->DBNavigator1->ConfirmDelete = false;
  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_RoomGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_GuestCatGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_BillsCatGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FocusLastGrid()
{
  if( m_pLastGrid && m_pLastGrid->CanFocus() )
   {
     m_pLastGrid->SetFocus();
     m_pLastGrid->OnEnter( m_pLastGrid );
   }
}
auto_ptr<TDBDataSet*> __fastcall TForm1::GetDSets( int iTab )
 {
   auto_ptr<TDBDataSet*> ap;
   TDBDataSet** p;
   switch( iTab )
    {
      /*case 0:
        {
          ap = auto_ptr<TDBDataSet*>( (p=new TDBDataSet*[2]) );
          p[0] = NULL; p[1] = NULL;
          return ap;
        }*/
      case 1:
        ap = auto_ptr<TDBDataSet*>( (p=new TDBDataSet*[4]) );
        p[2] = DMod1->qRoom2; p[1] = DMod1->qGuestCat2;
        p[0] = DMod1->qBillCat2;
        p[3] = NULL;
        return ap;
      case 2:
        ap = auto_ptr<TDBDataSet*>( (p=new TDBDataSet*[2]) );
        p[0] = NULL; p[1] = NULL;
        return ap;
      case 0:
      case 3:
        ap = auto_ptr<TDBDataSet*>( (p=new TDBDataSet*[6]) );
        p[0] = DMod1->tbGuest; p[1] = DMod1->tbPayments;
        p[2] = DMod1->tbAddCh; p[3] = DMod1->tbBill;
        p[4] = DMod1->tbGuestBillData; p[5] = NULL;
        return ap;
    }
 }

void __fastcall TForm1::PageControl1Changing(TObject *Sender,
      bool &AllowChange)
{
     auto_ptr<TDBDataSet*> arr = GetDSets( PageControl1->ActivePage->PageIndex );

   bool bRes = false;
   try {
     bRes = DMod1->PostUpdates( arr );
   }
   catch( EDBEngineError *e )
    {
      MessageBox( Application->MainForm->Handle, e->Message.c_str(), "Error", MB_OK|MB_ICONSTOP );
      //delete e;
    }
   catch( Exception *e )
    {
      MessageBox( Application->MainForm->Handle, e->Message.c_str(), "Error", MB_OK|MB_ICONSTOP );
      //delete e;
    }

   if( bRes == false )
    {
      FocusLastGrid();
      AllowChange = false;
      return;
    }

   LastFocus_ = ActiveControl;
   AllowChange = true;
   return;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormShow(TObject *Sender)
{
  NavForm->FormPlacement1->RestoreFormPlacement();
  FHelper->FormPlacement1->RestoreFormPlacement();

  //m_pNavForm->Show();
  //SetWindowPos( m_pNavForm->Handle, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
  AssRdProp( RxDBGrid_Bills, AnsiString("_DEPOSIT_RECEIVD") );
  AssRdProp( RxDBGrid_Bills, AnsiString("_BALANCE_DUE") );
  AssRdProp( RxDBGrid_Bills, AnsiString("BILL_NO") );

  //AssRdProp( RxDBGrid_Rooms, AnsiString("_RATE") );
  AssRdProp( RxDBGrid_Rooms, AnsiString("_NIGHTS") );
  AssRdProp( RxDBGrid_Rooms, AnsiString("_CHARGE") );

  AssElipsisProp( RxDBGrid_MRooms, AnsiString("DATE_IN") );
  AssElipsisProp( RxDBGrid_MRooms, AnsiString("DATE_OUT") );
  AssElipsisProp( RxDBGrid_MAddCh, AnsiString("MKDATE") );

  AssElipsisProp( RxDBGrid_Bills, AnsiString("DATE_") );
  AssElipsisProp( RxDBGrid_Rooms, AnsiString("DATE_IN") );
  AssElipsisProp( RxDBGrid_Rooms, AnsiString("DATE_OUT") );
  AssElipsisProp( RxDBGrid_AddCharges, AnsiString("MKDATE") );

  if( PageControl1->ActivePage->PageIndex == 0 && m_bFirstShow )
   {
     m_bFirstShow = false;
     if( DBEdit_BillNo->CanFocus() ) DBEdit_BillNo->SetFocus();
   }
}
//---------------------------------------------------------------------------






void __fastcall TForm1::RxDBGrid_GuestGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_BillsGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_RoomsGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_AddChargesGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_PaymentsGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  NavForm->FormPlacement1->SaveFormPlacement();
  FHelper->FormPlacement1->SaveFormPlacement();
  NavForm->Visible = false;
  FHelper->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_GuestEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
    //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_Guest,
    NavForm->Caption = "All Guests";

  NavForm->DBNavigator1->ConfirmDelete = true;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_BillsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_Bills,
    NavForm->Caption = "Bills of selected guest";

  NavForm->DBNavigator1->ConfirmDelete = true;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_RoomsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_Rooms,
    NavForm->Caption = "Rooms used of selected bill";

   NavForm->DBNavigator1->ConfirmDelete = false;

   m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_AddChargesEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_AddCharges,
    NavForm->Caption = "Additional charges of selected bill";

  NavForm->DBNavigator1->ConfirmDelete = false;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_PaymentsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  //if( dynamic_cast<TDBGrid*>(Sender) )
    NavForm->BndGrid = RxDBGrid_Payments,
    NavForm->Caption = "Payments of selected bill";

  NavForm->DBNavigator1->ConfirmDelete = false;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  auto_ptr<TDBDataSet*> arr = GetDSets( PageControl1->ActivePage->PageIndex );
  DMod1->PostUpdates( arr );
  CanClose = true;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::RxDBGrid_MRoomsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = RxDBGrid_MRooms,
  NavForm->Caption = "Used rooms";

  NavForm->DBNavigator1->ConfirmDelete = false;
  FHelper->BndCtrl = NULL;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_MAddChEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = RxDBGrid_MAddCh,
  NavForm->Caption = "Additional charges";

  NavForm->DBNavigator1->ConfirmDelete = false;
  FHelper->BndCtrl = NULL;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_MPaymentsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = RxDBGrid_MPayments,
  NavForm->Caption = "Payments";

  NavForm->DBNavigator1->ConfirmDelete = false;
  FHelper->BndCtrl = NULL;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_MRoomsGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_MAddChGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_MPaymentsGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_FNameEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_LNameEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );

  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBCalcEdit_DepositEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBDateEdit1Enter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Bill );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_AddressEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_CityEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );

  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBComboBox_StateEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_FolioEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Bill );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_GIEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Bill );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_PostEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBLookupCombo_CategoryEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Guest );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_DetailsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Bill );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBMemo_CommentsEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = NULL;
  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Bill );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DateEdit1Change(TObject *Sender)
{
  if( !DMod1 ) return;

  if( DateEdit1->Date < TDateTime(1990, 1, 1) )
   {
     //Action = false;
     //MessageBox( Application->MainForm->Handle, "Bad date. Needing [Date To] >= '1 Jan 1990'", "Error", MB_OK|MB_ICONSTOP );
     return;
   }

  DMod1->qBillSum->ParamByName( "d1" )->AsDateTime =
      DateEdit1->Date;
  DMod1->qBillSumAll->ParamByName( "d1" )->AsDateTime =
    DateEdit1->Date;


  DMod1->srBillSum->DataSet->DisableControls();
  if( DMod1->srBillSum->DataSet->Active )
    DMod1->srBillSum->DataSet->Close();
  ((TQuery*)DMod1->srBillSum->DataSet)->Prepared = true;
  DMod1->srBillSum->DataSet->Open();
  DMod1->srBillSum->DataSet->EnableControls();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DateEdit2Change(TObject *Sender)
{
  if( !DMod1 ) return;

  if( DateEdit2->Date < TDateTime(1990, 1, 1) )
   {
     //Action = false;
     //MessageBox( Application->MainForm->Handle, "Bad date. Needing [Date To] >= '1 Jan 1990'", "Error", MB_OK|MB_ICONSTOP );
     return;
   }

  DMod1->qBillSum->ParamByName( "d2" )->AsDateTime =
    DateEdit2->Date;
  DMod1->qBillSumAll->ParamByName( "d2" )->AsDateTime =
    DateEdit2->Date;

  DMod1->srBillSum->DataSet->DisableControls();
  if( DMod1->srBillSum->DataSet->Active )
    DMod1->srBillSum->DataSet->Close();
  ((TQuery*)DMod1->srBillSum->DataSet)->Prepared = true;
  DMod1->srBillSum->DataSet->Open();
  DMod1->srBillSum->DataSet->EnableControls();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  Application->OnException = AppException;

  DateEdit1->Date = TDateTime::CurrentDate() - 31;
  DateEdit2->Date = TDateTime::CurrentDate() + 1;

  m_arrLastFocus[ 0 ] = DBEdit_BillNo;
  //ActiveControl = DBEdit_BillNo;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{
  DMod1->srBillSum->DataSet = DMod1->qBillSumAll;
  BitBtn_SumRep->Enabled = false;

  DMod1->srBillSum->DataSet->DisableControls();
  if( DMod1->srBillSum->DataSet->Active )
    DMod1->srBillSum->DataSet->Close();
  ((TQuery*)DMod1->srBillSum->DataSet)->Prepared = true;
  DMod1->srBillSum->DataSet->Open();
  DMod1->srBillSum->DataSet->EnableControls();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
  DMod1->srBillSum->DataSet = DMod1->qBillSum;
  BitBtn_SumRep->Enabled = true;

  DMod1->srBillSum->DataSet->DisableControls();
  if( DMod1->srBillSum->DataSet->Active )
    DMod1->srBillSum->DataSet->Close();
  ((TQuery*)DMod1->srBillSum->DataSet)->Prepared = true;
  DMod1->srBillSum->DataSet->Open();
  DMod1->srBillSum->DataSet->EnableControls();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::RxDBGrid_SumRepGetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
  TDBGrid *p = dynamic_cast<TDBGrid*>( Sender );
  if( p && p->Enabled )
    SetupFldClr( Field, Background, AFont, Highlight );    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_SumRepEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );
  
  NavForm->BndGrid = RxDBGrid_SumRep,
  NavForm->Caption = "Bill's summary report";

  NavForm->DBNavigator1->ConfirmDelete = false;

  m_pLastGrid = dynamic_cast<TDBGrid*>( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBEdit_BillNoEnter(TObject *Sender)
{
  LastFocus_ = dynamic_cast<TWinControl*>( Sender );

  FHelper->MakeBind( dynamic_cast<TWinControl*>(Sender), SR_Bill );
}
//---------------------------------------------------------------------------


void __fastcall TForm1::PageControl1Change(TObject *Sender)
{        //TWinControl *pp = LastFocus_;
  if( LastFocus_ && LastFocus_->CanFocus() )
   {
     ActiveControl = LastFocus_;
     //LastFocus_->SetFocus();
     //if( dynamic_cast<TCustomControl*>(LastFocus_) )
       //dynamic_cast<TCustomControl*>(LastFocus_)->OnEnter( LastFocus_ );
   }
  else if( LastFocus_ == NULL && PageControl1->ActivePage->PageIndex == 0 &&
           DBEdit_BillNo->CanFocus()
         ) ActiveControl = DBEdit_BillNo;//DBEdit_BillNo->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn_SumRepClick(TObject *Sender)
{
  if( QuickReport_Sum->m_Busy )
   {
     MessageBox( Application->MainForm->Handle, "Report is running",
       "Error", MB_OK|MB_ICONSTOP );
     return;
   }

  QuickReport_Sum->Date1 = DateEdit1->Date;
  QuickReport_Sum->Date2 = DateEdit2->Date;
  QuickReport_Sum->Cat   = DMod1->tbCat_ForLstID_CAT->Value;

  try {
    QuickReport_Sum->m_Busy = true;
    QuickReport_Sum->Preview();
   }
  catch( Exception *e )
   {
     QuickReport_Sum->m_Busy = false;
     throw e;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
  if( MessageBox(Application->MainForm->Handle, "Remove all listed bills ?",
              "Message", MB_YESNO|MB_ICONQUESTION) == IDNO
    ) return;

  auto_ptr<TQuery> apQ( new TQuery(NULL) );
  apQ->DatabaseName = MainDmod->DB1->DatabaseName;
  apQ->SQL->Add( "DELETE * FROM TmpBill" );
  apQ->ExecSQL();
  apQ->SQL->Clear();
  if( RadioButton2->Checked )
   {
      apQ->SQL->Add( "INSERT INTO TmpBill SELECT b.BILL_NO as [BILL_NO] "
        "FROM GuestBill b INNER JOIN Guest g ON b.ID_GUEST=g.ID_GUEST "
        "WHERE b.DATE_ >= :date1 and b.DATE_ <= :date2 and g.ID_CAT = :id_cat" );

      TParam *pDate1 = apQ->Params->ParamByName( "date1" );
      pDate1->DataType = ftDateTime; pDate1->ParamType = ptInput;
      pDate1->Value = DateEdit1->Date;

      TParam *pDate2 = apQ->Params->ParamByName( "date2" );
      pDate2->DataType = ftDateTime; pDate2->ParamType = ptInput;
      pDate2->Value = DateEdit2->Date;

      TParam *pBN = apQ->Params->ParamByName( "id_cat" );
      pBN->DataType = ftInteger; pBN->ParamType = ptInput;
      pBN->Value = DMod1->tbCat_ForLstID_CAT->Value;

      apQ->ExecSQL();
   }
  else
   {
     apQ->SQL->Add( "INSERT INTO TmpBill SELECT b.BILL_NO as [BILL_NO] "
        "FROM GuestBill b INNER JOIN Guest g ON b.ID_GUEST=g.ID_GUEST "
        "WHERE b.DATE_ >= :date1 and b.DATE_ <= :date2" );

      TParam *pDate1 = apQ->Params->ParamByName( "date1" );
      pDate1->DataType = ftDateTime; pDate1->ParamType = ptInput;
      pDate1->Value = DateEdit1->Date;

      TParam *pDate2 = apQ->Params->ParamByName( "date2" );
      pDate2->DataType = ftDateTime; pDate2->ParamType = ptInput;
      pDate2->Value = DateEdit2->Date;

      apQ->ExecSQL();
   }

  apQ->SQL->Clear();
  apQ->SQL->Add( "DELETE * FROM GuestBill bb where bb.BILL_NO in (select [BILL_NO] from TmpBill)" );
  apQ->ExecSQL();
  int iRec = apQ->RowsAffected;

  apQ->SQL->Clear();
  apQ->SQL->Add( "DELETE * FROM TmpBill" );
  apQ->ExecSQL();

  DMod1->srBillSum->DataSet->DisableControls();
  DMod1->srBillSum->DataSet->Close();
  DMod1->srBillSum->DataSet->Open();
  DMod1->srBillSum->DataSet->EnableControls();

  MessageBox( Application->MainForm->Handle,
              Format("Deleted %u bills", ARRAYOFCONST((iRec))).c_str(),
              "Message", MB_OK|MB_ICONINFORMATION );
}
//---------------------------------------------------------------------------


void __fastcall TForm1::RxDBGrid_MRoomsEditButtonClick(TObject *Sender)
{
  InputDF( dynamic_cast<TRxDBGrid*>(Sender) );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_MAddChEditButtonClick(TObject *Sender)
{
  InputDF( dynamic_cast<TRxDBGrid*>(Sender) );    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_BillsEditButtonClick(TObject *Sender)
{
  InputDF( dynamic_cast<TRxDBGrid*>(Sender) );    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_RoomsEditButtonClick(TObject *Sender)
{
  InputDF( dynamic_cast<TRxDBGrid*>(Sender) );    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RxDBGrid_AddChargesEditButtonClick(TObject *Sender)
{
  InputDF( dynamic_cast<TRxDBGrid*>(Sender) );
}
//---------------------------------------------------------------------------

void CallPFF( WORD Key, TDataSet *pD )
 {
   if( Key == VK_F12 ) pD->Post();
   else pD->Cancel();
 }

void __fastcall TForm1::RxDBGrid_MRoomsKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if( Key == VK_F12 || Key == VK_ESCAPE )
   {
     TRxDBGrid *pG;
     TDBEdit *pE;
     TRxDBLookupCombo *pLC;
     TDBDateEdit *pD;
     TRxDBCalcEdit *pCE;
     TDBComboBox *pCBN;
     TDBMemo *pM;

     if( (pG=dynamic_cast<TRxDBGrid*>(Sender)) )
      {
        if( pG->DataSource->DataSet->State == dsEdit ||
            pG->DataSource->DataSet->State == dsInsert
          ) pG->DataSource->DataSet->Post();
        else MessageBeep( -1 ) ;
      }
     else if( (pE=dynamic_cast<TDBEdit*>(Sender)) )
      {
        if( pE->DataSource->DataSet->State == dsEdit ||
            pE->DataSource->DataSet->State == dsInsert
          ) CallPFF( Key, pE->DataSource->DataSet );
        else MessageBeep( -1 ) ;
      }
     else if( (pLC=dynamic_cast<TRxDBLookupCombo*>(Sender)) )
      {
        if( pLC->DataSource->DataSet->State == dsEdit ||
            pLC->DataSource->DataSet->State == dsInsert
          ) CallPFF( Key, pLC->DataSource->DataSet );
        else MessageBeep( -1 ) ;
      }
     else if( (pD=dynamic_cast<TDBDateEdit*>(Sender)) )
      {
        if( pD->DataSource->DataSet->State == dsEdit ||
            pD->DataSource->DataSet->State == dsInsert
          ) CallPFF( Key, pD->DataSource->DataSet );
        else MessageBeep( -1 ) ;
      }
     else if( (pCE=dynamic_cast<TRxDBCalcEdit*>(Sender)) )
      {
        if( pCE->DataSource->DataSet->State == dsEdit ||
            pCE->DataSource->DataSet->State == dsInsert
          ) CallPFF( Key, pCE->DataSource->DataSet );
        else MessageBeep( -1 ) ;
      }
     else if( (pCBN=dynamic_cast<TDBComboBox*>(Sender)) )
      {
        if( pCBN->DataSource->DataSet->State == dsEdit ||
            pCBN->DataSource->DataSet->State == dsInsert
          ) CallPFF( Key, pCBN->DataSource->DataSet );
        else MessageBeep( -1 ) ;
      }
     else if( (pM=dynamic_cast<TDBMemo*>(Sender)) )
      {
        if( pM->DataSource->DataSet->State == dsEdit ||
            pM->DataSource->DataSet->State == dsInsert
          ) CallPFF( Key, pM->DataSource->DataSet );
        else MessageBeep( -1 ) ;
      }
   }

}
//---------------------------------------------------------------------------




