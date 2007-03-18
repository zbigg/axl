/*
* File:		dconfig.h
*
* Id:		$Id: dconfig.h 1.1 03/10/16 00:02:54+02:00 zbigg@marl.(none) $
*
* Directory config header for Dev-C++.
*/

/* Installation prefix */
#define PACKAGE_PREFIX "."
/* Binaries directory */
#define PACKAGE_BINDIR PACKAGE_PREFIX
/* System binaries directory */
#define PACKAGE_SBINDIR  PACKAGE_PREFIX
/* Libraries directory */
#define PACKAGE_LIBDIR  PACKAGE_PREFIX"\\lib"
/* Data directory */
#define PACKAGE_DATADIR PACKAGE_PREFIX
/* Config directory */
#define PACKAGE_CONFDIR PACKAGE_PREFIX"\\conf"
/* Include directory */
#define PACKAGE_INCDIR PACKAGE_PREFIX"\\include"
/* Package private include directory */
#define PACKAGE_PKG_INCDIR PACKAGE_PREFIX"\\include\\axl"
/* Package private libraries directory */
#define PACKAGE_PKG_LIBDIR PACKAGE_PREFIX"\\modules"
/* Package private data directory */
#define PACKAGE_PKG_DATADIR PACKAGE_PREFIX"\\share"
