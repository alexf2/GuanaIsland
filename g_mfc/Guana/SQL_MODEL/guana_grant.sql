USE guana_bill
go

--GAdmin       admin
--GOperator    operator
--GCManeger    maneger
--GVahter      vahter
--GBuh         buh
--GGuest       guest
--GDir         director

/*if not exists (select * from master..syslogins where name = N'DOMAIN1\Administrator')
	exec sp_grantlogin N'DOMAIN1\Administrator'
	exec sp_defaultdb N'DOMAIN1\Administrator', N'master'
	exec sp_defaultlanguage N'DOMAIN1\Administrator', N'us_english'
*/

if not exists (select * from master..syslogins where name = N'GAdmin')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GAdmin', null, @logindb, @loginlang
END
GO

if not exists (select * from master..syslogins where name = N'GBuh')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GBuh', null, @logindb, @loginlang
END
GO

if not exists (select * from master..syslogins where name = N'GCManeger')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GCManeger', null, @logindb, @loginlang
END
GO

if not exists (select * from master..syslogins where name = N'GDir')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GDir', null, @logindb, @loginlang
END
GO

if not exists (select * from master..syslogins where name = N'GGuest')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GGuest', null, @logindb, @loginlang
END
GO

if not exists (select * from master..syslogins where name = N'GOperator')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GOperator', null, @logindb, @loginlang
END
GO

if not exists (select * from master..syslogins where name = N'GVahter')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'guana_bill', @loginlang = N'us_english'
	if @logindb is null or not exists (select * from master..sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master..syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'GVahter', null, @logindb, @loginlang
END
GO

--exec sp_addsrvrolemember N'DOMAIN1\Administrator', sysadmin


if not exists (select * from sysusers where name = N'GAdmin' and uid < 16382)
	EXEC sp_grantdbaccess N'GAdmin', N'GAdmin'
GO

if not exists (select * from sysusers where name = N'GBuh' and uid < 16382)
	EXEC sp_grantdbaccess N'GBuh', N'GBuh'
GO

if not exists (select * from sysusers where name = N'GCManeger' and uid < 16382)
	EXEC sp_grantdbaccess N'GCManeger', N'GCManeger'
GO

if not exists (select * from sysusers where name = N'GDir' and uid < 16382)
	EXEC sp_grantdbaccess N'GDir', N'GDir'
GO

if not exists (select * from sysusers where name = N'GGuest' and uid < 16382)
	EXEC sp_grantdbaccess N'GGuest', N'GGuest'
GO

if not exists (select * from sysusers where name = N'GOperator' and uid < 16382)
	EXEC sp_grantdbaccess N'GOperator', N'GOperator'
GO

if not exists (select * from sysusers where name = N'GVahter' and uid < 16382)
	EXEC sp_grantdbaccess N'GVahter', N'GVahter'
GO

exec sp_addrolemember N'db_datareader', N'GBuh'
GO

exec sp_addrolemember N'db_datareader', N'GCManeger'
GO

exec sp_addrolemember N'db_datareader', N'GDir'
GO

exec sp_addrolemember N'db_datareader', N'GGuest'
GO

exec sp_addrolemember N'db_datareader', N'GOperator'
GO

exec sp_addrolemember N'db_datareader', N'GVahter'
GO

exec sp_addrolemember N'db_owner', N'GAdmin'
GO


GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[AdditionalCharge]  TO [GOperator]
GO

GRANT  REFERENCES  ON [dbo].[BillCategory]  TO [GOperator]
GO

GRANT  REFERENCES  ON [dbo].[CategoryGuest]  TO [GOperator]
GO

GRANT  REFERENCES  ON [dbo].[CategoryGuest_BillCategory]  TO [GOperator]
GO

DENY  SELECT  ON [dbo].[Department]  TO [GOperator] CASCADE 
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[Emplyee]  TO [GCManeger]
GO

GRANT  REFERENCES  ON [dbo].[Emplyee]  TO [GOperator]
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[Guest]  TO [GOperator]
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[GuestBill]  TO [GOperator]
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[GuestBillData]  TO [GOperator]
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[Job]  TO [GCManeger]
GO

DENY  SELECT  ON [dbo].[Job]  TO [GOperator] CASCADE 
GO

GRANT  REFERENCES  ON [dbo].[JobCategory]  TO [GCManeger]
GO

GRANT  REFERENCES ,  INSERT ,  UPDATE  ON [dbo].[Operation]  TO [GBuh]
GO

GRANT  REFERENCES ,  INSERT ,  UPDATE  ON [dbo].[Operation]  TO [GCManeger]
GO

GRANT  REFERENCES ,  INSERT ,  UPDATE  ON [dbo].[Operation]  TO [GDir]
GO

GRANT  REFERENCES ,  INSERT ,  UPDATE  ON [dbo].[Operation]  TO [GOperator]
GO

DENY  DELETE  ON [dbo].[Operation]  TO [GOperator] CASCADE 
GO

GRANT  REFERENCES ,  INSERT ,  UPDATE  ON [dbo].[Operation]  TO [GVahter]
GO

GRANT  REFERENCES  ON [dbo].[OperationDescription]  TO [GOperator]
GO

GRANT  REFERENCES  ON [dbo].[Operator]  TO [GOperator]
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[Payments]  TO [GOperator]
GO

GRANT  REFERENCES  ON [dbo].[Room]  TO [GOperator]
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[WorkAlloc]  TO [GCManeger]
GO

DENY  SELECT  ON [dbo].[WorkAlloc]  TO [GOperator] CASCADE 
GO

DENY  SELECT  ON [dbo].[WorkTimeLog]  TO [GOperator] CASCADE 
GO

GRANT  REFERENCES ,  INSERT ,  DELETE ,  UPDATE  ON [dbo].[WorkTimeLog]  TO [GVahter]
GO

GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

DENY  EXECUTE  ON [dbo].[AddCategoryes]  TO [GCManeger] CASCADE 
GO

GRANT  EXECUTE  ON [dbo].[AddCategoryes]  TO [GOperator]
GO

GRANT  EXECUTE  ON [dbo].[Emplyee_delcascade]  TO [GCManeger]
GO

DENY  EXECUTE  ON [dbo].[Emplyee_delcascade]  TO [GOperator] CASCADE 
GO

GRANT  EXECUTE  ON [dbo].[Get_BillRecommended]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[Get_BillRecommended]  TO [GOperator]
GO

GRANT  EXECUTE  ON [dbo].[Guest_delcascade]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[Guest_delcascade]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[GuestBill_delcascade]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[GuestBill_delcascade]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[GuestBill_delcascade_time]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[GuestBill_delcascade_time]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[Job_delcascade]  TO [GCManeger]
GO

DENY  EXECUTE  ON [dbo].[Job_delcascade]  TO [GOperator] CASCADE 
GO


DENY  EXECUTE  ON [dbo].[OperationDescription_delcascade]  TO [GOperator] CASCADE 
GO


DENY  EXECUTE  ON [dbo].[Operator_delcascade]  TO [GOperator] CASCADE 
GO


GRANT  EXECUTE  ON [dbo].[PayEnroll]  TO [GBuh]
GO

DENY  EXECUTE  ON [dbo].[PayEnroll]  TO [GCManeger] CASCADE 
GO

GRANT  EXECUTE  ON [dbo].[PayEnroll]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[procAdditionalCharge]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[procAdditionalCharge]  TO [GCManeger]
GO

GRANT  EXECUTE  ON [dbo].[procAdditionalCharge]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[procCategoryName]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[procCategoryName]  TO [GCManeger]
GO

GRANT  EXECUTE  ON [dbo].[procCategoryName]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[procGuestBill]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[procGuestBill]  TO [GCManeger]
GO

GRANT  EXECUTE  ON [dbo].[procGuestBill]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[procGuestData]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[procGuestData]  TO [GCManeger]
GO

GRANT  EXECUTE  ON [dbo].[procGuestData]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[procRoom]  TO [GBuh]
GO

GRANT  EXECUTE  ON [dbo].[procRoom]  TO [GCManeger]
GO

GRANT  EXECUTE  ON [dbo].[procRoom]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[RecalcGuestBill]  TO [GBuh]
GO

DENY  EXECUTE  ON [dbo].[RecalcGuestBill]  TO [GCManeger] CASCADE 
GO

GRANT  EXECUTE  ON [dbo].[RecalcGuestBill]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[RecalcGuestBill_Data]  TO [GBuh]
GO

DENY  EXECUTE  ON [dbo].[RecalcGuestBill_Data]  TO [GCManeger] CASCADE 
GO

GRANT  EXECUTE  ON [dbo].[RecalcGuestBill_Data]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[RecalcGuestBill_Root]  TO [GBuh]
GO

DENY  EXECUTE  ON [dbo].[RecalcGuestBill_Root]  TO [GCManeger] CASCADE 
GO

GRANT  EXECUTE  ON [dbo].[RecalcGuestBill_Root]  TO [GOperator]
GO


GRANT  EXECUTE  ON [dbo].[WorkAlloc_delcascade]  TO [GCManeger]
GO

DENY  EXECUTE  ON [dbo].[WorkAlloc_delcascade]  TO [GOperator] CASCADE 
GO
