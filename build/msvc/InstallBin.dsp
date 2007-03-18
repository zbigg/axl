# Microsoft Developer Studio Project File - Name="InstallBin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=InstallBin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "InstallBin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "InstallBin.mak" CFG="InstallBin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "InstallBin - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "InstallBin - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "InstallBin"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "InstallBin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "out_release"
# PROP BASE Intermediate_Dir "out_release"
# PROP BASE Cmd_Line "NMAKE /f InstallBin.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "InstallBin.exe"
# PROP BASE Bsc_Name "InstallBin.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "out_release"
# PROP Intermediate_Dir "out_release"
# PROP Cmd_Line "nmake /nologo /f "InstallBin.mak" ZCOMPATNAME=zcompat  XLNAME=xl BDIR="out_release" Install"
# PROP Rebuild_Opt "/a"
# PROP Target_File "InstallBin.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "InstallBin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "out_debug"
# PROP BASE Intermediate_Dir "out_debug"
# PROP BASE Cmd_Line "NMAKE /f InstallBin.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "InstallBin.exe"
# PROP BASE Bsc_Name "InstallBin.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out_debug"
# PROP Intermediate_Dir "out_debug"
# PROP Cmd_Line "nmake /nologo /f "InstallBin.mak" ZCOMPATNAME=zcompatd XLNAME=xld BDIR="out_debug" Install"
# PROP Rebuild_Opt "/a"
# PROP Target_File "InstallBin.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "InstallBin - Win32 Release"
# Name "InstallBin - Win32 Debug"

!IF  "$(CFG)" == "InstallBin - Win32 Release"

!ELSEIF  "$(CFG)" == "InstallBin - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\Install.txt

!IF  "$(CFG)" == "InstallBin - Win32 Release"

# PROP Intermediate_Dir "out_release"

!ELSEIF  "$(CFG)" == "InstallBin - Win32 Debug"

# PROP Intermediate_Dir "out_debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InstallBin.mak

!IF  "$(CFG)" == "InstallBin - Win32 Release"

# PROP Intermediate_Dir "out_release"

!ELSEIF  "$(CFG)" == "InstallBin - Win32 Debug"

# PROP Intermediate_Dir "out_debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
