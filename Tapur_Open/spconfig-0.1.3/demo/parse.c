/* parse.c - v0.1.3
 *	* simple spconfig demo app
 *	* parses a file passed on the commandline
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/spconfig.h"

#ifdef WIN32
#define print	for(i=0;i<depth;++i)printf("    ");printf
#else
#define print(x...)	for(i=0;i<depth;++i)printf("   ");printf(x);fflush(stdout);
#endif

void printNode(int depth,spConfigNode*n)
{
	int i,x;
	print("[ %s ]\t( %i %s, %i %s )\n",
	n->name,
	n->numChildren,
	(n->numChildren==1?"child":"children"),
	n->numAttributes,
	(n->numAttributes==1?"attribute":"attributes")
	);
	if(n->text)
	{
		print("   Its text is %i bytes long\n",strlen(n->text));
	}
	if(n->numAttributes)
	{
			print("its attributes are:\n");
		for(x=0;x<n->numAttributes;++x)
		{

//			printf("[%s]\n",n->attribute[i]->name);
			switch(n->attributes[x]->type)
			{
				case SP_CONFIG_VALUE_NONE:
					print("   [%s] -no type-\n",
					n->attributes[x]->name);
					break;
				case SP_CONFIG_VALUE_INT:
					print("   [%s] -integer- [%i]\n",
					n->attributes[x]->name,
					n->attributes[x]->value.i);
					break;
				case SP_CONFIG_VALUE_FLOAT:
					print("   [%s] -float- [%f]\n",
					n->attributes[x]->name,
					n->attributes[x]->value.d);
					break;
				case SP_CONFIG_VALUE_STRING:
					print("   [%s] -string- [%s]\n",
					n->attributes[x]->name,
					n->attributes[x]->value.string);
					break;
			}
		}
	}
	if(n->children)
	{
		print("its children are:\n");
		for(i=0;i<n->numChildren;++i)
			printNode(depth+1,n->children[i]);	
	}
}

int main(int argc,char**argv)
{
	spConfigNode*rootNode;
	char *messages;

	if(argc!=3 && argc!=2)
	{
		printf("usage: %s [config file] <errorlevel>\nerror level is optional",argv[0]);
		return -1;
	}

	printf("Loading config from file %s...\n",argv[1]);

	if(argc==3)
		DEBUGLEVEL_spconfig((signed char)atoi(argv[2]));
	else
		DEBUGLEVEL_spconfig(127);	//minimum debugging

	DEBUGFD_spconfig(stdout);

	rootNode=spConfigNodeNew();

	spConfigLoad(rootNode,argv[1]);

	spConfigNodeName(rootNode,"rootNode");

	printNode(0,rootNode);

	printf("Message log:\n%s\n",messages=spConfigMessagesList(rootNode));
	free(messages);

	spConfigNodeFree(rootNode);

	//printf("memStats:\n");

	DEBUGMEMSTAT_spconfig();

	return 0;
}
