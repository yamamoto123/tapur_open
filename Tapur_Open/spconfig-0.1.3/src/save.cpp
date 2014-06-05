/* save.c - v0.1.3
 *	* write a memorystructure to a config file.
 *	* save-to-string not yet implemented.
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 *
 * Rewrite almost by PAAK
 */

#include <stdio.h>
#include "spconfig.h"
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include "spconfig.h"
#include "debug.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

//FILE *fp_debug;

void node2file(FILE*file,spConfigNode*node,int depth)
{
	spConfigAttribute*att;
	int i;
	ENTRY;

	for(i=0;i<depth;++i)
		fprintf(file,"\t");	/*indent it*/
	fprintf(file,"<%s",node->name);/*open tag*/
	for(i=0;i<node->numAttributes;++i)
	{
		att=node->attributes[i];
		fprintf(file," %s",att->name);
		switch(att->type&0xf) /*&0xf is to strip child bit.  kludge*/
		{
			case SP_CONFIG_VALUE_NONE:
				break;
			case SP_CONFIG_VALUE_INT:
				fprintf(file,"=%d",att->value.i);
				break;
			case SP_CONFIG_VALUE_FLOAT:
				fprintf(file,"=%f",att->value.d);
				break;
			case SP_CONFIG_VALUE_STRING:
				fprintf(file,"=\"%s\"",att->value.string);
		}
	}			/*values all printed.*/
	fprintf(file,">");	/*close tag*/
	if(node->text)
		fprintf(file,"%s",node->text);	/*print text*/
	if(node->numChildren > 0) {
		fprintf(file,"\n");
		for(i=0;i<node->numChildren;++i)		/*print children*/
			node2file(file,node->children[i],depth+1);
		for(i=0;i<depth;++i)
			fprintf(file,"\t");	/*indent it*/
	}
	fprintf(file,"</%s>\n",node->name);		/*close tag*/

	EXIT;
}

int spConfigSave(spConfigNode*root,char*filename)
{
	int i;
	FILE*file;
	ENTRY;
#ifdef WIN32
	file=fopen(filename,"w+");	//paak
#else
	file=fopen(filename,"rw");
#endif
	if(!file)
	{
		return -1;
	}

	if(root->text)
		fprintf(file,"%s",root->text);
	if(root->numAttributes)
	{
		//warn, no vals in root node.
	}

	for(i=0;i<root->numChildren;++i)
	{
		node2file(file,root->children[i],0);
	}

	fclose(file);	// paak

	return(0);
}


typedef struct
{
	char *string;
	int size;
	int offset;
} str;

void putChr(str *string, char c)
{
	if(string->offset+3>=string->size)
	{
		// reallocはバッファの位置を変更する場合があるため、必ずポインタを設定しなおす
		string->string = (char *)realloc(string->string,string->size*2);
//		realloc(string->string,string->size*2);
		string->size*=2;
	}
	string->string[string->offset++]=c;
}

void putStr(str *string, char *str)
{
	int i;

	for (i = 0; i < (int)strlen(str); i++) {
		putChr(string, *(str+i));
	}
	string->string[string->offset] = '\0';

//fputs(string->string, fp_debug);
//fputs("\n", fp_debug);
}

void node2str(str *string, spConfigNode *node, int depth)
{
	spConfigAttribute*att;
	int i;
	char buf[1024];
	ENTRY;

	for(i=0;i<depth;++i) {
		putStr(string,"\t");	/*indent it*/
	}
	sprintf(buf,"<%s",node->name);/*open tag*/
	putStr(string,buf);
	for(i=0;i<node->numAttributes;++i)
	{
		att=node->attributes[i];
		sprintf(buf," %s",att->name);
		putStr(string, buf);
		switch(att->type&0xf) /*&0xf is to strip child bit.  kludge*/
		{
			case SP_CONFIG_VALUE_NONE:
				break;
			case SP_CONFIG_VALUE_INT:
				sprintf(buf,"=%d",att->value.i);
				putStr(string, buf);
				break;
			case SP_CONFIG_VALUE_FLOAT:
				sprintf(buf,"=%f",att->value.d);
				putStr(string, buf);
				break;
			case SP_CONFIG_VALUE_STRING:
				sprintf(buf,"=\"%s\"",att->value.string);
				putStr(string, buf);
				break;
		}
	}			/*values all printed.*/
	putStr(string,">");	/*close tag*/
	if(node->text) {
		putStr(string, node->text);	/*print text*/
	}
	if(node->numChildren > 0) {
		putStr(string,"\n");
		for(i=0;i<node->numChildren;++i) 		/*print children*/
			node2str(string,node->children[i],depth+1);
		for(i=0;i<depth;++i)
			putStr(string,"\t");	/*indent it*/
	}
	sprintf(buf,"</%s>\n",node->name);		/*close tag*/
	putStr(string, buf);

	EXIT;
}

/* starts with an arbitrary 256 byte buffer.  as more junk is added, it
 * is realloc'd to be 2x larger
 * buffer is whole thing, location is current position offset
 */
char *spConfigSaveStr(spConfigNode*root)
{
	str *string;
	char *saved;
	int i;
	ENTRY;

//fp_debug = fopen("C:\\test.log", "w");
	
	string=(str*)calloc(sizeof(str), 1);
	string->size=256;
	string->offset=0;
	string->string=(char*)calloc(sizeof(char), 256);		// 不足したら内部でreallocされる
	
	if(root->text)
		putStr(string,root->text);
	if(root->numAttributes)
	{
		//warn, no vals in root node.
	}

	for(i=0;i<root->numChildren;++i)
	{
		node2str(string, root->children[i], 0);
	}

//fclose(fp_debug);
	
	saved=string->string;
	free(string);
	
	return(saved);
}
