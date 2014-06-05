/* find.c - v0.1.3
 *	* functions relating to finding a node/attribute within a
 *		memorystructure
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <malloc.h>
#include <string.h>
#include "spconfig.h"
#include "debug.h"

// This code makes use of non-strict data types.  
//        if this bugs you, don't read it :)

/* internal funcs */

/* 
a few of these are node/attribute neutral, relying on a 'type'
param to decide on nodes (set) or attributes (unset)

further, a found param is passed to indicate whether we have
found a match (for Next searches).  in this case, a 0 means not found
(looking for found first), and a 1 means we've found it already.
if we find the reference, but not the next one, return -1
*/

//search node (shallow)
void *searchNode(spConfigNode*node, char*name, void*ref, char type, char found)
{
	int i;
	void *ret=0;

	ENTRY;
	DEBUG(-120,"name [%s] ref [%x] type [%i] found [%i]\n",name,(int)ref,type,found);
	DEBUG(-119,"Children [%i]\n",node->numChildren);
	if(type)	//node mode
	{
		for(i=0;i<node->numChildren;++i)
		{
			DEBUG(-127,"Matching [%s] and [%s]\n",name,node->children[i]->name);
			if(found)	//looking for match
			{
				if(!strcmp(node->children[i]->name,name))
				{
					DEBUG(-125,"got it.\n");
					return((void*)node->children[i]);
				}
			}
			else		//looking for reference
			{
				if(node->children[i]==ref)
				{
					DEBUG(-125,"got ref?\n");
					++found;
					ret=(void*)-1;
				}
			}
		}
	}
	else		//attribute mode
	{
		for(i=0;i<node->numAttributes;++i)
		{
			if(found)	//looking for match
			{
				if(!strcmp(node->attributes[i]->name,name))
				{
					DEBUG(-125,"got it\n");
					return((void*)node->attributes[i]);
				}
			}
			else
			{
				if(node->attributes[i]==ref)
				{
					DEBUG(-125,"got ref\n");
					++found;
					ret=(void*)-1;
				}
			}
		}

	}
	return((void*)ret);	//might have found the reference...
}

//search child nodes recursivly, then self
void *searchDeep(spConfigNode*node,char *name,void*ref,char type,char found)
{
	int i;
	void *ret=NULL;
	ENTRY;

	if(ref==NULL)
		found=1;

	for(i=0;i<node->numChildren;++i)
	{
		ret=searchDeep(node->children[i],name,ref,type,found);
		if(ret)
		{
			if(ret==(void*)-1)
			{
				DEBUG(-125,"Looks like we found something\n");
				++found;
			}
			else
				return(ret);
	}
	}

	//ok, so it wasn't there, but maybe ref was, lets look in root node
	return(searchNode(node,name,ref,type,found));
	
}

/**********************/
/* The find API stuff */
/**********************/

spConfigNode *spConfigNodeFind(spConfigNode *node,char *name,char deep,spConfigNode *found)
{
	spConfigNode *ret=NULL;
	ENTRY;
	if(deep)
	{
		ret=(spConfigNode*)searchDeep(node,name,(void*)found,1,0);
	}
	else
	{
// PAAK		ret=searchNode(node,name,(void*)found,1,0);
		ret=(spConfigNode*)searchNode(node,name,(void*)found,1,1);
	}

	if(ret==(spConfigNode*)-1)	//found ref, but nothing after it
		ret=NULL;	//pretend like nothing happened

	return(ret);
}

// PAAK spConfigAttribute *spCondfigAttributeFind(spConfigNode *node,char *name,char deep, spConfigAttribute *found)
spConfigAttribute *spConfigAttributeFind(spConfigNode *node,char *name,char deep, spConfigAttribute *found)
{
	spConfigAttribute*ret=NULL;
	ENTRY;

	if(deep)
	{
		ret=(spConfigAttribute*)searchDeep(node,name,(void*)found,0,0);
	}
	else
	{
// PAAK		ret=searchNode(node,name,(void*)found,0,0);
		ret=(spConfigAttribute*)searchNode(node,name,(void*)found,0,1);
	}

	if(ret==(spConfigAttribute*)-1)
		ret=NULL;

	return(ret);
}

