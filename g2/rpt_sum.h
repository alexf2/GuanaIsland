//----------------------------------------------------------------------------
#ifndef rpt_sumH
#define rpt_sumH
//----------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\QuickRpt.hpp>
#include <vcl\QRCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Qrctrls.hpp>
#include <quickrpt.hpp>
#include "RxMemDS.hpp"
#include <Db.hpp>
#include <DBTables.hpp>
//----------------------------------------------------------------------------
class TQuickReport_Sum : public TQuickRep
{
__published:
    TQRBand *QRBand_Ttl;
    TQRLabel *QRLabel_Ttl1;
    TQRLabel *QRLabel_Acct;
    TQRLabel *QRLabel_Date;
    TQRBand *QRBand_Main;
    TQRSubDetail *QRSubDetail_Content;
    TQRBand *QRBand_Hdr;
    TQRBand *QRBand_Fother;
    TQRBand *QRBand_PageFother;
    TQRLabel *QRLabel_Foother;
    TQRSysData *QRSysData1;
    TQRSysData *QRSysData2;
    TRxMemoryData *RxMemoryData_Master;
    TRxMemoryData *RxMemoryData_Tbl;
    TIntegerField *RxMemoryData_MasterMASTER;
    TStringField *RxMemoryData_TblFOLIO;
    TStringField *RxMemoryData_TblGUEST_NAME;
    TIntegerField *RxMemoryData_TblNO_OF_GUESTS;
    TCurrencyField *RxMemoryData_TblROOMS_PAY;
    TCurrencyField *RxMemoryData_TblTOTAL;
    TCurrencyField *RxMemoryData_TblDEPOSIT;
    TCurrencyField *RxMemoryData_TblBALANCE_DUE;
    TCurrencyField *RxMemoryData_TblPAYMENTS;
    TQRLabel *QRLabel1;
    TQRLabel *QRLabel2;
    TQRLabel *QRLabel3;
    TQRLabel *QRLabel4;
    TQRLabel *QRLabel5;
    TQRLabel *QRLabel6;
    TQRLabel *QRLabel7;
    TQRLabel *QRLabel8;
    TQRLabel *QRLabel_Total;
    TQRDBText *QRDBText1;
    TQRDBText *QRDBText2;
    TQRDBText *QRDBText3;
    TQRDBText *QRDBText4;
    TQRDBText *QRDBText5;
    TQRDBText *QRDBText6;
    TQRDBText *QRDBText7;
    TQRDBText *QRDBText_Payment;
    TQRDBText *QRDBText_Total;
    TQRLabel *QRLabel10;
    TIntegerField *RxMemoryData_TblNIGHTS;
    TQRDBText *QRDBText_Total2;
    TQRDBText *QRDBText11;
    TQRDBText *QRDBText12;
    TQRDBText *QRDBText13;
    TQRDBText *QRDBText14;
    TQRDBText *QRDBText15;
    TQRDBText *QRDBText16;
    TRxMemoryData *RxMemoryData_Summ;
    TQuery *qSelectBill;
    TIntegerField *qSelectBillBILL_NO;
    TIntegerField *qSelectBillID_GUEST;
    TStringField *qSelectBillFOLIO;
    TDateTimeField *qSelectBillDATE_;
    TStringField *qSelectBillHEARD_OF_GI;
    TStringField *qSelectBillDETAILS;
    TMemoField *qSelectBillCOMMENTS;
    TStringField *qSelectBillFNAME;
    TStringField *qSelectBillLNAME;
    TStringField *qSelectBillCITY;
    TStringField *qSelectBillSTATE;
    TStringField *qSelectBillADDRESS;
    TIntegerField *qSelectBillPOST;
    TIntegerField *qSelectBillID_CAT;
    TCurrencyField *qSelectBill_DEPOSIT_RECEIVD;
    TCurrencyField *qSelectBill_BALANCE_DUE;
    TCurrencyField *qSelectBillDEPOSIT;
    TStringField *qSelectBillLFNAME;
    TIntegerField *qSelectBillSUM_GUESTS;
    TIntegerField *qSelectBillSUM_NIGHTS;
    TCurrencyField *qSelectBillROOMSPAY;
    TCurrencyField *qSelectBillSUM_PAYMENTS;
    TCurrencyField *qSelectBillTOTAL;
    TQuery *qCalc_BD;
    TIntegerField *RxMemoryData_SummNO_OF_GUESTS;
    TIntegerField *RxMemoryData_SummSUM_NIGHTS;
    TCurrencyField *RxMemoryData_SummSUM_ROOMS;
    TCurrencyField *RxMemoryData_SummSUM_DEPOSIT;
    TCurrencyField *RxMemoryData_SummSUM_BALANCE;
    TCurrencyField *RxMemoryData_SummSUM_PAYMENTS;
    TCurrencyField *RxMemoryData_SummSUM_TOTAL;
    TQuery *qSumPayments;
    TQuery *qSumAdd;
    TQRLabel *QRLabel_Deposit;
    TQRLabel *QRLabel_Payment;
    TQRLabel *QRLabel_Rooms;
    TQRShape *QRShape1;
    void __fastcall QuickReport_SumAfterPreview(TObject *Sender);
    void __fastcall QuickReport_SumBeforePrint(TQuickRep *Sender,
          bool &PrintReport);
    void __fastcall qSelectBillCalcFields(TDataSet *DataSet);
private:
  Variant m_vD1, m_vD2, m_vCat;
  Variant m_vtNameCat;
  Variant m_vtAcctNo;
  void AssField( TField* pF, char *lpName, TDBDataSet *pD );
  void  CloseDSets();
  void EraseAllDynamic();
  void AssSysAcc();

public:
   bool m_Busy;

   __fastcall TQuickReport_Sum(TComponent* Owner);

   void SetD1( Variant );
   void SetD2( Variant );
   void SetCat( Variant );
   __property Variant Date1 = { read = m_vD1, write = SetD1 };
   __property Variant Date2 = { read = m_vD2, write = SetD2 };
   __property Variant Cat = { read = m_vCat, write = SetCat };
};
//----------------------------------------------------------------------------
extern TQuickReport_Sum *QuickReport_Sum;
//----------------------------------------------------------------------------
#endif