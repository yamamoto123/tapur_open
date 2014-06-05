/* debug.h, v2.0 (posix,win32) */
	
#include <stdio.h>
	
#ifdef USE_MCD
#define _MCD_CHECK
#endif

#include "mcd.h"

#ifndef DEBUG_INCLUDES
#define DEBUG_INCLUDES

extern FILE* debugfd_spconfig;
extern signed char debuglevel_spconfig;
extern unsigned long debugdepth_spconfig,debugcount_spconfig;

#ifdef SP_DEBUG

#ifdef WIN32
	#include <stdarg.h>
	#define DEBUG	WinDEBUG
	void WinDEBUG(char level,char*string,...);
#else
	#define DEBUG(l,x...)	if(l>=debuglevel_spconfig)\
	{\
	for(debugcount_spconfig=0;debugcount_spconfig<debugdepth_spconfig;++debugcount_spconfig)\
		fprintf(debugfd_spconfig?debugfd_spconfig:stderr,"\t");\
	fprintf(debugfd_spconfig?debugfd_spconfig:stderr,__FUNCTION__ ": \t" x);\
	fflush(debugfd_spconfig);\
	}
#endif

#define DEBUGFD(x)	debugfd_spconfig=x;
#define DEBUGLEVEL(x)	debuglevel_spconfig=x;
#define ENTRY		DEBUG(-64,"Entry\n");++debugdepth_spconfig;
#define EXIT		--debugdepth_spconfig;DEBUG(-64,"Exit\n");
#define return(x)	do{EXIT;return x;}while(0)
#define MEMSTAT 	showMemStats();

#else	//SP_DEBUG

#ifdef WIN32
	#include <stdarg.h>
	#define DEBUG	WinDEBUG
	void WinDEBUG(char level,char*string,...);
#else
	#define DEBUG(l,x...)
#endif

#define DEBUGFD(x)	
#define DEBUGLEVEL(x)
#define ENTRY		
#define EXIT		
#define MEMSTAT

#endif	//SP_DEBUG

#endif
