/* stack.c - v0.1.3
 *	* implementation of a basic procedural stack
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <malloc.h>
#include "debug.h"
#include "stack.h"

#define INITCACHE	256
//#define SPC_USE_MALLOC_CACHE	//comment me outto disable malloc caching, it might be buggy/slower

/* This should someday do a malloc cache so we dont have to 
malloc and free so many times. not only a performance hit, 
but also an instability issue with some gcc's */

/*initCache initializes our cache, and sets up some sort of caching thing*/
/*finiCache cleans up all stuff set up by initCache or expanded by expandCache*/
/*expandCache is called nowhere yet, but will expand the cache as necessary*/
//init and fini are called from load.c (parseMain).  expand is used internally

Stack	*stackBucket=NULL;
unsigned char	*stackBucketMask=NULL;
int	stackBucketSize=0;

/* current malloc cacher:  we malloc INITCACHE Stacks, and manage those.  
a mask is used to see which ones are in use.
*/

int initCache()
{
	ENTRY;
#ifdef SPC_USE_MALLOC_CACHE
	stackBucket=(Stack*)malloc(sizeof(Stack)*INITCACHE);
	stackBucketSize=INITCACHE;
	stackBucketMask=calloc(INITCACHE/sizeof(char),1);
#endif
	return(0);
}

void	finiCache()
{
	ENTRY;
#ifdef SPC_USE_MALLOC_CACHE
	free(stackBucket);
	free(stackBucketMask);
#endif
	EXIT;
}

void	expandCache()
{
	ENTRY;
#ifdef SPC_USE_MALLOC_CACHE
	stackBucket=realloc(stackBucket,stackBucketSize*2);
	stackBucketMask=realloc(stackBucketMask,stackBucketSize/sizeof(char));
	stackBucketSize*=2;	
#endif
	EXIT;
}

Stack*	newStack()
{
#ifdef SPC_USE_MALLOC_CACHE
	unsigned int i,j;
	ENTRY;
	for(i=0;i<stackBucketSize;++i)
	{
		if(stackBucketMask[i]!=0xff)
		{//look for which one isnt used
			for(j=1;j<256;j=j<<1)
				if(!stackBucketMask[i]&j)
				{	//got it
					stackBucketMask[i]|=j;	//set bucket flag
					return(stackBucket+(i*sizeof(char)+j)*sizeof(Stack));
				}
		}
	}
	//blast, didn't find it =(
	expandCache();
	return newStack();

#else
	Stack*s;
	ENTRY
	s=(Stack*)malloc(sizeof(Stack));
	s->stack=0;	//no data
	s->depth=0;	//no data
	return(s);
#endif
}

item*	newItem()
{
	item*i;
	ENTRY;
	i=(item*)malloc(sizeof(item));
	return(i);
}

void	freeItem(item*i)
{
	ENTRY
	if(i)
	{
		if(i->next)
			freeItem(i->next);
		free(i);
	}
	EXIT
}

void freeStack(Stack*s)
{
#ifdef SPC_USE_MALLOC_CACHE
	//clear bucket flag.
	unsigned int offset=(s-stackBucket)/sizeof(Stack);
	ENTRY;
	stackBucketMask[offset/sizeof(char)]^=
			1<<(offset%sizeof(char));
#else
	ENTRY;
	if(s)
	{
		DEBUG(-12,"Stack Depth [%i]\n",s->depth);
		while(s->depth>0)
			stackPop(s);
		free(s);
	}
#endif
	EXIT;
}

void stackPush(Stack*stack,void*val)
{
	int x;
	item *i;
	ENTRY;
	i=newItem();
	DEBUG(-96,"got new item [%x][%x]\n",(int)i,(int)stack->stack);
	i->next=stack->stack;
	i->val=val;
	DEBUG(-96,"Pushed value [%x]\n",(int)val);
	stack->stack=i;
	++stack->depth;
	i=stack->stack;
	for(x=0;x<stack->depth;++x)
	{
		DEBUG(-127,"stack vals[%i]: [%x]\n",x,(int)i);
		i=i->next;
	}
	EXIT;
}

void* stackPop(Stack*stack)
{
	void	*ret=(void*)-1;
	item	*tmp;
	ENTRY;
	DEBUG(-64,"Popping stack [%x][%i]\n",(int)stack,stack->depth);
	if(stack && stack->depth)
	{
		tmp=stack->stack;	//isolate the stack struct
		DEBUG(-4,"tmp is [%x]\n",(int)tmp);
		if(tmp)
		{
			ret=tmp->val;		//get the ret val
			tmp=tmp->next;		//next item on the stack
			stack->stack->next=0;	//stop freeItem from cleaning stack
			freeItem(stack->stack);	//free the popped value
		}
		stack->stack=tmp;	//restore the remainder of stack
		--stack->depth;
		return(ret);
	}
	return((void*)-1);
}
