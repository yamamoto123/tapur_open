/* message.c - v0.1.3
 *	* message handling routines
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <string.h>
#include <math.h>
#include <malloc.h>
#include "spconfig.h"
#include "debug.h"

int intLen(int i)
{
	ENTRY
	DEBUG(-64,"input [%d] output [%d]\n",i,1+(int)(log((float)i)/log(10.0)) );
	return(1+(int)(log((float)i)/log(10.0)));
}

char	*spConfigMessagesList(spConfigNode*rootNode)
{
	char*text;
	int pos=0;
	int size=1;
	spConfigMessage*msg;
	ENTRY

	if(!rootNode || !rootNode->message)
	{
		return(0);
	}

	//if(!(text=(char*)malloc(8)))
	//{
	//	DEBUG(3,"Malloc Failed\n");
	//	return(0);
	//}
	msg=rootNode->message;

	//[Type] Filename: line L,column c: [text]
	
	while(msg)
	{
		size+=intLen(msg->line);
		size+=intLen(msg->col);
		size+=strlen(msg->filename);
		size+=strlen(msg->text);
		switch(msg->type)
		{
			case SP_CONFIG_MESSAGE_INFO:
				size+=strlen("INFO : line , column : \n");
				break;
			case SP_CONFIG_MESSAGE_NOTICE:
				size+=strlen("NOTICE : line , column : \n");
                                break;
			case SP_CONFIG_MESSAGE_WARNING:
				size+=strlen("WARNING : line , column : \n");
                                break;
			case SP_CONFIG_MESSAGE_ERROR:
				size+=strlen("ERROR : line , column : \n");
                                break;
			default:
				size+=strlen(" : line , column : \n");
                                break;
		}	
		msg=msg->next;
	};
	DEBUG(0,"Full Message Length [%d]\n",size);
	text=(char*)malloc(size);
	msg=rootNode->message;

	while(msg)
	{
		switch(msg->type)
		{
			case SP_CONFIG_MESSAGE_INFO:
				pos+=sprintf(text+pos,"INFO ");break;
			case SP_CONFIG_MESSAGE_NOTICE:
				pos+=sprintf(text+pos,"NOTICE ");break;
			case SP_CONFIG_MESSAGE_WARNING:
				pos+=sprintf(text+pos,"WARNING ");break;
			case SP_CONFIG_MESSAGE_ERROR:
				pos+=sprintf(text+pos,"ERROR ");break;
		}
		pos+=sprintf(text+pos,"%s: line %d, column %d: %s\n",msg->filename,msg->line,msg->col,msg->text);
		msg=msg->next;
	}

	return(text);
}

void	spConfigMessagesClear(spConfigNode*rootNode)
{
	ENTRY

	spConfigMessageFree(rootNode->message);

	EXIT
}
