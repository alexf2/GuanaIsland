//---------------------------------------------------------------------------
#include <vcl.h>
#include <bdeutils.hpp>
#pragma hdrstop

#include "helper.h"
#include "main_dmod.h"
#include "dm1.h"
#include "dm2.h"
#include "rpt_ord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TFHelper *FHelper = NULL;

void TFHelper::MakePay( bool bFlEnrol )
 {
   MainDmod->DB1->StartTransaction();

   try {
     DMod1->tbBill_BALANCE_DUE->ReadOnly = false;
     DMod1->tbBill_DEPOSIT_RECEIVD->ReadOnly = false;

     DMod1->tbGuest->Edit(), DMod1->tbBill->Edit();

     if( bFlEnrol )
      {
        if( DMod1->tbGuestDEPOSIT->AsCurrency < DMod1->tbBill_TOTAL->AsCurrency )
         {
           DMod1->tbBill_BALANCE_DUE->AsCurrency = DMod1->tbBill_TOTAL->AsCurrency - DMod1->tbGuestDEPOSIT->AsCurrency;
           DMod1->tbGuestDEPOSIT->AsCurrency = 0;
         }
        else
         {
           DMod1->tbBill_BALANCE_DUE->AsCurrency = 0;
           DMod1->tbGuestDEPOSIT->AsCurrency = DMod1->tbGuestDEPOSIT->AsCurrency - DMod1->tbBill_TOTAL->AsCurrency;
         }

        DMod1->tbBill_DEPOSIT_RECEIVD->AsCurrency = DMod1->tbBill_TOTAL->AsCurrency - DMod1->tbBill_BALANCE_DUE->AsCurrency;
      }
     else
      {
        DMod1->tbGuestDEPOSIT->AsCurrency = DMod1->tbGuestDEPOSIT->AsCurrency + DMod1->tbBill_DEPOSIT_RECEIVD->AsCurrency;
        DMod1->tbBill_DEPOSIT_RECEIVD->AsCurrency = 0;
        DMod1->tbBill_BALANCE_DUE->AsCurrency = 0;
      }

     DMod1->tbBill->Post();
     DMod1->tbGuest->Post(); 
    }
   catch( Exception *e )
    {
      try {
        DMod1->tbBill->Cancel();
        DMod1->tbGuest->Cancel();
       }
      catch( Exception* )
       {
       }

      DMod1->tbBill_BALANCE_DUE->ReadOnly = true;
      DMod1->tbBill_DEPOSIT_RECEIVD->ReadOnly = true;

      MainDmod->DB1->Rollback();

      try {
        DMod1->tbGuest->Refresh();
        DMod1->tbBill->Refresh();
       }
      catch( Exception* )
       {
       }

      MessageBox( Application->MainForm->Handle, e->Message.c_str(),
        "Error", MB_OK|MB_ICONSTOP );
      return;
    }

   DMod1->tbBill_BALANCE_DUE->ReadOnly = true;
   DMod1->tbBill_DEPOSIT_RECEIVD->ReadOnly = true;

   MainDmod->DB1->Commit();
 }

void TFHelper::CheckState()
 {
   if( DMod1 )
    {
      bool bFl = !(DMod1->srGuest->DataSet->Bof && DMod1->srGuest->DataSet->Eof);
      if( BitBtn_RemoveGuest->Enabled != bFl ) BitBtn_RemoveGuest->Enabled = bFl;

      bFl = !(DMod1->srGuest->DataSet->State == dsEdit ||
              DMod1->srGuest->DataSet->State == dsInsert ||
              DMod1->srBill->DataSet->State == dsInsert);
      if( BitBtn_NewGuest->Enabled != bFl ) BitBtn_NewGuest->Enabled = bFl;

      bFl = !(DMod1->srBill->DataSet->Bof && DMod1->srBill->DataSet->Eof);
      if( BitBtn_RemoveBill->Enabled != bFl ) BitBtn_RemoveBill->Enabled = bFl;

      bFl = !(DMod1->srBill->DataSet->State == dsEdit ||
              DMod1->srGuest->DataSet->State == dsInsert ||
              DMod1->srBill->DataSet->State == dsInsert);
      if( BitBtn_NewBill->Enabled != bFl ) BitBtn_NewBill->Enabled = bFl;

      bFl = !(DMod1->srBill->DataSet->State != dsBrowse ||
              DMod1->srGuest->DataSet->State != dsBrowse ||
              DMod1->srBill->DataSet->Bof && DMod1->srBill->DataSet->Eof);
      if( bFl )
       {
         bool bb = DMod1->tbBill_DEPOSIT_RECEIVD->AsCurrency == 0 &&
           DMod1->tbBill_BALANCE_DUE->AsCurrency == 0;
         BitBtn_PayEnrol->Enabled = bb;
         BitBtn_MoneyBack->Enabled = !bb;
       }
      else
        BitBtn_PayEnrol->Enabled = bFl,
        BitBtn_MoneyBack->Enabled = bFl;
    }
 }

void TFHelper::MakeBind( TWinControl* p, TE_Srcs src )
 {
   BndCtrl = p;
   //if( p && p->Visible == false )
   PageControl1->ActivePage = PageControl1->Pages[ src == SR_Bill ? 2:0 ];
 }

void __fastcall TFHelper::Loaded(void)
{
  TForm::Loaded();
  Visible = False;
  HandleNeeded();
}

void __fastcall TFHelper::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams( Params );
  if (dynamic_cast<TForm*>(Owner))
    Params.WndParent = dynamic_cast<TForm*>(Owner)->Handle;
  else
    ShowMessage("No Cast in ChildForm CreateParams");
  Params.Style &= ~WS_CHILD;
}


//---------------------------------------------------------------------------
__fastcall TFHelper::TFHelper(TComponent* Owner)
    : TForm(Owner), m_pAct( NULL )
{
  //m_apLdbGuest = auto_ptr<TDBLocate>( new TDBLocate() );
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_NewGuestClick(TObject *Sender)
{
  DMod1->srGuest->DataSet->Insert();
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_RemoveGuestClick(TObject *Sender)
{
  DMod1->srGuest->DataSet->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_NewBillClick(TObject *Sender)
{
  DMod1->srBill->DataSet->Insert();
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_RemoveBillClick(TObject *Sender)
{
  DMod1->srBill->DataSet->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_PayEnrolClick(TObject *Sender)
{
  MakePay( true );
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_MoneyBackClick(TObject *Sender)
{
  MakePay( false );
}
//---------------------------------------------------------------------------
void __fastcall TFHelper::BitBtn_GoGuestClick(TObject *Sender)
{
  if( DMod1->qFindGuest->Active ) DMod1->qFindGuest->Close();

  DMod1->qFindGuest->ParamByName( "lname" )->AsString = Edit_LName->Text;
  DMod1->qFindGuest->ParamByName( "fname" )->AsString = Edit_FName->Text;

  if( !DMod1->qFindGuest->Prepared ) DMod1->qFindGuest->Prepared = true;
  DMod1->qFindGuest->Open();

  if( DMod1->qFindGuest->Eof &&  DMod1->qFindGuest->Bof )
   {
     MessageBox( Application->MainForm->Handle, "The Guest not found",
        "Information", MB_OK|MB_ICONINFORMATION );
     return;
   }

  DMod1->qFindGuest->Next();

  auto_ptr<TDBLocate> ap( new TDBLocate() );

  ap->DataSet = DMod1->tbGuest;
  ap->IndexSwitch = false;

  ap->Locate( "ID_GUEST", DMod1->qFindGuest->FieldByName("ID_GUEST")->Value, true, false );
  DMod1->qFindGuest->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFHelper::BitBtn_GoBillClick(TObject *Sender)
{

  int id_guest, bill_no;
  if( RadioButton_No->Checked )
   {
      if( DMod1->qFndBillNo->Active ) DMod1->qFndBillNo->Close();

      char *pp;
      DMod1->qFndBillNo->ParamByName( "bill_no" )->AsInteger =
       strtoul( Edit_BillNo->Text.c_str(), &pp, 10 );

      if( !DMod1->qFndBillNo->Prepared ) DMod1->qFndBillNo->Prepared = true;
      DMod1->qFndBillNo->Open();

      if( DMod1->qFndBillNo->Eof &&  DMod1->qFndBillNo->Bof )
       {
         MessageBox( Application->MainForm->Handle, "The Bill not found",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qFndBillNo->Next();

      bill_no = strtoul( Edit_BillNo->Text.c_str(), &pp, 10 );
      id_guest = DMod1->qFndBillNo->FieldByName("ID_GUEST")->AsInteger;
   }
  else
   {
      if( DMod1->qFndFolio->Active ) DMod1->qFndFolio->Close();

      DMod1->qFndFolio->ParamByName( "folio" )->AsString = Edit_Folio->Text;

      if( !DMod1->qFndFolio->Prepared ) DMod1->qFndFolio->Prepared = true;
      DMod1->qFndFolio->Open();

      if( DMod1->qFndFolio->Eof &&  DMod1->qFndFolio->Bof )
       {
         MessageBox( Application->MainForm->Handle, "The Bill not found",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qFndFolio->Next();

      bill_no = DMod1->qFndFolio->FieldByName("BILL_NO")->AsInteger;
      id_guest = DMod1->qFndFolio->FieldByName("ID_GUEST")->AsInteger;
   }


  auto_ptr<TDBLocate> ap( new TDBLocate() );

  ap->DataSet = DMod1->tbGuest;
  ap->IndexSwitch = false;

  ap->Locate( "ID_GUEST", id_guest, true, false );

  auto_ptr<TDBLocate> ap2( new TDBLocate() );

  ap2->DataSet = DMod1->tbBill;
  ap2->IndexSwitch = false;

  ap2->Locate( "BILL_NO", bill_no, true, false );


  DMod1->qFndFolio->Close();
  DMod1->qFndBillNo->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFHelper::BitBtn_LastTimeClick(TObject *Sender)
{
  int id_guest, bill_no;
  if( !RadioButton_Current->Checked )
   {
      if( DMod1->qTimeGuest->Active ) DMod1->qTimeGuest->Close();

      if( !DMod1->qTimeGuest->Prepared ) DMod1->qTimeGuest->Prepared = true;
      DMod1->qTimeGuest->Open();

      if( DMod1->qTimeGuest->Eof &&  DMod1->qTimeGuest->Bof )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }
      DMod1->qTimeGuest->Next();

      if( DMod1->qTimeGuest2->Active ) DMod1->qTimeGuest2->Close();

      if( DMod1->qTimeGuest->FieldByName("MDAT")->IsNull )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }
      DMod1->qTimeGuest2->ParamByName( "date_" )->Value = DMod1->qTimeGuest->FieldByName("MDAT")->Value;

      if( !DMod1->qTimeGuest2->Prepared ) DMod1->qTimeGuest2->Prepared = true;
      DMod1->qTimeGuest2->Open();

      if( DMod1->qTimeGuest2->Eof &&  DMod1->qTimeGuest2->Bof )
       {
         MessageBox( Application->MainForm->Handle, "No bills for this date",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qTimeGuest2->Next();

      bill_no = DMod1->qTimeGuest2->FieldByName("BILL_NO")->AsInteger;
      id_guest = DMod1->qTimeGuest2->FieldByName("ID_GUEST")->AsInteger;
   }
  else
   {
   //qTimeBill
    //
     if( DMod1->srGuest->DataSet->Bof && DMod1->srGuest->DataSet->Eof )
      {
        MessageBox( Application->MainForm->Handle, "For this action needing current guest",
            "Warning", MB_OK|MB_ICONEXCLAMATION );
         return;
      }

     if( DMod1->qTimeBill->Active ) DMod1->qTimeBill->Close();

      DMod1->qTimeBill->ParamByName( "id_guest" )->Value = DMod1->tbGuestID_GUEST->Value;
      if( !DMod1->qTimeBill->Prepared ) DMod1->qTimeBill->Prepared = true;
      DMod1->qTimeBill->Open();

      if( DMod1->qTimeBill->Eof &&  DMod1->qTimeBill->Bof )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qTimeBill->Next();

      if( DMod1->qTimeBill2->Active ) DMod1->qTimeBill2->Close();

      if( DMod1->qTimeBill->FieldByName("MDAT")->IsNull )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qTimeBill2->ParamByName( "date_" )->Value = DMod1->qTimeBill->FieldByName("MDAT")->Value;
      DMod1->qTimeBill2->ParamByName( "id_guest" )->Value = DMod1->tbGuestID_GUEST->Value;

      if( !DMod1->qTimeBill2->Prepared ) DMod1->qTimeBill2->Prepared = true;
      DMod1->qTimeBill2->Open();

      if( DMod1->qTimeBill2->Eof &&  DMod1->qTimeBill2->Bof )
       {
         MessageBox( Application->MainForm->Handle, "No bills for this date",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qTimeBill2->Next();

      bill_no = DMod1->qTimeBill2->FieldByName("BILL_NO")->AsInteger;
      id_guest = DMod1->qTimeBill2->FieldByName("ID_GUEST")->AsInteger;
   }

  auto_ptr<TDBLocate> ap( new TDBLocate() );

  ap->DataSet = DMod1->tbGuest;
  ap->IndexSwitch = false;

  ap->Locate( "ID_GUEST", id_guest, true, false );

  auto_ptr<TDBLocate> ap2( new TDBLocate() );

  ap2->DataSet = DMod1->tbBill;
  ap2->IndexSwitch = false;

  ap2->Locate( "BILL_NO", bill_no, true, false );


  DMod1->qTimeGuest2->Close();
  DMod1->qTimeGuest->Close();
  DMod1->qTimeBill->Close();
  DMod1->qTimeBill2->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFHelper::BitBtn_LastNoClick(TObject *Sender)
{
  int id_guest, bill_no;
  if( !RadioButton_Current->Checked )
   {
      if( DMod1->qBillGuest->Active ) DMod1->qBillGuest->Close();

      if( !DMod1->qBillGuest->Prepared ) DMod1->qBillGuest->Prepared = true;
      DMod1->qBillGuest->Open();

      if( DMod1->qBillGuest->Eof &&  DMod1->qBillGuest->Bof )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }
      DMod1->qBillGuest->Next();

      if( DMod1->qBillGuest2->Active ) DMod1->qBillGuest2->Close();

      if( DMod1->qBillGuest->FieldByName("MBILL")->IsNull )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }
      DMod1->qBillGuest2->ParamByName( "bill_no" )->Value = DMod1->qBillGuest->FieldByName("MBILL")->Value;

      if( !DMod1->qBillGuest2->Prepared ) DMod1->qBillGuest2->Prepared = true;
      DMod1->qBillGuest2->Open();

      if( DMod1->qBillGuest2->Eof &&  DMod1->qBillGuest2->Bof )
       {
         MessageBox( Application->MainForm->Handle, "No bills for this date",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qBillGuest2->Next();

      bill_no = DMod1->qBillGuest->FieldByName("MBILL")->AsInteger;
      id_guest = DMod1->qBillGuest2->FieldByName("ID_GUEST")->AsInteger;
   }
  else
   {
   //qTimeBill
    //
     if( DMod1->srGuest->DataSet->Bof && DMod1->srGuest->DataSet->Eof )
      {
        MessageBox( Application->MainForm->Handle, "For this action needing current guest",
            "Warning", MB_OK|MB_ICONEXCLAMATION );
         return;
      }

     if( DMod1->qBillBill->Active ) DMod1->qBillBill->Close();

      DMod1->qBillBill->ParamByName( "id_guest" )->Value = DMod1->tbGuestID_GUEST->Value;
      if( !DMod1->qBillBill->Prepared ) DMod1->qBillBill->Prepared = true;
      DMod1->qBillBill->Open();

      if( DMod1->qBillBill->Eof &&  DMod1->qBillBill->Bof )
       {
         MessageBox( Application->MainForm->Handle, "Bills list is empty",
            "Information", MB_OK|MB_ICONINFORMATION );
         return;
       }

      DMod1->qBillBill->Next();

      bill_no = DMod1->qBillBill->FieldByName("MBILL")->AsInteger;
      id_guest = DMod1->tbGuestID_GUEST->AsInteger;
   }

  auto_ptr<TDBLocate> ap( new TDBLocate() );

  ap->DataSet = DMod1->tbGuest;
  ap->IndexSwitch = false;

  ap->Locate( "ID_GUEST", id_guest, true, false );

  auto_ptr<TDBLocate> ap2( new TDBLocate() );

  ap2->DataSet = DMod1->tbBill;
  ap2->IndexSwitch = false;

  ap2->Locate( "BILL_NO", bill_no, true, false );


  DMod1->qBillBill->Close();
  DMod1->qBillGuest->Close();
  DMod1->qBillGuest2->Close();
}
//---------------------------------------------------------------------------


void __fastcall TFHelper::BitBtn_OrdinalClick(TObject *Sender)
{
  if( DMod1->srBill->DataSet->Bof && DMod1->srBill->DataSet->Eof )
   {
     MessageBox( Application->MainForm->Handle, "For this action needing current bill",
       "Warning", MB_OK|MB_ICONEXCLAMATION );
     return;
   }
  if( QuickReport_Ordinal->m_Busy )
   {
     MessageBox( Application->MainForm->Handle, "Report is running",
       "Error", MB_OK|MB_ICONSTOP );
     return;
   }
  QuickReport_Ordinal->BillNo = DMod1->tbBillBILL_NO->Value;


  try {
    QuickReport_Ordinal->m_Busy = true;
    QuickReport_Ordinal->Preview();
   }
  catch( Exception *e )
   {
     QuickReport_Ordinal->m_Busy = false;
     throw e;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFHelper::Edit_FolioEnter(TObject *Sender)
{
  RadioButton_Folio->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFHelper::Edit_BillNoEnter(TObject *Sender)
{
  RadioButton_No->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFHelper::Edit_FullChange(TObject *Sender)
{
  AnsiString ss = Edit_Full->Text;
  ss.TrimRight(), ss.TrimLeft();
  if( ss == "" ) return;

  /*if( m_apLdbGuest->DataSet == NULL )
    m_apLdbGuest->DataSet = DMod1->tbGuest,
      m_apLdbGuest->IndexSwitch = false;*/

  int iPos;
  AnsiString ss2 = "";
  if( (iPos=ss.Pos(" ")) != 0 )
    ss2 = ss.SubString( iPos + 1, ss.Length() ),
    ss = ss.SubString( 0, iPos - 1 );

  if( ss2.Length() )
   {
      Variant vParams[ 2 ] =
       {
         Variant(ss.c_str()), Variant(ss2.c_str())
       };

      DMod1->tbGuest->Locate( "FNAME;LNAME", VarArrayOf(vParams, 1),
       TLocateOptions() << loPartialKey << loCaseInsensitive );
   }
  else
   {
      DMod1->tbGuest->Locate( "FNAME", Variant(ss.c_str()),
        TLocateOptions() << loPartialKey << loCaseInsensitive );
      //m_apLdbGuest->Locate( "FNAME;LNAME", VarArrayOf(vParams, 1), false, false );
   }
}
//---------------------------------------------------------------------------

