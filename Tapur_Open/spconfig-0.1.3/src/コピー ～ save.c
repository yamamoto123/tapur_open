/* save.c - v0.1.3
 *	* write a memorystructure to a config file.
 *	* save-to-string not yet implemented.
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
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

// Rewrite by PAAK
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
//void node2file(FILE*file,spConfigNode*node,int depth)
//{
//	spConfigAttribute*att;
//	int i;
//	ENTRY;
//	
//	for(i=0;i<depth;++i)
//		fprintf(file,"\t");	/*indent it*/
//	fprintf(file,"<%s ",node->name);/*open tag*/
//	for(i=0;i<node->numAttributes;++i)
//	{
//		att=node->attributes[i];
//		fprintf(file,"%s",att->name);
//		switch(att->type&0xf) /*&0xf is to strip child bit.  kludge*/
//		{
//			case SP_CONFIG_VALUE_NONE:
//				fprintf(file," ");
//				break;
//			case SP_CONFIG_VALUE_INT:
//				fprintf(file,"=%d ",att->value.i);
//				break;
//			case SP_CONFIG_VALUE_FLOAT:
//				fprintf(file,"=%f ",att->value.d);
//				break;
//			case SP_CONFIG_VALUE_STRING:
//				fprintf(file,"=\"%s\" ",att->value.string);
//		}		
//	}			/*values all printed.*/
//	fprintf(file,">\n");	/*close tag*/
//	if(node->text)
//		fprintf(file,"%s",node->text);	/*print text*/
//	for(i=0;i<node->numChildren;++i)		/*print children*/
//		node2file(file,node->children[i],depth+1);
//	fprintf(file,"</>");		/*close tag*/
//	
//	EXIT;
//}

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

/*void node2str(char*buffer,int *size,int *offset,spConfigNode*node,int depth)
{
	spConfigValue*val;
	unsigned int i;
	char*str;

	str=buffer+*offset;

	for(i=0;i<depth;++i)
		ravsnprintf(buffer,size,offset,"\t");
	ravsnprintf(buffer,size,offset,"[%s ",node->name);
	for(i=0;i<node->values;++i)
	{
		val=node->value[i];
		ravsnprintf(buffer,size,offset,"%s",val->name);
		switch(val->type&0x0f)
		{
			case SP_CONFIG_VALUE_NONE:
				ravsnprintf(buffer,size,offset," ");
				break;
			case SP_CONFIG_VALUE_INT:
				ravsnprintf(buffer,size,offset,"=%d ",val->value.i);
				break;
			case SP_CONFIG_VALUE_FLOAT:
				ravsnprintf(buffer,size,offset,"=%f ",val->value.d);
				break;
			case
					 SP_CONFIG_VALUE_STRING:

		}
	}
	ravsnprintf(buffer,size,offset,"]\n");
	if(node->text)
		ravsnprintf(buffer,size,offset,"%s",node->text);
	for(i=0;i<node->children;++i)
		node2str(buffer,size,offset,node->child[i],depth+1);
	ravsnprintf(buffer,size,offset,"[/]");
}*/

typedef struct
{
	char *string;
	int size;
	int offset;
} str;

void putStr(str *string,char c)
{
	if(string->offset+3>=string->size)
	{
		realloc(string->string,string->size*2);
		string->size*=2;
	}
			string->string[string->offset++]=c;

#ifdef _DEBUG
printf("%c", c);
#endif
}

int attr2str(spConfigAttribute *attr,str *string)
{
	int i;
	char buffer[32];
	ENTRY;
	//write name
	for(i=0;i<(int)strlen(attr->name);++i)
	{
		switch(attr->name[i])
		{
			case '"':case '#': case ']':
			case '/':case '<': case '\\':
				putStr(string,'\\');
			default:
				putStr(string,attr->name[i]);
		}
	}
	//write = and value
	if(attr->type&0xf)	//an actual attr, not a child/created attr
	{
		putStr(string,'=');
		
		switch(attr->type&0xf)
		{
			case SP_CONFIG_VALUE_INT:
				snprintf(buffer,32,"%i",attr->value.i);
				for(i=0;i<(int)strlen(buffer);++i)
					putStr(string,buffer[i]);
				break;
			case SP_CONFIG_VALUE_FLOAT:
				snprintf(buffer,32,"%f",attr->value.d);
				for(i=0;i<(int)strlen(buffer);++i)
					putStr(string,buffer[i]);
				break;
			case SP_CONFIG_VALUE_STRING:
				putStr(string,'"');
				for(i=0;i<(int)strlen(attr->value.string);++i)
				{
					switch(attr->value.string[i])
					{
						case '"':case '[': case '\\':
						case '#':case '/': case '>':
							putStr(string,'\\');
						default:
							putStr(string,buffer[i]);				
					}
				}
				putStr(string,'"');
				break;
		}
	}
	
	return(0);
}

int text2str(char*text,str*string)
{
	int i;
	ENTRY;
	for(i=0;i<(int)strlen(text);++i)
	{
		switch(text[i])
		{
			case '<':case '[': case '#':
			case '/':case '\\':
				putStr(string,'\\');
			default:
				putStr(string,text[i]);
		}
	}
	return(0);
}

int node2str(spConfigNode*node,str*string)
{
	int i;
	ENTRY;
	
	if(node->text)
		text2str(node->text,string);
	putStr(string,'<');
	for(i=0;i<(int)strlen(node->name);++i)
	{
		switch(node->name[i])
		{
			case '[':case '>': case '\\':
			case '"':case '/':
				putStr(string,'\\');
			default:
				putStr(string,node->name[i]);
		}
	}
	putStr(string,' ');
	for(i=0;i<node->numAttributes;++i)
	{
		attr2str(node->attributes[i],string);
	}
	if(node->numChildren)
	{
		putStr(string,'>');
		
		for(i=0;i<node->numChildren;++i)
		{
			node2str(node->children[i],string);
		}
		
		putStr(string,'<');
		putStr(string,'/');
		for(i=0;i<(int)strlen(node->name);++i)
		{
			switch(node->name[i])
			{
				case '[':case '>': case '\\':
				case '"':case '/':
					putStr(string,'\\');
				default:
					putStr(string,node->name[i]);
			}
		}		
		putStr(string,'>');
	}
	else
	{
		putStr(string,'/');
		putStr(string,'>');
	}
	
	return(0);
}

/* starts with an arbitrary 256 byte buffer.  as more junk is added, it
 * is realloc'd to be 2x larger
 * buffer is whole thing, location is current position offset
 */
char *spConfigSaveStr(spConfigNode*root)
{
	str *string;
	char *saved;
	ENTRY;
	
	string=(str*)malloc(sizeof(str));
	string->size=256;
	string->offset=0;
	string->string=(char*)malloc(sizeof(char)*256);		// •s‘«‚µ‚½‚ç“à•”‚Årealloc‚³‚ê‚é
	
	node2str(root,string);
	
	saved=string->string;
	free(string);
	
	return(saved);
	
/*	buffer=(char*)malloc(256);
	if(!buffer)
	{
		return NULL;
	}	

	if(root->text)
		ravsnprintf(buffer,&bufsize,&offset,"%s",root->text);
	if(root->values)
	{
		//warning, no vals in root
	}
	for(i=0;i<root->children;++i)
	{
		node2str(buffer,&bufsize,&offset,root->child[i],0);
	}
	return buffer;*/
}
