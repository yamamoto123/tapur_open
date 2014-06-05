/* spconfig.c - v0.1.3
 *	* some general library routines
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include "spconfig.h"
#include "debug.h"

/*
callback isn't implemented elsewhere just yet

int	spConfigIncludeCallbackSet(spConfigNode*root,void*(callback))
{
	ENTRY
	if(root->callback)
	{
		DEBUG(1,"Previous Callback Overwritten\n");
		root->callback=callback;
		return(1);
	}
	root->callback=callback;
	return(0);
}

int	spConfigIncludeCallbackRemove(spConfigNode*root)
{
	ENTRY

	if(root->callback)
	{
		root->callback=0;
		return(0);
	}

	DEBUG(0,"Unnecessary Callback Remove\n");

	return(0);
}*/



/*some handy functions that don't exist elsewhere*/

/* not always present, so we'll use our own */
char*Strndup(char*str,int n)
{
	char *pstr;
	int i;
	ENTRY
	pstr=(char *)calloc(n+1,1);
	if(!pstr)
	{
		DEBUG(3,"Calloc Failed\n");
		return(0);
	}

	for(i=0;i<n;++i)
		pstr[i]=str[i];

	return(pstr);
}

/* re-allocating snprintf */
/* because we can't rely on vsnprintf to return correct values,
we must do some calculations by hand.*/
/*further, malloc is bad. so this is dead until libc cleans up its act*/
#if 0

char* ravsnprintf(char*buffer,int *size,int *offset,char*fmt,...)
{
	char*pos;
	int len,size_remaining;
	va_list args;
	va_start(args,fmt);

	ENTRY

	DEBUG(-3,"size [%d] offset [%d]\n",*size,*offset);

	DEBUG(-8,"buffer [%x][%s]\n",(int)buffer,buffer);

	pos=buffer+*offset;

	//size_remaining=*size-(*size-*offset-1);
	size_remaining=*size-*offset-1;
	DEBUG(-3,"Bytes Remaining [%d]\n",size_remaining);

	len=vsnprintf(pos,*size-*offset-1,fmt,args);
	va_end(args);

	DEBUG(-4,"Looping...\n");

	while(len == -1 || len >= size_remaining)
	{
		DEBUG(-2,"Expanding STString Size...\n");
		buffer=(char*)SPrealloc(buffer,*size*2,*size);
		if(!buffer)
		{
			DEBUG(-2,"Realloc failed\n");
			va_end(args);
			return(0);
		}
		size_remaining+=*size;	//double length
		*size*=2;
		DEBUG(-2,"Expanded Size [%d]\n",*size);
		va_start(args,fmt);
		len=vsnprintf(pos,*size-*offset-1,fmt,args);
		va_end(args);
		DEBUG(-1,"Returned len: [%d] out of [%d]\n",len,size_remaining);
	}
	DEBUG(-8,"1st char [%d:%c]\n",pos[0],pos[0]);
	DEBUG(-3,"Length Printed [%d]\n",len);
	*offset+=len;
	va_end(args);
	return(buffer);
}
#endif
