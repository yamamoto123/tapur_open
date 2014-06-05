/*debug.c, V2.0 (posix, win32)*/

#include "debug.h"

FILE*debugfd_spconfig=0;
signed char debuglevel_spconfig=-100;
unsigned long debugdepth_spconfig=2, debugcount_spconfig=0;

#ifdef SP_DEBUG

#ifdef WIN32
/* msvc doesn't like var args in macros.  so we make it a function */
#include <stdarg.h>

void WinDEBUG(char l, char*string, ...)
{
	va_list args;
	va_start(args,string);
	if(l>=debuglevel_spconfig)
	{
	//for(debugcount_spconfig=0;debugcount_spconfig<debugdepth_spconfig;++debugcount_spconfig)
	//	fprintf(debugfd_spconfig?debugfd_spconfig:stderr,"\t");
	//vfprintf(debugfd_spconfig?debugfd_spconfig:stderr,__FILE__ ": \t",args);
	fprintf(debugfd_spconfig?debugfd_spconfig:stderr,__FILE__ ": ");
	vfprintf(debugfd_spconfig?debugfd_spconfig:stderr, string, args);
	}
	va_end(args);
}
#endif

void DEBUGFD_spconfig(FILE*f){debugfd_spconfig=f;}
void DEBUGLEVEL_spconfig(signed char l){debuglevel_spconfig=l;}
void DEBUGMEMSTAT_spconfig(void){MEMSTAT;}

#else	//SP_DEBUG
#ifdef WIN32
	void WinDEBUG(char l,char*string,...){return;}
#endif

void DEBUGFD_spconfig(FILE*f){}
void DEBUGLEVEL_spconfig(signed char l){}
void DEBUGMEMSTAT_spconfig(void){}


#endif	//SP_DEBUG
