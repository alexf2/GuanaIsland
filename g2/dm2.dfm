�
 TDMOD2 0�  TPF0TDMod2DMod2LeftjTop� Height� Width� TQuery	qRoomRateDatabaseNameDB2SQL.Strings,select RATE from Room where ID_ROOM=:id_room Params.Data
     id_room        Left&TopR  TQueryqTotalRoomsDatabaseNameDB2SQL.Strings3select Sum([_RATE]*([DATE_OUT]-[DATE_IN])) as [SAM]-  from GuestBillData where [BILL_NO]=:bill_no Params.Data
     bill_no        Left� Top2  TQuery	qTotalADdDatabaseNameDB2SQL.Strings3select sum([AMOUNT]) as [SAM] from AdditionalCharge  where [BILL_NO] = :bill_no Params.Data
     bill_no        Left� Top1  TQueryqTotalPaymentsDatabaseNameDB2SQL.Strings+select sum([AMOUNT]) as [SAM] from Payments  where [BILL_NO] = :bill_no Params.Data
     bill_no        LeftTop3  	TDatabaseDB2	AliasNameGUANA2	Connected	DatabaseNameDB2LoginPromptParams.StringsUSER NAME=admin SessionNameDefaultLeftTop   