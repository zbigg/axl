 /*
   * File:	sgetopt.h
   *
   * Id:	$Id: sgetopt.h 1.3 04/01/14 16:36:36+01:00 zbig@pentagon.eu.org $
   *
   * Project:	common use utilities
   *
   * Author Zbigniew Zagorski <zzbigg@o2.pl>
   *
   * Copyright:
   *
   *   This program is Copyright(c) Zbigniew Zagorski 2002,
   *   All rights reserved, and is distributed as free software under the
   *   license in the file "LICENSE", which is included in the distribution.
 */
#ifndef __sgetopt_h_
#define __sgetopt_h_

#define    SGO_ALLOW_POSTFIX	1
#define	   SGO_NO_PARAM		2
#define    SGO_BOOL		1
#define    SGO_NEGBOOL		2
#define    SGO_STRING		3
#define    SGO_INT		4
#define    SGO_CALLBACK	        5
#define    SGO_DOUBLE		6

struct sgo_rec {
   char*    name;
   char*    shortcut;
   int	    flags;
   int	    type;
   void*    param;
   long	    size;
   const char* info;
};

typedef struct sgo_rec sgo_rec;
/*
   ##
   ## =Function sgetopt
   ##
   ##   parse commandline options
   ##
   ## =Synopsis
   ##	|#include "sgetopt.h"
   ##	|
   ##	|int sgetopt(struct sgo_rec* rec,int no,
   ##   |            int* argc,char*** argv,int flags);
   ##
   ## =Description
   ##
   ##	This function gets table 
   ##    |
   ##    |struct sgo_rec {
   ##    |   char*       name;
   ##    |   char*       shortcut;
   ##    |   int         flags;
   ##    |   int         type;
   ##    |   void*       param;
   ##    |   long        size;
   ##    |   const char* info;
   ##    |};
   ##	 
   ##	 These constants are passed as flags:
   ##    |  #define    SGO_ALLOW_POSTFIX  1
   ##    |  #define    SGO_NO_PARAM       2
   ##	
   ##	 These are possible types of data:
   ##    |  #define    SGO_BOOL         1
   ##    |  #define    SGO_NEGBOOL	2
   ##    |  #define    SGO_STRING       3
   ##    |  #define    SGO_INT		4
   ##    |  #define    SGO_CALLBACK	5
   ##    |  #define    SGO_DOUBLE       6
   ##
   ##	 
   ##
   ## =Return value
   ##
   ##
   ##
   ## =Author
   ##
   ##	Zbigniew Zagorski <zzbigg@o2.pl>
   ##
*/
int sgetopt(struct sgo_rec* rec,int no,
            int* argc,char*** argv,int flags,
	    void (*usage)(const char* msg));

int sgetopt_genusage(struct sgo_rec* records,int no,FILE* output);

#endif

