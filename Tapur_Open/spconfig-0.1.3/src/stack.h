/* stack.h - v0.1.3
 *	* a basic procedural stack
 */

/* Copyright (C) 2003 softpixel (http://softpixel.com/)
 * This file is covered by a BSD-Style License, available in
 * the LICENSE file in the root directory of this package, as well as at
 * http://prj.softpixel.com/licenses/#bsd
 */

/* Simple stack push/pop operations.  internal to spconfig */

typedef struct itemS
{
	void*		val;
	struct itemS	*next;
}item;

typedef struct
{
	item		*stack;
	int 		depth;
}Stack;

void	finiCache();

Stack*	newStack();
void	freeStack(Stack*stack);

void	stackPush(Stack*stack,void*val);
void*	stackPop(Stack*stack);
