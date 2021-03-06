; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=TGuestAdd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "guana.h"

ClassCount=11
Class1=CGuanaApp
Class2=CGuanaDlg

ResourceCount=34
Resource2=IDD_ABOUTBOX
Resource3=IDD_GUANA_DIALOG (English (U.S.))
Resource4="IDD_DIALOG_BILL" (English (U.S.))
Resource5=IDD_DIALOG_RECREATE (English (U.S.))
Resource6=IDB_BITMAP_ADD (English (U.S.))
Resource7=IDD_DIALOG_TIME (English (U.S.))
Resource8=IDR_ACCELERATOR1 (Russian)
Resource1=IDR_MAINFRAME
Class3=TConnectDlg
Resource9=IDD_DIALOG_CONNECT (English (U.S.))
Resource10=IDD_DIALOG_COMMENTS (English (U.S.))
Resource11=IDD_DIALOG_LOGIN (English (U.S.))
Resource12=IDD_DIALOG_LOOKUP (English (U.S.))
Resource13=IDR_ACCELERATOR1
Class4=CReportSelectDlg
Resource14=IDD_DIALOG_LOGIN
Class5=CRecreateDlg
Resource15=IDD_BILLRPTSELECT (English (U.S.))
Resource16=IDD_DIALOG_MANY_TO_MANY
Resource17=IDD_ABOUTBOX (English (U.S.))
Resource18=IDD_DIALOG_LOOKUP
Resource19=IDD_DIALOG_TECH (English (U.S.))
Resource20=IDD_DIALOG_MANY_TO_MANY (English (U.S.))
Resource21=IDD_TSPASHDLG_DIALOG
Resource22=IDD_DIALOG_CONNECT
Resource23=IDD_DIALOG_TECH
Resource24=IDD_DLG_SEARCH_GUEST
Resource25=IDD_DLG_SEARCH_GUEST (English (U.S.))
Resource26=IDD_DIALOG_TIME
Resource27=IDD_TGUESTADD_DIALOG
Class6=TMyMCCtrl
Resource28=IDD_DIALOG_BILL (English (U.S.))
Class7=TSearchGuest
Resource29=IDD_DIALOG_COMMENTS
Resource30=IDD_DIALOG_BILL
Class8=TSpashDlg
Resource31=IDD_TDEPOSIT_DLG_DIALOG
Class9=TDeposit_Dlg
Resource32=IDD_DIALOG_RECREATE
Class10=TGuestAdd
Resource33=IDD_BILLRPTSELECT
Class11=TBillAdd
Resource34=IDD_TBILLADD_DIALOG (Russian)

[CLS:CGuanaApp]
Type=0
HeaderFile=guana.h
ImplementationFile=guana.cpp
Filter=N

[CLS:CGuanaDlg]
Type=0
HeaderFile=guanaDlg.h
ImplementationFile=guanaDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_GUANA_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_TAB1,SysTabControl32,1342193794
Control2=IDC_EXIT,button,1342242816
Control3=IDC_CHECK1,button,1342242819
Control4=IDC_RADIO1,button,1342177289

[DLG:IDD_DIALOG_TECH (English (U.S.))]
Type=1
Class=?
ControlCount=14
Control1=IDC_DATAGRID_ROOM,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control2=IDC_RADIO_NAM,button,1342308361
Control3=IDC_RADIO_RATE,button,1342177289
Control4=IDC_DATAGRID_GUESTCAT,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control5=IDC_DATAGRID_BILLCAT,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control6=IDC_DATAGRID_RECOMMBILLCAT,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control7=IDC_DC_ROOM,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control8=IDC_DC_BILLCAT,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control9=IDC_DC_GUESTCAT,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control10=IDC_DC_RECOMMBILLCAT,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control11=IDC_STATIC_ROOM,static,1342308353
Control12=IDC_STATIC_ALLGCAT,static,1342308353
Control13=IDC_STATIC_LCAT,static,1342308353
Control14=IDC_STATIC_SELGCAT,static,1342308353

[DLG:"IDD_DIALOG_BILL" (English (U.S.))]
Type=1
Class=?
ControlCount=0

[DLG:IDD_DIALOG_BILL (English (U.S.))]
Type=1
Class=?
ControlCount=60
Control1=IDC_DATAGRID_GUEST,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control2=IDC_RADIO_FNAM,button,1342308361
Control3=IDC_RADIO_CITY,button,1342177289
Control4=IDC_RADIO_CAT,button,1342177289
Control5=IDC_RADIO_DEPOSIT,button,1342177289
Control6=IDC_DATAGRID_BILL,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control7=IDC_RADIO_DATE,button,1342308361
Control8=IDC_RADIO_FOLIO,button,1342177289
Control9=IDC_RADIO_NIGHTS,button,1342177289
Control10=IDC_RADIO_BALANCEDUE,button,1342177289
Control11=IDC_EDIT_BILLNO,edit,1484849280
Control12=IDC_EDIT_FOLIO,edit,1484849280
Control13=IDC_PICK_DATE,SysDateTimePick32,1476460576
Control14=IDC_EDIT_GI,edit,1484849280
Control15=IDC_EDIT_DETAILS,edit,1484849280
Control16=IDC_DATAGRID_ADDCHARGES,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control17=IDC_DATAGRID_PAYMENTS,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control18=IDC_DATAGRID_ROOMSALLOC,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control19=IDC_EDIT_TOTALADD,edit,1350633600
Control20=IDC_EDIT_PAYMENTS,edit,1350633600
Control21=IDC_EDIT_TOTALROOMS,edit,1350633600
Control22=IDC_EDIT_SUBTOTAL,edit,1350633600
Control23=IDC_EDIT_TAXANDSERV,edit,1350633600
Control24=IDC_EDIT_TOTAL,edit,1350633600
Control25=IDC_EDIT_DEPOSIT,edit,1350633600
Control26=IDC_EDIT_BALANCEDUE,edit,1350633600
Control27=IDC_BUTTON_COMMENT,button,1476460544
Control28=IDC_BUTTON_PAYENROL,button,1476460544
Control29=IDC_BUTTON_MONEYBACK,button,1476460544
Control30=IDC_BUTTON_SEARCH,button,1342242816
Control31=IDC_STATIC_GUEST,static,1342308353
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC_P1,static,1342308352
Control34=IDC_STATIC,static,1342308353
Control35=IDC_STATIC_M1,static,1342308352
Control36=IDC_DC_GUEST,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control37=IDC_DC_BILL,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control38=IDC_DC_ADDCHARGES,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control39=IDC_DC_PAYMENTS,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control40=IDC_DC_ROOMSALLOC,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control41=IDC_STATIC,static,1342308353
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_STATIC_BILL,static,1342308353
Control45=IDC_STATIC_FRAME,button,1342177287
Control46=IDC_STATIC,static,1342308352
Control47=IDC_STATIC,static,1342308352
Control48=IDC_STATIC,static,1342308353
Control49=IDC_STATIC,static,1342308352
Control50=IDC_STATIC,static,1342308352
Control51=IDC_STATIC,static,1342308352
Control52=IDC_STATIC_EQ1,static,1342308352
Control53=IDC_STATIC_P2,static,1342308352
Control54=IDC_STATIC_EQ2,static,1342308352
Control55=IDC_STATIC,static,1342308353
Control56=IDC_STATIC,static,1342308352
Control57=IDC_DC_CATEGORY_G,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control58=IDC_DC_ROOM,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control59=IDC_DC_CATPAY,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control60=IDC_BUTTON_PRT,button,1476460544

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIALOG_TIME (English (U.S.))]
Type=1
Class=?
ControlCount=17
Control1=IDC_DATAGRID_GB,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control2=IDC_RADIO_FNAM,button,1342308361
Control3=IDC_RADIO_DATE,button,1342177289
Control4=IDC_RADIO_ALL,button,1342308361
Control5=IDC_RADIO_SEL,button,1342177289
Control6=IDC_DATALIST_CAT,{F0D2F21C-CCB0-11D0-A316-00AA00688B10},1342242816
Control7=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242848
Control8=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242848
Control9=IDC_BUTTON_REFILL,button,1342242816
Control10=IDC_BUTTON_JUMP,button,1342242816
Control11=IDC_BUTTON_REPORT,button,1476460544
Control12=IDC_STATIC_GB,static,1342308353
Control13=IDC_DC_GB,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control14=IDC_DC_CATEGORY_G,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_BUTTON_REMOVE,button,1342242816

[ACL:IDR_ACCELERATOR1]
Type=1
Class=?
Command1=ID_OK
CommandCount=1

[CLS:TConnectDlg]
Type=0
HeaderFile=TConnectDlg.h
ImplementationFile=TConnectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_BUTTON32772

[TB:IDB_BITMAP_ADD (English (U.S.))]
Type=1
Command1=ID_BUTTON32772
CommandCount=1

[DLG:IDD_DIALOG_LOOKUP (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_LIST,listbox,1344340225

[DLG:IDD_DIALOG_COMMENTS (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_EDIT_TXT,edit,1350635716
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG_CONNECT (English (U.S.))]
Type=1
Class=TConnectDlg
ControlCount=15
Control1=IDC_EDIT_SERVER,edit,1350631552
Control2=IDC_EDIT_DBNAME,edit,1350631552
Control3=IDC_RADIO_WINNT,button,1342373897
Control4=IDC_RADIO3,button,1342177289
Control5=IDC_EDIT_US,edit,1350762624
Control6=IDC_EDIT_PWD,edit,1350631552
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC_USER,static,1342308352
Control11=IDC_STATIC_PWD,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342177283
Control15=IDC_BTN_RECREATE,button,1342242816

[DLG:IDD_DIALOG_MANY_TO_MANY (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDC_DATAGRID_T1,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control2=IDC_BUTTON_REM,button,1342246656
Control3=IDC_BUTTON_ADD,button,1342246656
Control4=IDC_DATAGRID_T2,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control5=IDOK,button,1342246657
Control6=IDCANCEL,button,1342246656
Control7=IDC_DC_T1,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control8=IDC_DC_T2,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824

[DLG:IDD_BILLRPTSELECT (English (U.S.))]
Type=1
Class=CReportSelectDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_RADIO_ORDINAL,button,1342373897
Control3=IDC_RADIO_PRETTY,button,1342177289

[CLS:CReportSelectDlg]
Type=0
HeaderFile=ReportSelectDlg.h
ImplementationFile=ReportSelectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CReportSelectDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_RECREATE]
Type=1
Class=CRecreateDlg
ControlCount=8
Control1=IDC_EDIT_PATH,edit,1350631552
Control2=IDC_EDIT_DSIZE,edit,1350631552
Control3=IDC_EDIT_LSIZE,edit,1350631552
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDCANCEL,button,1342242816
Control8=IDOK,button,1342242817

[CLS:CRecreateDlg]
Type=0
HeaderFile=RecreateDlg.h
ImplementationFile=RecreateDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRecreateDlg

[ACL:IDR_ACCELERATOR1 (Russian)]
Type=1
Class=?
Command1=IDC_BUTTON_SEARCH
Command2=ID_OK
CommandCount=2

[DLG:IDD_DIALOG_CONNECT]
Type=1
Class=TConnectDlg
ControlCount=15
Control1=IDC_EDIT_SERVER,edit,1350631552
Control2=IDC_EDIT_DBNAME,edit,1350631552
Control3=IDC_RADIO_WINNT,button,1342373897
Control4=IDC_RADIO3,button,1342177289
Control5=IDC_EDIT_US,edit,1350762624
Control6=IDC_EDIT_PWD2,edit,1350631584
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC_USER,static,1342308352
Control11=IDC_STATIC_PWD,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342177283
Control15=IDC_BTN_RECREATE,button,1342242816

[DLG:IDD_DIALOG_MANY_TO_MANY]
Type=1
Class=?
ControlCount=8
Control1=IDC_DATAGRID_T1,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control2=IDC_BUTTON_REM,button,1342246656
Control3=IDC_BUTTON_ADD,button,1342246656
Control4=IDC_DATAGRID_T2,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control5=IDOK,button,1342246657
Control6=IDCANCEL,button,1342246656
Control7=IDC_DC_T1,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control8=IDC_DC_T2,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824

[DLG:IDD_DIALOG_TECH]
Type=1
Class=?
ControlCount=14
Control1=IDC_DATAGRID_ROOM,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control2=IDC_RADIO_NAM,button,1342308361
Control3=IDC_RADIO_RATE,button,1342177289
Control4=IDC_DATAGRID_GUESTCAT,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control5=IDC_DATAGRID_BILLCAT,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control6=IDC_DATAGRID_RECOMMBILLCAT,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control7=IDC_DC_ROOM,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control8=IDC_DC_BILLCAT,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control9=IDC_DC_GUESTCAT,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control10=IDC_DC_RECOMMBILLCAT,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control11=IDC_STATIC_ROOM,static,1342308353
Control12=IDC_STATIC_ALLGCAT,static,1342308353
Control13=IDC_STATIC_LCAT,static,1342308353
Control14=IDC_STATIC_SELGCAT,static,1342308353

[DLG:IDD_DIALOG_BILL]
Type=1
Class=?
ControlCount=61
Control1=IDC_DATAGRID_GUEST,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control2=IDC_RADIO_FNAM,button,1342308361
Control3=IDC_RADIO_CITY,button,1342177289
Control4=IDC_RADIO_CAT,button,1342177289
Control5=IDC_RADIO_DEPOSIT,button,1342177289
Control6=IDC_DATAGRID_BILL,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control7=IDC_RADIO_DATE,button,1342308361
Control8=IDC_RADIO_FOLIO,button,1342177289
Control9=IDC_RADIO_NIGHTS,button,1342177289
Control10=IDC_RADIO_BALANCEDUE,button,1342177289
Control11=IDC_EDIT_BILLNO,edit,1484849280
Control12=IDC_EDIT_FOLIO,edit,1484849280
Control13=IDC_PICK_DATE,SysDateTimePick32,1476460576
Control14=IDC_EDIT_GI,edit,1484849280
Control15=IDC_EDIT_DETAILS,edit,1484849280
Control16=IDC_DATAGRID_ADDCHARGES,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control17=IDC_DATAGRID_PAYMENTS,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control18=IDC_DATAGRID_ROOMSALLOC,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816
Control19=IDC_EDIT_TOTALADD,edit,1350633600
Control20=IDC_EDIT_PAYMENTS,edit,1350633600
Control21=IDC_EDIT_TOTALROOMS,edit,1350633600
Control22=IDC_EDIT_SUBTOTAL,edit,1350633600
Control23=IDC_EDIT_TAXANDSERV,edit,1350633600
Control24=IDC_EDIT_TOTAL,edit,1350633600
Control25=IDC_EDIT_DEPOSIT,edit,1350633600
Control26=IDC_EDIT_BALANCEDUE,edit,1350633600
Control27=IDC_BUTTON_COMMENT,button,1476460544
Control28=IDC_BUTTON_PAYENROL,button,1476460544
Control29=IDC_BUTTON_MONEYBACK,button,1476460544
Control30=IDC_BUTTON_SEARCH,button,1342242816
Control31=IDC_STATIC_GUEST,static,1342308353
Control32=IDC_STATIC_3,static,1342308352
Control33=IDC_STATIC_P1,static,1342308352
Control34=IDC_STATIC_2,static,1342308353
Control35=IDC_STATIC_M1,static,1342308352
Control36=IDC_DC_GUEST,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control37=IDC_DC_BILL,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control38=IDC_DC_ADDCHARGES,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control39=IDC_DC_PAYMENTS,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control40=IDC_DC_ROOMSALLOC,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control41=IDC_STATIC_24,static,1342308353
Control42=IDC_STATIC_23,static,1342308352
Control43=IDC_STATIC_22,static,1342308352
Control44=IDC_STATIC_BILL,static,1342308353
Control45=IDC_STATIC_FRAME,button,1342177287
Control46=IDC_STATIC_20,static,1342308352
Control47=IDC_STATIC_21,static,1342308352
Control48=IDC_STATIC_1,static,1342308353
Control49=IDC_STATIC_9,static,1342308352
Control50=IDC_STATIC_4,static,1342308352
Control51=IDC_STATIC_5,static,1342308352
Control52=IDC_STATIC_EQ1,static,1342308352
Control53=IDC_STATIC_P2,static,1342308352
Control54=IDC_STATIC_EQ2,static,1342308352
Control55=IDC_STATIC_6,static,1342308353
Control56=IDC_STATIC_7,static,1342308352
Control57=IDC_DC_CATEGORY_G,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control58=IDC_DC_ROOM,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control59=IDC_DC_CATPAY,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control60=IDC_BUTTON_WIZ,button,1342242816
Control61=IDC_BUTTON_PRT2,button,1476460544

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIALOG_TIME]
Type=1
Class=?
ControlCount=17
Control1=IDC_DATAGRID_GB,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342373888
Control2=IDC_RADIO_FNAM,button,1342308361
Control3=IDC_RADIO_DATE,button,1342177289
Control4=IDC_RADIO_ALL,button,1342308361
Control5=IDC_RADIO_SEL,button,1342177289
Control6=IDC_DATALIST_CAT,{F0D2F21C-CCB0-11D0-A316-00AA00688B10},1342242816
Control7=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242848
Control8=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242848
Control9=IDC_BUTTON_REFILL,button,1342242816
Control10=IDC_BUTTON_JUMP,button,1342242816
Control11=IDC_BUTTON_REPORT,button,1476460544
Control12=IDC_STATIC_GB,static,1342308353
Control13=IDC_DC_GB,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control14=IDC_DC_CATEGORY_G,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_BUTTON_REMOVE,button,1342242816

[DLG:IDD_DIALOG_LOOKUP]
Type=1
Class=?
ControlCount=1
Control1=IDC_LIST,listbox,1344340225

[DLG:IDD_DIALOG_COMMENTS]
Type=1
Class=?
ControlCount=3
Control1=IDC_EDIT_TXT,edit,1350635716
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[DLG:IDD_BILLRPTSELECT]
Type=1
Class=CReportSelectDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_RADIO_ORDINAL,button,1342373897
Control3=IDC_RADIO_PRETTY,button,1342177289

[DLG:IDD_DIALOG_RECREATE (English (U.S.))]
Type=1
Class=CRecreateDlg
ControlCount=8
Control1=IDC_EDIT_PATH,edit,1350631552
Control2=IDC_EDIT_DSIZE,edit,1350631552
Control3=IDC_EDIT_LSIZE,edit,1350631552
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDCANCEL,button,1342242816
Control8=IDOK,button,1342242817

[DLG:IDD_DIALOG_LOGIN (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_EDIT_PASSW,edit,1350631584
Control2=IDOK,button,1342242817
Control3=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_LOGIN]
Type=1
Class=?
ControlCount=3
Control1=IDC_EDIT_PASSW,edit,1350631584
Control2=IDOK,button,1342242817
Control3=IDC_STATIC,static,1342308352

[CLS:TMyMCCtrl]
Type=0
HeaderFile=TMyMCCtrl.h
ImplementationFile=TMyMCCtrl.cpp
BaseClass=CMonthCalCtrl
Filter=W

[CLS:TSearchGuest]
Type=0
HeaderFile=TSearchGuest.h
ImplementationFile=TSearchGuest.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_BUTTON32772

[DLG:IDD_DLG_SEARCH_GUEST (English (U.S.))]
Type=1
Class=TSearchGuest
ControlCount=7
Control1=IDC_EDIT_FNAME,edit,1350631552
Control2=IDC_EDIT_LNAME,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[DLG:IDD_DLG_SEARCH_GUEST]
Type=1
Class=TSearchGuest
ControlCount=7
Control1=IDC_EDIT_FNAME,edit,1350631552
Control2=IDC_EDIT_LNAME,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[CLS:TSpashDlg]
Type=0
HeaderFile=TSpashDlg.h
ImplementationFile=TSpashDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:TDeposit_Dlg]
Type=0
HeaderFile=TDeposit_Dlg.h
ImplementationFile=TDeposit_Dlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_BUTTON32772

[DLG:IDD_TDEPOSIT_DLG_DIALOG]
Type=1
Class=TDeposit_Dlg
ControlCount=4
Control1=IDC_EDIT_DEPOS,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDC_BUTTON_SUB,button,1342242816
Control4=IDCANCEL,button,1342242816

[DLG:IDD_TSPASHDLG_DIALOG]
Type=1
Class=TSpashDlg
ControlCount=3
Control1=IDC_PROGRESS1,msctls_progress32,1350565888
Control2=IDC_STATIC_TTL,static,1342308353
Control3=IDC_STATIC,static,1342177283

[CLS:TGuestAdd]
Type=0
HeaderFile=TGuestAdd.h
ImplementationFile=TGuestAdd.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_STATE

[DLG:IDD_TGUESTADD_DIALOG]
Type=1
Class=TGuestAdd
ControlCount=21
Control1=IDC_DATACOMBO_CAT,{F0D2F21C-CCB0-11D0-A316-00AA00688B10},1342242816
Control2=IDC_EDIT_FNAME,edit,1350631552
Control3=IDC_EDIT_LNAME,edit,1350631552
Control4=IDC_EDIT_DEPOSIT,edit,1350631552
Control5=IDC_EDIT_ADDRESS,edit,1350631552
Control6=IDC_EDIT_CITY,edit,1350631552
Control7=IDC_COMBO_STATE,combobox,1344340290
Control8=IDC_EDIT_POST,edit,1350631552
Control9=IDCANCEL,button,1342242816
Control10=IDOK,button,1342242817
Control11=IDOK2,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352

[DLG:IDD_TBILLADD_DIALOG (Russian)]
Type=1
Class=TBillAdd
ControlCount=25
Control1=IDC_EDIT_BILLNO,edit,1484849280
Control2=IDC_DATETIMEPICKER_DATE,SysDateTimePick32,1342242848
Control3=IDC_EDIT_FOLIO,edit,1350631552
Control4=IDC_EDIT_GI,edit,1350631552
Control5=IDC_DATACOMBO_ROOM,{F0D2F21C-CCB0-11D0-A316-00AA00688B10},1342242816
Control6=IDC_EDIT_NOOFGUESTS,edit,1350631552
Control7=IDC_SPIN1,msctls_updown32,1342177334
Control8=IDC_DATETIMEPICKER_DATEFROM,SysDateTimePick32,1342242848
Control9=IDC_DATETIMEPICKER_DATETO,SysDateTimePick32,1342242848
Control10=IDC_EDIT_DETAILS,edit,1350631552
Control11=IDC_EDIT_COMMENTS,edit,1350635716
Control12=IDOK,button,1342242817
Control13=IDCANCEL,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308353
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352

[CLS:TBillAdd]
Type=0
HeaderFile=TBillAdd.h
ImplementationFile=TBillAdd.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_BUTTON32772
VirtualFilter=dWC

