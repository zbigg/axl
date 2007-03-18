/*
* File:		uconfig.h
*
* Id:		$Id: uconfig.h 1.1 03/10/16 00:02:54+02:00 zbigg@marl.(none) $
*
*  User config header.

    If configure script fails to detect some feature that your system
    has and you want to use you should uncomment it here.
*/

/* Build MySQL module */
/* #define AXL_WITH_MYSQL 1 */

/* Build PostgreSQL module */
/* #define AXL_WITH_POSTGRESQL 1 */

/* Build PostgreSQL module */
/* #define AXL_WITH_ALLEGRO 1 */

/* Build Readline module */
/* #define AXL_WITH_READLINE 1 */

/* Build gd module */
/* #define AXL_WITH_GD 1 */

/* Build libgd have gdImageCreateTrueColor */
/* #define GD_TRUECOLOR 1 */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #define ssize_t 1 */

/*define if have zpio library */
/* #define HAVE_ZPIO 1 */

/*define if have CreateFile function from Win32 API */
/* #define HAVE_CREATEFILE 1 */

/*define if have LoadLibrary function from Win32 API */
/* #define HAVE_LOADLIBRARY 1 */


/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #define const 1 */

/* Define if you don't have vprintf but do have _doprnt.  */
/* #define HAVE_DOPRNT 1 */

/* Define if you have the strftime function.  */
/* #define HAVE_STRFTIME 1 */

/* Define if you have the vprintf function.  */
/* #define HAVE_VPRINTF 1 */

/* Define as __inline if that's what the C compiler calls it.  */
/* #define inline __inline */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #define mode_t int */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #define off_t long */

/* Define as the return type of signal handlers (int or void).  */
/* #define RETSIGTYPE 1 */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #define size_t unsigned */

/* Define if you have the ANSI C header files.  */
/* #define STDC_HEADERS 1 */

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
/* #define TIME_WITH_SYS_TIME 1 */

/* Define if your <sys/time.h> declares struct tm.  */
/* #define TM_IN_SYS_TIME 1 */

/* Define if you have the _popen function.  */
/* #define HAVE__POPEN 1 */

/* Define if you have the access function.  */
/* #define HAVE_ACCESS 1 */

/* Define if you have the dlopen function.  */
/* #define HAVE_DLOPEN 1 */

/* Define if you have the fork function.  */
/* #define HAVE_FORK 1 */

/* Define if you have the gettimeofday function.  */
/* #define HAVE_GETTIMEOFDAY 1 */

/* Define if you have the mktime function.  */
/* #define HAVE_MKTIME 1 */

/* Define if you have the pipe function.  */
/* #define HAVE_PIPE 1 */

/* Define if you have the popen function.  */
/* #define HAVE_POPEN 1 */

/* Define if you have the sendfile function.  */
/* #define HAVE_SENDFILE 1 */

/* Define if you have the socket function.  */
/* #define HAVE_SOCKET 1 */

/* Define if you have the strcasecmp function.  */
/* #define HAVE_STRCASECMP 1 */

/* Define if you have the strcspn function.  */
/* #define HAVE_STRCSPN 1 */

/* Define if you have the strdup function.  */
/* #define HAVE_STRDUP 1 */

/* Define if you have the strerror function.  */
/* #define HAVE_STRERROR 1 */

/* Define if you have the stricmp function.  */
/* #define HAVE_STRICMP 1 */

/* Define if you have the strstr function.  */
/* #define HAVE_STRSTR 1 */

/* Define if you have the strtod function.  */
/* #define HAVE_STRTOD 1 */

/* Define if you have the strtol function.  */
/* #define HAVE_STRTOL 1 */

/* Define if you have the <fcntl.h> header file.  */
/* #define HAVE_FCNTL_H 1 */

/* Define if you have the <history.h> header file.  */
/* #define HAVE_HISTORY_H 1 */

/* Define if you have the <libpg-fe.h> header file.  */
/* #define HAVE_LIBPG_FE_H 1 */

/* Define if you have the <limits.h> header file.  */
/* #define HAVE_LIMITS_H 1 */

/* Define if you have the <malloc.h> header file.  */
/* #define HAVE_MALLOC_H 1 */

/* Define if you have the <pgsql/libpq-fe.h> header file.  */
/* #define HAVE_PGSQL_LIBPQ_FE_H 1 */

/* Define if you have the <readline.h> header file.  */
/* #define HAVE_READLINE_H 1 */

/* Define if you have the <readline/history.h> header file.  */
/* #define HAVE_READLINE_HISTORY_H 1 */

/* Define if you have the <readline/readline.h> header file.  */
/* #define HAVE_READLINE_READLINE_H 1 */

/* Define if you have the <sys/time.h> header file.  */
/* #define HAVE_SYS_TIME_H 1 */

/* Define if you have the <unistd.h> header file.  */
/* #define HAVE_UNISTD_H 1 */

/* Define if you have the dl library (-ldl).  */
/* #define HAVE_LIBDL 1 */
