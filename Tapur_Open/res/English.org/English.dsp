# Microsoft Developer Studio Project File - Name="English" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=English - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "English.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "English.mak" CFG="English - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "English - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "English - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "English - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"D:\Tapur\Release\res\en.dll" /NOENTRY
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "English - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"D:\Tapur\Debug\lang.dll" /pdbtype:sept /NOENTRY
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "English - Win32 Release"
# Name "English - Win32 Debug"
# Begin Source File

SOURCE=..\discon32.ico
# End Source File
# Begin Source File

SOURCE=.\English.rc
# End Source File
# Begin Source File

SOURCE=..\F_MSG_CLOSE.ico
# End Source File
# Begin Source File

SOURCE=..\F_MSG_DRAG.ico
# End Source File
# Begin Source File

SOURCE=..\F_MSG_OPEN.ico
# End Source File
# Begin Source File

SOURCE=..\F_MSG_TRASH.ico
# End Source File
# Begin Source File

SOURCE=..\F_RESP_CLOSE.ico
# End Source File
# Begin Source File

SOURCE=..\F_RESP_DRAG.ico
# End Source File
# Begin Source File

SOURCE=..\F_RESP_OPEN.ico
# End Source File
# Begin Source File

SOURCE=..\F_RESP_TRASH.ico
# End Source File
# Begin Source File

SOURCE=..\F_ROOT.ico
# End Source File
# Begin Source File

SOURCE=..\harrow.cur
# End Source File
# Begin Source File

SOURCE=..\icon1.ico
# End Source File
# Begin Source File

SOURCE=..\icon2.ico
# End Source File
# Begin Source File

SOURCE=..\link.cur
# End Source File
# Begin Source File

SOURCE=..\MSG_RECORD.ico
# End Source File
# Begin Source File

SOURCE=..\MSG_SIMPLE.ico
# End Source File
# Begin Source File

SOURCE=..\new32.ico
# End Source File
# Begin Source File

SOURCE=..\nodrop.cur
# End Source File
# Begin Source File

SOURCE=..\PAUSED.BMP
# End Source File
# Begin Source File

SOURCE=..\PAUSEF.BMP
# End Source File
# Begin Source File

SOURCE=..\PAUSEU.BMP
# End Source File
# Begin Source File

SOURCE=..\PAUSEX.BMP
# End Source File
# Begin Source File

SOURCE=..\PLAY_APP.ico
# End Source File
# Begin Source File

SOURCE=..\PLAYD.BMP
# End Source File
# Begin Source File

SOURCE=..\PLAYF.BMP
# End Source File
# Begin Source File

SOURCE=..\PLAYU.BMP
# End Source File
# Begin Source File

SOURCE=..\PLAYX.BMP
# End Source File
# Begin Source File

SOURCE=..\rcv1.ico
# End Source File
# Begin Source File

SOURCE=..\rcv2.ico
# End Source File
# Begin Source File

SOURCE=..\recmod_t.bmp
# End Source File
# Begin Source File

SOURCE=..\RESP_SIMPLE.ico
# End Source File
# Begin Source File

SOURCE=..\STOPD.BMP
# End Source File
# Begin Source File

SOURCE=..\STOPF.BMP
# End Source File
# Begin Source File

SOURCE=..\STOPU.BMP
# End Source File
# Begin Source File

SOURCE=..\STOPX.BMP
# End Source File
# Begin Source File

SOURCE=..\T_RECORD.ico
# End Source File
# Begin Source File

SOURCE=..\T_RECORD_BOTH.ico
# End Source File
# Begin Source File

SOURCE=..\T_RECORD_PAUSE.ico
# End Source File
# Begin Source File

SOURCE=..\T_RECORD_RCV.ico
# End Source File
# Begin Source File

SOURCE=..\T_RECORD_SND.ico
# End Source File
# Begin Source File

SOURCE=..\T_RECORD_STOP.ico
# End Source File
# Begin Source File

SOURCE=..\T_VRECORD.ico
# End Source File
# Begin Source File

SOURCE=..\T_VRECORD_PAUSE.ico
# End Source File
# Begin Source File

SOURCE=..\T_VRECORD_STOP.ico
# End Source File
# Begin Source File

SOURCE=..\Tapur.ico
# End Source File
# Begin Source File

SOURCE=..\..\Tapur.rgs
# End Source File
# Begin Source File

SOURCE=..\tapur32.ico
# End Source File
# Begin Source File

SOURCE=..\tapur48.bmp
# End Source File
# Begin Source File

SOURCE=..\TapurDoc.ico
# End Source File
# Begin Source File

SOURCE=..\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\VMSG_RECORD.ico
# End Source File
# Begin Source File

SOURCE=..\VMSG_SIMPLE.ico
# End Source File
# Begin Source File

SOURCE=..\VPLAY_APP.ico
# End Source File
# End Target
# End Project
