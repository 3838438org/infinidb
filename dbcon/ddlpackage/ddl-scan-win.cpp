/* Copyright (C) 2014 InfiniDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>
#include <unistd.h>

/* Use prototypes in function declarations. */
#define YY_USE_PROTOS

/* The "const" storage-class-modifier is valid. */
#define YY_USE_CONST

#else	/* ! __cplusplus */

#if __STDC__

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif	/* __STDC__ */
#endif	/* ! __cplusplus */

#ifdef __TURBOC__
 #pragma warn -rch
 #pragma warn -use
#include <io.h>
#include <stdlib.h>
#define YY_USE_CONST
#define YY_USE_PROTOS
#endif

#ifdef YY_USE_CONST
#define yyconst const
#else
#define yyconst
#endif


#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif

/* Returned upon end-of-file. */
#define YY_NULL 0

/* Promotes a possibly negative, possibly signed char to an unsigned
 * integer for use as an array index.  If the signed char is negative,
 * we want to instead treat it as an 8-bit unsigned char, hence the
 * double cast.
 */
#define YY_SC_TO_UI(c) ((unsigned int) (unsigned char) c)

/* Enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN.
 */
#define BEGIN yy_start = 1 + 2 *

/* Translate the current start state into a value that can be later handed
 * to BEGIN to return to the state.  The YYSTATE alias is for lex
 * compatibility.
 */
#define YY_START ((yy_start - 1) / 2)
#define YYSTATE YY_START

/* Action number for EOF rule of a given start state. */
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)

/* Special action meaning "start processing a new file". */
#define YY_NEW_FILE yyrestart( yyin )

#define YY_END_OF_BUFFER_CHAR 0

/* Size of default input buffer. */
#define YY_BUF_SIZE 16384

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yyleng;
extern FILE *yyin, *yyout;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2

/* The funky do-while in the following #define is used to turn the definition
 * int a single C statement (which needs a semi-colon terminator).  This
 * avoids problems with code like:
 *
 * 	if ( condition_holds )
 *		yyless( 5 );
 *	else
 *		do_something_else();
 *
 * Prior to using the do-while the compiler would get upset at the
 * "else" because it interpreted the "if" statement as being all
 * done when it reached the ';' after the yyless() call.
 */

/* Return all but the first 'n' matched characters back to the input stream. */

#define yyless(n) \
	do \
		{ \
		/* Undo effects of setting up yytext. */ \
		*yy_cp = yy_hold_char; \
		YY_RESTORE_YY_MORE_OFFSET \
		yy_c_buf_p = yy_cp = yy_bp + n - YY_MORE_ADJ; \
		YY_DO_BEFORE_ACTION; /* set up yytext again */ \
		} \
	while ( 0 )

#define unput(c) yyunput( c, yytext_ptr )

/* The following is because we cannot portably get our hands on size_t
 * (without autoconf's help, which isn't available because we want
 * flex-generated scanners to compile on their own).
 */
typedef unsigned int yy_size_t;


struct yy_buffer_state
	{
	FILE *yy_input_file;

	char *yy_ch_buf;		/* input buffer */
	char *yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	yy_size_t yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	 * characters.
	 */
	int yy_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	 * and can realloc() it to grow it, and should free() it to
	 * delete it.
	 */
	int yy_is_our_buffer;

	/* Whether this is an "interactive" input source; if so, and
	 * if we're using stdio for input, then we want to use getc()
	 * instead of fread(), to make sure we stop fetching input after
	 * each newline.
	 */
	int yy_is_interactive;

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	int yy_at_bol;

	/* Whether to try to fill the input buffer when we reach the
	 * end of it.
	 */
	int yy_fill_buffer;

	int yy_buffer_status;
#define YY_BUFFER_NEW 0
#define YY_BUFFER_NORMAL 1
	/* When an EOF's been seen but there's still some text to process
	 * then we mark the buffer as YY_EOF_PENDING, to indicate that we
	 * shouldn't try reading from the input source any more.  We might
	 * still have a bunch of tokens to match, though, because of
	 * possible backing-up.
	 *
	 * When we actually see the EOF, we change the status to "new"
	 * (via yyrestart()), so that the user can continue scanning by
	 * just pointing yyin at a new input file.
	 */
#define YY_BUFFER_EOF_PENDING 2
	};

static YY_BUFFER_STATE yy_current_buffer = 0;

/* We provide macros for accessing buffer states in case in the
 * future we want to put the buffer states in a more general
 * "scanner state".
 */
#define YY_CURRENT_BUFFER yy_current_buffer


/* yy_hold_char holds the character lost when yytext is formed. */
static char yy_hold_char;

static int yy_n_chars;		/* number of characters read into yy_ch_buf */


int yyleng;

/* Points to current character in buffer. */
static char *yy_c_buf_p = (char *) 0;
static int yy_init = 1;		/* whether we need to initialize */
static int yy_start = 0;	/* start state number */

/* Flag which is used to allow yywrap()'s to do buffer switches
 * instead of setting up a fresh yyin.  A bit of a hack ...
 */
static int yy_did_buffer_switch_on_eof;

void yyrestart YY_PROTO(( FILE *input_file ));

void yy_switch_to_buffer YY_PROTO(( YY_BUFFER_STATE new_buffer ));
void yy_load_buffer_state YY_PROTO(( void ));
YY_BUFFER_STATE yy_create_buffer YY_PROTO(( FILE *file, int size ));
void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));
void yy_init_buffer YY_PROTO(( YY_BUFFER_STATE b, FILE *file ));
void yy_flush_buffer YY_PROTO(( YY_BUFFER_STATE b ));
#define YY_FLUSH_BUFFER yy_flush_buffer( yy_current_buffer )

YY_BUFFER_STATE yy_scan_buffer YY_PROTO(( char *base, yy_size_t size ));
YY_BUFFER_STATE yy_scan_string YY_PROTO(( yyconst char *yy_str ));
YY_BUFFER_STATE yy_scan_bytes YY_PROTO(( yyconst char *bytes, int len ));

static void *yy_flex_alloc YY_PROTO(( yy_size_t ));
static void *yy_flex_realloc YY_PROTO(( void *, yy_size_t ));
static void yy_flex_free YY_PROTO(( void * ));

#define yy_new_buffer yy_create_buffer

#define yy_set_interactive(is_interactive) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_is_interactive = is_interactive; \
	}

#define yy_set_bol(at_bol) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_at_bol = at_bol; \
	}

#define YY_AT_BOL() (yy_current_buffer->yy_at_bol)


#define yywrap() 1
#define YY_SKIP_YYWRAP
typedef unsigned char YY_CHAR;
FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;
typedef int yy_state_type;
extern char *yytext;
#define yytext_ptr yytext

static yy_state_type yy_get_previous_state YY_PROTO(( void ));
static yy_state_type yy_try_NUL_trans YY_PROTO(( yy_state_type current_state ));
static int yy_get_next_buffer YY_PROTO(( void ));
static void yy_fatal_error YY_PROTO(( yyconst char msg[] ));

/* Done after the current pattern has been matched and before the
 * corresponding action - sets up yytext.
 */
#define YY_DO_BEFORE_ACTION \
	yytext_ptr = yy_bp; \
	yyleng = (int) (yy_cp - yy_bp); \
	yy_hold_char = *yy_cp; \
	*yy_cp = '\0'; \
	yy_c_buf_p = yy_cp;

#define YY_NUM_RULES 84
#define YY_END_OF_BUFFER 85
static yyconst short int yy_accept[386] =
    {   0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       85,   84,   80,   79,   82,   13,   82,   82,   82,   16,
       81,   83,   81,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   81,   81,   81,   81,   81,   81,   81,   10,
       12,   84,   11,   84,   14,   15,   80,   16,   80,   17,
       17,   16,    0,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   81,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   81,   74,   81,   81,   81,   81,   81,   81,
       48,   81,   52,   81,   81,   81,   81,   81,   81,   81,
       81,   65,   81,   81,   81,   81,   81,    0,   11,    0,

       14,   80,   17,    0,   18,   81,    2,   81,   81,   81,
       81,   81,   81,   81,   81,   81,   81,   81,   81,   29,
       81,   81,   81,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   41,   43,   81,   81,   81,   81,   49,   81,
       81,   81,   81,   81,   81,   81,   81,   81,   81,   61,
       81,   81,   81,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   81,   81,   81,    6,   81,   81,   81,   81,
       81,   81,   26,   81,   81,   81,   81,   81,   34,   81,
       81,   81,   81,   37,   81,   81,   81,   81,   81,   81,
       81,   81,   50,   81,   81,   81,   81,   72,   81,   81,

       81,   81,   81,   81,   81,   81,   81,   81,   81,   68,
       81,   81,   81,    3,   81,   81,   81,   81,   81,   81,
        9,   81,   81,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   81,   81,   70,   81,   81,   39,   81,   81,
       44,   81,   81,   81,   81,   81,   81,   81,   81,   81,
       81,   81,   81,   81,   63,   81,   81,   81,   81,   81,
       81,    1,   81,    8,   81,   76,   81,   81,   20,   81,
       81,   24,   81,   81,   81,   81,   81,   33,   71,   35,
       75,   81,   81,   81,   81,   81,   81,   47,   81,   81,
       81,   81,   81,   57,   81,   81,   81,   81,   81,   81,

       66,   67,   81,   81,   81,    5,   81,   73,   21,   19,
       81,   81,   81,   28,   30,   81,   81,   36,   81,   81,
       42,   81,   81,   51,   53,   81,   55,   81,   81,   81,
       81,   81,   64,   81,   81,   69,   81,   81,   81,   81,
       27,   81,   32,   81,   81,   45,   46,   81,   81,   58,
       81,   62,   81,   77,   81,   81,    7,   81,   81,   81,
       38,   40,   54,   81,   81,   81,   78,   81,   22,   81,
       31,   56,   81,   81,   81,   23,   81,   81,   60,   81,
       25,   59,   81,    4,    0
    } ;

static yyconst int yy_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    2,    4,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    2,    1,    1,    1,    5,    6,    1,    7,    8,
        9,    6,   10,    6,   11,   12,    6,   13,   13,   13,
       13,   13,   13,   13,   13,   13,   13,    6,    6,    6,
        6,    6,    1,    1,   16,   17,   18,   19,   20,   21,
       22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
       32,   33,   34,   35,   36,   37,   38,   39,   40,   25,
        6,    1,    6,    6,   14,   15,   16,   17,   18,   19,

       20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
       30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
       40,   25,    1,    1,    1,    1,    1,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,

       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25
    } ;

static yyconst int yy_meta[41] =
    {   0,
        1,    1,    2,    2,    3,    1,    1,    1,    1,    1,
        1,    1,    3,    3,    1,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3
    } ;

static yyconst short int yy_base[393] =
    {   0,
        0,    0,   33,   35,  418,  417,  418,  417,  416,  415,
      421,  428,   43,   46,  428,  428,  407,   40,  406,   42,
        0,  428,   38,  394,   36,   47,   29,   34,   50,  397,
       57,   46,  387,   59,  395,   63,   69,   55,  398,  428,
      428,  404,  428,  405,  428,  428,   92,   77,    0,   85,
       87,   96,  100,    0,  376,  391,  374,  373,  385,  372,
       98,   92,  385,  371,  368,  104,  366,  371,  378,  375,
      368,  364,  369,    0,  367,   82,  354,   88,  364,  373,
      356,  101,    0,  357,  106,  116,   99,  373,  354,  370,
      357,    0,  349,  360,  364,  362,  348,  371,  428,  372,

      428,    0,  122,  365,  364,  352,    0,  355,  344,  349,
      354,  107,  353,  334,  341,  334,  351,  333,  345,  340,
      123,  343,  345,  330,  336,  325,  342,  337,  329,  335,
      334,  318,  332,    0,  333,  336,  335,  324,    0,  320,
      326,  310,  326,  315,  315,  321,  324,  304,  304,    0,
      310,  301,  308,  294,  304,  300,  315,  297,  129,  299,
      295,  313,  297,  309,  302,  122,  297,  294,  301,  285,
      284,  298,  282,  288,  279,  281,  278,  285,    0,  282,
      275,  274,  284,    0,  288,  267,  281,  282,  280,  269,
      268,  279,    0,  266,  274,  273,  280,    0,  262,  266,

      260,  268,  264,  270,  269,  264,  269,  250,  250,    0,
      260,  260,  253,    0,  257,  245,  260,  258,  259,  256,
        0,  246,  245,  240,  252,  242,  246,  253,  241,  234,
      246,  245,  244,  229,    0,  240,  237,    0,  244,  239,
        0,  228,  227,  216,  231,  238,  219,  219,  231,  230,
      225,  218,  223,  218,    0,  216,  228,  223,  222,  212,
      224,    0,  210,    0,  218,    0,  202,  201,  201,  199,
      217,    0,  197,  203,  203,  194,  128,    0,    0,    0,
        0,  199,  211,  199,  192,  186,  185,    0,  204,  194,
      196,  179,  189,    0,  199,  187,  186,  200,  178,  177,

        0,    0,  195,  177,  191,    0,  188,    0,    0,    0,
      183,  192,  185,    0,    0,  187,  184,    0,  167,  174,
        0,  166,  165,    0,    0,  168,    0,  179,  161,  181,
      159,  157,    0,  172,  158,    0,  157,  156,  159,  151,
        0,  159,    0,  165,  144,    0,    0,  154,  162,    0,
      145,    0,  146,    0,  139,  158,    0,  142,  142,  155,
        0,    0,    0,  139,  121,  134,    0,  125,  118,  131,
        0,    0,  129,  108,  104,    0,   82,   79,    0,   75,
        0,    0,   53,    0,  428,  156,  159,  162,   57,  165,
      168,  171

    } ;

static yyconst short int yy_def[393] =
    {   0,
      385,    1,  386,  386,  387,  387,  388,  388,  386,  386,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      389,  385,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  385,
      385,  390,  385,  391,  385,  385,  385,  385,  392,  385,
      385,  385,  385,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  390,  385,  391,

      385,  392,  385,  385,  385,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,

      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,

      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
      389,  389,  389,  389,    0,  385,  385,  385,  385,  385,
      385,  385

    } ;

static yyconst short int yy_nxt[469] =
    {   0,
       12,   13,   14,   13,   12,   15,   16,   15,   15,   17,
       18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
       28,   21,   21,   29,   21,   30,   21,   31,   32,   33,
       34,   21,   35,   36,   37,   38,   39,   21,   21,   21,
       40,   41,   40,   41,   47,   47,   47,   47,   47,   47,
       49,   60,   48,   51,   52,   55,   56,   69,   61,   54,
       71,   53,   65,   72,   57,   62,   66,   70,   63,   73,
       74,   64,   78,   58,   84,   81,   67,   75,   76,   68,
       79,   82,   87,   94,   90,   95,   80,  384,   96,   48,
       88,   85,   91,   47,   47,   47,   53,   50,   92,  103,

      131,   93,   89,  383,   53,  132,   53,   51,   52,  104,
      104,  382,  105,  112,  381,   53,  133,  113,  114,  115,
      116,  120,  135,  380,  121,  143,  136,  140,  141,  144,
      122,  145,  149,  150,  103,  165,  146,  219,  175,  166,
      379,   53,  176,  316,  147,  211,  212,  317,  378,  148,
      377,  376,  375,  374,  373,  220,   12,   12,   12,   42,
       42,   42,   44,   44,   44,   98,   98,   98,  100,  100,
      100,  102,  372,  102,  371,  370,  369,  368,  367,  366,
      365,  364,  363,  362,  361,  360,  359,  358,  357,  356,
      355,  354,  353,  352,  351,  350,  349,  348,  347,  346,

      345,  344,  343,  342,  341,  340,  339,  338,  337,  336,
      335,  334,  333,  332,  331,  330,  329,  328,  327,  326,
      325,  324,  323,  322,  321,  320,  319,  318,  315,  314,
      313,  312,  311,  310,  309,  308,  307,  306,  305,  304,
      303,  302,  301,  300,  299,  298,  297,  296,  295,  294,
      293,  292,  291,  290,  289,  288,  287,  286,  285,  284,
      283,  282,  281,  280,  279,  278,  277,  276,  275,  274,
      273,  272,  271,  270,  269,  268,  267,  266,  265,  264,
      263,  262,  261,  260,  259,  258,  257,  256,  255,  254,
      253,  252,  251,  250,  249,  248,  247,  246,  245,  244,

      243,  242,  241,  240,  239,  238,  237,  236,  235,  234,
      233,  232,  231,  230,  229,  228,  227,  226,  225,  224,
      223,  222,  221,  218,  217,  216,  215,  214,  213,  210,
      209,  208,  207,  206,  205,  204,  203,  202,  201,  200,
      199,  198,  197,  196,  195,  194,  193,  192,  191,  190,
      189,  188,  187,  186,  185,  184,  183,  182,  181,  180,
      179,  178,  177,  174,  173,  172,  171,  170,  169,  168,
      167,  164,  163,  162,  161,  160,  105,  105,  101,   99,
      159,  158,  157,  156,  155,  154,  153,  152,  151,  142,
      139,  138,  137,  134,  130,  129,  128,  127,  126,  125,

      124,  123,  119,  118,  117,  111,  110,  109,  108,  107,
      106,  101,   99,   97,   86,   83,   77,   59,   50,   48,
      385,   46,   46,   45,   45,   43,   43,   11,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385
    } ;

static yyconst short int yy_chk[469] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        3,    3,    4,    4,   13,   13,   13,   14,   14,   14,
       18,   25,   18,   20,   20,   23,   23,   27,   25,  389,
       28,   20,   26,   28,   23,   25,   26,   27,   25,   28,
       29,   25,   31,   23,   34,   32,   26,   29,   29,   26,
       31,   32,   36,   38,   37,   38,   31,  383,   38,   48,
       36,   34,   37,   47,   47,   47,   48,   50,   37,   51,

       76,   37,   36,  380,   50,   76,   51,   52,   52,   53,
       53,  378,   53,   61,  377,   52,   76,   61,   62,   62,
       62,   66,   78,  375,   66,   85,   78,   82,   82,   85,
       66,   86,   87,   87,  103,  112,   86,  166,  121,  112,
      374,  103,  121,  277,   86,  159,  159,  277,  373,   86,
      370,  369,  368,  366,  365,  166,  386,  386,  386,  387,
      387,  387,  388,  388,  388,  390,  390,  390,  391,  391,
      391,  392,  364,  392,  360,  359,  358,  356,  355,  353,
      351,  349,  348,  345,  344,  342,  340,  339,  338,  337,
      335,  334,  332,  331,  330,  329,  328,  326,  323,  322,

      320,  319,  317,  316,  313,  312,  311,  307,  305,  304,
      303,  300,  299,  298,  297,  296,  295,  293,  292,  291,
      290,  289,  287,  286,  285,  284,  283,  282,  276,  275,
      274,  273,  271,  270,  269,  268,  267,  265,  263,  261,
      260,  259,  258,  257,  256,  254,  253,  252,  251,  250,
      249,  248,  247,  246,  245,  244,  243,  242,  240,  239,
      237,  236,  234,  233,  232,  231,  230,  229,  228,  227,
      226,  225,  224,  223,  222,  220,  219,  218,  217,  216,
      215,  213,  212,  211,  209,  208,  207,  206,  205,  204,
      203,  202,  201,  200,  199,  197,  196,  195,  194,  192,

      191,  190,  189,  188,  187,  186,  185,  183,  182,  181,
      180,  178,  177,  176,  175,  174,  173,  172,  171,  170,
      169,  168,  167,  165,  164,  163,  162,  161,  160,  158,
      157,  156,  155,  154,  153,  152,  151,  149,  148,  147,
      146,  145,  144,  143,  142,  141,  140,  138,  137,  136,
      135,  133,  132,  131,  130,  129,  128,  127,  126,  125,
      124,  123,  122,  120,  119,  118,  117,  116,  115,  114,
      113,  111,  110,  109,  108,  106,  105,  104,  100,   98,
       97,   96,   95,   94,   93,   91,   90,   89,   88,   84,
       81,   80,   79,   77,   75,   73,   72,   71,   70,   69,

       68,   67,   65,   64,   63,   60,   59,   58,   57,   56,
       55,   44,   42,   39,   35,   33,   30,   24,   19,   17,
       11,   10,    9,    8,    7,    6,    5,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
      385,  385,  385,  385,  385,  385,  385,  385
    } ;

static yy_state_type yy_last_accepting_state;
static char *yy_last_accepting_cpos;

/* The intent behind this definition is that it'll catch
 * any uses of REJECT which flex missed.
 */
#define REJECT reject_used_but_not_detected
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0
#define YY_RESTORE_YY_MORE_OFFSET
char *yytext;
#define INITIAL 0
/*

Copyright (C) 2009-2012 Calpont Corporation.

Use of and access to the Calpont InfiniDB Community software is subject to the
terms and conditions of the Calpont Open Source License Agreement. Use of and
access to the Calpont InfiniDB Enterprise software is subject to the terms and
conditions of the Calpont End User License Agreement.

This program is distributed in the hope that it will be useful, and unless
otherwise noted on your license agreement, WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Please refer to the Calpont Open Source License Agreement and the Calpont End
User License Agreement for more details.

You should have received a copy of either the Calpont Open Source License
Agreement or the Calpont End User License Agreement along with this program; if
not, it is your responsibility to review the terms and conditions of the proper
Calpont license agreement by visiting http://www.calpont.com for the Calpont
InfiniDB Enterprise End User License Agreement or http://www.infinidb.org for
the Calpont InfiniDB Community Calpont Open Source License Agreement.

Calpont may make changes to these license agreements from time to time. When
these changes are made, Calpont will make a new copy of the Calpont End User
License Agreement available at http://www.calpont.com and a new copy of the
Calpont Open Source License Agreement available at http:///www.infinidb.org.
You understand and agree that if you use the Program after the date on which
the license agreement authorizing your use has changed, Calpont will treat your
use as acceptance of the updated License.

*/
/* $Id: ddl.l 8618 2012-06-07 22:27:04Z chao $ */
#include <iostream>
#include <vector>
#include <stdio.h>
#include "ddlpkg.h"

#ifdef _MSC_VER
#include "ddl-gram-win.h"
#else
#include "ddl-gram.h"
#endif

int lineno = 1;
void ddlerror(char *s);

/* Handles to the buffer that the lexer uses internally */
static YY_BUFFER_STATE scanbufhandle;
static char *scanbuf;

static char* scanner_copy (char *str);

#define YY_NO_UNPUT 1
/* %option header-file="ddl-scan.h" */
#define check1 1

#define check2 2

#define inquote 3

#define endquote 4


/* Macros after this point can all be overridden by user definitions in
 * section 1.
 */

#ifndef YY_SKIP_YYWRAP
#ifdef __cplusplus
extern "C" int yywrap YY_PROTO(( void ));
#else
extern int yywrap YY_PROTO(( void ));
#endif
#endif

#ifndef YY_NO_UNPUT
static void yyunput YY_PROTO(( int c, char *buf_ptr ));
#endif

#ifndef yytext_ptr
static void yy_flex_strncpy YY_PROTO(( char *, yyconst char *, int ));
#endif

#ifdef YY_NEED_STRLEN
static int yy_flex_strlen YY_PROTO(( yyconst char * ));
#endif

#ifndef YY_NO_INPUT
#ifdef __cplusplus
static int yyinput YY_PROTO(( void ));
#else
static int input YY_PROTO(( void ));
#endif
#endif

#if YY_STACK_USED
static int yy_start_stack_ptr = 0;
static int yy_start_stack_depth = 0;
static int *yy_start_stack = 0;
#ifndef YY_NO_PUSH_STATE
static void yy_push_state YY_PROTO(( int new_state ));
#endif
#ifndef YY_NO_POP_STATE
static void yy_pop_state YY_PROTO(( void ));
#endif
#ifndef YY_NO_TOP_STATE
static int yy_top_state YY_PROTO(( void ));
#endif

#else
#define YY_NO_PUSH_STATE 1
#define YY_NO_POP_STATE 1
#define YY_NO_TOP_STATE 1
#endif

#ifdef YY_MALLOC_DECL
YY_MALLOC_DECL
#else
#if __STDC__
#ifndef __cplusplus
#include <stdlib.h>
#endif
#else
/* Just try to get by without declaring the routines.  This will fail
 * miserably on non-ANSI systems for which sizeof(size_t) != sizeof(int)
 * or sizeof(void*) != sizeof(int).
 */
#endif
#endif

/* Amount of stuff to slurp up with each read. */
#ifndef YY_READ_BUF_SIZE
#define YY_READ_BUF_SIZE 8192
#endif

/* Copy whatever the last rule matched to the standard output. */

#ifndef ECHO
/* This used to be an fputs(), but since the string might contain NUL's,
 * we now use fwrite().
 */
#define ECHO (void) fwrite( yytext, yyleng, 1, yyout )
#endif

/* Gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
 * is returned in "result".
 */
#ifndef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( yy_current_buffer->yy_is_interactive ) \
		{ \
		int c = '*', n; \
		for ( n = 0; n < max_size && \
			     (c = getc( yyin )) != EOF && c != '\n'; ++n ) \
			buf[n] = (char) c; \
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		if ( c == EOF && ferror( yyin ) ) \
			YY_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
		} \
	else if ( ((result = fread( buf, 1, max_size, yyin )) == 0) \
		  && ferror( yyin ) ) \
		YY_FATAL_ERROR( "input in flex scanner failed" );
#endif

/* No semi-colon after return; correct usage is to write "yyterminate();" -
 * we don't want an extra ';' after the "return" because that will cause
 * some compilers to complain about unreachable statements.
 */
#ifndef yyterminate
#define yyterminate() return YY_NULL
#endif

/* Number of entries by which start-condition stack grows. */
#ifndef YY_START_STACK_INCR
#define YY_START_STACK_INCR 25
#endif

/* Report a fatal error. */
#ifndef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) yy_fatal_error( msg )
#endif

/* Default declaration of generated scanner - a define so the user can
 * easily add parameters.
 */
#ifndef YY_DECL
#define YY_DECL int yylex YY_PROTO(( void ))
#endif

/* Code executed at the beginning of each rule, after yytext and yyleng
 * have been set up.
 */
#ifndef YY_USER_ACTION
#define YY_USER_ACTION
#endif

/* Code executed at the end of each rule. */
#ifndef YY_BREAK
#define YY_BREAK break;
#endif

#define YY_RULE_SETUP \
	YY_USER_ACTION

YY_DECL
	{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;




	if ( yy_init )
		{
		yy_init = 0;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! yy_start )
			yy_start = 1;	/* first start state */

		if ( ! yyin )
			yyin = stdin;

		if ( ! yyout )
			yyout = stdout;

		if ( ! yy_current_buffer )
			yy_current_buffer =
				yy_create_buffer( yyin, YY_BUF_SIZE );

		yy_load_buffer_state();
		}

	while ( 1 )		/* loops until end-of-file is reached */
		{
		yy_cp = yy_c_buf_p;

		/* Support of yytext. */
		*yy_cp = yy_hold_char;

		/* yy_bp points to the position in yy_ch_buf of the start of
		 * the current run.
		 */
		yy_bp = yy_cp;

		yy_current_state = yy_start;
yy_match:
		do
			{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			if ( yy_accept[yy_current_state] )
				{
				yy_last_accepting_state = yy_current_state;
				yy_last_accepting_cpos = yy_cp;
				}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 386 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 428 );

yy_find_action:
		yy_act = yy_accept[yy_current_state];
		if ( yy_act == 0 )
			{ /* have to back up */
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			yy_act = yy_accept[yy_current_state];
			}

		YY_DO_BEFORE_ACTION;


do_action:	/* This label is used only to access EOF actions. */


		switch ( yy_act )
	{ /* beginning of action switch */
			case 0: /* must back up */
			/* undo the effects of YY_DO_BEFORE_ACTION */
			*yy_cp = yy_hold_char;
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			goto yy_find_action;

case 1:
YY_RULE_SETUP
{return ACTION;}
	YY_BREAK
case 2:
YY_RULE_SETUP
{return ADD;}
	YY_BREAK
case 3:
YY_RULE_SETUP
{return ALTER;}
	YY_BREAK
case 4:
YY_RULE_SETUP
{return AUTO_INCREMENT;}
	YY_BREAK
case 5:
YY_RULE_SETUP
{return CASCADE;}
	YY_BREAK
case 6:
YY_RULE_SETUP
{return IDB_CHAR;}
	YY_BREAK
case 7:
YY_RULE_SETUP
{return IDB_CHAR;}
	YY_BREAK
case 8:
YY_RULE_SETUP
{return BIGINT;}
	YY_BREAK
case 9:
YY_RULE_SETUP
{BEGIN(check1);return CHECK;}
	YY_BREAK
case 10:
YY_RULE_SETUP
{BEGIN(check2); return '(';}
	YY_BREAK
case 11:
*yy_cp = yy_hold_char; /* undo effects of setting up yytext */
yy_c_buf_p = yy_cp -= 1;
YY_DO_BEFORE_ACTION; /* set up yytext again */
YY_RULE_SETUP
{BEGIN(check1); ddllval.str = scanner_copy(ddltext); return CP_SEARCH_CONDITION_TEXT;}
	YY_BREAK
case 12:
YY_RULE_SETUP
{BEGIN(0); return ')';}
	YY_BREAK
case 13:
YY_RULE_SETUP
{BEGIN(inquote);return yytext[0];}
	YY_BREAK
case 14:
*yy_cp = yy_hold_char; /* undo effects of setting up yytext */
yy_c_buf_p = yy_cp -= 1;
YY_DO_BEFORE_ACTION; /* set up yytext again */
YY_RULE_SETUP
{BEGIN(endquote); ddllval.str = scanner_copy(ddltext); return SCONST;}
	YY_BREAK
case 15:
YY_RULE_SETUP
{BEGIN(0); return yytext[0];}
	YY_BREAK
case 16:
YY_RULE_SETUP
{ddllval.str = scanner_copy(ddltext); return ICONST;}
	YY_BREAK
case 17:
YY_RULE_SETUP
{ddllval.str = scanner_copy(ddltext); return FCONST;}
	YY_BREAK
case 18:
YY_RULE_SETUP
{ddllval.str = scanner_copy(ddltext); return FCONST;}
	YY_BREAK
case 19:
YY_RULE_SETUP
{return COMMENT;}
	YY_BREAK
case 20:
YY_RULE_SETUP
{return COLUMN;}
	YY_BREAK
case 21:
YY_RULE_SETUP
{return COLUMNS;}
	YY_BREAK
case 22:
YY_RULE_SETUP
{return CONSTRAINT;}
	YY_BREAK
case 23:
YY_RULE_SETUP
{return CONSTRAINTS;}
	YY_BREAK
case 24:
YY_RULE_SETUP
{return CREATE;}
	YY_BREAK
case 25:
YY_RULE_SETUP
{return CURRENT_USER;}
	YY_BREAK
case 26:
YY_RULE_SETUP
{ddllval.str=strdup("date"); return DATE;}
	YY_BREAK
case 27:
YY_RULE_SETUP
{return DATETIME;}
	YY_BREAK
case 28:
YY_RULE_SETUP
{return DECIMAL;}
	YY_BREAK
case 29:
YY_RULE_SETUP
{return DECIMAL;}
	YY_BREAK
case 30:
YY_RULE_SETUP
{return DEFAULT;}
	YY_BREAK
case 31:
YY_RULE_SETUP
{return DEFERRABLE;}
	YY_BREAK
case 32:
YY_RULE_SETUP
{return DEFERRED;}
	YY_BREAK
case 33:
YY_RULE_SETUP
{return IDB_DELETE;}
	YY_BREAK
case 34:
YY_RULE_SETUP
{return DROP;}
	YY_BREAK
case 35:
YY_RULE_SETUP
{return ENGINE;}
	YY_BREAK
case 36:
YY_RULE_SETUP
{return FOREIGN;}
	YY_BREAK
case 37:
YY_RULE_SETUP
{return FULL;}
	YY_BREAK
case 38:
YY_RULE_SETUP
{return IMMEDIATE;}
	YY_BREAK
case 39:
YY_RULE_SETUP
{return INDEX;}
	YY_BREAK
case 40:
YY_RULE_SETUP
{return INITIALLY;}
	YY_BREAK
case 41:
YY_RULE_SETUP
{return IDB_INT;}
	YY_BREAK
case 42:
YY_RULE_SETUP
{return INTEGER;}
	YY_BREAK
case 43:
YY_RULE_SETUP
{return KEY;}
	YY_BREAK
case 44:
YY_RULE_SETUP
{return MATCH;}
	YY_BREAK
case 45:
YY_RULE_SETUP
{return MAX_ROWS;}
	YY_BREAK
case 46:
YY_RULE_SETUP
{return MIN_ROWS;}
	YY_BREAK
case 47:
YY_RULE_SETUP
{return MODIFY;}
	YY_BREAK
case 48:
YY_RULE_SETUP
{return NO;}
	YY_BREAK
case 49:
YY_RULE_SETUP
{return NOT;}
	YY_BREAK
case 50:
YY_RULE_SETUP
{return NULL_TOK;}
	YY_BREAK
case 51:
YY_RULE_SETUP
{return NUMERIC;}
	YY_BREAK
case 52:
YY_RULE_SETUP
{return ON;}
	YY_BREAK
case 53:
YY_RULE_SETUP
{return PARTIAL;}
	YY_BREAK
case 54:
YY_RULE_SETUP
{return PRECISION;}
	YY_BREAK
case 55:
YY_RULE_SETUP
{return PRIMARY;}
	YY_BREAK
case 56:
YY_RULE_SETUP
{return REFERENCES;}
	YY_BREAK
case 57:
YY_RULE_SETUP
{return RENAME;}
	YY_BREAK
case 58:
YY_RULE_SETUP
{return RESTRICT;}
	YY_BREAK
case 59:
YY_RULE_SETUP
{return SESSION_USER;}
	YY_BREAK
case 60:
YY_RULE_SETUP
{return SYSTEM_USER;}
	YY_BREAK
case 61:
YY_RULE_SETUP
{return SET;}
	YY_BREAK
case 62:
YY_RULE_SETUP
{return SMALLINT;}
	YY_BREAK
case 63:
YY_RULE_SETUP
{return TABLE;}
	YY_BREAK
case 64:
YY_RULE_SETUP
{return TINYINT;}
	YY_BREAK
case 65:
YY_RULE_SETUP
{return TO;}
	YY_BREAK
case 66:
YY_RULE_SETUP
{return UNIQUE;}
	YY_BREAK
case 67:
YY_RULE_SETUP
{return UPDATE;}
	YY_BREAK
case 68:
YY_RULE_SETUP
{return USER;}
	YY_BREAK
case 69:
YY_RULE_SETUP
{return VARCHAR;}
	YY_BREAK
case 70:
YY_RULE_SETUP
{return IDB_FLOAT;}
	YY_BREAK
case 71:
YY_RULE_SETUP
{return DOUBLE;}
	YY_BREAK
case 72:
YY_RULE_SETUP
{return REAL;}
	YY_BREAK
case 73:
YY_RULE_SETUP
{return CHARSET;}
	YY_BREAK
case 74:
YY_RULE_SETUP
{return IF;}
	YY_BREAK
case 75:
YY_RULE_SETUP
{return EXISTS;}
	YY_BREAK
case 76:
YY_RULE_SETUP
{return CHANGE;}
	YY_BREAK
case 77:
YY_RULE_SETUP
{return TRUNCATE;}
	YY_BREAK
case 78:
YY_RULE_SETUP
{return VARBINARY;}
	YY_BREAK
case 79:
YY_RULE_SETUP
{ lineno++;}
	YY_BREAK
case 80:
YY_RULE_SETUP
{
    /* ignore */
}
	YY_BREAK
case 81:
YY_RULE_SETUP
{ddllval.str = scanner_copy(ddltext); return IDENT;}
	YY_BREAK
case 82:
YY_RULE_SETUP
{
    return ddltext[0];
}
	YY_BREAK
case 83:
YY_RULE_SETUP
{
    /* ignore */
}
	YY_BREAK
case 84:
YY_RULE_SETUP
ECHO;
	YY_BREAK
case YY_STATE_EOF(INITIAL):
case YY_STATE_EOF(check1):
case YY_STATE_EOF(check2):
case YY_STATE_EOF(inquote):
case YY_STATE_EOF(endquote):
	yyterminate();

	case YY_END_OF_BUFFER:
		{
		/* Amount of text matched not including the EOB char. */
		int yy_amount_of_matched_text = (int) (yy_cp - yytext_ptr) - 1;

		/* Undo the effects of YY_DO_BEFORE_ACTION. */
		*yy_cp = yy_hold_char;
		YY_RESTORE_YY_MORE_OFFSET

		if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_NEW )
			{
			/* We're scanning a new file or input source.  It's
			 * possible that this happened because the user
			 * just pointed yyin at a new source and called
			 * yylex().  If so, then we have to assure
			 * consistency between yy_current_buffer and our
			 * globals.  Here is the right place to do so, because
			 * this is the first action (other than possibly a
			 * back-up) that will match for the new input source.
			 */
			yy_n_chars = yy_current_buffer->yy_n_chars;
			yy_current_buffer->yy_input_file = yyin;
			yy_current_buffer->yy_buffer_status = YY_BUFFER_NORMAL;
			}

		/* Note that here we test for yy_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since yy_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the
		 * end-of-buffer state).  Contrast this with the test
		 * in input().
		 */
		if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			{ /* This was really a NUL. */
			yy_state_type yy_next_state;

			yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			/* Okay, we're now positioned to make the NUL
			 * transition.  We couldn't have
			 * yy_get_previous_state() go ahead and do it
			 * for us because it doesn't know how to deal
			 * with the possibility of jamming (and we don't
			 * want to build jamming into it because then it
			 * will run more slowly).
			 */

			yy_next_state = yy_try_NUL_trans( yy_current_state );

			yy_bp = yytext_ptr + YY_MORE_ADJ;

			if ( yy_next_state )
				{
				/* Consume the NUL. */
				yy_cp = ++yy_c_buf_p;
				yy_current_state = yy_next_state;
				goto yy_match;
				}

			else
				{
				yy_cp = yy_c_buf_p;
				goto yy_find_action;
				}
			}

		else switch ( yy_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
				yy_did_buffer_switch_on_eof = 0;

				if ( yywrap() )
					{
					/* Note: because we've taken care in
					 * yy_get_next_buffer() to have set up
					 * yytext, we can now set up
					 * yy_c_buf_p so that if some total
					 * hoser (like flex itself) wants to
					 * call the scanner after we return the
					 * YY_NULL, it'll still work - another
					 * YY_NULL will get returned.
					 */
					yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
					}

				else
					{
					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				yy_c_buf_p =
					yytext_ptr + yy_amount_of_matched_text;

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_match;

			case EOB_ACT_LAST_MATCH:
				yy_c_buf_p =
				&yy_current_buffer->yy_ch_buf[yy_n_chars];

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_find_action;
			}
		break;
		}

	default:
		YY_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	} /* end of action switch */
		} /* end of scanning one token */
	} /* end of yylex */


/* yy_get_next_buffer - try to read in a new buffer
 *
 * Returns a code representing an action:
 *	EOB_ACT_LAST_MATCH -
 *	EOB_ACT_CONTINUE_SCAN - continue scanning from current position
 *	EOB_ACT_END_OF_FILE - end of file
 */

static int yy_get_next_buffer()
	{
	register char *dest = yy_current_buffer->yy_ch_buf;
	register char *source = yytext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
		YY_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

	if ( yy_current_buffer->yy_fill_buffer == 0 )
		{ /* Don't try to fill the buffer, so this is an EOF. */
		if ( yy_c_buf_p - yytext_ptr - YY_MORE_ADJ == 1 )
			{
			/* We matched a single character, the EOB, so
			 * treat this as a final EOF.
			 */
			return EOB_ACT_END_OF_FILE;
			}

		else
			{
			/* We matched some text prior to the EOB, first
			 * process it.
			 */
			return EOB_ACT_LAST_MATCH;
			}
		}

	/* Try to read more data. */

	/* First move last chars to start of buffer. */
	number_to_move = (int) (yy_c_buf_p - yytext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_EOF_PENDING )
		/* don't do the read, it's not guaranteed to return an EOF,
		 * just force an EOF
		 */
		yy_current_buffer->yy_n_chars = yy_n_chars = 0;

	else
		{
		int num_to_read =
			yy_current_buffer->yy_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
			{ /* Not enough room in the buffer - grow it. */
#ifdef YY_USES_REJECT
			YY_FATAL_ERROR(
"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			/* just a shorter name for the current buffer */
			YY_BUFFER_STATE b = yy_current_buffer;

			int yy_c_buf_p_offset =
				(int) (yy_c_buf_p - b->yy_ch_buf);

			if ( b->yy_is_our_buffer )
				{
				int new_size = b->yy_buf_size * 2;

				if ( new_size <= 0 )
					b->yy_buf_size += b->yy_buf_size / 8;
				else
					b->yy_buf_size *= 2;

				b->yy_ch_buf = (char *)
					/* Include room in for 2 EOB chars. */
					yy_flex_realloc( (void *) b->yy_ch_buf,
							 b->yy_buf_size + 2 );
				}
			else
				/* Can't grow it, we don't own it. */
				b->yy_ch_buf = 0;

			if ( ! b->yy_ch_buf )
				YY_FATAL_ERROR(
				"fatal error - scanner input buffer overflow" );

			yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

			num_to_read = yy_current_buffer->yy_buf_size -
						number_to_move - 1;
#endif
			}

		if ( num_to_read > YY_READ_BUF_SIZE )
			num_to_read = YY_READ_BUF_SIZE;

		/* Read in more data. */
		YY_INPUT( (&yy_current_buffer->yy_ch_buf[number_to_move]),
			yy_n_chars, num_to_read );

		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	if ( yy_n_chars == 0 )
		{
		if ( number_to_move == YY_MORE_ADJ )
			{
			ret_val = EOB_ACT_END_OF_FILE;
			yyrestart( yyin );
			}

		else
			{
			ret_val = EOB_ACT_LAST_MATCH;
			yy_current_buffer->yy_buffer_status =
				YY_BUFFER_EOF_PENDING;
			}
		}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	yy_n_chars += number_to_move;
	yy_current_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
	yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

	yytext_ptr = &yy_current_buffer->yy_ch_buf[0];

	return ret_val;
	}


/* yy_get_previous_state - get the state just before the EOB char was reached */

static yy_state_type yy_get_previous_state()
	{
	register yy_state_type yy_current_state;
	register char *yy_cp;

	yy_current_state = yy_start;

	for ( yy_cp = yytext_ptr + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
		{
		register YY_CHAR yy_c = (*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : 1);
		if ( yy_accept[yy_current_state] )
			{
			yy_last_accepting_state = yy_current_state;
			yy_last_accepting_cpos = yy_cp;
			}
		while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
			{
			yy_current_state = (int) yy_def[yy_current_state];
			if ( yy_current_state >= 386 )
				yy_c = yy_meta[(unsigned int) yy_c];
			}
		yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
		}

	return yy_current_state;
	}


/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *	next_state = yy_try_NUL_trans( current_state );
 */

#ifdef YY_USE_PROTOS
static yy_state_type yy_try_NUL_trans( yy_state_type yy_current_state )
#else
static yy_state_type yy_try_NUL_trans( yy_current_state )
yy_state_type yy_current_state;
#endif
	{
	register int yy_is_jam;
	register char *yy_cp = yy_c_buf_p;

	register YY_CHAR yy_c = 1;
	if ( yy_accept[yy_current_state] )
		{
		yy_last_accepting_state = yy_current_state;
		yy_last_accepting_cpos = yy_cp;
		}
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
		yy_current_state = (int) yy_def[yy_current_state];
		if ( yy_current_state >= 386 )
			yy_c = yy_meta[(unsigned int) yy_c];
		}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 385);

	return yy_is_jam ? 0 : yy_current_state;
	}


#ifndef YY_NO_UNPUT
#ifdef YY_USE_PROTOS
static void yyunput( int c, register char *yy_bp )
#else
static void yyunput( c, yy_bp )
int c;
register char *yy_bp;
#endif
	{
	register char *yy_cp = yy_c_buf_p;

	/* undo effects of setting up yytext */
	*yy_cp = yy_hold_char;

	if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
		{ /* need to shift things up to make room */
		/* +2 for EOB chars. */
		register int number_to_move = yy_n_chars + 2;
		register char *dest = &yy_current_buffer->yy_ch_buf[
					yy_current_buffer->yy_buf_size + 2];
		register char *source =
				&yy_current_buffer->yy_ch_buf[number_to_move];

		while ( source > yy_current_buffer->yy_ch_buf )
			*--dest = *--source;

		yy_cp += (int) (dest - source);
		yy_bp += (int) (dest - source);
		yy_current_buffer->yy_n_chars =
			yy_n_chars = yy_current_buffer->yy_buf_size;

		if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
			YY_FATAL_ERROR( "flex scanner push-back overflow" );
		}

	*--yy_cp = (char) c;


	yytext_ptr = yy_bp;
	yy_hold_char = *yy_cp;
	yy_c_buf_p = yy_cp;
	}
#endif	/* ifndef YY_NO_UNPUT */


#ifdef __cplusplus
static int yyinput()
#else
static int input()
#endif
	{
	int c;

	*yy_c_buf_p = yy_hold_char;

	if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
		{
		/* yy_c_buf_p now points to the character we want to return.
		 * If this occurs *before* the EOB characters, then it's a
		 * valid NUL; if not, then we've hit the end of the buffer.
		 */
		if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			/* This was really a NUL. */
			*yy_c_buf_p = '\0';

		else
			{ /* need more input */
			int offset = yy_c_buf_p - yytext_ptr;
			++yy_c_buf_p;

			switch ( yy_get_next_buffer() )
				{
				case EOB_ACT_LAST_MATCH:
					/* This happens because yy_g_n_b()
					 * sees that we've accumulated a
					 * token and flags that we need to
					 * try matching the token before
					 * proceeding.  But for input(),
					 * there's no matching to consider.
					 * So convert the EOB_ACT_LAST_MATCH
					 * to EOB_ACT_END_OF_FILE.
					 */

					/* Reset buffer status. */
					yyrestart( yyin );

					/* fall through */

				case EOB_ACT_END_OF_FILE:
					{
					if ( yywrap() )
						return EOF;

					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
					}

				case EOB_ACT_CONTINUE_SCAN:
					yy_c_buf_p = yytext_ptr + offset;
					break;
				}
			}
		}

	c = *(unsigned char *) yy_c_buf_p;	/* cast for 8-bit char's */
	*yy_c_buf_p = '\0';	/* preserve yytext */
	yy_hold_char = *++yy_c_buf_p;


	return c;
	}


#ifdef YY_USE_PROTOS
void yyrestart( FILE *input_file )
#else
void yyrestart( input_file )
FILE *input_file;
#endif
	{
	if ( ! yy_current_buffer )
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );

	yy_init_buffer( yy_current_buffer, input_file );
	yy_load_buffer_state();
	}


#ifdef YY_USE_PROTOS
void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
#else
void yy_switch_to_buffer( new_buffer )
YY_BUFFER_STATE new_buffer;
#endif
	{
	if ( yy_current_buffer == new_buffer )
		return;

	if ( yy_current_buffer )
		{
		/* Flush out information for old buffer. */
		*yy_c_buf_p = yy_hold_char;
		yy_current_buffer->yy_buf_pos = yy_c_buf_p;
		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	yy_current_buffer = new_buffer;
	yy_load_buffer_state();

	/* We don't actually know whether we did this switch during
	 * EOF (yywrap()) processing, but the only time this flag
	 * is looked at is after yywrap() is called, so it's safe
	 * to go ahead and always set it.
	 */
	yy_did_buffer_switch_on_eof = 1;
	}


#ifdef YY_USE_PROTOS
void yy_load_buffer_state( void )
#else
void yy_load_buffer_state()
#endif
	{
	yy_n_chars = yy_current_buffer->yy_n_chars;
	yytext_ptr = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
	yyin = yy_current_buffer->yy_input_file;
	yy_hold_char = *yy_c_buf_p;
	}


#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size )
#else
YY_BUFFER_STATE yy_create_buffer( file, size )
FILE *file;
int size;
#endif
	{
	YY_BUFFER_STATE b;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_buf_size = size;

	/* yy_ch_buf has to be 2 characters longer than the size given because
	 * we need to put in 2 end-of-buffer characters.
	 */
	b->yy_ch_buf = (char *) yy_flex_alloc( b->yy_buf_size + 2 );
	if ( ! b->yy_ch_buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_is_our_buffer = 1;

	yy_init_buffer( b, file );

	return b;
	}


#ifdef YY_USE_PROTOS
void yy_delete_buffer( YY_BUFFER_STATE b )
#else
void yy_delete_buffer( b )
YY_BUFFER_STATE b;
#endif
	{
	if ( ! b )
		return;

	if ( b == yy_current_buffer )
		yy_current_buffer = (YY_BUFFER_STATE) 0;

	if ( b->yy_is_our_buffer )
		yy_flex_free( (void *) b->yy_ch_buf );

	yy_flex_free( (void *) b );
	}


#ifndef YY_ALWAYS_INTERACTIVE
#ifndef YY_NEVER_INTERACTIVE
extern int isatty YY_PROTO(( int ));
#endif
#endif

#ifdef YY_USE_PROTOS
void yy_init_buffer( YY_BUFFER_STATE b, FILE *file )
#else
void yy_init_buffer( b, file )
YY_BUFFER_STATE b;
FILE *file;
#endif


	{
	yy_flush_buffer( b );

	b->yy_input_file = file;
	b->yy_fill_buffer = 1;

#if YY_ALWAYS_INTERACTIVE
	b->yy_is_interactive = 1;
#else
#if YY_NEVER_INTERACTIVE
	b->yy_is_interactive = 0;
#else
	b->yy_is_interactive = file ? (isatty( fileno(file) ) > 0) : 0;
#endif
#endif
	}


#ifdef YY_USE_PROTOS
void yy_flush_buffer( YY_BUFFER_STATE b )
#else
void yy_flush_buffer( b )
YY_BUFFER_STATE b;
#endif

	{
	if ( ! b )
		return;

	b->yy_n_chars = 0;

	/* We always need two end-of-buffer characters.  The first causes
	 * a transition to the end-of-buffer state.  The second causes
	 * a jam in that state.
	 */
	b->yy_ch_buf[0] = YY_END_OF_BUFFER_CHAR;
	b->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;

	b->yy_buf_pos = &b->yy_ch_buf[0];

	b->yy_at_bol = 1;
	b->yy_buffer_status = YY_BUFFER_NEW;

	if ( b == yy_current_buffer )
		yy_load_buffer_state();
	}


#ifndef YY_NO_SCAN_BUFFER
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_buffer( char *base, yy_size_t size )
#else
YY_BUFFER_STATE yy_scan_buffer( base, size )
char *base;
yy_size_t size;
#endif
	{
	YY_BUFFER_STATE b;

	if ( size < 2 ||
	     base[size-2] != YY_END_OF_BUFFER_CHAR ||
	     base[size-1] != YY_END_OF_BUFFER_CHAR )
		/* They forgot to leave room for the EOB's. */
		return 0;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_buffer()" );

	b->yy_buf_size = size - 2;	/* "- 2" to take care of EOB's */
	b->yy_buf_pos = b->yy_ch_buf = base;
	b->yy_is_our_buffer = 0;
	b->yy_input_file = 0;
	b->yy_n_chars = b->yy_buf_size;
	b->yy_is_interactive = 0;
	b->yy_at_bol = 1;
	b->yy_fill_buffer = 0;
	b->yy_buffer_status = YY_BUFFER_NEW;

	yy_switch_to_buffer( b );

	return b;
	}
#endif


#ifndef YY_NO_SCAN_STRING
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_string( yyconst char *yy_str )
#else
YY_BUFFER_STATE yy_scan_string( yy_str )
yyconst char *yy_str;
#endif
	{
	int len;
	for ( len = 0; yy_str[len]; ++len )
		;

	return yy_scan_bytes( yy_str, len );
	}
#endif


#ifndef YY_NO_SCAN_BYTES
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_bytes( yyconst char *bytes, int len )
#else
YY_BUFFER_STATE yy_scan_bytes( bytes, len )
yyconst char *bytes;
int len;
#endif
	{
	YY_BUFFER_STATE b;
	char *buf;
	yy_size_t n;
	int i;

	/* Get memory for full buffer, including space for trailing EOB's. */
	n = len + 2;
	buf = (char *) yy_flex_alloc( n );
	if ( ! buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_bytes()" );

	for ( i = 0; i < len; ++i )
		buf[i] = bytes[i];

	buf[len] = buf[len+1] = YY_END_OF_BUFFER_CHAR;

	b = yy_scan_buffer( buf, n );
	if ( ! b )
		YY_FATAL_ERROR( "bad buffer in yy_scan_bytes()" );

	/* It's okay to grow etc. this buffer, and we should throw it
	 * away when we're done.
	 */
	b->yy_is_our_buffer = 1;

	return b;
	}
#endif


#ifndef YY_NO_PUSH_STATE
#ifdef YY_USE_PROTOS
static void yy_push_state( int new_state )
#else
static void yy_push_state( new_state )
int new_state;
#endif
	{
	if ( yy_start_stack_ptr >= yy_start_stack_depth )
		{
		yy_size_t new_size;

		yy_start_stack_depth += YY_START_STACK_INCR;
		new_size = yy_start_stack_depth * sizeof( int );

		if ( ! yy_start_stack )
			yy_start_stack = (int *) yy_flex_alloc( new_size );

		else
			yy_start_stack = (int *) yy_flex_realloc(
					(void *) yy_start_stack, new_size );

		if ( ! yy_start_stack )
			YY_FATAL_ERROR(
			"out of memory expanding start-condition stack" );
		}

	yy_start_stack[yy_start_stack_ptr++] = YY_START;

	BEGIN(new_state);
	}
#endif


#ifndef YY_NO_POP_STATE
static void yy_pop_state()
	{
	if ( --yy_start_stack_ptr < 0 )
		YY_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(yy_start_stack[yy_start_stack_ptr]);
	}
#endif


#ifndef YY_NO_TOP_STATE
static int yy_top_state()
	{
	return yy_start_stack[yy_start_stack_ptr - 1];
	}
#endif

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

#ifdef YY_USE_PROTOS
static void yy_fatal_error( yyconst char msg[] )
#else
static void yy_fatal_error( msg )
char msg[];
#endif
	{
	(void) fprintf( stderr, "%s\n", msg );
	exit( YY_EXIT_FAILURE );
	}



/* Redefine yyless() so it works in section 3 code. */

#undef yyless
#define yyless(n) \
	do \
		{ \
		/* Undo effects of setting up yytext. */ \
		yytext[yyleng] = yy_hold_char; \
		yy_c_buf_p = yytext + n; \
		yy_hold_char = *yy_c_buf_p; \
		*yy_c_buf_p = '\0'; \
		yyleng = n; \
		} \
	while ( 0 )


/* Internal utility routines. */

#ifndef yytext_ptr
#ifdef YY_USE_PROTOS
static void yy_flex_strncpy( char *s1, yyconst char *s2, int n )
#else
static void yy_flex_strncpy( s1, s2, n )
char *s1;
yyconst char *s2;
int n;
#endif
	{
	register int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
	}
#endif

#ifdef YY_NEED_STRLEN
#ifdef YY_USE_PROTOS
static int yy_flex_strlen( yyconst char *s )
#else
static int yy_flex_strlen( s )
yyconst char *s;
#endif
	{
	register int n;
	for ( n = 0; s[n]; ++n )
		;

	return n;
	}
#endif


#ifdef YY_USE_PROTOS
static void *yy_flex_alloc( yy_size_t size )
#else
static void *yy_flex_alloc( size )
yy_size_t size;
#endif
	{
	return (void *) malloc( size );
	}

#ifdef YY_USE_PROTOS
static void *yy_flex_realloc( void *ptr, yy_size_t size )
#else
static void *yy_flex_realloc( ptr, size )
void *ptr;
yy_size_t size;
#endif
	{
	/* The cast to (char *) in the following accommodates both
	 * implementations that use char* generic pointers, and those
	 * that use void* generic pointers.  It works with the latter
	 * because both ANSI C and C++ allow castless assignment from
	 * any pointer type to void*, and deal with argument conversions
	 * as though doing an assignment.
	 */
	return (void *) realloc( (char *) ptr, size );
	}

#ifdef YY_USE_PROTOS
static void yy_flex_free( void *ptr )
#else
static void yy_flex_free( ptr )
void *ptr;
#endif
	{
	free( ptr );
	}

#if YY_MAIN
int main()
	{
	yylex();
	return 0;
	}
#endif


void ddlerror(char const *s)
{
	printf("yyerror: %d: %s at %s\n", lineno, s, yytext);
}

typedef std::vector<char*> valbuf_t;

static valbuf_t valbuf;

/*
 * Called before any actual parsing is done
 */
void scanner_init(const char *str)
{
	size_t slen = strlen(str);

	/*
	 * Might be left over after ereport()
	 */
	if (YY_CURRENT_BUFFER)
		yy_delete_buffer(YY_CURRENT_BUFFER);

	/*
	 * Make a scan buffer with special termination needed by flex.
	 */
	scanbuf =  (char *)malloc(slen + 2);
	memcpy(scanbuf, str, slen);
	scanbuf[slen] = scanbuf[slen + 1] = YY_END_OF_BUFFER_CHAR;
	scanbufhandle = yy_scan_buffer(scanbuf, slen + 2);

	BEGIN(INITIAL);

        valbuf.clear();
}


/*
 * Called after parsing is done to clean up after scanner_init()
 */


void scanner_finish(void)
{
  char* str;

   yy_delete_buffer(scanbufhandle);
   free(scanbuf);
   unsigned int i;
   for(i=0; i<valbuf.size(); i++) {
     str = valbuf[i];
     if(str) {
//        std::cout << "valbuf:(" << str << ")" << std::endl;
        free(valbuf[i]);
     }
   }
   valbuf.clear();
}

char* scanner_copy (char *str)
{
  char* nv = strdup(str);
  if(nv)
    valbuf.push_back(nv);
  return nv;
}
