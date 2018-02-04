//---------------------------------------------------------------------------
#ifndef helperH
#define helperH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <bdeutils.hpp>
#include "Placemnt.hpp"
#include <memory>
using namespace std;
//---------------------------------------------------------------------------
enum TE_Srcs { SR_Bill, SR_Guest };

class TFHelper : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TDBNavigator *DBNavigator1;
    TBitBtn *BitBtn_NewGuest;
    TBitBtn *BitBtn_RemoveGuest;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TBitBtn *BitBtn_NewBill;
    TBitBtn *BitBtn_RemoveBill;
    TDBNavigator *DBNavigator2;
    TBitBtn *BitBtn_PayEnrol;
    TBitBtn *BitBtn_MoneyBack;
    TEdit *Edit_FName;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit_LName;
    TBitBtn *BitBtn_GoGuest;
    TLabel *Label3;
    TEdit *Edit_BillNo;
    TLabel *Label4;
    TEdit *Edit_Folio;
    TBitBtn *BitBtn_GoBill;
    TBitBtn *BitBtn_LastTime;
    TBitBtn *BitBtn_LastNo;
    TBevel *Bevel1;
    TTabSheet *TabSheet5;
    TBitBtn *BitBtn_Pretty;
    TBitBtn *BitBtn_Ordinal;
    TRadioButton *RadioButton_No;
    TRadioButton *RadioButton_Folio;
    TPanel *Panel1;
    TRadioButton *RadioButton_All;
    TRadioButton *RadioButton_Current;
    TRadioButton *RadioButton1;
    TFormPlacement *FormPlacement1;
    TEdit *Edit_Full;
    TLabel *Label5;
    TBevel *Bevel2;
    void __fastcall BitBtn_NewGuestClick(TObject *Sender);
    void __fastcall BitBtn_RemoveGuestClick(TObject *Sender);
    void __fastcall BitBtn_NewBillClick(TObject *Sender);
    void __fastcall BitBtn_RemoveBillClick(TObject *Sender);
    void __fastcall BitBtn_PayEnrolClick(TObject *Sender);
    void __fastcall BitBtn_MoneyBackClick(TObject *Sender);
    void __fastcall BitBtn_GoGuestClick(TObject *Sender);
    void __fastcall BitBtn_GoBillClick(TObject *Sender);
    void __fastcall BitBtn_LastTimeClick(TObject *Sender);
    void __fastcall BitBtn_LastNoClick(TObject *Sender);
    void __fastcall BitBtn_OrdinalClick(TObject *Sender);
    void __fastcall Edit_FolioEnter(TObject *Sender);
    void __fastcall Edit_BillNoEnter(TObject *Sender);
    void __fastcall Edit_FullChange(TObject *Sender);
private:	// User declarations
  TWinControl *m_pAct;
  void SetBinding( TWinControl *p )
   {
     m_pAct = p;
   }
public:		// User declarations
    __fastcall TFHelper(TComponent* Owner);

    __property TWinControl *BndCtrl = { read = m_pAct, write = SetBinding };

    void MakeBind( TWinControl*, TE_Srcs );
    void CheckState();

protected:
  void __fastcall CreateParams(Controls::TCreateParams &Params);
  void __fastcall Loaded(void);

  void MakePay( bool );

  //auto_ptr<TDBLocate> m_apLdbGuest;
};
//---------------------------------------------------------------------------
extern PACKAGE TFHelper *FHelper;
//---------------------------------------------------------------------------
#endif
