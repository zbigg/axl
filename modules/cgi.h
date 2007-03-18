/* -*- zz-c -*- */
/*
  File  : cgi.h

  Author   :  Zbigniew Zagorski
  e-mail   :  <zzbigg@o2.pl>


*/

#ifndef __cgi_h_
#define __cgi_h_

#ifdef __cplusplus
extern "C" {
#endif

/** Fills buf with value of parameter name.
 (name is name of object in html form).
 Warning. This function is UNSAFE.
 Should be used only with cgi_getparamlen.
*/
int cgi_getparam(char *name,char *buf);

/** Returns pointer to specified param str.
*/
char* cgi_getparam_ex(char* name);

/** Returns length of param.
*/
int   cgi_getparamlen(char* name);

/** Returns pointer to cgi parameters.
*/
char * cgi_getinputstr();

/** Checks CONTENT_LENGTH and reads data from STDIN . On error returns 0 else 1.
*/
int cgi_init(void);

/** Frees parameters memory. Should be called at end of program.
*/
void cgi_done(void);

/** Parses CGI-style string 
*/
int cgi_parsestring(char *str, char *output);

extern int cgi_readed;
#ifdef __cplusplus
}
#endif

#endif   //__cgi_h_
