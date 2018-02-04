# Microsoft Developer Studio Project File - Name="guana" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=guana - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "guana.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "guana.mak" CFG="guana - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "guana - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "guana - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "guana - Win32 Release"
# Name "guana - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "DATA_COMBO"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\cnct_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\com_chk.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\Seagate Software\Crystal Reports\crdates.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\CRHelpers.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_comm.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_many_to_many.cpp
# End Source File
# Begin Source File

SOURCE=.\fld_binder.cpp
# End Source File
# Begin Source File

SOURCE=.\guana.cpp
# End Source File
# Begin Source File

SOURCE=.\guana.rc
# End Source File
# Begin Source File

SOURCE=.\guanaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\login.cpp
# End Source File
# Begin Source File

SOURCE=.\lookup.cpp
# End Source File
# Begin Source File

SOURCE=.\my_dbnav.cpp
# End Source File
# Begin Source File

SOURCE=.\my_edit.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\Seagate Software\Crystal Reports\peplus.cpp"
# End Source File
# Begin Source File

SOURCE=.\prop_bill.cpp
# End Source File
# Begin Source File

SOURCE=.\prop_tech.cpp
# End Source File
# Begin Source File

SOURCE=.\prop_time.cpp
# End Source File
# Begin Source File

SOURCE=.\RecreateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TBillAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\TConnectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TDeposit_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TGuestAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\TSearchGuest.cpp
# End Source File
# Begin Source File

SOURCE=.\TSpashDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cnct_dlg.h
# End Source File
# Begin Source File

SOURCE=.\com_chk.h
# End Source File
# Begin Source File

SOURCE="..\..\..\Seagate Software\Crystal Reports\crdates.h"
# End Source File
# Begin Source File

SOURCE=.\CRHelpers.h
# End Source File
# Begin Source File

SOURCE=.\dlg_comm.h
# End Source File
# Begin Source File

SOURCE=.\dlg_many_to_many.h
# End Source File
# Begin Source File

SOURCE=.\fld_binder.h
# End Source File
# Begin Source File

SOURCE=.\guana.h
# End Source File
# Begin Source File

SOURCE=.\guanaDlg.h
# End Source File
# Begin Source File

SOURCE=.\login.h
# End Source File
# Begin Source File

SOURCE=.\lookup.h
# End Source File
# Begin Source File

SOURCE=.\my_dbnav.h
# End Source File
# Begin Source File

SOURCE=.\my_edit.h
# End Source File
# Begin Source File

SOURCE="..\..\..\Seagate Software\Crystal Reports\peplus.h"
# End Source File
# Begin Source File

SOURCE=.\prop_bill.h
# End Source File
# Begin Source File

SOURCE=.\prop_tech.h
# End Source File
# Begin Source File

SOURCE=.\prop_time.h
# End Source File
# Begin Source File

SOURCE=.\RecreateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReportSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TBillAdd.h
# End Source File
# Begin Source File

SOURCE=.\TConnectDlg.h
# End Source File
# Begin Source File

SOURCE=.\TDeposit_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\TGuestAdd.h
# End Source File
# Begin Source File

SOURCE=.\TSearchGuest.h
# End Source File
# Begin Source File

SOURCE=.\TSpashDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\b_add.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_begin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_cancel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_del.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_edit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_end.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_next.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_ok.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_priv.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_refresh.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_w.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GENW.ico
# End Source File
# Begin Source File

SOURCE=.\res\guana.ico
# End Source File
# Begin Source File

SOURCE=.\res\guana.rc2
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXEditListImg.bmp
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXGridListImg.bmp
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXHandCursor.cur
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXHistoryComboImg.bmp
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXNetBrowseTreeImg.bmp
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXNoHandCursor.cur
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXRUpBar.bmp
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXSpinHor.cur
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXSpinVer.cur
# End Source File
# Begin Source File

SOURCE=.\OX\RC\OXWebBar.bmp
# End Source File
# End Group
# Begin Group "OX"

# PROP Default_Filter "h;hpp;hxx;hm;inl;*.cpp"
# Begin Source File

SOURCE=.\OX\OXBitmapButton.cpp
# End Source File
# Begin Source File

SOURCE=.\OX\OXBitmapButton.h
# End Source File
# Begin Source File

SOURCE=.\OX\OXBitmapButtonRes.h
# End Source File
# Begin Source File

SOURCE=.\OX\Oxdib.cpp
# End Source File
# Begin Source File

SOURCE=.\OX\Oxdib.h
# End Source File
# Begin Source File

SOURCE=.\OX\OXStaticText.cpp
# End Source File
# Begin Source File

SOURCE=.\OX\OXStaticText.h
# End Source File
# End Group
# Begin Group "ADO_DSRC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ADO_DSRC\_recordset.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\_recordset.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\adodc.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\adodc.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\field.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\field.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\fields.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\fields.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\font.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\font.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\properties.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\properties.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\property.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_DSRC\property.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_DSRC"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "ADO_GRID"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ADO_GRID\column.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\column.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\columns.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\columns.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\dataformatdisp.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\dataformatdisp.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\datagrid.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\datagrid.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\font1.cpp
# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\font1.h
# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\picture.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\picture.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\selbookmarks.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\selbookmarks.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\split.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\split.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\splits.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\splits.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\stddataformatsdisp.cpp

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADO_GRID\stddataformatsdisp.h

!IF  "$(CFG)" == "guana - Win32 Release"

# PROP Intermediate_Dir "ADO_GRID\EXE"

!ELSEIF  "$(CFG)" == "guana - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "DATA_LIST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DATA_COMBO\datacombo.cpp
# End Source File
# Begin Source File

SOURCE=.\DATA_COMBO\datacombo.h
# End Source File
# Begin Source File

SOURCE=.\DATA_LIST\datalist.cpp
# End Source File
# Begin Source File

SOURCE=.\DATA_LIST\datalist.h
# End Source File
# Begin Source File

SOURCE=.\DATA_COMBO\font2.cpp
# End Source File
# Begin Source File

SOURCE=.\DATA_COMBO\font2.h
# End Source File
# Begin Source File

SOURCE=.\DATA_LIST\font5.cpp
# End Source File
# Begin Source File

SOURCE=.\DATA_LIST\font5.h
# End Source File
# Begin Source File

SOURCE=.\DATA_COMBO\picture2.cpp
# End Source File
# Begin Source File

SOURCE=.\DATA_COMBO\picture2.h
# End Source File
# Begin Source File

SOURCE=.\DATA_LIST\picture5.cpp
# End Source File
# Begin Source File

SOURCE=.\DATA_LIST\picture5.h
# End Source File
# End Group
# Begin Group "MASK_EDIT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MASK_EDIT\font8.cpp
# End Source File
# Begin Source File

SOURCE=.\MASK_EDIT\font8.h
# End Source File
# Begin Source File

SOURCE=.\MASK_EDIT\msmask.cpp
# End Source File
# Begin Source File

SOURCE=.\MASK_EDIT\msmask.h
# End Source File
# Begin Source File

SOURCE=.\MASK_EDIT\picture8.cpp
# End Source File
# Begin Source File

SOURCE=.\MASK_EDIT\picture8.h
# End Source File
# End Group
# Begin Group "PICKER"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PICKER\dtpicker.cpp
# End Source File
# Begin Source File

SOURCE=.\PICKER\dtpicker.h
# End Source File
# Begin Source File

SOURCE=.\PICKER\font10.cpp
# End Source File
# Begin Source File

SOURCE=.\PICKER\font10.h
# End Source File
# Begin Source File

SOURCE=.\PICKER\picture10.cpp
# End Source File
# Begin Source File

SOURCE=.\PICKER\picture10.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE="..\..\..\Seagate Software\Crystal Reports\crpe32m.lib"
# End Source File
# Begin Source File

SOURCE=E:\MVS\VC98\Lib\ODBCCP32.LIB
# End Source File
# End Target
# End Project
# Section guana : {F0D2F21A-CCB0-11D0-A316-00AA00688B10}
# 	2:5:Class:CDataCombo
# 	2:10:HeaderFile:data_combo\datacombo.h
# 	2:8:ImplFile:data_combo\datacombo.cpp
# End Section
# Section guana : {20DD1B9E-87C4-11D1-8BE3-0000F8754DA1}
# 	2:21:DefaultSinkHeaderFile:picker\dtpicker.h
# 	2:16:DefaultSinkClass:CDTPicker
# End Section
# Section guana : {F0D2F217-CCB0-11D0-A316-00AA00688B10}
# 	2:5:Class:CDataList
# 	2:10:HeaderFile:data_list\datalist.h
# 	2:8:ImplFile:data_list\datalist.cpp
# End Section
# Section guana : {CDE57A50-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CColumns
# 	2:10:HeaderFile:columns.h
# 	2:8:ImplFile:columns.cpp
# End Section
# Section guana : {C932BA85-4374-101B-A56C-00AA003668DC}
# 	2:21:DefaultSinkHeaderFile:mask_edit\msmask.h
# 	2:16:DefaultSinkClass:CMSMask
# End Section
# Section guana : {CDE57A54-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSplit
# 	2:10:HeaderFile:split.h
# 	2:8:ImplFile:split.cpp
# End Section
# Section guana : {E675F3F0-91B5-11D0-9484-00A0C91110ED}
# 	2:5:Class:CDataFormatDisp
# 	2:10:HeaderFile:dataformatdisp.h
# 	2:8:ImplFile:dataformatdisp.cpp
# End Section
# Section guana : {0000054D-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CFields
# 	2:10:HeaderFile:fields.h
# 	2:8:ImplFile:fields.cpp
# End Section
# Section guana : {FBB87595-FFF5-11D1-991D-006097C99385}
# 	2:5:Class:C_RecordsetNavbar
# 	2:10:HeaderFile:rsnav\_recordsetnavbar.h
# 	2:8:ImplFile:rsnav\_recordsetnavbar.cpp
# End Section
# Section guana : {99FF4676-FFC3-11D0-BD02-00C04FC2FB86}
# 	2:5:Class:CStdDataFormatsDisp
# 	2:10:HeaderFile:stddataformatsdisp.h
# 	2:8:ImplFile:stddataformatsdisp.cpp
# End Section
# Section guana : {CDE57A4F-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CColumn
# 	2:10:HeaderFile:column.h
# 	2:8:ImplFile:column.cpp
# End Section
# Section guana : {0000054C-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CField
# 	2:10:HeaderFile:field.h
# 	2:8:ImplFile:field.cpp
# End Section
# Section guana : {CDE57A53-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSplits
# 	2:10:HeaderFile:splits.h
# 	2:8:ImplFile:splits.cpp
# End Section
# Section guana : {CDE57A43-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:21:DefaultSinkHeaderFile:ado_grid\datagrid.h
# 	2:16:DefaultSinkClass:CDataGrid
# End Section
# Section guana : {47A738F1-7FAF-11D0-B148-00A0C922E820}
# 	2:5:Class:CAdodc
# 	2:10:HeaderFile:adodc.h
# 	2:8:ImplFile:adodc.cpp
# End Section
# Section guana : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section guana : {00000504-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CProperties
# 	2:10:HeaderFile:properties.h
# 	2:8:ImplFile:properties.cpp
# End Section
# Section guana : {F0D2F21C-CCB0-11D0-A316-00AA00688B10}
# 	2:21:DefaultSinkHeaderFile:data_combo\datacombo.h
# 	2:16:DefaultSinkClass:CDataCombo
# End Section
# Section guana : {4D6CC9A0-DF77-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSMask
# 	2:10:HeaderFile:mask_edit\msmask.h
# 	2:8:ImplFile:mask_edit\msmask.cpp
# End Section
# Section guana : {CDE57A52-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSelBookmarks
# 	2:10:HeaderFile:selbookmarks.h
# 	2:8:ImplFile:selbookmarks.cpp
# End Section
# Section guana : {F0D2F219-CCB0-11D0-A316-00AA00688B10}
# 	2:21:DefaultSinkHeaderFile:data_list\datalist.h
# 	2:16:DefaultSinkClass:CDataList
# End Section
# Section guana : {20DD1B9B-87C4-11D1-8BE3-0000F8754DA1}
# 	2:5:Class:CDTPicker
# 	2:10:HeaderFile:picker\dtpicker.h
# 	2:8:ImplFile:picker\dtpicker.cpp
# End Section
# Section guana : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section guana : {67397AA3-7FB1-11D0-B148-00A0C922E820}
# 	2:21:DefaultSinkHeaderFile:ado_dsrc\adodc.h
# 	2:16:DefaultSinkClass:CAdodc
# End Section
# Section guana : {00000503-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CProperty
# 	2:10:HeaderFile:property.h
# 	2:8:ImplFile:property.cpp
# End Section
# Section guana : {0000054F-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:C_Recordset
# 	2:10:HeaderFile:_recordset.h
# 	2:8:ImplFile:_recordset.cpp
# End Section
# Section guana : {58F3D268-FEDF-11D0-9C7F-0060081840F3}
# 	2:21:DefaultSinkHeaderFile:rsnav\_recordsetnavbar.h
# 	2:16:DefaultSinkClass:C_RecordsetNavbar
# End Section
# Section guana : {CDE57A41-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CDataGrid
# 	2:10:HeaderFile:datagrid.h
# 	2:8:ImplFile:datagrid.cpp
# End Section
