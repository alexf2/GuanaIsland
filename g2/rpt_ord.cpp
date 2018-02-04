//----------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "rpt_ord.h"
#include "main_dmod.h"
#include "dm1.h"
//----------------------------------------------------------------------------
#pragma link "RxMemDS"
#pragma resource "*.dfm"
TQuickReport_Ordinal *QuickReport_Ordinal;

void TQuickReport_Ordinal::AssField( TField* pF, char *lpName, TDBDataSet *pD )
 {
   Variant vvt; vvt.ChangeType( varNull );
   pF->Value = pD->Eof ? vvt:pD->FieldByName(lpName)->Value;
 }

void TQuickReport_Ordinal::EraseTmpTables()
 {
   auto_ptr<TQuery> apQ( new TQuery(NULL) );
   apQ->SQL->Add( "DELETE * FROM Part_AddCh" );
   apQ->DatabaseName = MainDmod->DB1->DatabaseName;
   apQ->ExecSQL();

   apQ->SQL->Clear();
   apQ->SQL->Add( "DELETE * FROM Part_AddChSum" );
   apQ->ExecSQL();
 }

void TQuickReport_Ordinal::SetBillNo( Variant vtBillNo )
 {
   CloseDSets();

   m_vBillNo = vtBillNo;
   qBill->ParamByName( "bill_no" )->Value = vtBillNo;
   qBill2->ParamByName( "bill_no" )->Value = vtBillNo;
   qBill3->ParamByName( "bill_no" )->Value = vtBillNo;
   qTotalAdd->ParamByName( "bill_no" )->Value = vtBillNo;
   qPayments->ParamByName( "bill_no" )->Value = vtBillNo;
   qTotalPayments->ParamByName( "bill_no" )->Value = vtBillNo;
   char cch[64]; sprintf( cch, "%u", (int)vtBillNo );
   QRLabel_BN->Caption = "Bill No: " + AnsiString(cch);

   EraseTmpTables();

   auto_ptr<TQuery> apQ( new TQuery(NULL) );
   apQ->DatabaseName = MainDmod->DB1->DatabaseName;

   char cBuf[ 256 ];
   sprintf( cBuf, "INSERT INTO Part_AddCh SELECT a.ID_AC as ID_AC,b.NAME as NAME,a.AMOUNT as AMOUNT,a.MKDATE as MKDATE from "
     "AdditionalCharge a INNER JOIN BillCategory b ON a.ID_BILL=b.ID_BILL"
     " WHERE a.BILL_NO=%u  ORDER BY a.ID_BILL", (int)vtBillNo );

   apQ->SQL->Clear();
   apQ->SQL->Add( cBuf );
   apQ->ExecSQL();

   sprintf( cBuf,
     "INSERT INTO Part_AddChSum SELECT p.NAME as NAME, sum(p.AMOUNT) as AMOUNT from "
     "Part_AddCh p"
     " GROUP BY NAME ORDER BY NAME" );

   apQ->SQL->Clear();
   apQ->SQL->Add( cBuf );
   apQ->ExecSQL();

   auto_ptr<TQuery> apQ1( new TQuery(NULL) );
   auto_ptr<TQuery> apQ2( new TQuery(NULL) );
   apQ1->DatabaseName = MainDmod->DB1->DatabaseName;
   apQ2->DatabaseName = MainDmod->DB1->DatabaseName;

   apQ1->SQL->Add( "SELECT * FROM Part_AddCh ORDER BY [NAME]" );
   apQ2->SQL->Add( "SELECT * FROM Part_AddChSum  ORDER BY [NAME]" );
   apQ1->Open(); apQ2->Open();
   apQ1->First(), apQ2->First();

   qPayments->Open(); qPayments->First();

   RxMemoryData_AddCh->Active = true;

   while( !apQ1->Eof || !apQ1->Eof || !qPayments->Eof )
    {
      RxMemoryData_AddCh->Append();

      AssField( RxMemoryData_AddChID_AC, "ID_AC", apQ1.get() );
      AssField( RxMemoryData_AddChNAME, "NAME", apQ1.get() );
      AssField( RxMemoryData_AddChAMOUNT, "AMOUNT", apQ1.get() );
      AssField( RxMemoryData_AddChMKDATE, "MKDATE", apQ1.get() );

      AssField( RxMemoryData_AddChNAME2, "NAME", apQ2.get() );
      AssField( RxMemoryData_AddChAMOUNT2, "AMOUNT", apQ2.get() );

      AssField( RxMemoryData_AddChP_AMOUNT, "AMOUNT", qPayments );
      AssField( RxMemoryData_AddChP_DESCR, "DESCRIPTION", qPayments );

      RxMemoryData_AddCh->Post();

      if( !apQ1->Eof ) apQ1->Next();
      if( !apQ2->Eof ) apQ2->Next();
      if( !qPayments->Eof ) qPayments->Next();
    }

    qPayments->Close(); apQ1->Close(); apQ2->Close();

    RxMemoryData_AddCh->First();
    EraseTmpTables();

    RxMemoryData_Summ->Active = true;
    RxMemoryData_Summ->Append();

    RxMemoryData_SummSUBTOTAL->Value = DMod1->tbBill_SUBTOTAL->Value;
    RxMemoryData_SummTAX_AND_SRV->Value = DMod1->tbBill_TAX_AND_SERVICE->Value;
    RxMemoryData_SummTOTAL->Value = DMod1->tbBill_TOTAL->Value;
    RxMemoryData_SummDEPOSIT->Value = DMod1->tbBill_DEPOSIT_RECEIVD->Value;
    RxMemoryData_SummPAYMENTS->Value = DMod1->tbBill_TOTAL_PAYMENTS->Value;
    RxMemoryData_SummBALANCE_DUE->Value = DMod1->tbBill_BALANCE_DUE->Value;

    RxMemoryData_Summ->Post();
    RxMemoryData_Summ->First();
 }

//----------------------------------------------------------------------------
__fastcall TQuickReport_Ordinal::TQuickReport_Ordinal(TComponent* Owner)
    : TQuickRep(Owner), m_vBillNo( 0L ), m_Busy( false )
{
}
//----------------------------------------------------------------------------

void __fastcall TQuickReport_Ordinal::QuickReport_OrdinalBeforePrint(
      TQuickRep *Sender, bool &PrintReport)
{
  qBill->Prepared = true;
  qBill->Open();
  //qBill->Next();

  qStatus->Prepared = true;
  qStatus->Open();

  qRoom->Prepared = true;
  qRoom->Open();

  qRoomSumm->Prepared = true;
  qRoomSumm->Open();

  qBill2->Prepared = true;
  qBill2->Open();
  qBill3->Prepared = true;
  qBill3->Open();

  qAddCh->Prepared = true;
  qAddCh->Open();
  qAddChSum->Prepared = true;
  qAddChSum->Open();

  qTotalAdd->Prepared = true;
  qTotalAdd->Open();
  qTotalPayments->Prepared = true;
  qTotalPayments->Open();
}
//---------------------------------------------------------------------------

void  TQuickReport_Ordinal::CloseDSets()
 {
  if( qStatus->Active ) qStatus->Close();
  if( qBill->Active ) qBill->Close();
  if( qRoom->Active ) qRoom->Close();
  if( qRoomSumm->Active ) qRoomSumm->Close();
  if( qBill2->Active ) qBill2->Close();
  if( qBill3->Active ) qBill3->Close();
  if( qAddCh->Active ) qAddCh->Close();
  if( qAddChSum->Active ) qAddChSum->Close();
  if( qTotalAdd->Active ) qTotalAdd->Close();
  if( qTotalPayments->Active ) qTotalPayments->Close();

  if( RxMemoryData_AddCh->Active )
   {
     RxMemoryData_AddCh->Close();
     RxMemoryData_AddCh->EmptyTable();
   }
  if( RxMemoryData_Summ->Active )
   {
     RxMemoryData_Summ->Close();
     RxMemoryData_Summ->EmptyTable();
   }
 }

void __fastcall TQuickReport_Ordinal::QuickReport_OrdinalAfterPreview(
      TObject *Sender)
{
  m_Busy = false;
  CloseDSets();
}
//---------------------------------------------------------------------------


void __fastcall TQuickReport_Ordinal::qStatusCalcFields(TDataSet *DataSet)
{
  AnsiString c1, c2, c3;

  c1 = qStatusCITY->AsString; c2 = qStatusSTATE->AsString;
  c3 = qStatusPOST->AsString;
  qStatusCPSS2->Value = Variant(c1 + ", " + c2 + ", " + c3);
    //qStatusCSP->AsString = "kkk";
    //qStatusCPSS2->AsString = "kkk";
}
//---------------------------------------------------------------------------

