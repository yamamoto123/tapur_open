/* spconfig.h - v0.1.3
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#ifndef SPCONFIG_H
#define SPCONFIG_H

#include <stdio.h>

#define SPURTG_VERSION_H                0
#define SPURTG_VERSION_L                1
#define SPURTG_VERSION_C                2


#define SP_CONFIG_MESSAGE_INFO		0x00
#define SP_CONFIG_MESSAGE_NOTICE	0x01
#define SP_CONFIG_MESSAGE_WARNING	0x02
#define SP_CONFIG_MESSAGE_ERROR		0x03

typedef struct spConfigMessageS
{
	char	type;		/* SP_CONFIG_MESSAGE_* */
	char	*text;
	char	*filename;
	int	line,col;
	struct spConfigMessageS *next; /* linkedlist */
} spConfigMessage;


#define SP_CONFIG_VALUE_NONE	0x00
#define SP_CONFIG_VALUE_INT	0x01
#define SP_CONFIG_VALUE_FLOAT	0x02
#define SP_CONFIG_VALUE_STRING	0x03
#define SP_CONFIG_VALUE_CHILD	0x40
#define SP_CONFIG_VALUE_CREATED 0x80

typedef struct
{
	char	*name;
	char	type;		/* SP_CONFIG_VALUE_* */
	union
	{
		int i;
		double d;
		char *string;
	}	value;
} spConfigAttribute;


#define SP_CONFIG_NODE_ALLOW_TEXT	0x01
#define SP_CONFIG_NODE_ALLOW_ATTRIBUTES	0x02
#define SP_CONFIG_NODE_ALLOW_NODES	0x04
#define SP_CONFIG_NODE_CHILD		0x80

typedef struct spConfigNodeS
{
	char			*name;
	char			confFlag; /* SP_CONFIG_NODE_* */
	char			*text;
	int			numAttributes;
	int			numChildren;
	spConfigAttribute	**attributes;
	struct spConfigNodeS	*parent;
	struct spConfigNodeS	**children;
	spConfigMessage		*message;
	void *(*callback)(struct spConfigNodeS *node,char *string);
} spConfigNode;



spConfigNode		*spConfigNodeNew(void);
spConfigAttribute	*spConfigAttributeNew(void);
spConfigMessage 	*spConfigMessageNew(void);

spConfigNode		*spConfigNodeCopy(spConfigNode *node);
spConfigAttribute	*spConfigAttributeCopy(spConfigAttribute *attribute);
spConfigMessage 	*spConfigMessageCopy(spConfigMessage *msg);

spConfigNode		*spConfigNodeClone(spConfigNode *node);
spConfigAttribute	*spConfigAttributeClone(spConfigAttribute *value);
spConfigMessage 	*spConfigMessageClone(spConfigMessage *msg);

void	spConfigNodeFree(spConfigNode *node);
void	spConfigAttributeFree(spConfigAttribute *attribute);
void	spConfigMessageFree(spConfigMessage *msg);

int	spConfigNodeAddChild(spConfigNode *parent,spConfigNode *child);
int	spConfigNodeAddAttribute(spConfigNode *parent,spConfigAttribute *att);
int	spConfigNodeAddAttributeInt(spConfigNode *parent,char *name,int i);
int	spConfigNodeAddAttributeDouble(spConfigNode *parent,char *name,double d);
int	spConfigNodeAddAttributeString(spConfigNode *parent,char *name,char *text);

int	spConfigNodeName(spConfigNode *node,char *name);
int	spConfigAttributeName(spConfigAttribute *attribute,char *name);

int	spConfigLoad(spConfigNode *root,char *filename);
int	spConfigLoadStr(spConfigNode *root,char *name,char *string);

int	spConfigSave(spConfigNode *root,char *filename);
char	*spConfigSaveStr(spConfigNode *root);

char	*spConfigMessagesList(spConfigNode *rootNode);
void	spConfigMessagesClear(spConfigNode *rootNode);

spConfigNode		*spConfigNodeFind(spConfigNode *root, char *name, 
				char deep, spConfigNode *previous);

spConfigAttribute	*spConfigAttributeFind(spConfigNode *root, char *name,
				char deep, spConfigAttribute *previous);

spConfigNode		*spConfigNodeFindPath(spConfigNode *root,char *path);
spConfigAttribute	*spConfigAttributeFindPath(spConfigNode *root,
				char *path);
char			*spConfigNodeGetPath(spConfigNode *node);

int	spConfigIncludeCallbackSet(spConfigNode *root,void *(callback));
int	spConfigIncludeCallbackRemove(spConfigNode *root);

void	DEBUGFD_spconfig(FILE *output);
void	DEBUGLEVEL_spconfig(signed char l);
void	DEBUGMEMSTAT_spconfig(void);

#endif
