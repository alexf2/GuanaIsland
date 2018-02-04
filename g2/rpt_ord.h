//----------------------------------------------------------------------------
#ifndef rpt_ordH
#define rpt_ordH
//----------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\QuickRpt.hpp>
#include <vcl\QRCtrls.hpp>
#include <Qrctrls.hpp>
#include <quickrpt.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include "RxMemDS.hpp"
//----------------------------------------------------------------------------
class TQuickReport_Ordinal : public TQuickRep
{
__published:
    TQRBand *QRBand1;
    TQRLabel *QRLabel1;
    TQRImage *QRImage1;
    TQRLabel *QRLabel2;
    TQRLabel *QRLabel3;
    TQuery *qBill;

    TQuery *qStatus;
    TDataSource *srBill;
    TQRBand *QRBand2;
    TQRLabel *QRLabel4;
    TQRDBText *QRDBText1;
    TQRLabel *QRLabel5;
    TQRDBText *QRDBText2;
    TQRLabel *QRLabel6;
    TQRDBText *QRDBText3;
    TQRLabel *QRLabel7;
    TQRDBText *QRDBText4;
    TQRLabel *QRLabel8;
    TQRDBText *QRDBText5;
    TQRDBText *QRDBText6;
    TQRDBText *QRDBText7;
    TQRLabel *QRLabel9;
    TQRDBText *QRDBText9;
    TQRBand *QRBand_RoomHdr;
    TQRSubDetail *QRSubDetail_Room;
    TQRLabel *QRLabel10;
    TQRLabel *QRLabel11;
    TQRLabel *QRLabel12;
    TQRLabel *QRLabel13;
    TQRBand *QRBand_RoomFother;
    TQRDBText *QRDBText10;
    TQuery *qRoom;
    TQRDBText *QRDBText11;
    TQRDBText *QRDBText12;
    TQRDBText *QRDBText13;
    TQuery *qRoomSumm;
    TQRDBText *QRDBText14;
    TQRDBText *QRDBText15;
    TQRLabel *QRLabel14;
    TIntegerField *qStatusPOST;
    TStringField *qStatusSTATE;
    TStringField *qStatusCITY;
    TStringField *qStatusNAMECAT;
    TMemoField *qStatusFLNAME;
    TStringField *qStatusADDRESS;
    TStringField *qStatusCSP;
    TStringField *qStatusCPSS2;
    TQRSubDetail *QRSubDetail1;
    TQuery *qBill2;
    TQuery *qBill3;
    TQRLabel *QRLabel15;
    TQRDBText *QRDBText8;
    TQRSubDetail *QRSubDetail2;
    TQRBand *QRBand_AddCh_Hdr;
    TQRBand *QRBand_AddCh_Fother;
    TQRLabel *QRLabel16;
    TQRLabel *QRLabel17;
    TQRLabel *QRLabel18;
    TQRLabel *QRLabel19;
    TQRLabel *QRLabel20;
    TQRLabel *QRLabel21;
    TQRDBText *QRDBText16;
    TQRDBText *QRDBText17;
    TQRDBText *QRDBText18;
    TQRDBText *QRDBText19;
    TQuery *qAddChSum;
    TQuery *qAddCh;
    TQRLabel *QRLabel22;
    TQRDBText *QRDBText20;
    TIntegerField *qAddChID_AC;
    TStringField *qAddChNAME;
    TDateTimeField *qAddChMKDATE;
    TCurrencyField *qAddChAMOUNT;
    TStringField *qAddChSumNAME;
    TCurrencyField *qAddChSumAMOUNT;
    TRxMemoryData *RxMemoryData_AddCh;
    TIntegerField *RxMemoryData_AddChID_AC;
    TStringField *RxMemoryData_AddChNAME;
    TCurrencyField *RxMemoryData_AddChAMOUNT;
    TDateTimeField *RxMemoryData_AddChMKDATE;
    TStringField *RxMemoryData_AddChNAME2;
    TCurrencyField *RxMemoryData_AddChAMOUNT2;
    TQRDBText *QRDBText21;
    TQuery *qTotalAdd;
    TQRLabel *QRLabel23;
    TQRLabel *QRLabel24;
    TQRDBText *QRDBText22;
    TQRDBText *QRDBText23;
    TQRDBText *QRDBText24;
    TIntegerField *qRoomDT_DIFF;
    TDateTimeField *qRoomDATE_IN;
    TDateTimeField *qRoomDATE_OUT;
    TIntegerField *qRoomNO_OF_GUESTS;
    TIntegerField *qRoomID_ROOM;
    TIntegerField *qRoomBILL_NO;
    TStringField *qRoomNAME;
    TCurrencyField *qRoomRATE;
    TCurrencyField *qRoomCHARGE;
    TIntegerField *qBillBILL_NO;
    TIntegerField *qBillID_GUEST;
    TStringField *qBillFOLIO;
    TDateTimeField *qBillDATE_;
    TStringField *qBillHEARD_OF_GI;
    TStringField *qBillDETAILS;
    TMemoField *qBillCOMMENTS;
    TCurrencyField *qBill_DEPOSIT_RECEIVD;
    TCurrencyField *qBill_BALANCE_DUE;
    TFloatField *qRoomSummSNOG;
    TFloatField *qRoomSummSTIM;
    TCurrencyField *qRoomSummTAMOUNT;
    TCurrencyField *qTotalAddTAMOUNT;
    TCurrencyField *RxMemoryData_AddChP_AMOUNT;
    TStringField *RxMemoryData_AddChP_DESCR;
    TQuery *qPayments;
    TQRDBText *QRDBText25;
    TQRDBText *QRDBText26;
    TQRLabel *QRLabel25;
    TQRLabel *QRLabel26;
    TQRLabel *QRLabel27;
    TQRDBText *QRDBText27;
    TQuery *qTotalPayments;
    TCurrencyField *qTotalPaymentsTAMOUNT;
    TQRSubDetail *QRSubDetail_End;
    TQRBand *QRBand3;
    TQRLabel *QRLabel_BN;
    TQRSysData *QRSysData1;
    TQRSysData *QRSysData2;
    TQRLabel *QRLabel29;
    TMemoField *qBill3COMMENTS;
    TQRDBText *QRDBText28;
    TRxMemoryData *RxMemoryData_Summ;
    TCurrencyField *RxMemoryData_SummSUBTOTAL;
    TCurrencyField *RxMemoryData_SummTAX_AND_SRV;
    TCurrencyField *RxMemoryData_SummTOTAL;
    TCurrencyField *RxMemoryData_SummDEPOSIT;
    TCurrencyField *RxMemoryData_SummPAYMENTS;
    TCurrencyField *RxMemoryData_SummBALANCE_DUE;
    TQRLabel *QRLabel28;
    TQRDBText *QRDBText29;
    TQRLabel *QRLabel30;
    TQRDBText *QRDBText30;
    TQRLabel *QRLabel34;
    TQRDBText *QRDBText34;
    TQRLabel *QRLabel35;
    TQRDBText *QRDBText35;
    TQRLabel *QRLabel36;
    TQRDBText *QRDBText36;
    TQRLabel *QRLabel37;
    TQRDBText *QRDBText37;
    void __fastcall QuickReport_OrdinalBeforePrint(TQuickRep *Sender,
          bool &PrintReport);
    void __fastcall QuickReport_OrdinalAfterPreview(TObject *Sender);
    void __fastcall qStatusCalcFields(TDataSet *DataSet);
private:
  Variant m_vBillNo;

  void AssField( TField* pF, char *lpName, TDBDataSet *pD );
  void EraseTmpTables();
  void  CloseDSets();

public:
   bool m_Busy;
   __fastcall TQuickReport_Ordinal(TComponent* Owner);

     void SetBillNo( Variant );
   __property Variant BillNo = { read = m_vBillNo, write = SetBillNo };
};
//----------------------------------------------------------------------------
extern TQuickReport_Ordinal *QuickReport_Ordinal;
//----------------------------------------------------------------------------
#endif