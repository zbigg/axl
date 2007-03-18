# Microsoft Developer Studio Project File - Name="libxl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libxl - Win32 Release Static
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libxl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libxl.mak" CFG="libxl - Win32 Release Static"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libxl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libxl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libxl - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE "libxl - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libxl"
# PROP Scc_LocalPath "..\..\.."

!IF  "$(CFG)" == "libxl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\out_release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBXL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\..\..\..\common" /I ".\..\..\..\include" /I "..\\" /D "NDEBUG" /D "HAVE_CONFIG_H" /D "_WINDOWS" /D "_USRDLL" /D "LIBXL_EXPORTS" /D "WIN32" /D "_MBCS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 zcompat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"..\out_release/xl.dll" /libpath:"..\out_release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\out_debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBXL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\..\..\..\common" /I ".\..\..\..\include" /I "..\\" /D "_DEBUG" /D "HAVE_CONFIG_H" /D "_WINDOWS" /D "_USRDLL" /D "LIBXL_EXPORTS" /D "WIN32" /D "_MBCS" /FR /YX /FD /GZ /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zcompatd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /def:"..\..\..\src\libxl\libxl.def" /out:"..\out_debug/xld.dll" /pdbtype:sept /libpath:"..\out_debug" /lib
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libxl___Win32_Debug_Static"
# PROP BASE Intermediate_Dir "libxl___Win32_Debug_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../out_debug"
# PROP Intermediate_Dir "Debug_Static"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo
# ADD MTL /nologo
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\..\..\..\common" /I ".\..\..\..\include" /I "..\\" /D "_DEBUG" /D "HAVE_CONFIG_H" /D "_WINDOWS" /D "_USRDLL" /D "LIBXL_EXPORTS" /D "WIN32" /D "_MBCS" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\..\..\..\common" /I ".\..\..\..\include" /I "..\\" /D "_DEBUG" /D "HAVE_CONFIG_H" /D "_WINDOWS" /D "_USRDLL" /D "LIBXL_EXPORTS" /D "WIN32" /D "_MBCS" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\out_debug/sxld.lib"

!ELSEIF  "$(CFG)" == "libxl - Win32 Release Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libxl___Win32_Release_Static"
# PROP BASE Intermediate_Dir "libxl___Win32_Release_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../out_release"
# PROP Intermediate_Dir "Release_Static"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo
# ADD MTL /nologo
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\..\..\..\common" /I ".\..\..\..\include" /I "..\\" /D "_DEBUG" /D "HAVE_CONFIG_H" /D "_WINDOWS" /D "_USRDLL" /D "LIBXL_EXPORTS" /D "WIN32" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\..\..\..\common" /I ".\..\..\..\include" /I "..\\" /D "NDEBUG" /D "HAVE_CONFIG_H" /D "_WINDOWS" /D "_USRDLL" /D "LIBXL_EXPORTS" /D "WIN32" /D "_MBCS" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\out_debug/sxld.lib"
# ADD LIB32 /nologo /out:"..\out_release/sxl.lib"

!ENDIF 

# Begin Target

# Name "libxl - Win32 Release"
# Name "libxl - Win32 Debug"
# Name "libxl - Win32 Debug Static"
# Name "libxl - Win32 Release Static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "XL Sources"

# PROP Default_Filter ""
# Begin Group "XL Internal headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\libxl\arch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\axl_i.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\codegen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\grammar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\lex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\lexfunc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\proc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\tools.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\treegen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\vm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xltree.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\libxl\axl_i.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\builtin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\code.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\codegen.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\context.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\except.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\frame.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\grammar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\grammar.y

!IF  "$(CFG)" == "libxl - Win32 Release"

# Begin Custom Build
InputDir=\src\axl\src\libxl
InputPath=..\..\..\src\libxl\grammar.y

BuildCmds= \
	bison -y -d $(InputPath) \
	move y.tab.c $(InputDir)\grammar.c \
	move y.tab.h $(InputDir)\grammar.h \
	

"$(InputDir)\grammar.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\grammar.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug"

# Begin Custom Build
InputDir=\src\axl\src\libxl
InputPath=..\..\..\src\libxl\grammar.y

BuildCmds= \
	bison -y -d $(InputPath) \
	move y.tab.c $(InputDir)\grammar.c \
	move y.tab.h $(InputDir)\grammar.h \
	

"$(InputDir)\grammar.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\grammar.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug Static"

# Begin Custom Build
InputDir=\src\axl\src\libxl
InputPath=..\..\..\src\libxl\grammar.y

BuildCmds= \
	bison -y -d $(InputPath) \
	move y.tab.c $(InputDir)\grammar.c \
	move y.tab.h $(InputDir)\grammar.h \
	

"$(InputDir)\grammar.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\grammar.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "libxl - Win32 Release Static"

# Begin Custom Build
InputDir=\src\axl\src\libxl
InputPath=..\..\..\src\libxl\grammar.y

BuildCmds= \
	bison -y -d $(InputPath) \
	move y.tab.c $(InputDir)\grammar.c \
	move y.tab.h $(InputDir)\grammar.h \
	

"$(InputDir)\grammar.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\grammar.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\lex.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\lexerr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\lexfunc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\objarray.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\object.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\objenv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\objres.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\proc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\symbol.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\symtab.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\treegen.c

!IF  "$(CFG)" == "libxl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\valmeth.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\valoper.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\value.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\vm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xlasm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xldebug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xlmap.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xlstr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xltools.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\libxl\xltree.c

!IF  "$(CFG)" == "libxl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "XL Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\include\axl\axl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\axl\xlcore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\axl\xlmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\axl\xlstr.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\..\src\libxl\libxl.def

!IF  "$(CFG)" == "libxl - Win32 Release"

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "libxl - Win32 Debug Static"

!ELSEIF  "$(CFG)" == "libxl - Win32 Release Static"

!ENDIF 

# End Source File
# End Target
# End Project
