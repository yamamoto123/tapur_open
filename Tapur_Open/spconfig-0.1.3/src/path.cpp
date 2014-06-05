/* path.c - v0.1.3
 *	* converting a textual "path" into a node pointer
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <string.h>
#include <malloc.h>
#include "spconfig.h"
#include "stack.h"

#include "debug.h"

char* ravsnprintf(char*buffer,int *size,int *offset,char*fmt,...);

spConfigNode	*spConfigNodeFindPath(spConfigNode*root,char*path)
{
	spConfigNode	*found,*oldFound;
	int size;	//length of path string
	int start,end;	//position within path string
	int i;
	ENTRY

	if(!root || !path)
	{
		return(0);
	}

	if(path[0]!='/')
	{
		start=0;
		//please start path's with a root '/' :^)
	}
	else
		start=1;
	end=1;
	size=strlen(path);
	found=root;

	while(1)
	{
		//we start at start position, and scan for a '/'.  set end to that location.
		//compare child node names with the string between start and end
		//if not found, return 0, else set found to that child, set end to start, and start over

		while(end<size && path[end]!='/')
			++end;

		DEBUG(-2,"start [%d] end [%d]\n",start,end);

		oldFound=found;

		for(i=0;i<found->numChildren;++i)
		{
			if(!strncmp(found->children[i]->name,path+start,end-start))
			{
				DEBUG(-2,"Match found\n");
				found=found->children[i];
			}
		}
		if(oldFound==found)	//didnt find any matches
		{
			DEBUG(0,"No Matches Found\n");
			return(0);
		}
		++end;	//skip the '/'
		start=end;	//set start to last end place
		if(end==size)	//end of the string?
		{
			return(found);
		}
	}

	return(0);
}

spConfigAttribute   *spConfigAttributeFindPath(spConfigNode*root,char*path)
{
//        spConfigAttribute*found=0;
//        char *genstring;
        int i=0,j;
        
        ENTRY;
        //skip preceding /, if any
        if(path[0]=='/')
                ++path;
        //scan to a '/'
        while(path[i] && path[i]!='/')
        {
                ++i;
        }
        if(path[i]==0)  //end of string, this must be the value name
        {
                for(j=0;j<root->numAttributes;++j)
                        if(!strcmp(root->attributes[j]->name,path))
                                return(root->attributes[j]);
        }
        else    //got a node name.  let's see if we can find it.
        {
                for(j=0;j<root->numChildren;++j)
                        if(!strncmp(root->children[j]->name,path,i))
                                return( spConfigAttributeFindPath(root->children[j],path+i) );
                        
        }
        return(NULL);
}


char            *spConfigNodeGetPath(spConfigNode*node)
{
	char*path,*str;
	int size=2,pos=1;
	spConfigNode	*n;
	Stack*nameList;
	Stack*sizeList;
	ENTRY
	if(!node)
		return(0);

	nameList=newStack();
	sizeList=newStack();

	DEBUG(-2,"Pushing Name [%s]\n",node->name);
	stackPush(nameList,(void*)node->name);
	stackPush(sizeList,(void*)node->name);
	//path=(char*)malloc(2);

	//path[0]='/';	//root directory

	if(node->parent)
	{
		n=node->parent;
		while(n->parent)
		{
			DEBUG(-2,"Pushing Name [%s]\n",n->name);
			stackPush(nameList,(void*)n->name);
			stackPush(sizeList,(void*)n->name);
			n=n->parent;
		}
		
	}

	str=(char*)stackPop(sizeList);
	while(str && str!=(char*)-1)
	{
		size+=strlen(str)+1;
		//snprintf();
		//DEBUG(-2,"ravprinting [%s]\n",str);
		//path=ravsnprintf(path,&size,&pos,"%s/",str);
		//DEBUG(-2,"position [%d]\n",pos);
		str=(char*)stackPop(sizeList);
	}

	DEBUG(-1,"Path Size: %d\n",size);
	path=(char*)malloc(size);
	path[0]='/';

	str=(char*)stackPop(nameList);
	while(str && str!=(char*)-1)
	{
		DEBUG(-4,"adding [%s] to path [%i]\n",str,pos);
		pos+=sprintf(path+pos,"%s/",str);
		str=(char*)stackPop(nameList);
		DEBUG(-5,"new str [%x]\n",(int)str);
		//DEBUG(-8,"[%s]\n",str);
	}

	DEBUG(-4,"cleaning up\n");	

	freeStack(sizeList);
	freeStack(nameList);

	return(path);
}
