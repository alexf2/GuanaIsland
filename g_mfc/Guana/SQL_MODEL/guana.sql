/*if exists (select name from master..sysdatabases where name = 'guana_bill') USE guana_bill

if exists (select name from master..sysdatabases where name = 'guana_bill')
 begin   
   if exists (select * from master..sysobjects where id = object_id('BillCategory') and type = 'U')
     drop table BillCategory 
   if exists (select * from master..sysobjects where id = object_id('Guest') and type = 'U')
     drop table Guest
   if exists (select * from master..sysobjects where id = object_id('GuestBill') and type = 'U')
     drop table GuestBill
   if exists (select * from master..sysobjects where id = object_id('AdditionalCharge') and type = 'U')
     drop table AdditionalCharge
   if exists (select * from master..sysobjects where id = object_id('CategoryGuest_BillCategory') and type = 'U')
     drop table CategoryGuest_BillCategory
   if exists (select * from master..sysobjects where id = object_id('Room') and type = 'U')
     drop table Room
   if exists (select * from master..sysobjects where id = object_id('GuestBillData') and type = 'U')
     drop table GuestBillData
   
 end
*/
USE master
GO
if exists (select name from master..sysdatabases where name = 'guana_bill') DROP DATABASE guana_bill


if exists (select name from master..sysdevices where name = 'dguana_bill_log')
  EXEC sp_dropdevice dguana_bill_log, DELFILE  
if exists (select name from master..sysdevices where name = 'dguana_bill')
  EXEC sp_dropdevice dguana_bill, DELFILE  
GO

USE master
GO
CREATE DATABASE guana_bill
	ON (name='dguana_bill', filename='f:\data\dguana_bill.dat', size=8MB) 
        LOG ON (name='dguana_bill_log', filename='f:\data\dguana_bill_log.dat', size=4MB)
GO


USE guana_bill
go

CREATE TABLE BillCategory (
       ID_BILL              int IDENTITY(1,1) not null,
       NAME                 varchar(30) NOT NULL
                                   CONSTRAINT CKC_UNIQUE25
                                          unique,
       CONSTRAINT XPKBillCategory 
              PRIMARY KEY nonclustered (ID_BILL)
)
go

CREATE TABLE CategoryGuest (
       ID_CAT               int IDENTITY(1,1) not null,
       NAMECAT              varchar(25) NOT NULL
                                   CONSTRAINT CKC_UNIQUE26
                                          unique,
       CONSTRAINT XPKCategoryGuest 
              PRIMARY KEY nonclustered (ID_CAT)
)
go


CREATE TABLE Guest (
       ID_GUEST             int IDENTITY(1,1) not null,
       ID_CAT               int NOT NULL,       
       FNAME                varchar(30) NOT NULL,
       LNAME                varchar(30) NOT NULL,
       CITY                 varchar(20) NOT NULL,

       STATE                varchar(20) NOT NULL,

       ADDRESS              varchar(50) NOT NULL,
       POST                 int NOT NULL
                                   CONSTRAINT CKC_POST7
                                          CHECK (POST > 0),
       DEPOSIT              money NOT NULL default 0
                                   CONSTRAINT CKC_DEPOSIT5
                                          CHECK (DEPOSIT >= 0),
       __NAMECAT              varchar(25) NOT NULL default 'REPLIC',
       CONSTRAINT XPKGuest 
              PRIMARY KEY nonclustered (ID_GUEST), 
       CONSTRAINT CFK_CATEGORY
              FOREIGN KEY (ID_CAT)
                             REFERENCES CategoryGuest,
      CONSTRAINT CKC_UNIQ_Guest unique( FNAME, LNAME )
)
go


CREATE TABLE GuestBill (
       BILL_NO              int IDENTITY(1,1) not null,

       ID_EMP               int NULL default NULL,

       ID_GUEST             int NOT NULL,
       FOLIO                varchar(15) NOT NULL,
       DATE_                datetime NOT NULL
                                   CONSTRAINT CURRENT_TIMESTAMP5
                                          DEFAULT CURRENT_TIMESTAMP
                                   CONSTRAINT CKC_DATE5
                                          CHECK (DATE_ >= '1 JAN 1990'),       
       
       HEARD_OF_GI          varchar(35) NOT NULL,
       DETAILS              varchar(64)  NULL,
       COMMENTS             varchar(256) NULL,

       _TOTAL_ADDITIONAL    money NOT NULL default 0,
       _TOTAL_PAYMENTS      money NOT NULL default 0, 
       _TOTAL_ROOMSPAY      money NOT NULL default 0,

       _SUBTOTAL            money NOT NULL default 0,       
       _TAX_AND_SERVICE     money NOT NULL default 0,
     
       _TOTAL               money NOT NULL default 0,

       _DEPOSIT_RECIEVED    money NOT NULL default 0,
       _BALANCE_DUE         money NOT NULL default 0,

       _TOTAL_GUEST        int NOT NULL default 0,
       _TOTAL_NIGHTS       int NOT NULL default 0,

       _PAY_ENROLMENT      tinyint NOT NULL default 0,
       
       CONSTRAINT XPKGuestBill 
              PRIMARY KEY nonclustered (BILL_NO), 
       CONSTRAINT CFK_Guest

              FOREIGN KEY (ID_GUEST)
                             REFERENCES Guest
)
go

CREATE TABLE Payments (
	ID_PAY	         int IDENTITY(1,1) not null,
      BILL_NO              int NOT NULL,
      AMOUNT               money NOT NULL
                                   CONSTRAINT CKC_AMOUNT_CHECK
                                          CHECK (AMOUNT >= 0),
      DESCRIPTION          varchar(64) NOT NULL,

      CONSTRAINT  XPKPayments
             PRIMARY KEY nonclustered (ID_PAY),
      CONSTRAINT  CFK_Pyments
             FOREIGN KEY (BILL_NO) REFERENCES GuestBill                             
)
go

CREATE TABLE AdditionalCharge (
       ID_AC	          int IDENTITY(1,1) not null,

       BILL_NO              int NOT NULL,
                                   
       ID_BILL              int NOT NULL,
       AMOUNT               money NOT NULL
              CONSTRAINT CKC_Amount check (AMOUNT >= 0 ), 
       MKDATE               datetime NULL DEFAULT NULL
                                   CONSTRAINT CKC_DATE55
                                          CHECK (MKDATE is null or ((not MKDATE is null) and MKDATE >= '1 JAN 1990')),

       CONSTRAINT XPKAdditionalCharge 
              PRIMARY KEY nonclustered (ID_AC), 
       CONSTRAINT CFK_BillCategory
              FOREIGN KEY (ID_BILL)
                             REFERENCES BillCategory, 
       CONSTRAINT CFK_GuestBill
              FOREIGN KEY (BILL_NO)
                             REFERENCES GuestBill,

       __NAME                 varchar(30) NOT NULL default 'REPLIC',
       -- CONSTRAINT CKC_UniqAdditionalCharge unique(BILL_NO, ID_BILL)       
)
go


CREATE TABLE CategoryGuest_BillCategory (
       ID_CAT               int NOT NULL,
       ID_BILL              int NOT NULL,

       CONSTRAINT XPKCategoryGuest_BillCategory 
              PRIMARY KEY nonclustered (ID_CAT, ID_BILL), 
       CONSTRAINT R_14
              FOREIGN KEY (ID_BILL)
                             REFERENCES BillCategory, 
       CONSTRAINT R_13
              FOREIGN KEY (ID_CAT)
                             REFERENCES CategoryGuest
)
go



CREATE TABLE Room (
       ID_ROOM              int IDENTITY(1,1) not null,
       NAME                 varchar(40) NOT NULL
                                   CONSTRAINT CKC_UNIQUE29
                                          unique,
       RATE                 money NOT NULL
                                   CONSTRAINT CKC_RATE5
                                          CHECK (RATE BETWEEN 0 AND 100000),
       CONSTRAINT XPKRoom 
              PRIMARY KEY nonclustered (ID_ROOM)
)
go


CREATE TABLE GuestBillData (
       ID_DB                int IDENTITY(1,1) not null,
       BILL_NO              int NOT NULL,                                   
       NO_OF_GUESTS         int NOT NULL
                                   CONSTRAINT CKC_NO_OF_GUESTS5
                                          CHECK (NO_OF_GUESTS BETWEEN 0 AND 50),
       DATE_IN              datetime NOT NULL
                                   CONSTRAINT CKC_DATA_IN5
                                          CHECK (DATE_IN >= '1 JAN 1990'),
       DATE_OUT             datetime NULL,
                                   
       ID_ROOM              int NOT NULL,
       _NIGHTS              int NOT NULL default 0,       
       _RATE                money NOT NULL default 0,       
       _CHARGE              money NOT NULL default 0,

       __NAME                 varchar(40) NOT NULL default 'REPLIC',

       CONSTRAINT CKC_DATA_OUT5
                                   CHECK ((DATE_OUT >= '1 JAN 1990' AND DATE_OUT >= DATE_IN) OR DATE_OUT IS NULL),
       
       -- CONSTRAINT CKC_UNIQ_GuestBillData unique( BILL_NO, ID_ROOM ),
       CONSTRAINT XPKGuestBillData 
              PRIMARY KEY nonclustered (ID_DB), 
       CONSTRAINT FKS_Room
              FOREIGN KEY (ID_ROOM)
                             REFERENCES Room, 
       CONSTRAINT FKS_GuestBill
              FOREIGN KEY (BILL_NO)
                             REFERENCES GuestBill
)
go

/*****************************************************************************
   AV-Start
*****************************************************************************/

CREATE TABLE OperationDescription (
	op_code smallint NOT NULL ,
	source varchar (100) NOT NULL ,
	description varchar (100) NOT NULL,

      CONSTRAINT XPKOperationDescription 
              PRIMARY KEY nonclustered (op_code)
) 
GO

CREATE TABLE Operation (
	id_op int IDENTITY (1, 1) NOT NULL ,
	user_ varchar (50) NOT NULL DEFAULT user_name(),
	date datetime NOT NULL DEFAULT getdate(),
	op_code smallint NOT NULL ,
	old_val varchar (100) NULL ,
	new_val varchar (100) NULL ,
	add_info varchar (500) NULL,

      CONSTRAINT XPKOperation 
              PRIMARY KEY nonclustered (id_op),
      CONSTRAINT FKS_Operation
              FOREIGN KEY (op_code)
                             REFERENCES OperationDescription
) 
GO

CREATE TABLE Operator (
	user_ varchar (50) NOT NULL,
	Name varchar (50) NOT NULL,

      CONSTRAINT XPKOperator
              PRIMARY KEY nonclustered (user_),
      CONSTRAINT CKC_UNIQ_Operator unique( user_, Name )
)
GO
/*
CREATE TABLE Invitee (
	id_invitee int IDENTITY (1, 1) NOT NULL ,
	id_guest int NOT NULL ,
	lname varchar (30) NOT NULL ,
	fname varchar (30) NOT NULL ,
	city varchar (20) NOT NULL ,
	state char (2) NOT NULL ,
	ADDRESS varchar (50) NOT NULL ,
	POST int NOT NULL 
) 
ALTER TABLE Invitee WITH NOCHECK ADD 
	CONSTRAINT PK_Invitee PRIMARY KEY  NONCLUSTERED 
	( id_invitee )   
ALTER TABLE Invitee ADD 
	CONSTRAINT FK_Invitee_Guest FOREIGN KEY 
	( id_guest ) REFERENCES Guest ( ID_GUEST )
*/

CREATE VIEW OperView
AS
SELECT Operator.Name, Operation.date, 
    OperationDescription.source, OperationDescription.description, 
    Operation.old_val, Operation.new_val, Operation.add_info, 
    Operation.op_code, Operation.user_
FROM Operation INNER JOIN
    OperationDescription ON 
    Operation.op_code = OperationDescription.op_code INNER JOIN
    Operator ON Operation.user_ = Operator.user_
go

/*****************************************************************************
   AV-End
*****************************************************************************/


--Second part of tables ######################################################

CREATE TABLE TimeType (
       ID_TYP               int IDENTITY,
       NAME                 varchar(20) NOT NULL unique,
       TIME_STA             datetime NOT NULL,
       TIME_END             datetime NOT NULL,
       PERCENT_PAY          int NOT NULL CHECK (PERCENT_PAY BETWEEN 0 AND 1000),

       CONSTRAINT XPKTimeType 
              PRIMARY KEY nonclustered (ID_TYP),
       CONSTRAINT CKCTimeType_SE CHECK (TIME_END > TIME_STA)
)
go


CREATE TABLE JobCategory (
       ID_CAT               int IDENTITY,
       NAME                 varchar(20) NOT NULL unique,
       COMMENTS             varchar(64) NULL  default null,

       CONSTRAINT XPKJobCategory 
              PRIMARY KEY nonclustered (ID_CAT)
)
go


CREATE TABLE Job (
       ID_JOB               int IDENTITY,
       ID_CAT               int NOT NULL,
       NAME                 varchar(40) NOT NULL unique,
       PAY                  money NOT NULL CHECK (PAY >= 0),

       CONSTRAINT XPKJob 
              PRIMARY KEY nonclustered (ID_JOB), 
       CONSTRAINT XFKJobJobCat
              FOREIGN KEY (ID_CAT)
                             REFERENCES JobCategory
)
go


CREATE TABLE Status (
       ID_STAT              int IDENTITY,
       NAME                 varchar(30) NOT NULL unique,

       CONSTRAINT XPKStatus 
              PRIMARY KEY nonclustered (ID_STAT)
)
go


CREATE TABLE Department (
       ID_DEP               int IDENTITY,
       NAME                 varchar(30) NOT NULL unique,

       CONSTRAINT XPKDepartment 
              PRIMARY KEY nonclustered (ID_DEP)
)
go


CREATE TABLE Language (
       ID_LAN               int IDENTITY,
       NAME                 varchar(20) NOT NULL unique,

       CONSTRAINT XPKLanguage 
              PRIMARY KEY nonclustered (ID_LAN)
)
go


CREATE TABLE Emplyee (
       ID_EMP               int IDENTITY,

       ID_JOB               int NULL default null,
       ID_STAT              int NOT NULL,
       ID_DEP               int NOT NULL,
       ID_LAN               int NOT NULL,

       FULL_NAME            varchar(64) NOT NULL unique,
       SSN                  varchar(11) NOT NULL,
       SEX                  bit NOT NULL,
       BIRTH_DAY            datetime NOT NULL CHECK (BIRTH_DAY >= '1 JAN 1900'),
       ADDRESS              varchar(40) NOT NULL,
       CITY                 varchar(20) NOT NULL,
       ZIP                  varchar(10) NOT NULL,
       HOME_PHONE           varchar(12) NULL default null,
       MOBIL_PHONE          varchar(12) NULL default null,
       PAGER                varchar(30) NULL default null,
       EMAIL                varchar(64) NULL default null,
       DATE_OF_JOB_START    datetime NULL default null,
       ID_EMP_BOSS          int NULL default null,

       CONSTRAINT XPKEmplyee 
              PRIMARY KEY nonclustered (ID_EMP), 

       CONSTRAINT XFKEmplyeeJob
              FOREIGN KEY (ID_JOB)
                             REFERENCES Job, 
       CONSTRAINT XFKEmplyeeStatus
              FOREIGN KEY (ID_STAT)
                             REFERENCES Status, 
       CONSTRAINT XFKEmplyeeDepartment
              FOREIGN KEY (ID_DEP)
                             REFERENCES Department, 
       CONSTRAINT XFKEmplyeeEmplyee
              FOREIGN KEY (ID_EMP_BOSS)
                             REFERENCES Emplyee (ID_EMP), 
       CONSTRAINT XFKEmplyeeLanguage
              FOREIGN KEY (ID_LAN)
                             REFERENCES Language,

       CONSTRAINT CKCSta CHECK 
                              (DATE_OF_JOB_START is null OR DATE_OF_JOB_START > BIRTH_DAY)
)
go


CREATE TABLE WorkAlloc (
       ID_PLAN              int IDENTITY,

       ID_JOB               int NOT NULL,
       ID_EMP               int NULL default null,
       CLOCK_START          datetime NOT NULL,
       CLOCK_END            datetime NOT NULL,
       PAY_RATE             money NOT NULL CHECK (PAY_RATE >= 0),

       CONSTRAINT XPKWorkAlloc 
              PRIMARY KEY nonclustered (ID_PLAN), 

       CONSTRAINT XFKWorkAllocJob
              FOREIGN KEY (ID_JOB)
                             REFERENCES Job, 
       CONSTRAINT XFKWorkAllocEmplyee
              FOREIGN KEY (ID_EMP)
                             REFERENCES Emplyee,

      CONSTRAINT CKCStaEnd CHECK (CLOCK_END > CLOCK_START)
)
go


CREATE TABLE WorkTimeLog (
       DATE_                datetime NOT NULL,
       ID_PLAN              int NOT NULL,

       CLOCK_IN             datetime NOT NULL,
       CLOCK_OUT            datetime NULL,
       
       CONSTRAINT XPKWorkTimeLog 
              PRIMARY KEY nonclustered (DATE_, ID_PLAN), 
        
       CONSTRAINT XFKWorkTimeLogWorkAlloc
              FOREIGN KEY (ID_PLAN)
                             REFERENCES WorkAlloc,

      CONSTRAINT CKCClockInOut CHECK 
              (CLOCK_OUT is null OR (not (CLOCK_OUT is null) AND (CLOCK_IN < CLOCK_OUT))),
      CONSTRAINT CKCClockOut CHECK (CLOCK_OUT >= '1 JAN 1900'),
      CONSTRAINT CKCClockIn CHECK (CLOCK_IN is null OR (not (CLOCK_IN is null) AND CLOCK_IN >= '1 JAN 1900'))
)
go

ALTER TABLE GuestBill 
  ADD CONSTRAINT XFKGuestEmpyee FOREIGN KEY (ID_EMP) REFERENCES Emplyee (ID_EMP)
go
--Second part of tables ######################################################




create procedure AddCategoryes
  @bill_no integer, @id_guest integer
as
 begin
   declare @id_cat integer

   --SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
   begin transaction AddCat

   select @id_cat = ID_CAT from Guest where ID_GUEST = @id_guest
   insert into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT)
     select BILL_NO = @bill_no, cb.ID_BILL, AMOUNT = 0 
       from CategoryGuest_BillCategory cb INNER JOIN BillCategory b 
            ON cb.ID_BILL = b.ID_BILL and cb.ID_CAT = @id_cat

   commit transaction AddCat
   --SET TRANSACTION ISOLATION LEVEL READ COMMITED

 end
go

create procedure PayEnroll
  @bill_no integer, @flag integer
as
 begin
  declare @deposit  money
  declare @id_guest integer
  declare @_total money
  declare @_balance_due money, @deposit_rsp money

  --SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
  begin transaction

  select 
    @id_guest = ID_GUEST, @_total = _TOTAL, @deposit_rsp = _DEPOSIT_RECIEVED 
      from GuestBill where BILL_NO = @bill_no 
  select @deposit = DEPOSIT from Guest where ID_GUEST = @id_guest

  if( @flag = 1 )
   begin
    if( @deposit < @_total )
     begin
       select @_balance_due = @_total - @deposit
       select @deposit = 0
     end
    else
     begin
       select @_balance_due = 0
       select @deposit = @deposit - @_total
     end

    update Guest set DEPOSIT = @deposit where ID_GUEST = @id_guest
    update GuestBill set 
       _DEPOSIT_RECIEVED = @_total - @_balance_due,
       _BALANCE_DUE = @_balance_due,
       _PAY_ENROLMENT = 1
         where BILL_NO = @bill_no
   end
  else
   begin
    select @deposit = @deposit + @deposit_rsp

    update Guest set DEPOSIT = @deposit where ID_GUEST = @id_guest
    update GuestBill set 
       _DEPOSIT_RECIEVED = 0,
       _BALANCE_DUE = 0,
       _PAY_ENROLMENT = 0
         where BILL_NO = @bill_no
   end
   
  commit transaction
  --SET TRANSACTION ISOLATION LEVEL READ COMMITED

 end
go

create procedure Get_BillRecommended
  @id_cat2 integer
as
  select b.ID_BILL, b.NAME  
    from CategoryGuest c INNER JOIN CategoryGuest_BillCategory cb ON c.ID_CAT = cb.ID_CAT
      INNER JOIN BillCategory b ON b.ID_BILL = cb.ID_BILL
	  where c.ID_CAT = @id_cat2
go

create procedure RecalcGuestBill_Root
  @bill_no integer
as
 begin 
  declare @subtotal money,
          @tax money,
          @total money,
          @payments money,
          @cnt_pyments integer

  select @subtotal = g._TOTAL_ADDITIONAL +  g._TOTAL_ROOMSPAY 
    from GuestBill g
      where g.BILL_NO = @bill_no
  select @tax = @subtotal / 100.0 * 17.0
  
  select @payments = sum(a.AMOUNT), @cnt_pyments = count(a.AMOUNT)
    from Payments a
      where a.BILL_NO = @bill_no
  if @cnt_pyments = 0
    select @payments = 0

  select @total = @subtotal + @tax - @payments

  update GuestBill set 
    _SUBTOTAL = @subtotal,
    _TAX_AND_SERVICE = @tax,
    _TOTAL = @total,
    _TOTAL_PAYMENTS = @payments
        where BILL_NO = @bill_no  
 end
go

create procedure RecalcGuestBill_Data
 	@bill_no integer
as
 begin
    declare @summcharge money, @cnt integer,  @summnights integer

    select @summcharge = sum(a._CHARGE), @cnt = count(a.ID_DB),
           @summnights = sum(a._NIGHTS)
      from GuestBillData a where a.BILL_NO = @bill_no

    if @cnt = 0
      select @summcharge = 0, @summnights = 0

    update GuestBill set 
       _TOTAL_ROOMSPAY = @summcharge,
       _TOTAL_GUEST    = @cnt,
       _TOTAL_NIGHTS   = @summnights
          where BILL_NO = @bill_no      

    exec RecalcGuestBill_Root @bill_no
 end
go

create trigger tU_Guest_Repl on Guest for INSERT, UPDATE as
 begin
   if not UPDATE(ID_CAT) return

   update Guest  set
     __NAMECAT = cg.NAMECAT
     from Guest g INNER JOIN inserted i ON g.ID_GUEST=i.ID_GUEST
          INNER JOIN CategoryGuest cg ON cg.ID_CAT = i.ID_CAT  
 end
go

create trigger tU_AdditionalCharge_Repl on AdditionalCharge for INSERT, UPDATE as
 begin
   if not UPDATE(ID_BILL) return

   update AdditionalCharge set
     __NAME = b.NAME
     from AdditionalCharge a INNER JOIN inserted i ON a.BILL_NO=i.BILL_NO AND a.ID_BILL=i.ID_BILL
          INNER JOIN BillCategory b ON b.ID_BILL = i.ID_BILL
 end
go
--***** AF start
create trigger tU_AssEmplyee on GuestBill for INSERT as
 begin
   declare @iID_EMP integer

   select @iID_EMP = ID_EMP from      
     Operator o INNER JOIN Emplyee e ON e.FULL_NAME = o.NAME 
        where o.user_ = user_name()

   update GuestBill set 
     ID_EMP = @iID_EMP
     from GuestBill b INNER JOIN inserted i ON b.BILL_NO = i.BILL_NO 
       
 end
go
--***** AF end
create trigger tU_GuestBillData_Repl on GuestBillData for INSERT, UPDATE as
 begin
   if not UPDATE(ID_ROOM) return

   update GuestBillData set
     __NAME = r.NAME
     from GuestBillData g INNER JOIN inserted i ON g.ID_DB=i.ID_DB 
          INNER JOIN Room r ON r.ID_ROOM = i.ID_ROOM
 end
go

create trigger tU_Payments on Payments for INSERT, UPDATE as
 begin
   if not (UPDATE(AMOUNT) or UPDATE(BILL_NO)) return

   declare @bill_no integer
   declare tCurs cursor for select distinct BILL_NO  from inserted
   open tCurs

   fetch next from  tCurs into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec RecalcGuestBill_Root  @bill_no
      fetch next from  tCurs into @bill_no
    end

   close tCurs
   deallocate tCurs 
 end
go

create trigger tU_Payments2 on Payments for DELETE as
 begin
   declare @bill_no integer
   declare tCurs cursor for select distinct BILL_NO  from inserted
   open tCurs

   fetch next from  tCurs into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec RecalcGuestBill_Root  @bill_no
      fetch next from  tCurs into @bill_no
    end

   close tCurs
   deallocate tCurs 
 end
go

create trigger tU_GuestBillDataCalc_ins on GuestBillData for INSERT as
 begin
   if not (UPDATE(ID_ROOM) or UPDATE(DATE_IN) or UPDATE(DATE_OUT) or UPDATE(NO_OF_GUESTS)) return
   update GuestBillData  set 
     _NIGHTS = DATEDIFF( day, i.DATE_IN, i.DATE_OUT ),
     _RATE = r.RATE,
     _CHARGE = r.RATE * DATEDIFF( day, i.DATE_IN, i.DATE_OUT ) * g.NO_OF_GUESTS
        -- cast((i.DATE_OUT - i.DATE_IN) as int)
     from GuestBillData g INNER JOIN inserted i ON g.ID_DB=i.ID_DB 
          INNER JOIN Room r ON r.ID_ROOM = i.ID_ROOM  

   declare @bill_no integer
   declare tCurs3 cursor for select distinct BILL_NO  from inserted
   open tCurs3

   fetch next from  tCurs3 into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec RecalcGuestBill_Data  @bill_no
      fetch next from  tCurs3 into @bill_no
    end

   close tCurs3
   deallocate tCurs3
 end
go

create trigger tU_GuestBillDataCalc_upd on GuestBillData for UPDATE as
 begin
   if not (UPDATE(ID_ROOM) or UPDATE(DATE_IN) or UPDATE(DATE_OUT) or UPDATE(NO_OF_GUESTS)) return

   if UPDATE(ID_ROOM)
     update GuestBillData  set 
       _NIGHTS = DATEDIFF( day, i.DATE_IN, i.DATE_OUT ),
       _RATE = r.RATE,
       _CHARGE = r.RATE * DATEDIFF( day, i.DATE_IN, i.DATE_OUT ) * g.NO_OF_GUESTS
          -- cast((i.DATE_OUT - i.DATE_IN) as int)
       from GuestBillData g INNER JOIN inserted i ON g.ID_DB=i.ID_DB 
            INNER JOIN Room r ON r.ID_ROOM = i.ID_ROOM  
   else
     update GuestBillData  set 
       _NIGHTS = DATEDIFF( day, i.DATE_IN, i.DATE_OUT ),
       _CHARGE = g._RATE * DATEDIFF( day, i.DATE_IN, i.DATE_OUT ) * g.NO_OF_GUESTS
          -- cast((i.DATE_OUT - i.DATE_IN) as int)
       from GuestBillData g INNER JOIN inserted i ON g.ID_DB=i.ID_DB 
            INNER JOIN Room r ON r.ID_ROOM = i.ID_ROOM  


   declare @bill_no integer
   declare tCurs3 cursor for select distinct BILL_NO  from inserted
   open tCurs3

   fetch next from  tCurs3 into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec RecalcGuestBill_Data  @bill_no
      fetch next from  tCurs3 into @bill_no
    end

   close tCurs3
   deallocate tCurs3
 end
go


create  trigger tU_GuestBillDataCalc2 on GuestBillData for DELETE as
 begin   
   declare @bill_no integer
   declare tCurs cursor for select distinct BILL_NO  from deleted
   open tCurs

   fetch next from  tCurs into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec RecalcGuestBill_Data  @bill_no
      fetch next from  tCurs into @bill_no
    end

   close tCurs
   deallocate tCurs
 end
go



create procedure RecalcGuestBill
 	@bill_no integer,
	@relevant integer
as
 begin
   declare @cnt_r integer
   declare @summam money

   if  @relevant = 0
    begin
      select @cnt_r = count(a.AMOUNT), @summam = sum(a.AMOUNT) 
         from AdditionalCharge a where a.BILL_NO = @bill_no

      if @cnt_r = 0
         select @summam = 0

      update GuestBill set _TOTAL_ADDITIONAL = @summam 
        where BILL_NO = @bill_no      
    end

   exec RecalcGuestBill_Root @bill_no
 end
go

create trigger tU_AdditionalCharge_Recalc on AdditionalCharge for INSERT, UPDATE as
 begin
   declare @cnt_null integer, @cnt_not_null integer
    
   declare @bill_no integer, @id_bill integer
   declare tCurs cursor for select distinct BILL_NO, ID_BILL  from inserted
   open tCurs

   fetch next from  tCurs into @bill_no, @id_bill
   while @@FETCH_STATUS = 0
    begin
      select @cnt_null = sum(
               CASE MKDATE
                 WHEN null THEN 1
                 ELSE 0
               END),
             @cnt_not_null = sum(
               CASE MKDATE
                 WHEN null THEN 0
                 ELSE 1
               END)
        from AdditionalCharge 
          where BILL_NO = @bill_no and ID_BILL = @id_bill

      if @cnt_null IS NULL select @cnt_null = 0
      if @cnt_not_null IS NULL select @cnt_not_null = 0

      if @cnt_not_null <> 0 and @cnt_null <> 0 
       begin
          close tCurs
          deallocate tCurs
          raiserror 30002 'You have set summaryzed and detaild charge in the same category'
          rollback transaction
          return
       end

       if @cnt_null <> 0 and @cnt_null <> 1 
       begin          
          close tCurs
          deallocate tCurs
          raiserror 30002 'You may set summaryzed charge only once'
          rollback transaction
          return
       end

      fetch next from  tCurs into @bill_no, @id_bill      
    end

   close tCurs
   deallocate tCurs
 
   declare tCurs2 cursor for select distinct BILL_NO, ID_BILL  from inserted
   open tCurs2 

   fetch next from  tCurs2 into @bill_no, @id_bill
   while @@FETCH_STATUS = 0
    begin      
      exec RecalcGuestBill  @bill_no, 0        
      fetch next from  tCurs2 into @bill_no, @id_bill
    end

   close tCurs2
   deallocate tCurs2   
 end
go

create trigger tU_AdditionalCharge_Recalc2 on AdditionalCharge for DELETE as
 begin
   declare @bill_no integer
   declare tCurs cursor for select distinct BILL_NO  from deleted
   open tCurs

   fetch next from  tCurs into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec RecalcGuestBill  @bill_no, 0  
      fetch next from  tCurs into @bill_no
    end

   close tCurs
   deallocate tCurs
 end
go


/*****************************************************************************
   AV-Start
*****************************************************************************/
insert into OperationDescription (op_code,source,description)
values (110,'insdel_Guest_trigger','New Guest Added')
insert into OperationDescription (op_code,source,description)
values (120,'insdel_Guest_trigger','Guest Deleted')
insert into OperationDescription (op_code,source,description)
values (101,'upd_Guest_trigger','Guest DEPOSIT changed')
insert into OperationDescription (op_code,source,description)
values (102,'upd_Guest_trigger','Guest Category changed')
insert into OperationDescription (op_code,source,description)
values (103,'upd_Guest_trigger','Guest Name changed')
insert into OperationDescription (op_code,source,description)
values (104,'upd_Guest_trigger','Guest Address changed')

/*insert into OperationDescription (op_code,source,description)
values (210,'insdel_Invitee_trigger','New Invitee Added')
insert into OperationDescription (op_code,source,description)
values (220,'insdel_Invitee_trigger','Invitee Deleted')
insert into OperationDescription (op_code,source,description)
values (201,'upd_Invitee_trigger','Invitee Name changed')
insert into OperationDescription (op_code,source,description)
values (202,'upd_Invitee_trigger','Invitee Address changed')*/

insert into OperationDescription (op_code,source,description)
values (310,'insdel_Payment_trigger','New Payment Added')
insert into OperationDescription (op_code,source,description)
values (320,'insdel_Payment_trigger','Payment Deleted')
insert into OperationDescription (op_code,source,description)
values (301,'upd_Payment_trigger','Payment amount changed')

insert into OperationDescription (op_code,source,description)
values (410,'insdel_GuestBillData_trigger','New Room Added')
insert into OperationDescription (op_code,source,description)
values (420,'insdel_GuestBillData_trigger','Room Deleted')
insert into OperationDescription (op_code,source,description)
values (401,'upd_GuestBillData_trigger','Num of guests changed')
insert into OperationDescription (op_code,source,description)
values (402,'upd_GuestBillData_trigger','Date in changed')
insert into OperationDescription (op_code,source,description)
values (403,'upd_GuestBillData_trigger','Date out changed')
insert into OperationDescription (op_code,source,description)
values (404,'upd_GuestBillData_trigger','Room changed')

insert into OperationDescription (op_code,source,description)
values (510,'insdel_AdditionalCharge_trigger','Additional Charge Added')
insert into OperationDescription (op_code,source,description)
values (520,'insdel_AdditionalCharge_trigger','Additional Charge  Deleted')
insert into OperationDescription (op_code,source,description)
values (501,'upd_AdditionalCharge_trigger','Charge Amount changed')

insert into Operator values ('dbo','Alex Verbitsky')

insert into Operator values ('GAdmin','Alex GAdmin')
insert into Operator values ('GOperator','Alex GOperator')
insert into Operator values ('GCManeger','Alex GCManeger')
insert into Operator values ('GVahter','Alex GVahter')
insert into Operator values ('GBuh','Alex GBuh')
insert into Operator values ('GGuest','Alex GGuest')
insert into Operator values ('GDir','Alex GDir')

go
/*****************************************************************************
   AV-End
*****************************************************************************/

insert into Department (NAME) values('Department 1')
insert into Department (NAME) values('Department 2')
insert into Department (NAME) values('Department 3')
insert into Department (NAME) values('Department 4')
insert into Department (NAME) values('Department 5')
insert into Department (NAME) values('Department 6')
go

insert into Status (NAME) values('Full-time')
insert into Status (NAME) values('Part-time')
insert into Status (NAME) values('Temporary')
go

insert into Language (NAME) values('English')
insert into Language (NAME) values('German')
insert into Language (NAME) values('Russian')
insert into Language (NAME) values('French')
insert into Language (NAME) values('Spanish')
go

insert into TimeType (NAME, TIME_STA, TIME_END, PERCENT_PAY) 
  values( 'Regular', '1 JAN 1900 9:00:00 AM', '1 JAN 1900 6:00:00 PM', 100 )
insert into TimeType (NAME, TIME_STA, TIME_END, PERCENT_PAY) 
  values( 'Overtime', '1 JAN 1900 9:00:00 AM', '1 JAN 1900 6:00:00 PM', 150 )
insert into TimeType (NAME, TIME_STA, TIME_END, PERCENT_PAY) 
  values( 'Extra overtime', '1 JAN 1900 9:00:00 AM', '1 JAN 1900 6:00:00 PM', 200 )

insert into TimeType (NAME, TIME_STA, TIME_END, PERCENT_PAY) 
  values( 'Regular night', '1 JAN 1900 6:00:00 PM', '2 JAN 1900 9:00:00 AM', 200 )
insert into TimeType (NAME, TIME_STA, TIME_END, PERCENT_PAY) 
  values( 'Overtime night', '1 JAN 1900 6:00:00 PM', '2 JAN 1900 9:00:00 AM', 300 )
insert into TimeType (NAME, TIME_STA, TIME_END, PERCENT_PAY) 
  values( 'Extra overtime night', '1 JAN 1900 6:00:00 PM', '2 JAN 1900 9:00:00 AM', 500 )
go

insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 1', 'cat1 comment' )
insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 2', 'cat2 comment' )
insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 3', 'cat3 comment' )
insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 4', 'cat4 comment' )
insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 5', 'cat5 comment' )
insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 6', 'cat6 comment' )
insert into JobCategory (NAME, COMMENTS) 
  values( 'Jobcategory 7', 'cat7 comment' )
go

insert into Job (ID_CAT, NAME, PAY) values( 1, 'Job 1', 5 )
insert into Job (ID_CAT, NAME, PAY) values( 1, 'Job 2', 7 )
insert into Job (ID_CAT, NAME, PAY) values( 2, 'Job 3', 2 )
insert into Job (ID_CAT, NAME, PAY) values( 2, 'Job 4', 8 )
insert into Job (ID_CAT, NAME, PAY) values( 3, 'Job 5', 10 )
insert into Job (ID_CAT, NAME, PAY) values( 3, 'Job 6', 7 )
insert into Job (ID_CAT, NAME, PAY) values( 3, 'Job 7', 17 )
insert into Job (ID_CAT, NAME, PAY) values( 1, 'Job 8', 12 )
insert into Job (ID_CAT, NAME, PAY) values( 4, 'Job 9', 20 )
insert into Job (ID_CAT, NAME, PAY) values( 4, 'Job 10', 50 )
insert into Job (ID_CAT, NAME, PAY) values( 4, 'Job 11', 10 )
insert into Job (ID_CAT, NAME, PAY) values( 4, 'Job 12', 15 )
insert into Job (ID_CAT, NAME, PAY) values( 5, 'Job 13', 21 )
insert into Job (ID_CAT, NAME, PAY) values( 5, 'Job 14', 6 )
insert into Job (ID_CAT, NAME, PAY) values( 5, 'Job 15', 25 )
go

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( null, 1, 1, 1, 'Alex Verbitsky', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', NULL )


insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( null, 2, 1, 1, 'Alex GAdmin', '111-11-1112', 0, '1 JAN 1960', 'Address 2', 'City 2',
   '11111-1102', '111-11-02', '222-22-02', '999-99-02', 'aaa@bbb.02', '2 JAN 1990', NULL )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'Alex GOperator', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 1 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'Alex GCManeger', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 2 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'Alex GVahter', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 1 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'Alex GBuh', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 2 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'Alex GDir', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 1 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'FName8 LName8', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 2 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'FName9 LName9', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 1 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'FName10 LName10', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 2 )

insert into Emplyee (ID_JOB, ID_STAT, ID_DEP, ID_LAN, FULL_NAME, SSN, SEX,
 BIRTH_DAY, ADDRESS, CITY, ZIP, HOME_PHONE, MOBIL_PHONE, PAGER, 
 EMAIL, DATE_OF_JOB_START, ID_EMP_BOSS) 
  values( 1, 1, 1, 1, 'FName11 LName11', '111-11-1111', 0, '1 JAN 1960', 'Address 1', 'City 1',
   '11111-1111', '111-11-11', '222-22-22', '999-99-99', 'aaa@bbb.01', '1 JAN 1990', 1 )
go


insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 1, 3, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 7 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 2, 4, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 10 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 3, 4, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 15 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 4, 5, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 10 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 5, 6, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 20 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 1, 7, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 22 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 2, 7, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM', 5 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 3, 7, '1 JAN 1900 6:00 AM', '1 JAN 1900 8:00 PM', 5 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 5, 8, '1 JAN 1900 6:00 AM', '1 JAN 1900 8:00 PM', 10 )
insert into WorkAlloc ( ID_JOB, ID_EMP, CLOCK_START, CLOCK_END, PAY_RATE ) 
  values ( 5, 8, '1 JAN 1900 6:00 PM', '2 JAN 1900 8:00 AM', 11 )
go

insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '1 JAN 1998', 1, '1 JAN 1900 9:00 AM', '1 JAN 1900 6:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '2 JAN 1998', 1, '1 JAN 1900 9:00 AM', '1 JAN 1900 6:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '3 JAN 1998', 1, '1 JAN 1900 9:00 AM', '1 JAN 1900 6:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '4 JAN 1998', 1, '1 JAN 1900 9:00 AM', '1 JAN 1900 6:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '5 JAN 1998', 1, '1 JAN 1900 9:00 AM', '1 JAN 1900 6:00 PM' )

insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '1 JAN 1998', 2, '1 JAN 1900 10:00 AM', '1 JAN 1900 6:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '2 JAN 1998', 2, '1 JAN 1900 10:00 AM', '1 JAN 1900 6:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '3 JAN 1998', 2, '1 JAN 1900 10:00 AM', '1 JAN 1900 6:00 PM' )

insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '1 JAN 1998', 3, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '2 JAN 1998', 3, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '3 JAN 1998', 3, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '4 JAN 1998', 3, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM' )
insert into WorkTimeLog ( DATE_, ID_PLAN, CLOCK_IN, CLOCK_OUT ) 
  values ( '5 JAN 1998', 3, '1 JAN 1900 9:00 AM', '1 JAN 1900 5:00 PM' )
go

insert into CategoryGuest (NAMECAT) values ('Normal')
insert into CategoryGuest (NAMECAT) values ('FS - HJ/GJ&FAMILY')
insert into CategoryGuest (NAMECAT) values ('FS - FRIENDS')
insert into CategoryGuest (NAMECAT) values ('TFG/MF')
insert into CategoryGuest (NAMECAT) values ('GIWS')
insert into CategoryGuest (NAMECAT) values ('GIG(MGT)')
insert into CategoryGuest (NAMECAT) values ('GIG(Cns, Emp, etc)')
go

insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (1, 'FName1', 'Lname1', 'City1', 'AL', 'Address1', 1, 1 )
insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (1, 'FName2', 'Lname2', 'City2', 'AL', 'Address2', 2, 2 )
insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (1, 'FName3', 'Lname3', 'City3', 'AL', 'Address3', 3, 3 )

insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (2, 'FName11', 'Lname11', 'City11', 'AL', 'Address11', 11, 11 )
insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (2, 'FName12', 'Lname12', 'City12', 'AL', 'Address12', 12, 12 )
insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (2, 'FName13', 'Lname13', 'City13', 'AL', 'Address13', 13, 13 )

insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (3, 'FName21', 'Lname21', 'City21', 'AL', 'Address21', 21, 21 )
insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (3, 'FName22', 'Lname22', 'City22', 'AL', 'Address22', 22, 22 )
insert into Guest (ID_CAT, FNAME, LNAME, CITY, STATE, ADDRESS, POST, DEPOSIT )
  values (3, 'FName23', 'Lname23', 'City23', 'AL', 'Address23', 23, 23 )
go

insert into Room (NAME, RATE) values( 'Room 1', 1 )
insert into Room (NAME, RATE) values( 'Room 2', 2 )
insert into Room (NAME, RATE) values( 'Room 3', 3 )
insert into Room (NAME, RATE) values( 'Room 4', 4 )
insert into Room (NAME, RATE) values( 'Room 5', 5 )
insert into Room (NAME, RATE) values( 'Room 6', 6 )
insert into Room (NAME, RATE) values( 'Room 7', 7 )
insert into Room (NAME, RATE) values( 'Room 8', 8 )
insert into Room (NAME, RATE) values( 'Room 9', 9 )
insert into Room (NAME, RATE) values( 'Room 10', 10 )
insert into Room (NAME, RATE) values( 'Room 11', 11 )
insert into Room (NAME, RATE) values( 'Room 12', 12 )
insert into Room (NAME, RATE) values( 'Room 13', 13 )
go

insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 1, '1 JAN 1991', 'Folio 1', 'HEARD_OF_GI 1', 'DETAILS 1', 'DESCRIPTION 1', 3 )

insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 1, '1 JAN 1992', 'Folio 2', 'HEARD_OF_GI 2', 'DETAILS 2', 'DESCRIPTION 2', 3)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 1, '1 JAN 1993', 'Folio 3', 'HEARD_OF_GI 3', 'DETAILS 3', 'DESCRIPTION 3', 3)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 1, '1 JAN 1994', 'Folio 4', 'HEARD_OF_GI 4', 'DETAILS 4', 'DESCRIPTION 4', 4)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 1, '1 JAN 1995', 'Folio 5', 'HEARD_OF_GI 5', 'DETAILS 5', 'DESCRIPTION 5', 4)

insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 2, '11 JAN 1991', 'Folio 11', 'HEARD_OF_GI 11', 'DETAILS 11', 'DESCRIPTION 11', 4)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 2, '11 JAN 1992', 'Folio 12', 'HEARD_OF_GI 12', 'DETAILS 12', 'DESCRIPTION 12', 3)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 2, '11 JAN 1993', 'Folio 13', 'HEARD_OF_GI 13', 'DETAILS 13', 'DESCRIPTION 13', 4)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 2, '11 JAN 1994', 'Folio 14', 'HEARD_OF_GI 14', 'DETAILS 14', 'DESCRIPTION 14', 4)
insert into GuestBill( ID_GUEST, DATE_, FOLIO, HEARD_OF_GI, DETAILS, COMMENTS, ID_EMP )
  values( 2, '11 JAN 1995', 'Folio 15', 'HEARD_OF_GI 15', 'DETAILS 15', 'DESCRIPTION 15', 3)
go


insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 1, 1, '1 JAN 1991', '1 MAR 1991', 1, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 1, 2, '1 JAN 1991', '1 MAR 1991', 2, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 1, 3, '1 JAN 1991', '1 MAR 1991', 3, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 1, 4, '1 JAN 1991', '1 MAR 1991', 4, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 1, 5, '1 JAN 1991', '1 MAR 1991', 5, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 2, 11, '21 JAN 1991', '21 MAR 1991', 3, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 2, 12, '21 JAN 1991', '21 MAR 1991', 4, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 3, 21, '20 JAN 1991', '20 MAR 1991', 1, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 3, 22, '20 JAN 1991', '20 MAR 1991', 2, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 3, 22, '20 JAN 1991', '20 MAR 1991', 3, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 4, 21, '22 JAN 1991', '22 MAR 1991', 1, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 4, 22, '22 JAN 1991', '22 MAR 1991', 2, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 4, 22, '22 JAN 1991', '22 MAR 1991', 3, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 5, 5, '5 JAN 1991', '5 FEB 1991', 5, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 6, 6, '6 JAN 1991', '5 FEB 1991', 5, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 7, 6, '6 JAN 1991', '5 FEB 1991', 5, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 8, 6, '6 JAN 1991', '5 FEB 1991', 5, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 9, 6, '6 JAN 1991', '5 FEB 1991', 5, 0, 0, 0 )

insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 10, 6, '6 JAN 1991', '5 FEB 1991', 5, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 10, 1, '1 JAN 1991', '1 FEB 1991', 1, 0, 0, 0 )
insert into GuestBillData( BILL_NO, NO_OF_GUESTS, DATE_IN, DATE_OUT, ID_ROOM, _NIGHTS, _CHARGE, _RATE )
  values( 10, 1, '1 JAN 1991', '1 FEB 1991', 2, 0, 0, 0 )


insert  into BillCategory (NAME) values( 'Bar' )
insert  into BillCategory (NAME) values( 'Meals' )
insert  into BillCategory (NAME) values( 'Phone' )
insert  into BillCategory (NAME) values( 'Fax' )
insert  into BillCategory (NAME) values( 'Incoming Fax' )
insert  into BillCategory (NAME) values( 'Boutique' )
insert  into BillCategory (NAME) values( 'Taxi Arrival & Departure' )
insert  into BillCategory (NAME) values( 'Taxi other' )
insert  into BillCategory (NAME) values( 'Boat Arrival & Departure' )
insert  into BillCategory (NAME) values( 'Extra boat runs' )
insert  into BillCategory (NAME) values( 'Outside boat charter' )
insert  into BillCategory (NAME) values( 'Sunset cruise' )
insert  into BillCategory (NAME) values( 'Snorkel trips' )
insert  into BillCategory (NAME) values( 'Deep sea fishing' )
insert  into BillCategory (NAME) values( 'Diving' )
insert  into BillCategory (NAME) values( 'Castaway picnic' )
insert  into BillCategory (NAME) values( 'Water sports' )
insert  into BillCategory (NAME) values( 'Massage' )
insert  into BillCategory (NAME) values( 'Other' )

insert  into BillCategory (NAME) values( 'Outside labour' )
insert  into BillCategory (NAME) values( 'Inside labour' )
insert  into BillCategory (NAME) values( 'Outside overtime' )
insert  into BillCategory (NAME) values( 'Inside overtime' )
insert  into BillCategory (NAME) values( 'Cessna meals' )
insert  into BillCategory (NAME) values( 'Meals G/H' )
insert  into BillCategory (NAME) values( 'Guest Meals' )
insert  into BillCategory (NAME) values( 'Babysitting' )
insert  into BillCategory (NAME) values( 'Taxi transfer' )
insert  into BillCategory (NAME) values( 'Boat transfer' )
insert  into BillCategory (NAME) values( 'GI boat charter' )
insert  into BillCategory (NAME) values( 'Outside charter' )
insert  into BillCategory (NAME) values( 'Office Supplies' )
insert  into BillCategory (NAME) values( 'Vehicle use' )
insert  into BillCategory (NAME) values( 'Pool maintenance' )

insert  into BillCategory (NAME) values( 'Water skiing' )
go


insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(1, 1)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(1, 2)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(1, 3)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(1, 4)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(1, 5)

insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(2, 6)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(2, 7)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(2, 8)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(2, 9)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(2, 10)

insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(3, 11)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(3, 12)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(3, 13)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(3, 14)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(3, 15)

insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(4, 16)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(4, 17)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(4, 18)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(4, 19)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(4, 20)

insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 1)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 2)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 3)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 4)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 5)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 6)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(5, 7)

insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 1)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 2)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 3)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 4)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 5)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 6)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(6, 7)

insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 1)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 2)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 3)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 4)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 5)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 6)
insert  into CategoryGuest_BillCategory (ID_CAT, ID_BILL) values(7, 7)
go

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 1, 10, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 2, 11, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 3, 12, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 4, 13, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 5, 14, '19910101')
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 5, 14, '19910102')
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 5, 14, '19910103')
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(1, 5, 14, '19910104')

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(2, 1, 20, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(2, 2, 21, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(2, 3, 22, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(2, 4, 23, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(2, 5, 24, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(3, 1, 30, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(3, 2, 31, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(3, 3, 32, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(3, 4, 33, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(3, 5, 34, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(4, 1, 40, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(4, 2, 41, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(4, 3, 42, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(4, 4, 43, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(4, 5, 44, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(5, 1, 50, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(5, 2, 51, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(5, 3, 52, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(5, 4, 53, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(5, 5, 54, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(6, 10, 111, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(6, 11, 112, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(6, 12, 113, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(6, 13, 114, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(6, 14, 115, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(7, 10, 121, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(7, 11, 122, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(7, 12, 123, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(7, 13, 124, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(7, 14, 125, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(8, 10, 131, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(8, 11, 132, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(8, 12, 133, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(8, 13, 134, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(8, 14, 135, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(9, 10, 141, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(9, 11, 142, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(9, 12, 143, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(9, 13, 144, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(9, 14, 145, null)

insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(10, 10, 151, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(10, 11, 152, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(10, 12, 153, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(10, 13, 154, null)
insert  into AdditionalCharge (BILL_NO, ID_BILL, AMOUNT, MKDATE) values(10, 14, 155, null)


go


-- ReportByCategory procedures
CREATE PROCEDURE procAdditionalCharge;1 
   @CategoryId Int, @DateFrom DateTime, @DateTo DateTime
AS
   SELECT AC.Bill_No, BC.Name, AC.Amount
   FROM 
      AdditionalCharge AC INNER JOIN 
      BillCategory BC ON AC.Id_Bill = BC.Id_Bill INNER JOIN
      GuestBill GB ON GB.Bill_No = AC.Bill_No INNER JOIN
      Guest G ON G.Id_Guest = GB.Id_Guest
   WHERE
      G.Id_Cat = @CategoryId AND
      GB.Date_ BETWEEN @DateFrom AND @DateTo
   ORDER BY
      AC.Bill_No
GO
CREATE PROCEDURE procGuestData;1
   @CategoryId Int, @DateFrom DateTime, @DateTo DateTime
AS 
   SELECT 
      GB.Bill_No, GB.Folio, G.FName+' '+G.LName AS FullName, GB._Total_Guest, 
      GB._Total_Nights, GB._Total_RoomSpay
   FROM 
      Guest G INNER JOIN
      GuestBill GB ON G.Id_Guest = GB.Id_Guest
   WHERE
      G.ID_Cat = @CategoryID AND
      GB.Date_ BETWEEN @DateFrom AND @DateTo
   ORDER BY
      GB.Bill_No
GO
CREATE PROCEDURE procCategoryName;1
   @CategoryId Int
AS
   SELECT NameCat
   FROM CategoryGuest
   WHERE Id_Cat = @CategoryId
GO

-- BillReport's procedures
CREATE PROCEDURE procAdditionalCharge;2
   @BillNo Int
AS
   SELECT BC.Name, 'Amount'=SUM(AC.Amount)
   FROM
      AdditionalCharge AC INNER JOIN
      BillCategory BC ON AC.Id_Bill = BC.Id_Bill
   WHERE AC.Bill_No = @BillNo
   GROUP BY BC.Name
GO
CREATE PROCEDURE procRoom;1
   @BillNo Int
AS
   SELECT R.Name, GBD.No_Of_Guests, GBD.Date_In, GBD.Date_Out, 
          'Rate' = GBD._Rate, 'Nights' = GBD._Nights, 'Charge' = GBD._Charge
   FROM 
      GuestBillData GBD INNER JOIN
      Room R ON R.Id_Room = GBD.Id_Room
   WHERE GBD.Bill_No = @BillNo
   ORDER BY R.Name
GO
CREATE PROCEDURE procGuestBill;1
   @BillNo Int
AS
   SELECT 
      CategoryName = CG.NameCat,
      FullName = G.FName+' '+G.LName, G.City, G.State, G.Address, G.Post,
      GB.Date_,  GB.Folio, HeadrdOfGI = GB.Heard_Of_GI, GB.Details, 
      TotalAdditional = GB._Total_Additional, 
      Subtotal = GB._Subtotal, TaxAndService = GB._Tax_And_Service, 
      Total = GB._Total, BalanceDue = GB._Balance_Due,
      TotalRoomSpay = GB._Total_RoomSpay,
      DepositReceived = GB._Deposit_Recieved,
      Payments = GB._Total_Payments
   FROM 
      Guest G INNER JOIN 
      GuestBill GB ON G.Id_Guest = GB.Id_Guest INNER JOIN
      CategoryGuest CG ON CG.Id_Cat = G.Id_Cat
   WHERE GB.Bill_No = @BillNo
GO

--Cascading integrity start ######################################################

create procedure GuestBill_delcascade_time
      @d1 datetime,
      @d2 datetime
AS
 begin        
   --SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
   begin transaction

   create table #DelIDS (BILL_NO integer primary key)

   insert into #DelIDS 
     select distinct BILL_NO 
       from GuestBill g where g.DATE_ >= @d1 and g.DATE_ <= @d2

   delete from GuestBillData 
     where BILL_NO IN (select * from #DelIDS)
   delete from AdditionalCharge 
     where BILL_NO IN (select * from #DelIDS)
   delete from Payments 
     where BILL_NO IN (select * from #DelIDS)
   delete from GuestBill 
     where BILL_NO IN (select * from #DelIDS)

   drop table #DelIDS

   commit transaction
   --SET TRANSACTION ISOLATION LEVEL READ COMMITED
 end
go

create procedure GuestBill_delcascade
	@bill_no integer        
AS
 begin        
   --SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
   begin transaction

   delete GuestBillData
     where ID_DB in (select ID_DB from GuestBillData g where g.BILL_NO = @bill_no)

   delete AdditionalCharge where BILL_NO = @bill_no

   delete Payments where BILL_NO = @bill_no

   delete GuestBill  where BILL_NO = @bill_no

   commit transaction
   --SET TRANSACTION ISOLATION LEVEL READ COMMITED
 end
go

create procedure Guest_delcascade
	@id_guest integer        
AS
 begin        
   --SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
   begin transaction

   delete GuestBillData
     where ID_DB in (select ID_DB from GuestBillData g where 
       (g.BILL_NO in (select BILL_NO from GuestBill gb where gb.ID_GUEST = @id_guest)))

   delete AdditionalCharge
     where BILL_NO in (select BILL_NO from GuestBill g where g.ID_GUEST = @id_guest)

   delete Payments 
     where BILL_NO in (select BILL_NO from GuestBill g where g.ID_GUEST = @id_guest)

   /*****************************************************************************
   AV-End
   ****************************************************************************/
   --delete Invitee where ID_GUEST = @id_guest   
   /*****************************************************************************
   AV-End
   *****************************************************************************/

   delete GuestBill where ID_GUEST = @id_guest   
   delete Guest where ID_GUEST = @id_guest   

   commit transaction 
   --SET TRANSACTION ISOLATION LEVEL READ COMMITED
 end
go

create procedure Operator_delcascade
	@user_ varchar(50)        
AS
 begin        
   begin transaction

   delete Operation where user_ = @user_
   delete Operator  where user_ = @user_

   commit transaction
 end
go

create procedure OperationDescription_delcascade
	@op_code integer        
AS
 begin        
   begin transaction

   delete Operation where op_code = @op_code
   delete OperationDescription  where op_code = @op_code

   commit transaction
 end
go

create procedure WorkAlloc_delcascade
	@id_plan integer        
AS
 begin        
   begin transaction

   delete WorkTimeLog where ID_PLAN = @id_plan
   delete WorkAlloc  where ID_PLAN = @id_plan

   commit transaction
 end
go


create procedure Job_delcascade
	@id_job integer        
AS
 begin        
   begin transaction

   delete WorkAlloc where ID_JOB = @id_job
   update Emplyee set ID_JOB = NULL where ID_JOB = @id_job
   delete Job  where ID_JOB = @id_job

   commit transaction
 end
go

create procedure Emplyee_delcascade
	@id_emp integer        
AS
 begin        
   begin transaction

   declare @bill_no integer
   declare csrBill cursor for select distinct BILL_NO from GuestBill where ID_EMP = @id_emp
   open csrBill

   fetch next from  csrBill into @bill_no
   while @@FETCH_STATUS = 0
    begin
      exec GuestBill_delcascade  @bill_no
      fetch next from  csrBill into @bill_no
    end

   close csrBill
   deallocate csrBill 

   update WorkAlloc set ID_EMP = NULL where ID_EMP = @id_emp    
   update Emplyee set ID_EMP_BOSS = NULL where ID_EMP_BOSS = @id_emp
   delete Emplyee  where ID_EMP = @id_emp

   commit transaction
 end
go

--Cascading integrity end ######################################################

/*****************************************************************************
   AV-Start (log triggers)
*****************************************************************************/
CREATE TRIGGER upd_Guest_trigger ON Guest 
FOR UPDATE
AS
BEGIN
  if UPDATE(DEPOSIT) 
  BEGIN
      insert into Operation (op_code, old_val, new_val, add_info)
         select 101, CONVERT(VARCHAR, d.Deposit), CONVERT( VARCHAR,i.Deposit ),
            ( 'id:' + CONVERT(VARCHAR,i.id_guest) + ' Name:' +  i.fname  + ' ' +  i.lname)      
             from inserted i, deleted d  where d .ID_GUEST=i.ID_GUEST
  END
  if UPDATE(ID_CAT) 
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 102, CONVERT(VARCHAR, d.ID_CAT), CONVERT( VARCHAR,i.ID_CAT ),
             ( 'id:'+CONVERT(VARCHAR,i.id_guest)+' Name:'+ i.fname +' '+ i.lname)      
             from inserted i, deleted d  where d .ID_GUEST=i.ID_GUEST
  END
  if UPDATE(FNAME) OR  UPDATE(LNAME)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 103, d.fname + ' '+ d.lname,  i.fname + ' ' + i.lname,
             ( 'id:'+CONVERT(VARCHAR,i.id_guest))      
         from inserted i, deleted d  where d .ID_GUEST=i.ID_GUEST
  END  
  if UPDATE(CITY) OR  UPDATE(STATE) OR  UPDATE(ADDRESS) OR  UPDATE(POST)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 104,
        d.city+' '+d.state+' '+d.address+' '+d.state+ ' '+CONVERT(VARCHAR,d.post),
        i.city+' '+i.state+' '+i.address+' '+i.state+ ' '+CONVERT(VARCHAR,i.post),
        ( 'id:'+CONVERT(VARCHAR,i.id_guest)+' Name:'+ i.fname +' '+ i.lname)     
             from inserted i, deleted d  where d .ID_GUEST=i.ID_GUEST
  END  
END
go
CREATE TRIGGER insdel_Guest_trigger ON Guest 
FOR INSERT, DELETE 
AS
BEGIN
    insert into Operation (op_code,old_val, new_val, add_info)
         select 110, NULL, NULL,
             ( 'id:' + CONVERT(VARCHAR,i.id_guest) +  ' Name:' + i.fname +'  '+ i.lname  + ' Deposit:' +CONVERT(VARCHAR, i.deposit))      
             from inserted i
    insert into Operation (op_code,old_val, new_val, add_info)
         select 120, NULL, NULL,
             ( 'id:' + CONVERT(VARCHAR,d.id_guest) +  ' Name:' + d.fname +' '+ d.lname  + ' Deposit:' + CONVERT(VARCHAR, d.deposit))      
             from deleted d
END  
go

CREATE TRIGGER insdel_AdditionalCharge_trigger ON AdditionalCharge
FOR  INSERT, DELETE AS
BEGIN
    insert into Operation (op_code,old_val, new_val, add_info)
         select 510, NULL, NULL,
          ( 'id_ac:' + CONVERT(VARCHAR,i.id_ac) + 
            ' bill_no:' + CONVERT(VARCHAR,i.bill_no) + 
            ' id_bill:' + CONVERT(VARCHAR,i.id_bill) +      
            ' amount:' + CONVERT(VARCHAR,i.amount) +      
            ' mkdate:' + CONVERT(VARCHAR,i.mkdate)  )      
           from inserted i

    insert into Operation (op_code,old_val, new_val, add_info)
         select 520, NULL, NULL,
          ( 'id_ac:' + CONVERT(VARCHAR,d.id_ac) + 
            ' bill_no:' + CONVERT(VARCHAR,d.bill_no) + 
            ' id_bill:' + CONVERT(VARCHAR,d.id_bill) +      
            ' amount:' + CONVERT(VARCHAR,d.amount) +      
            ' mkdate:' + CONVERT(VARCHAR,d.mkdate)  )      
         from deleted d
END  
go
CREATE TRIGGER upd_AdditionalCharge_trigger ON AdditionalCharge
FOR UPDATE
AS
BEGIN
  if UPDATE(AMOUNT)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 501, CONVERT(VARCHAR,d.amount),  CONVERT(VARCHAR,i.amount),
             ( 'id_ac:'+CONVERT(VARCHAR,i.id_ac) +
               ' bill_no:'+CONVERT(VARCHAR,i.bill_no) + 
               ' id_bill:'+CONVERT(VARCHAR,i.id_bill) )      
         from inserted i, deleted d  where d .ID_AC=i.ID_AC
  END  
END
go
CREATE TRIGGER insdel_GuestBillData_trigger ON GuestBillData
FOR  INSERT, DELETE AS
BEGIN
    insert into Operation (op_code,old_val, new_val, add_info)
         select 410, NULL, NULL,
          ( 'id_db:' + CONVERT(VARCHAR,i.id_db) + 
            ' bill_no:' + CONVERT(VARCHAR,i.bill_no) + 
            ' no_of_guests:' + CONVERT(VARCHAR,i.no_of_guests) +      
            ' date_in:' + CONVERT(VARCHAR,i.date_in) +      
            ' date_out:' + CONVERT(VARCHAR,i.date_out) +      
            ' id_room:' + CONVERT(VARCHAR,i.id_room) )      
           from inserted i

    insert into Operation (op_code,old_val, new_val, add_info)
         select 420, NULL, NULL,
          ( 'id_db:' + CONVERT(VARCHAR,d.id_db) + 
            ' bill_no:' + CONVERT(VARCHAR,d.bill_no) + 
            ' no_of_guests:' + CONVERT(VARCHAR,d.no_of_guests) +      
            ' date_in:' + CONVERT(VARCHAR,d.date_in) +      
            ' date_out:' + CONVERT(VARCHAR,d.date_out) +      
            ' id_room:' + CONVERT(VARCHAR,d.id_room) )      
         from deleted d
END  
go
CREATE TRIGGER upd_GuestBillData_trigger ON GuestBillData
FOR UPDATE
AS
BEGIN
  if UPDATE(NO_OF_GUESTS)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 401, CONVERT(VARCHAR,d.no_of_guests),  CONVERT(VARCHAR,i.no_of_guests),
             ( 'id_db:'+CONVERT(VARCHAR,i.id_db) +
               ' bill_no:'+CONVERT(VARCHAR,i.bill_no))      
         from inserted i, deleted d  where d .ID_DB=i.ID_DB
  END  
  if UPDATE(date_in)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 402, CONVERT(VARCHAR,d.date_in),  CONVERT(VARCHAR,i.date_in),
             ( 'id_db:'+CONVERT(VARCHAR,i.id_db) +
               ' bill_no:'+CONVERT(VARCHAR,i.bill_no))      
         from inserted i, deleted d  where d .ID_DB=i.ID_DB
  END  
  if UPDATE(date_out)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 403, CONVERT(VARCHAR,d.date_out),  CONVERT(VARCHAR,i.date_out),
             ( 'id_db:'+CONVERT(VARCHAR,i.id_db) +
               ' bill_no:'+CONVERT(VARCHAR,i.bill_no))      
         from inserted i, deleted d  where d .ID_DB=i.ID_DB
  END  
  if UPDATE(id_room)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 404, CONVERT(VARCHAR,d.id_room),  CONVERT(VARCHAR,i.id_room),
             ( 'id_db:'+CONVERT(VARCHAR,i.id_db) +
               ' bill_no:'+CONVERT(VARCHAR,i.bill_no))      
         from inserted i, deleted d  where d .ID_DB=i.ID_DB
  END  
END
go
CREATE TRIGGER insdel_Payments_trigger ON Payments
FOR  INSERT, DELETE 
AS
BEGIN
    insert into Operation (op_code,old_val, new_val, add_info)
         select 310, NULL, NULL,
          ( 'id_pay:' + CONVERT(VARCHAR,i.id_pay) + 
            ' bill_no:' + CONVERT(VARCHAR,i.bill_no) + 
            ' amount:' + CONVERT(VARCHAR,i.amount) +      
            ' desc:' + i.description)      
             from inserted i

    insert into Operation (op_code,old_val, new_val, add_info)
         select 320, NULL, NULL,
          ( 'id_pay:' + CONVERT(VARCHAR,d.id_pay) + 
            ' bill_no:' + CONVERT(VARCHAR,d.bill_no) + 
            ' amount:' + CONVERT(VARCHAR,d.amount) +      
            ' desc:' +d.description)      
          from deleted d
END  
go

CREATE TRIGGER upd_Payments_trigger ON Payments
FOR UPDATE
AS
BEGIN
  if UPDATE(AMOUNT)
  BEGIN
      insert into Operation (op_code,old_val,new_val, add_info)
         select 301, CONVERT(VARCHAR,d.amount),  CONVERT(VARCHAR,i.amount),
             ( 'id_pay:'+CONVERT(VARCHAR,i.id_pay) +
               ' bill_no:'+CONVERT(VARCHAR,i.bill_no))      
         from inserted i, deleted d  where d.ID_PAY=i.ID_PAY
  END  
END
go
/*****************************************************************************
   AV-End
*****************************************************************************/
