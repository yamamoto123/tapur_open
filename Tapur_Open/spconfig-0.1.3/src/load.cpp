/* load.c - v0.1.3
 *	* parse a config file into a memorystructure
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

#include <stdlib.h> //for atoi/atof
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "spconfig.h"
#include "debug.h"
#include "stack.h"

/*
	Theory of operation:
	we have a state, which contains the parse type (text, node, etc),
	and the constructing string (string we are loading).
	each item in the state is stackable.  each time the current
	state parser decides we need to change states, we push the state
	variables, and start anew.  when the state parser is completed, 
	we perform any necessary clean up on the data, and pop the
	state variables.

	this implementation probably leaks memory, but might not.
*/

/*************************************************/
/* a small string sublib to navagate the strings */
/*************************************************/

typedef struct
{
	char*name;	//filename/stringname
	char*str;
	int len;
	int pos;
	int row,col;
} STString;

STString*	newSTString(int len)
{
	STString*s;
	ENTRY;
	DEBUG(-127,"New string len [%i]\n",len);
	s=(STString*)malloc(sizeof(STString));
	DEBUG(-80,"string ptr [%x]\n",(int)s);
	s->str=(char*)malloc(len+1);
	DEBUG(-80,"char array [%x]\n",(int)s->str);
	DEBUG(-90,"filling in values\n");
	s->name=NULL;
	s->len=len;
    s->str[0]=0;
	s->pos=0;
	s->row=0;
	s->col=0;


	return(s);
}

//new string from a string
STString* newSTStringS(char *string)
{
	STString *s;
	ENTRY

	s=(STString*)malloc(sizeof(STString));
	s->str=string;
	s->name=NULL;
	s->len=strlen(string);
	s->pos=0;
	s->row=0;
	s->col=0;

	return(s);
}

void	freeSTString(STString*s)
{
	ENTRY;
	DEBUG(-124,"STString ptr [%x]\n",(int)s);
	if(s)
	{
		if(s->name)
			free(s->name);
		free(s->str);
		free(s);
	}
	EXIT;
}

char getChar(STString*s)
{
	ENTRY;
	if(s->pos==s->len)
		return(-1);	//end of string
	++s->pos;

	if(s->str[s->pos-1]=='\n')
	{
		s->row++;
		s->col=1;
	}
	else
		++s->col;
	DEBUG(-64,"[%c]\n",s->str[s->pos-1]);
	return(s->str[s->pos-1]);
}

void	addChar(STString*s,char c)
{
	ENTRY;

	if(!s)
	{
		EXIT;
		return;
	}	
	if(s->pos+1>=s->len)
	{
		DEBUG(-4,"Expanding string [%x][%i][%i]\n",(int)s->str,s->pos,s->len);
		s->str=(char*)realloc(s->str,s->len*2);
		DEBUG(-10,"realloc passed...\n");
		s->len*=2;
		DEBUG(-4,"STString Expanded [%x][%i][%i]\n",(int)s->str,s->pos,s->len);
	}
	s->str[s->pos++]=c;
	s->str[s->pos]=0;	//null terminate

	EXIT;
}

char peekChar(STString*s)
{
	ENTRY;
	if(s->pos==s->len)
		return(-1);

	return(s->str[s->pos]);
}

void ungetChar(STString*s)
{
	ENTRY;
	if(s->pos)
		--s->pos;
	if(s->str[s->pos]=='\n')
	{
		s->row--;
		s->col=-1;	//invalid, cuz we don't know =)
	}

	EXIT;
}

/***********************/
/* the state variables */
/***********************/

//these things need to be maintained by spConfigParse* functions, 
// and passed around instead of being global.  this opens up thread-safe spconfig
// and will prolly fix a few bugs that have yet to be reported.

typedef struct DefineS
{
	char *name;
	char *value;
	struct DefineS *next;
} Define;

typedef enum
{
	Done,
	Text,
	LineComment,
	BlockComment,
	Node,
	Attribute,
	Quote,
	System
} States;

typedef enum
{
	Include,
	Info,
	Notice,
	Warning,
	Error,
	Def,
	Undef,
	Ifdef,
	Ifndef,
	Else,
	Endif,
	Ifeq,
	Ifneq
} SystemDirective;
	
#define STATE_FLAGS_ESCAPED	1	// when we get a foreward slash
#define STATE_FLAGS_QUOTES	2	// when we're in a quoted string
typedef struct
{
	char		flags;		//various flags
	unsigned int	defDepth;	//ifdef/endif level
	unsigned int	defActive;	//depth at which we become active
	States		state;		//what state are we in?
	STString		*string;	//what have we got so far?
	spConfigNode	*node;		//what node are we working on?
	spConfigAttribute	*attribute;		//attribute we're working on
	void		*info;		// private parser info
	STString		*input;		//for message use only, parsers shouldn't touch!

	spConfigNode	*root;		//our root node

	//stack, for state push/pops
	Stack	*inputS;	//input stack (includes)	
	Stack	*stateS;	//state stack
	Stack	*stringS;	//string stack
	Stack	*nodeS;		//node stack
	Stack	*infoS;		//info stack

	//defines stuff	
	Define	*defines;	//start of our defines list
}State;

State state;	//quasi-thread safe;

/**********************/
/* the state handlers */
/**********************/

/*message wrappers*/
void addMessage(int type, char *text)
{
	spConfigMessage*msg,*walk;
	ENTRY;

	msg=spConfigMessageNew();
	msg->line=state.input->row;
	msg->col=state.input->col;
	msg->type=type;
	msg->text=strdup(text);	//don't free the text!  this is internal anyway
	msg->filename=strdup(state.input->name);
	msg->next=NULL;

	DEBUG(-120,"[%i][%i][%s][%s]\n",msg->line,msg->col,msg->filename,msg->text);
		
	walk=state.root->message;
	if(!walk)	//first post! ;^)
	{
		state.node->message=msg;
		DEBUG(-128,"first message\n");
		EXIT;
		return;
	}
	// walk the list, and stick msg on the end
	DEBUG(-128,"walking messages\n");
	while(walk->next)
	{
		DEBUG(-128,"walk [%x]",(int)walk);
		walk=walk->next;
	}
	DEBUG(-128,"updating messages\n");
	walk->next=msg;
	EXIT;
}

void initState()
{
	ENTRY;
	state.stateS=newStack();
	state.stringS=newStack();
	state.nodeS=newStack();
	state.infoS=newStack();
	state.inputS=newStack();
	state.defDepth=0;
	state.defActive=0;
	state.defines=NULL;
	EXIT;
}

void finiState()
{
	Define *def,*old;
	ENTRY;
	
	freeStack(state.stateS);
	freeStack(state.stringS);
	freeStack(state.nodeS);
	freeStack(state.infoS);
	freeStack(state.inputS);

	def=state.defines;
	while(def)
	{
		free(def->name);
		if(def->value)
			free(def->value);
		old=def;
		def=def->next;
		free(old);
	}
	
	EXIT;
}

void	popState()
{
	ENTRY;
	freeSTString(state.string);
	if(state.info)
		free(state.info);

	state.state  = (States)(int)stackPop(state.stateS);
	state.string = (STString*)	stackPop(state.stringS);
	state.node   = (spConfigNode*)	stackPop(state.nodeS);
	state.info   = 			stackPop(state.infoS);
	//state.value  = NULL;	//comments in values can happen
	EXIT;
}

void	pushState(States newstate, STString*newstring, spConfigNode*newnode, void*newinfo)
{
	ENTRY;
	stackPush(state.stateS,(void*)state.state);
	stackPush(state.stringS,(void*)state.string);
	stackPush(state.nodeS,(void*)state.node);
	stackPush(state.infoS,(void*)state.info);
	state.state=newstate;
	state.string=newstring;
	state.node=newnode;
	state.info=newinfo;
	//state.value=NULL;	//comments in values can happen.
	EXIT;
}

/*****************/
/* state Parsers */
/*****************/

/* return values:
	<0 = some error
	0  = ok, no errors
	>0 = parse state dependant
*/


int parseText(char c)
{
	ENTRY;
	if(state.flags&STATE_FLAGS_ESCAPED)
	{
		addChar(state.string,c);
		//free(state.node->text);
		//state.node->text=strdup(state.string->str);
		return(0);
	}
	switch(c)
	{
		case 0:		//end of file (doesn't seem to happen)
			DEBUG(-124,"Hey, the end of the file!\n");
			break;
		case '<': 
			//change state, get ready to parse node
			if(state.node->text)
				free(state.node->text);
			state.node->text=strdup(state.string->str);
			pushState(Node,newSTString(32),state.node,NULL);
			break;
		default:
			addChar(state.string,c);			
			//free(state.node->text);
			//state.node->text=strdup(state.string->str);
	}

	return(0);
}

/* helpers for parseAttribute, 
called to determine which kind of value the current attribute has.
no inputs, no outputs.  it's expected to operate on state.value only.

method:  if it contains only numbers, it's an integer.  if there's a '.'
it's a floating point (prepending +/- is ok, defaults to int).

if there are other chars in it, it's assumed to be a one-word string.
this makes things like "option=on" or "formatC=no" or "doStuff=yes" easy.

  we also check for special number formatting, like
  0b101010 or 011001100110b for binary
  0x72a9, deadbeefh, or $8000 for hex ($ prefix causes problems for money attributes...)
  1.4e12, -7.123487641234D-12 for scientific notation.
  a leading zero does not imply octal as it does in C.
*/
int isHex(void)
{
	int i;
	char *value;
	ENTRY;

	value=state.attribute->value.string;
	for(i=0;i<(int)strlen(value);++i)
	{
		switch(value[i])
		{
			case '0':case '1':case '2':case '3':
			case '4':case '5':case '6':case '7':
			case '8':case '9':
			case 'A':case 'a':case 'b':case 'B':
			case 'C':case 'c':case 'd':case 'D':
			case 'E':case 'e':case 'f':case 'F':
				break;
			case 'x':
			case 'X':
				if(i!=1 && i!=(int)strlen(value)-1)
					return(0);
				if(value[0]!='0')
					return(0);	//dont want 'fxbeef' getting thru
				break;
			case 'h':
			case 'H':
				if(i!=(int)strlen(value)-1)
					return(0);
				break;
			//case '$':
			//	if(i)
			//		return(0);
			default:
				return(0);
		}
	}

	return(1);
}

int isBin(void)
{
	char *value;
	int i;
	ENTRY;

	value=state.attribute->value.string;
	for(i=0;i<(int)strlen(value);++i)
	{
		switch(value[i])
		{
			case '0':
			case '1':
				break;
			case 'B':
			case 'b':
				if(i!=1 && i!=(int)strlen(value)-1)
					return(0);
				break;
			default:
				return(0);
		}
	}

	return(1);
}

int isE(void)
{
	char dots=0;	// dot is only allowed before an E
	char plusminus=0;	//bit 0 is pre-e sign, bit 1 is post-e sign.
	char eflag=0;
	char *value;
	int i;
	ENTRY;

	value=state.attribute->value.string;
	for(i=0;i<(int)strlen(value);++i)
	{
		switch(value[i])
		{
			case 'd':
			case 'D':
			case 'E':
			case 'e':
				if(eflag)
					return(0);
				++eflag;
				break;
			case '.':
				if(eflag)	//after e?
					return(0);
				else		//before e?
				{
					if(dots&1)
						return(0);
					dots++;
				}
				break;
			case '+':
			case '-':
				if(eflag)
				{
					if(plusminus&2)
						return(0);
					plusminus+=2;
				}
				else
				{
					if(plusminus&1)
						return(0);
					plusminus++;
				}
				break;
			default:
				if(value[i]<'0' || value[i] >'9')
					return(0);
		}
	}

	return(1);
}

int isNum(void)
{
	char *value;
	char dot=0;	
	int i;
	ENTRY;
	value=state.attribute->value.string;
	for(i=0;i<(int)strlen(value);++i)
	{
		switch(value[i])
		{
			case '0':case '1':case '2':
			case '3':case '4':case '5':
			case '6':case '7':case '8':
			case '9':
				break;
			case '.':
				if(dot)
					return(0);
				++dot;
				break;
			case '+':
			case '-':
				if(i)
					return(0);
			default:
				return(0);
		}
	}
	if(dot)
		return(2);	//floating flag
	return(1);	//1 is normal int.
}


void analyzeAttribute()
{
	int i;
	char *old;
	unsigned long value;
	ENTRY;
	
	if((i=isNum()))
	{
		if(i==2)	//floating
		{
			old=state.attribute->value.string;
			state.attribute->type=SP_CONFIG_VALUE_FLOAT;
			state.attribute->value.d=atof(old);
			free(old);
		}
		else		//int
		{
			old=state.attribute->value.string;
			state.attribute->type=SP_CONFIG_VALUE_INT;
			state.attribute->value.i=atoi(old);
			free(old);
		}
	}
	else if(isBin())
	{
		value=0;
		old=state.attribute->value.string;
		if(old[(int)strlen(old)-1]=='b')
		{
			for(i=0;i<(int)strlen(old)-1;++i)
			{
				value*=2;
				if(old[i]=='1')
					value++;
			}
		}
		else	//0b notation, start from 2, goto end
		{
			for(i=2;i<(int)strlen(old);++i)
			{
				value*=2;
				if(old[i]=='1')
					value++;
			}
		}
		state.attribute->type=SP_CONFIG_VALUE_INT;
		state.attribute->value.i=value;
		free(old);		
	}
	else if(isHex())
	{
		old=state.attribute->value.string;
		value=0;
		if(old[0]=='$')
		{
			for(i=1;i<(int)strlen(old);++i)
			{
				value<<=4;
				if(old[i]>='0' && old[i]<='9')
				{
					value+=old[i]-'0';
				}
				else //must be a-f
				{
					value+=10+tolower(old[i])-'a';
				}
			}
		}
		else if(old[1]=='x' || old[1]=='X')
		{
			for(i=2;i<(int)strlen(old);++i)
			{
				value<<=4;
				if(old[i]>='0' && old[i]<='9')
				{
					value+=old[i]-'0';
				}
				else //must be a-f
				{
					value+=10+tolower(old[i])-'a';
				}

			}
		}
		else // 'h' suffix
		{
			for(i=0;i<(int)strlen(old)-1;++i)
			{
				value<<=4;
				if(old[i]>='0' && old[i]<='9')
				{
					value+=old[i]-'0';
				}
				else //must be a-f
				{
					value+=10+tolower(old[i])-'a';
				}

			}
		}
		state.attribute->type=SP_CONFIG_VALUE_INT;
		state.attribute->value.i=value;
		free(old);
	}
	else if(isE())
	{
		old=state.attribute->value.string;
		state.attribute->type=SP_CONFIG_VALUE_FLOAT;
		state.attribute->value.d=atof(old);
		free(old);
	}
	//else	//must be a string or nothing :)
	/*if(state.attribute->value.string && strlen(state.attribute->value.string))
		state.attribute->type=SP_CONFIG_VALUE_STRING;*/
	EXIT;
	return;
}

//old analyze value...
/*	char isNumber=1,isFloating=0;
	unsigned int i=0;
	void*p;

	ENTRY;

	DEBUG(-8,"Analyzing [%s]\n",state.attribute->value.string);

	//first, check first char for optional sign

	switch(state.attribute->value.string[0])
	{
		case '+':
		case '-':
			i=1;	//don't check this one
			break;
		default:
			i=0;
	}

	//next, check all chars for non-numeric or '.'
	for(;i<strlen(state.attribute->value.string);++i)
	{
		switch(state.attribute->value.string[i])
		{
			case '.':
				isFloating=1;
				break;
			case '0':case '1':case '2':
			case '3':case '4':case '5':
			case '6':case '7':case '8':
			case '9':
				break;
			default:
				isNumber=0;//can't use hex or octal or binary yet :(
		}
	}

	if(isNumber)
	{
		p=state.attribute->value.string;
		if(isFloating)
		{
			state.attribute->value.d=atof(state.attribute->value.string);
			state.attribute->type=SP_CONFIG_VALUE_FLOAT;
		}
		else
		{
			state.attribute->value.i=atoi(state.attribute->value.string);
			state.attribute->type=SP_CONFIG_VALUE_INT;
		}
		free(p);	//clean up old string of number
	}
	else	//must be a one-word string
		state.attribute->type=SP_CONFIG_VALUE_STRING;


	EXIT;
	return;
}*/
/*
	parsing an attribute is somewhat easy.  we must watch out for 
	'/' and '>' though.
	
	instead of calling free and strdup for every added char, we do a
state.attribute->value.string=strdup(state.string->str);
	or
state.attribute->name=strdup(state.string->str);
	on each exit condition.  saves a bit of time, and a lot of mem fragmentation.
*/

int parseAttribute(char c)
{
	int *info;

	ENTRY;

	info=(int*)state.info;

	if(!state.info)	//just starting
	{
		state.info=(int*)malloc(sizeof(int)*4);	//extra ints will be used later for other stuff
		state.attribute=NULL;	//this is where the nullness should happen =)
		info=(int*)state.info;
		info[0]=0;
	}

	/* info[0] key:
		0 - initial state.  see if attribute name starts in raw or quote mode
		    if / or >, no attributes happen.  skip white space.
		1 - reading raw chunk, stop at =, ", white space, or / (done)
		2 - quoted state, stop at ", move to 1

		3 - determining attribute state, see if quote or raw
		4 - raw chunk, stop at " or white space. if we hit a /
		    (terminator), the attribute is done, and node is done too.
		5 - quoted chunk, stop at ", move to 4

	*/

	DEBUG(-40,"Value parse state [%i] - parsing char [%c]\n",info[0],c);

	switch(info[0])
	{
		case 0:	//name determanent state
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				info[0]=1;
				state.attribute=spConfigAttributeNew();
				//state.attribute->type|=SP_CONFIG_VALUE_STRING;
				addChar(state.string,c);
				//state.attribute->name=strdup(state.string->str);
			}
			else
			{
				switch(c)
				{
					case ' ':
					case '\t':
					case '\n':
						break;
					case '/':
					case '>':	//popstate (no mo' values) return 1
						//state.attribute->name=strdup(state.string->str);
						popState();
						return(1);
						break;
					case '"':
						state.flags|=STATE_FLAGS_QUOTES;
						info[0]=2;	//quote chunk
						if(state.attribute==NULL)
							state.attribute=spConfigAttributeNew();

						break;
					default:
						info[0]=1;	//name state
						if(state.attribute==NULL)
							state.attribute=spConfigAttributeNew();
						//state.attribute->type|=SP_CONFIG_VALUE_STRING;
						addChar(state.string,c);
						//state.attribute->name=strdup(state.string->str);
						break;
				}
			}
			break;
		case 1:		//non-quoted chunk
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				//free(state.attribute->name);
				addChar(state.string,c);
				//state.attribute->name=strdup(state.string->str);
			}
			else
			switch(c)
			{
				case ' ':
				case '\t':
				case '\n':	//whitespace == end of this val
					state.attribute->name=strdup(state.string->str);
					spConfigNodeAddAttribute(state.node,state.attribute);
					state.attribute=NULL;					
					freeSTString(state.string);
					state.string=newSTString(16);					
					info[0]=0;
					break;
				case '=':	//move to 3
					state.attribute->name=strdup(state.string->str);
					freeSTString(state.string);
					state.string=newSTString(16);
					state.attribute->type|=SP_CONFIG_VALUE_STRING;

					info[0]=3;
					break;
				case '"':	//entering quoted portion
					state.flags|=STATE_FLAGS_QUOTES;
					info[0]=2;
					break;
				case '>':
				case '/':	//terminators
					state.attribute->name=strdup(state.string->str);
					spConfigNodeAddAttribute(state.node,state.attribute);
					popState();
					return(1);
					break; 
				default:	//normal char
					DEBUG(-15,"normal char [%x][%x][%x]\n",(int)state.string,(int)state.attribute,(int)state.string->str);
					addChar(state.string,c);
					//free(state.attribute->name);
					//state.attribute->name=strdup(state.string->str);
					break;
			}
			break;
		case 2:	//quote chunk 
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				//free(state.attribute->name);
				addChar(state.string,c);
				//state.attribute->name=strdup(state.string->str);
			}
			else
			{
				switch(c)
				{
					case '"':
						state.flags&=~STATE_FLAGS_QUOTES;
						info[0]=1;
						break;
					default:
						addChar(state.string,c);
						//free(state.attribute->name);
						//state.attribute->name=strdup(state.string->str);
						break;
				}
			}
			break;
		case 3:	//attribute determinant state
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				info[0]=4;
				freeSTString(state.string);
				state.string=newSTString(32);
				addChar(state.string,c);
				//state.attribute->value.string=strdup(state.string->str);
			}
			else
			{
				switch(c)
				{
					case '=':	//extra equal sign. warn
						addMessage(SP_CONFIG_MESSAGE_WARNING,"Ignoring Extra '='");						
						break;
					case ' ':
					case '\t':
					case '\n':	//whitespace means we're done, no matter what
						addMessage(SP_CONFIG_MESSAGE_WARNING,"No assigned value after '='");
						freeSTString(state.string);
						state.string=newSTString(32);
						info[0]=0;
						break;
					case '>':
					case '/':	//blatant error (terminating while assigning a value)
							//we'll assign the value name, and bail from this state.
							//wait, this isnt an error...  yes, now it is.
						addMessage(SP_CONFIG_MESSAGE_ERROR,"Terminating while assigning value.  no value assigned");
						state.attribute->value.string=strdup(state.string->str);
						spConfigNodeAddAttribute(state.node,state.attribute);
						popState();
						return(1);
						break;
					case '"':	//quoted stuff!
						state.flags|=STATE_FLAGS_QUOTES;
						info[0]=5;
						freeSTString(state.string);
						state.string=newSTString(32);
						state.attribute->type|=SP_CONFIG_VALUE_STRING;
						break;
					default:
						info[0]=4;
						freeSTString(state.string);
						state.string=newSTString(16);
						addChar(state.string,c);
						//state.value->attribute.string=strdup(state.string->str);
				}
			}
			break;
		case 4:	//non-quoted chunk
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				addChar(state.string,c);
				//free(state.attribute->value.string);
				//state.attribute->value.string=strdup(state.string->str);
			}
			else
			{
				switch(c)
				{
					case ' ':
					case '\t':
					case '\n':
						//done with this one
						//analyze attribute string for type
						state.attribute->value.string=strdup(state.string->str);
						spConfigNodeAddAttribute(state.node,state.attribute);
						// analyzeAttribute(); PAAK
						info[0]=0;	//start over
						freeSTString(state.string);
						state.string=newSTString(32);
						state.attribute=NULL;
						break;
					case '"':	//entering quoted chunk
						state.flags|=STATE_FLAGS_QUOTES;
						info[0]=5;
						break;
					case '>':
					case '/':	//terminators
						state.attribute->value.string=strdup(state.string->str);
						// analyzeAttribute(); PAAK
						spConfigNodeAddAttribute(state.node,state.attribute);
						popState();
						return(1);
						break;
					default:
						addChar(state.string,c);
						//free(state.attribute->value.string);
						//state.attribute->value.string=strdup(state.string->str);
						break;
				}
			}
			break;
		case 5:	//quoted attribute (string)
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				addChar(state.string,c);
				//free(state.attribute->value.string);
				//state.attribute->value.string=strdup(state.string->str);
			}	
			else
			{
				switch(c)
				{
					case '"':	//end of quote, bail to raw
						state.flags&=~STATE_FLAGS_QUOTES;
						/*spConfigNodeAddAttribute(state.node,state.attribute);
						freeSTString(state.string);
						state.string=newSTString(32);*/
						info[0]=4;
						break;
					default:
						addChar(state.string,c);
						//free(state.attribute->value.string);
						//state.attribute->value.string=strdup(state.string->str);
						break;
				}
			}
			break;
		default:	//how did we get here?
			addMessage(SP_CONFIG_MESSAGE_ERROR,"How did we get here?");
			break;
	}

	/* value bypass test thing */
	/*if(c == ']' || c == '/')	//node terminators
	{
		popState();
		return(1);
	}*/

	return(0);
};

/*
Node parsing is somewhat complex, due to the many different ways they
can be terminated.  terminating nodes looks like a node creation, so we 
cannot actually malloc a node until we know for sure that it's not terminating
a previous one.

to help manage this, we have the info[0] int that indicates what we know/where
we are in parsing the node.  at some point, the node pointer in state changes
to to reflect the true new node that is created.  in some ways, it too is a
state machine, within the larger machine of ParseMain.  there is no
documentation as to how the states for info[0] correlate to numbers, i just
sort of went along picking numbers. ;^)  they're commented though

wow this is scary =)

to prevent calling free and then strdup on _every_ added char, we must do a
state.node->name=strdup(state.string->str);
on any exiting condidion to apply the node's parsed name.
*/

int parseNode(char c)
{
	int	*info;
	spConfigNode	*node;	//temp node ptr
	ENTRY;

	info=(int*)state.info;

	if(!state.info)	//just starting?
	{
		DEBUG(-20,"Initializing node parse state\n");
		state.info=malloc(sizeof(int)*4);	//extra ints will be used later
		info=(int*)state.info;
		info[0]=0;	//nothing
	}

	DEBUG(-15,"node parse state is [%i]\n",info[0]);

	switch(info[0])	//what's the state of the union?
	{
		case 0:		//initial state
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				node=spConfigNodeNew();	//create.  see default case below.
				spConfigNodeAddChild(state.node,node);
				state.node=node;
				addChar(state.string,c);
				info[0]=1;
				return(0);
			}
			switch(c)
			{
				case ' ':
				case '\t':	//skip white spaces
				case '\n':
					break;
				case '/':	//terminator
					info[0]=100;
					break;
				case '"':	//a quoted name/chunk
					state.flags|=STATE_FLAGS_QUOTES;
					info[0]=2;	//get whole quoted chunk
					node=spConfigNodeNew();
					spConfigNodeAddChild(state.node,node);
					state.node=node;					
					break;
				case '>':	//error <>
					addMessage(SP_CONFIG_MESSAGE_NOTICE,"Empty node.  ignored.");
					popState();
					return(1);
					break;
				default:	//nothing special (name)
					// we make the new node, and place it on the stack over the old one (which was a clone of previous node anyways)
					node=spConfigNodeNew();	//create...
					spConfigNodeAddChild(state.node,node);	//...link...
					state.node=node;	//... and overwrite
					addChar(state.string,c);
					//state.node->name=strdup(state.string->str);
					info[0]=1;	//set it to name mode
					break;
			}
			break;
		case 1:		//reading name (raw chunk)
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				DEBUG(-15,"Escaped, adding [%c]\n",c);
				//free(state.node->text);
				addChar(state.string,c);
				//state.node->name=strdup(state.string->str);
				return(0);
			}
			switch(c)
			{
				case ' ':
				case '\t':	//end of name
				case '\n':
					//now we are parsing values
					//freeSTString(state.string);
					//state.string=newSTString(32);
					if(state.node->name)
						free(state.node->name);
					state.node->name=strdup(state.string->str);
					info[0]=5;
					pushState(Attribute, newSTString(32), state.node, NULL);
					break;
				case '/':	//end if next char is ]
					info[0]=10;
					break;
				case '>':	//node ends elsewhere
					//push a text state.
					//never coming back, so clean up.
					if(state.node->name)
						free(state.node->name);
					state.node->name=strdup(state.string->str);
					pushState(Text,newSTString(32),state.node,NULL);
					break;
				case '"':	//quoted chunk
					state.flags|=STATE_FLAGS_QUOTES;
					info[0]=2;
					break;
				default:	//normal char
					//free(state.node->name);
					addChar(state.string,c);
					//state.node->name=strdup(state.string->str);
					break;
			}
			break;
		case 2:	//get a quoted chunk
			// can't use quote state, as it can modify the 
			// string pointer, making this one invalid
			if(state.flags&STATE_FLAGS_ESCAPED)
			{
				//free(state.node->text);
				addChar(state.string,c);
				//state.node->name=strdup(state.string->str);
				return(0);
			}
			switch(c)
			{
				case '"':// end of quoted chunk
					state.flags&=~STATE_FLAGS_QUOTES;
					//all done! (value mode now)
					//freeSTString(state.string);
					//state.string=newSTString(32);
					info[0]=1;
					//pushState(Attribute,newSTString(32),state.node,NULL);
					break;
				default:
					//free(state.node->name);
					addChar(state.string,c);
					//state.node->name=strdup(state.string->str);
					break;
			}
			break;
		case 5:		//done values, checking for  / or ]
			switch(c)
			{
				case '>':	//end of node, ends elsewhere
					DEBUG(-80,"hmm, looks like the end of this node\n");
					pushState(Text,newSTString(32),state.node,NULL);

					break;
				case '/':	//oo, a terminator
					info[0]=10;
					break;
				default:	//hmm, value shouldn't be done.
					addMessage(SP_CONFIG_MESSAGE_WARNING,"possible lost values, unexpeted character after values");
					//addChar(state.string,c);
					break;
			}
			break;
		case 6:		//similar to state two, but for values specifically
			break;
		case 10:	//if c == ], we are done (/ as previous c)
			switch(c)
			{
				case '>':	//nice, a clean ending
					//pop state, and we're done (?)
					DEBUG(-15,"Self terminating node\n");
					if(state.node->name)
						free(state.node->name);
					state.node->name=strdup(state.string->str);
					popState();
					break;
				default:	//hmm, this is bad...
					addMessage(SP_CONFIG_MESSAGE_NOTICE,"ignoring spurious / in node");
					break;
			}
			break;
		case 100:	//terminal node (non-quoted)
			//check for ] or a string.
			//a ] means we just pop the state,
			//a string involves getting the string, and then
			//comparing to the existing node's name
			DEBUG(-15,"In a terminal node [%c]\n",c);
			switch(c)
			{
				case '"':	//ok, a quoted node name
					state.flags|=STATE_FLAGS_QUOTES;
					if(state.string->pos)
					{
						addMessage(SP_CONFIG_MESSAGE_NOTICE,"Quoted name begins after valid text.  ignoring previous text");
					}
					info[0]=101;
					freeSTString(state.string);
					state.string=newSTString(16);
				case '>':
					if(state.flags&STATE_FLAGS_ESCAPED)
					{
						addChar(state.string,c);
						break;
					}
					//optionally compare names here
					DEBUG(-15,"Comparing names [%x][%x]\n",(int)state.string->str,(int)state.node->name);
					if(state.string && state.node->name && strcmp(state.string->str,state.node->name))
					{
						//names dont match, warning
						addMessage(SP_CONFIG_MESSAGE_NOTICE,"Terminator name does not match created name, ignoring");
					}
					DEBUG(-15,"exiting node state\n");
					popState();	//clean up this node
					return(1);	//flag down parseMain
					break;
					case ' ':case '\t':case'\n':
						break;	//skip whitespace
				default:
					//build names here
					addChar(state.string,c);
					break;
			}
			break;
		case 101:	//terminal node, quoted name
			switch(c)
			{
				case '"':	//the end ?
					state.flags&=~STATE_FLAGS_QUOTES;
					if(state.flags&STATE_FLAGS_ESCAPED)
					{
						addChar(state.string,c);
						break;
					}
					if(strcmp(state.string->str,state.node->name))
					{
						addMessage(SP_CONFIG_MESSAGE_WARNING,"Terminating name does not match created name, ignoring");
					}
					popState();
					return(1);
				default:
					addChar(state.string,c);
			}
			break;
	}

	return(0);
}

//system directive handlers
int doInclude(char*string)
{
	FILE*file;
	char *nextfile;
	int lim,len,pos,i,j,ret=0;
	STString *stream;
	ENTRY;

	pos=0;len=0;lim=32;
	nextfile=(char*)malloc(lim);
	for(i=0;i<(int)strlen(string)+1;++i)
	{
		switch(string[i])
		{
			case ' ': case 0:
			case '\n':case '\t':
				if(pos)
				{
					//end of some file name guy
					file=fopen(nextfile,"r");
					if(!file)
					{
						//hmm, files doesnt exist
						//free(nextfile);
						--ret;
					}
					//addMessage(SP_CONFIG_MESSAGE_INFO,"Including a file");
					fseek(file,0,SEEK_END);
					j=ftell(file);
					rewind(file);
					stream=newSTString(j*sizeof(char)+1);
					stream->row=1;
					stream->col=1;
					stream->name=strdup(nextfile);
					DEBUG(-4,"Buffer size is [%i]\n",j+1);
					fread(stream->str,1,j,file);
					fclose(file);
					
					//this reversifies include order.
					stackPush(state.inputS,(void*)state.input);
					state.input=stream;
					pos=0;
					len=0;
				}
				//else ignore leading/trailing spaces
				break;
			default:
				if(pos+1==lim)
				{
					nextfile=(char*)realloc(nextfile,lim*2);
					lim*=2;
				}
				nextfile[pos++]=string[i];
				nextfile[pos]=0;
				++len;

		};
	}
	free(nextfile);
	
	return(ret);
}

char *getName(char*string)
{
	int len,i;
	char *name;
	ENTRY;
	i=0;
	len=0;
	while(string[i]==' ' || string[i]=='\t' || string[i]=='\n')
		++i;
	while(string[i+len]!=' ' && string[i+len]!='\t' && string[i+len]!='\n' && string[i+len])
		++len;
	name=(char*)malloc(sizeof(char)*(len+1));
	memcpy(name,string+i,len);
	name[len]=0;
	return(name);
}

char *getVal(char*string)
{
	int len,start,i;
	char *value=NULL;
	ENTRY;
	
	i=0;
	len=0;
	while(string[i]==' ' || string[i]=='\t' || string[i]=='\n')
		++i;
	while(string[i+len]!=' ' && string[i+len]!='\t' && string[i+len]!='\n' && string[i+len])
		++len;
	if(string[i+len]==0)
		return(NULL);
	start=i+len;
	len=strlen(string)-start;
	value=(char*)malloc(sizeof(char)*(len+1));
	memcpy(value,string+start+1,len);
	value[len]=0;
	
	
	return(value);
}

void doDefine(char*string)
{
	char *name,*value;
	Define *def,*next;
	ENTRY;
	
	name=getName(string);
	value=getVal(string);
	DEBUG(-24,"defining [%s] with [%s]\n",name,value);
	def=state.defines;
	if(!def)	//first define
	{
		def=(Define*)malloc(sizeof(Define));
		def->name=name;
		def->value=value;
		def->next=NULL;
		state.defines=def;
		EXIT;
		return;
	}
	//not first post
	//the goal here is to put the defines in alphabetical order, so 
	//searching for them goes quicker (binary vs linear)
	while(strcmp(name,def->name)>=0 && def->next)
		def=def->next;
	
	if(strcmp(name,def->name)==0)
	{
		addMessage(SP_CONFIG_MESSAGE_WARNING,"Symbol already defined");
		EXIT;
		return;
	}
	
	next=def->next;	//save old next pointer
	def->next=(Define*)malloc(sizeof(Define));
	def=def->next;
	def->next=next;
	def->name=name;
	def->value=value;
		
	EXIT;
}

void unDefine(char*string)
{
	char *name;
	Define *def,*prev;
	ENTRY;
	name=getName(string);
	
	//now, walk the defines list. this could do a binary search since
	// the list should be in alphabetical order
	// of course, doing a binary search with linked lists is a feat in itselt....

	prev=NULL;
	def=state.defines;
	while(def)
	{
		if(strcmp(def->name,name)==0)
		{
			DEBUG(-24,"undefining [%s]\n",name);
			if(prev)	//have a previous, gotta make it point right
				prev->next=def->next;
			else		//no prev=first post.  clean up state pointer
				state.defines=def->next;
			free(def->name);
			if(def->value)
				free(def->value);
			free(def);
			free(name);
			EXIT;
			return;
		}
		prev=def;
		def=def->next;
	}
	DEBUG(-24,"[%s] wasn't found (undefined?)\n",name);
	addMessage(SP_CONFIG_MESSAGE_NOTICE,"cannot undefine Symbol because it doesn't seem to exist.");
	free(name);
	EXIT;
}

int ifDef(char*string)
{
	char *name;
	Define *def;
	ENTRY;
	def=state.defines;
	name=getName(string);

	while(def)
	{
		if(strcmp(name,def->name)==0)
		{
			DEBUG(-24,"[%s] is defined\n",name);
			free(name);
			return(1);
		}
		def=def->next;
	};
	free(name);
	return(0);
}

int ifEq(char*string)
{
	char *name,*value;
	Define *def;
	ENTRY;

	name=getName(string);
	value=getVal(string);
	def=state.defines;
	
	while(def)
	{
		if(strcmp(name,def->name)==0)
		{
			if(strcmp(value,def->value)==0)
			{
				free(name);
				free(value);
				return(1);
			}
			free(name);
			free(value);
			return(0);
		}
		def=def->next;
	}
	//addMessage(SP_CONFIG_MESSAGE_INFO,"ifeq failed\n");
	return(0);
}

//drop down a level, and bring active level with us if it's higher
void endIf()
{
	ENTRY;
	if(!state.defDepth)
	{
		addMessage(SP_CONFIG_MESSAGE_ERROR,"endif without opening if directive.");
		EXIT;
		return;
	}
	--state.defDepth;
	if(state.defActive>state.defDepth)
		state.defActive=state.defDepth;
	EXIT;
}

int parseSystem(char c)
{
	int *info;
	ENTRY;

	//right now, we just parse <message foo> types.
	// includes and conditionals come later.
	info=(int*)state.info;
	if(!state.info)
	{
		state.info=(int*)malloc(sizeof(int)*2);
		//(int*)state.info[0]=0;
		info=(int*)state.info;
		info[0]=0;
	}

	/* info[0] breakdown:
		0 - just entered state/skipping leading white space
		1 - parsing system type (stop at more white space or ])
		2 - parsing attribute for type (stop at ] )
		
	  after 1 completes, check string against known system operations
	  info[1] holds the system type
		see enum for SystemDirective above

	*/

	if(state.flags&STATE_FLAGS_ESCAPED)
	{
		//add char to message string (won't be this easy with more stuff)
		addChar(state.string,c);
	}
	else
	{
		switch(info[0])
		{
			case 0:	//skip whitespace
				if(state.flags&STATE_FLAGS_ESCAPED)
				{
					info[0]=1;
					break;	//let us hope they didn't escape a whitespace char for some reason...
				}
				switch(c)
				{
					case ' ':case '\t':case '\n':
						break;
					case ']':
						addMessage(SP_CONFIG_MESSAGE_NOTICE,"Empty System directive");
						break;
					default:
						info[0]=1;
				}
				break;
			case 1:	//looking for more whitespace, or >
				if(state.flags&STATE_FLAGS_ESCAPED)
				{
					addMessage(SP_CONFIG_MESSAGE_NOTICE,"You shouldn't be escaping in a system name!");
					break;
				}
				switch(c)
				{
					case ' ':case '\n':case '\t':case ']':
						//check string for stuff
						if(!strcmp("include",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Include;
						}
						else if(!strcmp("info",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Info;
						}	//other messages
						else if(!strcmp("notice",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Notice;
						}
						else if(!strcmp("warning",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Warning;
						}
						else if(!strcmp("error",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Error;
						}
						else if(!strcmp("define",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Def;
						}
						else if(!strcmp("undef",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Undef;
						}else if(!strcmp("ifdef",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Ifdef;
						}
						else if(!strcmp("ifndef",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Ifndef;
						}else if(!strcmp("else",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Else;
							if(state.defDepth==state.defActive)	//if we're active now, else ends that
								--state.defActive;
							else if(state.defDepth-1==state.defActive)	//if previous level is active, now we are
								++state.defActive;							
						}else if(!strcmp("endif",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Endif;
							endIf();	//HACK, the ']' prematurely kills the state when we dont get white space
						}
						else if(!strcmp("ifeq",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Ifeq;
						}else if(!strcmp("ifneq",state.string->str))
						{
							freeSTString(state.string);
							state.string=newSTString(32);
							info[1]=Ifneq;
						}
						else
							info[1]=-1;	//invalid!
						info[0]=2;
						if(c==']')	//non-spaced directive (e.g. [endif] )
							popState();
						break;
					default:
						addChar(state.string,c);
				}
				break;
			case 2://read the remainder of the system, stopping only at ]
				if(state.flags&STATE_FLAGS_ESCAPED)
					addChar(state.string,c);
				else
				switch(c)
				{
					case ']':
						//all done, apply string and bail
						switch(info[1])
						{
							case Include:
								//do include stuff someday
								//addMessage(SP_CONFIG_MESSAGE_ERROR,"sorry, includes aren't supported yet :(");
								if(state.defDepth==state.defActive)
								{
									if(doInclude(state.string->str))
									{
										addMessage(SP_CONFIG_MESSAGE_WARNING,"Couldn't find one or more specified include files");
									}
								}
								break;
							case Info: //info (not bad)
								if(state.defDepth==state.defActive)
								{
									addMessage(SP_CONFIG_MESSAGE_INFO,state.string->str);
								}
								break;
							case Notice: //notice (not too bad)
								if(state.defDepth==state.defActive)
								{
									addMessage(SP_CONFIG_MESSAGE_NOTICE,state.string->str);
								}
								break;
							case Warning: //warning (kinda bad)
								if(state.defDepth!=state.defActive)
								{
									break;
								}
								addMessage(SP_CONFIG_MESSAGE_WARNING,state.string->str);
								break;
							case Error: //error (ReallyBad)
								if(state.defDepth==state.defActive)
								{
									addMessage(SP_CONFIG_MESSAGE_ERROR,state.string->str);
								}
								break;
							case Def:	//define
								if(state.defDepth==state.defActive)
								{
									doDefine(state.string->str);
								}
								break;
							case Undef: //undef
								if(state.defDepth==state.defActive)
								{
									unDefine(state.string->str);
								}
								break;
							case Ifdef://ifdef
								++state.defDepth;
								if(state.defActive==state.defDepth-1)
								{
									if(ifDef(state.string->str))
										++state.defActive;
								}
								break;
							case Ifndef://ifndef
								++state.defDepth;
								if(state.defActive==state.defDepth-1)
								{
									if(!ifDef(state.string->str))
										++state.defActive;
								}
								break;
							case Else://else
								//for some reason, this part never
								//executes, so it's handled above
								break;
							case Endif://endif	(not called often, only if endif ends with a space)
								//endIf(); //j/k, this is never supposed to happen, lets we get some errors.
								break;
							case Ifeq://ifeq
								++state.defDepth;
								if(state.defActive==state.defDepth-1)
								{
									if(ifEq(state.string->str))
										++state.defActive;
								}
								break;
							case Ifneq://ifneq
								++state.defDepth;
								if(state.defActive==state.defDepth-1)
								{
									if(!ifEq(state.string->str))
										++state.defActive;
								}
								break;

							default:
								addMessage(SP_CONFIG_MESSAGE_WARNING,"ignoring unrecognized system directive");
						};
						popState();
					default:
						addChar(state.string,c);
				}
				break;
		}
	}
	
	return(0);
}

int ParseMain(spConfigNode*root,char*name,STString*string)
{

	char c;
	ENTRY;
	
	// to use include files, we must move state-initialization into the api funcs.
	//  this is simple, but i'll do it later.  this also causes state.input to be volatile
	//  so we gotta stackify that as well (and only push/pop on include/eof)
	
	initState();

	pushState(Done,NULL,NULL,NULL);	//when stack is exhausted, we are done

	pushState(Text,newSTString(32),root,NULL);	//seed stack with known state

	state.input=string;	//so warnings can get position info
	state.root=root;
	
	/*here, we read each char and hand it off to the proper parser */

	while((int)state.input !=-1 && peekChar(state.input)!=-1 && state.state!=Done)
	{
		c=getChar(state.input);
		
		/* we look for a '\', and set escaped when it happens */
		if(c=='\\')
		{
			/*
			It's tempting to just have it 
			addChar(state.string,getChar);
			but addchar can change the string pointer,
			and we don't know where that may need to change
			elsewhere, so we require all the handlers to check 
			for this and handle intelligently. */
			DEBUG(-24,"Escape char\n");
			
			c=getChar(state.input);
			state.flags|=STATE_FLAGS_ESCAPED;
			
		}
		// look for system stuff (must not be in comment state though)
		else if(c=='[' && state.state!=LineComment && state.state!=BlockComment)
		{
			pushState(System,newSTString(32),state.node,NULL);
			DEBUG(-64,"System mode entered\n");
		}
		/* we look for comments here to reduce the handler code */
		else if(!(state.flags&STATE_FLAGS_QUOTES))
		{
/* PAAK
			if(c=='#')
			{
				//push state, enter line comment
				// if we're already in line comment, ignore
				if(state.state!=LineComment)
				{
					pushState(LineComment,NULL,state.node,NULL);
					DEBUG(-64,"Line comment mode entered [%i][%i]\n",string->row,string->col);
				}
			}
			else if(c=='/')
			{
				if(peekChar(state.input)=='/')
				{
					if(state.state!=LineComment)
					{
						pushState(LineComment,NULL,state.node,NULL);
						DEBUG(-64,"Line comment entered [%i][%i]\n",string->row,string->col);
					}
				}
					else if(peekChar(state.input)=='*')
				{
					getChar(state.input);	// '*'
					c=getChar(state.input);	// first comment char
					pushState(BlockComment,NULL,state.node,NULL);
					DEBUG(-64,"Block comment entered [%i][%i]\n",string->row,string->col);
				}
			}
*/
		}
		/* now we've determined comment stuff, let us hand out 
		data to parse handlers */

		/* small handlers may be inlined here */

		DEBUG(-68,"Parsing char [%c] for state [%i]\n",c,state.state);
		switch(state.state)
		{
			case System:
				parseSystem(c);
				break;
			case LineComment:
				DEBUG(-64,"Checking line termination\n");
				if(c=='\n')
				{
					DEBUG(-66,"NewLine found, exiting comment...\n");
					popState();
					DEBUG(-64,"Line comment Exited [%i][%i]\n",string->row,string->col);
				}
				break;
			case BlockComment:
				DEBUG(-64,"Checking block termination\n");
				if(c=='*')
				{
					if(peekChar(string)=='/')
					{
						DEBUG(-66,"End found, exiting block...\n");
						getChar(string);	// '/'
						popState();
						DEBUG(-64,"Block comment exited [%i][%i]\n",string->row,string->col);
					}
				}
				break;
			case Text:
				if(state.defDepth!=state.defActive)
					break;
				parseText(c);
				break;
			case Node:
				if(state.defDepth!=state.defActive)
					break;
				switch(parseNode(c))
				{
					case 0:	//nothing exciting
						break;
					case 1:	//oo, terminal node!
						if(state.node==state.root)
						{
							addMessage(SP_CONFIG_MESSAGE_WARNING,"attempt to exit root node.  ignored");
							//popState();	//kill text state (might be funky)
							break;
						}
						popState();//kill text state
						popState();//kill node state
						break;

				}
				break;
			case Attribute:
				if(state.defDepth!=state.defActive)
					break;
				switch(parseAttribute(c))
				{
					case 0:	//nothing fun
						break;
					case 1:	//terminator, unget so node gets the char
						ungetChar(string);
						break;
				}
				break;
			case Quote:	//this may be redundant/unuseful (modifies state.string pointer)
				if(state.defDepth!=state.defActive)
					break;
				DEBUG(-64,"Checking for \" to end quote\n");
				if(!(state.flags & STATE_FLAGS_ESCAPED) && c=='"')
				{
					popState();
				}
				else
					addChar(state.string,c);
				break;
			default:
				break;
		}

		//clean up escaped flag
		state.flags&=~STATE_FLAGS_ESCAPED;
		
		//resolve include files
		if(peekChar(state.input)==-1)	//end of this string?
		{
			freeSTString(state.input);
			state.input=(STString*)stackPop(state.inputS);
		}
	}
	//flush state (no leaks)
	while((int)state.string!=-1 && (int)state.info!=-1 && (int)state.node !=-1)
		popState();

	finiState();
	finiCache();	//clean up our cache mess.

	/* above might be dangerous, as we may lose important values that are
	still in use...  therefore it is imperative that each handler is
	'clean' and doesn't assign data from the stack, but copies it instead,
	especially in the case of an error. */

	return (0);
}

/*********************/
/* the api functions */
/*********************/

int spConfigLoad(spConfigNode*root,char*filename)
{
	STString *string;
	int i,ret;
	FILE*confFile;

	ENTRY;

	if(!filename || !root)
	{
		DEBUG(2,"invalid pointer for Filename or root\n");
		return(-1);
	}
	if((confFile=fopen(filename,"ro")) ==NULL)
	{
		DEBUG(4,"Unable to open [%s] for reading\n",filename);
		return(-2);
	}

	fseek(confFile,0,SEEK_END);
	i=ftell(confFile);
	rewind(confFile);
	string=(STString*)malloc(sizeof(STString));
	string->str=(char*)malloc(i*sizeof(char)+1);
	if(!string || !string->str)
	{
		DEBUG(126,"Out of memory\n");
		return(-4);
	}
	string->str[i]=0;	//null terminate, just in case :)
	string->pos=0;
	string->len=i;
	string->row=1;
	string->col=1;
	string->name=strdup(filename);
	DEBUG(-4,"Buffer size is [%i]\n",i+1);
	fread(string->str,1,i,confFile);
	fclose(confFile);
	
	ret=ParseMain(root,filename,string);
	
	//these are freed in the parsemain include cleanup loop
	//free(string->str);
	//free(string->name);
	//free(string);

	return(ret);
}

int spConfigLoadStr(spConfigNode*root,char*name,char*string)
{
	STString *str;

	ENTRY;

	str=(STString*)malloc(sizeof(STString));
	str->str=string;
	str->len=strlen(string);
	str->name=strdup(name);
	str->pos=0;
	str->row=1;
	str->col=1;

	return(ParseMain(root,name,str));
}
