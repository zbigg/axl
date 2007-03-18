###################################################################
#
# Platform     : MSW
#
# File	       : InstallBin.mak
#
# Project      : axl
#
# Id           : $Id: InstallBin.mak 1.4 04/01/19 00:47:56+01:00 nikt@nowy.dom $
#
# 	Author Zbigniew Zagorski <zzbigg@o2.pl>
#
###################################################################
#
!IF "$(DESTDIR)" == ""
#
# To change install directory change path that appears below
#
DESTDIR=\axl
#
!ENDIF

install:
	@echo off
	@cd $(BDIR)
	@-mkdir "$(DESTDIR)"
	@-mkdir "$(DESTDIR)\modules"

	@copy axl.exe "$(DESTDIR)"
	@copy $(XLNAME).dll  "$(DESTDIR)"
	@copy $(ZCOMPATNAME).dll "$(DESTDIR)"
	
	@-copy modules\*.dll "$(DESTDIR)\modules"

	@cd ..
	@-mkdir "$(DESTDIR)\modules"
	@-mkdir "$(DESTDIR)\modules\axl"
	@cd ..\..\stdlib\axlinc
	@copy "*.*" "$(DESTDIR)\modules"
	@cd axl
	@copy "*.*" "$(DESTDIR)\modules\axl"
	@cd ..\..\..
	@cd modules\axlinc
	@copy "*.*" "$(DESTDIR)\modules"




	
