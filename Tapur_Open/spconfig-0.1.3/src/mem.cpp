/* mem.c - v0.1.3
 *	* internal memory-related routines
 *	* node/attribute/message management routines
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


/* a kludge around some realloc problems.  not currently used, i hope =) */
void*	SPrealloc(void*location,int newsize,int oldsize)
{
	void*chunk;
	ENTRY

	DEBUG(-16,"pre-malloc [%d] [%d]\n",oldsize,newsize);
	chunk=malloc(newsize);
	
	DEBUG(-16,"post-malloc\n");
	if(!chunk)
	{
		DEBUG(3,"Malloc failed\n");
		return(0);
	}
	DEBUG(-6,"memcpy [%x] [%x] [%d]\n",(int)chunk,(int)location,oldsize);
	memcpy(chunk,location,oldsize);
	DEBUG(-6,"freeing\n");
	free(location);
	return(chunk);
}

spConfigNode	*spConfigNodeNew()
{
	spConfigNode *n;
	ENTRY
	n=(spConfigNode*)calloc(sizeof(spConfigNode),1);
	DEBUG(-2,"   * ptr: [0x%x]\n",(int)n);

	n->confFlag=0x07;	//allow text, values, and nodes

	return (n);
}

spConfigAttribute	*spConfigAttributeNew()
{
	spConfigAttribute	*v;
	ENTRY
	v=(spConfigAttribute*)calloc(sizeof(spConfigAttribute),1);
	DEBUG(-2,"   * ptr: [0x%x]\n",(int)v);

	v->type=0;	//no type yet

	return(v);
}


spConfigMessage	*spConfigMessageNew()
{
	spConfigMessage	*m;
	ENTRY
	m=(spConfigMessage*)calloc(sizeof(spConfigMessage),1);
	DEBUG(-2,"   * ptr: [0x%x]\n",(int)m);
	return(m);
}

/* copy copies pointers (same instance) */
spConfigNode	*spConfigNodeCopy(spConfigNode*node)
{
	spConfigNode*n;
	ENTRY
	n=spConfigNodeNew();
	if(!n)
	{
		DEBUG(3,"Node New Failed\n");
		return(0);
	}
	memcpy(n,node,sizeof(spConfigNode));
	return (n);
}

spConfigAttribute	*spConfigAttributeCopy(spConfigAttribute*att)
{
	spConfigAttribute*v;
	ENTRY
	v=spConfigAttributeNew();
	if(!v)
	{
		DEBUG(3,"Value New Failed\n");
		return(0);
	}
	memcpy(v,att,sizeof(spConfigAttribute));
	return(v);
}

spConfigMessage	*spConfigMessageCopy(spConfigMessage*msg)
{
	spConfigMessage*m;
	ENTRY
	m=spConfigMessageNew();
	if(!m)
	{
		DEBUG(3,"Message New Failed\n");
		return(0);
	}
	memcpy(m,msg,sizeof(spConfigMessage));	
	return (m);
}

/* clone allocates more space and copies pointer contents (new instance) */
/* this makes it recursive (clones the clone's nodes/values) 
   so a tree that points to itself will eventually crash (infinite recursion) */
spConfigNode	*spConfigNodeClone(spConfigNode*node)
{
	int i;
	spConfigNode *n;
	ENTRY
	n=spConfigNodeNew();
	if(!n)
	{
		DEBUG(3,"Node New Failed\n");
		return(0);
	}
	memcpy(n,node,sizeof(spConfigNode));

	if(node->name)
		if(!(n->name=strdup(node->name)))
		{
			DEBUG(3,"strdup Failed\n");
			free(n);
			return(0);
		}
	if(node->text)
		if(!(n->text=strdup(node->text)))
		{
			DEBUG(3,"strdup Failed\n");
			free(n->name);
			free(n);
			return(0);
		}

	n->attributes=(spConfigAttribute**)malloc(sizeof(spConfigAttribute*)*node->numAttributes);
	n->children=(spConfigNode**) malloc(sizeof(spConfigNode*) *node->numChildren);



	for(i=0;i<node->numAttributes;++i)
		if(!(n->attributes[i]=spConfigAttributeClone(node->attributes[i])))
		{
			DEBUG(3,"spConfigAttributeClone failed\n");
			for(i-=1;i>=0;--i)
				spConfigAttributeFree(n->attributes[i]);
			free(n->children);
			free(n->attributes);
			free(n->text);
			free(n->name);
			free(n);
			return(0);
		}
	for(i=0;i<node->numChildren;++i)
		if(!(n->children[i]=spConfigNodeClone(node->children[i])))
		{
			DEBUG(3,"spConfigNodeClone failed\n");
			for(i-=1;i>=0;--i)
				spConfigNodeFree(n->children[i]);
			free(n->children);
			for(i=0;i<n->numAttributes;++i)
				spConfigAttributeFree(n->attributes[i]);
			free(n->attributes);
			free(n->text);
			free(n->name);
			free(n);
			return(0);
		}

	if(node->message)
		if(!(n->message=spConfigMessageClone(node->message)))
		{
			DEBUG(3,"spConfigMessageClone failed\n");
			for(i=0;i<n->numAttributes;++i)
				spConfigAttributeFree(n->attributes[i]);
			free(n->attributes);
			for(i=0;i<n->numChildren;++i)
				spConfigNodeFree(n->children[i]);
			free(n->children);
			free(n->text);
			free(n->name);
			free(n);
			return(0);
		}

	n->parent=NULL;		//how to properly handle this?
	
	return (n);
}

spConfigAttribute	*spConfigAttributeClone(spConfigAttribute*att)
{
	spConfigAttribute*v;
	ENTRY
	v=spConfigAttributeNew();

	if(!v)
	{
		DEBUG(3,"Malloc Failed\n");
		return (0);
	}

	memcpy(v,att,sizeof(spConfigAttribute));

	if(att->name)
		v->name=strdup(att->name);
	if(att->type==SP_CONFIG_VALUE_STRING)
		v->value.string=strdup(att->value.string);
	return(v);
}

spConfigMessage	*spConfigMessageClone(spConfigMessage*msg)
{
	spConfigMessage*m;
	ENTRY

	m=spConfigMessageNew();

	if(!m)
	{
		DEBUG(3,"Malloc Failed\n");
		return(0);
	}

	memcpy(m,msg,sizeof(spConfigMessage));

	if(msg->text)
		m->text=strdup(msg->text);
	if(msg->filename)
		m->filename=strdup(msg->filename);
	if(msg->next)
		m->next=spConfigMessageClone(msg->next);

	return(m);
}

void	spConfigNodeFree(spConfigNode*node)
{
	int i;
	ENTRY;
	if(!node)
	{
		EXIT
		return;
	}
	if(node->name)
		free(node->name);
	if(node->text)
		free(node->text);
	for(i=0;i<node->numAttributes;++i)
		spConfigAttributeFree(node->attributes[i]);
	if(node->attributes)
		free(node->attributes);
	for(i=0;i<node->numChildren;++i)
		spConfigNodeFree(node->children[i]);
	if(node->children)
		free(node->children);
	if(node->message)
		spConfigMessageFree(node->message);
	free(node);
	EXIT
}

void	spConfigAttributeFree(spConfigAttribute*att)
{
	ENTRY
	if(!att)
	{
		EXIT
		return;
	}
	if(att->type==SP_CONFIG_VALUE_STRING)
		free(att->value.string);
	if(att->name)
		free(att->name);
	free(att);
	EXIT
}

void	spConfigMessageFree(spConfigMessage*msg)
{
	ENTRY
	if(!msg)
	{
		EXIT
		return;
	}
	if(msg->text)
		free(msg->text);
	if(msg->filename)
		free(msg->filename);
	if(msg->next)
		spConfigMessageFree(msg->next);
	free(msg);
	EXIT
}

int     spConfigNodeAddChild(spConfigNode*parent,spConfigNode*child)
{
	spConfigNode	**nodelist;
	ENTRY
	if(!parent || !child)
		return(-1);

	nodelist=(spConfigNode**)malloc(sizeof(spConfigNode*)*(parent->numChildren+1));
	if(!nodelist)
		return(-2);
	if(parent->children)
	{
		memcpy(nodelist,parent->children,sizeof(spConfigNode*)*parent->numChildren);
		free(parent->children);		//free parents list (its wrong)
	}
	parent->children=nodelist;		//replace with new list (new child appended)
	nodelist[parent->numChildren]=child;
	++parent->numChildren;		//update parent's child count
	if(child->parent)
	{
		//child is already in a tree somewhere, this will cause confusion
		child->parent=parent;
		return (1);
	}
	child->parent=parent;
	return (0);
}

int     spConfigNodeAddAttribute(spConfigNode*parent,spConfigAttribute*att)
{
	spConfigAttribute**attList;
	ENTRY
	if(!parent ||!att)
	{
		return(-1);
	}
	attList=(spConfigAttribute**)malloc(sizeof(spConfigAttribute*)*(parent->numAttributes+1));
	if(parent->attributes)
		memcpy(attList,parent->attributes,sizeof(spConfigAttribute*)*parent->numAttributes);
	attList[parent->numAttributes]=att;
	DEBUG(-65,"Adding attribute [%x] to parent [%x]\n",(int)att,(int)parent);
	++parent->numAttributes;
	if(parent->attributes)		//does parent already have some values?
		free(parent->attributes);
	parent->attributes=attList;

	return(0);
}

int     spConfigNodeAddAttributeInt(spConfigNode*parent,char*name,int i)
{
	spConfigAttribute*v;
	ENTRY

	if(!parent)
		return(-1);

	v=spConfigAttributeNew();
	spConfigAttributeName(v,name);
	v->type=SP_CONFIG_VALUE_INT;
	v->value.i=i;

	spConfigNodeAddAttribute(parent,v);

	return(0);
}

int     spConfigNodeAddAttributeDouble(spConfigNode*parent,char*name,double d)
{
	spConfigAttribute*v;
	ENTRY

	if(!parent)
		return(-1);

	v=spConfigAttributeNew();
	spConfigAttributeName(v,name);
	v->type=SP_CONFIG_VALUE_FLOAT;
	v->value.d=d;

	spConfigNodeAddAttribute(parent,v);

	return(0);
}

int     spConfigNodeAddAttributeString(spConfigNode*parent,char*name,char*text)
{
	spConfigAttribute*v;
	ENTRY
	if(!parent)
		return(-1);

	v=spConfigAttributeNew();
	spConfigAttributeName(v,name);
	v->type=SP_CONFIG_VALUE_STRING;
	v->value.string=strdup(text);

	spConfigNodeAddAttribute(parent,v);

	return(0);
}

int     spConfigNodeName(spConfigNode*node,char*name)
{
	ENTRY
	if(!node || !name)
		return (-1);
	if(node->name)	// eek, lost the name pointer
	{
		node->name=strdup(name);
		return (1);
	}
	node->name=strdup(name);
	return (0);
}

int     spConfigAttributeName(spConfigAttribute*att,char*name)
{
	ENTRY
	if(!att || !name)
		return (-1);
	if(att->name)		//lost name pointer
	{
		att->name=strdup(name);
		return (1);
	}
	att->name=strdup(name);
	return (0);
}
