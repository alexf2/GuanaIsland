//----------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <set>
#include <list>
using namespace std;
#pragma hdrstop

#include "main_dmod.h"
#include "dm1.h"
#include "rpt_sum.h"
#include "guana2_mf.h"
//----------------------------------------------------------------------------
#pragma link "RxMemDS"
#pragma resource "*.dfm"
TQuickReport_Sum *QuickReport_Sum;

void TQuickReport_Sum::SetD1( Variant vvt )
 {
   m_vD1 = vvt;
 }
void TQuickReport_Sum::SetD2( Variant vvt )
 {
   m_vD2 = vvt;
 }
void TQuickReport_Sum::SetCat( Variant vvt )
 {
   m_vCat = vvt;
   auto_ptr<TQuery> apQ( new TQuery(NULL) );
   apQ->DatabaseName = MainDmod->DB1->DatabaseName;
   apQ->SQL->Add(
     Format( "SELECT [NAMECAT], [ACCT_NO] FROM CategoryGuest where [ID_CAT]=%u", ARRAYOFCONST(((int)vvt)) )
   );
   apQ->Open();
   apQ->First();

   m_vtNameCat = apQ->FieldByName( "NAMECAT" )->Value;
   m_vtAcctNo = apQ->FieldByName( "ACCT_NO" )->Value;

   apQ->Close();
 }

void TQuickReport_Sum::AssField( TField* pF, char *lpName, TDBDataSet *pD )
 {
   Variant vvt; vvt.ChangeType( varNull );
   pF->Value = pD->Eof ? vvt:pD->FieldByName(lpName)->Value;
 }

void TQuickReport_Sum::CloseDSets()
 {
  //if( qStatus->Active ) qStatus->Close();

  if( RxMemoryData_Master->Active )
   {
     RxMemoryData_Master->Close();
     RxMemoryData_Master->EmptyTable();
   }
  if( RxMemoryData_Tbl->Active )
   {
     RxMemoryData_Tbl->Close();
     RxMemoryData_Tbl->EmptyTable();
   }
  if( RxMemoryData_Summ->Active )
   {
     RxMemoryData_Summ->Close();
     RxMemoryData_Summ->EmptyTable();
   }
 }

//----------------------------------------------------------------------------
__fastcall TQuickReport_Sum::TQuickReport_Sum(TComponent* Owner)
    : TQuickRep(Owner)
{
  m_Busy = false;
}
//----------------------------------------------------------------------------
void __fastcall TQuickReport_Sum::QuickReport_SumAfterPreview(
      TObject *Sender)
{
  m_Busy = false;
  CloseDSets();
  EraseAllDynamic();
}
struct TCatItem
 {
   TCatItem(): cSum( 0 )
    {
    }
   TCatItem( const TCatItem& r )
    {
      this->operator=( r );
    }
   TCatItem& operator=( const TCatItem& r )
    {
      id = r.id; sName = r.sName; acct = r.acct; cSum = r.cSum;
    }
   bool operator<( const TCatItem& r )
    {
      return sName < r.sName;
    }
   /*bool operator==( const TCatItem& r )
    {
      return id == r.id;
    }*/
   int id, acct;
   AnsiString sName;
   Currency cSum;
 };
inline bool operator<( const TCatItem& r, const TCatItem& r2 )
 {
   return r.sName < r2.sName;
 }
inline bool operator==( const TCatItem& r, const TCatItem& r2 )
 {
   return r.id == r2.id;
 }
//---------------------------------------------------------------------------
void __fastcall TQuickReport_Sum::QuickReport_SumBeforePrint(
      TQuickRep *Sender, bool &PrintReport)
{
  RxMemoryData_Master->Active = true;
  RxMemoryData_Master->EmptyTable();
  RxMemoryData_Master->Append();
  RxMemoryData_MasterMASTER->AsInteger = 1;
  RxMemoryData_Master->Post();
  RxMemoryData_Master->First();

  QRLabel_Ttl1->Caption =
    Format( "GUANA ISLAND %s REGISTER", ARRAYOFCONST((((AnsiString)m_vtNameCat).c_str())) );
  QRLabel_Acct->Caption =
    Format( "ACCT. NO. %u", ARRAYOFCONST(((int)m_vtAcctNo)) );
  QRLabel_Date->Caption =
    Format( "%s - %s", ARRAYOFCONST(( ((TDateTime)m_vD1).FormatString("MMMM/d/yyyy").c_str(),
      ((TDateTime)m_vD2).FormatString("MMMM/d/yyyy").c_str() )) );
  QRLabel_Foother->Caption =
    Format( "%s Register", ARRAYOFCONST((((AnsiString)m_vtNameCat).c_str())) );


  qSelectBill->ParamByName( "date1" )->Value  = m_vD1;
  qSelectBill->ParamByName( "date2" )->Value  = m_vD2;
  qSelectBill->ParamByName( "id_cat" )->Value = m_vCat;
  qSelectBill->Prepared = true;
  qSelectBill->Open(); qSelectBill->First();

  int iNO_OF_GUESTS = 0;
  int iSUM_NIGHTS = 0;
  Currency cSUM_ROOMS = 0, cSUM_DEPOSIT = 0, cSUM_BALANCE = 0, cSUM_PAYMENTS = 0,
           cSUM_TOTAL = 0;

  set<TCatItem> setCat;

  auto_ptr<TQuery> apQCat( new TQuery(NULL) );
  apQCat->DatabaseName = MainDmod->DB1->DatabaseName;

  apQCat->SQL->Add( "SELECT distinct a.ID_BILL as [BNN], b.NAME as [BNAME], b.ACCT_NO as [ACCT] "
    "FROM AdditionalCharge a INNER JOIN BillCategory b ON a.ID_BILL=b.ID_BILL "
    "WHERE BILL_NO=:bill_no" );

  //apQCat->Params->AddParam( apQCat->Params->CreateParam(ftInteger, "bill_no", ptInput) );
  //TParam *pParamBN = new TParam( apQCat->Params, ptInput );
  TParam *pParamBN = apQCat->Params->ParamByName( "bill_no" );
  pParamBN->DataType = ftInteger; pParamBN->ParamType = ptInput;

  apQCat->Prepared = true;

  while( !qSelectBill->Eof )
   {
     pParamBN->Value = qSelectBillBILL_NO->Value;
     apQCat->Open(); apQCat->First();
     while( !apQCat->Eof )
      {
        TCatItem it;
        it.id = apQCat->Fields[0]->AsInteger;
        it.acct = apQCat->Fields[2]->AsInteger;
        it.sName = apQCat->Fields[1]->AsString;

        setCat.insert( it );

        apQCat->Next();
      }
     apQCat->Close();

     qSelectBill->Next();
   }

  RxMemoryData_Tbl->Active = false;
  RxMemoryData_Summ->Active = false;

  list<TCatItem> lstCat;
  copy( setCat.begin(), setCat.end(), inserter(lstCat, lstCat.begin()) );
  setCat.clear();
  lstCat.sort();
  /*list<TCatItem>::iterator i1( lstCat.begin() );
  list<TCatItem>::iterator i2( lstCat.end() );
  AnsiString ss;
  for( ; i1 != i2; ++i1 )
    ss += (*i1).sName + AnsiString(", ");
  MessageBox( NULL, ss.c_str(), "Categoryes", MB_OK );*/

  EraseAllDynamic();

  AssSysAcc();

  RxMemoryData_Tbl->Active = false;
  RxMemoryData_Summ->Active = false;

  list<TCatItem>::iterator i1( lstCat.begin() );
  list<TCatItem>::iterator i2( lstCat.end() );
  int iLeft = QRLabel_Total->Left + QRLabel_Total->Width + 3;
  int iRight = QRLabel_Total->Left + 2*QRLabel_Total->Width;
  //int iLeftP = QRLabel_Total->Size.Left + QRLabel_Total->Size.Width + 1;
  for( int i = 0; i1 != i2; ++i1, iLeft += QRLabel_Total->Width,
       iRight += QRLabel_Total->Width, ++i )
   { //QRLabel_Total
     TQRLabel* pL = new TQRLabel( QuickReport_Sum );
     pL->Parent = QRBand_Hdr;
     pL->Alignment = taRightJustify;
     pL->AutoSize = false; pL->AutoStretch = false;
     pL->Caption = (*i1).sName;
     //Format( "%s\n%u", ARRAYOFCONST(((*i1).sName.c_str(), (*i1).acct)) );
     pL->Font = QRLabel_Total->Font;
     pL->Height = QRLabel_Total->Height; pL->Width = QRLabel_Total->Width;
     pL->Top = QRLabel_Total->Top;
     pL->Left = iRight - pL->Width; pL->WordWrap = true; pL->Enabled = true;
     pL->Tag = 99;

     pL = new TQRLabel( QuickReport_Sum );
     pL->Parent = QRBand_Hdr;
     pL->Alignment = taCenter;
     pL->AutoSize = false; pL->AutoStretch = false;
     pL->Caption = AnsiString( (*i1).acct );
     //Format( "%s\n%u", ARRAYOFCONST(((*i1).sName.c_str(), (*i1).acct)) );
     pL->Font = QRLabel_Payment->Font;
     pL->Height = QRLabel_Payment->Height; pL->Width = QRLabel_Payment->Width;
     pL->Top = QRLabel_Payment->Top;
     pL->Left = iRight - pL->Width; pL->WordWrap = true; pL->Enabled = true;
     pL->Tag = 99;

     TQRDBText* pT = new TQRDBText( QuickReport_Sum );
     pT->Parent = QRSubDetail_Content;
     pT->Alignment = taRightJustify;
     pT->AutoSize = true; pT->AutoStretch = false;
     //pT->Caption = AnsiString( (*i1).acct );
     //Format( "%s\n%u", ARRAYOFCONST(((*i1).sName.c_str(), (*i1).acct)) );
     pT->Font = QRDBText_Payment->Font;
     pT->Height = QRDBText_Payment->Height; pT->Width = QRLabel_Payment->Width;
     pT->Top = QRDBText_Payment->Top;
     pT->Left = iRight - pT->Width; pT->WordWrap = true; pT->Enabled = true;
     pT->Tag = 99;
     pT->DataSet = RxMemoryData_Tbl;
     pT->DataField = Format( "F##%u", ARRAYOFCONST((i)) );

     pT = new TQRDBText( QuickReport_Sum );
     pT->Parent = QRBand_Fother;
     pT->Alignment = taRightJustify;
     pT->AutoSize = true; pT->AutoStretch = false;
     //pT->Caption = AnsiString( (*i1).acct );
     //Format( "%s\n%u", ARRAYOFCONST(((*i1).sName.c_str(), (*i1).acct)) );
     pT->Font = QRDBText_Total2->Font;
     pT->Height = QRDBText_Total2->Height; pT->Width = QRDBText_Total2->Width;
     pT->Top = QRDBText_Total2->Top;
     pT->Left = iRight - pT->Width; pT->WordWrap = true; pT->Enabled = true;
     pT->Tag = 99;
     pT->DataSet = RxMemoryData_Summ;
     pT->DataField = Format( "FS##%u", ARRAYOFCONST((i)) );



     TFieldDef *pFD = RxMemoryData_Tbl->FieldDefs->AddFieldDef();
     pFD->DataType = ftCurrency;
     pFD->Name = Format( "F##%u", ARRAYOFCONST((i)) );
     pFD->Precision = 15;
     pFD->Required = false;

     //TCurrencyField *pCF = dynamic_cast<TCurrencyField*>(RxMemoryData_Tbl->FieldByName( pFD->Name ));
     //pCF->DisplayFormat = "#,##0.00;(#,##0.00)";

     TCurrencyField *pCF = new TCurrencyField( RxMemoryData_Tbl );
     pCF->Precision = 15;
     pCF->DisplayFormat = "#,##0.00;(#,##0.00)";
     pCF->Alignment = taRightJustify;
     pCF->Calculated = false;
     //pCF->Currency = true;
     //pCF->AsCurrency = 18.25;
     pCF->DisplayWidth = 10;
     pCF->FieldKind = fkData;
     pCF->FieldName = Format( "F##%u", ARRAYOFCONST((i)) );
     pCF->Tag = 101;
     pCF->DataSet = RxMemoryData_Tbl;
     //pCF->Size =
     //RxMemoryData_Tbl->Fields->Items[ RxMemoryData_Tbl->Fields->Count - 1 ] = pCF;

     pFD = RxMemoryData_Summ->FieldDefs->AddFieldDef();
     pFD->DataType = ftCurrency;
     pFD->Name = Format( "FS##%u", ARRAYOFCONST((i)) );
     pFD->Precision = 15;
     pFD->Required = false;

     //TCurrencyField *pCF = dynamic_cast<TCurrencyField*>(RxMemoryData_Tbl->FieldByName( pFD->Name ));
     //pCF->DisplayFormat = "#,##0.00;(#,##0.00)";

     pCF = new TCurrencyField( RxMemoryData_Summ );
     pCF->Precision = 15;
     pCF->DisplayFormat = "#,##0.00;(#,##0.00)";
     pCF->Alignment = taRightJustify;
     pCF->Calculated = false;
     //pCF->Currency = true;
     //pCF->AsCurrency = 18.25;
     pCF->DisplayWidth = 10;
     pCF->FieldKind = fkData;
     pCF->FieldName = Format( "FS##%u", ARRAYOFCONST((i)) );
     pCF->Tag = 101;
     pCF->DataSet = RxMemoryData_Summ;
   }

  //RxMemoryData_Tbl->FieldDefs->Update();
  //RxMemoryData_Summ->FieldDefs->Update();

  RxMemoryData_Tbl->Active = true;
  RxMemoryData_Tbl->EmptyTable();
  RxMemoryData_Summ->Active = true;
  RxMemoryData_Summ->EmptyTable();
  qSelectBill->First();

  /*RxMemoryData_Tbl->Edit();
  RxMemoryData_Tbl->FieldByName( "F##0" )->AsCurrency = 8.25;
  RxMemoryData_Tbl->Post();*/

  auto_ptr<TQuery> apQDynCat( new TQuery(NULL) );
  apQDynCat->DatabaseName = MainDmod->DB1->DatabaseName;

  apQDynCat->SQL->Add( "select sum([AMOUNT]) from AdditionalCharge where [BILL_NO]=:bill_no and [ID_BILL]=:id_bill" );

  TParam *pParamDyn_BillNo = apQDynCat->Params->ParamByName( "bill_no" );
  TParam *pParamDyn_IdBill = apQDynCat->Params->ParamByName( "id_bill" );
  pParamDyn_BillNo->DataType = ftInteger; pParamDyn_BillNo->ParamType = ptInput;
  pParamDyn_IdBill->DataType = ftInteger; pParamDyn_IdBill->ParamType = ptInput;

  while( !qSelectBill->Eof )
   {
     RxMemoryData_Tbl->Append();

     RxMemoryData_TblFOLIO->Value = qSelectBillFOLIO->Value;
     RxMemoryData_TblGUEST_NAME->Value = qSelectBillLFNAME->Value;
     RxMemoryData_TblNO_OF_GUESTS->Value = qSelectBillSUM_GUESTS->Value;
     RxMemoryData_TblNIGHTS->Value = qSelectBillSUM_NIGHTS->Value;
     RxMemoryData_TblROOMS_PAY->Value = qSelectBillROOMSPAY->Value;
     RxMemoryData_TblTOTAL->Value = qSelectBillTOTAL->Value;
     RxMemoryData_TblDEPOSIT->Value = qSelectBill_DEPOSIT_RECEIVD->Value;
     RxMemoryData_TblBALANCE_DUE->Value = qSelectBill_BALANCE_DUE->Value;
     RxMemoryData_TblPAYMENTS->Value = qSelectBillSUM_PAYMENTS->Value;

     iNO_OF_GUESTS += qSelectBillSUM_GUESTS->AsInteger;
     iSUM_NIGHTS += qSelectBillSUM_NIGHTS->AsInteger;
     cSUM_ROOMS += qSelectBillROOMSPAY->AsCurrency;
     cSUM_DEPOSIT += qSelectBill_DEPOSIT_RECEIVD->AsCurrency;
     cSUM_BALANCE += qSelectBill_BALANCE_DUE->AsCurrency;
     cSUM_PAYMENTS += qSelectBillSUM_PAYMENTS->AsCurrency;
     cSUM_TOTAL += qSelectBillTOTAL->AsCurrency;

     i1 = lstCat.begin();
     i2 = lstCat.end();
     apQDynCat->Params->Items[ 0 ]->Value = qSelectBillBILL_NO->Value;
     for( int i = 0; i1 != i2; ++i1, ++i )
      {
         apQDynCat->Params->Items[ 1 ]->Value = (*i1).id;
         apQDynCat->Open();
         RxMemoryData_Tbl->FieldByName( Format( "F##%u", ARRAYOFCONST((i)) ) )->Value =
            apQDynCat->Fields[ 0 ]->Value;
         (*i1).cSum += apQDynCat->Fields[ 0 ]->AsCurrency;
         apQDynCat->Close();
      }

     RxMemoryData_Tbl->Post();

     qSelectBill->Next();
   }

  qSelectBill->Close();

  RxMemoryData_Tbl->First();
  RxMemoryData_Summ->First();

  RxMemoryData_Summ->Append();

  RxMemoryData_SummNO_OF_GUESTS->AsInteger = iNO_OF_GUESTS;
  RxMemoryData_SummSUM_NIGHTS->AsInteger = iSUM_NIGHTS;
  RxMemoryData_SummSUM_ROOMS->AsCurrency = cSUM_ROOMS;
  RxMemoryData_SummSUM_DEPOSIT->AsCurrency = cSUM_DEPOSIT;
  RxMemoryData_SummSUM_BALANCE->AsCurrency = cSUM_BALANCE;
  RxMemoryData_SummSUM_PAYMENTS->AsCurrency = cSUM_PAYMENTS;
  RxMemoryData_SummSUM_TOTAL->AsCurrency = cSUM_TOTAL;

  i1 = lstCat.begin();
  i2 = lstCat.end();
  for( int i = 0; i1 != i2; ++i1, ++i )
   {
     //Currency cc = (*i1).cSum;
     RxMemoryData_Summ->FieldByName( Format( "FS##%u", ARRAYOFCONST((i)) ) )->AsCurrency = (*i1).cSum;
   }


  RxMemoryData_Summ->Post();
  RxMemoryData_Summ->First();



}
void TQuickReport_Sum::AssSysAcc()
 {
   auto_ptr<TQuery> apQCat( new TQuery(NULL) );
   apQCat->DatabaseName = MainDmod->DB1->DatabaseName;
   apQCat->SQL->Add( "select [ACCT_NO] from BillCategory where [NAME]=:name" );

   TParam *pParamBN = apQCat->Params->ParamByName( "name" );
   pParamBN->DataType = ftString; pParamBN->ParamType = ptInput;
   apQCat->Prepared = true;

   pParamBN->AsString = "w[SYS]Room&Board";
   apQCat->Open();
   QRLabel_Rooms->Caption = apQCat->FieldByName( "ACCT_NO" )->AsString;
   apQCat->Close();

   pParamBN->AsString = "w[SYS]Deposit";
   apQCat->Open();
   QRLabel_Deposit->Caption = apQCat->FieldByName( "ACCT_NO" )->AsString;
   apQCat->Close();

   pParamBN->AsString = "w[SYS]Payment";
   apQCat->Open();
   QRLabel_Payment->Caption = apQCat->FieldByName( "ACCT_NO" )->AsString;
   apQCat->Close();
 }
typedef auto_ptr<TComponent> TAP_Component;
typedef auto_ptr<TFieldDef> TAP_Fld;
void TQuickReport_Sum::EraseAllDynamic()
 {
   list<TAP_Component> lst;
   list<TAP_Fld> lst2;
   for( int i = ComponentCount - 1; i >= 0; --i )
     if( Components[i]->Tag == 99 )
      {
        lst.push_back( TAP_Component(Components[i]) );
        RemoveComponent( Components[i] );
      }
   for( int i = RxMemoryData_Tbl->ComponentCount - 1; i >= 0; --i )
     if( RxMemoryData_Tbl->Components[i]->Tag == 101 )
      {
        lst.push_back( TAP_Component(RxMemoryData_Tbl->Components[i]) );
        RxMemoryData_Tbl->RemoveComponent( RxMemoryData_Tbl->Components[i] );
      }
   for( int i = RxMemoryData_Summ->ComponentCount - 1; i >= 0; --i )
     if( RxMemoryData_Summ->Components[i]->Tag == 101 )
      {
        lst.push_back( TAP_Component(RxMemoryData_Summ->Components[i]) );
        RxMemoryData_Summ->RemoveComponent( RxMemoryData_Summ->Components[i] );
      }

   for( int i = RxMemoryData_Tbl->FieldDefs->Count - 1; i >= 0; --i )
     if( RxMemoryData_Tbl->FieldDefs->Items[i]->Name.Pos("##") != 0 )
       lst2.push_back( TAP_Fld(RxMemoryData_Tbl->FieldDefs->Items[i]) );

   for( int i = RxMemoryData_Summ->FieldDefs->Count - 1; i >= 0; --i )
     if( RxMemoryData_Summ->FieldDefs->Items[i]->Name.Pos("##") != 0 )
       lst2.push_back( TAP_Fld(RxMemoryData_Summ->FieldDefs->Items[i]) );


 }
//---------------------------------------------------------------------------
void __fastcall TQuickReport_Sum::qSelectBillCalcFields(TDataSet *DataSet)
{
  qSelectBillLFNAME->AsString = qSelectBillFNAME->AsString + " " + qSelectBillLNAME->AsString;

  qCalc_BD->Close();
  if( qCalc_BD->Prepared == false ) qCalc_BD->Prepared = true;
  qCalc_BD->ParamByName( "bill_no" )->Value = qSelectBillBILL_NO->Value;
  qCalc_BD->Open();

  qSelectBillSUM_GUESTS->AsInteger = qCalc_BD->FieldByName( "SUM_GUESTS" )->AsInteger;
  qSelectBillSUM_NIGHTS->AsInteger = qCalc_BD->FieldByName( "SUM_NIGHTS" )->AsInteger;
  qSelectBillROOMSPAY->AsCurrency = qCalc_BD->FieldByName( "ROOMSPAY" )->AsCurrency;

  qCalc_BD->Close();

  qSumPayments->Close();
  if( qSumPayments->Prepared == false ) qSumPayments->Prepared = true;
  qSumPayments->ParamByName( "bill_no" )->Value = qSelectBillBILL_NO->Value;
  qSumPayments->Open();

  qSelectBillSUM_PAYMENTS->AsCurrency = qSumPayments->FieldByName( "SUM_PAYMENTS" )->AsCurrency;

  qSumPayments->Close();


  qSumAdd->Close();
  if( qSumAdd->Prepared == false ) qSumAdd->Prepared = true;
  qSumAdd->ParamByName( "bill_no" )->Value = qSelectBillBILL_NO->Value;
  qSumAdd->Open();

  Currency cSubTotal = qSelectBillROOMSPAY->AsCurrency +
    qSumAdd->FieldByName( "SUM_ADD" )->AsCurrency;
  qSelectBillTOTAL->AsCurrency = cSubTotal + cSubTotal * 17.0 / 100.0 -
    qSelectBillSUM_PAYMENTS->AsCurrency;

  qSumAdd->Close();
}
//---------------------------------------------------------------------------
