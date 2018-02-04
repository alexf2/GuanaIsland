//---------------------------------------------------------------------------
#ifndef nformH
#define nformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <dbgrids.hpp>
//---------------------------------------------------------------------------
class TNavForm : public TForm
{
__published:	// IDE-managed Components
    TFormPlacement *FormPlacement1;
    TDBNavigator *DBNavigator1;void __fastcall DBNavigator1BeforeAction(TObject *Sender,
          TNavigateBtn Button);
private:	// User declarations
  TDBGrid *m_pGrid;
  void SetBinding( TDBGrid *p )
   {
     m_pGrid = p;
     DBNavigator1->DataSource = !m_pGrid ? NULL:p->DataSource;
   }
public:		// User declarations
    __fastcall TNavForm(TComponent* Owner);

    __property TDBGrid *BndGrid = { read = m_pGrid, write = SetBinding };

protected:
  void __fastcall CreateParams(Controls::TCreateParams &Params);
  void __fastcall Loaded(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TNavForm *NavForm;
//---------------------------------------------------------------------------
#endif
