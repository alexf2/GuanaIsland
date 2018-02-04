//---------------------------------------------------------------------------
#ifndef dm2H
#define dm2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TDMod2 : public TDataModule
{
__published:	// IDE-managed Components
    TQuery *qRoomRate;
    TQuery *qTotalRooms;
    TQuery *qTotalADd;
    TQuery *qTotalPayments;
    TDatabase *DB2;
private:	// User declarations
public:		// User declarations
    __fastcall TDMod2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDMod2 *DMod2;
//---------------------------------------------------------------------------
#endif
