#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#ifdef WIN32
    #include <windows.h>
    #include <io.h>
    #include <process.h>
    #define  MYVOID             void

#endif
//Log{
#define MAXLOGSIZE 2000000
#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
char logfilename1[]="MyLog1.log";
char logfilename2[]="MyLog2.log";
char logstr[16000];
char datestr[16];
char timestr[16];
char mss[4];
CRITICAL_SECTION cs_log;
FILE *flog;
#ifdef WIN32
void InitLog()
{
	InitializeCriticalSection(&cs_log);
}
void QuitLog()
{
	DeleteCriticalSection(&cs_log);
}
void Lock(CRITICAL_SECTION *l) {
    EnterCriticalSection(l);
}
void Unlock(CRITICAL_SECTION *l) {
    LeaveCriticalSection(l);
}
void sleep_ms(int ms) {
    Sleep(ms);
}

#endif
void LogV(const char *pszFmt,va_list argp) {
    struct tm *now;
    struct timeb tb;

 
    if (NULL==pszFmt||0==pszFmt[0]) return;
    if (-1==_vsnprintf(logstr,ARRSIZE(logstr),pszFmt,argp)) logstr[ARRSIZE(logstr)-1]=0;
    ftime(&tb);
    now=localtime(&tb.time);
    sprintf(datestr,"%04d-%02d-%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday);
    sprintf(timestr,"%02d:%02d:%02d",now->tm_hour     ,now->tm_min  ,now->tm_sec );
    sprintf(mss,"%03d",tb.millitm);
    printf("%s %s.%s %s\n",datestr,timestr,mss,logstr);
    flog=fopen(logfilename1,"a");
    if (NULL!=flog) {
        fprintf(flog,"%s %s.%s %s\n",datestr,timestr,mss,logstr);
        if (ftell(flog)>MAXLOGSIZE) {
            fclose(flog);
            if (rename(logfilename1,logfilename2)) {
                remove(logfilename2);
                rename(logfilename1,logfilename2);
            }
            flog=fopen(logfilename1,"a");
            if (NULL==flog) return;
        }
        fclose(flog);
    }
}
void Log(const char *pszFmt,...) {

	
    va_list argp;
 
    Lock(&cs_log);
	 
    va_start(argp,pszFmt);
    LogV(pszFmt,argp);
    va_end(argp);
    Unlock(&cs_log);
}
