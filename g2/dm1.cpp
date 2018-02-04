//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dm1.h"
#include "dm2.h"
#include "guana2_mf.h"
#include "main_dmod.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDMod1 *DMod1 = NULL;

void MyRefr( TDataSet *DataSet )
 {
   TBookmark bmCurrent;
   bmCurrent = DataSet->GetBookmark();

   DataSet->DisableControls();
   try {
      DataSet->Active = false, DataSet->Active = true;
    }
   catch( Exception *e )
    {
      DataSet->FreeBookmark( bmCurrent );
      DataSet->EnableControls();
      throw e;
    }
   try {
      //DataSet->GotoBookmark( bmCurrent );
    }
   catch( Exception *e )
    {
    }
   DataSet->FreeBookmark( bmCurrent );
   DataSet->EnableControls();
 }

bool __fastcall TDMod1::PostUpdates( auto_ptr<TDBDataSet*>& rA )
 {
   TDBDataSet **p = rA.get();

   bool bFlUpd = false;
   int iCnt = 0;
   for( ; *p; ++p, iCnt++ )
    {
      if( (*p)->State == dsEdit || (*p)->State == dsInsert )
        (*p)->Post();
      if( (*p)->CachedUpdates ) bFlUpd = true;
    }

   if( bFlUpd ) MainDmod->DB1->ApplyUpdates( rA.get(), iCnt - 1 );

   return true;
 }


//---------------------------------------------------------------------------
__fastcall TDMod1::TDMod1(TComponent* Owner)
    : TDataModule(Owner)
{
  m_bFl_RoomsNew = m_bFl_AddChNew = m_bFl_BillNew =
  m_bFl_PaymentsNew = false;

  m_bQGuestCat_Ins = m_bQBillCat_Ins = m_bQRoom_Ins = false;
}
//---------------------------------------------------------------------------
void __fastcall TDMod1::tbGuestBillDataBeforePost(TDataSet *DataSet)
{
  try {
     /*if( DMod2->qRoomRate->Prepared == false ) DMod2->qRoomRate->Prepare();

     if( DMod2->qRoomRate->Active ) DMod2->qRoomRate->Close();
     DMod2->qRoomRate->ParamByName( "id_room" )->Value = tbGuestBillDataID_ROOM->Value;

     DMod2->qRoomRate->Open();
     DMod2->qRoomRate->Next();*/

     /*AssRdProp( Form1->RxDBGrid_Rooms, AnsiString("_RATE"), false );
     tbGuestBillData_RATE->AsCurrency = DMod2->qRoomRate->FieldByName( "RATE" )->AsCurrency;
     AssRdProp( Form1->RxDBGrid_Rooms, AnsiString("_RATE") );*/
    }
   catch( EDatabaseError  *e )
    {
      MessageBox( Application->MainForm->Handle, e->Message.c_str(),
        "Error", MB_OK|MB_ICONSTOP );
    }
   catch( Exception *e )
    {
      MessageBox( Application->MainForm->Handle, e->Message.c_str(),
        "Error", MB_OK|MB_ICONSTOP );
    }

   if( DMod2->qRoomRate->Active ) DMod2->qRoomRate->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::DMod1Create(TObject *Sender)
{
  tbGuest->Active = true;
  tbBill->Active = true;

  tbRoom->Active = true;
  tbGuestCat->Active = true;
  tbBillCat->Active = true;

  qRoom2->Active = true;
  qGuestCat2->Active = true;
  qBillCat2->Active = true;

  tbPayments->Active = true;
  tbGuestBillData->Active = true;
  tbAddCh->Active = true;

  tbBill->Refresh();
  tbPayments->Refresh();
  tbGuestBillData->Refresh();
  tbAddCh->Refresh();

  qBillSum->ParamByName( "d1" )->AsDateTime =
    Form1->DateEdit1->Date;
  qBillSumAll->ParamByName( "d1" )->AsDateTime =
    Form1->DateEdit1->Date;

  qBillSum->ParamByName( "d2" )->AsDateTime =
    Form1->DateEdit2->Date;
  qBillSumAll->ParamByName( "d2" )->AsDateTime =
    Form1->DateEdit2->Date;

  if( tbCat_ForLst->Active == false ) tbCat_ForLst->Open();
  tbCat_ForLst->First();
  qBillSum->ParamByName( "id_cat" )->Value =
    tbCat_ForLstID_CAT->Value;


  //qBillSumAll->Prepared = true;
  //qBillSum->Prepared = true;
  qBillSumAll->Open();
  qBillSum->Open();

  //tbBill->First();

  //tbGuest->Last(); tbGuest->First();
}
//---------------------------------------------------------------------------


void __fastcall TDMod1::tbPaymentsAfterPost(TDataSet *DataSet)
{
  if( m_bFl_PaymentsNew ) DataSet->Refresh(),
    m_bFl_PaymentsNew = false;

  tbBill->Edit();
  tbBillCalcFields( NULL );
  tbBill->Post();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestBillDataAfterInsert(TDataSet *DataSet)
{
  m_bFl_RoomsNew = true;
  TDateTime dt = TDateTime::CurrentDate();
  tbGuestBillDataDATE_IN->AsDateTime = dt; dt += 3;
  tbGuestBillDataDATE_OUT->AsDateTime = dt;
  tbGuestBillDataNO_OF_GUESTS->AsInteger = 2;

  tbRoom->First();
  if( !tbRoomID_ROOM->IsNull )
    tbGuestBillDataID_ROOM->AsVariant = tbRoomID_ROOM->AsVariant;

  if( DMod2->qRoomRate->Prepared == false ) DMod2->qRoomRate->Prepare();

  if( DMod2->qRoomRate->Active ) DMod2->qRoomRate->Close();
  DMod2->qRoomRate->ParamByName( "id_room" )->Value = tbGuestBillDataID_ROOM->Value;

  DMod2->qRoomRate->Open();
  //DMod2->qRoomRate->Next();
  tbGuestBillData_RATE->AsCurrency = DMod2->qRoomRate->FieldByName( "RATE" )->AsCurrency;
  DMod2->qRoomRate->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestBillDataAfterCancel(TDataSet *DataSet)
{
  m_bFl_RoomsNew = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbPaymentsAfterCancel(TDataSet *DataSet)
{
  m_bFl_PaymentsNew = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbPaymentsAfterInsert(TDataSet *DataSet)
{
  m_bFl_PaymentsNew = true;

  tbPaymentsAMOUNT->AsCurrency = 0;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestBillDataAfterPost(TDataSet *DataSet)
{
  if( m_bFl_RoomsNew ) DataSet->Refresh(),
    m_bFl_RoomsNew = false;

  tbBill->Edit();
  tbBillCalcFields( NULL );
  tbBill->Post();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbAddChAfterCancel(TDataSet *DataSet)
{
  m_bFl_AddChNew = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbAddChAfterInsert(TDataSet *DataSet)
{
  m_bFl_AddChNew = true;

  tbAddChAMOUNT->AsCurrency = 0;

  tbBillCat->First();
  if( !tbBillCatID_BILL->IsNull )
    tbAddChID_BILL->AsVariant = tbBillCatID_BILL->AsVariant;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbAddChAfterPost(TDataSet *DataSet)
{
  if( m_bFl_AddChNew ) DataSet->Refresh(),
    m_bFl_AddChNew = false;

  tbBill->Edit();
  tbBillCalcFields( NULL );
  tbBill->Post();
}
//---------------------------------------------------------------------------




void __fastcall TDMod1::tbBillAfterCancel(TDataSet *DataSet)
{
  m_bFl_BillNew = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbBillAfterInsert(TDataSet *DataSet)
{
  m_bFl_BillNew = true;
  tbBillDATE_->AsDateTime = TDateTime::CurrentDate();

  AssRdProp( Form1->RxDBGrid_Bills, AnsiString("_DEPOSIT_RECEIVD"), false );
  AssRdProp( Form1->RxDBGrid_Bills, AnsiString("_BALANCE_DUE"), false );
  tbBill_DEPOSIT_RECEIVD->AsCurrency = 0;
  tbBill_BALANCE_DUE->AsCurrency = 0;
  AssRdProp( Form1->RxDBGrid_Bills, AnsiString("_DEPOSIT_RECEIVD") );
  AssRdProp( Form1->RxDBGrid_Bills, AnsiString("_BALANCE_DUE") );
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbBillAfterPost(TDataSet *DataSet)
{
  if( m_bFl_BillNew ) DataSet->Refresh(),
    m_bFl_BillNew = false;
}
//---------------------------------------------------------------------------


void __fastcall TDMod1::tbRoomAfterInsert(TDataSet *DataSet)
{
  tbRoomRATE->AsCurrency = 0;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestAfterInsert(TDataSet *DataSet)
{
  tbGuestDEPOSIT->AsCurrency = 0;
  tbGuestCat->First();
  if( !tbGuestCatID_CAT->IsNull )
    tbGuestID_CAT->AsVariant = tbGuestCatID_CAT->AsVariant;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestBillDataCalcFields(TDataSet *DataSet)
{
  tbGuestBillData_NIGHTS->AsInteger = tbGuestBillDataDATE_OUT->AsDateTime - tbGuestBillDataDATE_IN->AsDateTime;
  tbGuestBillData_CHARGE->AsCurrency =
    tbGuestBillData_RATE->AsCurrency *
    /*Currency(tbGuestBillDataNO_OF_GUESTS->AsInteger) **/
    Currency(tbGuestBillData_NIGHTS->AsInteger);
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbBillCalcFields(TDataSet *DataSet)
{
  //if( !DMod2 || !DMod2->qTotalRooms ) return;
  try {
     if( DMod2->qTotalRooms->Prepared == false ) DMod2->qTotalRooms->Prepare();
     if( DMod2->qTotalADd->Prepared == false ) DMod2->qTotalADd->Prepare();
     if( DMod2->qTotalPayments->Prepared == false ) DMod2->qTotalPayments->Prepare();

     if( DMod2->qTotalRooms->Active ) DMod2->qTotalRooms->Close();
     if( DMod2->qTotalADd->Active ) DMod2->qTotalADd->Close();
     if( DMod2->qTotalPayments->Active ) DMod2->qTotalPayments->Close();

     DMod2->qTotalRooms->ParamByName( "bill_no" )->Value = tbBillBILL_NO->Value;
     DMod2->qTotalADd->ParamByName( "bill_no" )->Value = tbBillBILL_NO->Value;
     DMod2->qTotalPayments->ParamByName( "bill_no" )->Value = tbBillBILL_NO->Value;


     DMod2->qTotalRooms->Open();
     DMod2->qTotalRooms->Next();

     DMod2->qTotalADd->Open();
     DMod2->qTotalADd->Next();

     DMod2->qTotalPayments->Open();
     DMod2->qTotalPayments->Next();

     tbBill_TOTAL_ROOMS->AsCurrency =
       DMod2->qTotalRooms->FieldByName( "SAM" )->IsNull ? Currency(0):DMod2->qTotalRooms->FieldByName( "SAM" )->AsCurrency;

     tbBill_TOTAL_ADDCH->AsCurrency =
       DMod2->qTotalADd->FieldByName( "SAM" )->IsNull ? Currency(0):DMod2->qTotalADd->FieldByName( "SAM" )->AsCurrency;

     tbBill_TOTAL_PAYMENTS->AsCurrency =
       DMod2->qTotalPayments->FieldByName( "SAM" )->IsNull ? Currency(0):DMod2->qTotalPayments->FieldByName( "SAM" )->AsCurrency;

     tbBill_SUBTOTAL->AsCurrency = tbBill_TOTAL_ADDCH->AsCurrency + tbBill_TOTAL_ROOMS->AsCurrency;
     tbBill_TAX_AND_SERVICE->AsCurrency = tbBill_SUBTOTAL->AsCurrency * 17.0 / 100.0;
     tbBill_TOTAL->AsCurrency = tbBill_SUBTOTAL->AsCurrency + tbBill_TAX_AND_SERVICE->AsCurrency - tbBill_TOTAL_PAYMENTS->AsCurrency;
    }
   catch( EDatabaseError  *e )
    {
      MessageBox( Application->MainForm->Handle, e->Message.c_str(),
        "Error", MB_OK|MB_ICONSTOP );
    }
   catch( Exception *e )
    {
      MessageBox( Application->MainForm->Handle, e->Message.c_str(),
        "Error", MB_OK|MB_ICONSTOP );
    }

   if( DMod2->qTotalRooms->Active ) DMod2->qTotalRooms->Close();
   if( DMod2->qTotalADd->Active ) DMod2->qTotalADd->Close();
   if( DMod2->qTotalPayments->Active ) DMod2->qTotalPayments->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbCat_ForLstAfterScroll(TDataSet *DataSet)
{
  if( Form1->DBLookupListBox_Cat->Visible == false ) return;

  /*if( Form1->RadioButton1->Checked )
   {
     qBillSum->ParamByName( "id_cat" )->AsInteger =
       tbCat_ForLstID_CAT->AsInteger;
     return;
   }*/

  qBillSum->DisableControls();
  if( qBillSum->Active )
    qBillSum->Close();

  qBillSum->ParamByName( "id_cat" )->Value =
    tbCat_ForLstID_CAT->Value;
  qBillSum->Prepared = true;

  qBillSum->Open();
  qBillSum->EnableControls();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestBeforePost(TDataSet *DataSet)
{
  if( tbGuestDEPOSIT->IsNull )
    tbGuestDEPOSIT->AsCurrency = 0;
}
//---------------------------------------------------------------------------



void __fastcall TDMod1::qGuestCat2BeforePost(TDataSet *DataSet)
{
  m_bQGuestCat_Ins = DataSet->State == dsInsert;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qGuestCat2PostError(TDataSet *DataSet,
      EDatabaseError *E, TDataAction &Action)
{
  m_bQGuestCat_Ins = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qBillCat2BeforePost(TDataSet *DataSet)
{
  m_bQBillCat_Ins = DataSet->State == dsInsert;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qBillCat2PostError(TDataSet *DataSet,
      EDatabaseError *E, TDataAction &Action)
{
  m_bQBillCat_Ins = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qRoom2BeforePost(TDataSet *DataSet)
{
  m_bQRoom_Ins = DataSet->State == dsInsert;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qRoom2PostError(TDataSet *DataSet,
      EDatabaseError *E, TDataAction &Action)
{
  m_bQRoom_Ins = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qBillCat2AfterPost(TDataSet *DataSet)
{
  if( m_bQBillCat_Ins ) MyRefr( DataSet );
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qGuestCat2AfterPost(TDataSet *DataSet)
{
  if( m_bQGuestCat_Ins ) MyRefr( DataSet );
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::qRoom2AfterPost(TDataSet *DataSet)
{
  if( m_bQRoom_Ins ) MyRefr( DataSet );
}
//---------------------------------------------------------------------------


void __fastcall TDMod1::tbGuestBillDataAfterDelete(TDataSet *DataSet)
{
  tbBill->Edit();
  tbBillCalcFields( NULL );
  tbBill->Post();
}
//---------------------------------------------------------------------------

void __fastcall TDMod1::tbGuestBillDataID_ROOMChange(TField *Sender)
{
  if( DMod2->qRoomRate->Prepared == false ) DMod2->qRoomRate->Prepare();

  if( DMod2->qRoomRate->Active ) DMod2->qRoomRate->Close();
  DMod2->qRoomRate->ParamByName( "id_room" )->Value = tbGuestBillDataID_ROOM->Value;

  DMod2->qRoomRate->Open();
  //DMod2->qRoomRate->Next();
  tbGuestBillData_RATE->AsCurrency = DMod2->qRoomRate->FieldByName( "RATE" )->AsCurrency;
  DMod2->qRoomRate->Close();
}
//---------------------------------------------------------------------------

