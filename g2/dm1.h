//---------------------------------------------------------------------------
#ifndef dm1H
#define dm1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBTables.hpp>
#include <Db.hpp>
#include <memory>
using namespace std;
//---------------------------------------------------------------------------
class TDMod1 : public TDataModule
{
__published:	// IDE-managed Components
    TDataSource *srRoom;
    TDataSource *srGuestCat;
    TDataSource *srBillCat;
    TTable *tbRoom;
    TAutoIncField *tbRoomID_ROOM;
    TStringField *tbRoomNAME;
    TTable *tbGuestCat;
    TTable *tbBillCat;
    TAutoIncField *tbGuestCatID_CAT;
    TStringField *tbGuestCatNAMECAT;
    TAutoIncField *tbBillCatID_BILL;
    TStringField *tbBillCatNAME;
    TDataSource *srGuestBillData;
    TTable *tbGuestBillData;
    TAutoIncField *tbGuestBillDataID_DB;
    TIntegerField *tbGuestBillDataBILL_NO;
    TIntegerField *tbGuestBillDataNO_OF_GUESTS;
    TDateTimeField *tbGuestBillDataDATE_IN;
    TDateTimeField *tbGuestBillDataDATE_OUT;
    TIntegerField *tbGuestBillDataID_ROOM;
    TCurrencyField *tbGuestBillData_RATE;
    TCurrencyField *tbRoomRATE;
    TIntegerField *tbGuestBillData_NIGHTS;
    TCurrencyField *tbGuestBillData_CHARGE;
    TStringField *tbGuestBillData__ROOM_NAME_;
    TTable *tbGuest;
    TDataSource *srGuest;
    TTable *tbAddCh;
    TTable *tbPayments;
    TDataSource *srBill;
    TDataSource *srAddCh;
    TDataSource *srPayments;
    TAutoIncField *tbGuestID_GUEST;
    TStringField *tbGuestFNAME;
    TStringField *tbGuestLNAME;
    TStringField *tbGuestCITY;
    TStringField *tbGuestSTATE;
    TStringField *tbGuestADDRESS;
    TIntegerField *tbGuestPOST;
    TIntegerField *tbGuestID_CAT;
    TCurrencyField *tbGuestDEPOSIT;
    TStringField *tbGuest_L_CAT;
    TAutoIncField *tbAddChID_AC;
    TIntegerField *tbAddChBILL_NO;
    TDateTimeField *tbAddChMKDATE;
    TIntegerField *tbAddChID_BILL;
    TCurrencyField *tbAddChAMOUNT;
    TStringField *tbAddCh_L_NAME;
    TAutoIncField *tbPaymentsID_PAY;
    TIntegerField *tbPaymentsBILL_NO;
    TStringField *tbPaymentsDESCRIPTION;
    TCurrencyField *tbPaymentsAMOUNT;
    TTable *tbYesNo;
    TTable *tbBill;
    TAutoIncField *tbBillBILL_NO;
    TIntegerField *tbBillID_GUEST;
    TStringField *tbBillFOLIO;
    TDateTimeField *tbBillDATE_;
    TStringField *tbBillHEARD_OF_GI;
    TStringField *tbBillDETAILS;
    TMemoField *tbBillCOMMENTS;
    TCurrencyField *tbBill_DEPOSIT_RECEIVD;
    TCurrencyField *tbBill_BALANCE_DUE;
    TCurrencyField *tbBill_TOTAL_ADDCH;
    TCurrencyField *tbBill_TOTAL_PAYMENTS;
    TCurrencyField *tbBill_TOTAL_ROOMS;
    TCurrencyField *tbBill_SUBTOTAL;
    TCurrencyField *tbBill_TAX_AND_SERVICE;
    TCurrencyField *tbBill_TOTAL;
    TTable *tbStates;
    TDataSource *srStates;
    TDataSource *srBillSum;
    TQuery *qBillSum;
    TIntegerField *qBillSumBILL_NO;
    TIntegerField *qBillSumID_GUEST;
    TStringField *qBillSumFOLIO;
    TDateTimeField *qBillSumDATE_;
    TStringField *qBillSumHEARD_OF_GI;
    TStringField *qBillSumDETAILS;
    TStringField *qBillSumFNAME;
    TStringField *qBillSumLNAME;
    TStringField *qBillSumCITY;
    TIntegerField *qBillSumID_CAT;
    TCurrencyField *qBillSum_DEPOSIT_RECEIVD;
    TCurrencyField *qBillSum_BALANCE_DUE;
    TStringField *qBillSum_L_CAT;
    TQuery *qBillSumAll;
    TDateTimeField *DateTimeField1;
    TStringField *StringField1;
    TStringField *StringField2;
    TStringField *StringField3;
    TIntegerField *IntegerField1;
    TIntegerField *IntegerField2;
    TStringField *StringField4;
    TStringField *StringField5;
    TStringField *StringField6;
    TCurrencyField *CurrencyField1;
    TCurrencyField *CurrencyField2;
    TIntegerField *IntegerField3;
    TStringField *StringField7;
    TTable *tbCat_ForLst;
    TDataSource *srCat_ForLst;
    TAutoIncField *tbCat_ForLstID_CAT;
    TStringField *tbCat_ForLstNAMECAT;
    TQuery *qFindGuest;
    TQuery *qFndBillNo;
    TQuery *qFndFolio;
    TQuery *qTimeGuest;
    TQuery *qTimeBill;
    TQuery *qTimeGuest2;
    TQuery *qTimeBill2;
    TQuery *qBillGuest;
    TQuery *qBillGuest2;
    TQuery *qBillBill;
    TIntegerField *tbGuestCatACCT_NO;
    TIntegerField *tbBillCatACCT_NO;
    TDataSource *srBillCat2;
    TDataSource *srGuestCat2;
    TDataSource *srRoom2;
    TQuery *qBillCat2;
    TQuery *qGuestCat2;
    TQuery *qRoom2;
    TIntegerField *qBillCat2ID_BILL;
    TStringField *qBillCat2NAME;
    TIntegerField *qBillCat2ACCT_NO;
    TIntegerField *qRoom2ID_ROOM;
    TStringField *qRoom2NAME;
    TCurrencyField *qRoom2RATE;
    TIntegerField *qGuestCat2ID_CAT;
    TStringField *qGuestCat2NAMECAT;
    TIntegerField *qGuestCat2ACCT_NO;
    void __fastcall tbGuestBillDataBeforePost(TDataSet *DataSet);
    void __fastcall DMod1Create(TObject *Sender);
    void __fastcall tbPaymentsAfterPost(TDataSet *DataSet);
    void __fastcall tbGuestBillDataAfterInsert(TDataSet *DataSet);
    void __fastcall tbGuestBillDataAfterCancel(TDataSet *DataSet);
    void __fastcall tbPaymentsAfterCancel(TDataSet *DataSet);
    void __fastcall tbPaymentsAfterInsert(TDataSet *DataSet);
    void __fastcall tbGuestBillDataAfterPost(TDataSet *DataSet);
    void __fastcall tbAddChAfterCancel(TDataSet *DataSet);
    void __fastcall tbAddChAfterInsert(TDataSet *DataSet);
    void __fastcall tbAddChAfterPost(TDataSet *DataSet);
    
    void __fastcall tbBillAfterCancel(TDataSet *DataSet);
    void __fastcall tbBillAfterInsert(TDataSet *DataSet);
    void __fastcall tbBillAfterPost(TDataSet *DataSet);
    void __fastcall tbRoomAfterInsert(TDataSet *DataSet);
    void __fastcall tbGuestAfterInsert(TDataSet *DataSet);
    void __fastcall tbGuestBillDataCalcFields(TDataSet *DataSet);
    void __fastcall tbBillCalcFields(TDataSet *DataSet);
    
    void __fastcall tbCat_ForLstAfterScroll(TDataSet *DataSet);
    void __fastcall tbGuestBeforePost(TDataSet *DataSet);
    void __fastcall qGuestCat2BeforePost(TDataSet *DataSet);
    void __fastcall qGuestCat2PostError(TDataSet *DataSet,
          EDatabaseError *E, TDataAction &Action);
    void __fastcall qBillCat2BeforePost(TDataSet *DataSet);
    void __fastcall qBillCat2PostError(TDataSet *DataSet,
          EDatabaseError *E, TDataAction &Action);
    void __fastcall qRoom2BeforePost(TDataSet *DataSet);
    void __fastcall qRoom2PostError(TDataSet *DataSet, EDatabaseError *E,
          TDataAction &Action);
    void __fastcall qBillCat2AfterPost(TDataSet *DataSet);
    void __fastcall qGuestCat2AfterPost(TDataSet *DataSet);
    void __fastcall qRoom2AfterPost(TDataSet *DataSet);
    
    void __fastcall tbGuestBillDataAfterDelete(TDataSet *DataSet);
    void __fastcall tbGuestBillDataID_ROOMChange(TField *Sender);
private:	// User declarations
  bool m_bFl_PaymentsNew, m_bFl_AddChNew,
   m_bFl_RoomsNew, m_bFl_BillNew;

  bool m_bQGuestCat_Ins, m_bQBillCat_Ins, m_bQRoom_Ins;

public:		// User declarations
    __fastcall TDMod1(TComponent* Owner);

    bool __fastcall PostUpdates( auto_ptr<TDBDataSet*>& rA );
};
//---------------------------------------------------------------------------
extern PACKAGE TDMod1 *DMod1;
void MyRefr( TDataSet *DataSet );
//---------------------------------------------------------------------------
#endif
