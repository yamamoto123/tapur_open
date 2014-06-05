/*
	
	debug header file
	2004/02/23 -paak-
	
*/
#ifndef __PO_DEBUG_H
#define __PO_DEBUG_H

//#define DEBUG_VIEW_LINE			/* view line and file info */

//#define DEBUG_ERR					/* error log			*/
//#define DEBUG_WRN					/* warning log			*/
//#define DEBUG_MAIN  				/* main debug			*/
//#define DEBUG_FA			 	 	/* file access debug	*/
//#define DEBUG_HP					/* http access debug	*/
//#define DEBUG_ST					/* setting debug		*/
//#define DEBUG_RS					/* response debug		*/
//#define DEBUG_TF					/* transfer debug		*/
//#define DEBUG_DC					/* device control debug */
//#define DEBUG_MODS					/* access modules debug */
//#define DEBUG_MOD					/* dso module debug		*/

/* view line and file info */
#ifdef DEBUG_VIEW_LINE
#define LINE_FEED fprintf(stderr, "[LINE=%d]@[%s]@[PID=%d]=> ", __LINE__, __FILE__, getpid())
#define CARRIAGE_RETURN fprintf(stderr, "\n")
#else
#define LINE_FEED
#define CARRIAGE_RETURN
#endif

/* error debug */
#ifdef DEBUG_ERR
#define LOG_ERR(x) LINE_FEED;fprintf(stderr, "ERROR : ");fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_ERR(x)
#endif

/* warning debug */
#ifdef DEBUG_WRN
#define LOG_WRN(x) LINE_FEED;fprintf(stderr, "WARN : ");fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_WRN(x)
#endif

/* main debug */
#ifdef DEBUG_MAIN
#define LOG_MAIN(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_MAIN(x)
#endif

/* file access debug */
#ifdef DEBUG_FA
#define LOG_FA(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_FA(x)
#endif

/* http access debug */
#ifdef DEBUG_HP
#define LOG_HP(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_HP(x)
#endif

/* setting debug */
#ifdef DEBUG_ST
#define LOG_ST(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_ST(x)
#endif

/* response debug */
#ifdef DEBUG_RS
#define LOG_RS(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_RS(x)
#endif

/* transfer debug */
#ifdef DEBUG_TF
#define LOG_TF(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_TF(x)
#endif

/* device control */
#ifdef DEBUG_DC
#define LOG_DC(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_DC(x)
#endif

/* access modules debug */
#ifdef DEBUG_MODS
#define LOG_MODS(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_MODS(x)
#endif

/* dso module debug */
#ifdef DEBUG_MOD
#define LOG_MOD(x) LINE_FEED;fprintf(stderr, x);CARRIAGE_RETURN
#else
#define LOG_MOD(x)
#endif


#endif	// __PO_DEBUG_H
