//---------------------------------------------------------------------------
#ifndef guana2_mfH
#define guana2_mfH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "Placemnt.hpp"
#include "RxGrdCpt.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "RXDBCtrl.hpp"
#include "RXSplit.hpp"
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "RXCtrls.hpp"
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include "RxLookup.hpp"
#include "ToolEdit.hpp"
#include "CurrEdit.hpp"
#include "RxDBComb.hpp"
#include <memory>
#include <list>
using namespace std;
//---------------------------------------------------------------------------

struct TCtrlEntry
 {
   TControl *p;
   TColor key;
 };
TColor GetClr( TControl* );
void SetClr( TControl*, TColor );
struct TBndCtrl
 {
   TBndCtrl(): m_pSrc( NULL ), m_bState( true )
    {
    }
   TBndCtrl( const TBndCtrl& rC )
    {
      this->operator=( rC );
    }
   TBndCtrl& operator=( const TBndCtrl& rC )
    {
      m_pSrc = rC.m_pSrc;  m_s = rC.m_s; m_bState = rC.m_bState;
      return *this;
    }
   TBndCtrl& operator,( TControl* pCtrl )
    {
      TCtrlEntry ent; ent.p = pCtrl; ent.key = GetClr( pCtrl );
      m_s.push_back( ent );
      return *this;
    }
   void Bnd( TDataSource *pSrc )
    {
      m_pSrc = pSrc;
    }

   void CheckState();
   bool Empty() const { return m_s.size() == 0; }

protected:
   bool m_bState;
   TDataSource *m_pSrc;
   list<TCtrlEntry> m_s;
 };

class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TRxGradientCaption *RxGradientCaption1;
    TRxDBGrid *RxDBGrid_Room;
    TPanel *Panel1;
    TRxSplitter *RxSplitter2;
    TRxLabel *RxLabel1;
    TPanel *Panel3;
    TRxLabel *RxLabel2;
    TRxLabel *RxLabel3;
    TRxDBGrid *RxDBGrid_GuestCat;
    TRxDBGrid *RxDBGrid_BillsCat;
    TRxDBGrid *RxDBGrid_Rooms;
    TRxLabel *RxLabel4;
    TRxLabel *RxLabel5;
    TRxLabel *RxLabel6;
    TRxLabel *RxLabel7;
    TRxLabel *RxLabel8;
    TRxSplitter *RxSplitter1;
    TRxSplitter *RxSplitter3;
    TRxDBGrid *RxDBGrid_Guest;
    TRxDBGrid *RxDBGrid_Bills;
    TRxDBGrid *RxDBGrid_AddCharges;
    TRxDBGrid *RxDBGrid_Payments;
    TPanel *Panel22;
    TFormStorage *FormStorage1;
    TPanel *Panel2;
    TPanel *Panel4;
    TPanel *Panel5;
    TRxDBGrid *RxDBGrid_MRooms;
    TRxLabel *RxLabel10;
    TPanel *Panel6;
    TPanel *Panel7;
    TRxSplitter *RxSplitter4;
    TRxLabel *RxLabel23;
    TRxLabel *RxLabel24;
    TRxDBGrid *RxDBGrid_MAddCh;
    TRxDBGrid *RxDBGrid_MPayments;
    TDBMemo *DBMemo_Comments;
    TRxSplitter *RxSplitter5;
    TPanel *Panel8;
    TRxLabel *RxLabel27;
    TDBEdit *DBEdit_DepositReceived;
    TRxLabel *RxLabel28;
    TDBEdit *DBEdit_Balance;
    TDBEdit *DBEdit_Total;
    TRxLabel *RxLabel32;
    TRxLabel *RxLabel31;
    TDBEdit *DBEdit_Subtotal;
    TLabel *Label5;
    TLabel *Label6;
    TDBEdit *DBEdit_Payments;
    TRxLabel *RxLabel30;
    TDBEdit *DBEdit_TaxServ;
    TRxLabel *RxLabel29;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label2;
    TLabel *Label1;
    TDBEdit *DBEdit_TotalRooms;
    TRxLabel *RxLabel25;
    TRxLabel *RxLabel26;
    TDBEdit *DBEdit_TotalAdd;
    TPanel *Panel9;
    TRxLabel *RxLabel9;
    TDBEdit *DBEdit_BillNo;
    TRxLabel *RxLabel12;
    TDBEdit *DBEdit_FName;
    TRxLabel *RxLabel22;
    TRxDBCalcEdit *RxDBCalcEdit_Deposit;
    TRxLabel *RxLabel13;
    TDBEdit *DBEdit_LName;
    TRxLabel *RxLabel14;
    TRxLabel *RxLabel18;
    TDBEdit *DBEdit_Address;
    TRxLabel *RxLabel19;
    TDBEdit *DBEdit_City;
    TRxLabel *RxLabel21;
    TRxLabel *RxLabel15;
    TDBEdit *DBEdit_Folio;
    TRxLabel *RxLabel11;
    TDBEdit *DBEdit_GI;
    TRxLabel *RxLabel17;
    TDBEdit *DBEdit_Details;
    TRxLabel *RxLabel20;
    TDBEdit *DBEdit_Post;
    TRxDBLookupCombo *RxDBLookupCombo_Category;
    TRxLabel *RxLabel16;
    TDBDateEdit *DBDateEdit1;
    TDBComboBox *DBComboBox_State;
    TRxDBGrid *RxDBGrid_SumRep;
    TPanel *Panel10;
    TRxSplitter *RxSplitter6;
    TDateEdit *DateEdit1;
    TDateEdit *DateEdit2;
    TRxLabel *RxLabel33;
    TRxLabel *RxLabel34;
    TRadioButton *RadioButton1;
    TRadioButton *RadioButton2;
    TBitBtn *BitBtn_SumRep;
    TDBLookupListBox *DBLookupListBox_Cat;
    TBitBtn *BitBtn1;
    void __fastcall RxDBGrid_RoomEnter(TObject *Sender);
    void __fastcall RxDBGrid_GuestCatEnter(TObject *Sender);
    void __fastcall RxDBGrid_BillsCatEnter(TObject *Sender);
    void __fastcall RxDBGrid_RoomGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_GuestCatGetCellParams(TObject *Sender, TField *Field,
          TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_BillsCatGetCellParams(TObject *Sender, TField *Field,
          TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall PageControl1Changing(TObject *Sender,
          bool &AllowChange);
    
    void __fastcall FormShow(TObject *Sender);
    void __fastcall RxDBGrid_GuestGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_BillsGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_RoomsGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_AddChargesGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_PaymentsGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall RxDBGrid_GuestEnter(TObject *Sender);
    void __fastcall RxDBGrid_BillsEnter(TObject *Sender);
    void __fastcall RxDBGrid_RoomsEnter(TObject *Sender);
    void __fastcall RxDBGrid_AddChargesEnter(TObject *Sender);
    void __fastcall RxDBGrid_PaymentsEnter(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    
    void __fastcall RxDBGrid_MRoomsEnter(TObject *Sender);
    void __fastcall RxDBGrid_MAddChEnter(TObject *Sender);
    void __fastcall RxDBGrid_MPaymentsEnter(TObject *Sender);
    void __fastcall RxDBGrid_MRoomsGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_MAddChGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_MPaymentsGetCellParams(TObject *Sender,
          TField *Field, TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall DBEdit_FNameEnter(TObject *Sender);
    void __fastcall DBEdit_LNameEnter(TObject *Sender);
    void __fastcall RxDBCalcEdit_DepositEnter(TObject *Sender);
    void __fastcall DBDateEdit1Enter(TObject *Sender);
    void __fastcall DBEdit_AddressEnter(TObject *Sender);
    void __fastcall DBEdit_CityEnter(TObject *Sender);
    void __fastcall DBComboBox_StateEnter(TObject *Sender);
    void __fastcall DBEdit_FolioEnter(TObject *Sender);
    void __fastcall DBEdit_GIEnter(TObject *Sender);
    void __fastcall DBEdit_PostEnter(TObject *Sender);
    void __fastcall RxDBLookupCombo_CategoryEnter(TObject *Sender);
    void __fastcall DBEdit_DetailsEnter(TObject *Sender);
    void __fastcall DBMemo_CommentsEnter(TObject *Sender);
    void __fastcall DateEdit1Change(TObject *Sender);
    void __fastcall DateEdit2Change(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall RadioButton1Click(TObject *Sender);
    void __fastcall RadioButton2Click(TObject *Sender);
    
    void __fastcall RxDBGrid_SumRepGetCellParams(TObject *Sender, TField *Field,
          TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall RxDBGrid_SumRepEnter(TObject *Sender);
    void __fastcall DBEdit_BillNoEnter(TObject *Sender);
    
    void __fastcall PageControl1Change(TObject *Sender);
    void __fastcall BitBtn_SumRepClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    
    void __fastcall RxDBGrid_MRoomsEditButtonClick(TObject *Sender);
    void __fastcall RxDBGrid_MAddChEditButtonClick(TObject *Sender);
    void __fastcall RxDBGrid_BillsEditButtonClick(TObject *Sender);
    void __fastcall RxDBGrid_RoomsEditButtonClick(TObject *Sender);
    void __fastcall RxDBGrid_AddChargesEditButtonClick(TObject *Sender);
    void __fastcall RxDBGrid_MRoomsKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    
    
private:	// User declarations
  TForm *m_pNavForm;
  TWinControl* m_arrLastFocus[ 4 ];
  bool m_bFirstShow;

public:		// User declarations
    __fastcall TForm1(TComponent* Owner);

    void __fastcall FocusLastGrid();
    TDBGrid *m_pLastGrid;

    auto_ptr<TDBDataSet*> __fastcall GetDSets( int iTab );

    TWinControl* GetLF()
     {
       return m_arrLastFocus[ PageControl1->ActivePage->PageIndex ];
     }
    void SetLF( TWinControl* p )
     {
       m_arrLastFocus[ PageControl1->ActivePage->PageIndex ] = p;
     }
    __property TWinControl *LastFocus_ = { read = GetLF, write = SetLF };

protected:
    void __fastcall MyIdleHandler( TObject *Sender, bool &Done );
    void __fastcall AppException( TObject *Sender, Sysutils::Exception *E );

    TBndCtrl m_bndGuest, m_bndBill;
};

void SetupFldClr( TField *Field, TColor &Background, TFont* rF, bool bSel );


//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
void AssRdProp( TDBGrid* pG, AnsiString& rS, bool bFl = true );
void AssElipsisProp( TDBGrid* pG, AnsiString& rS );
void InputDF( TRxDBGrid* pG );
//---------------------------------------------------------------------------
#endif
