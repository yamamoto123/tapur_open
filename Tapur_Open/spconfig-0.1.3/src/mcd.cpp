/*
 * Softpixel MemCheckDeluxe v1.0.2
 *  2002 cwright <cwright@softpixel.com>
 */

/* Copyright (C) 2002 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */


#ifdef _MCD_CHECK
#undef _MCD_CHECK	//don't want trace versions in this file =)
#endif
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include "mcd.h"

typedef struct ChunkS
{
	int size;
	int line;
	char *function;
	char *file;
	void *ptr;
	struct ChunkS *next;
}Chunk;

Chunk *MemoryChunks=0,*LastChunk=0;
unsigned int smallest=0,largest=0,maximum=0,current=0;

void addChunk(void*p,unsigned int size,char*fun,char*file,int line)
{
	Chunk *c=(Chunk*)malloc(sizeof(Chunk));

	c->size=size;
	c->line=line;
	c->function=fun;
	c->file=file;
	c->ptr=p;
	c->next=NULL;
	current+=size;
	if(current>maximum)
		maximum=current;
	if(size>largest)
		largest=size;
	if(size<smallest || smallest==0)
		smallest=size;

	if(!MemoryChunks)
	{
		MemoryChunks=c;
		LastChunk=c;
	}
	else
	{
		/*s=MemoryChunks;
		while(s->next)
		{
			s=s->next;
		}
		s->next=c;*/
		LastChunk->next=c;
		LastChunk=c;
	}
}

void delChunk(void*p,char*fun,char*file,int line)
{
	Chunk *s,*prev;

	if(!MemoryChunks)
	{
		printf("hmm, empty list\n");
		return;
	}
	if(p==MemoryChunks->ptr)	//first chunk?
	{
		if(MemoryChunks->next)	//fix up MemoryChunks
		{
			prev=MemoryChunks;
			MemoryChunks=MemoryChunks->next;
			current-=prev->size;
			free(prev);
		}
		else			//no more chunks
		{
			current-=MemoryChunks->size;
			free(MemoryChunks);
			MemoryChunks=0;
			LastChunk=0;
		}
		return;
	}
	else	//drat, must walk list and see where it is
	{		
		s=MemoryChunks;
		prev=NULL;
		//printf("\n");
		while(s)
		{
			//printf("[%X][%X]\n",(int)s->ptr,(int)p);
			if(s->ptr==p)
			{
				if(s->next)
				{
					prev->next=s->next;
				}
				else	//erasing lastchunk.  better keep it safe.
				{
					prev->next=NULL;
					LastChunk=prev;
				}
				current-=s->size;
				free(s);
				return;
			}
			prev=s;
			s=s->next;
		}
	}
	//printf("[%X] Not in list[%s][%s][%i]\n",(int)p,fun,file,line);
}

void *MCD_malloc(int size,char*fun,char*file,int line)
{
	void*p=malloc(size);
#ifdef MCD_VERBOSE
	fprintf(stderr,"malloc [%X][%ib] from %s:%s, line %i\n",
			(int)p,size,file,fun,line);
#endif
	addChunk(p,size,fun,file,line);
	return p;
}

void *MCD_calloc(int size,char*fun,char*file,int line)
{
	void*p=calloc(size,1);
#ifdef MCD_VERBOSE
	fprintf(stderr,"calloc [%X][%ib] from %s:%s, line %i\n",
			(int)p,size,file,fun,line);
#endif

	addChunk(p,size,fun,file,line);
	return p;
}

void *MCD_realloc(void *p,int size,char*fun,char*file,int line)
{
	void*n=realloc(p,size);
#ifdef MCD_VERBOSE
	fprintf(stderr,"realloc [%X]->[%X][%ib] from %s:%s, line %i\n",
			(int)p,(int)n,size,file,fun,line);
#endif

	delChunk(p,fun,file,line);
	addChunk(n,size,fun,file,line);
	return n;
}

char *MCD_strdup(char*s,char*fun,char*file,int line)
{
	char*n=strdup(s);
#ifdef MCD_VERBOSE
	fprintf(stderr,"strdup [%X] from %s:%s, line %i\n",
			(int)n,file,fun,line);
#endif

	addChunk(n,strlen(n),fun,file,line);
	return n;
}

void MCD_free(void *p,char*fun,char*file,int line)
{
	delChunk(p,file,fun,line);
#ifdef MCD_VERBOSE
	fprintf(stderr,"free [%X] from %s:%s, line %i\n",
			(int)p,file,fun,line);
#endif

	free(p);
}

void showMemStats()
{
	Chunk*c=MemoryChunks;
	int total=0;
	if(!c)
		return;
	printf("Memory Stats:\n");
	while(c && c->next)
	{
		printf("\t%i bytes allocated in %s:%s, line %i [%X]\n",c->size,c->function,c->file,c->line,(int)c->ptr);
		total+=c->size;
		c=c->next;
	}
	printf("\t\ttotal bytes allocated and not freed: %i\n",total);
	printf("\t\tPeak memory usage:                   %i\n",maximum);
	printf("\t\tLargest allocation:                  %i\n",largest);
	printf("\t\tSmallest allocation:                 %i\n",smallest);
	printf("\n");
}
