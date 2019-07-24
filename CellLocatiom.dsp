# Microsoft Developer Studio Project File - Name="CellLocatiom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CellLocatiom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CellLocatiom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CellLocatiom.mak" CFG="CellLocatiom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CellLocatiom - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CellLocatiom - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CellLocatiom - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib Psapi.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "CellLocatiom - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib Psapi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CellLocatiom - Win32 Release"
# Name "CellLocatiom - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Ado.cpp
# End Source File
# Begin Source File

SOURCE=.\AdoCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\AdoRecordSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CellLocatiom.cpp
# End Source File
# Begin Source File

SOURCE=.\CellLocatiom.rc
# End Source File
# Begin Source File

SOURCE=.\CellLocatiomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomControlSite.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDown.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMark.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQry.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReal.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRst.cpp
# End Source File
# Begin Source File

SOURCE=.\excel9.cpp
# End Source File
# Begin Source File

SOURCE=.\ImpIDispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\log_out.cpp
# End Source File
# Begin Source File

SOURCE=.\MyExcel.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\TabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\WebPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Ado.h
# End Source File
# Begin Source File

SOURCE=.\AdoCommand.h
# End Source File
# Begin Source File

SOURCE=.\AdoRecordSet.h
# End Source File
# Begin Source File

SOURCE=.\CellLocatiom.h
# End Source File
# Begin Source File

SOURCE=.\CellLocatiomDlg.h
# End Source File
# Begin Source File

SOURCE=.\ComboListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CustomControlSite.h
# End Source File
# Begin Source File

SOURCE=.\DlgBs.h
# End Source File
# Begin Source File

SOURCE=.\DlgDown.h
# End Source File
# Begin Source File

SOURCE=.\DlgEdit.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoad.h
# End Source File
# Begin Source File

SOURCE=.\DlgMark.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreview.h
# End Source File
# Begin Source File

SOURCE=.\DlgQry.h
# End Source File
# Begin Source File

SOURCE=.\DlgReal.h
# End Source File
# Begin Source File

SOURCE=.\DlgRst.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\Draw.h
# End Source File
# Begin Source File

SOURCE=.\excel9.h
# End Source File
# Begin Source File

SOURCE=.\ImpIDispatch.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\log_out.h
# End Source File
# Begin Source File

SOURCE=.\msg_stru.h
# End Source File
# Begin Source File

SOURCE=.\MyExcel.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\TabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\Tools.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\WebPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CellLocatiom.ico
# End Source File
# Begin Source File

SOURCE=.\res\CellLocatiom.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Cursor.cur
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\Sys.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tabs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TabsDisabled.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section CellLocatiom : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section CellLocatiom : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
