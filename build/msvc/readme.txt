$Id: readme.txt 1.1 03/10/16 00:02:54+02:00 zbigg@marl.(none) $

The README file for M$ Visual Studio (R)(TM) (as thay want ;-P)
build.

If you want to fast build AXL using MS Visual C++ IDE (R)(TM) simply
open axl.dsw workspace in IDE.

Select the InstallBin project and Release configuration.

Edit InstallBin.mak (seen in Workspace Window) and change
installation directory as described.

Now you can build the AXL by selecing "Build" command on current
(InstallBin) project.

The projects are:
	libxl/libxl.dsp
    	building AXL library - DLL
	axl/axl.dsp
		build commandline tool axl
	stdlib/stdlib.dsp
		build module stdaxl.dll - standard AXL library

Output files go into .\out_debug or .\out_release directory, depending
on active configuration.


