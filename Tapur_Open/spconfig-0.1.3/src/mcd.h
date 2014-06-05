/*
 * Softpixel MemCheckDeluxe v1.0.2
 *  2002 cwright <cwright@softpixel.com>
 */

/* Copyright (C) 2002 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */


#ifndef MCD_H
#define MCD_H
#ifdef _MCD_CHECK

#ifdef WIN32
	#define __FUNCTION__ __FILE__
#endif

#warning Building with memory checking.  expect lower performance.

#ifdef strdup
#undef strdup
#define strdup(p)	MCD_strdup(p,__FUNCTION__,__FILE__,__LINE__)
#endif

#define malloc(size)	MCD_malloc(size,__FUNCTION__,__FILE__,__LINE__)
#define calloc(n,s)	MCD_calloc(s*n,__FUNCTION__,__FILE__,__LINE__)
#define	realloc(p,s)	MCD_realloc(p,s,__FUNCTION__,__FILE__,__LINE__)
#define free(p)		MCD_free(p,__FUNCTION__,__FILE__,__LINE__)

#endif

/*!!! These are called by the defines only.  dont use directly. !!!*/
void *MCD_malloc(int size,char*,char*,int);
void *MCD_calloc(int size,char*,char*,int);
void *MCD_realloc(void *p,int size,char*,char*,int);
char *MCD_strdup(char*s,char*,char*,int);
void MCD_free(void *p,char*,char*,int);

//call me!
void showMemStats(void);

#endif
