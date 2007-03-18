/*
* File:		config.h
*
* Id:		$Id: config.h 1.1 03/10/16 00:02:54+02:00 zbigg@marl.(none) $
*
* Config header for Dev-C++
*/
/* config.h.in.  Generated from configure.in by autoheader.  */

/* define if Allegro game programming library is used */
#undef AXL_WITH_ALLEGRO

/* define if gd library is used dupa*/
#undef AXL_WITH_POSTGRESQL

/* define if Readline library is used */
#undef AXL_WITH_READLINE

/* tell other libraries that we're inside axl */
#define BUILD_XL 1

/* define if gd library is used and has truecolor capabilities */
#undef GD_TRUECOLOR

/* Define to 1 if you have the `access' function. */
#define HAVE_ACCESS 1

/* define if have CreateFile system call */
#define HAVE_CREATEFILE 1

/* Define to 1 if you have the `dlopen' function. */
#undef HAVE_DLOPEN

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
#undef HAVE_DOPRNT

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fork' function. */
#undef HAVE_FORK

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `gettimeofday' function. */
#undef HAVE_GETTIMEOFDAY

/* Define to 1 if you have the <history.h> header file. */
#undef HAVE_HISTORY_H

/* Define to 1 if you have the `inet_aton' function. */
#define HAVE_INET_ATON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the `dl' library (-ldl). */
#undef HAVE_LIBDL

/* Define to 1 if you have the <libpg-fe.h> header file. */
#undef HAVE_LIBPG_FE_H

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* define if have LoadLibrary system call */
#define HAVE_LOADLIBRARY 1

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mktime' function. */
#define HAVE_MKTIME 1

/* Define to 1 if you have the <pgsql/libpq-fe.h> header file. */
#undef HAVE_PGSQL_LIBPQ_FE_H

/* Define to 1 if you have the `pipe' function. */
#undef HAVE_PIPE

/* Define to 1 if you have the `popen' function. */
#undef HAVE_POPEN

/* Define to 1 if you have the <readline.h> header file. */
#undef HAVE_READLINE_H

/* Define to 1 if you have the <readline/history.h> header file. */
#undef HAVE_READLINE_HISTORY_H

/* Define to 1 if you have the <readline/readline.h> header file. */
#undef HAVE_READLINE_READLINE_H

/* Define to 1 if you have the `sendfile' function. */
#undef HAVE_SENDFILE

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#undef HAVE_STRCASECMP

/* Define to 1 if you have the `strcspn' function. */
#define HAVE_STRCSPN 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define to 1 if you have the `stricmp' function. */
#define HAVE_STRICMP 1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtod' function. */
#define HAVE_STRTOD 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* always have zpio library */
#define HAVE_ZPIO 1

/* Define to 1 if you have the `_pipe' function. */
#define HAVE__PIPE 1

/* Define to 1 if you have the `_popen' function. */
#define HAVE__POPEN 1

/* package short name */
#define PACKAGE axl

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "zzbigg@o2.pl"

/* Define to the full name of this package. */
#define PACKAGE_NAME "AXL"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "AXL Application EXtentsion Library"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "axl"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.1.14"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#undef TIME_WITH_SYS_TIME

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
#define TM_IN_SYS_TIME 1

/* package version */
#define VERSION "0.1.14"

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define as `__inline' if that's what the C compiler calls it, or to nothing
   if it is not supported. */
#undef inline

/* Define to `int' if <sys/types.h> does not define. */
#undef mode_t

/* Define to `long' if <sys/types.h> does not define. */
#undef off_t

/* Define to `unsigned' if <sys/types.h> does not define. */
#define size_t

/* Define to `long' if <sys/types.h> does not define. */
#undef ssize_t
