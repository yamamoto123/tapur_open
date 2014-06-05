/* test.c - v0.1.3
 *	* simple spconfig test app
 *	* constructs a memorystructure
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include "spconfig.h"

int main()
{
	spConfigNode*n,*child,*child2,*child3,*child3b,*tmp;
	spConfigAttribute*v;
	spConfigMessage*m;

	DEBUGLEVEL_spconfig(-128);


	n=spConfigNodeNew();
	child=spConfigNodeNew();
	child2=spConfigNodeNew();
	child3=spConfigNodeNew();
	child3b=spConfigNodeNew();

	spConfigNodeName(n,"TestNode Name");
	spConfigNodeName(child,"Child Node");
	spConfigNodeName(child2,"Child2 Node");
	spConfigNodeName(child3,"Child3 Node");
	spConfigNodeName(child3b,"Child3b Node");

	spConfigNodeAddChild(n,child);

	spConfigNodeAddChild(child,child2);

	spConfigNodeAddChild(child2,child3);
	spConfigNodeAddChild(child2,child3b);

	v=spConfigAttributeNew();

	spConfigAttributeName(v,"Test Value Name");

	m=spConfigMessageNew();

	printf("Testing get path...\n");

	printf("Path to node is: [%s]\n",spConfigNodeGetPath(child3));

	printf("testing FindPath on bad path\n");
	tmp=spConfigNodeFindPath(n,"/1234567812345678/Child2 Node/");
	printf("node found is [0x%X], looking for [none]\n",(int)tmp);

	printf("testing FindPath on a valid path\n");
	tmp=spConfigNodeFindPath(n,"/Child Node/Child2 Node/Child3 Node/");
	printf("node found is [0x%X], looking for [0x%X]\n",(int)tmp,(int)child3);

	printf("testing find functions...\n");
	tmp=spConfigNodeFind(n,"Child3b Node",0,NULL);
	printf("found [0x%x], looking for [none]\n",(int)tmp);
	tmp=spConfigNodeFind(n,"Child3b Node",1,NULL);
	printf("found [0x%x], looking for [0x%x]\n",(int)tmp,(int)child3b);
	tmp=spConfigNodeFind(n,"Child3b Node",1,tmp);
	printf("found [0x%x], looking for [none]\n",(int)tmp);

	spConfigNodeFree(n);
	spConfigAttributeFree(v);
	spConfigMessageFree(m);
	return 0;
}
