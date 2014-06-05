/* spconfig.h - v0.1.3
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#ifndef SPCONFIG_H
#define SPCONFIG_H

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


extern spConfigNode		*spConfigNodeNew(void);
extern spConfigAttribute	*spConfigAttributeNew(void);
extern spConfigMessage 	*spConfigMessageNew(void);

extern spConfigNode		*spConfigNodeCopy(spConfigNode *node);
extern spConfigAttribute	*spConfigAttributeCopy(spConfigAttribute *attribute);
extern spConfigMessage 	*spConfigMessageCopy(spConfigMessage *msg);

extern spConfigNode		*spConfigNodeClone(spConfigNode *node);
extern spConfigAttribute	*spConfigAttributeClone(spConfigAttribute *value);
extern spConfigMessage 	*spConfigMessageClone(spConfigMessage *msg);

extern void	spConfigNodeFree(spConfigNode *node);
extern void	spConfigAttributeFree(spConfigAttribute *attribute);
extern void	spConfigMessageFree(spConfigMessage *msg);

extern int	spConfigNodeAddChild(spConfigNode *parent,spConfigNode *child);
extern int	spConfigNodeAddAttribute(spConfigNode *parent,spConfigAttribute *att);
extern int	spConfigNodeAddAttributeInt(spConfigNode *parent,char *name,int i);
extern int	spConfigNodeAddAttributeDouble(spConfigNode *parent,char *name,double d);
extern int	spConfigNodeAddAttributeString(spConfigNode *parent,char *name,char *text);

extern int	spConfigNodeName(spConfigNode *node,char *name);
extern int	spConfigAttributeName(spConfigAttribute *attribute,char *name);

extern int	spConfigLoad(spConfigNode *root,char *filename);
extern int	spConfigLoadStr(spConfigNode *root,char *name,char *string);

extern int	spConfigSave(spConfigNode *root,char *filename);
extern char	*spConfigSaveStr(spConfigNode *root);

extern char	*spConfigMessagesList(spConfigNode *rootNode);
extern void	spConfigMessagesClear(spConfigNode *rootNode);

extern spConfigNode		*spConfigNodeFind(spConfigNode *root, char *name, 
				char deep, spConfigNode *previous);

extern spConfigAttribute	*spConfigAttributeFind(spConfigNode *root, char *name,
				char deep, spConfigAttribute *previous);

extern spConfigNode		*spConfigNodeFindPath(spConfigNode *root,char *path);
extern spConfigAttribute	*spConfigAttributeFindPath(spConfigNode *root,
				char *path);
extern char			*spConfigNodeGetPath(spConfigNode *node);

extern int	spConfigIncludeCallbackSet(spConfigNode *root,void *(callback));
extern int	spConfigIncludeCallbackRemove(spConfigNode *root);

extern void	DEBUGFD_spconfig(FILE *output);
extern void	DEBUGLEVEL_spconfig(signed char l);
extern void	DEBUGMEMSTAT_spconfig(void);

#endif
