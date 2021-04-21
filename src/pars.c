/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "pars.yacc"

/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2003 Grace Development Team
 * 
 * Maintained by Evgeny Stambulchik
 * 
 * 
 *                           All Rights Reserved
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*  
 * 
 * evaluate expressions, commands, parameter files
 * 
 */

#include <config.h>
#include <cmath.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#if defined(HAVE_SYS_PARAM_H)
#  include <sys/param.h>
#endif

/* bison not always handles it well itself */
#if defined(HAVE_ALLOCA_H)
#  include <alloca.h>
#endif

#include "defines.h"
#include "globals.h"
#include "cephes/cephes.h"
#include "device.h"
#include "utils.h"
#include "files.h"
#include "graphs.h"
#include "graphutils.h"
#include "plotone.h"
#include "dlmodule.h"
#include "t1fonts.h"
#include "ssdata.h"
#include "protos.h"
#include "parser.h"
#include "mathstuff.h"

#define MAX_PARS_STRING_LENGTH  4096

#define CAST_DBL_TO_BOOL(x) (fabs(x) < 0.5 ? 0:1)

typedef double (*ParserFnc)();

extern graph *g;

static double  s_result;    /* return value if a scalar expression is scanned*/
static grarr *v_result;    /* return value if a vector expression is scanned*/

static int expr_parsed, vexpr_parsed;

static int interr;

static grarr freelist[100]; 	/* temporary vectors */
static int fcnt = 0;		/* number of the temporary vectors allocated */

static target trgt_pool[100]; 	/* pool of temporary targets */
static int tgtn = 0;		/* number of the temporary targets used */

int naxis = 0;	/* current axis */
static int curline, curbox, curellipse, curstring;
/* these guys attempt to avoid reentrancy problems */
static int gotparams = FALSE, gotread = FALSE, gotnlfit = FALSE; 
int readxformat;
static int nlfit_gno, nlfit_setno, nlfit_nsteps;
static double *nlfit_warray = NULL;

char batchfile[GR_MAXPATHLEN] = "",
     paramfile[GR_MAXPATHLEN] = "",
     readfile[GR_MAXPATHLEN] = "";

static char f_string[MAX_PARS_STRING_LENGTH]; /* buffer for string to parse */
static int pos;

/* the graph, set, and its length of the parser's current state */
static int whichgraph;
static int whichset;

/* the graph and set of the left part of a vector assignment */
static int vasgn_gno;
static int vasgn_setno;

static int alias_force = FALSE; /* controls whether aliases can override
                                                       existing keywords */

extern char print_file[];
extern char *close_input;

static int filltype_obs;

static int index_shift = 0;     /* 0 for C, 1 for F77 index notation */

static void free_tmpvrbl(grarr *vrbl);
static void copy_vrbl(grarr *dest, grarr *src);
static int find_set_bydata(double *data, target *tgt);

static int getcharstr(void);
static void ungetchstr(void);
static int follow(int expect, int ifyes, int ifno);

static int yylex(void);
static int yyparse(void);
static void yyerror(char *s);

static int findf(symtab_entry *keytable, char *s);

/* Total (intrinsic + user-defined) list of functions and keywords */
symtab_entry *key;


#line 212 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    KEY_VAR = 258,                 /* KEY_VAR  */
    KEY_VEC = 259,                 /* KEY_VEC  */
    KEY_CONST = 260,               /* KEY_CONST  */
    KEY_UNIT = 261,                /* KEY_UNIT  */
    KEY_FUNC_I = 262,              /* KEY_FUNC_I  */
    KEY_FUNC_D = 263,              /* KEY_FUNC_D  */
    KEY_FUNC_NN = 264,             /* KEY_FUNC_NN  */
    KEY_FUNC_ND = 265,             /* KEY_FUNC_ND  */
    KEY_FUNC_DD = 266,             /* KEY_FUNC_DD  */
    KEY_FUNC_NND = 267,            /* KEY_FUNC_NND  */
    KEY_FUNC_PPD = 268,            /* KEY_FUNC_PPD  */
    KEY_FUNC_PPPD = 269,           /* KEY_FUNC_PPPD  */
    KEY_FUNC_PPPPD = 270,          /* KEY_FUNC_PPPPD  */
    KEY_FUNC_PPPPPD = 271,         /* KEY_FUNC_PPPPPD  */
    INDEX = 272,                   /* INDEX  */
    DATE = 273,                    /* DATE  */
    VAR_D = 274,                   /* VAR_D  */
    VEC_D = 275,                   /* VEC_D  */
    CONSTANT = 276,                /* CONSTANT  */
    UCONSTANT = 277,               /* UCONSTANT  */
    FUNC_I = 278,                  /* FUNC_I  */
    FUNC_D = 279,                  /* FUNC_D  */
    FUNC_NN = 280,                 /* FUNC_NN  */
    FUNC_ND = 281,                 /* FUNC_ND  */
    FUNC_DD = 282,                 /* FUNC_DD  */
    FUNC_NND = 283,                /* FUNC_NND  */
    FUNC_PPD = 284,                /* FUNC_PPD  */
    FUNC_PPPD = 285,               /* FUNC_PPPD  */
    FUNC_PPPPD = 286,              /* FUNC_PPPPD  */
    FUNC_PPPPPD = 287,             /* FUNC_PPPPPD  */
    ABOVE = 288,                   /* ABOVE  */
    ABSOLUTE = 289,                /* ABSOLUTE  */
    ALIAS = 290,                   /* ALIAS  */
    ALT = 291,                     /* ALT  */
    ALTXAXIS = 292,                /* ALTXAXIS  */
    ALTYAXIS = 293,                /* ALTYAXIS  */
    ANGLE = 294,                   /* ANGLE  */
    ANTIALIASING = 295,            /* ANTIALIASING  */
    APPEND = 296,                  /* APPEND  */
    ARRANGE = 297,                 /* ARRANGE  */
    ARROW = 298,                   /* ARROW  */
    ASCENDING = 299,               /* ASCENDING  */
    ASPLINE = 300,                 /* ASPLINE  */
    AUTO = 301,                    /* AUTO  */
    AUTOSCALE = 302,               /* AUTOSCALE  */
    AUTOTICKS = 303,               /* AUTOTICKS  */
    AVALUE = 304,                  /* AVALUE  */
    AVG = 305,                     /* AVG  */
    BACKGROUND = 306,              /* BACKGROUND  */
    BAR = 307,                     /* BAR  */
    BARDY = 308,                   /* BARDY  */
    BARDYDY = 309,                 /* BARDYDY  */
    BASELINE = 310,                /* BASELINE  */
    BATCH = 311,                   /* BATCH  */
    BEGIN = 312,                   /* BEGIN  */
    BELOW = 313,                   /* BELOW  */
    BETWEEN = 314,                 /* BETWEEN  */
    BLACKMAN = 315,                /* BLACKMAN  */
    BLOCK = 316,                   /* BLOCK  */
    BOTH = 317,                    /* BOTH  */
    BOTTOM = 318,                  /* BOTTOM  */
    BOX = 319,                     /* BOX  */
    CD = 320,                      /* CD  */
    CENTER = 321,                  /* CENTER  */
    CHAR = 322,                    /* CHAR  */
    CHART = 323,                   /* CHART  */
    CHRSTR = 324,                  /* CHRSTR  */
    CLEAR = 325,                   /* CLEAR  */
    CLICK = 326,                   /* CLICK  */
    CLIP = 327,                    /* CLIP  */
    CLOSE = 328,                   /* CLOSE  */
    COEFFICIENTS = 329,            /* COEFFICIENTS  */
    COLOR = 330,                   /* COLOR  */
    COMMENT = 331,                 /* COMMENT  */
    COMPLEX = 332,                 /* COMPLEX  */
    COMPUTING = 333,               /* COMPUTING  */
    CONSTRAINTS = 334,             /* CONSTRAINTS  */
    COPY = 335,                    /* COPY  */
    CYCLE = 336,                   /* CYCLE  */
    DAYMONTH = 337,                /* DAYMONTH  */
    DAYOFWEEKL = 338,              /* DAYOFWEEKL  */
    DAYOFWEEKS = 339,              /* DAYOFWEEKS  */
    DAYOFYEAR = 340,               /* DAYOFYEAR  */
    DDMMYY = 341,                  /* DDMMYY  */
    DECIMAL = 342,                 /* DECIMAL  */
    DEF = 343,                     /* DEF  */
    DEFAULT = 344,                 /* DEFAULT  */
    DEFINE = 345,                  /* DEFINE  */
    DEGREESLAT = 346,              /* DEGREESLAT  */
    DEGREESLON = 347,              /* DEGREESLON  */
    DEGREESMMLAT = 348,            /* DEGREESMMLAT  */
    DEGREESMMLON = 349,            /* DEGREESMMLON  */
    DEGREESMMSSLAT = 350,          /* DEGREESMMSSLAT  */
    DEGREESMMSSLON = 351,          /* DEGREESMMSSLON  */
    DESCENDING = 352,              /* DESCENDING  */
    DESCRIPTION = 353,             /* DESCRIPTION  */
    DEVICE = 354,                  /* DEVICE  */
    DFT = 355,                     /* DFT  */
    DIFFERENCE = 356,              /* DIFFERENCE  */
    DISK = 357,                    /* DISK  */
    DOWN = 358,                    /* DOWN  */
    DPI = 359,                     /* DPI  */
    DROP = 360,                    /* DROP  */
    DROPLINE = 361,                /* DROPLINE  */
    ECHO = 362,                    /* ECHO  */
    ELLIPSE = 363,                 /* ELLIPSE  */
    ENGINEERING = 364,             /* ENGINEERING  */
    ERRORBAR = 365,                /* ERRORBAR  */
    EXIT = 366,                    /* EXIT  */
    EXPONENTIAL = 367,             /* EXPONENTIAL  */
    FFT = 368,                     /* FFT  */
    FILEP = 369,                   /* FILEP  */
    FILL = 370,                    /* FILL  */
    FIT = 371,                     /* FIT  */
    FIXED = 372,                   /* FIXED  */
    FIXEDPOINT = 373,              /* FIXEDPOINT  */
    FLUSH = 374,                   /* FLUSH  */
    FOCUS = 375,                   /* FOCUS  */
    FOLLOWS = 376,                 /* FOLLOWS  */
    FONTP = 377,                   /* FONTP  */
    FORCE = 378,                   /* FORCE  */
    FORMAT = 379,                  /* FORMAT  */
    FORMULA = 380,                 /* FORMULA  */
    FRAMEP = 381,                  /* FRAMEP  */
    FREE = 382,                    /* FREE  */
    FREQUENCY = 383,               /* FREQUENCY  */
    FROM = 384,                    /* FROM  */
    GENERAL = 385,                 /* GENERAL  */
    GETP = 386,                    /* GETP  */
    GRAPH = 387,                   /* GRAPH  */
    GRAPHNO = 388,                 /* GRAPHNO  */
    GRID = 389,                    /* GRID  */
    HAMMING = 390,                 /* HAMMING  */
    HANNING = 391,                 /* HANNING  */
    HARDCOPY = 392,                /* HARDCOPY  */
    HBAR = 393,                    /* HBAR  */
    HELP = 394,                    /* HELP  */
    HGAP = 395,                    /* HGAP  */
    HIDDEN = 396,                  /* HIDDEN  */
    HISTOGRAM = 397,               /* HISTOGRAM  */
    HMS = 398,                     /* HMS  */
    HORIZI = 399,                  /* HORIZI  */
    HORIZONTAL = 400,              /* HORIZONTAL  */
    HORIZO = 401,                  /* HORIZO  */
    ID = 402,                      /* ID  */
    IFILTER = 403,                 /* IFILTER  */
    IMAX = 404,                    /* IMAX  */
    IMIN = 405,                    /* IMIN  */
    IN = 406,                      /* IN  */
    INCREMENT = 407,               /* INCREMENT  */
    INOUT = 408,                   /* INOUT  */
    INT = 409,                     /* INT  */
    INTEGRATE = 410,               /* INTEGRATE  */
    INTERPOLATE = 411,             /* INTERPOLATE  */
    INVDFT = 412,                  /* INVDFT  */
    INVERT = 413,                  /* INVERT  */
    INVFFT = 414,                  /* INVFFT  */
    JUST = 415,                    /* JUST  */
    KILL = 416,                    /* KILL  */
    LABEL = 417,                   /* LABEL  */
    LANDSCAPE = 418,               /* LANDSCAPE  */
    LAYOUT = 419,                  /* LAYOUT  */
    LEFT = 420,                    /* LEFT  */
    LEGEND = 421,                  /* LEGEND  */
    LENGTH = 422,                  /* LENGTH  */
    LINCONV = 423,                 /* LINCONV  */
    LINE = 424,                    /* LINE  */
    LINEAR = 425,                  /* LINEAR  */
    LINESTYLE = 426,               /* LINESTYLE  */
    LINEWIDTH = 427,               /* LINEWIDTH  */
    LINK = 428,                    /* LINK  */
    LOAD = 429,                    /* LOAD  */
    LOCTYPE = 430,                 /* LOCTYPE  */
    LOG = 431,                     /* LOG  */
    LOGARITHMIC = 432,             /* LOGARITHMIC  */
    LOGIT = 433,                   /* LOGIT  */
    LOGX = 434,                    /* LOGX  */
    LOGXY = 435,                   /* LOGXY  */
    LOGY = 436,                    /* LOGY  */
    MAGIC = 437,                   /* MAGIC  */
    MAGNITUDE = 438,               /* MAGNITUDE  */
    MAJOR = 439,                   /* MAJOR  */
    MAP = 440,                     /* MAP  */
    MAXP = 441,                    /* MAXP  */
    MESH = 442,                    /* MESH  */
    MINP = 443,                    /* MINP  */
    MINOR = 444,                   /* MINOR  */
    MMDD = 445,                    /* MMDD  */
    MMDDHMS = 446,                 /* MMDDHMS  */
    MMDDYY = 447,                  /* MMDDYY  */
    MMDDYYHMS = 448,               /* MMDDYYHMS  */
    MMSSLAT = 449,                 /* MMSSLAT  */
    MMSSLON = 450,                 /* MMSSLON  */
    MMYY = 451,                    /* MMYY  */
    MONTHDAY = 452,                /* MONTHDAY  */
    MONTHL = 453,                  /* MONTHL  */
    MONTHS = 454,                  /* MONTHS  */
    MONTHSY = 455,                 /* MONTHSY  */
    MOVE = 456,                    /* MOVE  */
    NEGATE = 457,                  /* NEGATE  */
    NEW = 458,                     /* NEW  */
    NONE = 459,                    /* NONE  */
    NONLFIT = 460,                 /* NONLFIT  */
    NORMAL = 461,                  /* NORMAL  */
    NXY = 462,                     /* NXY  */
    OFF = 463,                     /* OFF  */
    OFFSET = 464,                  /* OFFSET  */
    OFFSETX = 465,                 /* OFFSETX  */
    OFFSETY = 466,                 /* OFFSETY  */
    OFILTER = 467,                 /* OFILTER  */
    ON = 468,                      /* ON  */
    ONREAD = 469,                  /* ONREAD  */
    OP = 470,                      /* OP  */
    OPPOSITE = 471,                /* OPPOSITE  */
    OUT = 472,                     /* OUT  */
    PAGE = 473,                    /* PAGE  */
    PARA = 474,                    /* PARA  */
    PARAMETERS = 475,              /* PARAMETERS  */
    PARZEN = 476,                  /* PARZEN  */
    PATTERN = 477,                 /* PATTERN  */
    PERIOD = 478,                  /* PERIOD  */
    PERP = 479,                    /* PERP  */
    PHASE = 480,                   /* PHASE  */
    PIE = 481,                     /* PIE  */
    PIPE = 482,                    /* PIPE  */
    PLACE = 483,                   /* PLACE  */
    POINT = 484,                   /* POINT  */
    POLAR = 485,                   /* POLAR  */
    POLYI = 486,                   /* POLYI  */
    POLYO = 487,                   /* POLYO  */
    POP = 488,                     /* POP  */
    PORTRAIT = 489,                /* PORTRAIT  */
    POWER = 490,                   /* POWER  */
    PREC = 491,                    /* PREC  */
    PREPEND = 492,                 /* PREPEND  */
    PRINT = 493,                   /* PRINT  */
    PS = 494,                      /* PS  */
    PUSH = 495,                    /* PUSH  */
    PUTP = 496,                    /* PUTP  */
    RAND = 497,                    /* RAND  */
    READ = 498,                    /* READ  */
    REAL = 499,                    /* REAL  */
    RECIPROCAL = 500,              /* RECIPROCAL  */
    REDRAW = 501,                  /* REDRAW  */
    REFERENCE = 502,               /* REFERENCE  */
    REGNUM = 503,                  /* REGNUM  */
    REGRESS = 504,                 /* REGRESS  */
    RESIZE = 505,                  /* RESIZE  */
    RESTRICT = 506,                /* RESTRICT  */
    REVERSE = 507,                 /* REVERSE  */
    RIGHT = 508,                   /* RIGHT  */
    RISER = 509,                   /* RISER  */
    ROT = 510,                     /* ROT  */
    ROUNDED = 511,                 /* ROUNDED  */
    RSUM = 512,                    /* RSUM  */
    RULE = 513,                    /* RULE  */
    RUNAVG = 514,                  /* RUNAVG  */
    RUNMAX = 515,                  /* RUNMAX  */
    RUNMED = 516,                  /* RUNMED  */
    RUNMIN = 517,                  /* RUNMIN  */
    RUNSTD = 518,                  /* RUNSTD  */
    SAVEALL = 519,                 /* SAVEALL  */
    SCALE = 520,                   /* SCALE  */
    SCIENTIFIC = 521,              /* SCIENTIFIC  */
    SCROLL = 522,                  /* SCROLL  */
    SD = 523,                      /* SD  */
    SET = 524,                     /* SET  */
    SETNUM = 525,                  /* SETNUM  */
    SFORMAT = 526,                 /* SFORMAT  */
    SIGN = 527,                    /* SIGN  */
    SIZE = 528,                    /* SIZE  */
    SKIP = 529,                    /* SKIP  */
    SLEEP = 530,                   /* SLEEP  */
    SMITH = 531,                   /* SMITH  */
    SORT = 532,                    /* SORT  */
    SOURCE = 533,                  /* SOURCE  */
    SPEC = 534,                    /* SPEC  */
    SPLINE = 535,                  /* SPLINE  */
    SPLIT = 536,                   /* SPLIT  */
    STACK = 537,                   /* STACK  */
    STACKED = 538,                 /* STACKED  */
    STACKEDBAR = 539,              /* STACKEDBAR  */
    STACKEDHBAR = 540,             /* STACKEDHBAR  */
    STAGGER = 541,                 /* STAGGER  */
    START = 542,                   /* START  */
    STOP = 543,                    /* STOP  */
    STRING = 544,                  /* STRING  */
    SUM = 545,                     /* SUM  */
    SUBTITLE = 546,                /* SUBTITLE  */
    SWAP = 547,                    /* SWAP  */
    SYMBOL = 548,                  /* SYMBOL  */
    TARGET = 549,                  /* TARGET  */
    TICKLABEL = 550,               /* TICKLABEL  */
    TICKP = 551,                   /* TICKP  */
    TICKSP = 552,                  /* TICKSP  */
    TIMER = 553,                   /* TIMER  */
    TIMESTAMP = 554,               /* TIMESTAMP  */
    TITLE = 555,                   /* TITLE  */
    TO = 556,                      /* TO  */
    TOP = 557,                     /* TOP  */
    TRIANGULAR = 558,              /* TRIANGULAR  */
    TYPE = 559,                    /* TYPE  */
    UP = 560,                      /* UP  */
    UPDATEALL = 561,               /* UPDATEALL  */
    USE = 562,                     /* USE  */
    VERSION = 563,                 /* VERSION  */
    VERTI = 564,                   /* VERTI  */
    VERTICAL = 565,                /* VERTICAL  */
    VERTO = 566,                   /* VERTO  */
    VGAP = 567,                    /* VGAP  */
    VIEW = 568,                    /* VIEW  */
    VX1 = 569,                     /* VX1  */
    VX2 = 570,                     /* VX2  */
    VXMAX = 571,                   /* VXMAX  */
    VY1 = 572,                     /* VY1  */
    VY2 = 573,                     /* VY2  */
    VYMAX = 574,                   /* VYMAX  */
    WELCH = 575,                   /* WELCH  */
    WITH = 576,                    /* WITH  */
    WORLD = 577,                   /* WORLD  */
    WRAP = 578,                    /* WRAP  */
    WRITE = 579,                   /* WRITE  */
    WX1 = 580,                     /* WX1  */
    WX2 = 581,                     /* WX2  */
    WY1 = 582,                     /* WY1  */
    WY2 = 583,                     /* WY2  */
    X_TOK = 584,                   /* X_TOK  */
    X0 = 585,                      /* X0  */
    X1 = 586,                      /* X1  */
    XAXES = 587,                   /* XAXES  */
    XAXIS = 588,                   /* XAXIS  */
    XCOR = 589,                    /* XCOR  */
    XMAX = 590,                    /* XMAX  */
    XMIN = 591,                    /* XMIN  */
    XY = 592,                      /* XY  */
    XYAXES = 593,                  /* XYAXES  */
    XYBOXPLOT = 594,               /* XYBOXPLOT  */
    XYCOLOR = 595,                 /* XYCOLOR  */
    XYCOLPAT = 596,                /* XYCOLPAT  */
    XYDX = 597,                    /* XYDX  */
    XYDXDX = 598,                  /* XYDXDX  */
    XYDXDXDYDY = 599,              /* XYDXDXDYDY  */
    XYDXDY = 600,                  /* XYDXDY  */
    XYDY = 601,                    /* XYDY  */
    XYDYDY = 602,                  /* XYDYDY  */
    XYHILO = 603,                  /* XYHILO  */
    XYR = 604,                     /* XYR  */
    XYSIZE = 605,                  /* XYSIZE  */
    XYSTRING = 606,                /* XYSTRING  */
    XYVMAP = 607,                  /* XYVMAP  */
    XYZ = 608,                     /* XYZ  */
    Y_TOK = 609,                   /* Y_TOK  */
    Y0 = 610,                      /* Y0  */
    Y1 = 611,                      /* Y1  */
    Y2 = 612,                      /* Y2  */
    Y3 = 613,                      /* Y3  */
    Y4 = 614,                      /* Y4  */
    YAXES = 615,                   /* YAXES  */
    YAXIS = 616,                   /* YAXIS  */
    YEAR = 617,                    /* YEAR  */
    YMAX = 618,                    /* YMAX  */
    YMIN = 619,                    /* YMIN  */
    YYMMDD = 620,                  /* YYMMDD  */
    YYMMDDHMS = 621,               /* YYMMDDHMS  */
    ZERO = 622,                    /* ZERO  */
    ZNORM = 623,                   /* ZNORM  */
    FITPARM = 624,                 /* FITPARM  */
    FITPMAX = 625,                 /* FITPMAX  */
    FITPMIN = 626,                 /* FITPMIN  */
    NUMBER = 627,                  /* NUMBER  */
    NEW_TOKEN = 628,               /* NEW_TOKEN  */
    OR = 629,                      /* OR  */
    AND = 630,                     /* AND  */
    GT = 631,                      /* GT  */
    LT = 632,                      /* LT  */
    LE = 633,                      /* LE  */
    GE = 634,                      /* GE  */
    EQ = 635,                      /* EQ  */
    NE = 636,                      /* NE  */
    UMINUS = 637,                  /* UMINUS  */
    NOT = 638                      /* NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define KEY_VAR 258
#define KEY_VEC 259
#define KEY_CONST 260
#define KEY_UNIT 261
#define KEY_FUNC_I 262
#define KEY_FUNC_D 263
#define KEY_FUNC_NN 264
#define KEY_FUNC_ND 265
#define KEY_FUNC_DD 266
#define KEY_FUNC_NND 267
#define KEY_FUNC_PPD 268
#define KEY_FUNC_PPPD 269
#define KEY_FUNC_PPPPD 270
#define KEY_FUNC_PPPPPD 271
#define INDEX 272
#define DATE 273
#define VAR_D 274
#define VEC_D 275
#define CONSTANT 276
#define UCONSTANT 277
#define FUNC_I 278
#define FUNC_D 279
#define FUNC_NN 280
#define FUNC_ND 281
#define FUNC_DD 282
#define FUNC_NND 283
#define FUNC_PPD 284
#define FUNC_PPPD 285
#define FUNC_PPPPD 286
#define FUNC_PPPPPD 287
#define ABOVE 288
#define ABSOLUTE 289
#define ALIAS 290
#define ALT 291
#define ALTXAXIS 292
#define ALTYAXIS 293
#define ANGLE 294
#define ANTIALIASING 295
#define APPEND 296
#define ARRANGE 297
#define ARROW 298
#define ASCENDING 299
#define ASPLINE 300
#define AUTO 301
#define AUTOSCALE 302
#define AUTOTICKS 303
#define AVALUE 304
#define AVG 305
#define BACKGROUND 306
#define BAR 307
#define BARDY 308
#define BARDYDY 309
#define BASELINE 310
#define BATCH 311
#define BEGIN 312
#define BELOW 313
#define BETWEEN 314
#define BLACKMAN 315
#define BLOCK 316
#define BOTH 317
#define BOTTOM 318
#define BOX 319
#define CD 320
#define CENTER 321
#define CHAR 322
#define CHART 323
#define CHRSTR 324
#define CLEAR 325
#define CLICK 326
#define CLIP 327
#define CLOSE 328
#define COEFFICIENTS 329
#define COLOR 330
#define COMMENT 331
#define COMPLEX 332
#define COMPUTING 333
#define CONSTRAINTS 334
#define COPY 335
#define CYCLE 336
#define DAYMONTH 337
#define DAYOFWEEKL 338
#define DAYOFWEEKS 339
#define DAYOFYEAR 340
#define DDMMYY 341
#define DECIMAL 342
#define DEF 343
#define DEFAULT 344
#define DEFINE 345
#define DEGREESLAT 346
#define DEGREESLON 347
#define DEGREESMMLAT 348
#define DEGREESMMLON 349
#define DEGREESMMSSLAT 350
#define DEGREESMMSSLON 351
#define DESCENDING 352
#define DESCRIPTION 353
#define DEVICE 354
#define DFT 355
#define DIFFERENCE 356
#define DISK 357
#define DOWN 358
#define DPI 359
#define DROP 360
#define DROPLINE 361
#define ECHO 362
#define ELLIPSE 363
#define ENGINEERING 364
#define ERRORBAR 365
#define EXIT 366
#define EXPONENTIAL 367
#define FFT 368
#define FILEP 369
#define FILL 370
#define FIT 371
#define FIXED 372
#define FIXEDPOINT 373
#define FLUSH 374
#define FOCUS 375
#define FOLLOWS 376
#define FONTP 377
#define FORCE 378
#define FORMAT 379
#define FORMULA 380
#define FRAMEP 381
#define FREE 382
#define FREQUENCY 383
#define FROM 384
#define GENERAL 385
#define GETP 386
#define GRAPH 387
#define GRAPHNO 388
#define GRID 389
#define HAMMING 390
#define HANNING 391
#define HARDCOPY 392
#define HBAR 393
#define HELP 394
#define HGAP 395
#define HIDDEN 396
#define HISTOGRAM 397
#define HMS 398
#define HORIZI 399
#define HORIZONTAL 400
#define HORIZO 401
#define ID 402
#define IFILTER 403
#define IMAX 404
#define IMIN 405
#define IN 406
#define INCREMENT 407
#define INOUT 408
#define INT 409
#define INTEGRATE 410
#define INTERPOLATE 411
#define INVDFT 412
#define INVERT 413
#define INVFFT 414
#define JUST 415
#define KILL 416
#define LABEL 417
#define LANDSCAPE 418
#define LAYOUT 419
#define LEFT 420
#define LEGEND 421
#define LENGTH 422
#define LINCONV 423
#define LINE 424
#define LINEAR 425
#define LINESTYLE 426
#define LINEWIDTH 427
#define LINK 428
#define LOAD 429
#define LOCTYPE 430
#define LOG 431
#define LOGARITHMIC 432
#define LOGIT 433
#define LOGX 434
#define LOGXY 435
#define LOGY 436
#define MAGIC 437
#define MAGNITUDE 438
#define MAJOR 439
#define MAP 440
#define MAXP 441
#define MESH 442
#define MINP 443
#define MINOR 444
#define MMDD 445
#define MMDDHMS 446
#define MMDDYY 447
#define MMDDYYHMS 448
#define MMSSLAT 449
#define MMSSLON 450
#define MMYY 451
#define MONTHDAY 452
#define MONTHL 453
#define MONTHS 454
#define MONTHSY 455
#define MOVE 456
#define NEGATE 457
#define NEW 458
#define NONE 459
#define NONLFIT 460
#define NORMAL 461
#define NXY 462
#define OFF 463
#define OFFSET 464
#define OFFSETX 465
#define OFFSETY 466
#define OFILTER 467
#define ON 468
#define ONREAD 469
#define OP 470
#define OPPOSITE 471
#define OUT 472
#define PAGE 473
#define PARA 474
#define PARAMETERS 475
#define PARZEN 476
#define PATTERN 477
#define PERIOD 478
#define PERP 479
#define PHASE 480
#define PIE 481
#define PIPE 482
#define PLACE 483
#define POINT 484
#define POLAR 485
#define POLYI 486
#define POLYO 487
#define POP 488
#define PORTRAIT 489
#define POWER 490
#define PREC 491
#define PREPEND 492
#define PRINT 493
#define PS 494
#define PUSH 495
#define PUTP 496
#define RAND 497
#define READ 498
#define REAL 499
#define RECIPROCAL 500
#define REDRAW 501
#define REFERENCE 502
#define REGNUM 503
#define REGRESS 504
#define RESIZE 505
#define RESTRICT 506
#define REVERSE 507
#define RIGHT 508
#define RISER 509
#define ROT 510
#define ROUNDED 511
#define RSUM 512
#define RULE 513
#define RUNAVG 514
#define RUNMAX 515
#define RUNMED 516
#define RUNMIN 517
#define RUNSTD 518
#define SAVEALL 519
#define SCALE 520
#define SCIENTIFIC 521
#define SCROLL 522
#define SD 523
#define SET 524
#define SETNUM 525
#define SFORMAT 526
#define SIGN 527
#define SIZE 528
#define SKIP 529
#define SLEEP 530
#define SMITH 531
#define SORT 532
#define SOURCE 533
#define SPEC 534
#define SPLINE 535
#define SPLIT 536
#define STACK 537
#define STACKED 538
#define STACKEDBAR 539
#define STACKEDHBAR 540
#define STAGGER 541
#define START 542
#define STOP 543
#define STRING 544
#define SUM 545
#define SUBTITLE 546
#define SWAP 547
#define SYMBOL 548
#define TARGET 549
#define TICKLABEL 550
#define TICKP 551
#define TICKSP 552
#define TIMER 553
#define TIMESTAMP 554
#define TITLE 555
#define TO 556
#define TOP 557
#define TRIANGULAR 558
#define TYPE 559
#define UP 560
#define UPDATEALL 561
#define USE 562
#define VERSION 563
#define VERTI 564
#define VERTICAL 565
#define VERTO 566
#define VGAP 567
#define VIEW 568
#define VX1 569
#define VX2 570
#define VXMAX 571
#define VY1 572
#define VY2 573
#define VYMAX 574
#define WELCH 575
#define WITH 576
#define WORLD 577
#define WRAP 578
#define WRITE 579
#define WX1 580
#define WX2 581
#define WY1 582
#define WY2 583
#define X_TOK 584
#define X0 585
#define X1 586
#define XAXES 587
#define XAXIS 588
#define XCOR 589
#define XMAX 590
#define XMIN 591
#define XY 592
#define XYAXES 593
#define XYBOXPLOT 594
#define XYCOLOR 595
#define XYCOLPAT 596
#define XYDX 597
#define XYDXDX 598
#define XYDXDXDYDY 599
#define XYDXDY 600
#define XYDY 601
#define XYDYDY 602
#define XYHILO 603
#define XYR 604
#define XYSIZE 605
#define XYSTRING 606
#define XYVMAP 607
#define XYZ 608
#define Y_TOK 609
#define Y0 610
#define Y1 611
#define Y2 612
#define Y3 613
#define Y4 614
#define YAXES 615
#define YAXIS 616
#define YEAR 617
#define YMAX 618
#define YMIN 619
#define YYMMDD 620
#define YYMMDDHMS 621
#define ZERO 622
#define ZNORM 623
#define FITPARM 624
#define FITPMAX 625
#define FITPMIN 626
#define NUMBER 627
#define NEW_TOKEN 628
#define OR 629
#define AND 630
#define GT 631
#define LT 632
#define LE 633
#define GE 634
#define EQ 635
#define NE 636
#define UMINUS 637
#define NOT 638

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 142 "pars.yacc"

    int     ival;
    double  dval;
    char   *sval;
    double *dptr;
    target *trgt;
    grarr  *vrbl;

#line 1037 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);


/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_KEY_VAR = 3,                    /* KEY_VAR  */
  YYSYMBOL_KEY_VEC = 4,                    /* KEY_VEC  */
  YYSYMBOL_KEY_CONST = 5,                  /* KEY_CONST  */
  YYSYMBOL_KEY_UNIT = 6,                   /* KEY_UNIT  */
  YYSYMBOL_KEY_FUNC_I = 7,                 /* KEY_FUNC_I  */
  YYSYMBOL_KEY_FUNC_D = 8,                 /* KEY_FUNC_D  */
  YYSYMBOL_KEY_FUNC_NN = 9,                /* KEY_FUNC_NN  */
  YYSYMBOL_KEY_FUNC_ND = 10,               /* KEY_FUNC_ND  */
  YYSYMBOL_KEY_FUNC_DD = 11,               /* KEY_FUNC_DD  */
  YYSYMBOL_KEY_FUNC_NND = 12,              /* KEY_FUNC_NND  */
  YYSYMBOL_KEY_FUNC_PPD = 13,              /* KEY_FUNC_PPD  */
  YYSYMBOL_KEY_FUNC_PPPD = 14,             /* KEY_FUNC_PPPD  */
  YYSYMBOL_KEY_FUNC_PPPPD = 15,            /* KEY_FUNC_PPPPD  */
  YYSYMBOL_KEY_FUNC_PPPPPD = 16,           /* KEY_FUNC_PPPPPD  */
  YYSYMBOL_INDEX = 17,                     /* INDEX  */
  YYSYMBOL_DATE = 18,                      /* DATE  */
  YYSYMBOL_VAR_D = 19,                     /* VAR_D  */
  YYSYMBOL_VEC_D = 20,                     /* VEC_D  */
  YYSYMBOL_CONSTANT = 21,                  /* CONSTANT  */
  YYSYMBOL_UCONSTANT = 22,                 /* UCONSTANT  */
  YYSYMBOL_FUNC_I = 23,                    /* FUNC_I  */
  YYSYMBOL_FUNC_D = 24,                    /* FUNC_D  */
  YYSYMBOL_FUNC_NN = 25,                   /* FUNC_NN  */
  YYSYMBOL_FUNC_ND = 26,                   /* FUNC_ND  */
  YYSYMBOL_FUNC_DD = 27,                   /* FUNC_DD  */
  YYSYMBOL_FUNC_NND = 28,                  /* FUNC_NND  */
  YYSYMBOL_FUNC_PPD = 29,                  /* FUNC_PPD  */
  YYSYMBOL_FUNC_PPPD = 30,                 /* FUNC_PPPD  */
  YYSYMBOL_FUNC_PPPPD = 31,                /* FUNC_PPPPD  */
  YYSYMBOL_FUNC_PPPPPD = 32,               /* FUNC_PPPPPD  */
  YYSYMBOL_ABOVE = 33,                     /* ABOVE  */
  YYSYMBOL_ABSOLUTE = 34,                  /* ABSOLUTE  */
  YYSYMBOL_ALIAS = 35,                     /* ALIAS  */
  YYSYMBOL_ALT = 36,                       /* ALT  */
  YYSYMBOL_ALTXAXIS = 37,                  /* ALTXAXIS  */
  YYSYMBOL_ALTYAXIS = 38,                  /* ALTYAXIS  */
  YYSYMBOL_ANGLE = 39,                     /* ANGLE  */
  YYSYMBOL_ANTIALIASING = 40,              /* ANTIALIASING  */
  YYSYMBOL_APPEND = 41,                    /* APPEND  */
  YYSYMBOL_ARRANGE = 42,                   /* ARRANGE  */
  YYSYMBOL_ARROW = 43,                     /* ARROW  */
  YYSYMBOL_ASCENDING = 44,                 /* ASCENDING  */
  YYSYMBOL_ASPLINE = 45,                   /* ASPLINE  */
  YYSYMBOL_AUTO = 46,                      /* AUTO  */
  YYSYMBOL_AUTOSCALE = 47,                 /* AUTOSCALE  */
  YYSYMBOL_AUTOTICKS = 48,                 /* AUTOTICKS  */
  YYSYMBOL_AVALUE = 49,                    /* AVALUE  */
  YYSYMBOL_AVG = 50,                       /* AVG  */
  YYSYMBOL_BACKGROUND = 51,                /* BACKGROUND  */
  YYSYMBOL_BAR = 52,                       /* BAR  */
  YYSYMBOL_BARDY = 53,                     /* BARDY  */
  YYSYMBOL_BARDYDY = 54,                   /* BARDYDY  */
  YYSYMBOL_BASELINE = 55,                  /* BASELINE  */
  YYSYMBOL_BATCH = 56,                     /* BATCH  */
  YYSYMBOL_BEGIN = 57,                     /* BEGIN  */
  YYSYMBOL_BELOW = 58,                     /* BELOW  */
  YYSYMBOL_BETWEEN = 59,                   /* BETWEEN  */
  YYSYMBOL_BLACKMAN = 60,                  /* BLACKMAN  */
  YYSYMBOL_BLOCK = 61,                     /* BLOCK  */
  YYSYMBOL_BOTH = 62,                      /* BOTH  */
  YYSYMBOL_BOTTOM = 63,                    /* BOTTOM  */
  YYSYMBOL_BOX = 64,                       /* BOX  */
  YYSYMBOL_CD = 65,                        /* CD  */
  YYSYMBOL_CENTER = 66,                    /* CENTER  */
  YYSYMBOL_CHAR = 67,                      /* CHAR  */
  YYSYMBOL_CHART = 68,                     /* CHART  */
  YYSYMBOL_CHRSTR = 69,                    /* CHRSTR  */
  YYSYMBOL_CLEAR = 70,                     /* CLEAR  */
  YYSYMBOL_CLICK = 71,                     /* CLICK  */
  YYSYMBOL_CLIP = 72,                      /* CLIP  */
  YYSYMBOL_CLOSE = 73,                     /* CLOSE  */
  YYSYMBOL_COEFFICIENTS = 74,              /* COEFFICIENTS  */
  YYSYMBOL_COLOR = 75,                     /* COLOR  */
  YYSYMBOL_COMMENT = 76,                   /* COMMENT  */
  YYSYMBOL_COMPLEX = 77,                   /* COMPLEX  */
  YYSYMBOL_COMPUTING = 78,                 /* COMPUTING  */
  YYSYMBOL_CONSTRAINTS = 79,               /* CONSTRAINTS  */
  YYSYMBOL_COPY = 80,                      /* COPY  */
  YYSYMBOL_CYCLE = 81,                     /* CYCLE  */
  YYSYMBOL_DAYMONTH = 82,                  /* DAYMONTH  */
  YYSYMBOL_DAYOFWEEKL = 83,                /* DAYOFWEEKL  */
  YYSYMBOL_DAYOFWEEKS = 84,                /* DAYOFWEEKS  */
  YYSYMBOL_DAYOFYEAR = 85,                 /* DAYOFYEAR  */
  YYSYMBOL_DDMMYY = 86,                    /* DDMMYY  */
  YYSYMBOL_DECIMAL = 87,                   /* DECIMAL  */
  YYSYMBOL_DEF = 88,                       /* DEF  */
  YYSYMBOL_DEFAULT = 89,                   /* DEFAULT  */
  YYSYMBOL_DEFINE = 90,                    /* DEFINE  */
  YYSYMBOL_DEGREESLAT = 91,                /* DEGREESLAT  */
  YYSYMBOL_DEGREESLON = 92,                /* DEGREESLON  */
  YYSYMBOL_DEGREESMMLAT = 93,              /* DEGREESMMLAT  */
  YYSYMBOL_DEGREESMMLON = 94,              /* DEGREESMMLON  */
  YYSYMBOL_DEGREESMMSSLAT = 95,            /* DEGREESMMSSLAT  */
  YYSYMBOL_DEGREESMMSSLON = 96,            /* DEGREESMMSSLON  */
  YYSYMBOL_DESCENDING = 97,                /* DESCENDING  */
  YYSYMBOL_DESCRIPTION = 98,               /* DESCRIPTION  */
  YYSYMBOL_DEVICE = 99,                    /* DEVICE  */
  YYSYMBOL_DFT = 100,                      /* DFT  */
  YYSYMBOL_DIFFERENCE = 101,               /* DIFFERENCE  */
  YYSYMBOL_DISK = 102,                     /* DISK  */
  YYSYMBOL_DOWN = 103,                     /* DOWN  */
  YYSYMBOL_DPI = 104,                      /* DPI  */
  YYSYMBOL_DROP = 105,                     /* DROP  */
  YYSYMBOL_DROPLINE = 106,                 /* DROPLINE  */
  YYSYMBOL_ECHO = 107,                     /* ECHO  */
  YYSYMBOL_ELLIPSE = 108,                  /* ELLIPSE  */
  YYSYMBOL_ENGINEERING = 109,              /* ENGINEERING  */
  YYSYMBOL_ERRORBAR = 110,                 /* ERRORBAR  */
  YYSYMBOL_EXIT = 111,                     /* EXIT  */
  YYSYMBOL_EXPONENTIAL = 112,              /* EXPONENTIAL  */
  YYSYMBOL_FFT = 113,                      /* FFT  */
  YYSYMBOL_FILEP = 114,                    /* FILEP  */
  YYSYMBOL_FILL = 115,                     /* FILL  */
  YYSYMBOL_FIT = 116,                      /* FIT  */
  YYSYMBOL_FIXED = 117,                    /* FIXED  */
  YYSYMBOL_FIXEDPOINT = 118,               /* FIXEDPOINT  */
  YYSYMBOL_FLUSH = 119,                    /* FLUSH  */
  YYSYMBOL_FOCUS = 120,                    /* FOCUS  */
  YYSYMBOL_FOLLOWS = 121,                  /* FOLLOWS  */
  YYSYMBOL_FONTP = 122,                    /* FONTP  */
  YYSYMBOL_FORCE = 123,                    /* FORCE  */
  YYSYMBOL_FORMAT = 124,                   /* FORMAT  */
  YYSYMBOL_FORMULA = 125,                  /* FORMULA  */
  YYSYMBOL_FRAMEP = 126,                   /* FRAMEP  */
  YYSYMBOL_FREE = 127,                     /* FREE  */
  YYSYMBOL_FREQUENCY = 128,                /* FREQUENCY  */
  YYSYMBOL_FROM = 129,                     /* FROM  */
  YYSYMBOL_GENERAL = 130,                  /* GENERAL  */
  YYSYMBOL_GETP = 131,                     /* GETP  */
  YYSYMBOL_GRAPH = 132,                    /* GRAPH  */
  YYSYMBOL_GRAPHNO = 133,                  /* GRAPHNO  */
  YYSYMBOL_GRID = 134,                     /* GRID  */
  YYSYMBOL_HAMMING = 135,                  /* HAMMING  */
  YYSYMBOL_HANNING = 136,                  /* HANNING  */
  YYSYMBOL_HARDCOPY = 137,                 /* HARDCOPY  */
  YYSYMBOL_HBAR = 138,                     /* HBAR  */
  YYSYMBOL_HELP = 139,                     /* HELP  */
  YYSYMBOL_HGAP = 140,                     /* HGAP  */
  YYSYMBOL_HIDDEN = 141,                   /* HIDDEN  */
  YYSYMBOL_HISTOGRAM = 142,                /* HISTOGRAM  */
  YYSYMBOL_HMS = 143,                      /* HMS  */
  YYSYMBOL_HORIZI = 144,                   /* HORIZI  */
  YYSYMBOL_HORIZONTAL = 145,               /* HORIZONTAL  */
  YYSYMBOL_HORIZO = 146,                   /* HORIZO  */
  YYSYMBOL_ID = 147,                       /* ID  */
  YYSYMBOL_IFILTER = 148,                  /* IFILTER  */
  YYSYMBOL_IMAX = 149,                     /* IMAX  */
  YYSYMBOL_IMIN = 150,                     /* IMIN  */
  YYSYMBOL_IN = 151,                       /* IN  */
  YYSYMBOL_INCREMENT = 152,                /* INCREMENT  */
  YYSYMBOL_INOUT = 153,                    /* INOUT  */
  YYSYMBOL_INT = 154,                      /* INT  */
  YYSYMBOL_INTEGRATE = 155,                /* INTEGRATE  */
  YYSYMBOL_INTERPOLATE = 156,              /* INTERPOLATE  */
  YYSYMBOL_INVDFT = 157,                   /* INVDFT  */
  YYSYMBOL_INVERT = 158,                   /* INVERT  */
  YYSYMBOL_INVFFT = 159,                   /* INVFFT  */
  YYSYMBOL_JUST = 160,                     /* JUST  */
  YYSYMBOL_KILL = 161,                     /* KILL  */
  YYSYMBOL_LABEL = 162,                    /* LABEL  */
  YYSYMBOL_LANDSCAPE = 163,                /* LANDSCAPE  */
  YYSYMBOL_LAYOUT = 164,                   /* LAYOUT  */
  YYSYMBOL_LEFT = 165,                     /* LEFT  */
  YYSYMBOL_LEGEND = 166,                   /* LEGEND  */
  YYSYMBOL_LENGTH = 167,                   /* LENGTH  */
  YYSYMBOL_LINCONV = 168,                  /* LINCONV  */
  YYSYMBOL_LINE = 169,                     /* LINE  */
  YYSYMBOL_LINEAR = 170,                   /* LINEAR  */
  YYSYMBOL_LINESTYLE = 171,                /* LINESTYLE  */
  YYSYMBOL_LINEWIDTH = 172,                /* LINEWIDTH  */
  YYSYMBOL_LINK = 173,                     /* LINK  */
  YYSYMBOL_LOAD = 174,                     /* LOAD  */
  YYSYMBOL_LOCTYPE = 175,                  /* LOCTYPE  */
  YYSYMBOL_LOG = 176,                      /* LOG  */
  YYSYMBOL_LOGARITHMIC = 177,              /* LOGARITHMIC  */
  YYSYMBOL_LOGIT = 178,                    /* LOGIT  */
  YYSYMBOL_LOGX = 179,                     /* LOGX  */
  YYSYMBOL_LOGXY = 180,                    /* LOGXY  */
  YYSYMBOL_LOGY = 181,                     /* LOGY  */
  YYSYMBOL_MAGIC = 182,                    /* MAGIC  */
  YYSYMBOL_MAGNITUDE = 183,                /* MAGNITUDE  */
  YYSYMBOL_MAJOR = 184,                    /* MAJOR  */
  YYSYMBOL_MAP = 185,                      /* MAP  */
  YYSYMBOL_MAXP = 186,                     /* MAXP  */
  YYSYMBOL_MESH = 187,                     /* MESH  */
  YYSYMBOL_MINP = 188,                     /* MINP  */
  YYSYMBOL_MINOR = 189,                    /* MINOR  */
  YYSYMBOL_MMDD = 190,                     /* MMDD  */
  YYSYMBOL_MMDDHMS = 191,                  /* MMDDHMS  */
  YYSYMBOL_MMDDYY = 192,                   /* MMDDYY  */
  YYSYMBOL_MMDDYYHMS = 193,                /* MMDDYYHMS  */
  YYSYMBOL_MMSSLAT = 194,                  /* MMSSLAT  */
  YYSYMBOL_MMSSLON = 195,                  /* MMSSLON  */
  YYSYMBOL_MMYY = 196,                     /* MMYY  */
  YYSYMBOL_MONTHDAY = 197,                 /* MONTHDAY  */
  YYSYMBOL_MONTHL = 198,                   /* MONTHL  */
  YYSYMBOL_MONTHS = 199,                   /* MONTHS  */
  YYSYMBOL_MONTHSY = 200,                  /* MONTHSY  */
  YYSYMBOL_MOVE = 201,                     /* MOVE  */
  YYSYMBOL_NEGATE = 202,                   /* NEGATE  */
  YYSYMBOL_NEW = 203,                      /* NEW  */
  YYSYMBOL_NONE = 204,                     /* NONE  */
  YYSYMBOL_NONLFIT = 205,                  /* NONLFIT  */
  YYSYMBOL_NORMAL = 206,                   /* NORMAL  */
  YYSYMBOL_NXY = 207,                      /* NXY  */
  YYSYMBOL_OFF = 208,                      /* OFF  */
  YYSYMBOL_OFFSET = 209,                   /* OFFSET  */
  YYSYMBOL_OFFSETX = 210,                  /* OFFSETX  */
  YYSYMBOL_OFFSETY = 211,                  /* OFFSETY  */
  YYSYMBOL_OFILTER = 212,                  /* OFILTER  */
  YYSYMBOL_ON = 213,                       /* ON  */
  YYSYMBOL_ONREAD = 214,                   /* ONREAD  */
  YYSYMBOL_OP = 215,                       /* OP  */
  YYSYMBOL_OPPOSITE = 216,                 /* OPPOSITE  */
  YYSYMBOL_OUT = 217,                      /* OUT  */
  YYSYMBOL_PAGE = 218,                     /* PAGE  */
  YYSYMBOL_PARA = 219,                     /* PARA  */
  YYSYMBOL_PARAMETERS = 220,               /* PARAMETERS  */
  YYSYMBOL_PARZEN = 221,                   /* PARZEN  */
  YYSYMBOL_PATTERN = 222,                  /* PATTERN  */
  YYSYMBOL_PERIOD = 223,                   /* PERIOD  */
  YYSYMBOL_PERP = 224,                     /* PERP  */
  YYSYMBOL_PHASE = 225,                    /* PHASE  */
  YYSYMBOL_PIE = 226,                      /* PIE  */
  YYSYMBOL_PIPE = 227,                     /* PIPE  */
  YYSYMBOL_PLACE = 228,                    /* PLACE  */
  YYSYMBOL_POINT = 229,                    /* POINT  */
  YYSYMBOL_POLAR = 230,                    /* POLAR  */
  YYSYMBOL_POLYI = 231,                    /* POLYI  */
  YYSYMBOL_POLYO = 232,                    /* POLYO  */
  YYSYMBOL_POP = 233,                      /* POP  */
  YYSYMBOL_PORTRAIT = 234,                 /* PORTRAIT  */
  YYSYMBOL_POWER = 235,                    /* POWER  */
  YYSYMBOL_PREC = 236,                     /* PREC  */
  YYSYMBOL_PREPEND = 237,                  /* PREPEND  */
  YYSYMBOL_PRINT = 238,                    /* PRINT  */
  YYSYMBOL_PS = 239,                       /* PS  */
  YYSYMBOL_PUSH = 240,                     /* PUSH  */
  YYSYMBOL_PUTP = 241,                     /* PUTP  */
  YYSYMBOL_RAND = 242,                     /* RAND  */
  YYSYMBOL_READ = 243,                     /* READ  */
  YYSYMBOL_REAL = 244,                     /* REAL  */
  YYSYMBOL_RECIPROCAL = 245,               /* RECIPROCAL  */
  YYSYMBOL_REDRAW = 246,                   /* REDRAW  */
  YYSYMBOL_REFERENCE = 247,                /* REFERENCE  */
  YYSYMBOL_REGNUM = 248,                   /* REGNUM  */
  YYSYMBOL_REGRESS = 249,                  /* REGRESS  */
  YYSYMBOL_RESIZE = 250,                   /* RESIZE  */
  YYSYMBOL_RESTRICT = 251,                 /* RESTRICT  */
  YYSYMBOL_REVERSE = 252,                  /* REVERSE  */
  YYSYMBOL_RIGHT = 253,                    /* RIGHT  */
  YYSYMBOL_RISER = 254,                    /* RISER  */
  YYSYMBOL_ROT = 255,                      /* ROT  */
  YYSYMBOL_ROUNDED = 256,                  /* ROUNDED  */
  YYSYMBOL_RSUM = 257,                     /* RSUM  */
  YYSYMBOL_RULE = 258,                     /* RULE  */
  YYSYMBOL_RUNAVG = 259,                   /* RUNAVG  */
  YYSYMBOL_RUNMAX = 260,                   /* RUNMAX  */
  YYSYMBOL_RUNMED = 261,                   /* RUNMED  */
  YYSYMBOL_RUNMIN = 262,                   /* RUNMIN  */
  YYSYMBOL_RUNSTD = 263,                   /* RUNSTD  */
  YYSYMBOL_SAVEALL = 264,                  /* SAVEALL  */
  YYSYMBOL_SCALE = 265,                    /* SCALE  */
  YYSYMBOL_SCIENTIFIC = 266,               /* SCIENTIFIC  */
  YYSYMBOL_SCROLL = 267,                   /* SCROLL  */
  YYSYMBOL_SD = 268,                       /* SD  */
  YYSYMBOL_SET = 269,                      /* SET  */
  YYSYMBOL_SETNUM = 270,                   /* SETNUM  */
  YYSYMBOL_SFORMAT = 271,                  /* SFORMAT  */
  YYSYMBOL_SIGN = 272,                     /* SIGN  */
  YYSYMBOL_SIZE = 273,                     /* SIZE  */
  YYSYMBOL_SKIP = 274,                     /* SKIP  */
  YYSYMBOL_SLEEP = 275,                    /* SLEEP  */
  YYSYMBOL_SMITH = 276,                    /* SMITH  */
  YYSYMBOL_SORT = 277,                     /* SORT  */
  YYSYMBOL_SOURCE = 278,                   /* SOURCE  */
  YYSYMBOL_SPEC = 279,                     /* SPEC  */
  YYSYMBOL_SPLINE = 280,                   /* SPLINE  */
  YYSYMBOL_SPLIT = 281,                    /* SPLIT  */
  YYSYMBOL_STACK = 282,                    /* STACK  */
  YYSYMBOL_STACKED = 283,                  /* STACKED  */
  YYSYMBOL_STACKEDBAR = 284,               /* STACKEDBAR  */
  YYSYMBOL_STACKEDHBAR = 285,              /* STACKEDHBAR  */
  YYSYMBOL_STAGGER = 286,                  /* STAGGER  */
  YYSYMBOL_START = 287,                    /* START  */
  YYSYMBOL_STOP = 288,                     /* STOP  */
  YYSYMBOL_STRING = 289,                   /* STRING  */
  YYSYMBOL_SUM = 290,                      /* SUM  */
  YYSYMBOL_SUBTITLE = 291,                 /* SUBTITLE  */
  YYSYMBOL_SWAP = 292,                     /* SWAP  */
  YYSYMBOL_SYMBOL = 293,                   /* SYMBOL  */
  YYSYMBOL_TARGET = 294,                   /* TARGET  */
  YYSYMBOL_TICKLABEL = 295,                /* TICKLABEL  */
  YYSYMBOL_TICKP = 296,                    /* TICKP  */
  YYSYMBOL_TICKSP = 297,                   /* TICKSP  */
  YYSYMBOL_TIMER = 298,                    /* TIMER  */
  YYSYMBOL_TIMESTAMP = 299,                /* TIMESTAMP  */
  YYSYMBOL_TITLE = 300,                    /* TITLE  */
  YYSYMBOL_TO = 301,                       /* TO  */
  YYSYMBOL_TOP = 302,                      /* TOP  */
  YYSYMBOL_TRIANGULAR = 303,               /* TRIANGULAR  */
  YYSYMBOL_TYPE = 304,                     /* TYPE  */
  YYSYMBOL_UP = 305,                       /* UP  */
  YYSYMBOL_UPDATEALL = 306,                /* UPDATEALL  */
  YYSYMBOL_USE = 307,                      /* USE  */
  YYSYMBOL_VERSION = 308,                  /* VERSION  */
  YYSYMBOL_VERTI = 309,                    /* VERTI  */
  YYSYMBOL_VERTICAL = 310,                 /* VERTICAL  */
  YYSYMBOL_VERTO = 311,                    /* VERTO  */
  YYSYMBOL_VGAP = 312,                     /* VGAP  */
  YYSYMBOL_VIEW = 313,                     /* VIEW  */
  YYSYMBOL_VX1 = 314,                      /* VX1  */
  YYSYMBOL_VX2 = 315,                      /* VX2  */
  YYSYMBOL_VXMAX = 316,                    /* VXMAX  */
  YYSYMBOL_VY1 = 317,                      /* VY1  */
  YYSYMBOL_VY2 = 318,                      /* VY2  */
  YYSYMBOL_VYMAX = 319,                    /* VYMAX  */
  YYSYMBOL_WELCH = 320,                    /* WELCH  */
  YYSYMBOL_WITH = 321,                     /* WITH  */
  YYSYMBOL_WORLD = 322,                    /* WORLD  */
  YYSYMBOL_WRAP = 323,                     /* WRAP  */
  YYSYMBOL_WRITE = 324,                    /* WRITE  */
  YYSYMBOL_WX1 = 325,                      /* WX1  */
  YYSYMBOL_WX2 = 326,                      /* WX2  */
  YYSYMBOL_WY1 = 327,                      /* WY1  */
  YYSYMBOL_WY2 = 328,                      /* WY2  */
  YYSYMBOL_X_TOK = 329,                    /* X_TOK  */
  YYSYMBOL_X0 = 330,                       /* X0  */
  YYSYMBOL_X1 = 331,                       /* X1  */
  YYSYMBOL_XAXES = 332,                    /* XAXES  */
  YYSYMBOL_XAXIS = 333,                    /* XAXIS  */
  YYSYMBOL_XCOR = 334,                     /* XCOR  */
  YYSYMBOL_XMAX = 335,                     /* XMAX  */
  YYSYMBOL_XMIN = 336,                     /* XMIN  */
  YYSYMBOL_XY = 337,                       /* XY  */
  YYSYMBOL_XYAXES = 338,                   /* XYAXES  */
  YYSYMBOL_XYBOXPLOT = 339,                /* XYBOXPLOT  */
  YYSYMBOL_XYCOLOR = 340,                  /* XYCOLOR  */
  YYSYMBOL_XYCOLPAT = 341,                 /* XYCOLPAT  */
  YYSYMBOL_XYDX = 342,                     /* XYDX  */
  YYSYMBOL_XYDXDX = 343,                   /* XYDXDX  */
  YYSYMBOL_XYDXDXDYDY = 344,               /* XYDXDXDYDY  */
  YYSYMBOL_XYDXDY = 345,                   /* XYDXDY  */
  YYSYMBOL_XYDY = 346,                     /* XYDY  */
  YYSYMBOL_XYDYDY = 347,                   /* XYDYDY  */
  YYSYMBOL_XYHILO = 348,                   /* XYHILO  */
  YYSYMBOL_XYR = 349,                      /* XYR  */
  YYSYMBOL_XYSIZE = 350,                   /* XYSIZE  */
  YYSYMBOL_XYSTRING = 351,                 /* XYSTRING  */
  YYSYMBOL_XYVMAP = 352,                   /* XYVMAP  */
  YYSYMBOL_XYZ = 353,                      /* XYZ  */
  YYSYMBOL_Y_TOK = 354,                    /* Y_TOK  */
  YYSYMBOL_Y0 = 355,                       /* Y0  */
  YYSYMBOL_Y1 = 356,                       /* Y1  */
  YYSYMBOL_Y2 = 357,                       /* Y2  */
  YYSYMBOL_Y3 = 358,                       /* Y3  */
  YYSYMBOL_Y4 = 359,                       /* Y4  */
  YYSYMBOL_YAXES = 360,                    /* YAXES  */
  YYSYMBOL_YAXIS = 361,                    /* YAXIS  */
  YYSYMBOL_YEAR = 362,                     /* YEAR  */
  YYSYMBOL_YMAX = 363,                     /* YMAX  */
  YYSYMBOL_YMIN = 364,                     /* YMIN  */
  YYSYMBOL_YYMMDD = 365,                   /* YYMMDD  */
  YYSYMBOL_YYMMDDHMS = 366,                /* YYMMDDHMS  */
  YYSYMBOL_ZERO = 367,                     /* ZERO  */
  YYSYMBOL_ZNORM = 368,                    /* ZNORM  */
  YYSYMBOL_FITPARM = 369,                  /* FITPARM  */
  YYSYMBOL_FITPMAX = 370,                  /* FITPMAX  */
  YYSYMBOL_FITPMIN = 371,                  /* FITPMIN  */
  YYSYMBOL_NUMBER = 372,                   /* NUMBER  */
  YYSYMBOL_NEW_TOKEN = 373,                /* NEW_TOKEN  */
  YYSYMBOL_374_ = 374,                     /* '?'  */
  YYSYMBOL_375_ = 375,                     /* ':'  */
  YYSYMBOL_OR = 376,                       /* OR  */
  YYSYMBOL_AND = 377,                      /* AND  */
  YYSYMBOL_GT = 378,                       /* GT  */
  YYSYMBOL_LT = 379,                       /* LT  */
  YYSYMBOL_LE = 380,                       /* LE  */
  YYSYMBOL_GE = 381,                       /* GE  */
  YYSYMBOL_EQ = 382,                       /* EQ  */
  YYSYMBOL_NE = 383,                       /* NE  */
  YYSYMBOL_384_ = 384,                     /* '+'  */
  YYSYMBOL_385_ = 385,                     /* '-'  */
  YYSYMBOL_386_ = 386,                     /* '*'  */
  YYSYMBOL_387_ = 387,                     /* '/'  */
  YYSYMBOL_388_ = 388,                     /* '%'  */
  YYSYMBOL_UMINUS = 389,                   /* UMINUS  */
  YYSYMBOL_NOT = 390,                      /* NOT  */
  YYSYMBOL_391_ = 391,                     /* '^'  */
  YYSYMBOL_392_ = 392,                     /* '.'  */
  YYSYMBOL_393_ = 393,                     /* ';'  */
  YYSYMBOL_394_ = 394,                     /* '('  */
  YYSYMBOL_395_ = 395,                     /* ')'  */
  YYSYMBOL_396_ = 396,                     /* ','  */
  YYSYMBOL_397_ = 397,                     /* '['  */
  YYSYMBOL_398_ = 398,                     /* ']'  */
  YYSYMBOL_399_ = 399,                     /* '='  */
  YYSYMBOL_YYACCEPT = 400,                 /* $accept  */
  YYSYMBOL_full_list = 401,                /* full_list  */
  YYSYMBOL_multi_list = 402,               /* multi_list  */
  YYSYMBOL_list = 403,                     /* list  */
  YYSYMBOL_expr = 404,                     /* expr  */
  YYSYMBOL_sexpr = 405,                    /* sexpr  */
  YYSYMBOL_iexpr = 406,                    /* iexpr  */
  YYSYMBOL_nexpr = 407,                    /* nexpr  */
  YYSYMBOL_indx = 408,                     /* indx  */
  YYSYMBOL_jdate = 409,                    /* jdate  */
  YYSYMBOL_jrawdate = 410,                 /* jrawdate  */
  YYSYMBOL_array = 411,                    /* array  */
  YYSYMBOL_vexpr = 412,                    /* vexpr  */
  YYSYMBOL_asgn = 413,                     /* asgn  */
  YYSYMBOL_lside_array = 414,              /* lside_array  */
  YYSYMBOL_vasgn = 415,                    /* vasgn  */
  YYSYMBOL_defines = 416,                  /* defines  */
  YYSYMBOL_regionset = 417,                /* regionset  */
  YYSYMBOL_parmset = 418,                  /* parmset  */
  YYSYMBOL_actions = 419,                  /* actions  */
  YYSYMBOL_options = 420,                  /* options  */
  YYSYMBOL_set_setprop = 421,              /* set_setprop  */
  YYSYMBOL_setprop = 422,                  /* setprop  */
  YYSYMBOL_axisfeature = 423,              /* axisfeature  */
  YYSYMBOL_tickattr = 424,                 /* tickattr  */
  YYSYMBOL_ticklabelattr = 425,            /* ticklabelattr  */
  YYSYMBOL_axislabeldesc = 426,            /* axislabeldesc  */
  YYSYMBOL_axisbardesc = 427,              /* axisbardesc  */
  YYSYMBOL_nonlfitopts = 428,              /* nonlfitopts  */
  YYSYMBOL_selectgraph = 429,              /* selectgraph  */
  YYSYMBOL_selectset = 430,                /* selectset  */
  YYSYMBOL_setaxis = 431,                  /* setaxis  */
  YYSYMBOL_axis = 432,                     /* axis  */
  YYSYMBOL_proctype = 433,                 /* proctype  */
  YYSYMBOL_tickspectype = 434,             /* tickspectype  */
  YYSYMBOL_filtertype = 435,               /* filtertype  */
  YYSYMBOL_filtermethod = 436,             /* filtermethod  */
  YYSYMBOL_xytype = 437,                   /* xytype  */
  YYSYMBOL_graphtype = 438,                /* graphtype  */
  YYSYMBOL_pagelayout = 439,               /* pagelayout  */
  YYSYMBOL_pageorient = 440,               /* pageorient  */
  YYSYMBOL_regiontype = 441,               /* regiontype  */
  YYSYMBOL_scaletype = 442,                /* scaletype  */
  YYSYMBOL_onoff = 443,                    /* onoff  */
  YYSYMBOL_runtype = 444,                  /* runtype  */
  YYSYMBOL_sourcetype = 445,               /* sourcetype  */
  YYSYMBOL_justchoice = 446,               /* justchoice  */
  YYSYMBOL_inoutchoice = 447,              /* inoutchoice  */
  YYSYMBOL_formatchoice = 448,             /* formatchoice  */
  YYSYMBOL_signchoice = 449,               /* signchoice  */
  YYSYMBOL_direction = 450,                /* direction  */
  YYSYMBOL_worldview = 451,                /* worldview  */
  YYSYMBOL_datacolumn = 452,               /* datacolumn  */
  YYSYMBOL_sortdir = 453,                  /* sortdir  */
  YYSYMBOL_sorton = 454,                   /* sorton  */
  YYSYMBOL_ffttype = 455,                  /* ffttype  */
  YYSYMBOL_fourierdata = 456,              /* fourierdata  */
  YYSYMBOL_fourierloadx = 457,             /* fourierloadx  */
  YYSYMBOL_fourierloady = 458,             /* fourierloady  */
  YYSYMBOL_windowtype = 459,               /* windowtype  */
  YYSYMBOL_interpmethod = 460,             /* interpmethod  */
  YYSYMBOL_stattype = 461,                 /* stattype  */
  YYSYMBOL_font_select = 462,              /* font_select  */
  YYSYMBOL_lines_select = 463,             /* lines_select  */
  YYSYMBOL_pattern_select = 464,           /* pattern_select  */
  YYSYMBOL_color_select = 465,             /* color_select  */
  YYSYMBOL_linew_select = 466,             /* linew_select  */
  YYSYMBOL_opchoice_sel = 467,             /* opchoice_sel  */
  YYSYMBOL_opchoice = 468,                 /* opchoice  */
  YYSYMBOL_parmset_obs = 469,              /* parmset_obs  */
  YYSYMBOL_axislabeldesc_obs = 470,        /* axislabeldesc_obs  */
  YYSYMBOL_setprop_obs = 471,              /* setprop_obs  */
  YYSYMBOL_tickattr_obs = 472,             /* tickattr_obs  */
  YYSYMBOL_ticklabelattr_obs = 473,        /* ticklabelattr_obs  */
  YYSYMBOL_colpat_obs = 474,               /* colpat_obs  */
  YYSYMBOL_opchoice_sel_obs = 475,         /* opchoice_sel_obs  */
  YYSYMBOL_opchoice_obs = 476              /* opchoice_obs  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  523
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11879

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  400
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  77
/* YYNRULES -- Number of rules.  */
#define YYNRULES  839
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1673

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   638


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   388,     2,     2,
     394,   395,   386,   384,   396,   385,   392,   387,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   375,   393,
       2,   399,     2,   374,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   397,     2,   398,   391,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   376,
     377,   378,   379,   380,   381,   382,   383,   389,   390
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   623,   623,   624,   628,   635,   636,   639,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   658,
     661,   664,   667,   670,   673,   680,   714,   722,   725,   728,
     731,   734,   738,   742,   746,   750,   754,   758,   762,   766,
     770,   774,   778,   782,   786,   789,   792,   795,   798,   801,
     804,   807,   810,   813,   816,   820,   827,   834,   841,   848,
     855,   862,   869,   876,   881,   886,   889,   892,   895,   898,
     901,   904,   913,   921,   932,   935,   938,   941,   944,   947,
     950,   953,   956,   959,   964,   967,   971,   980,   990,   999,
    1009,  1012,  1027,  1030,  1046,  1050,  1063,  1079,  1083,  1105,
    1123,  1141,  1157,  1186,  1196,  1206,  1216,  1231,  1242,  1253,
    1264,  1275,  1286,  1297,  1308,  1319,  1334,  1345,  1356,  1371,
    1382,  1393,  1408,  1419,  1430,  1449,  1464,  1479,  1499,  1514,
    1530,  1553,  1572,  1591,  1601,  1610,  1623,  1636,  1649,  1664,
    1675,  1686,  1701,  1712,  1723,  1738,  1749,  1760,  1775,  1786,
    1797,  1812,  1823,  1834,  1849,  1860,  1871,  1886,  1897,  1908,
    1923,  1934,  1945,  1955,  1965,  1978,  1982,  1986,  1990,  1994,
    2005,  2032,  2043,  2053,  2070,  2078,  2089,  2093,  2097,  2102,
    2108,  2126,  2129,  2136,  2147,  2150,  2153,  2156,  2159,  2162,
    2169,  2177,  2184,  2194,  2197,  2200,  2215,  2229,  2243,  2257,
    2272,  2276,  2279,  2282,  2285,  2288,  2291,  2294,  2297,  2301,
    2306,  2310,  2314,  2317,  2322,  2326,  2331,  2334,  2341,  2348,
    2355,  2365,  2368,  2371,  2374,  2377,  2380,  2383,  2414,  2417,
    2424,  2431,  2438,  2448,  2451,  2454,  2457,  2460,  2463,  2466,
    2497,  2500,  2507,  2514,  2521,  2531,  2534,  2537,  2540,  2543,
    2546,  2549,  2552,  2556,  2573,  2576,  2583,  2590,  2597,  2605,
    2608,  2611,  2614,  2617,  2620,  2623,  2639,  2642,  2645,  2648,
    2651,  2654,  2658,  2664,  2668,  2672,  2676,  2679,  2683,  2687,
    2690,  2694,  2702,  2715,  2725,  2728,  2738,  2746,  2753,  2760,
    2767,  2775,  2782,  2789,  2797,  2804,  2811,  2818,  2825,  2828,
    2831,  2834,  2838,  2847,  2851,  2858,  2865,  2872,  2879,  2886,
    2893,  2900,  2907,  2914,  2921,  2928,  2935,  2943,  2950,  2957,
    2965,  2972,  2979,  2986,  2993,  3000,  3007,  3015,  3024,  3027,
    3030,  3033,  3037,  3041,  3044,  3048,  3052,  3056,  3060,  3065,
    3072,  3076,  3079,  3082,  3083,  3089,  3092,  3099,  3103,  3107,
    3114,  3117,  3120,  3123,  3126,  3133,  3136,  3141,  3163,  3168,
    3176,  3183,  3188,  3200,  3203,  3206,  3209,  3213,  3218,  3223,
    3226,  3236,  3239,  3242,  3245,  3248,  3251,  3255,  3258,  3261,
    3264,  3267,  3273,  3279,  3286,  3298,  3301,  3304,  3307,  3327,
    3331,  3335,  3338,  3341,  3344,  3347,  3364,  3379,  3384,  3389,
    3394,  3397,  3400,  3408,  3413,  3417,  3421,  3425,  3438,  3441,
    3447,  3454,  3458,  3462,  3469,  3477,  3485,  3494,  3502,  3506,
    3509,  3513,  3516,  3519,  3522,  3526,  3529,  3532,  3535,  3538,
    3545,  3550,  3553,  3556,  3559,  3562,  3569,  3570,  3574,  3577,
    3581,  3584,  3587,  3590,  3593,  3596,  3599,  3602,  3605,  3608,
    3611,  3615,  3619,  3623,  3627,  3631,  3636,  3640,  3644,  3662,
    3682,  3686,  3691,  3696,  3700,  3704,  3708,  3712,  3716,  3720,
    3724,  3728,  3732,  3737,  3743,  3746,  3749,  3752,  3755,  3758,
    3761,  3764,  3767,  3770,  3773,  3777,  3782,  3790,  3797,  3804,
    3805,  3806,  3807,  3808,  3809,  3810,  3811,  3822,  3829,  3836,
    3843,  3851,  3858,  3865,  3872,  3879,  3886,  3893,  3900,  3907,
    3914,  3921,  3928,  3935,  3942,  3949,  3956,  3963,  3970,  3977,
    3984,  3992,  4003,  4010,  4017,  4024,  4031,  4039,  4047,  4054,
    4061,  4068,  4075,  4084,  4091,  4098,  4105,  4112,  4119,  4126,
    4133,  4140,  4147,  4162,  4169,  4176,  4187,  4195,  4202,  4209,
    4216,  4223,  4231,  4238,  4245,  4252,  4259,  4269,  4276,  4283,
    4290,  4300,  4304,  4308,  4311,  4317,  4321,  4328,  4341,  4354,
    4367,  4383,  4384,  4388,  4389,  4390,  4391,  4395,  4396,  4397,
    4398,  4399,  4400,  4401,  4402,  4403,  4404,  4405,  4406,  4410,
    4411,  4412,  4416,  4417,  4421,  4422,  4426,  4427,  4428,  4429,
    4430,  4431,  4432,  4433,  4434,  4435,  4436,  4437,  4438,  4439,
    4440,  4441,  4442,  4443,  4444,  4448,  4449,  4450,  4451,  4452,
    4453,  4457,  4458,  4462,  4463,  4467,  4468,  4469,  4470,  4471,
    4472,  4473,  4474,  4475,  4476,  4479,  4480,  4481,  4482,  4485,
    4486,  4489,  4490,  4491,  4492,  4493,  4497,  4498,  4508,  4509,
    4510,  4513,  4514,  4515,  4518,  4519,  4520,  4521,  4522,  4523,
    4524,  4525,  4526,  4527,  4528,  4529,  4530,  4531,  4532,  4533,
    4534,  4535,  4536,  4537,  4538,  4539,  4540,  4541,  4542,  4543,
    4544,  4545,  4546,  4547,  4548,  4549,  4552,  4553,  4554,  4557,
    4558,  4559,  4560,  4561,  4562,  4565,  4566,  4569,  4570,  4571,
    4572,  4573,  4574,  4575,  4576,  4579,  4580,  4583,  4584,  4587,
    4588,  4589,  4590,  4594,  4595,  4599,  4600,  4601,  4605,  4606,
    4607,  4611,  4612,  4613,  4614,  4615,  4616,  4617,  4621,  4622,
    4623,  4626,  4627,  4628,  4629,  4630,  4631,  4632,  4636,  4640,
    4648,  4661,  4674,  4684,  4694,  4713,  4728,  4734,  4735,  4736,
    4741,  4753,  4756,  4759,  4763,  4766,  4771,  4773,  4775,  4777,
    4779,  4780,  4782,  4791,  4798,  4805,  4813,  4814,  4815,  4816,
    4818,  4820,  4824,  4828,  4834,  4840,  4845,  4850,  4857,  4864,
    4871,  4878,  4886,  4893,  4900,  4907,  4915,  4918,  4926,  4928,
    4931,  4935,  4936,  4937,  4938,  4943,  4944,  4954,  4968,  4972,
    4995,  5003,  5004,  5007,  5010,  5013,  5014,  5015,  5018,  5023,
    5031,  5032,  5033,  5034,  5035,  5036,  5043,  5052,  5064,  5071,
    5079,  5089,  5090,  5099,  5106,  5108,  5115,  5122,  5123,  5124,
    5131,  5153,  5154,  5155,  5158,  5164,  5165,  5166,  5167,  5168
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "KEY_VAR", "KEY_VEC",
  "KEY_CONST", "KEY_UNIT", "KEY_FUNC_I", "KEY_FUNC_D", "KEY_FUNC_NN",
  "KEY_FUNC_ND", "KEY_FUNC_DD", "KEY_FUNC_NND", "KEY_FUNC_PPD",
  "KEY_FUNC_PPPD", "KEY_FUNC_PPPPD", "KEY_FUNC_PPPPPD", "INDEX", "DATE",
  "VAR_D", "VEC_D", "CONSTANT", "UCONSTANT", "FUNC_I", "FUNC_D", "FUNC_NN",
  "FUNC_ND", "FUNC_DD", "FUNC_NND", "FUNC_PPD", "FUNC_PPPD", "FUNC_PPPPD",
  "FUNC_PPPPPD", "ABOVE", "ABSOLUTE", "ALIAS", "ALT", "ALTXAXIS",
  "ALTYAXIS", "ANGLE", "ANTIALIASING", "APPEND", "ARRANGE", "ARROW",
  "ASCENDING", "ASPLINE", "AUTO", "AUTOSCALE", "AUTOTICKS", "AVALUE",
  "AVG", "BACKGROUND", "BAR", "BARDY", "BARDYDY", "BASELINE", "BATCH",
  "BEGIN", "BELOW", "BETWEEN", "BLACKMAN", "BLOCK", "BOTH", "BOTTOM",
  "BOX", "CD", "CENTER", "CHAR", "CHART", "CHRSTR", "CLEAR", "CLICK",
  "CLIP", "CLOSE", "COEFFICIENTS", "COLOR", "COMMENT", "COMPLEX",
  "COMPUTING", "CONSTRAINTS", "COPY", "CYCLE", "DAYMONTH", "DAYOFWEEKL",
  "DAYOFWEEKS", "DAYOFYEAR", "DDMMYY", "DECIMAL", "DEF", "DEFAULT",
  "DEFINE", "DEGREESLAT", "DEGREESLON", "DEGREESMMLAT", "DEGREESMMLON",
  "DEGREESMMSSLAT", "DEGREESMMSSLON", "DESCENDING", "DESCRIPTION",
  "DEVICE", "DFT", "DIFFERENCE", "DISK", "DOWN", "DPI", "DROP", "DROPLINE",
  "ECHO", "ELLIPSE", "ENGINEERING", "ERRORBAR", "EXIT", "EXPONENTIAL",
  "FFT", "FILEP", "FILL", "FIT", "FIXED", "FIXEDPOINT", "FLUSH", "FOCUS",
  "FOLLOWS", "FONTP", "FORCE", "FORMAT", "FORMULA", "FRAMEP", "FREE",
  "FREQUENCY", "FROM", "GENERAL", "GETP", "GRAPH", "GRAPHNO", "GRID",
  "HAMMING", "HANNING", "HARDCOPY", "HBAR", "HELP", "HGAP", "HIDDEN",
  "HISTOGRAM", "HMS", "HORIZI", "HORIZONTAL", "HORIZO", "ID", "IFILTER",
  "IMAX", "IMIN", "IN", "INCREMENT", "INOUT", "INT", "INTEGRATE",
  "INTERPOLATE", "INVDFT", "INVERT", "INVFFT", "JUST", "KILL", "LABEL",
  "LANDSCAPE", "LAYOUT", "LEFT", "LEGEND", "LENGTH", "LINCONV", "LINE",
  "LINEAR", "LINESTYLE", "LINEWIDTH", "LINK", "LOAD", "LOCTYPE", "LOG",
  "LOGARITHMIC", "LOGIT", "LOGX", "LOGXY", "LOGY", "MAGIC", "MAGNITUDE",
  "MAJOR", "MAP", "MAXP", "MESH", "MINP", "MINOR", "MMDD", "MMDDHMS",
  "MMDDYY", "MMDDYYHMS", "MMSSLAT", "MMSSLON", "MMYY", "MONTHDAY",
  "MONTHL", "MONTHS", "MONTHSY", "MOVE", "NEGATE", "NEW", "NONE",
  "NONLFIT", "NORMAL", "NXY", "OFF", "OFFSET", "OFFSETX", "OFFSETY",
  "OFILTER", "ON", "ONREAD", "OP", "OPPOSITE", "OUT", "PAGE", "PARA",
  "PARAMETERS", "PARZEN", "PATTERN", "PERIOD", "PERP", "PHASE", "PIE",
  "PIPE", "PLACE", "POINT", "POLAR", "POLYI", "POLYO", "POP", "PORTRAIT",
  "POWER", "PREC", "PREPEND", "PRINT", "PS", "PUSH", "PUTP", "RAND",
  "READ", "REAL", "RECIPROCAL", "REDRAW", "REFERENCE", "REGNUM", "REGRESS",
  "RESIZE", "RESTRICT", "REVERSE", "RIGHT", "RISER", "ROT", "ROUNDED",
  "RSUM", "RULE", "RUNAVG", "RUNMAX", "RUNMED", "RUNMIN", "RUNSTD",
  "SAVEALL", "SCALE", "SCIENTIFIC", "SCROLL", "SD", "SET", "SETNUM",
  "SFORMAT", "SIGN", "SIZE", "SKIP", "SLEEP", "SMITH", "SORT", "SOURCE",
  "SPEC", "SPLINE", "SPLIT", "STACK", "STACKED", "STACKEDBAR",
  "STACKEDHBAR", "STAGGER", "START", "STOP", "STRING", "SUM", "SUBTITLE",
  "SWAP", "SYMBOL", "TARGET", "TICKLABEL", "TICKP", "TICKSP", "TIMER",
  "TIMESTAMP", "TITLE", "TO", "TOP", "TRIANGULAR", "TYPE", "UP",
  "UPDATEALL", "USE", "VERSION", "VERTI", "VERTICAL", "VERTO", "VGAP",
  "VIEW", "VX1", "VX2", "VXMAX", "VY1", "VY2", "VYMAX", "WELCH", "WITH",
  "WORLD", "WRAP", "WRITE", "WX1", "WX2", "WY1", "WY2", "X_TOK", "X0",
  "X1", "XAXES", "XAXIS", "XCOR", "XMAX", "XMIN", "XY", "XYAXES",
  "XYBOXPLOT", "XYCOLOR", "XYCOLPAT", "XYDX", "XYDXDX", "XYDXDXDYDY",
  "XYDXDY", "XYDY", "XYDYDY", "XYHILO", "XYR", "XYSIZE", "XYSTRING",
  "XYVMAP", "XYZ", "Y_TOK", "Y0", "Y1", "Y2", "Y3", "Y4", "YAXES", "YAXIS",
  "YEAR", "YMAX", "YMIN", "YYMMDD", "YYMMDDHMS", "ZERO", "ZNORM",
  "FITPARM", "FITPMAX", "FITPMIN", "NUMBER", "NEW_TOKEN", "'?'", "':'",
  "OR", "AND", "GT", "LT", "LE", "GE", "EQ", "NE", "'+'", "'-'", "'*'",
  "'/'", "'%'", "UMINUS", "NOT", "'^'", "'.'", "';'", "'('", "')'", "','",
  "'['", "']'", "'='", "$accept", "full_list", "multi_list", "list",
  "expr", "sexpr", "iexpr", "nexpr", "indx", "jdate", "jrawdate", "array",
  "vexpr", "asgn", "lside_array", "vasgn", "defines", "regionset",
  "parmset", "actions", "options", "set_setprop", "setprop", "axisfeature",
  "tickattr", "ticklabelattr", "axislabeldesc", "axisbardesc",
  "nonlfitopts", "selectgraph", "selectset", "setaxis", "axis", "proctype",
  "tickspectype", "filtertype", "filtermethod", "xytype", "graphtype",
  "pagelayout", "pageorient", "regiontype", "scaletype", "onoff",
  "runtype", "sourcetype", "justchoice", "inoutchoice", "formatchoice",
  "signchoice", "direction", "worldview", "datacolumn", "sortdir",
  "sorton", "ffttype", "fourierdata", "fourierloadx", "fourierloady",
  "windowtype", "interpmethod", "stattype", "font_select", "lines_select",
  "pattern_select", "color_select", "linew_select", "opchoice_sel",
  "opchoice", "parmset_obs", "axislabeldesc_obs", "setprop_obs",
  "tickattr_obs", "ticklabelattr_obs", "colpat_obs", "opchoice_sel_obs",
  "opchoice_obs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,    63,    58,   629,   630,   631,   632,
     633,   634,   635,   636,    43,    45,    42,    47,    37,   637,
     638,    94,    46,    59,    40,    41,    44,    91,    93,    61
};
#endif

#define YYPACT_NINF (-900)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-171)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    3356,  -900,  -289,  -319,   -53,  -900,  -274,  -264,  -256,  -248,
    -241,  -234,  -197,  -160,  -124,   -87,    20,  -900,  -900,   315,
     -80,    92,    79,  -900,  -900,   271,  7045,  5886,   289,    97,
     289,   315,  -900,   295,   -13,   289,   289,  -900,   -30,  9479,
    6077,   -19,  -900,   -71,  -900,   493, 11486,   202,   289,   -16,
    -900,   270,   289,   -11,  -900,  -900,    -3,     3,    14,  -900,
    -900,   -39,  4977,    19,  6268,  -144,   289,   -32,  -900,    25,
    -900,   315,   259,    31,  5250,  -900,    93,   254,  -900,   289,
      34,  4286,  -900,   413,   -50,    39,    43,   315,    44,  -900,
    -900,  -900,  -900,  -900,   289,  -900,   -16,  -900, 10864,   315,
     -58,   315,  9711,  5506,  -900,   111,   315,   315, 10864,  6459,
     169,  7045,  -900,   289, 10864,  7521,  -900,  -900,  -900,  -900,
    -900,  -900,   679,  7802,   315,  -900,  -900,  -900,  -900,  -900,
    -900,   -93,  -900,    46,  -900,  -900,  -900,  -900,  -900,  -900,
     -89,  -900, 10864,   -64,    47,    50,  -900, 10864,  8179,  8179,
    8179,   444,    71,  -900,  3199,     4,  4936,  -900,    69,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  6947, 11479,  -900,
      -7,    78,  -900,    80,    82,  -900,  -900,  -166,  9479, 10864,
   10864,  8179,  8179, 10864, 10864,  8179, 10864, 10864, 10864, 10864,
   10864,  -900,    60,   -60,    86,   180, 10864,    60,   -48,  -900,
    -900,  -900,  9856,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,   289,    88,  -900,  -900,    89,    91,   109,
     110,   114,   120,   121,   122,   124,  -900,   194, 10864, 10864,
     -27,  -900,  -900,  -900,  -900,  -900,  -900, 10864, 10864, 10864,
     370,  -291,   149,   151,  -900,  -900,  -900,  -900,   176,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
     176,   275,   304,  8578, 10864,   289,   305,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,   183,   289,   176,   -81,   315,  5319,
     176,   149,  -900,   194,   -27,   458,   149,  -900,  -900,  -900,
    -900, 10864,   289, 10864,   289, 10864,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,    -4,    60, 10864,  -900,  -900,
    -900,  -900,  -900,   176, 10864,  -900,  9479,   176,   315,  8179,
     315,   315,  -900,  -900,    86,   322,   685,   316,  9479, 10864,
      60, 10864, 10864,   -27, 10864, 10864, 10864, 10864,   633,  -900,
    -900,  -900,  -900,  -900,   315,  7947,  -900,   -27,   742,   149,
    -900,  -900,  -900,  -900,    60,   296,   176, 10864, 10864, 10864,
     297,   289,   315,   475,  -900,  -900, 10864,   273,  -900,  -900,
   10864,  -900, 10864, 11009,  -900,  5319,  -900,  -900,  -900,     9,
    8725,  -900,   176, 10864,   289,    75,    75,   176,    75,  9479,
   10864,   134, 10864,   315,  -900,  -900,  -900,  -900,   315,   315,
    -900,  8179,   176,  -900,  5319,  -262,  -900,  -900,  -900, 10864,
   10864,  -900,   328,   289, 10864,   -27, 10864,   831,   149,  -900,
    -900,  -900,  -900, 10864,   176,  -900,  -900,  -900,   225,  -900,
    -900,   334,   289, 10864,   864,  -900,  -900,  -900,  -900, 10864,
     176,  -900,  -900,  -900,  -900,  -285,  -900, 10864, 10864, 10864,
   10864,   920, 10864, 10864, 10864, 10864,    86,  -900, 10864, 10864,
   10864, 10864,  1126,   -41,    60,   148,   315,    60,   148,  5319,
      60, 10864, 10864, 10864,   218,   219,   227,   -82,   230,   227,
     230,  5995,  6185,  -900,  3718,  -900, 10864,  8179,  8179,  8179,
    8179,  8179,  8179,  8179,  8179,  8179,  8179,  8179,  8179,  8179,
    8179,   445, 10864,   229,  -900,  8179,  8179,  8179,  8179,  8179,
    8179,  8179,  8179,  8179,  8179,  8179,  8179,  8179,  8179,  8179,
    8179,   326,   491,   -72,    60,    60,    60,    45,   556,    -7,
    -900,   828,  -129,   289, 10864,    60,  1949,  7111,    60,   289,
   10864,    36,   278, 10864, 10864,  5697,  7045, 10864,   166,  -900,
    -900,  -900,  -900,   402,   730, 10864,  4436,  4749,   265,  -900,
    -900,   315,   315,  8179, 10864,  -900,  6377,   176,   247,   245,
    5319,  -900,  3199,   250,  7669,  8010,  8407,   255,   256,  1327,
    1372,   257,  1398,  1436,  1463,  1519,  -900,  9479,   176,    54,
     315,   260,  -900,  -900,  -900,  -900,  -900, 10864,   176,  -900,
     176, 10864, 10864, 10864, 10864, 10864, 10864, 10864, 10864, 10864,
    9856,  -900, 10864,  -900,  -900,  -900,  -900,  5319,  -900,  -900,
    -900,   218,   218,  8430, 10864, 10864, 10864, 10864, 10864, 10864,
   10864, 10864, 10864, 10864, 10864, 10864, 10864, 10864, 10864,  -900,
     315, 10864, 10864,   176,  -900,  -900,   176, 10864,  4035,  -156,
   10864,     6,   289,   377,   261,  -900,  -900,  -900,  -900, 10864,
     263,   176,  5319,   176,   441,  -900,  -900,  -900,  -900,   264,
    5319,   176,   267,  3199,  1580,   284,   287,  -900,   -54,  -900,
    -900,  -900,  -900,  -900, 10864,  -900,  -900,  5319,  -900,  -900,
     289,  -900,  5319,  5319, 10864,   288, 10864, 10864, 10864, 10864,
    -900,  -900, 10864,  -900,   -33,   379,   384,  1604,   291,   315,
     176,   292,    60,  7907,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,   293,  8140,   320,   299,  -900,   176, 10864, 10864,   298,
     176,   176,   289,   176,   289,   176,   289,  5319,   176,  -900,
    1625,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  1659,   301,   302,   306,  8450,  -900,  -900,    -6,
    -900,  1694, 10864,   176,  -900,  -900,  -900, 10864,  5319,   315,
   10864,   176,  -900, 10864,  5319,   987,  5319,  5319,  5319,  5319,
   10864,  -900,  -900,  -900,  -900,  5319,  5319,  5319,  5319, 10864,
     289,   289,  -900,  -900,  -900,  -900,  -900,  -900,   309,  -900,
    -900,  -900,  5319,  5319,  5319,  -900,  -900,   374,  -319,     2,
     -64,    47,    50,  -900,   -16,  9558, 11487,  3318,    74,   600,
    1068,  1503,  2039,  2066,  2039,  2066,  2039,  2066,  2039,  2066,
    2039,  2066,  2039,  2066,   106,   205,   106,   205,   227,   230,
     227,   230,   227,   230,   227,   230,  -900,  -326, 10864,  4141,
    4874,    74,   600,  1068,  1503,  2039,  2066,  2039,  2066,  2039,
    2066,  2039,  2066,  2039,  2066,  2039,  2066,   106,   205,   106,
     205,   227,   230,   227,   230,   227,   230,   227,   230,  3199,
    4936,   -28, 10864, 11486, 10864, 10864, 10864,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,   -16,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,   289,   427, 11486,
   10864, 10864,   289, 10864, 10864,  -900,  -900,  -900, 10864,  -900,
     176,   311,  -900, 10864,   179,   398, 10864,   -26,  -900,  -900,
    -900,  -900,  -900,  -900, 10864, 10864,   280,  -900,  -900,  -900,
    -900,   176,  -900, 10864,  -900,  -900,  -900,  -900,  -900,   289,
    1814,  -900,    60, 10110,  8957, 10864, 10864,  -900,  -900,  -900,
    -900,  -900,  -900,  1890,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,   435,   -36,   118,   -26,  7425,   176,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  1925, 10864,   289,   437,  4180,
     289,    68,  9102,   359, 10864,   289,    -2, 10864, 10864, 10255,
   10487,   -17,   329,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,    60,  -900, 10864,  -900,    60,  6920,   356,   357,  6692,
   10864, 10864,  -900,    40, 10864, 10632,   -15,   336,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,    60,   337,   339,  8640,
    -900, 10864,  -900,  -900,  -900,  -900,  -900, 10864,  8179,  8179,
    8179, 10864, 10864, 10864, 10864, 10864,  5319,  -900,  -900, 10864,
    8430,   343,  8682,   344,  1998,   363,  2122,  2196,  2233,  2257,
    1018,  2453,  3526,  3526,  3526,  3526,  3526,  3526,   248,   248,
     218,   218,   218,   218,  2305,  -900,  5319,  5319,  5319,  -900,
     365,  -900,  -900,   289,  5319,    60,  -900,   176, 10864, 10864,
    2475,  -900,  -900,  -900,  8179,  8179,  -900,  8179,   280,  -900,
    -900,  -900,  5319,   176,  5319,   315,  2502,  5319,  5319,  -900,
    2540,  -900,   371,   289, 10864,  -900,  -900,  8179,  -900, 10864,
   10864, 10864,  -900, 10864,  5319,  5319,  -900,   176,   176,   176,
   10864, 10864,  -900, 10864,  8324,  -900,  -900,  -900,  -900, 10864,
    5319,  5319,  -900,  5319,  5319,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,   640,  2573,  2600,
    -107,   176,   315,   229,   562, 10864, 10864,  5319,  8179,  8179,
    -900,  -900,  5319, 11486,  2639,  -900,  2676,  -900,   176, 10864,
    -900,  2699,  -900,   176,  -900,  -900,  -900, 10864,  5319,  -900,
    -900,  -900,  -900,   -10,  -900,  -900,  -900,  5319,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
     176, 10864,  -900,  -900,  -900,  -900,  -900,  -900,  5319,  -900,
   10864, 10864,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  2753, 10864,  -900,   176, 10864,  5319,  -900,   176,  -900,
    -900,  -900,  -900,  -900,  2805,   473,   476,  -900,   176,  -900,
    -900,  -900,  -900,  -900,  -900,    15,  5319,    22,  5319,  -900,
    -900,   289,  -900,  -900,  -900,    60, 10864,  2883,   376,  -900,
    -900,  -900,  -900,  -900,  -900,    60, 10864, 10864,  2883,   378,
    -900,  -900,  -900,  -900,  5319,  5319,    60,  5319,   -34,  -900,
    -900,  -900, 10864,  -900, 10864,  9334,  -900,   381,   380,  8777,
    8827,  9154,  9176,  9206,  9550,   382,  2920,  2943,  3717,  3740,
     383, 10864, 10864, 10864, 10864, 10864, 10864, 10864, 10864, 10864,
    -900,   176,  -900,   385,   387, 10864,  3763,  9578,  3858,  -900,
     388, 10864, 10864, 10864,  -310,  4305,   389,  4463,  -900,  -900,
    4492,  5319,   390,    13,  9732,  4531,   289, 10864, 10864,   289,
     394,  8538,   395,  8917,  9063,  8917,  9063,  -900, 10864, 10864,
    5319, 10864,  -900, 10864,  -900,  5319,  5319,  5319, 10864,  5319,
    5319, 10864,  -900,  -900,  -900,  -900,  -900,  -900,   176,  -900,
    5319, 10864,  -900,  5319,  -900, 10864,  -900,  -900,  -900,  -900,
    -900,  5319,   391,  -900,  -900,   399,   405, 10864,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  8179,  8179, 10864, 10864, 10864,
   10864,   407,  9945,  9977,   408,  4565,  4706,  4790,  4830,  5073,
   10864,  -900,  5094,    60,  -900,   -25,  -900,  5319,  5358,   410,
     289, 10864,  -900, 10864, 10864,  -900,    60,  -900, 10864,   -31,
    5403,  5564,   176, 10864,  -900,  5319,  5319,  5319,  5319,  5319,
    5319,  5319,  5319,  -900,  -900,    16,  -202, 10068, 10170, 10212,
   10312,  5738,  5946,  6120,  6299, 10864, 10864, 10864, 10864, 10864,
   10864, 10864,  -900, 10864,   412,  -900,  -900,  -900,   419, 10864,
   10864,   176,   400,   403,  6416,   423,  6505,   289, 10864, 10864,
     425,  -900,  -900,  -900,  -900,  -900,  -900,  -900,   428,  -900,
   10864,  -900,  -900,  -900,  -900,  8179, 10864, 10864, 10864,   424,
   10354, 10684,  6530,  6649,  6789,  5319,  5319,    60,    60,  5319,
     431,  -900,  -900, 10864,  -900, 10864,   176,  5319,  5319,    60,
      38,   432, 10704, 10731,  6818,  6941,  6964,  -900, 10864, 10864,
   10864,   452,   455, 10864,  5319,  1348,   460,  -900,  -900,  -900,
     434, 10864,  -900,  -900,  8179, 10864, 10864, 10885,  7130,  7204,
    -900,  -900,   463, 10864,  -900,   244,   464, 10907, 11031,  7241,
     335, 10864, 10864,   465,  7340,  -900,  -900,  -900,   467, 10864,
    -900,  -900,  8179,  -900,    60, 11051,  7540,   289, 10864,  -900,
   11071, 11098, 11118,   468, 10864,   176,  7622,  -900,  -900,  -900,
      60, 11138, 10864,   469,  7643,    60, 10864,   -45,  5319,  -900,
      60,   472,  -900
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,    18,     0,    20,    94,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,   576,     0,
       0,     0,   397,   401,   733,     0,     0,     0,     0,     0,
     350,     0,   423,     0,     0,     0,     0,   709,     0,     0,
       0,   352,   710,     0,   379,     0,     0,     0,     0,     0,
     565,     0,   360,     0,   737,   736,     0,     0,     0,   711,
     712,     0,     0,     0,     0,     0,     0,     0,   732,     0,
     731,     0,   419,     0,     0,   422,   354,     0,   421,     0,
      33,     0,   345,     0,     0,     0,     0,     0,     0,   641,
     644,   643,   645,   642,     0,   734,     0,   569,     0,     0,
       0,     0,     0,     0,   735,     0,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,    55,    56,    63,    57,
      58,    64,     0,     0,     0,    59,    60,    61,    62,   697,
     699,     0,   573,     0,   698,   700,   701,   702,   703,   704,
       0,   574,     0,    21,    22,    23,    19,     0,     0,     0,
       0,     0,     2,     5,     3,    97,     4,    15,     0,    16,
      17,    10,     8,    13,    14,    12,   436,     0,     0,    11,
       0,     0,    95,     0,     0,     9,   437,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,   398,
     399,   400,     0,   204,   597,   598,   599,   596,   613,   610,
     611,   601,   603,   606,   605,   602,   604,   607,   608,   609,
     614,   612,   600,     0,     0,    20,    94,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,   640,   639,    33,    21,    22,    23,     0,     0,     0,
       0,     0,   219,     0,   218,   222,   224,   223,   347,   178,
     179,   426,   303,   427,   592,   428,   593,   425,   429,   340,
     351,     0,     0,     0,     0,     0,     0,   278,   273,   276,
     275,   274,   176,   177,   173,     0,   302,     0,     0,   349,
     348,     0,   239,     0,     0,     0,   231,   230,   234,   236,
     235,     0,     0,     0,     0,     0,   343,   435,   434,   433,
     402,   432,   659,   667,   672,   671,   673,   661,   654,   682,
     678,   683,   679,   684,   680,   658,   655,   656,   674,   665,
     675,   662,   676,   685,   681,   664,   666,   670,   668,   669,
     660,   657,   663,   677,   342,     0,     0,     0,   320,   322,
     325,   324,   323,   361,     0,   566,     0,   359,     0,     0,
       0,     0,   408,   378,   377,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   701,     0,   304,
     318,   768,   319,   769,     0,     0,   253,     0,     0,   243,
     242,   247,   248,   246,     0,     0,   418,     0,     0,     0,
       0,     0,     0,     0,   690,   693,     0,     0,   692,   694,
       0,   691,     0,     0,   689,    87,    88,   752,   357,     0,
       0,   794,   362,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,   184,   187,   186,   188,     0,     0,
     371,     0,   417,   570,   358,     0,   646,   647,   341,     0,
       0,   424,     0,     0,     0,     0,     0,     0,   257,   256,
     262,   260,   758,     0,   290,   291,   293,   761,     0,   211,
     210,     0,     0,     0,     0,   266,   267,   270,   759,     0,
     286,   287,   289,   760,   338,     0,   192,     0,     0,     0,
       0,     0,   216,   228,   240,   254,   212,   213,     0,     0,
       0,     0,     0,   413,     0,     0,     0,     0,     0,   284,
       0,     0,     0,     0,    69,     0,    68,    97,   164,    83,
     162,     0,     0,     1,     0,    32,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
     802,   804,   803,     0,     0,     0,     0,     0,     0,   571,
     487,     0,     0,     0,     0,   202,    87,    91,     0,     0,
     165,   365,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,   180,     0,
       0,     0,   431,   298,   299,   301,   300,     0,   743,   742,
     407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     832,   831,   833,   226,   225,   756,   740,   745,   696,   695,
     221,    68,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,   739,   738,   741,   280,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   237,   757,   233,     0,
       0,   562,   564,   561,     0,   327,   326,   787,   321,     0,
     791,   200,     0,     0,     0,     0,     0,   376,     0,   762,
     314,   313,   312,   315,     0,   307,   309,   786,   308,   305,
       0,   306,   763,   764,     0,     0,     0,     0,     0,     0,
     249,   245,     0,   208,     0,     0,     0,    87,     0,     0,
     420,     0,     0,    87,   753,   622,   621,   623,   624,   430,
     750,     0,     0,    19,     0,   355,   356,     0,     0,     0,
     404,   405,     0,   411,     0,   409,     0,    92,    93,   201,
       0,   625,   626,   631,   633,   627,   629,   630,   628,   632,
     634,   185,     0,     0,     0,     0,     0,   707,   708,     0,
     372,     0,     0,   265,   263,   259,   261,     0,   292,     0,
       0,   272,   269,     0,   288,     0,   783,   782,   785,   784,
       0,   217,   229,   241,   255,   779,   778,   781,   780,     0,
       0,     0,   296,   636,   638,   635,   637,   294,     0,   297,
     295,   344,   166,   167,   168,    65,   163,     0,     0,     0,
       0,     0,     0,     6,   170,     0,     0,     0,    82,   140,
      81,   143,    75,   146,    76,   149,    77,   155,    78,   152,
      79,   158,    80,   161,    66,   117,    67,   120,    70,   123,
      71,   126,    72,   129,    73,   132,    27,     0,     0,     0,
       0,   139,   138,   142,   141,   145,   144,   148,   147,   154,
     153,   151,   150,   157,   156,   160,   159,   116,   115,   119,
     118,   122,   121,   125,   124,   128,   127,   131,   130,   172,
     171,     0,     0,     0,     0,     0,     0,   333,   329,   770,
     331,   774,   616,   619,   775,   771,   773,   772,   620,   617,
     618,   776,   777,   615,   330,    30,     0,   567,    44,    45,
      46,    47,    48,    49,    50,    51,   572,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   466,   467,     0,   460,
     485,     0,   462,     0,     0,     0,     0,     0,   474,   480,
     477,   476,   479,   475,     0,     0,     0,   799,   459,   458,
     363,   486,   364,     0,   452,   455,   454,   453,   215,     0,
       0,   798,     0,     0,     0,     0,     0,   440,   444,   442,
     441,   443,   439,     0,    29,    28,    96,   495,   557,   559,
     558,   560,     0,     0,     0,     0,     0,   546,   493,   554,
     555,   795,   556,   494,   796,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   522,   540,   541,   821,   531,   492,
     829,     0,   653,     0,   651,     0,     0,   732,   731,     0,
       0,     0,   652,     0,     0,     0,     0,     0,   489,   497,
     504,   507,   510,   517,   490,   820,     0,     0,     0,     0,
     203,     0,    52,    34,   104,    35,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    85,   370,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,    81,    75,    76,    77,    78,    79,    80,    66,    67,
      70,    71,    72,    73,     0,   369,   277,   754,   279,   174,
       0,   594,   595,     0,   196,     0,   198,   199,     0,     0,
       0,   353,   563,    89,     0,     0,   392,     0,     0,   766,
     311,   310,   317,   765,   316,     0,     0,   250,   790,   251,
       0,   191,     0,     0,     0,    99,   373,     0,   205,   207,
       0,   206,   751,     0,   792,   793,   101,   406,   412,   410,
       0,     0,   102,     0,     0,   103,   705,   706,   368,     0,
     264,   258,   374,   268,   271,   577,   578,   579,   580,   582,
     581,   583,   584,   585,   586,   587,   588,     0,     0,     0,
     415,   414,     0,     0,     0,     0,     0,   169,     0,     0,
     788,   789,   332,     0,     0,   337,     0,   568,   473,     0,
     469,     0,   470,   472,   468,   464,   461,     0,   807,   749,
     747,   748,   746,     0,   808,   482,   481,   478,   839,   836,
     837,   838,   835,   800,   457,   456,   832,   833,   805,   451,
     214,     0,   801,   448,   449,   797,   446,   445,   447,   450,
       0,     0,   650,   649,   648,   552,   548,   547,   834,   549,
     550,     0,     0,   528,   526,     0,   525,   524,   532,   825,
     824,   826,   543,   544,     0,     0,     0,   523,   527,   687,
     688,   686,   830,   529,   530,     0,   533,     0,   534,   822,
     823,     0,   811,   503,   814,     0,     0,   498,     0,   809,
     513,   508,   511,   813,   812,     0,     0,     0,   817,     0,
     810,   514,   509,   512,   501,   502,     0,   818,     0,   519,
     815,   816,     0,   488,     0,     0,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   339,   197,     0,     0,     0,     0,     0,     0,   767,
       0,     0,     0,     0,     0,     0,     0,     0,   193,   194,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,   134,   135,   136,   137,   334,     0,     0,
     465,     0,   367,     0,   483,   366,   806,   553,     0,   496,
     539,     0,   828,   827,   536,   535,   538,   537,   542,   515,
     505,     0,   516,   506,   499,     0,   500,   591,   589,   590,
     518,   819,     0,   714,   713,     0,     0,     0,    37,    36,
     109,    38,   107,   108,   106,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   391,     0,     0,    26,     0,   394,   252,     0,     0,
       0,     0,   383,     0,     0,   385,     0,   395,     0,   182,
       0,     0,   416,     0,    98,   335,   336,   471,   484,   551,
     545,   520,   521,   386,   387,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   195,     0,     0,   730,   728,   729,     0,     0,
       0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,   725,   723,   721,   727,   722,   724,     0,    53,
       0,    39,   110,    40,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   220,   232,     0,     0,   244,
       0,   100,   384,     0,   396,     0,   183,   285,   283,     0,
       0,     0,     0,     0,     0,     0,     0,   744,     0,     0,
       0,     0,     0,     0,   189,   209,     0,   715,   716,   717,
       0,     0,    41,   112,     0,     0,     0,     0,     0,     0,
     390,   389,     0,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   720,   718,   719,     0,     0,
      42,   113,     0,   380,     0,     0,     0,     0,     0,   388,
       0,     0,     0,     0,     0,   282,     0,    54,    43,   114,
       0,     0,     0,     0,     0,     0,     0,     0,   755,   381,
       0,     0,   382
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -900,  -900,  -900,   351,     0,  1524,  -170,  1755,   -37,  -900,
    -900,  1140,  2000,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,   290,  -900,  -900,  -900,  -900,  -900,  2371,
     221,  -900,  -164,  -900,  -900,   829,  -900,   -76,  -900,  -900,
    -900,  -900,   360,   769,  -900,  -301,  -900,  -900,  -899,  -900,
    -900,   -20,  -586,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,   -52,    48,   -14,    23,   195,  -192,  -900,  -900,
    -900,  -900,  -900,  -900,  -292,  -174,  -149
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,   151,   152,   153,   415,   638,   416,   639,   679,   609,
     779,   251,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   599,  1078,  1053,  1028,  1017,   306,   291,
     253,   169,   170,  1217,  1450,   269,  1143,   223,   944,   759,
     760,   791,   837,   434,   171,   448,  1285,  1080,   344,  1312,
     418,   660,   172,  1198,   799,   173,  1456,  1610,  1638,  1558,
    1538,   174,   277,   435,   279,   436,   437,   983,  1252,   175,
    1033,   176,  1084,  1059,   655,  1034,  1263
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     154,   697,  1016,   569,  1547,   428,   282,   283,   608,   191,
     380,   613,   355,   617,   618,   510,   621,  1409,  1230,   815,
    1535,   202,   362,   690,  1233,   202,  1141,   250,  1447,  1319,
    1282,  1350,  1309,   350,   177,   484,  1258,  1259,  1196,   289,
     295,   691,   241,   397,   446,   593,  1145,   242,   203,  1226,
     256,   460,   923,   465,   302,  1607,   280,   476,   481,   443,
    1240,  1434,   378,   299,   388,   504,  1142,   797,  1436,   507,
     351,   202,  1153,   830,   394,   255,  1551,   202,   191,   241,
     179,   278,   627,   831,   242,   382,  1490,   392,   298,   191,
     398,  1197,   798,    49,    50,   349,   525,   931,   444,    49,
      50,   541,  1249,   457,   395,   178,   354,   627,   765,   474,
     381,   202,   391,   932,   180,   491,   259,   260,   543,   430,
     181,   238,   239,   502,   772,   774,   461,   776,   466,  1283,
     182,   700,   477,   482,   692,   264,   241,   693,   183,  1260,
    1297,   242,   509,   192,   191,  1536,   184,   514,   516,   519,
     521,  1552,  1553,   185,   241,   594,   633,  1423,   241,   242,
     186,   261,   933,   242,   924,   303,  1608,   781,   274,   447,
    1448,   430,   505,   238,   239,   968,   508,   446,   606,   610,
     191,   612,   615,   934,   709,   619,   202,   622,   623,   624,
     625,   591,   782,  1559,  1560,   262,   604,   187,   241,   266,
    1310,   241,   595,   242,  1311,   263,   242,   238,   239,   363,
     241,    49,    50,  1299,   241,   242,   590,  1284,   274,   242,
    1554,   168,   257,   653,   935,   936,   937,  1261,   281,   304,
      96,    97,   925,   368,   188,   300,   627,  1555,   191,   657,
     195,  1249,   352,   201,   202,   264,  1250,   661,   662,   663,
     305,  1231,   271,   345,   431,  1537,  1251,   383,   274,   393,
     654,  1609,  1320,  1449,  1351,   926,   265,  1158,   241,   650,
     189,   938,   421,   242,   698,   939,  1262,   202,   783,   695,
     784,   999,   365,   239,   634,   627,   658,   432,   596,   597,
     635,   368,   400,   198,  1435,   659,  1346,   598,   462,   785,
     467,  1437,   447,   702,   478,   483,   431,   190,   440,   266,
     541,   627,   636,  1014,   196,   542,   696,   346,  1635,  1556,
     445,   940,   449,   946,   947,   833,   834,   468,   469,   941,
     942,   705,   627,  1015,  1417,   511,  1557,  1286,   197,   432,
     993,   239,  1287,   497,   433,   503,   202,  1300,    96,    97,
    1669,  1670,   721,   729,   835,  1266,   710,   525,   191,   713,
     284,   627,   272,   592,   288,   786,   787,   741,   706,   356,
     202,   727,   887,   238,   239,   301,   732,   733,  1301,   267,
     446,   354,   943,   358,   463,  1250,   268,   788,   401,   722,
     755,   359,   525,   836,   419,  1251,   541,   360,   651,   747,
     756,   542,  1032,  -170,  1058,  1083,   753,   433,   361,  1496,
     241,   199,   762,   384,   720,   242,   652,   273,  1305,   399,
     657,  1249,  1060,  1085,   274,   402,   420,  1636,   423,   777,
     780,   429,   792,   438,  1090,   805,   757,   439,   441,   200,
     506,   713,   479,   789,   523,   790,   512,    49,    50,   513,
     801,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   808,   524,   540,   238,   239,   560,  1637,
    1253,   613,   601,  1113,   602,  1115,   603,   202,   629,   814,
     525,   630,   178,   641,   651,   642,   241,   816,   817,   818,
     819,   242,   537,   538,   539,   129,   130,   540,   825,   826,
     827,   828,  1267,   643,   644,   447,   347,   758,   645,   694,
    1012,   842,   843,   844,   646,   647,   648,   274,   649,   966,
     134,   135,   136,   137,   138,   139,   857,   858,   860,   862,
     864,   866,   868,   870,   872,   874,   876,   878,   880,   882,
     884,   568,  1029,   588,  1055,   889,   891,   893,   895,   897,
     899,   901,   903,   905,   907,   909,   911,   913,   915,   917,
     919,   723,   980,   988,   307,  1250,   275,   995,   627,   238,
     239,  1009,  1306,   238,   239,  1251,   680,   681,   687,   712,
     688,   715,   716,  1000,    96,    97,   717,  1013,   276,   724,
     752,   556,   557,   558,   967,  1035,   559,   744,   749,   981,
     989,   802,   809,   713,   996,   735,   241,   810,  1010,   677,
     241,   242,   886,   433,   308,   242,  1020,  1030,   540,  1056,
    1081,   559,   544,   751,   979,    49,    50,  1106,   888,   994,
     921,   922,  1086,  1008,   674,   675,   676,  1110,  1016,   677,
    1092,  1019,  1112,  1091,  1114,  1093,  1116,  1117,  1118,  1119,
    1148,  1097,  1098,  1101,   793,   525,  1109,  1149,  1151,   794,
     795,  1152,  1153,  1154,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1156,
    1172,  1136,  1137,  1157,  1165,  1173,  1175,  1138,  1177,  1180,
    1144,   569,  1182,  1186,  1268,  1183,  1192,   177,  1193,  1150,
    1239,   241,  1194,   945,  1160,  1222,   242,  1247,  1281,   526,
    1295,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,  1162,  1321,   677,   838,  1333,  1334,
    1643,  1644,  1352,  1354,  1164,  1355,  1166,  1167,  1168,  1371,
    1372,  1161,  1170,   492,   526,   856,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,  1374,
     202,   677,   309,  1380,   525,  1393,   678,  1184,  1185,  1406,
    1432,   982,  1441,  1433,  1445,  1458,   997,  1457,  1465,  1470,
    1011,  1480,  1481,  1486,  1492,  1495,  1513,   493,  1021,  1031,
    1503,  1057,  1082,  1504,  1514,  1581,   254,  1022,  1582,   191,
     718,  1515,  1200,  1525,  1526,   202,  1540,  1201,  1577,   297,
    1203,    49,    50,  1204,   311,  1578,   348,  1223,  1584,  1597,
    1218,  1589,  1087,  1088,  1590,   946,   947,  1603,  1611,  1219,
    1625,   379,   526,   390,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,  1620,   494,   677,
    1621,  1108,   368,   525,   699,  1624,   238,   239,  1633,   956,
    1639,  1647,  1649,   285,  1660,  1665,  1389,  1672,   840,   957,
     948,   949,   459,   950,   951,   853,  1288,     0,   475,   591,
       0,   952,   953,   954,   955,     0,   525,     0,  1227,     0,
    1023,   129,   130,   241,  1024,   958,     0,     0,   242,     0,
       0,  1135,   239,   202,   590,     0,     0,   274,     0,  1237,
       0,     0,     0,     0,     0,     0,   134,   135,   136,   137,
     138,   139,  1232,     0,  1234,     0,  1236,  1358,     0,     0,
       0,  1365,     0,     0,     0,     0,   570,   589,     0,   600,
       0,     0,   525,     0,     0,  1025,   605,     0,    96,    97,
     368,  1274,   959,     0,     0,     0,     0,     0,  1026,     0,
    1241,   626,     0,     0,     0,     0,   632,     0,   495,     0,
    1176,     0,     0,  1248,     0,     0,  1257,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
    1276,   559,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,
    1213,  1214,  1215,  1216,     0,  1278,     0,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,  1255,   677,     0,  1291,  1277,     0,   734,
    1202,     0,     0,     0,     0,     0,   241,   960,     0,  1296,
     525,   242,  1304,     0,     0,     0,     0,     0,     0,  1316,
    1318,   592,     0,     0,     0,     0,  1412,     0,     0,     0,
       0,     0,     0,     0,   961,   962,  1327,     0,     0,  1338,
    1344,  1345,     0,     0,  1347,     0,     0,     0,     0,     0,
       0,     0,     0,   963,     0,     0,     0,     0,     0,  1331,
     525,     0,  1342,     0,     0,     0,     0,     0,  1359,  1361,
    1363,     0,  1366,  1367,  1368,  1369,     0,     0,     0,     0,
       0,     0,     0,     0,  1330,   707,   526,  1341,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,   964,   677,     0,   719,     0,     0,   742,   726,
     155,     0,     0,     0,     0,     0,     0,     0,   525,     0,
       0,     0,     0,     0,   713,   713,     0,   713,     0,     0,
       0,     0,     0,   743,     0,     0,     0,     0,     0,     0,
    1256,     0,     0,     0,  1395,     0,     0,   612,     0,  1132,
       0,  1132,     0,     0,     0,     0,     0,     0,     0,     0,
    1400,  1401,     0,     0,   713,     0,     0,     0,     0,  1405,
       0,     0,     0,     0,  1474,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,     0,     0,  1411,     0,   807,  1413,  1415,
       0,     0,     0,     0,     0,     0,     0,     0,   526,  1420,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,     0,     0,
     813,  1332,     0,     0,  1343,     0,     0,     0,     0,     0,
       0,  1425,     0,   832,     0,     0,   839,     0,     0,   841,
    1426,  1427,     0,     0,     0,     0,     0,     0,   517,   517,
     517,     0,  1429,     0,   526,  1430,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,   820,     0,     0,     0,
       0,   517,   517,     0,     0,   517,  1440,     0,     0,     0,
       0,     0,   927,   928,   929,   930,  1443,     0,   600,     0,
     965,   969,     0,     0,   972,   978,     0,   990,     0,   525,
       0,   998,  1451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1018,     0,     0,  1054,  1079,     0,     0,     0,
     525,     0,  1472,  1473,     0,  1475,  1476,  1477,  1478,  1479,
       0,     0,     0,     0,     0,  1482,  1390,     0,     0,     0,
       0,  1487,  1488,     0,   544,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,  1500,  1501,   677,
       0,     0,     0,     0,     0,     0,     0,     0,  1505,  1506,
     525,  1507,     0,  1508,     0,     0,     0,     0,  1509,     0,
       0,  1510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1511,     0,  1410,     0,  1512,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,     0,   525,   540,
    1146,     0,     0,     0,     0,  1517,  1519,  1521,  1522,  1523,
    1524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,  1159,     0,     0,
       0,     0,     0,     0,  1544,     0,     0,     0,  1546,   517,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,  1178,   829,     0,     0,   544,  1570,  1571,  1572,  1573,
    1574,  1575,     0,  1576,     0,     0,     0,     0,     0,  1579,
     193,   525,     0,     0,     0,     0,     0,     0,  1587,  1588,
       0,     0,   258,     0,   270,     0,     0,     0,     0,   286,
     287,     0,     0,   290,     0,  1592,  1594,  1595,  1596,     0,
       0,     0,   353,     0,     0,     0,   357,     0,     0,     0,
       0,   517,     0,  1604,     0,  1605,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,  1617,  1618,
    1619,     0,   544,   422,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,  1627,  1629,  1630,     0,   442,     0,
       0,     0,     0,  1634,   570,   589,   525,     0,     0,   464,
       0,  1645,  1646,     0,   480,     0,     0,   485,     0,  1650,
       0,     0,  1651,     0,  1623,     0,     0,   525,  1656,     0,
       0,     0,     0,     0,  1661,     0,     0,     0,     0,     0,
       0,     0,  1664,     0,   854,     0,  1668,   517,   517,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   517,   517,
     517,   525,     0,     0,     0,   517,   517,   517,   517,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   517,   517,
     517,   526,   607,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   525,   628,   540,     0,
       0,     0,   526,  1099,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,   517,  1254,     0,   545,   640,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,     0,     0,   559,     0,     0,     0,     0,  1100,     0,
       0,  1272,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,     0,  1102,     0,     0,   683,     0,   686,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,   701,   677,   703,   417,
    1322,     0,  1103,     0,  1324,  1329,   525,   526,  1340,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,  1353,     0,   451,     0,  1104,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   486,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     711,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   683,   526,   559,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,   525,     0,  1382,  1105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   611,     0,     0,     0,     0,
       0,     0,     0,   766,     0,     0,     0,   525,   770,   771,
     773,   631,   775,   778,   545,     0,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
       0,   559,     0,     0,     0,     0,  1155,   803,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   656,     0,   677,   811,     0,     0,   526,
    1174,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,     0,     0,     0,
     525,  1190,  1424,     0,   202,     0,     0,     0,   684,   685,
       0,     0,     0,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1191,     0,     0,     0,     0,
     704,   525,     0,     0,     0,     0,     0,     0,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,   544,     0,
    1199,     0,     0,     0,  1439,     0,     0,   970,     0,     0,
       0,     0,   708,   991,  1442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1446,   973,     0,  1027,     0,
     238,   239,     0,   684,   725,     0,     0,   728,     0,   730,
     731,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     740,     0,     0,     0,   525,     0,     0,     0,   518,   520,
     522,  1107,   745,   746,   748,     0,     0,   241,     0,     0,
       0,   754,   242,     0,     0,   761,     0,     0,   764,     0,
       0,   274,     0,     0,     0,     0,     0,   974,   769,     0,
       0,   614,   616,     0,     0,   620,     0,     0,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   975,   800,   677,     0,     0,     0,   804,
    1271,   806,     0,     0,     0,     0,  1147,     0,   525,     0,
       0,     0,   976,     0,     0,     0,     0,     0,   812,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   517,   517,
     517,     0,     0,     0,     0,     0,     0,   821,   822,   823,
     824,     0,  1534,   977,  1163,   525,     0,     0,     0,     0,
       0,     0,     0,     0,   526,  1545,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   525,
       0,   677,     0,     0,     0,     0,  1280,     0,     0,     0,
       0,     0,     0,     0,   517,   517,  1187,   517,  1188,   526,
    1189,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,   517,     0,     0,
       0,  1292,     0,     0,     0,     0,     0,   525,     0,   971,
       0,     0,   987,     0,   517,   992,     0,     0,     0,  1001,
    1007,     0,     0,     0,     0,     0,  1601,  1602,     0,     0,
       0,  1052,  1077,     0,  1220,  1221,     0,     0,  1606,   714,
       0,     0,     0,     0,     0,     0,     0,     0,   517,   517,
       0,   167,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
     194,     0,  1111,   194,  1373,     0,     0,     0,   252,     0,
       0,     0,   194,     0,     0,     0,     0,   685,     0,     0,
       0,   296,     0,  1653,     0,     0,   310,  -171,  -171,  -171,
    -171,  -171,  -171,   535,   536,   537,   538,   539,     0,  1663,
     540,     0,   364,     0,  1667,   389,     0,     0,     0,  1671,
       0,   796,   194,  1140,  -171,  -171,  -171,  -171,  -171,  -171,
     554,   555,   556,   557,   558,     0,     0,   559,   194,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     194,     0,   194,     0,   458,   525,     0,   194,   194,     0,
       0,  1238,     0,     0,     0,     0,  1243,     0,     0,     0,
       0,     0,     0,   496,  1169,   194,   526,   525,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,     0,  1375,     0,
       0,     0,     0,  1270,   525,     0,     0,   859,   861,   863,
     865,   867,   869,   871,   873,   875,   877,   879,   881,   883,
     885,     0,     0,     0,     0,   890,   892,   894,   896,   898,
     900,   902,   904,   906,   908,   910,   912,   914,   916,   918,
     920,  1294,   525,     0,  1298,     0,     0,     0,     0,  1308,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,     0,  1376,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,  1089,     0,   517,   517,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   525,     0,   677,     0,     0,     0,     0,  1377,
       0,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,     0,     0,  1378,     0,     0,     0,     0,     0,   194,
       0,   525,     0,     0,     0,     0,     0,  1381,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
    1235,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,  1394,   525,     0,
       0,  1379,     0,     0,     0,   517,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1242,     0,  1244,  1245,
       0,   525,     0,  1246,     0,     0,     0,     0,     0,   194,
       0,   194,   194,     0,     0,     0,     0,     0,     0,  1264,
    1265,     0,     0,     0,     0,     0,     0,     0,  1269,     0,
       0,     0,     0,     0,   517,   194,     0,     0,  1273,  1275,
       0,  1279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,     0,   525,     0,     0,     0,     0,
       0,     0,   517,     0,     0,     0,     0,     0,     0,     0,
       0,  1293,     0,     0,     0,     0,     0,     0,     0,  1307,
       0,     0,  1313,  1314,   194,     0,     0,     0,     0,   194,
     194,     0,     0,     0,     0,     0,     0,     0,  1323,     0,
       0,  1328,     0,     0,  1339,     0,     0,   525,     0,     0,
    1349,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,  1438,  1357,     0,     0,   526,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,  1370,     0,   677,     0,     0,     0,
       0,  1385,     0,     0,     0,     0,   526,   194,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,   855,     0,     0,  1391,     0,
       0,     0,     0,  1383,  1384,   525,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
    1499,   677,  1396,  1502,     0,  1398,  1392,     0,  1399,     0,
       0,     0,   525,     0,     0,     0,     0,   526,  1402,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,   525,     0,     0,     0,  1407,
       0,     0,   194,   194,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1408,     0,     0,     0,
       0,   194,  1422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,  1541,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1418,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,   194,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,  1586,  1419,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,  1444,     0,     0,  1421,     0,     0,  1360,  1362,
    1364,     0,     0,     0,     0,     0,     0,     0,     0,  1452,
    1455,     0,     0,     0,     0,  1171,     0,     0,     0,     0,
     194,     0,     0,     0,     0,     0,  1471,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,  1489,  1428,
       0,     0,     0,     0,  1386,  1387,     0,  1388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1655,     0,     0,     0,     0,     0,  1397,     0,   526,
     194,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,  1404,     0,   677,     0,     0,     0,
       0,  1431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1516,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,  1414,  1416,
       0,     0,     0,     0,     0,  1532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1542,     0,  1543,     0,
       0,     0,     0,     0,     0,     0,     0,   526,  1550,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,     0,   -87,
    1569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,  1580,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,  1591,  1466,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,     0,  1467,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    -7,     1,  1622,     0,
       0,     0,     0,     0,     0,     0,  1626,     0,     0,     0,
       0,     0,     0,     0,     2,     3,     4,     5,     0,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
       0,    16,     0,    17,    18,     0,     0,    19,    20,     0,
       0,     0,    21,    22,    23,     0,    24,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
      27,    28,     0,     0,     0,     0,    29,     0,     0,    30,
       0,     0,     0,     0,     0,     0,    31,    32,     0,     0,
       0,     0,     0,     0,     0,    33,    34,     0,     0,     0,
       0,     0,     0,     0,    35,    36,    37,    38,     0,     0,
       0,     0,     0,    39,    40,  1518,  1520,    41,     0,    42,
       0,     0,    43,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,     0,     0,     0,    48,    49,    50,
       0,     0,     0,    51,     0,    52,     0,     0,    53,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,    57,    58,    59,     0,    60,     0,    61,     0,     0,
       0,     0,    62,     0,    63,    64,     0,     0,     0,    65,
      66,     0,     0,     0,     0,     0,   194,     0,     0,     0,
       0,    67,    68,    69,    70,     0,     0,     0,   525,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,     0,     0,     0,  1593,     0,     0,     0,     0,
       0,     0,     0,   526,    74,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,     0,    75,
     540,     0,     0,   194,    76,    77,    78,    79,    80,    81,
       0,     0,    82,    83,    84,    85,     0,    86,    87,     0,
       0,     0,     0,    88,  1628,    89,    90,    91,    92,    93,
      94,     0,     0,     0,    95,    96,    97,     0,     0,     0,
       0,    98,     0,    99,   100,     0,     0,   101,   102,     0,
       0,     0,  1652,     0,     0,   103,   104,   105,   106,     0,
     107,     0,     0,     0,   108,   109,   110,     0,     0,     0,
     111,     0,   112,   113,   114,     0,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,     0,   122,   123,     0,
     124,   125,   126,   127,   128,   129,   130,     0,   131,   132,
     133,     0,   526,  1225,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
     134,   135,   136,   137,   138,   139,   140,   141,    -7,     1,
       0,     0,     0,     0,   142,   143,   144,   145,   146,     0,
       0,     0,     0,     0,     0,     0,   847,   848,     4,   525,
     147,   148,     0,     0,     0,     0,   149,     0,     0,    -7,
     150,     0,     0,    16,     0,    17,    18,     0,     0,    19,
      20,     0,   525,     0,    21,    22,    23,     0,     0,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,    27,    28,     0,   544,     0,     0,    29,     0,
       0,    30,     0,     0,     0,     0,     0,     0,    31,    32,
       0,     0,     0,     0,     0,     0,     0,    33,    34,     0,
       0,     0,     0,     0,     0,     0,    35,    36,    37,    38,
       0,     0,     0,     0,     0,    39,    40,     0,     0,    41,
       0,    42,     0,     0,    43,     0,     0,    44,    45,     0,
       0,     0,    46,     0,    47,     0,     0,     0,     0,    48,
      49,    50,     0,     0,     0,    51,     0,    52,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,     0,    60,     0,    61,
     544,     0,     0,     0,    62,     0,    63,    64,     0,     0,
       0,    65,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,  -171,  -171,  -171,  -171,  -171,  -171,
     672,   673,   674,   675,   676,     0,     0,   677,     0,    71,
       0,    72,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,    76,    77,    78,    79,
       0,    81,     0,     0,    82,    83,   849,    85,     0,    86,
      87,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    96,    97,     0,
       0,     0,     0,    98,     0,    99,   100,     0,     0,   101,
     102,     0,     0,     0,     0,     0,     0,   103,     0,   105,
     106,     0,   107,     0,     0,     0,   108,   109,   110,     0,
       0,     0,   111,     0,   112,   113,   114,     0,     0,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,   122,
     123,     0,   124,     0,     0,     0,     0,   129,   130,     0,
     131,   132,   133,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,     0,     0,     0,    24,   142,   850,   851,   852,
       0,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,    -7,     0,  1468,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1469,   545,     0,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,     0,     0,   559,     0,     0,     0,     0,  1483,
       0,     0,     0,   525,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    70,     0,     0,     0,     0,     0,     0,
      24,     0,   545,     0,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,     0,     0,   559,
       0,     0,     0,     0,  1485,     0,     0,     0,   312,     0,
       0,     0,   313,   314,   315,   316,   317,   318,     0,     0,
       0,   319,   320,   321,   322,   323,   324,   243,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
       0,     0,   326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,     0,     0,     0,     0,
     327,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   328,     0,   104,     0,   525,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,   204,   205,
     206,     0,   424,     0,     0,     0,     0,   425,     0,   116,
     117,   118,   119,   120,   121,   191,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,    68,     0,    70,     0,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,   147,
     247,     0,   243,     0,     0,   248,     0,     0,     0,   249,
       0,     0,     0,  1139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   341,     0,    95,    96,
      97,     0,     0,     0,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
     104,     0,     0,     0,     0,  1036,     0,  1037,     0,     0,
       0,     0,     0,     0,     0,   544,    24,     0,     0,     0,
       0,     0,     0,   426,   116,   117,   118,   119,   120,   121,
       0,     0,     0,  1038,     0,   125,   126,   127,   128,   129,
     130,   202,     0,     0,   525,   526,  1228,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
       0,     0,   540,     0,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,     0,   342,   343,     0,     0,   244,
     245,   246,   146,   525,     0,     0,     0,     0,   368,     0,
    1039,  1040,     0,     0,   147,   247,     0,     0,    49,    50,
     248,     0,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,   525,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1041,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,   207,    70,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
       0,     0,     0,     0,   241,  1042,     0,     0,     0,   242,
       0,  1025,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1043,     0,     0,     0,     0,     0,
       0,     0,  1044,  1045,     0,     0,     0,     0,   243,   526,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,     0,     0,     0,
       0,  1491,     0,     0,    95,    96,    97,     0,  1046,     0,
    1047,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1048,  1049,  1050,     0,   104,     0,   525,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1051,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,  1061,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,    24,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
       0,  1062,   525,     0,     0,     0,     0,     0,     0,     0,
     147,   247,     0,     0,   202,     0,   248,     0,     0,     0,
     249,     0,     0,     0,     0,     0,     0,   545,  1063,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   525,     0,   559,     0,     0,     0,     0,  1493,
       0,     0,     0,     0,     0,     0,   526,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,    49,    50,   677,     0,     0,     0,     0,  1494,     0,
       0,     0,     0,     0,     0,     0,   544,     0,    54,    55,
    1064,     0,     0,    56,     0,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,   239,   677,     0,     0,  1065,     0,  1498,     0,     0,
       0,     0,     0,  1066,     0,  1067,     0,  1068,  1069,   526,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,   241,   544,  1070,
    1071,  1527,   242,     0,  1025,     0,  1072,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1073,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,     0,     0,   224,   225,   226,     5,     0,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
       0,     0,  1074,     0,     0,     0,     0,    24,  1075,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,   366,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,   202,  1076,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   525,     0,   677,     0,   368,
       0,     0,  1528,   134,   135,   136,   137,   138,   139,    49,
      50,     0,     0,     0,     0,     0,   525,   369,   244,   245,
     246,   146,     0,     0,     0,     0,    54,    55,     0,     0,
       0,    56,     0,   147,   247,   370,     0,     0,     0,   248,
       0,   371,     0,   249,   372,     0,     0,     0,   238,   239,
       0,     0,   373,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,   526,    70,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,   241,  1529,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   243,
       0,   677,     0,     0,     0,     0,  1530,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,   545,  1229,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,     0,     0,   559,   374,   104,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,   375,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
      24,   403,   125,   126,   127,   128,   129,   130,   376,     0,
     545,     0,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,     0,     0,   559,     0,     0,
       0,   134,   135,   377,   137,   138,   139,     0,     0,     0,
       0,   525,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,   404,     0,     0,     0,     0,     0,     0,
       0,   147,   247,     0,     0,     0,     0,   248,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,   405,     0,   406,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,   408,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,   409,   526,  1531,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,     0,     0,
    1533,     0,   243,     0,     0,     0,     0,     0,     0,     0,
     410,     0,     0,   411,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   412,    95,    96,
      97,     0,     0,   413,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   414,    24,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,   452,     0,   125,   126,   127,   128,   129,
     130,   202,     0,     0,     0,     0,   525,     0,     0,     0,
       0,     0,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,   246,   146,     0,     0,     0,     0,     0,   368,     0,
       0,     0,     0,     0,   147,   247,     0,     0,    49,    50,
     248,     0,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,     0,     0,     0,     0,   454,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,   526,    70,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,   241,   224,   225,   226,     5,   242,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,    24,   243,   677,
       0,     0,     0,     0,  1539,     0,     0,     0,     0,     0,
     525,   456,     0,  1002,  1003,     0,     0,     0,     0,     0,
       0,     0,   202,     0,    95,    96,    97,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,   104,     0,     0,  1548,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1004,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,    49,
      50,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,   238,   239,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
       0,     0,     0,    68,     0,    70,     0,     0,     0,     0,
     147,   247,     0,     0,     0,     0,   248,     0,     0,     0,
     249,     0,     0,     0,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,   526,   243,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,     0,     0,
    1549,   202,     0,     0,     0,    95,    96,    97,   525,     0,
    1005,  1006,     0,     0,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,   525,    49,    50,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,   135,   136,   137,   138,   139,   238,   239,     0,
       0,   240,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,    68,     0,    70,     0,     0,     0,     0,     0,
       0,   147,   247,     0,     0,     0,     0,   248,     0,     0,
       0,   249,     0,     0,   241,   224,   225,   226,     5,   242,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,    24,   243,   677,
       0,     0,     0,     0,  1565,     0,     0,     0,     0,     0,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   202,     0,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,   292,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   293,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,   544,     0,    49,
      50,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,   238,   239,
       0,     0,   294,     0,     0,   244,   245,   246,   146,     0,
       0,     0,     0,    68,     0,    70,     0,     0,     0,     0,
     147,   247,     0,     0,     0,     0,   248,     0,     0,     0,
     249,     0,     0,     0,     0,   241,   224,   225,   226,     5,
     242,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   385,     0,     0,     0,     0,     0,     0,    24,   243,
     526,   525,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,     0,  1566,   202,     0,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,   386,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,   526,
       0,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,     0,     0,   540,     0,     0,     0,
     845,   116,   117,   118,   119,   120,   121,     0,     0,   525,
      49,    50,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,   134,   135,   136,   137,   138,   139,     0,   525,   238,
     239,     0,     0,   387,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,    68,     0,    70,     0,     0,     0,
       0,   147,   247,     0,     0,     0,     0,   248,     0,     0,
       0,   249,     0,     0,     0,     0,   241,   224,   225,   226,
       5,   242,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,    24,
     243,   677,     0,     0,     0,     0,  1567,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   471,   525,     0,     0,
       0,     0,     0,     0,   202,     0,    95,    96,    97,     0,
       0,     0,     0,     0,     0,     0,     0,   472,     0,     0,
       0,     0,   525,     0,     0,     0,     0,     0,   104,   545,
       0,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,     0,     0,   559,     0,     0,     0,
     846,   368,   116,   117,   118,   119,   120,   121,     0,     0,
       0,    49,    50,   125,   126,   127,   128,   129,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   135,   136,   137,   138,   139,     0,     0,
       0,   239,     0,     0,     0,     0,     0,   244,   245,   246,
     146,     0,     0,     0,     0,    68,     0,    70,     0,     0,
       0,     0,   147,   247,     0,     0,     0,     0,   248,     0,
       0,     0,   249,     0,     0,     0,     0,   241,     0,     0,
       0,   525,   242,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1568,     0,     0,     0,     0,
       0,   243,     0,     0,     0,     0,     0,     0,     0,     0,
     224,   225,   226,     5,   473,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,    95,    96,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,     0,     0,     0,   104,
       0,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,   202,   677,     0,
       0,     0,   -90,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,   525,  1583,   134,   135,   136,   137,   138,   139,     0,
       0,     0,     0,     0,    49,    50,  1335,     0,   244,   245,
     246,   146,     0,     0,     0,     0,     0,     0,     0,     0,
     525,    54,    55,   147,   247,     0,    56,     0,     0,   248,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,   526,
      70,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,     0,     0,     0,
     241,  1585,     0,     0,   526,   242,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1598,     0,     0,     0,
       0,     0,     0,     0,   243,     0,     0,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,   525,     0,  1336,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,    17,    18,   525,     0,     0,  1337,
       0,     0,     0,     0,   561,   202,     0,     0,     0,   562,
       0,     0,     0,     0,     0,     0,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1599,   134,   135,   136,   137,
     138,   139,    49,    50,  1325,     0,     0,     0,     0,     0,
       0,   244,   245,   246,   146,   563,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,   147,   247,     0,     0,
       0,     0,   248,     0,     0,     0,   249,     0,   564,     0,
       0,   238,   239,     0,     0,     0,     0,   204,   205,   206,
       0,     0,     0,     0,     0,     0,    68,     0,    70,   565,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   241,   224,
     225,   226,     5,   242,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   241,     0,     0,     0,     0,
     242,    24,   243,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1600,   202,     0,    95,    96,
      97,     0,   526,  1326,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
     104,     0,     0,     0,  1614,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,     0,     0,
     566,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,    49,    50,   125,   126,   127,   128,   129,
     130,   567,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,     0,   525,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,   136,   137,   138,   139,
     132,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,   246,   146,     0,     0,     0,     0,    68,     0,    70,
       0,     0,     0,     0,   147,   247,     0,     0,   141,     0,
     248,     0,     0,     0,   249,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,   274,     0,     0,     0,  1615,   526,   568,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   243,     0,   677,     0,     0,     0,     0,
    1616,     0,   525,     0,     0,     0,     0,     0,     0,   984,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,   207,     0,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   985,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,   120,
     121,     0,   986,     0,     0,     0,   125,   126,   127,   128,
     129,   130,     0,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,  1289,     0,     0,     0,    24,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,     0,  1249,     0,     0,
       0,     0,     0,     0,     0,   147,   247,     0,     0,     0,
       0,   248,     0,     0,   526,   249,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1631,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
     225,   226,     5,     0,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,    49,    50,     0,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,     0,    54,    55,     0,     0,   526,    56,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,     0,     0,
    1632,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    70,     0,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,  1250,   677,     0,     0,     0,     0,  1642,     0,     0,
       0,  1251,     0,     0,   525,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,   243,     0,     0,
      54,    55,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,  1290,     0,     0,    68,     0,    70,
       0,     0,     0,     0,   526,   104,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1648,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,    95,
      96,    97,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     247,   104,     0,     0,     0,   248,     0,     0,     0,   249,
     224,   225,   226,     5,     0,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,   125,   126,   127,   128,
     129,   130,    24,     0,     0,     0,   487,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,     0,     0,     0,   489,   490,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   247,     0,     0,     0,
       0,   248,     0,     0,   526,   249,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   525,
       0,   677,     0,     0,    49,    50,  1654,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,   224,   225,   226,     5,     0,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      70,     0,     0,     0,     0,     0,   526,    24,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,   526,  1662,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   525,     0,   677,     0,     0,     0,     0,  1666,
       0,     0,     0,   545,   243,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,     0,     0,
     559,     0,     0,     0,  1094,     0,     0,     0,     0,     0,
      95,    96,    97,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,    54,    55,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   736,     0,     0,   737,     0,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,    68,     0,    70,     0,   498,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,   135,   136,   137,
     138,   139,   525,     0,     0,   500,   501,     0,     0,     0,
       0,   244,   245,   246,   146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   247,     0,   243,
       0,     0,   248,     0,     0,     0,   249,   224,   225,   226,
       5,     0,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,     0,     0,     0,    95,    96,    97,     0,     0,
     738,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   739,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
       0,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,  1179,     0,     0,   677,     0,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,    49,    50,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
       0,   147,   247,    56,     0,     0,     0,   248,     0,     0,
       0,   249,   224,   225,   226,     5,     0,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,     0,
       0,   540,     0,     0,     0,  1095,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,     0,     0,     0,     0,   515,     0,   544,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
       0,     0,   525,     0,     0,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,   544,    54,    55,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
      68,    69,    70,     0,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,  1181,     0,
       0,   677,     0,   134,   135,   136,   137,   138,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,     0,   147,   148,     0,    80,     0,     0,   149,
       0,     0,  1403,   150,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,     0,   224,   225,   226,     5,
       0,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,     0,     0,   191,     0,   125,
     126,   127,   128,   129,   130,     0,     0,     0,     0,     0,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   245,   246,   146,     0,     0,     0,
       0,     0,     0,     0,   525,     0,     0,     0,   147,   148,
      49,    50,     0,     0,   149,     0,     0,     0,   150,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,     0,     0,    68,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,   545,   767,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,     0,     0,   559,   525,
       0,     0,  1096,     0,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
     243,   677,     0,     0,   545,   845,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
       0,   559,     0,     0,     0,  1195,    95,    96,    97,   544,
       0,     0,     0,     0,     0,     0,   682,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,    54,    55,     0,   768,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,    68,  -171,    70,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,   134,   135,   136,   137,   138,   139,     0,   525,
       0,     0,     0,     0,     0,     0,     0,   244,   245,   246,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,     0,     0,     0,   243,   248,     0,
       0,     0,   249,     0,     0,   224,   225,   226,     5,     0,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,     0,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,     0,     0,
       0,     0,     0,     0,   545,   104,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
       0,   559,   202,     0,     0,  1356,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,   526,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,  1095,     0,   134,
     135,   136,   137,   138,   139,   544,     0,     0,     0,    49,
      50,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,     0,   147,
     247,    56,     0,     0,     0,   248,     0,     0,     0,   249,
     224,   225,   226,     5,     0,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    70,     0,     0,  1302,     0,
       0,   526,    24,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,     0,     0,   540,     0,
       0,     0,  1459,     0,     0,     0,   525,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,   243,
       0,   545,     0,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,     0,     0,   559,     0,
       0,     0,  1460,     0,     0,    95,    96,    97,   525,     0,
       0,     0,     0,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
       0,     0,   125,   126,   127,   128,   129,   130,    68,     0,
      70,  -171,     0,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,     0,     0,   540,     0,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   147,   247,     0,   243,     0,     0,   248,     0,     0,
       0,   249,   224,   225,   226,     5,     0,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
       0,  1303,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1453,     0,     0,     0,     0,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,     0,     0,  -171,     0,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,     0,     0,   559,     0,   134,   135,   136,   137,
     138,   139,     0,     0,     0,     0,    49,    50,     0,     0,
       0,   244,   245,   246,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,   147,   247,    56,     0,
       0,     0,   248,     0,     0,     0,   249,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    70,     0,     0,     0,     0,     0,   526,    24,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,     0,     0,   540,     0,     0,   191,  1461,
     545,     0,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,     0,     0,   559,     0,     0,
       0,  1462,   544,     0,     0,     0,   243,     0,  1454,     0,
     526,     0,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,    17,    18,   540,     0,     0,
     544,  1463,    95,    96,    97,   561,     0,     0,     0,     0,
     562,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,    54,    55,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,     0,     0,     0,     0,   125,
     126,   127,   128,   129,   130,    68,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,   563,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,     0,     0,     0,     0,     0,   564,
       0,     0,     0,   244,   245,   246,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   147,   247,
     565,   243,     0,     0,   248,     0,     0,     0,   249,   224,
     225,   226,     5,     0,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,    95,    96,    97,
       0,     0,     0,     0,   544,     0,     0,     0,     0,     0,
       0,    24,     0,     0,     0,     0,   241,     0,     0,   104,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   134,   135,   136,   137,   138,   139,     0,
       0,   566,     0,    49,    50,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,   567,   147,   247,    56,     0,     0,     0,   248,
       0,     0,     0,   249,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,   132,     0,     0,     0,     0,     0,    68,     0,    70,
       0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,   545,   191,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
       0,   559,     0,     0,     0,  1464,     0,     0,     0,     0,
     629,     0,   545,   243,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   525,     0,   559,
       0,     0,     0,  1484,     0,     0,     0,     0,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,   104,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,   120,
     121,     0,     0,   450,     0,     0,   125,   126,   127,   128,
     129,   130,    68,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,     0,     0,     0,     0,
     525,     0,     0,     0,     0,   147,   247,     0,   243,     0,
       0,   248,     0,     0,     0,   249,   545,     0,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,     0,     0,   559,    95,    96,    97,  1497,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
       0,     0,   368,     0,   525,     0,     0,     0,     0,     0,
     147,   247,    49,    50,     0,     0,   248,     0,     0,     0,
     637,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,    70,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   544,     0,   677,     0,     0,     0,
    1459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   526,   243,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,     0,  1461,     0,     0,     0,   525,     0,    95,    96,
      97,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,    54,    55,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   129,
     130,    68,   526,    70,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,     0,     0,   540,
       0,     0,     0,  1561,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,   246,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   247,     0,   243,     0,     0,
     248,     0,     0,     0,   249,   224,   225,   226,     5,     0,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,     0,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,     0,     0,
       0,     0,     0,     0,   545,   104,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,  1315,
       0,   559,     0,     0,     0,  1562,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,     0,     0,   540,     0,     0,     0,  1563,     0,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,    49,
      50,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,     0,   147,
     247,    56,     0,     0,     0,   248,     0,     0,     0,   249,
     224,   225,   226,     5,     0,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    70,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,   545,     0,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,     0,     0,   559,     0,     0,   525,  1564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,   526,   243,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,     0,  1561,
       0,     0,     0,   544,     0,    95,    96,    97,     0,     0,
       0,     0,     0,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
       0,  1317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
       0,     0,   125,   126,   127,   128,   129,   130,    68,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   147,   247,     0,   243,     0,     0,   248,     0,     0,
       0,   249,   224,   225,   226,     5,     0,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
      95,    96,    97,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,  1348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,   135,   136,   137,
     138,   139,     0,     0,     0,     0,    49,    50,     0,     0,
       0,   244,   245,   246,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,   147,   247,    56,     0,
       0,     0,   248,     0,     0,     0,   249,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    70,   544,     0,     0,     0,     0,   526,    24,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   525,     0,   677,     0,     0,   526,  1563,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   525,     0,   540,     0,     0,     0,  1612,
       0,     0,     0,     0,     0,   545,   243,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     525,     0,   559,     0,     0,     0,  1613,     0,     0,     0,
       0,     0,    95,    96,    97,     0,     0,     0,     0,     0,
     544,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,    54,    55,
     525,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,     0,     0,     0,     0,   125,
     126,   127,   128,   129,   130,    68,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   245,   246,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   147,   247,
       0,   243,     0,     0,   248,     0,     0,     0,   249,   526,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,    95,    96,    97,
    1612,   526,     0,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,     0,     0,   540,   104,
       0,     0,  1640,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   134,   135,   136,   137,   138,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     246,   763,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,   247,     0,     0,     0,     0,   248,
       0,     0,     0,   249,     0,   545,     0,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
       0,     0,   559,     0,     0,   526,  1641,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,     0,     0,   526,  1640,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,     0,     0,     0,  1657,     0,     0,     0,
       0,     0,   526,     0,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,     0,     0,   540,
       0,     0,   545,  1658,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,     0,     0,   559,
       0,     0,   526,  1659,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,   571,   677,
       0,     0,     0,  1658,   572,     0,   571,     0,     0,     0,
       0,     0,   572,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   202,   573,     0,     0,     0,     0,
       0,     0,   202,   573,   312,     0,     0,     0,   313,   314,
     315,   316,   317,   318,     0,     0,     0,   319,   320,   321,
     322,   323,   324,     0,   574,   575,     0,     0,     0,   576,
       0,     0,   574,   575,   577,   325,     0,   576,   326,     0,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   327,     0,     0,     0,
     578,     0,     0,     0,     0,     0,     0,     0,   578,   328,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   579,   580,     0,   581,     0,
     238,   239,   582,   579,   580,     0,   581,     0,   238,   239,
     582,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   241,     0,     0,
       0,     0,   242,     0,     0,   241,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,   583,     0,
       0,     0,     0,     0,     0,     0,   583,     0,     0,     0,
       0,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,   584,     0,     0,     0,     0,     0,     0,
       0,   584,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   585,     0,     0,     0,     0,     0,     0,     0,
     585,     0,     0,   586,     0,     0,     0,     0,     0,     0,
       0,   586,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   587,     0,     0,     0,     0,     0,     0,     0,
     587,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   342,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,     0,     0,     0,     0,     0,  1224
};

static const yytype_int16 yycheck[] =
{
       0,   293,   588,   167,    35,    81,    19,    20,   178,    69,
      62,   181,    49,   183,   184,    79,   186,   124,    46,   304,
      45,    75,    61,   104,   923,    75,   182,    27,    62,    46,
      66,    46,    34,    47,   323,   111,    62,    63,    44,    39,
      40,   122,   208,    75,   102,    52,    40,   213,    25,   375,
      27,   103,   124,   105,   125,    17,    33,   109,   110,    96,
     959,    46,    62,    40,    64,   158,   222,   329,    46,   158,
      47,    75,   398,   114,   218,    27,    60,    75,    69,   208,
     399,    33,   392,   124,   213,    62,   396,    64,    40,    69,
     122,    97,   354,   132,   133,    47,    22,    52,    98,   132,
     133,   392,    62,   103,   248,   394,   397,   392,    99,   109,
      62,    75,    64,    68,   167,   115,    19,    20,   155,   169,
     394,   171,   172,   123,   425,   426,   103,   428,   105,   165,
     394,   301,   109,   110,   215,   148,   208,   218,   394,   165,
    1039,   213,   142,   123,    69,   170,   394,   147,   148,   149,
     150,   135,   136,   394,   208,   162,   204,   167,   208,   213,
     394,    64,   117,   213,   236,   236,   128,    33,   222,   227,
     204,   169,   265,   171,   172,   304,   265,   102,   178,   179,
      69,   181,   182,   138,   354,   185,    75,   187,   188,   189,
     190,   168,    58,   395,   396,    98,   362,   394,   208,   212,
     202,   208,   209,   213,   206,   108,   213,   171,   172,   248,
     208,   132,   133,   145,   208,   213,   168,   253,   222,   213,
     204,     0,    27,   237,   179,   180,   181,   253,    33,   300,
     269,   270,   304,   122,   394,    40,   392,   221,    69,   239,
      19,    62,    47,    22,    75,   148,   206,   247,   248,   249,
     321,   279,    31,    51,   304,   280,   216,    62,   222,    64,
     237,   223,   279,   297,   279,   337,   169,   321,   208,    75,
     394,   226,    77,   213,   294,   230,   302,    75,   144,   293,
     146,   582,    61,   172,   332,   392,   313,   337,   295,   296,
     338,   122,    71,   214,   279,   322,   256,   304,   103,   165,
     105,   279,   227,   303,   109,   110,   304,   394,    87,   212,
     392,   392,   360,   147,   394,   397,   293,   115,    74,   303,
      99,   276,   101,   269,   270,   177,   178,   106,   107,   284,
     285,   345,   392,   167,  1233,   399,   320,   219,   246,   337,
     304,   172,   224,   122,   394,   124,    75,   279,   269,   270,
     395,   396,   366,   373,   206,    75,   356,    22,    69,   359,
     373,   392,    67,   168,   394,   231,   232,   387,   345,    99,
      75,   371,   542,   171,   172,   394,   376,   377,   310,   282,
     102,   397,   337,   394,   273,   206,   289,   253,   129,   366,
     117,   394,    22,   245,   301,   216,   392,   394,   204,   399,
     127,   397,   594,   399,   596,   597,   406,   394,   394,   396,
     208,   332,   412,   394,   366,   213,   222,   122,    59,   394,
     420,    62,   596,   597,   222,   394,   172,   183,   394,   429,
     430,    18,   432,   394,   604,   455,   163,   394,   394,   360,
     394,   441,   273,   309,     0,   311,   399,   132,   133,   399,
     450,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   463,   393,   391,   171,   172,   399,   225,
      72,   641,   394,   643,   394,   645,   394,    75,   392,   479,
      22,   301,   394,   394,   204,   394,   208,   487,   488,   489,
     490,   213,   386,   387,   388,   329,   330,   391,   498,   499,
     500,   501,   222,   394,   394,   227,   304,   234,   394,   288,
     586,   511,   512,   513,   394,   394,   394,   222,   394,   571,
     354,   355,   356,   357,   358,   359,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   392,   594,   392,   596,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   366,   576,   577,    71,   206,   271,   581,   392,   171,
     172,   585,   213,   171,   172,   216,   301,   273,   273,   358,
     397,   360,   361,   583,   269,   270,   264,   587,   293,   273,
     115,   386,   387,   388,   571,   595,   391,   301,   301,   576,
     577,   273,   377,   603,   581,   384,   208,   273,   585,   391,
     208,   213,   167,   394,   121,   213,   593,   594,   391,   596,
     597,   391,    22,   402,   576,   132,   133,   627,   399,   581,
     304,   140,   367,   585,   386,   387,   388,   637,  1224,   391,
     395,   593,   642,   396,   644,   395,   646,   647,   648,   649,
     273,   396,   396,   396,   433,    22,   396,   396,   395,   438,
     439,   220,   398,   396,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,   395,
     301,   681,   682,   396,   396,   301,   395,   687,   396,   396,
     690,   855,   372,   395,   986,   396,   395,   323,   396,   699,
     273,   208,   396,   147,   718,   396,   213,   396,   273,   374,
     273,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   724,   396,   391,   506,   372,   372,
     395,   396,   396,   396,   734,   396,   736,   737,   738,   396,
     396,   718,   742,    64,   374,   524,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   396,
      75,   391,   269,   398,    22,   394,   396,   767,   768,   129,
     297,   576,   396,   297,   396,   395,   581,   396,   396,   396,
     585,   396,   395,   395,   395,   395,   395,   108,   593,   594,
     396,   596,   597,   398,   395,   395,    27,    67,   395,    69,
     115,   396,   802,   396,   396,    75,   396,   807,   396,    40,
     810,   132,   133,   813,    45,   396,    47,   854,   395,   395,
     820,   396,   601,   602,   396,   269,   270,   396,   396,   829,
     396,    62,   374,    64,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   395,   169,   391,
     395,   630,   122,    22,   396,   395,   171,   172,   395,   569,
     396,   396,   395,    34,   396,   396,  1158,   395,   508,    41,
     314,   315,   103,   317,   318,   524,  1025,    -1,   109,   856,
      -1,   325,   326,   327,   328,    -1,    22,    -1,   888,    -1,
     160,   329,   330,   208,   164,    67,    -1,    -1,   213,    -1,
      -1,   680,   172,    75,   856,    -1,    -1,   222,    -1,   946,
      -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,
     358,   359,   922,    -1,   924,    -1,   926,  1097,    -1,    -1,
      -1,  1101,    -1,    -1,    -1,    -1,   167,   168,    -1,   170,
      -1,    -1,    22,    -1,    -1,   215,   177,    -1,   269,   270,
     122,  1003,   124,    -1,    -1,    -1,    -1,    -1,   228,    -1,
     960,   192,    -1,    -1,    -1,    -1,   197,    -1,   289,    -1,
     749,    -1,    -1,   973,    -1,    -1,   976,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
    1004,   391,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,  1005,    -1,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,   975,   391,    -1,  1026,  1004,    -1,   396,
     809,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,  1039,
      22,   213,  1042,    -1,    -1,    -1,    -1,    -1,    -1,  1049,
    1050,   856,    -1,    -1,    -1,    -1,  1226,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   236,   237,  1066,    -1,    -1,  1069,
    1070,  1071,    -1,    -1,  1074,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   255,    -1,    -1,    -1,    -1,    -1,  1066,
      22,    -1,  1069,    -1,    -1,    -1,    -1,    -1,  1098,  1099,
    1100,    -1,  1102,  1103,  1104,  1105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1066,   346,   374,  1069,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,   304,   391,    -1,   366,    -1,    -1,   396,   370,
       0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,  1154,  1155,    -1,  1157,    -1,    -1,
      -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,    -1,    -1,
     975,    -1,    -1,    -1,  1174,    -1,    -1,  1177,    -1,  1179,
      -1,  1181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1190,  1191,    -1,    -1,  1194,    -1,    -1,    -1,    -1,  1199,
      -1,    -1,    -1,    -1,  1374,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,  1225,    -1,   396,  1228,  1229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,  1239,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,
     396,  1066,    -1,    -1,  1069,    -1,    -1,    -1,    -1,    -1,
      -1,  1271,    -1,   504,    -1,    -1,   507,    -1,    -1,   510,
    1280,  1281,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,    -1,  1292,    -1,   374,  1295,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   181,   182,    -1,    -1,   185,  1326,    -1,    -1,    -1,
      -1,    -1,   563,   564,   565,   566,  1336,    -1,   569,    -1,
     571,   572,    -1,    -1,   575,   576,    -1,   578,    -1,    22,
      -1,   582,  1352,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   593,    -1,    -1,   596,   597,    -1,    -1,    -1,
      22,    -1,  1372,  1373,    -1,  1375,  1376,  1377,  1378,  1379,
      -1,    -1,    -1,    -1,    -1,  1385,  1165,    -1,    -1,    -1,
      -1,  1391,  1392,    -1,    22,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,  1407,  1408,   391,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1418,  1419,
      22,  1421,    -1,  1423,    -1,    -1,    -1,    -1,  1428,    -1,
      -1,  1431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1441,    -1,  1222,    -1,  1445,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    22,   391,
     691,    -1,    -1,    -1,    -1,  1465,  1466,  1467,  1468,  1469,
    1470,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   718,    -1,    -1,
      -1,    -1,    -1,    -1,  1494,    -1,    -1,    -1,  1498,   359,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   752,   396,    -1,    -1,    22,  1526,  1527,  1528,  1529,
    1530,  1531,    -1,  1533,    -1,    -1,    -1,    -1,    -1,  1539,
      16,    22,    -1,    -1,    -1,    -1,    -1,    -1,  1548,  1549,
      -1,    -1,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,
      36,    -1,    -1,    39,    -1,  1565,  1566,  1567,  1568,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,   441,    -1,  1583,    -1,  1585,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1598,  1599,
    1600,    -1,    22,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1614,  1615,  1616,    -1,    94,    -1,
      -1,    -1,    -1,  1623,   855,   856,    22,    -1,    -1,   105,
      -1,  1631,  1632,    -1,   110,    -1,    -1,   113,    -1,  1639,
      -1,    -1,  1642,    -1,   296,    -1,    -1,    22,  1648,    -1,
      -1,    -1,    -1,    -1,  1654,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1662,    -1,   524,    -1,  1666,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,    22,    -1,    -1,    -1,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   374,   178,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    22,   193,   391,    -1,
      -1,    -1,   374,   396,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   603,   975,    -1,   374,   223,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,  1002,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,    -1,   396,    -1,    -1,   273,    -1,   275,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,   302,   391,   304,    74,
    1061,    -1,   396,    -1,  1065,  1066,    22,   374,  1069,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,  1086,    -1,   102,    -1,   396,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     356,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   368,   374,   391,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    22,    -1,  1145,   396,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   401,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   419,    -1,    -1,    -1,    22,   424,   425,
     426,   196,   428,   429,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,   453,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   238,    -1,   391,   472,    -1,    -1,   374,
     396,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
      22,   396,  1253,    -1,    75,    -1,    -1,    -1,   273,   274,
      -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
     305,    22,    -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    22,    -1,
     396,    -1,    -1,    -1,  1325,    -1,    -1,   573,    -1,    -1,
      -1,    -1,   347,   579,  1335,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1346,   167,    -1,   594,    -1,
     171,   172,    -1,   368,   369,    -1,    -1,   372,    -1,   374,
     375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     385,    -1,    -1,    -1,    22,    -1,    -1,    -1,   148,   149,
     150,   627,   397,   398,   399,    -1,    -1,   208,    -1,    -1,
      -1,   406,   213,    -1,    -1,   410,    -1,    -1,   413,    -1,
      -1,   222,    -1,    -1,    -1,    -1,    -1,   228,   423,    -1,
      -1,   181,   182,    -1,    -1,   185,    -1,    -1,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   254,   449,   391,    -1,    -1,    -1,   454,
     396,   456,    -1,    -1,    -1,    -1,   692,    -1,    22,    -1,
      -1,    -1,   273,    -1,    -1,    -1,    -1,    -1,   473,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1098,  1099,
    1100,    -1,    -1,    -1,    -1,    -1,    -1,   492,   493,   494,
     495,    -1,  1483,   304,   730,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,  1496,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    22,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1154,  1155,   772,  1157,   774,   374,
     776,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,  1177,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    -1,    -1,    22,    -1,   574,
      -1,    -1,   577,    -1,  1194,   580,    -1,    -1,    -1,   584,
     585,    -1,    -1,    -1,    -1,    -1,  1577,  1578,    -1,    -1,
      -1,   596,   597,    -1,   830,   831,    -1,    -1,  1589,   359,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1228,  1229,
      -1,     0,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      19,    -1,   637,    22,   396,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   652,    -1,    -1,
      -1,    40,    -1,  1644,    -1,    -1,    45,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,  1660,
     391,    -1,    61,    -1,  1665,    64,    -1,    -1,    -1,  1670,
      -1,   441,    71,   688,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,    -1,   103,    22,    -1,   106,   107,    -1,
      -1,   957,    -1,    -1,    -1,    -1,   962,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   739,   124,   374,    22,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,    -1,    -1,   999,    22,    -1,    -1,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,    -1,    -1,    -1,    -1,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,  1037,    22,    -1,  1040,    -1,    -1,    -1,    -1,  1045,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,    -1,   396,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   603,    -1,  1465,  1466,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    22,    -1,   391,    -1,    -1,    -1,    -1,   396,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    22,    -1,    -1,    -1,    -1,    -1,  1143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
     925,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,  1173,    22,    -1,
      -1,   396,    -1,    -1,    -1,  1565,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   961,    -1,   963,   964,
      -1,    22,    -1,   968,    -1,    -1,    -1,    -1,    -1,   358,
      -1,   360,   361,    -1,    -1,    -1,    -1,    -1,    -1,   984,
     985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   993,    -1,
      -1,    -1,    -1,    -1,  1614,   384,    -1,    -1,  1003,  1004,
      -1,  1006,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   402,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,  1642,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1036,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1044,
      -1,    -1,  1047,  1048,   433,    -1,    -1,    -1,    -1,   438,
     439,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1063,    -1,
      -1,  1066,    -1,    -1,  1069,    -1,    -1,    22,    -1,    -1,
    1075,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,  1321,  1091,    -1,    -1,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,  1109,    -1,   391,    -1,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    -1,   374,   506,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,   524,    -1,    -1,   396,    -1,
      -1,    -1,    -1,  1148,  1149,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
    1406,   391,  1177,  1409,    -1,  1180,   396,    -1,  1183,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,   374,  1193,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    22,    -1,    -1,    -1,   396,
      -1,    -1,   601,   602,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   630,  1247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   374,  1490,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     374,   680,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,  1547,   396,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,  1337,    -1,    -1,   396,    -1,    -1,  1098,  1099,
    1100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1354,
    1355,    -1,    -1,    -1,    -1,   744,    -1,    -1,    -1,    -1,
     749,    -1,    -1,    -1,    -1,    -1,  1371,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,  1393,   396,
      -1,    -1,    -1,    -1,  1154,  1155,    -1,  1157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1647,    -1,    -1,    -1,    -1,    -1,  1177,    -1,   374,
     809,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,  1194,    -1,   391,    -1,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1457,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,  1228,  1229,
      -1,    -1,    -1,    -1,    -1,  1480,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1491,    -1,  1493,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,  1503,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
    1525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,  1540,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,  1560,   396,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,     1,  1603,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1611,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    41,    42,    -1,
      -1,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,   107,   108,  1465,  1466,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,   119,   120,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,   139,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,   155,   156,   157,    -1,   159,    -1,   161,    -1,    -1,
      -1,    -1,   166,    -1,   168,   169,    -1,    -1,    -1,   173,
     174,    -1,    -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,   203,
      -1,   205,    -1,    -1,    -1,  1565,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   374,   218,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,   233,
     391,    -1,    -1,  1222,   238,   239,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,    -1,   251,   252,    -1,
      -1,    -1,    -1,   257,  1614,   259,   260,   261,   262,   263,
     264,    -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,   275,    -1,   277,   278,    -1,    -1,   281,   282,    -1,
      -1,    -1,  1642,    -1,    -1,   289,   290,   291,   292,    -1,
     294,    -1,    -1,    -1,   298,   299,   300,    -1,    -1,    -1,
     304,    -1,   306,   307,   308,    -1,    -1,    -1,    -1,   313,
     314,   315,   316,   317,   318,   319,    -1,   321,   322,    -1,
     324,   325,   326,   327,   328,   329,   330,    -1,   332,   333,
     334,    -1,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
     354,   355,   356,   357,   358,   359,   360,   361,     0,     1,
      -1,    -1,    -1,    -1,   368,   369,   370,   371,   372,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    22,
     384,   385,    -1,    -1,    -1,    -1,   390,    -1,    -1,   393,
     394,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    41,
      42,    -1,    22,    -1,    46,    47,    48,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    64,    65,    -1,    22,    -1,    -1,    70,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,   157,    -1,   159,    -1,   161,
      22,    -1,    -1,    -1,   166,    -1,   168,   169,    -1,    -1,
      -1,   173,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,   201,
      -1,   203,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   233,    -1,    -1,    -1,    -1,   238,   239,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,    -1,   251,
     252,    -1,    -1,    -1,    -1,    -1,    -1,   259,   260,   261,
     262,   263,   264,    -1,    -1,    -1,    -1,   269,   270,    -1,
      -1,    -1,    -1,   275,    -1,   277,   278,    -1,    -1,   281,
     282,    -1,    -1,    -1,    -1,    -1,    -1,   289,    -1,   291,
     292,    -1,   294,    -1,    -1,    -1,   298,   299,   300,    -1,
      -1,    -1,   304,    -1,   306,   307,   308,    -1,    -1,    -1,
      -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   321,
     322,    -1,   324,    -1,    -1,    -1,    -1,   329,   330,    -1,
     332,   333,   334,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,   354,   355,   356,   357,   358,   359,   360,   361,
      -1,    -1,    -1,    -1,    -1,    50,   368,   369,   370,   371,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,   393,    -1,   396,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    22,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    91,    92,    93,    94,    95,    96,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   290,    -1,    22,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    -1,    61,    -1,   314,
     315,   316,   317,   318,   319,    69,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   186,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,
     355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   384,
     385,    -1,   242,    -1,    -1,   390,    -1,    -1,    -1,   394,
      -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   266,    -1,   268,   269,
     270,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
     290,    -1,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    50,    -1,    -1,    -1,
      -1,    -1,    -1,   207,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,    67,    -1,   325,   326,   327,   328,   329,
     330,    75,    -1,    -1,    22,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,   354,   355,   356,   357,   358,   359,
      -1,    -1,    -1,    -1,    -1,   365,   366,    -1,    -1,   369,
     370,   371,   372,    22,    -1,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,   384,   385,    -1,    -1,   132,   133,
     390,    -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    22,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   337,   188,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,    -1,   213,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   236,   237,    -1,    -1,    -1,    -1,   242,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
      -1,   396,    -1,    -1,   268,   269,   270,    -1,   272,    -1,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   286,   287,   288,    -1,   290,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,   328,   329,   330,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    62,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     384,   385,    -1,    -1,    75,    -1,   390,    -1,    -1,    -1,
     394,    -1,    -1,    -1,    -1,    -1,    -1,   374,    89,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    22,    -1,   391,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   132,   133,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   149,   150,
     151,    -1,    -1,   154,    -1,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,   172,   391,    -1,    -1,   176,    -1,   396,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,    -1,   188,   189,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,   208,    22,   210,
     211,   396,   213,    -1,   215,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   242,    -1,    -1,    -1,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,   273,    -1,    -1,    -1,    -1,    50,   279,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,   304,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    22,    -1,   391,    -1,   122,
      -1,    -1,   396,   354,   355,   356,   357,   358,   359,   132,
     133,    -1,    -1,    -1,    -1,    -1,    22,   140,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,    -1,   384,   385,   158,    -1,    -1,    -1,   390,
      -1,   164,    -1,   394,   167,    -1,    -1,    -1,   171,   172,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   374,   188,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,   208,   396,    -1,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   242,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   268,   269,   270,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,   289,   290,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   312,
      -1,   314,   315,   316,   317,   318,   319,    -1,    -1,    -1,
      50,    51,   325,   326,   327,   328,   329,   330,   331,    -1,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   354,   355,   356,   357,   358,   359,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   384,   385,    -1,    -1,    -1,    -1,   390,    -1,    -1,
      -1,   394,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,   217,   374,   396,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,
     396,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     250,    -1,    -1,   253,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,   268,   269,
     270,    -1,    -1,   273,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
     290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   305,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,    67,    -1,   325,   326,   327,   328,   329,
     330,    75,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,   359,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,   384,   385,    -1,    -1,   132,   133,
     390,    -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   374,   188,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,   208,    18,    19,    20,    21,   213,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    50,   242,   391,
      -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,
      22,   255,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,   268,   269,   270,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,   290,    -1,    -1,   396,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,   132,
     133,   325,   326,   327,   328,   329,   330,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,   171,   172,
      -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,    -1,
     384,   385,    -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,
     394,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,   222,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,   374,   242,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,
     396,    75,    -1,    -1,    -1,   268,   269,   270,    22,    -1,
     273,   274,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,    22,   132,   133,
      -1,    -1,   325,   326,   327,   328,   329,   330,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   354,   355,   356,   357,   358,   359,   171,   172,    -1,
      -1,   175,    -1,    -1,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,   384,   385,    -1,    -1,    -1,    -1,   390,    -1,    -1,
      -1,   394,    -1,    -1,   208,    18,    19,    20,    21,   213,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    50,   242,   391,
      -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    22,    -1,   132,
     133,   325,   326,   327,   328,   329,   330,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,   171,   172,
      -1,    -1,   175,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,    -1,
     384,   385,    -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,
     394,    -1,    -1,    -1,    -1,   208,    18,    19,    20,    21,
     213,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,   242,
     374,    22,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,    -1,   396,    75,    -1,   268,   269,   270,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
     395,   314,   315,   316,   317,   318,   319,    -1,    -1,    22,
     132,   133,   325,   326,   327,   328,   329,   330,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   354,   355,   356,   357,   358,   359,    -1,    22,   171,
     172,    -1,    -1,   175,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,
      -1,   384,   385,    -1,    -1,    -1,    -1,   390,    -1,    -1,
      -1,   394,    -1,    -1,    -1,    -1,   208,    18,    19,    20,
      21,   213,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    50,
     242,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,   268,   269,   270,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,   290,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
     395,   122,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,   132,   133,   325,   326,   327,   328,   329,   330,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   354,   355,   356,   357,   358,   359,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,
     372,    -1,    -1,    -1,    -1,   186,    -1,   188,    -1,    -1,
      -1,    -1,   384,   385,    -1,    -1,    -1,    -1,   390,    -1,
      -1,    -1,   394,    -1,    -1,    -1,    -1,   208,    -1,    -1,
      -1,    22,   213,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,   255,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    75,   391,    -1,
      -1,    -1,   395,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,    22,   396,   354,   355,   356,   357,   358,   359,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,   149,   150,   384,   385,    -1,   154,    -1,    -1,   390,
      -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   374,
     188,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
     208,   396,    -1,    -1,   374,   213,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     268,   269,   270,    22,    -1,   273,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,    37,    38,    22,    -1,    -1,   297,
      -1,    -1,    -1,    -1,    47,    75,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,
     328,   329,   330,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,   354,   355,   356,   357,
     358,   359,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,   369,   370,   371,   372,   118,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,   384,   385,    -1,    -1,
      -1,    -1,   390,    -1,    -1,    -1,   394,    -1,   141,    -1,
      -1,   171,   172,    -1,    -1,    -1,    -1,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,    18,
      19,    20,    21,   213,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,   208,    -1,    -1,    -1,    -1,
     213,    50,   242,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    75,    -1,   268,   269,
     270,    -1,   374,   273,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
     290,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
     283,    -1,    -1,    -1,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,   132,   133,   325,   326,   327,   328,   329,
     330,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,    -1,    22,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,   359,
     333,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,    -1,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,   384,   385,    -1,    -1,   361,    -1,
     390,    -1,    -1,    -1,   394,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,   222,    -1,    -1,    -1,   396,   374,   392,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   242,    -1,   391,    -1,    -1,    -1,    -1,
     396,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   258,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   268,
     269,   270,   337,    -1,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,    -1,
      -1,   290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,
     319,    -1,   321,    -1,    -1,    -1,   325,   326,   327,   328,
     329,   330,    -1,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,
     359,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
     369,   370,   371,   372,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,
      -1,   390,    -1,    -1,   374,   394,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,   149,   150,    -1,    -1,   374,   154,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,
     396,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,   188,    -1,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,   206,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,
      -1,   216,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   242,    -1,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   279,    -1,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,   374,   290,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,
     355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,   268,
     269,   270,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,
     385,   290,    -1,    -1,    -1,   390,    -1,    -1,    -1,   394,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,   314,   315,   316,   317,   318,
     319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,
     329,   330,    50,    -1,    -1,    -1,   335,   336,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,
     359,    -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,
     369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,
      -1,   390,    -1,    -1,   374,   394,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    22,
      -1,   391,    -1,    -1,   132,   133,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
     188,    -1,    -1,    -1,    -1,    -1,   374,    50,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   374,   396,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    22,    -1,   391,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,   374,   242,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,
     268,   269,   270,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,   167,    -1,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,
     328,   329,   330,   186,    -1,   188,    -1,   335,   336,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,
     358,   359,    22,    -1,    -1,   363,   364,    -1,    -1,    -1,
      -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   384,   385,    -1,   242,
      -1,    -1,   390,    -1,    -1,    -1,   394,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,   268,   269,   270,    -1,    -1,
     273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,    -1,    -1,    -1,
      -1,    -1,   325,   326,   327,   328,   329,   330,    -1,    -1,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,   354,   355,   356,   357,   358,   359,    -1,    -1,    -1,
      -1,   132,   133,    -1,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,   384,   385,   154,    -1,    -1,    -1,   390,    -1,    -1,
      -1,   394,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   187,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,   248,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   257,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,    22,    -1,    -1,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,    -1,    22,   149,   150,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
     186,   187,   188,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,   354,   355,   356,   357,   358,   359,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,   384,   385,    -1,   242,    -1,    -1,   390,
      -1,    -1,   248,   394,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   257,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   268,   269,   270,    -1,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,   290,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,    69,    -1,   325,
     326,   327,   328,   329,   330,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,
     356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   384,   385,
     132,   133,    -1,    -1,   390,    -1,    -1,    -1,   394,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   374,    57,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    22,
      -1,    -1,   395,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
     242,   391,    -1,    -1,   374,   395,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,   395,   268,   269,   270,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,   152,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,    -1,    -1,   325,   326,   327,   328,   329,   330,    -1,
      -1,   186,   374,   188,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,   354,   355,   356,   357,   358,   359,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,
     372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   384,   385,    -1,    -1,    -1,   242,   390,    -1,
      -1,    -1,   394,    -1,    -1,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,   374,   290,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    75,    -1,    -1,   395,    -1,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,   354,
     355,   356,   357,   358,   359,    22,    -1,    -1,    -1,   132,
     133,    -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,   384,
     385,   154,    -1,    -1,    -1,   390,    -1,    -1,    -1,   394,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    46,    -1,
      -1,   374,    50,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,   395,    -1,    -1,    -1,    22,    -1,    -1,   222,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,   242,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,   395,    -1,    -1,   268,   269,   270,    22,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,    -1,    -1,    -1,
      -1,    -1,   325,   326,   327,   328,   329,   330,   186,    -1,
     188,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,   354,   355,   356,   357,   358,   359,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   384,   385,    -1,   242,    -1,    -1,   390,    -1,    -1,
      -1,   394,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
     268,   269,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   279,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,   354,   355,   356,   357,
     358,   359,    -1,    -1,    -1,    -1,   132,   133,    -1,    -1,
      -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,   384,   385,   154,    -1,
      -1,    -1,   390,    -1,    -1,    -1,   394,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,   374,    50,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    69,   395,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   395,    22,    -1,    -1,    -1,   242,    -1,   244,    -1,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    37,    38,   391,    -1,    -1,
      22,   395,   268,   269,   270,    47,    -1,    -1,    -1,    -1,
      52,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,   149,   150,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,   325,
     326,   327,   328,   329,   330,   186,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,
     356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   385,
     162,   242,    -1,    -1,   390,    -1,    -1,    -1,   394,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,   208,    -1,    -1,   290,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   354,   355,   356,   357,   358,   359,    -1,
      -1,   283,    -1,   132,   133,    -1,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   304,   384,   385,   154,    -1,    -1,    -1,   390,
      -1,    -1,    -1,   394,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,   333,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   361,
      -1,    -1,    -1,    -1,   374,    69,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
     392,    -1,   374,   242,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    22,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,   268,
     269,   270,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   290,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,
     319,    -1,    -1,   322,    -1,    -1,   325,   326,   327,   328,
     329,   330,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,
     359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,   384,   385,    -1,   242,    -1,
      -1,   390,    -1,    -1,    -1,   394,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,   268,   269,   270,   395,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,   290,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,   328,   329,   330,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    -1,   122,    -1,    22,    -1,    -1,    -1,    -1,    -1,
     384,   385,   132,   133,    -1,    -1,   390,    -1,    -1,    -1,
     394,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    22,    -1,   391,    -1,    -1,    -1,
     395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   374,   242,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,   395,    -1,    -1,    -1,    22,    -1,   268,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,
     330,   186,   374,   188,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   395,   354,   355,   356,   357,   358,   359,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   384,   385,    -1,   242,    -1,    -1,
     390,    -1,    -1,    -1,   394,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,   374,   290,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   304,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,   354,
     355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,   132,
     133,    -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,   384,
     385,   154,    -1,    -1,    -1,   390,    -1,    -1,    -1,   394,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    22,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   374,   242,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    22,    -1,   268,   269,   270,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,    -1,    -1,    -1,
      -1,    -1,   325,   326,   327,   328,   329,   330,   186,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   354,   355,   356,   357,   358,   359,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   384,   385,    -1,   242,    -1,    -1,   390,    -1,    -1,
      -1,   394,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
     268,   269,   270,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,
     358,   359,    -1,    -1,    -1,    -1,   132,   133,    -1,    -1,
      -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,   384,   385,   154,    -1,
      -1,    -1,   390,    -1,    -1,    -1,   394,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,   188,    22,    -1,    -1,    -1,    -1,   374,    50,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    22,    -1,   391,    -1,    -1,   374,   395,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    22,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,   374,   242,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      22,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,
      -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,    -1,
      22,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,   149,   150,
      22,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,   325,
     326,   327,   328,   329,   330,   186,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,
     356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   385,
      -1,   242,    -1,    -1,   390,    -1,    -1,    -1,   394,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,   268,   269,   270,
     395,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,   290,
      -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   354,   355,   356,   357,   358,   359,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   384,   385,    -1,    -1,    -1,    -1,   390,
      -1,    -1,    -1,   394,    -1,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,   374,   395,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,   374,   395,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,
      -1,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,   374,   395,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,   374,   395,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    49,   391,
      -1,    -1,    -1,   395,    55,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    78,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    -1,    -1,    -1,    91,    92,    93,
      94,    95,    96,    -1,   105,   106,    -1,    -1,    -1,   110,
      -1,    -1,   105,   106,   115,   109,    -1,   110,   112,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,   167,    -1,   169,    -1,
     171,   172,   173,   166,   167,    -1,   169,    -1,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   208,    -1,    -1,
      -1,    -1,   213,    -1,    -1,   208,    -1,    -1,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   229,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   266,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     293,    -1,    -1,   304,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   353,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     353,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   365,   366,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   392,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   392
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     1,    18,    19,    20,    21,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    35,    37,    38,    41,
      42,    46,    47,    48,    50,    51,    61,    64,    65,    70,
      73,    80,    81,    89,    90,    98,    99,   100,   101,   107,
     108,   111,   113,   116,   119,   120,   124,   126,   131,   132,
     133,   137,   139,   142,   149,   150,   154,   155,   156,   157,
     159,   161,   166,   168,   169,   173,   174,   185,   186,   187,
     188,   201,   203,   205,   218,   233,   238,   239,   240,   241,
     242,   243,   246,   247,   248,   249,   251,   252,   257,   259,
     260,   261,   262,   263,   264,   268,   269,   270,   275,   277,
     278,   281,   282,   289,   290,   291,   292,   294,   298,   299,
     300,   304,   306,   307,   308,   313,   314,   315,   316,   317,
     318,   319,   321,   322,   324,   325,   326,   327,   328,   329,
     330,   332,   333,   334,   354,   355,   356,   357,   358,   359,
     360,   361,   368,   369,   370,   371,   372,   384,   385,   390,
     394,   401,   402,   403,   404,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   429,   430,   431,
     432,   444,   452,   455,   461,   469,   471,   323,   394,   399,
     167,   394,   394,   394,   394,   394,   394,   394,   394,   394,
     394,    69,   123,   405,   429,   430,   394,   246,   214,   332,
     360,   430,    75,   465,    52,    53,    54,   337,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   437,    18,    19,    20,    23,    24,    26,
      27,    28,    29,    30,    31,    32,    88,   115,   171,   172,
     175,   208,   213,   242,   369,   370,   371,   385,   390,   394,
     404,   411,   429,   430,   443,   463,   465,   466,   405,    19,
      20,    64,    98,   108,   148,   169,   212,   282,   289,   435,
     405,   430,    67,   122,   222,   271,   293,   462,   463,   464,
     465,   466,    19,    20,   373,   435,   405,   405,   394,   404,
     405,   429,    88,   115,   175,   404,   429,   443,   463,   465,
     466,   394,   125,   236,   300,   321,   428,    71,   121,   269,
     429,   443,    78,    82,    83,    84,    85,    86,    87,    91,
      92,    93,    94,    95,    96,   109,   112,   130,   143,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     235,   266,   365,   366,   448,    51,   115,   304,   443,   463,
     464,   465,   466,   405,   397,   408,    99,   405,   394,   394,
     394,   394,    61,   248,   429,   430,    64,    67,   122,   140,
     158,   164,   167,   175,   289,   312,   331,   356,   404,   443,
     462,   463,   465,   466,   394,    43,    88,   175,   404,   429,
     443,   463,   465,   466,   218,   248,   405,    75,   122,   394,
     430,   129,   394,    51,   103,   151,   153,   164,   165,   217,
     250,   253,   267,   273,   305,   404,   406,   407,   450,   301,
     172,   466,   405,   394,    56,    61,   207,   405,   437,    18,
     169,   304,   337,   394,   443,   463,   465,   466,   394,   394,
     430,   394,   405,   408,   404,   430,   102,   227,   445,   430,
     322,   407,    67,    88,   160,   175,   255,   404,   429,   443,
     462,   465,   466,   273,   405,   462,   465,   466,   430,   430,
     407,    67,    88,   255,   404,   443,   462,   465,   466,   273,
     405,   462,   465,   466,   437,   405,   407,   335,   336,   363,
     364,   404,    64,   108,   169,   289,   429,   430,   335,   336,
     363,   364,   404,   430,   158,   265,   394,   158,   265,   404,
      79,   399,   399,   399,   404,   248,   404,   411,   412,   404,
     412,   404,   412,     0,   393,    22,   374,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     391,   392,   397,   408,    22,   374,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   391,
     399,    47,    52,   118,   141,   162,   283,   304,   392,   432,
     443,    49,    55,    76,   105,   106,   110,   115,   141,   166,
     167,   169,   173,   229,   274,   293,   304,   353,   392,   443,
     463,   465,   466,    52,   162,   209,   295,   296,   304,   423,
     443,   394,   394,   394,   362,   443,   404,   405,   406,   409,
     404,   407,   404,   406,   412,   404,   412,   406,   406,   404,
     412,   406,   404,   404,   404,   404,   443,   392,   405,   392,
     301,   407,   443,   204,   332,   338,   360,   394,   405,   407,
     405,   394,   394,   394,   394,   394,   394,   394,   394,   394,
      75,   204,   222,   464,   465,   474,   407,   404,   313,   322,
     451,   404,   404,   404,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   391,   396,   408,
     301,   273,   278,   405,   407,   407,   405,   273,   397,   405,
     104,   122,   215,   218,   430,   464,   465,   474,   451,   396,
     406,   405,   404,   405,   407,   464,   465,   443,   407,   406,
     404,   405,   430,   404,   412,   430,   430,   264,   115,   443,
     463,   464,   465,   466,   273,   407,   443,   404,   407,   451,
     407,   407,   404,   404,   396,   430,   164,   167,   273,   304,
     407,   451,   396,   443,   301,   407,   407,   404,   407,   301,
     405,   430,   115,   404,   407,   117,   127,   163,   234,   439,
     440,   407,   404,   372,   407,    99,   405,    57,   152,   407,
     405,   405,   445,   405,   445,   405,   445,   404,   405,   410,
     404,    33,    58,   144,   146,   165,   231,   232,   253,   309,
     311,   441,   404,   430,   430,   430,   412,   329,   354,   454,
     407,   404,   273,   405,   407,   451,   407,   396,   404,   377,
     273,   405,   407,   396,   404,   304,   404,   404,   404,   404,
     396,   407,   407,   407,   407,   404,   404,   404,   404,   396,
     114,   124,   443,   177,   178,   206,   245,   442,   430,   443,
     442,   443,   404,   404,   404,   395,   395,    18,    19,   248,
     369,   370,   371,   403,   411,   429,   430,   404,   404,   412,
     404,   412,   404,   412,   404,   412,   404,   412,   404,   412,
     404,   412,   404,   412,   404,   412,   404,   412,   404,   412,
     404,   412,   404,   412,   404,   412,   167,   406,   399,   404,
     412,   404,   412,   404,   412,   404,   412,   404,   412,   404,
     412,   404,   412,   404,   412,   404,   412,   404,   412,   404,
     412,   404,   412,   404,   412,   404,   412,   404,   412,   404,
     412,   304,   140,   124,   236,   304,   337,   443,   443,   443,
     443,    52,    68,   117,   138,   179,   180,   181,   226,   230,
     276,   284,   285,   337,   438,   147,   269,   270,   314,   315,
     317,   318,   325,   326,   327,   328,   423,    41,    67,   124,
     209,   236,   237,   255,   304,   443,   462,   465,   304,   443,
     405,   407,   443,   167,   228,   254,   273,   304,   443,   463,
     464,   465,   466,   467,   258,   304,   321,   407,   464,   465,
     443,   405,   407,   304,   463,   464,   465,   466,   443,   445,
     404,   407,    66,    67,   115,   273,   274,   407,   463,   464,
     465,   466,   437,   404,   147,   167,   452,   427,   443,   463,
     465,   466,    67,   160,   164,   215,   228,   405,   426,   462,
     465,   466,   467,   470,   475,   404,    39,    41,    67,   124,
     125,   164,   209,   228,   236,   237,   272,   274,   286,   287,
     288,   304,   407,   425,   443,   462,   465,   466,   467,   473,
     475,    36,    62,    89,   151,   176,   184,   186,   188,   189,
     210,   211,   217,   228,   273,   279,   304,   407,   424,   443,
     447,   465,   466,   467,   472,   475,   367,   430,   430,   412,
     406,   396,   395,   395,   395,   395,   395,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   404,   405,   430,   396,
     404,   407,   404,   406,   404,   406,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   430,   404,   404,   404,   398,
     407,   182,   222,   436,   404,    40,   443,   405,   273,   396,
     404,   395,   220,   398,   396,   396,   395,   396,   321,   443,
     464,   465,   404,   405,   404,   396,   404,   404,   404,   407,
     404,   429,   301,   301,   396,   395,   430,   396,   443,   388,
     396,   388,   372,   396,   404,   404,   395,   405,   405,   405,
     396,   396,   395,   396,   396,   395,    44,    97,   453,   396,
     404,   404,   430,   404,   404,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,   433,   404,   404,
     405,   405,   396,   408,   392,   375,   375,   404,   375,   375,
      46,   279,   404,   448,   404,   407,   404,   408,   405,   273,
     448,   404,   407,   405,   407,   407,   407,   396,   404,    62,
     206,   216,   468,    72,   443,   463,   466,   404,    62,    63,
     165,   253,   302,   476,   407,   407,    75,   222,   474,   407,
     405,   396,   443,   407,   462,   407,   464,   465,   404,   407,
     396,   273,    66,   165,   253,   446,   219,   224,   476,    46,
     279,   404,   396,   407,   405,   273,   404,   448,   405,   145,
     279,   310,    46,   279,   404,    59,   213,   407,   405,    34,
     202,   206,   449,   407,   407,   304,   404,   304,   404,    46,
     279,   396,   443,   407,   443,   134,   273,   404,   407,   443,
     463,   465,   466,   372,   372,   134,   273,   297,   404,   407,
     443,   463,   465,   466,   404,   404,   256,   404,   304,   407,
      46,   279,   396,   443,   396,   396,   395,   407,   406,   404,
     412,   404,   412,   404,   412,   406,   404,   404,   404,   404,
     407,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     398,   405,   443,   407,   407,   396,   412,   412,   412,   474,
     430,   396,   396,   394,   405,   404,   407,   412,   407,   407,
     404,   404,   407,   248,   412,   404,   129,   396,   396,   124,
     430,   404,   406,   404,   412,   404,   412,   448,   396,   396,
     404,   396,   407,   167,   443,   404,   404,   404,   396,   404,
     404,   396,   297,   297,    46,   279,    46,   279,   405,   443,
     404,   396,   443,   404,   407,   396,   443,    62,   204,   297,
     434,   404,   407,    77,   244,   407,   456,   396,   395,   395,
     395,   395,   395,   395,   395,   396,   396,   396,   396,   396,
     396,   407,   404,   404,   406,   404,   404,   404,   404,   404,
     396,   395,   404,   396,   395,   396,   395,   404,   404,   407,
     396,   396,   395,   396,   396,   395,   396,   395,   396,   405,
     404,   404,   405,   396,   398,   404,   404,   404,   404,   404,
     404,   404,   404,   395,   395,   396,   407,   404,   412,   404,
     412,   404,   404,   404,   404,   396,   396,   396,   396,   396,
     396,   396,   407,   396,   443,    45,   170,   280,   460,   396,
     396,   405,   407,   407,   404,   443,   404,    35,   396,   396,
     407,    60,   135,   136,   204,   221,   303,   320,   459,   395,
     396,   395,   395,   395,   395,   396,   396,   396,   396,   407,
     404,   404,   404,   404,   404,   404,   404,   396,   396,   404,
     407,   395,   395,   396,   395,   396,   405,   404,   404,   396,
     396,   407,   404,   412,   404,   404,   404,   395,   396,   396,
     396,   443,   443,   396,   404,   404,   443,    17,   128,   223,
     457,   396,   395,   395,   396,   396,   396,   404,   404,   404,
     395,   395,   407,   296,   395,   396,   407,   404,   412,   404,
     404,   396,   396,   395,   404,    74,   183,   225,   458,   396,
     395,   395,   396,   395,   396,   404,   404,   396,   396,   395,
     404,   404,   412,   443,   396,   405,   404,   395,   395,   395,
     396,   404,   396,   443,   404,   396,   396,   443,   404,   395,
     396,   443,   395
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   400,   401,   401,   401,   402,   402,   403,   403,   403,
     403,   403,   403,   403,   403,   403,   403,   403,   403,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   405,   405,   405,   406,   407,   408,
     409,   409,   410,   410,   411,   411,   411,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   413,   413,   413,   413,   413,
     414,   415,   415,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   417,   417,   417,   417,   417,   417,
     417,   417,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     420,   420,   420,   420,   420,   420,   421,   421,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   427,   427,   427,
     427,   428,   428,   428,   428,   429,   429,   430,   430,   430,
     430,   431,   431,   432,   432,   432,   432,   433,   433,   433,
     433,   433,   433,   433,   433,   433,   433,   433,   433,   434,
     434,   434,   435,   435,   436,   436,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   437,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   438,   438,   438,   438,   438,
     438,   439,   439,   440,   440,   441,   441,   441,   441,   441,
     441,   441,   441,   441,   441,   442,   442,   442,   442,   443,
     443,   444,   444,   444,   444,   444,   445,   445,   446,   446,
     446,   447,   447,   447,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   449,   449,   449,   450,
     450,   450,   450,   450,   450,   451,   451,   452,   452,   452,
     452,   452,   452,   452,   452,   453,   453,   454,   454,   455,
     455,   455,   455,   456,   456,   457,   457,   457,   458,   458,
     458,   459,   459,   459,   459,   459,   459,   459,   460,   460,
     460,   461,   461,   461,   461,   461,   461,   461,   462,   462,
     463,   464,   465,   465,   465,   466,   467,   468,   468,   468,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   470,   470,   471,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   474,   474,   474,   475,   476,   476,   476,   476,   476
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     6,     3,     3,     3,
       3,     1,     2,     1,     4,     4,     6,     6,     6,     8,
       8,    10,    12,    14,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     8,    14,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     2,     2,
       3,     3,     3,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     1,     6,     4,
       8,     4,     4,     4,     4,     4,     6,     6,     6,     6,
       8,     8,    10,    12,    14,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     5,     5,     5,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     2,     3,     3,     3,     3,     4,
       1,     3,     3,     2,     4,     5,     2,     2,     2,     2,
       3,     3,     6,     8,     2,     3,     2,     2,     2,     9,
       5,     4,     2,     5,     5,     7,     4,     5,     4,     4,
       3,     3,     3,     4,     2,     4,     4,     4,     3,     9,
       2,     2,     2,     2,     4,     3,     2,     3,     2,     2,
       8,     3,     2,     2,     2,     3,     3,     2,     2,     3,
       2,     2,     8,     3,     2,     2,     2,     3,     3,     2,
       2,     3,     2,     2,     8,     3,     2,     2,     2,     3,
       4,     4,     6,     2,     2,     3,     2,     2,     4,     3,
       2,     3,     2,     3,     4,     3,     2,     2,     4,     3,
       2,     4,     3,     2,     2,     2,     2,     4,     2,     4,
       3,     7,    13,     8,     2,     8,     2,     2,     3,     2,
       2,     2,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     3,     3,     3,     3,     3,
       4,     4,     3,     3,     3,     3,     4,     4,     2,     2,
       2,     3,     2,     2,     2,     2,     3,     3,     2,     3,
       3,     3,     4,     3,     5,     6,     6,     4,     2,     5,
       2,     2,     2,     2,     3,     1,     1,     2,     2,     2,
       1,     2,     1,     4,     1,     3,     3,     2,     2,     2,
       1,     2,     2,     3,     3,     3,     5,     5,     4,     4,
       4,     2,     3,     4,     4,     2,     3,     2,     2,     1,
      12,    18,    20,     6,     8,     6,     6,     6,    12,    10,
      10,     6,     4,    10,     6,     6,     8,     1,     2,     2,
       2,     1,     2,     2,     3,     3,     4,     3,     2,     3,
       4,     3,     4,     2,     4,     4,     6,     2,     2,     1,
       3,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       3,     3,     2,     2,     2,     2,     1,     1,     2,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     4,
       4,     4,     3,     3,     3,     3,     4,     4,     3,     3,
       3,     4,     3,     3,     4,     5,     3,     3,     4,     4,
       4,     6,     4,     4,     3,     3,     3,     3,     4,     3,
       3,     4,     4,     5,     6,     3,     3,     1,     3,     2,
       2,     2,     2,     2,     2,     2,     4,     1,     2,     3,
       3,     2,     2,     2,     1,     3,     3,     1,     2,     2,
       1,     2,     2,     2,     2,     3,     3,     1,     3,     2,
       4,     4,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     2,     2,     3,     3,     3,     3,     3,
       1,     1,     3,     2,     2,     4,     1,     2,     2,     2,
       2,     4,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     2,     1,     2,     3,     4,     1,
       2,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     8,     2,     2,     1,     1,     1,
       3,     4,     2,     3,     4,    17,     3,     3,     2,     2,
       2,     2,     3,     3,     3,     4,     4,     5,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       4,     3,     4,     4,     2,     1,     1,     4,     3,     3,
       4,     4,     2,     2,     2,     4,     5,     4,     4,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       1,     1,     2,     2,     2,     2,     2,     3,     3,     1,
       2,     1,     1,     1,     2,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* full_list: expr  */
#line 624 "pars.yacc"
               {
            expr_parsed = TRUE;
            s_result = (yyvsp[0].dval);
        }
#line 5772 "y.tab.c"
    break;

  case 4: /* full_list: vexpr  */
#line 628 "pars.yacc"
                {
            vexpr_parsed = TRUE;
            v_result = (yyvsp[0].vrbl);
        }
#line 5781 "y.tab.c"
    break;

  case 8: /* list: parmset  */
#line 641 "pars.yacc"
                  {}
#line 5787 "y.tab.c"
    break;

  case 9: /* list: parmset_obs  */
#line 642 "pars.yacc"
                      {}
#line 5793 "y.tab.c"
    break;

  case 10: /* list: regionset  */
#line 643 "pars.yacc"
                    {}
#line 5799 "y.tab.c"
    break;

  case 11: /* list: setaxis  */
#line 644 "pars.yacc"
                  {}
#line 5805 "y.tab.c"
    break;

  case 12: /* list: set_setprop  */
#line 645 "pars.yacc"
                      {}
#line 5811 "y.tab.c"
    break;

  case 13: /* list: actions  */
#line 646 "pars.yacc"
                  {}
#line 5817 "y.tab.c"
    break;

  case 14: /* list: options  */
#line 647 "pars.yacc"
                  {}
#line 5823 "y.tab.c"
    break;

  case 15: /* list: asgn  */
#line 648 "pars.yacc"
               {}
#line 5829 "y.tab.c"
    break;

  case 16: /* list: vasgn  */
#line 649 "pars.yacc"
                {}
#line 5835 "y.tab.c"
    break;

  case 17: /* list: defines  */
#line 650 "pars.yacc"
                  {}
#line 5841 "y.tab.c"
    break;

  case 18: /* list: error  */
#line 651 "pars.yacc"
                {
	    return 1;
	}
#line 5849 "y.tab.c"
    break;

  case 19: /* expr: NUMBER  */
#line 658 "pars.yacc"
               {
	    (yyval.dval) = (yyvsp[0].dval);
	}
#line 5857 "y.tab.c"
    break;

  case 20: /* expr: VAR_D  */
#line 661 "pars.yacc"
                 {
	    (yyval.dval) = *((yyvsp[0].dptr));
	}
#line 5865 "y.tab.c"
    break;

  case 21: /* expr: FITPARM  */
#line 664 "pars.yacc"
                   {
	    (yyval.dval) = nonl_parms[(yyvsp[0].ival)].value;
	}
#line 5873 "y.tab.c"
    break;

  case 22: /* expr: FITPMAX  */
#line 667 "pars.yacc"
                   {
	    (yyval.dval) = nonl_parms[(yyvsp[0].ival)].max;
	}
#line 5881 "y.tab.c"
    break;

  case 23: /* expr: FITPMIN  */
#line 670 "pars.yacc"
                   {
	    (yyval.dval) = nonl_parms[(yyvsp[0].ival)].min;
	}
#line 5889 "y.tab.c"
    break;

  case 24: /* expr: array indx  */
#line 673 "pars.yacc"
                      {
            if ((yyvsp[0].ival) >= (yyvsp[-1].vrbl)->length) {
                errmsg("Access beyond array bounds");
                return 1;
            }
            (yyval.dval) = (yyvsp[-1].vrbl)->data[(yyvsp[0].ival)];
	}
#line 5901 "y.tab.c"
    break;

  case 25: /* expr: stattype '(' vexpr ')'  */
#line 680 "pars.yacc"
                                 {
	    double dummy, dummy2;
            int idummy, ind, length = (yyvsp[-1].vrbl)->length;
	    if ((yyvsp[-1].vrbl)->data == NULL) {
		yyerror("NULL variable, check set type");
		return 1;
	    }
	    switch ((yyvsp[-3].ival)) {
	    case MINP:
		(yyval.dval) = vmin((yyvsp[-1].vrbl)->data, length);
		break;
	    case MAXP:
		(yyval.dval) = vmax((yyvsp[-1].vrbl)->data, length);
		break;
            case AVG:
		stasum((yyvsp[-1].vrbl)->data, length, &(yyval.dval), &dummy);
                break;
            case SD:
		stasum((yyvsp[-1].vrbl)->data, length, &dummy, &(yyval.dval));
                break;
            case SUM:
		stasum((yyvsp[-1].vrbl)->data, length, &(yyval.dval), &dummy);
                (yyval.dval) *= length;
                break;
            case IMIN:
		minmax((yyvsp[-1].vrbl)->data, length, &dummy, &dummy2, &ind, &idummy);
                (yyval.dval) = (double) ind;
                break;
            case IMAX:
		minmax((yyvsp[-1].vrbl)->data, length, &dummy, &dummy2, &idummy, &ind);
                (yyval.dval) = (double) ind;
                break;
	    }
	}
#line 5940 "y.tab.c"
    break;

  case 26: /* expr: INT '(' vexpr ',' vexpr ')'  */
#line 714 "pars.yacc"
                                      {
	    if ((yyvsp[-3].vrbl)->length != (yyvsp[-1].vrbl)->length) {
		yyerror("X and Y are of different length");
		return 1;
            } else {
                (yyval.dval) = trapint((yyvsp[-3].vrbl)->data, (yyvsp[-1].vrbl)->data, NULL, NULL, (yyvsp[-3].vrbl)->length);
            }
	}
#line 5953 "y.tab.c"
    break;

  case 27: /* expr: array '.' LENGTH  */
#line 722 "pars.yacc"
                           {
	    (yyval.dval) = (yyvsp[-2].vrbl)->length;
	}
#line 5961 "y.tab.c"
    break;

  case 28: /* expr: selectset '.' LENGTH  */
#line 725 "pars.yacc"
                               {
	    (yyval.dval) = getsetlength((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno);
	}
#line 5969 "y.tab.c"
    break;

  case 29: /* expr: selectset '.' ID  */
#line 728 "pars.yacc"
                           {
	    (yyval.dval) = (yyvsp[-2].trgt)->setno;
	}
#line 5977 "y.tab.c"
    break;

  case 30: /* expr: selectgraph '.' ID  */
#line 731 "pars.yacc"
                             {
	    (yyval.dval) = (yyvsp[-2].ival);
	}
#line 5985 "y.tab.c"
    break;

  case 31: /* expr: CONSTANT  */
#line 735 "pars.yacc"
        {
            (yyval.dval) = ((ParserFnc) (key[(yyvsp[0].ival)].data)) ();
	}
#line 5993 "y.tab.c"
    break;

  case 32: /* expr: expr UCONSTANT  */
#line 739 "pars.yacc"
        {
	    (yyval.dval) = (yyvsp[-1].dval) * ((ParserFnc) (key[(yyvsp[0].ival)].data)) ();
	}
#line 6001 "y.tab.c"
    break;

  case 33: /* expr: RAND  */
#line 743 "pars.yacc"
        {
	    (yyval.dval) = drand48();
	}
#line 6009 "y.tab.c"
    break;

  case 34: /* expr: FUNC_I '(' iexpr ')'  */
#line 747 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-3].ival)].data)) ((yyvsp[-1].ival));
	}
#line 6017 "y.tab.c"
    break;

  case 35: /* expr: FUNC_D '(' expr ')'  */
#line 751 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-3].ival)].data)) ((yyvsp[-1].dval));
	}
#line 6025 "y.tab.c"
    break;

  case 36: /* expr: FUNC_ND '(' iexpr ',' expr ')'  */
#line 755 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].ival), (yyvsp[-1].dval));
	}
#line 6033 "y.tab.c"
    break;

  case 37: /* expr: FUNC_NN '(' iexpr ',' iexpr ')'  */
#line 759 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].ival), (yyvsp[-1].ival));
	}
#line 6041 "y.tab.c"
    break;

  case 38: /* expr: FUNC_DD '(' expr ',' expr ')'  */
#line 763 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].dval), (yyvsp[-1].dval));
	}
#line 6049 "y.tab.c"
    break;

  case 39: /* expr: FUNC_NND '(' iexpr ',' iexpr ',' expr ')'  */
#line 767 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-7].ival)].data)) ((yyvsp[-5].ival), (yyvsp[-3].ival), (yyvsp[-1].dval));
	}
#line 6057 "y.tab.c"
    break;

  case 40: /* expr: FUNC_PPD '(' expr ',' expr ',' expr ')'  */
#line 771 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-7].ival)].data)) ((yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval));
	}
#line 6065 "y.tab.c"
    break;

  case 41: /* expr: FUNC_PPPD '(' expr ',' expr ',' expr ',' expr ')'  */
#line 775 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-9].ival)].data)) ((yyvsp[-7].dval), (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval));
	}
#line 6073 "y.tab.c"
    break;

  case 42: /* expr: FUNC_PPPPD '(' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 779 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-11].ival)].data)) ((yyvsp[-9].dval), (yyvsp[-7].dval), (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval));
	}
#line 6081 "y.tab.c"
    break;

  case 43: /* expr: FUNC_PPPPPD '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 783 "pars.yacc"
        {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[-13].ival)].data)) ((yyvsp[-11].dval), (yyvsp[-9].dval), (yyvsp[-7].dval), (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval));
	}
#line 6089 "y.tab.c"
    break;

  case 44: /* expr: selectgraph '.' VX1  */
#line 786 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].v.xv1;
	}
#line 6097 "y.tab.c"
    break;

  case 45: /* expr: selectgraph '.' VX2  */
#line 789 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].v.xv2;
	}
#line 6105 "y.tab.c"
    break;

  case 46: /* expr: selectgraph '.' VY1  */
#line 792 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].v.yv1;
	}
#line 6113 "y.tab.c"
    break;

  case 47: /* expr: selectgraph '.' VY2  */
#line 795 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].v.yv2;
	}
#line 6121 "y.tab.c"
    break;

  case 48: /* expr: selectgraph '.' WX1  */
#line 798 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].w.xg1;
	}
#line 6129 "y.tab.c"
    break;

  case 49: /* expr: selectgraph '.' WX2  */
#line 801 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].w.xg2;
	}
#line 6137 "y.tab.c"
    break;

  case 50: /* expr: selectgraph '.' WY1  */
#line 804 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].w.yg1;
	}
#line 6145 "y.tab.c"
    break;

  case 51: /* expr: selectgraph '.' WY2  */
#line 807 "pars.yacc"
                              {
	    (yyval.dval) = g[(yyvsp[-2].ival)].w.yg2;
	}
#line 6153 "y.tab.c"
    break;

  case 52: /* expr: DATE '(' jdate ')'  */
#line 810 "pars.yacc"
                             {
            (yyval.dval) = (yyvsp[-1].dval);
	}
#line 6161 "y.tab.c"
    break;

  case 53: /* expr: DATE '(' iexpr ',' nexpr ',' nexpr ')'  */
#line 813 "pars.yacc"
                                                 { /* yr, mo, day */
	    (yyval.dval) = cal_and_time_to_jul((yyvsp[-5].ival), (yyvsp[-3].ival), (yyvsp[-1].ival), 12, 0, 0.0);
	}
#line 6169 "y.tab.c"
    break;

  case 54: /* expr: DATE '(' iexpr ',' nexpr ',' nexpr ',' nexpr ',' nexpr ',' expr ')'  */
#line 817 "pars.yacc"
        { /* yr, mo, day, hr, min, sec */
	    (yyval.dval) = cal_and_time_to_jul((yyvsp[-11].ival), (yyvsp[-9].ival), (yyvsp[-7].ival), (yyvsp[-5].ival), (yyvsp[-3].ival), (yyvsp[-1].dval));
	}
#line 6177 "y.tab.c"
    break;

  case 55: /* expr: VX1  */
#line 820 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            (yyval.dval) = g[whichgraph].v.xv1;
	}
#line 6189 "y.tab.c"
    break;

  case 56: /* expr: VX2  */
#line 827 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.xv2;
	}
#line 6201 "y.tab.c"
    break;

  case 57: /* expr: VY1  */
#line 834 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.yv1;
	}
#line 6213 "y.tab.c"
    break;

  case 58: /* expr: VY2  */
#line 841 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.yv2;
	}
#line 6225 "y.tab.c"
    break;

  case 59: /* expr: WX1  */
#line 848 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.xg1;
	}
#line 6237 "y.tab.c"
    break;

  case 60: /* expr: WX2  */
#line 855 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.xg2;
	}
#line 6249 "y.tab.c"
    break;

  case 61: /* expr: WY1  */
#line 862 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.yg1;
	}
#line 6261 "y.tab.c"
    break;

  case 62: /* expr: WY2  */
#line 869 "pars.yacc"
              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.yg2;
	}
#line 6273 "y.tab.c"
    break;

  case 63: /* expr: VXMAX  */
#line 876 "pars.yacc"
                {
	    double vx, vy;
            get_page_viewport(&vx, &vy);
            (yyval.dval) = vx;
	}
#line 6283 "y.tab.c"
    break;

  case 64: /* expr: VYMAX  */
#line 881 "pars.yacc"
                {
	    double vx, vy;
            get_page_viewport(&vx, &vy);
            (yyval.dval) = vy;
	}
#line 6293 "y.tab.c"
    break;

  case 65: /* expr: '(' expr ')'  */
#line 886 "pars.yacc"
                       {
	    (yyval.dval) = (yyvsp[-1].dval);
	}
#line 6301 "y.tab.c"
    break;

  case 66: /* expr: expr '+' expr  */
#line 889 "pars.yacc"
                        {
	    (yyval.dval) = (yyvsp[-2].dval) + (yyvsp[0].dval);
	}
#line 6309 "y.tab.c"
    break;

  case 67: /* expr: expr '-' expr  */
#line 892 "pars.yacc"
                        {
	    (yyval.dval) = (yyvsp[-2].dval) - (yyvsp[0].dval);
	}
#line 6317 "y.tab.c"
    break;

  case 68: /* expr: '-' expr  */
#line 895 "pars.yacc"
                                {
	    (yyval.dval) = -(yyvsp[0].dval);
	}
#line 6325 "y.tab.c"
    break;

  case 69: /* expr: '+' expr  */
#line 898 "pars.yacc"
                                {
	    (yyval.dval) = (yyvsp[0].dval);
	}
#line 6333 "y.tab.c"
    break;

  case 70: /* expr: expr '*' expr  */
#line 901 "pars.yacc"
                        {
	    (yyval.dval) = (yyvsp[-2].dval) * (yyvsp[0].dval);
	}
#line 6341 "y.tab.c"
    break;

  case 71: /* expr: expr '/' expr  */
#line 905 "pars.yacc"
        {
	    if ((yyvsp[0].dval) != 0.0) {
		(yyval.dval) = (yyvsp[-2].dval) / (yyvsp[0].dval);
	    } else {
		yyerror("Divide by zero");
		return 1;
	    }
	}
#line 6354 "y.tab.c"
    break;

  case 72: /* expr: expr '%' expr  */
#line 913 "pars.yacc"
                        {
	    if ((yyvsp[0].dval) != 0.0) {
		(yyval.dval) = fmod((yyvsp[-2].dval), (yyvsp[0].dval));
	    } else {
		yyerror("Divide by zero");
		return 1;
	    }
	}
#line 6367 "y.tab.c"
    break;

  case 73: /* expr: expr '^' expr  */
#line 921 "pars.yacc"
                        {
	    if ((yyvsp[-2].dval) < 0 && rint((yyvsp[0].dval)) != (yyvsp[0].dval)) {
		yyerror("Negative value raised to non-integer power");
		return 1;
            } else if ((yyvsp[-2].dval) == 0.0 && (yyvsp[0].dval) <= 0.0) {
		yyerror("Zero raised to non-positive power");
		return 1;
            } else {
                (yyval.dval) = pow((yyvsp[-2].dval), (yyvsp[0].dval));
            }
	}
#line 6383 "y.tab.c"
    break;

  case 74: /* expr: expr '?' expr ':' expr  */
#line 932 "pars.yacc"
                                 {
	    (yyval.dval) = (yyvsp[-4].dval) ? (yyvsp[-2].dval) : (yyvsp[0].dval);
	}
#line 6391 "y.tab.c"
    break;

  case 75: /* expr: expr GT expr  */
#line 935 "pars.yacc"
                       {
	   (yyval.dval) = ((yyvsp[-2].dval) > (yyvsp[0].dval));
	}
#line 6399 "y.tab.c"
    break;

  case 76: /* expr: expr LT expr  */
#line 938 "pars.yacc"
                        {
	   (yyval.dval) = ((yyvsp[-2].dval) < (yyvsp[0].dval));
	}
#line 6407 "y.tab.c"
    break;

  case 77: /* expr: expr LE expr  */
#line 941 "pars.yacc"
                       {
	   (yyval.dval) = ((yyvsp[-2].dval) <= (yyvsp[0].dval));
	}
#line 6415 "y.tab.c"
    break;

  case 78: /* expr: expr GE expr  */
#line 944 "pars.yacc"
                       {
	   (yyval.dval) = ((yyvsp[-2].dval) >= (yyvsp[0].dval));
	}
#line 6423 "y.tab.c"
    break;

  case 79: /* expr: expr EQ expr  */
#line 947 "pars.yacc"
                       {
	   (yyval.dval) = ((yyvsp[-2].dval) == (yyvsp[0].dval));
	}
#line 6431 "y.tab.c"
    break;

  case 80: /* expr: expr NE expr  */
#line 950 "pars.yacc"
                       {
	    (yyval.dval) = ((yyvsp[-2].dval) != (yyvsp[0].dval));
	}
#line 6439 "y.tab.c"
    break;

  case 81: /* expr: expr AND expr  */
#line 953 "pars.yacc"
                        {
	    (yyval.dval) = (yyvsp[-2].dval) && (yyvsp[0].dval);
	}
#line 6447 "y.tab.c"
    break;

  case 82: /* expr: expr OR expr  */
#line 956 "pars.yacc"
                       {
	    (yyval.dval) = (yyvsp[-2].dval) || (yyvsp[0].dval);
	}
#line 6455 "y.tab.c"
    break;

  case 83: /* expr: NOT expr  */
#line 959 "pars.yacc"
                   {
	    (yyval.dval) = !((yyvsp[0].dval));
	}
#line 6463 "y.tab.c"
    break;

  case 84: /* sexpr: CHRSTR  */
#line 964 "pars.yacc"
               {
            (yyval.sval) = (yyvsp[0].sval);
	}
#line 6471 "y.tab.c"
    break;

  case 85: /* sexpr: sexpr '.' sexpr  */
#line 967 "pars.yacc"
                          {
            (yyval.sval) = concat_strings((yyvsp[-2].sval), (yyvsp[0].sval));
            xfree((yyvsp[0].sval));
        }
#line 6480 "y.tab.c"
    break;

  case 86: /* sexpr: sexpr '.' expr  */
#line 971 "pars.yacc"
                         {
            char buf[32];
            set_locale_num(TRUE);
            sprintf(buf, "%g", (yyvsp[0].dval));
            set_locale_num(FALSE);
            (yyval.sval) = concat_strings((yyvsp[-2].sval), buf);
        }
#line 6492 "y.tab.c"
    break;

  case 87: /* iexpr: expr  */
#line 980 "pars.yacc"
             {
	    int itmp = rint((yyvsp[0].dval));
            if (fabs(itmp - (yyvsp[0].dval)) > 1.e-6) {
		yyerror("Non-integer value supplied for integer");
		return 1;
            }
            (yyval.ival) = itmp;
        }
#line 6505 "y.tab.c"
    break;

  case 88: /* nexpr: iexpr  */
#line 990 "pars.yacc"
              {
            if ((yyvsp[0].ival) < 0) {
		yyerror("Negative value supplied for non-negative");
		return 1;
            }
            (yyval.ival) = (yyvsp[0].ival);
	}
#line 6517 "y.tab.c"
    break;

  case 89: /* indx: '[' iexpr ']'  */
#line 999 "pars.yacc"
                      {
	    int itmp = (yyvsp[-1].ival) - index_shift;
            if (itmp < 0) {
		yyerror("Negative index");
		return 1;
            }
            (yyval.ival) = itmp;
	}
#line 6530 "y.tab.c"
    break;

  case 90: /* jdate: expr  */
#line 1009 "pars.yacc"
             {
            (yyval.dval) = (yyvsp[0].dval);
        }
#line 6538 "y.tab.c"
    break;

  case 91: /* jdate: sexpr  */
#line 1012 "pars.yacc"
                {
            double jul;
            Dates_format dummy;
            if (parse_date((yyvsp[0].sval), get_date_hint(), FALSE, &jul, &dummy)
                == RETURN_SUCCESS) {
                xfree((yyvsp[0].sval));
                (yyval.dval) = jul;
            } else {
                xfree((yyvsp[0].sval));
		yyerror("Invalid date");
		return 1;
            }
        }
#line 6556 "y.tab.c"
    break;

  case 92: /* jrawdate: expr  */
#line 1027 "pars.yacc"
                {
            (yyval.dval) = (yyvsp[0].dval);
        }
#line 6564 "y.tab.c"
    break;

  case 93: /* jrawdate: sexpr  */
#line 1030 "pars.yacc"
                {
            double jul;
            Dates_format dummy;
            if (parse_date((yyvsp[0].sval), get_date_hint(), TRUE, &jul, &dummy)
                == RETURN_SUCCESS) {
                xfree((yyvsp[0].sval));
                (yyval.dval) = jul;
            } else {
                xfree((yyvsp[0].sval));
		yyerror("Invalid date");
		return 1;
            }
        }
#line 6582 "y.tab.c"
    break;

  case 94: /* array: VEC_D  */
#line 1047 "pars.yacc"
        {
            (yyval.vrbl) = (yyvsp[0].vrbl);
	}
#line 6590 "y.tab.c"
    break;

  case 95: /* array: datacolumn  */
#line 1051 "pars.yacc"
        {
	    double *ptr = getcol(vasgn_gno, vasgn_setno, (yyvsp[0].ival));
            (yyval.vrbl) = &freelist[fcnt++];
            (yyval.vrbl)->type = GRARR_SET;
            (yyval.vrbl)->data = ptr;
            if (ptr == NULL) {
                errmsg("NULL variable - check set type");
                return 1;
            } else {
                (yyval.vrbl)->length = getsetlength(vasgn_gno, vasgn_setno);
            }
	}
#line 6607 "y.tab.c"
    break;

  case 96: /* array: selectset '.' datacolumn  */
#line 1064 "pars.yacc"
        {
	    double *ptr = getcol((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].ival));
            (yyval.vrbl) = &freelist[fcnt++];
            (yyval.vrbl)->type = GRARR_SET;
            (yyval.vrbl)->data = ptr;
            if (ptr == NULL) {
                errmsg("NULL variable - check set type");
                return 1;
            } else {
                (yyval.vrbl)->length = getsetlength((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno);
            }
	}
#line 6624 "y.tab.c"
    break;

  case 97: /* vexpr: array  */
#line 1080 "pars.yacc"
        {
            (yyval.vrbl) = (yyvsp[0].vrbl);
	}
#line 6632 "y.tab.c"
    break;

  case 98: /* vexpr: array '[' iexpr ':' iexpr ']'  */
#line 1084 "pars.yacc"
        {
            int start = (yyvsp[-3].ival) - index_shift, stop = (yyvsp[-1].ival) - index_shift;
            if (start < 0 || stop < start || stop >= (yyvsp[-5].vrbl)->length) {
		yyerror("Invalid index range");
            } else {
                int len = stop - start + 1;
	        double *ptr = xmalloc(len*SIZEOF_DOUBLE);
                if ((yyval.vrbl)->data == NULL) {
                    yyerror("Not enough memory");
                } else {
                    int i;
                    (yyval.vrbl) = &freelist[fcnt++];
	            (yyval.vrbl)->data = ptr;
                    (yyval.vrbl)->length = len;
                    (yyval.vrbl)->type = GRARR_TMP;
                    for (i = 0; i < len; i++) {
                        (yyval.vrbl)->data[i] = (yyvsp[-5].vrbl)->data[i + (yyvsp[-3].ival)];
                    }
                }
            }
	}
#line 6658 "y.tab.c"
    break;

  case 99: /* vexpr: MESH '(' nexpr ')'  */
#line 1106 "pars.yacc"
        {
            int len = (yyvsp[-1].ival);
            if (len < 1) {
                yyerror("npoints must be > 0");
            } else {
                double *ptr = allocate_index_data(len);
                if (ptr == NULL) {
                    errmsg("Malloc failed");
                    return 1;
                } else {
                    (yyval.vrbl) = &freelist[fcnt++];
                    (yyval.vrbl)->type = GRARR_TMP;
                    (yyval.vrbl)->data = ptr;
                    (yyval.vrbl)->length = len;
                }
            }
	}
#line 6680 "y.tab.c"
    break;

  case 100: /* vexpr: MESH '(' expr ',' expr ',' nexpr ')'  */
#line 1124 "pars.yacc"
        {
            int len = (yyvsp[-1].ival);
            if (len < 2) {
                yyerror("npoints must be > 1");
            } else {
                double *ptr = allocate_mesh((yyvsp[-5].dval), (yyvsp[-3].dval), len);
                if (ptr == NULL) {
                    errmsg("Malloc failed");
                    return 1;
                } else {
                    (yyval.vrbl) = &freelist[fcnt++];
                    (yyval.vrbl)->type = GRARR_TMP;
                    (yyval.vrbl)->data = ptr;
                    (yyval.vrbl)->length = len;
                }
            }
	}
#line 6702 "y.tab.c"
    break;

  case 101: /* vexpr: RAND '(' nexpr ')'  */
#line 1142 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    (yyval.vrbl)->data = xmalloc((yyvsp[-1].ival)*SIZEOF_DOUBLE);
            if ((yyval.vrbl)->data == NULL) {
                errmsg("Not enough memory");
                return 1;
            } else {
                (yyval.vrbl)->length = (yyvsp[-1].ival);
                (yyval.vrbl)->type = GRARR_TMP;
            }
            for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = drand48();
	    }
	}
#line 6722 "y.tab.c"
    break;

  case 102: /* vexpr: REGNUM '(' selectset ')'  */
#line 1158 "pars.yacc"
        {
	    int rtype, i, len;
            char *rarray;
            
            rtype = RESTRICT_REG0 + (yyvsp[-3].ival);
            
	    if (get_restriction_array((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno,
                rtype, FALSE, &rarray) != RETURN_SUCCESS) {
                errmsg("Error in region evaluation");
                return 1;
	    }

            len = getsetlength((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno);
            (yyval.vrbl) = &freelist[fcnt++];
	    (yyval.vrbl)->data = xmalloc(len*SIZEOF_DOUBLE);
            if ((yyval.vrbl)->data == NULL) {
                errmsg("Not enough memory");
                return 1;
            } else {
                (yyval.vrbl)->length = len;
                (yyval.vrbl)->type = GRARR_TMP;
            }
            for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = rarray[i];
	    }
            
            xfree(rarray);
	}
#line 6755 "y.tab.c"
    break;

  case 103: /* vexpr: RSUM '(' vexpr ')'  */
#line 1187 "pars.yacc"
        {
            int i;
            (yyval.vrbl) = &freelist[fcnt++];
            copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 1; i < (yyval.vrbl)->length; i++) {
                (yyval.vrbl)->data[i] += (yyval.vrbl)->data[i - 1];
            }
	}
#line 6769 "y.tab.c"
    break;

  case 104: /* vexpr: FUNC_I '(' vexpr ')'  */
#line 1197 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-3].ival)].data)) ((int) ((yyvsp[-1].vrbl)->data[i]));
	    }
	}
#line 6783 "y.tab.c"
    break;

  case 105: /* vexpr: FUNC_D '(' vexpr ')'  */
#line 1207 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-3].ival)].data)) (((yyvsp[-1].vrbl)->data[i]));
	    }
	}
#line 6797 "y.tab.c"
    break;

  case 106: /* vexpr: FUNC_DD '(' vexpr ',' vexpr ')'  */
#line 1217 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-3].vrbl)->length != (yyvsp[-1].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-3].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].vrbl)->data[i], (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6816 "y.tab.c"
    break;

  case 107: /* vexpr: FUNC_DD '(' expr ',' vexpr ')'  */
#line 1232 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].dval), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6831 "y.tab.c"
    break;

  case 108: /* vexpr: FUNC_DD '(' vexpr ',' expr ')'  */
#line 1243 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-3].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].vrbl)->data[i], (yyvsp[-1].dval));
	    }
	}
#line 6846 "y.tab.c"
    break;

  case 109: /* vexpr: FUNC_ND '(' iexpr ',' vexpr ')'  */
#line 1254 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-5].ival)].data)) ((yyvsp[-3].ival), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6861 "y.tab.c"
    break;

  case 110: /* vexpr: FUNC_NND '(' iexpr ',' iexpr ',' vexpr ')'  */
#line 1265 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-7].ival)].data)) ((yyvsp[-5].ival), (yyvsp[-3].ival), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6876 "y.tab.c"
    break;

  case 111: /* vexpr: FUNC_PPD '(' expr ',' expr ',' vexpr ')'  */
#line 1276 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-7].ival)].data)) ((yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6891 "y.tab.c"
    break;

  case 112: /* vexpr: FUNC_PPPD '(' expr ',' expr ',' expr ',' vexpr ')'  */
#line 1287 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-9].ival)].data)) ((yyvsp[-7].dval), (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6906 "y.tab.c"
    break;

  case 113: /* vexpr: FUNC_PPPPD '(' expr ',' expr ',' expr ',' expr ',' vexpr ')'  */
#line 1298 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-11].ival)].data)) ((yyvsp[-9].dval), (yyvsp[-7].dval), (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6921 "y.tab.c"
    break;

  case 114: /* vexpr: FUNC_PPPPPD '(' expr ',' expr ',' expr ',' expr ',' expr ',' vexpr ')'  */
#line 1309 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[-13].ival)].data)) ((yyvsp[-11].dval), (yyvsp[-9].dval), (yyvsp[-7].dval), (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].vrbl)->data[i]);
	    }
	}
#line 6936 "y.tab.c"
    break;

  case 115: /* vexpr: vexpr '+' vexpr  */
#line 1320 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] + (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 6955 "y.tab.c"
    break;

  case 116: /* vexpr: vexpr '+' expr  */
#line 1335 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] + (yyvsp[0].dval);
	    }
	}
#line 6970 "y.tab.c"
    break;

  case 117: /* vexpr: expr '+' vexpr  */
#line 1346 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].dval) + (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 6985 "y.tab.c"
    break;

  case 118: /* vexpr: vexpr '-' vexpr  */
#line 1357 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] - (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7004 "y.tab.c"
    break;

  case 119: /* vexpr: vexpr '-' expr  */
#line 1372 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] - (yyvsp[0].dval);
	    }
	}
#line 7019 "y.tab.c"
    break;

  case 120: /* vexpr: expr '-' vexpr  */
#line 1383 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].dval) - (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7034 "y.tab.c"
    break;

  case 121: /* vexpr: vexpr '*' vexpr  */
#line 1394 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] * (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7053 "y.tab.c"
    break;

  case 122: /* vexpr: vexpr '*' expr  */
#line 1409 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] * (yyvsp[0].dval);
	    }
	}
#line 7068 "y.tab.c"
    break;

  case 123: /* vexpr: expr '*' vexpr  */
#line 1420 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].dval) * (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7083 "y.tab.c"
    break;

  case 124: /* vexpr: vexpr '/' vexpr  */
#line 1431 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[0].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                }
                (yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] / (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7106 "y.tab.c"
    break;

  case 125: /* vexpr: vexpr '/' expr  */
#line 1450 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[0].dval) == 0.0) {
                errmsg("Divide by zero");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] / (yyvsp[0].dval);
	    }
	}
#line 7125 "y.tab.c"
    break;

  case 126: /* vexpr: expr '/' vexpr  */
#line 1465 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[0].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                }
		(yyval.vrbl)->data[i] = (yyvsp[-2].dval) / (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7144 "y.tab.c"
    break;

  case 127: /* vexpr: vexpr '%' vexpr  */
#line 1480 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[0].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                } else {
                    (yyval.vrbl)->data[i] = fmod((yyvsp[-2].vrbl)->data[i], (yyvsp[0].vrbl)->data[i]);
                }
	    }
	}
#line 7168 "y.tab.c"
    break;

  case 128: /* vexpr: vexpr '%' expr  */
#line 1500 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[0].dval) == 0.0) {
                errmsg("Divide by zero");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = fmod((yyvsp[-2].vrbl)->data[i], (yyvsp[0].dval));
	    }
	}
#line 7187 "y.tab.c"
    break;

  case 129: /* vexpr: expr '%' vexpr  */
#line 1515 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[0].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                } else {
		    (yyval.vrbl)->data[i] = fmod((yyvsp[-2].dval), (yyvsp[0].vrbl)->data[i]);
                }
	    }
	}
#line 7207 "y.tab.c"
    break;

  case 130: /* vexpr: vexpr '^' vexpr  */
#line 1531 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
	        if ((yyvsp[-2].vrbl)->data[i] < 0 && rint((yyvsp[0].vrbl)->data[i]) != (yyvsp[0].vrbl)->data[i]) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[-2].vrbl)->data[i] == 0.0 && (yyvsp[0].vrbl)->data[i] <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[-2].vrbl)->data[i], (yyvsp[0].vrbl)->data[i]);
                }
	    }
	}
#line 7234 "y.tab.c"
    break;

  case 131: /* vexpr: vexpr '^' expr  */
#line 1554 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
	        if ((yyvsp[-2].vrbl)->data[i] < 0 && rint((yyvsp[0].dval)) != (yyvsp[0].dval)) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[-2].vrbl)->data[i] == 0.0 && (yyvsp[0].dval) <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[-2].vrbl)->data[i], (yyvsp[0].dval));
                }
	    }
	}
#line 7257 "y.tab.c"
    break;

  case 132: /* vexpr: expr '^' vexpr  */
#line 1573 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
	        if ((yyvsp[-2].dval) < 0 && rint((yyvsp[0].vrbl)->data[i]) != (yyvsp[0].vrbl)->data[i]) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[-2].dval) == 0.0 && (yyvsp[0].vrbl)->data[i] <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[-2].dval), (yyvsp[0].vrbl)->data[i]);
                }
	    }
	}
#line 7280 "y.tab.c"
    break;

  case 133: /* vexpr: vexpr UCONSTANT  */
#line 1592 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-1].vrbl)->data[i] * ((ParserFnc) (key[(yyvsp[0].ival)].data)) ();
	    }
	}
#line 7294 "y.tab.c"
    break;

  case 134: /* vexpr: vexpr '?' expr ':' expr  */
#line 1601 "pars.yacc"
                                  {
            int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-4].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[-4].vrbl)->data[i]) ? (yyvsp[-2].dval) : (yyvsp[0].dval);
            }
	}
#line 7308 "y.tab.c"
    break;

  case 135: /* vexpr: vexpr '?' expr ':' vexpr  */
#line 1610 "pars.yacc"
                                   {
            int i;
	    if ((yyvsp[-4].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-4].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[-4].vrbl)->data[i]) ? (yyvsp[-2].dval) : (yyvsp[0].vrbl)->data[i];
            }
	}
#line 7326 "y.tab.c"
    break;

  case 136: /* vexpr: vexpr '?' vexpr ':' expr  */
#line 1623 "pars.yacc"
                                   {
            int i;
	    if ((yyvsp[-4].vrbl)->length != (yyvsp[-2].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-4].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[-4].vrbl)->data[i]) ? (yyvsp[-2].vrbl)->data[i] : (yyvsp[0].dval);
            }
	}
#line 7344 "y.tab.c"
    break;

  case 137: /* vexpr: vexpr '?' vexpr ':' vexpr  */
#line 1636 "pars.yacc"
                                    {
            int i;
	    if ((yyvsp[-4].vrbl)->length != (yyvsp[0].vrbl)->length || (yyvsp[-4].vrbl)->length != (yyvsp[-2].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-4].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[-4].vrbl)->data[i]) ? (yyvsp[-2].vrbl)->data[i] : (yyvsp[0].vrbl)->data[i];
            }
	}
#line 7362 "y.tab.c"
    break;

  case 138: /* vexpr: vexpr OR vexpr  */
#line 1650 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] || (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7381 "y.tab.c"
    break;

  case 139: /* vexpr: vexpr OR expr  */
#line 1665 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] || (yyvsp[0].dval);
	    }
	}
#line 7396 "y.tab.c"
    break;

  case 140: /* vexpr: expr OR vexpr  */
#line 1676 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].dval) || (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7411 "y.tab.c"
    break;

  case 141: /* vexpr: vexpr AND vexpr  */
#line 1687 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] && (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7430 "y.tab.c"
    break;

  case 142: /* vexpr: vexpr AND expr  */
#line 1702 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].vrbl)->data[i] && (yyvsp[0].dval);
	    }
	}
#line 7445 "y.tab.c"
    break;

  case 143: /* vexpr: expr AND vexpr  */
#line 1713 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[-2].dval) && (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7460 "y.tab.c"
    break;

  case 144: /* vexpr: vexpr GT vexpr  */
#line 1724 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] > (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7479 "y.tab.c"
    break;

  case 145: /* vexpr: vexpr GT expr  */
#line 1739 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] > (yyvsp[0].dval));
	    }
	}
#line 7494 "y.tab.c"
    break;

  case 146: /* vexpr: expr GT vexpr  */
#line 1750 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].dval) > (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7509 "y.tab.c"
    break;

  case 147: /* vexpr: vexpr LT vexpr  */
#line 1761 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] < (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7528 "y.tab.c"
    break;

  case 148: /* vexpr: vexpr LT expr  */
#line 1776 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] < (yyvsp[0].dval));
	    }
	}
#line 7543 "y.tab.c"
    break;

  case 149: /* vexpr: expr LT vexpr  */
#line 1787 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].dval) < (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7558 "y.tab.c"
    break;

  case 150: /* vexpr: vexpr GE vexpr  */
#line 1798 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] >= (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7577 "y.tab.c"
    break;

  case 151: /* vexpr: vexpr GE expr  */
#line 1813 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] >= (yyvsp[0].dval));
	    }
	}
#line 7592 "y.tab.c"
    break;

  case 152: /* vexpr: expr GE vexpr  */
#line 1824 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].dval) >= (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7607 "y.tab.c"
    break;

  case 153: /* vexpr: vexpr LE vexpr  */
#line 1835 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] <= (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7626 "y.tab.c"
    break;

  case 154: /* vexpr: vexpr LE expr  */
#line 1850 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] <= (yyvsp[0].dval));
	    }
	}
#line 7641 "y.tab.c"
    break;

  case 155: /* vexpr: expr LE vexpr  */
#line 1861 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].dval) <= (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7656 "y.tab.c"
    break;

  case 156: /* vexpr: vexpr EQ vexpr  */
#line 1872 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] == (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7675 "y.tab.c"
    break;

  case 157: /* vexpr: vexpr EQ expr  */
#line 1887 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] == (yyvsp[0].dval));
	    }
	}
#line 7690 "y.tab.c"
    break;

  case 158: /* vexpr: expr EQ vexpr  */
#line 1898 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].dval) == (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7705 "y.tab.c"
    break;

  case 159: /* vexpr: vexpr NE vexpr  */
#line 1909 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] != (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7724 "y.tab.c"
    break;

  case 160: /* vexpr: vexpr NE expr  */
#line 1924 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-2].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].vrbl)->data[i] != (yyvsp[0].dval));
	    }
	}
#line 7739 "y.tab.c"
    break;

  case 161: /* vexpr: expr NE vexpr  */
#line 1935 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[-2].dval) != (yyvsp[0].vrbl)->data[i]);
	    }
	}
#line 7754 "y.tab.c"
    break;

  case 162: /* vexpr: NOT vexpr  */
#line 1946 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = !(yyvsp[0].vrbl)->data[i];
            }
	}
#line 7768 "y.tab.c"
    break;

  case 163: /* vexpr: '(' vexpr ')'  */
#line 1956 "pars.yacc"
        {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[-1].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = (yyvsp[-1].vrbl)->data[i];
            }
	}
#line 7782 "y.tab.c"
    break;

  case 164: /* vexpr: '-' vexpr  */
#line 1965 "pars.yacc"
                                 {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[0].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = - (yyvsp[0].vrbl)->data[i];
            }
	}
#line 7796 "y.tab.c"
    break;

  case 165: /* asgn: VAR_D '=' expr  */
#line 1979 "pars.yacc"
        {
	    *((yyvsp[-2].dptr)) = (yyvsp[0].dval);
	}
#line 7804 "y.tab.c"
    break;

  case 166: /* asgn: FITPARM '=' expr  */
#line 1983 "pars.yacc"
        {
	    nonl_parms[(yyvsp[-2].ival)].value = (yyvsp[0].dval);
	}
#line 7812 "y.tab.c"
    break;

  case 167: /* asgn: FITPMAX '=' expr  */
#line 1987 "pars.yacc"
        {
	    nonl_parms[(yyvsp[-2].ival)].max = (yyvsp[0].dval);
	}
#line 7820 "y.tab.c"
    break;

  case 168: /* asgn: FITPMIN '=' expr  */
#line 1991 "pars.yacc"
        {
	    nonl_parms[(yyvsp[-2].ival)].min = (yyvsp[0].dval);
	}
#line 7828 "y.tab.c"
    break;

  case 169: /* asgn: array indx '=' expr  */
#line 1995 "pars.yacc"
        {
	    if ((yyvsp[-2].ival) >= (yyvsp[-3].vrbl)->length) {
		yyerror("Access beyond array bounds");
		return 1;
            }
            (yyvsp[-3].vrbl)->data[(yyvsp[-2].ival)] = (yyvsp[0].dval);
	}
#line 7840 "y.tab.c"
    break;

  case 170: /* lside_array: array  */
#line 2006 "pars.yacc"
        {
            target tgt;
            switch ((yyvsp[0].vrbl)->type) {
            case GRARR_SET:
                if (find_set_bydata((yyvsp[0].vrbl)->data, &tgt) == RETURN_SUCCESS) {
                    vasgn_gno   = tgt.gno;
                    vasgn_setno = tgt.setno;
                } else {
                    errmsg("Internal error");
		    return 1;
                }
                break;
            case GRARR_VEC:
                vasgn_gno   = -1;
                vasgn_setno = -1;
                break;
            default:
                /* It can NOT be a tmp array on the left side! */
                errmsg("Internal error");
	        return 1;
            }
            (yyval.vrbl) = (yyvsp[0].vrbl);
        }
#line 7868 "y.tab.c"
    break;

  case 171: /* vasgn: lside_array '=' vexpr  */
#line 2033 "pars.yacc"
        {
	    int i;
	    if ((yyvsp[-2].vrbl)->length != (yyvsp[0].vrbl)->length) {
                errmsg("Left and right vectors are of different lengths");
                return 1;
            }
	    for (i = 0; i < (yyvsp[-2].vrbl)->length; i++) {
	        (yyvsp[-2].vrbl)->data[i] = (yyvsp[0].vrbl)->data[i];
	    }
	}
#line 7883 "y.tab.c"
    break;

  case 172: /* vasgn: lside_array '=' expr  */
#line 2044 "pars.yacc"
        {
	    int i;
	    for (i = 0; i < (yyvsp[-2].vrbl)->length; i++) {
	        (yyvsp[-2].vrbl)->data[i] = (yyvsp[0].dval);
	    }
	}
#line 7894 "y.tab.c"
    break;

  case 173: /* defines: DEFINE NEW_TOKEN  */
#line 2054 "pars.yacc"
        {
	    symtab_entry tmpkey;
            double *var;
            
            var = xmalloc(SIZEOF_DOUBLE);
            *var = 0.0;
            
	    tmpkey.s = (yyvsp[0].sval);
	    tmpkey.type = KEY_VAR;
	    tmpkey.data = (void *) var;
	    if (addto_symtab(tmpkey) != RETURN_SUCCESS) {
	        yyerror("Adding new symbol failed");
	    }

            xfree((yyvsp[0].sval));
        }
#line 7915 "y.tab.c"
    break;

  case 174: /* defines: DEFINE NEW_TOKEN '[' ']'  */
#line 2071 "pars.yacc"
        {
	    if (define_parser_arr((yyvsp[-2].sval)) == NULL) {
	        yyerror("Adding new symbol failed");
	    }

            xfree((yyvsp[-2].sval));
        }
#line 7927 "y.tab.c"
    break;

  case 175: /* defines: DEFINE NEW_TOKEN '[' nexpr ']'  */
#line 2079 "pars.yacc"
        {
	    grarr *var;
            if ((var = define_parser_arr((yyvsp[-3].sval))) == NULL) {
	        yyerror("Adding new symbol failed");
	    } else {
                realloc_vrbl(var, (yyvsp[-1].ival));
            }

            xfree((yyvsp[-3].sval));
        }
#line 7942 "y.tab.c"
    break;

  case 176: /* defines: DEFINE VAR_D  */
#line 2090 "pars.yacc"
        {
            yyerror("Keyword already exists");
        }
#line 7950 "y.tab.c"
    break;

  case 177: /* defines: DEFINE VEC_D  */
#line 2094 "pars.yacc"
        {
            yyerror("Keyword already exists");
        }
#line 7958 "y.tab.c"
    break;

  case 178: /* defines: CLEAR VAR_D  */
#line 2098 "pars.yacc"
        {
            undefine_parser_var((void *) (yyvsp[0].dptr));
            xfree((yyvsp[0].dptr));
        }
#line 7967 "y.tab.c"
    break;

  case 179: /* defines: CLEAR VEC_D  */
#line 2103 "pars.yacc"
        {
            realloc_vrbl((yyvsp[0].vrbl), 0);
            undefine_parser_var((void *) (yyvsp[0].vrbl));
            xfree((yyvsp[0].vrbl));
        }
#line 7977 "y.tab.c"
    break;

  case 180: /* defines: ALIAS sexpr sexpr  */
#line 2108 "pars.yacc"
                            {
	    int position;

	    lowtoupper((yyvsp[0].sval));
	    if ((position = findf(key, (yyvsp[0].sval))) >= 0) {
	        symtab_entry tmpkey;
		tmpkey.s = (yyvsp[-1].sval);
		tmpkey.type = key[position].type;
		tmpkey.data = key[position].data;
		if (addto_symtab(tmpkey) != RETURN_SUCCESS) {
		    yyerror("Keyword already exists");
		}
	    } else {
	        yyerror("Aliased keyword not found");
	    }
	    xfree((yyvsp[-1].sval));
	    xfree((yyvsp[0].sval));
	}
#line 8000 "y.tab.c"
    break;

  case 181: /* defines: ALIAS FORCE onoff  */
#line 2126 "pars.yacc"
                            {
	    alias_force = (yyvsp[0].ival);
	}
#line 8008 "y.tab.c"
    break;

  case 182: /* defines: USE sexpr TYPE proctype FROM sexpr  */
#line 2129 "pars.yacc"
                                             {
	    if (load_module((yyvsp[0].sval), (yyvsp[-4].sval), (yyvsp[-4].sval), (yyvsp[-2].ival)) != 0) {
	        yyerror("DL module load failed");
	    }
	    xfree((yyvsp[-4].sval));
	    xfree((yyvsp[0].sval));
	}
#line 8020 "y.tab.c"
    break;

  case 183: /* defines: USE sexpr TYPE proctype FROM sexpr ALIAS sexpr  */
#line 2136 "pars.yacc"
                                                         {
	    if (load_module((yyvsp[-2].sval), (yyvsp[-6].sval), (yyvsp[0].sval), (yyvsp[-4].ival)) != 0) {
	        yyerror("DL module load failed");
	    }
	    xfree((yyvsp[-6].sval));
	    xfree((yyvsp[-2].sval));
	    xfree((yyvsp[0].sval));
	}
#line 8033 "y.tab.c"
    break;

  case 184: /* regionset: REGNUM onoff  */
#line 2147 "pars.yacc"
                     {
	    rg[(yyvsp[-1].ival)].active = (yyvsp[0].ival);
	}
#line 8041 "y.tab.c"
    break;

  case 185: /* regionset: REGNUM TYPE regiontype  */
#line 2150 "pars.yacc"
                                 {
	    rg[(yyvsp[-2].ival)].type = (yyvsp[0].ival);
	}
#line 8049 "y.tab.c"
    break;

  case 186: /* regionset: REGNUM color_select  */
#line 2153 "pars.yacc"
                              {
	    rg[(yyvsp[-1].ival)].color = (yyvsp[0].ival);
	}
#line 8057 "y.tab.c"
    break;

  case 187: /* regionset: REGNUM lines_select  */
#line 2156 "pars.yacc"
                              {
	    rg[(yyvsp[-1].ival)].lines = (yyvsp[0].ival);
	}
#line 8065 "y.tab.c"
    break;

  case 188: /* regionset: REGNUM linew_select  */
#line 2159 "pars.yacc"
                              {
	    rg[(yyvsp[-1].ival)].linew = (yyvsp[0].dval);
	}
#line 8073 "y.tab.c"
    break;

  case 189: /* regionset: REGNUM LINE expr ',' expr ',' expr ',' expr  */
#line 2163 "pars.yacc"
        {
	    rg[(yyvsp[-8].ival)].x1 = (yyvsp[-6].dval);
	    rg[(yyvsp[-8].ival)].y1 = (yyvsp[-4].dval);
	    rg[(yyvsp[-8].ival)].x2 = (yyvsp[-2].dval);
	    rg[(yyvsp[-8].ival)].y2 = (yyvsp[0].dval);
	}
#line 8084 "y.tab.c"
    break;

  case 190: /* regionset: REGNUM XY expr ',' expr  */
#line 2170 "pars.yacc"
        {
	    rg[(yyvsp[-4].ival)].x = xrealloc(rg[(yyvsp[-4].ival)].x, (rg[(yyvsp[-4].ival)].n + 1) * SIZEOF_DOUBLE);
	    rg[(yyvsp[-4].ival)].y = xrealloc(rg[(yyvsp[-4].ival)].y, (rg[(yyvsp[-4].ival)].n + 1) * SIZEOF_DOUBLE);
	    rg[(yyvsp[-4].ival)].x[rg[(yyvsp[-4].ival)].n] = (yyvsp[-2].dval);
	    rg[(yyvsp[-4].ival)].y[rg[(yyvsp[-4].ival)].n] = (yyvsp[0].dval);
	    rg[(yyvsp[-4].ival)].n++;
	}
#line 8096 "y.tab.c"
    break;

  case 191: /* regionset: LINK REGNUM TO selectgraph  */
#line 2177 "pars.yacc"
                                     {
	    rg[(yyvsp[-2].ival)].linkto = (yyvsp[0].ival);
	}
#line 8104 "y.tab.c"
    break;

  case 192: /* parmset: VERSION nexpr  */
#line 2184 "pars.yacc"
                      {
            if (set_project_version((yyvsp[0].ival)) != RETURN_SUCCESS) {
                errmsg("Project version is newer than software!");
            }
            if (get_project_version() < 50001) {
                map_fonts(FONT_MAP_ACEGR);
            } else {
                map_fonts(FONT_MAP_DEFAULT);
            }
        }
#line 8119 "y.tab.c"
    break;

  case 193: /* parmset: PAGE RESIZE nexpr ',' nexpr  */
#line 2194 "pars.yacc"
                                      {
            set_page_dimensions((yyvsp[-2].ival), (yyvsp[0].ival), TRUE);
        }
#line 8127 "y.tab.c"
    break;

  case 194: /* parmset: PAGE SIZE nexpr ',' nexpr  */
#line 2197 "pars.yacc"
                                    {
            set_page_dimensions((yyvsp[-2].ival), (yyvsp[0].ival), FALSE);
        }
#line 8135 "y.tab.c"
    break;

  case 195: /* parmset: DEVICE sexpr PAGE SIZE nexpr ',' nexpr  */
#line 2200 "pars.yacc"
                                                 {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[-5].sval));
            xfree((yyvsp[-5].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.pg.width =  (long) ((yyvsp[-2].ival)*dev.pg.dpi/72);
                dev.pg.height = (long) ((yyvsp[0].ival)*dev.pg.dpi/72);
                set_device_props(device_id, dev);
            }
        }
#line 8155 "y.tab.c"
    break;

  case 196: /* parmset: DEVICE sexpr DPI expr  */
#line 2215 "pars.yacc"
                                {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[-2].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.pg.dpi = (yyvsp[0].dval);
                set_device_props(device_id, dev);
            }
            xfree((yyvsp[-2].sval));
        }
#line 8174 "y.tab.c"
    break;

  case 197: /* parmset: DEVICE sexpr FONTP ANTIALIASING onoff  */
#line 2229 "pars.yacc"
                                                {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[-3].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.fontaa = (yyvsp[0].ival);
                set_device_props(device_id, dev);
            }
            xfree((yyvsp[-3].sval));
        }
#line 8193 "y.tab.c"
    break;

  case 198: /* parmset: DEVICE sexpr FONTP onoff  */
#line 2243 "pars.yacc"
                                   {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[-2].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.devfonts = (yyvsp[0].ival);
                set_device_props(device_id, dev);
            }
            xfree((yyvsp[-2].sval));
        }
#line 8212 "y.tab.c"
    break;

  case 199: /* parmset: DEVICE sexpr OP sexpr  */
#line 2257 "pars.yacc"
                                {
            int device_id;
            
            device_id = get_device_by_name((yyvsp[-2].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                if (parse_device_options(device_id, (yyvsp[0].sval)) != 
                                                        RETURN_SUCCESS) {
                    yyerror("Incorrect device option string");
                }
            }
            xfree((yyvsp[-2].sval));
            xfree((yyvsp[0].sval));
        }
#line 8232 "y.tab.c"
    break;

  case 200: /* parmset: HARDCOPY DEVICE sexpr  */
#line 2272 "pars.yacc"
                                {
            set_printer_by_name((yyvsp[0].sval));
            xfree((yyvsp[0].sval));
        }
#line 8241 "y.tab.c"
    break;

  case 201: /* parmset: REFERENCE DATE jrawdate  */
#line 2276 "pars.yacc"
                                  {
            set_ref_date((yyvsp[0].dval));
	}
#line 8249 "y.tab.c"
    break;

  case 202: /* parmset: DATE WRAP onoff  */
#line 2279 "pars.yacc"
                          {
            allow_two_digits_years((yyvsp[0].ival));
	}
#line 8257 "y.tab.c"
    break;

  case 203: /* parmset: DATE WRAP YEAR iexpr  */
#line 2282 "pars.yacc"
                               {
            set_wrap_year((yyvsp[0].ival));
	}
#line 8265 "y.tab.c"
    break;

  case 204: /* parmset: BACKGROUND color_select  */
#line 2285 "pars.yacc"
                                  {
	    setbgcolor((yyvsp[0].ival));
	}
#line 8273 "y.tab.c"
    break;

  case 205: /* parmset: PAGE BACKGROUND FILL onoff  */
#line 2288 "pars.yacc"
                                     {
	    setbgfill((yyvsp[0].ival));
	}
#line 8281 "y.tab.c"
    break;

  case 206: /* parmset: PAGE SCROLL expr '%'  */
#line 2291 "pars.yacc"
                               {
	    scroll_proc((int) (yyvsp[-1].dval));
	}
#line 8289 "y.tab.c"
    break;

  case 207: /* parmset: PAGE INOUT expr '%'  */
#line 2294 "pars.yacc"
                              {
	    scrollinout_proc((int) (yyvsp[-1].dval));
	}
#line 8297 "y.tab.c"
    break;

  case 208: /* parmset: LINK PAGE onoff  */
#line 2297 "pars.yacc"
                          {
	    scrolling_islinked = (yyvsp[0].ival);
	}
#line 8305 "y.tab.c"
    break;

  case 209: /* parmset: STACK WORLD expr ',' expr ',' expr ',' expr  */
#line 2302 "pars.yacc"
        {
	    add_world(whichgraph, (yyvsp[-6].dval), (yyvsp[-4].dval), (yyvsp[-2].dval), (yyvsp[0].dval));
	}
#line 8313 "y.tab.c"
    break;

  case 210: /* parmset: TIMER nexpr  */
#line 2306 "pars.yacc"
                      {
            timer_delay = (yyvsp[0].ival);
	}
#line 8321 "y.tab.c"
    break;

  case 211: /* parmset: TARGET selectset  */
#line 2310 "pars.yacc"
                           {
	    target_set = *((yyvsp[0].trgt));
	    set_parser_setno(target_set.gno, target_set.setno);
	}
#line 8330 "y.tab.c"
    break;

  case 212: /* parmset: WITH selectgraph  */
#line 2314 "pars.yacc"
                           {
	    set_parser_gno((yyvsp[0].ival));
	}
#line 8338 "y.tab.c"
    break;

  case 213: /* parmset: WITH selectset  */
#line 2317 "pars.yacc"
                         {
	    set_parser_setno((yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno);
	}
#line 8346 "y.tab.c"
    break;

  case 214: /* parmset: selectset LINK sourcetype sexpr  */
#line 2322 "pars.yacc"
                                          {
	    set_hotlink((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, 1, (yyvsp[0].sval), (yyvsp[-1].ival));
	    xfree((yyvsp[0].sval));
	}
#line 8355 "y.tab.c"
    break;

  case 215: /* parmset: selectset LINK onoff  */
#line 2326 "pars.yacc"
                               {
	    set_hotlink((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].ival), NULL, 0);
	}
#line 8363 "y.tab.c"
    break;

  case 216: /* parmset: WITH BOX  */
#line 2331 "pars.yacc"
                   {
	    curbox = next_box();
	}
#line 8371 "y.tab.c"
    break;

  case 217: /* parmset: WITH BOX nexpr  */
#line 2334 "pars.yacc"
                         {
            int no = (yyvsp[0].ival);
            if (is_valid_box(no) ||
                realloc_boxes(no + 1) == RETURN_SUCCESS) {
                curbox = no;
            }
	}
#line 8383 "y.tab.c"
    break;

  case 218: /* parmset: BOX onoff  */
#line 2341 "pars.yacc"
                    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
	        boxes[curbox].active = (yyvsp[0].ival);
            }
	}
#line 8395 "y.tab.c"
    break;

  case 219: /* parmset: BOX selectgraph  */
#line 2348 "pars.yacc"
                          {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
	        boxes[curbox].gno = (yyvsp[0].ival);
            }
	}
#line 8407 "y.tab.c"
    break;

  case 220: /* parmset: BOX expr ',' expr ',' expr ',' expr  */
#line 2355 "pars.yacc"
                                              {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
		boxes[curbox].x1 = (yyvsp[-6].dval);
		boxes[curbox].y1 = (yyvsp[-4].dval);
		boxes[curbox].x2 = (yyvsp[-2].dval);
		boxes[curbox].y2 = (yyvsp[0].dval);
	    }
	}
#line 8422 "y.tab.c"
    break;

  case 221: /* parmset: BOX LOCTYPE worldview  */
#line 2365 "pars.yacc"
                                {
	    box_loctype = (yyvsp[0].ival);
	}
#line 8430 "y.tab.c"
    break;

  case 222: /* parmset: BOX lines_select  */
#line 2368 "pars.yacc"
                           {
	    box_lines = (yyvsp[0].ival);
	}
#line 8438 "y.tab.c"
    break;

  case 223: /* parmset: BOX linew_select  */
#line 2371 "pars.yacc"
                           {
	    box_linew = (yyvsp[0].dval);
	}
#line 8446 "y.tab.c"
    break;

  case 224: /* parmset: BOX color_select  */
#line 2374 "pars.yacc"
                           {
	    box_color = (yyvsp[0].ival);
	}
#line 8454 "y.tab.c"
    break;

  case 225: /* parmset: BOX FILL color_select  */
#line 2377 "pars.yacc"
                                {
	    box_fillcolor = (yyvsp[0].ival);
	}
#line 8462 "y.tab.c"
    break;

  case 226: /* parmset: BOX FILL pattern_select  */
#line 2380 "pars.yacc"
                                  {
	    box_fillpat = (yyvsp[0].ival);
	}
#line 8470 "y.tab.c"
    break;

  case 227: /* parmset: BOX DEF  */
#line 2383 "pars.yacc"
                  {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
		boxes[curbox].lines = box_lines;
		boxes[curbox].linew = box_linew;
		boxes[curbox].color = box_color;
		if (get_project_version() <= 40102) {
                    switch (filltype_obs) {
                    case COLOR:
                        boxes[curbox].fillcolor = box_fillcolor;
		        boxes[curbox].fillpattern = 1;
                        break;
                    case PATTERN:
                        boxes[curbox].fillcolor = 1;
		        boxes[curbox].fillpattern = box_fillpat;
                        break;
                    default: /* NONE */
                        boxes[curbox].fillcolor = box_fillcolor;
		        boxes[curbox].fillpattern = 0;
                        break;
                    }
		} else {
                    boxes[curbox].fillcolor = box_fillcolor;
		    boxes[curbox].fillpattern = box_fillpat;
                }
                boxes[curbox].loctype = box_loctype;
	    }
	}
#line 8504 "y.tab.c"
    break;

  case 228: /* parmset: WITH ELLIPSE  */
#line 2414 "pars.yacc"
                       {
		curellipse = next_ellipse();
	}
#line 8512 "y.tab.c"
    break;

  case 229: /* parmset: WITH ELLIPSE nexpr  */
#line 2417 "pars.yacc"
                             {
            int no = (yyvsp[0].ival);
            if (is_valid_ellipse(no) ||
                realloc_ellipses(no + 1) == RETURN_SUCCESS) {
                curellipse = no;
            }
	}
#line 8524 "y.tab.c"
    break;

  case 230: /* parmset: ELLIPSE onoff  */
#line 2424 "pars.yacc"
                        {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
	        ellip[curellipse].active = (yyvsp[0].ival);
            }
	}
#line 8536 "y.tab.c"
    break;

  case 231: /* parmset: ELLIPSE selectgraph  */
#line 2431 "pars.yacc"
                              {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
	        ellip[curellipse].gno = (yyvsp[0].ival);
            }
	}
#line 8548 "y.tab.c"
    break;

  case 232: /* parmset: ELLIPSE expr ',' expr ',' expr ',' expr  */
#line 2438 "pars.yacc"
                                                  {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
		ellip[curellipse].x1 = (yyvsp[-6].dval);
		ellip[curellipse].y1 = (yyvsp[-4].dval);
		ellip[curellipse].x2 = (yyvsp[-2].dval);
		ellip[curellipse].y2 = (yyvsp[0].dval);
	    }
	}
#line 8563 "y.tab.c"
    break;

  case 233: /* parmset: ELLIPSE LOCTYPE worldview  */
#line 2448 "pars.yacc"
                                    {
	    ellipse_loctype = (yyvsp[0].ival);
	}
#line 8571 "y.tab.c"
    break;

  case 234: /* parmset: ELLIPSE lines_select  */
#line 2451 "pars.yacc"
                               {
	    ellipse_lines = (yyvsp[0].ival);
	}
#line 8579 "y.tab.c"
    break;

  case 235: /* parmset: ELLIPSE linew_select  */
#line 2454 "pars.yacc"
                               {
	    ellipse_linew = (yyvsp[0].dval);
	}
#line 8587 "y.tab.c"
    break;

  case 236: /* parmset: ELLIPSE color_select  */
#line 2457 "pars.yacc"
                               {
	    ellipse_color = (yyvsp[0].ival);
	}
#line 8595 "y.tab.c"
    break;

  case 237: /* parmset: ELLIPSE FILL color_select  */
#line 2460 "pars.yacc"
                                    {
	    ellipse_fillcolor = (yyvsp[0].ival);
	}
#line 8603 "y.tab.c"
    break;

  case 238: /* parmset: ELLIPSE FILL pattern_select  */
#line 2463 "pars.yacc"
                                      {
	    ellipse_fillpat = (yyvsp[0].ival);
	}
#line 8611 "y.tab.c"
    break;

  case 239: /* parmset: ELLIPSE DEF  */
#line 2466 "pars.yacc"
                      {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
		ellip[curellipse].lines = ellipse_lines;
		ellip[curellipse].linew = ellipse_linew;
		ellip[curellipse].color = ellipse_color;
		if (get_project_version() <= 40102) {
                    switch (filltype_obs) {
                    case COLOR:
                        ellip[curellipse].fillcolor = ellipse_fillcolor;
		        ellip[curellipse].fillpattern = 1;
                        break;
                    case PATTERN:
                        ellip[curellipse].fillcolor = 1;
		        ellip[curellipse].fillpattern = ellipse_fillpat;
                        break;
                    default: /* NONE */
                        ellip[curellipse].fillcolor = ellipse_fillcolor;
		        ellip[curellipse].fillpattern = 0;
                        break;
                    }
		} else {
                    ellip[curellipse].fillcolor = ellipse_fillcolor;
		    ellip[curellipse].fillpattern = ellipse_fillpat;
                }
		ellip[curellipse].loctype = ellipse_loctype;
	    }
	}
#line 8645 "y.tab.c"
    break;

  case 240: /* parmset: WITH LINE  */
#line 2497 "pars.yacc"
                    {
	    curline = next_line();
	}
#line 8653 "y.tab.c"
    break;

  case 241: /* parmset: WITH LINE nexpr  */
#line 2500 "pars.yacc"
                          {
            int no = (yyvsp[0].ival);
            if (is_valid_line(no) ||
                realloc_lines(no + 1) == RETURN_SUCCESS) {
                curline = no;
            }
	}
#line 8665 "y.tab.c"
    break;

  case 242: /* parmset: LINE onoff  */
#line 2507 "pars.yacc"
                     {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].active = (yyvsp[0].ival);
            }
	}
#line 8677 "y.tab.c"
    break;

  case 243: /* parmset: LINE selectgraph  */
#line 2514 "pars.yacc"
                           {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].gno = (yyvsp[0].ival);
            }
	}
#line 8689 "y.tab.c"
    break;

  case 244: /* parmset: LINE expr ',' expr ',' expr ',' expr  */
#line 2521 "pars.yacc"
                                               {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].x1 = (yyvsp[-6].dval);
	        lines[curline].y1 = (yyvsp[-4].dval);
	        lines[curline].x2 = (yyvsp[-2].dval);
	        lines[curline].y2 = (yyvsp[0].dval);
            }
	}
#line 8704 "y.tab.c"
    break;

  case 245: /* parmset: LINE LOCTYPE worldview  */
#line 2531 "pars.yacc"
                                 {
	    line_loctype = (yyvsp[0].ival);
	}
#line 8712 "y.tab.c"
    break;

  case 246: /* parmset: LINE linew_select  */
#line 2534 "pars.yacc"
                            {
	    line_linew = (yyvsp[0].dval);
	}
#line 8720 "y.tab.c"
    break;

  case 247: /* parmset: LINE lines_select  */
#line 2537 "pars.yacc"
                            {
	    line_lines = (yyvsp[0].ival);
	}
#line 8728 "y.tab.c"
    break;

  case 248: /* parmset: LINE color_select  */
#line 2540 "pars.yacc"
                            {
	    line_color = (yyvsp[0].ival);
	}
#line 8736 "y.tab.c"
    break;

  case 249: /* parmset: LINE ARROW nexpr  */
#line 2543 "pars.yacc"
                           {
	    line_arrow_end = (yyvsp[0].ival);
	}
#line 8744 "y.tab.c"
    break;

  case 250: /* parmset: LINE ARROW LENGTH expr  */
#line 2546 "pars.yacc"
                                 {
	    line_asize = (yyvsp[0].dval);
	}
#line 8752 "y.tab.c"
    break;

  case 251: /* parmset: LINE ARROW TYPE nexpr  */
#line 2549 "pars.yacc"
                                {
	    line_atype = (yyvsp[0].ival);
	}
#line 8760 "y.tab.c"
    break;

  case 252: /* parmset: LINE ARROW LAYOUT expr ',' expr  */
#line 2552 "pars.yacc"
                                          {
	    line_a_dL_ff = (yyvsp[-2].dval);
	    line_a_lL_ff = (yyvsp[0].dval);
	}
#line 8769 "y.tab.c"
    break;

  case 253: /* parmset: LINE DEF  */
#line 2556 "pars.yacc"
                   {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].lines = line_lines;
	        lines[curline].linew = line_linew;
	        lines[curline].color = line_color;
	        lines[curline].arrow_end = line_arrow_end;
	        lines[curline].arrow.length = line_asize;
	        lines[curline].arrow.type = line_atype;
	        lines[curline].arrow.dL_ff = line_a_dL_ff;
	        lines[curline].arrow.lL_ff = line_a_lL_ff;
	        lines[curline].loctype = line_loctype;
            }
	}
#line 8789 "y.tab.c"
    break;

  case 254: /* parmset: WITH STRING  */
#line 2573 "pars.yacc"
                      {
            curstring = next_string();
        }
#line 8797 "y.tab.c"
    break;

  case 255: /* parmset: WITH STRING nexpr  */
#line 2576 "pars.yacc"
                            {
            int no = (yyvsp[0].ival);
            if (is_valid_string(no) ||
                realloc_strings(no + 1) == RETURN_SUCCESS) {
                curstring = no;
            }
        }
#line 8809 "y.tab.c"
    break;

  case 256: /* parmset: STRING onoff  */
#line 2583 "pars.yacc"
                       {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
                pstr[curstring].active = (yyvsp[0].ival);
            }
        }
#line 8821 "y.tab.c"
    break;

  case 257: /* parmset: STRING selectgraph  */
#line 2590 "pars.yacc"
                             {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
                pstr[curstring].gno = (yyvsp[0].ival);
            }
        }
#line 8833 "y.tab.c"
    break;

  case 258: /* parmset: STRING expr ',' expr  */
#line 2597 "pars.yacc"
                               {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
	        pstr[curstring].x = (yyvsp[-2].dval);
	        pstr[curstring].y = (yyvsp[0].dval);
            }
	}
#line 8846 "y.tab.c"
    break;

  case 259: /* parmset: STRING LOCTYPE worldview  */
#line 2605 "pars.yacc"
                                   {
            string_loctype = (yyvsp[0].ival);
        }
#line 8854 "y.tab.c"
    break;

  case 260: /* parmset: STRING color_select  */
#line 2608 "pars.yacc"
                              {
            string_color = (yyvsp[0].ival);
        }
#line 8862 "y.tab.c"
    break;

  case 261: /* parmset: STRING ROT nexpr  */
#line 2611 "pars.yacc"
                           {
            string_rot = (yyvsp[0].ival);
        }
#line 8870 "y.tab.c"
    break;

  case 262: /* parmset: STRING font_select  */
#line 2614 "pars.yacc"
                             {
            string_font = (yyvsp[0].ival);
        }
#line 8878 "y.tab.c"
    break;

  case 263: /* parmset: STRING JUST nexpr  */
#line 2617 "pars.yacc"
                            {
            string_just = (yyvsp[0].ival);
        }
#line 8886 "y.tab.c"
    break;

  case 264: /* parmset: STRING CHAR SIZE expr  */
#line 2620 "pars.yacc"
                                {
            string_size = (yyvsp[0].dval);
        }
#line 8894 "y.tab.c"
    break;

  case 265: /* parmset: STRING DEF sexpr  */
#line 2623 "pars.yacc"
                           {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
	        set_plotstr_string(&pstr[curstring], (yyvsp[0].sval));
	        pstr[curstring].color = string_color;
	        pstr[curstring].font = string_font;
	        pstr[curstring].just = string_just;
	        pstr[curstring].loctype = string_loctype;
	        pstr[curstring].rot = string_rot;
	        pstr[curstring].charsize = string_size;
            }
	    xfree((yyvsp[0].sval));
	}
#line 8913 "y.tab.c"
    break;

  case 266: /* parmset: TIMESTAMP onoff  */
#line 2639 "pars.yacc"
                          {
            timestamp.active = (yyvsp[0].ival);
        }
#line 8921 "y.tab.c"
    break;

  case 267: /* parmset: TIMESTAMP font_select  */
#line 2642 "pars.yacc"
                                {
            timestamp.font = (yyvsp[0].ival);
        }
#line 8929 "y.tab.c"
    break;

  case 268: /* parmset: TIMESTAMP CHAR SIZE expr  */
#line 2645 "pars.yacc"
                                   {
            timestamp.charsize = (yyvsp[0].dval);
        }
#line 8937 "y.tab.c"
    break;

  case 269: /* parmset: TIMESTAMP ROT nexpr  */
#line 2648 "pars.yacc"
                              {
            timestamp.rot = (yyvsp[0].ival);
        }
#line 8945 "y.tab.c"
    break;

  case 270: /* parmset: TIMESTAMP color_select  */
#line 2651 "pars.yacc"
                                 {
            timestamp.color = (yyvsp[0].ival);
        }
#line 8953 "y.tab.c"
    break;

  case 271: /* parmset: TIMESTAMP expr ',' expr  */
#line 2654 "pars.yacc"
                                  {
	    timestamp.x = (yyvsp[-2].dval);
	    timestamp.y = (yyvsp[0].dval);
	}
#line 8962 "y.tab.c"
    break;

  case 272: /* parmset: TIMESTAMP DEF sexpr  */
#line 2658 "pars.yacc"
                              {
	  set_plotstr_string(&timestamp, (yyvsp[0].sval));
	  xfree((yyvsp[0].sval));
	}
#line 8971 "y.tab.c"
    break;

  case 273: /* parmset: DEFAULT lines_select  */
#line 2664 "pars.yacc"
                               {
	    grdefaults.lines = (yyvsp[0].ival);
	    box_lines = ellipse_lines = line_lines = (yyvsp[0].ival);
	}
#line 8980 "y.tab.c"
    break;

  case 274: /* parmset: DEFAULT linew_select  */
#line 2668 "pars.yacc"
                               {
	    grdefaults.linew = (yyvsp[0].dval);
	    box_linew = ellipse_linew = line_linew = (yyvsp[0].dval);
	}
#line 8989 "y.tab.c"
    break;

  case 275: /* parmset: DEFAULT color_select  */
#line 2672 "pars.yacc"
                               {
	    grdefaults.color = (yyvsp[0].ival);
	    box_color = ellipse_color = line_color = string_color = (yyvsp[0].ival);
	}
#line 8998 "y.tab.c"
    break;

  case 276: /* parmset: DEFAULT pattern_select  */
#line 2676 "pars.yacc"
                                 {
	    grdefaults.pattern = (yyvsp[0].ival);
	}
#line 9006 "y.tab.c"
    break;

  case 277: /* parmset: DEFAULT CHAR SIZE expr  */
#line 2679 "pars.yacc"
                                 {
	    grdefaults.charsize = (yyvsp[0].dval);
	    string_size = (yyvsp[0].dval);
	}
#line 9015 "y.tab.c"
    break;

  case 278: /* parmset: DEFAULT font_select  */
#line 2683 "pars.yacc"
                              {
	    grdefaults.font = (yyvsp[0].ival);
	    string_font = (yyvsp[0].ival);
	}
#line 9024 "y.tab.c"
    break;

  case 279: /* parmset: DEFAULT SYMBOL SIZE expr  */
#line 2687 "pars.yacc"
                                   {
	    grdefaults.symsize = (yyvsp[0].dval);
	}
#line 9032 "y.tab.c"
    break;

  case 280: /* parmset: DEFAULT SFORMAT sexpr  */
#line 2690 "pars.yacc"
                                {
	    strcpy(sformat, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9041 "y.tab.c"
    break;

  case 281: /* parmset: MAP FONTP nexpr TO sexpr ',' sexpr  */
#line 2694 "pars.yacc"
                                             {
	    if ((map_font_by_name((yyvsp[-2].sval), (yyvsp[-4].ival)) != RETURN_SUCCESS) && 
                (map_font_by_name((yyvsp[0].sval), (yyvsp[-4].ival)) != RETURN_SUCCESS)) {
                errmsg("Failed mapping a font");
            }
            xfree((yyvsp[-2].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9054 "y.tab.c"
    break;

  case 282: /* parmset: MAP COLOR nexpr TO '(' nexpr ',' nexpr ',' nexpr ')' ',' sexpr  */
#line 2702 "pars.yacc"
                                                                         {
	    CMap_entry cmap;
            cmap.rgb.red   = (yyvsp[-7].ival);
            cmap.rgb.green = (yyvsp[-5].ival);
            cmap.rgb.blue  = (yyvsp[-3].ival);
            cmap.ctype = COLOR_MAIN;
            cmap.cname = (yyvsp[0].sval);
            if (store_color((yyvsp[-10].ival), cmap) == RETURN_FAILURE) {
                errmsg("Failed mapping a color");
            }
	    xfree((yyvsp[0].sval));
        }
#line 9071 "y.tab.c"
    break;

  case 283: /* parmset: WORLD expr ',' expr ',' expr ',' expr  */
#line 2715 "pars.yacc"
                                                {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg1 = (yyvsp[-6].dval);
	    g[whichgraph].w.yg1 = (yyvsp[-4].dval);
	    g[whichgraph].w.xg2 = (yyvsp[-2].dval);
	    g[whichgraph].w.yg2 = (yyvsp[0].dval);
	}
#line 9086 "y.tab.c"
    break;

  case 284: /* parmset: ZNORM expr  */
#line 2725 "pars.yacc"
                     {
	    set_graph_znorm(whichgraph, (yyvsp[0].dval));
	}
#line 9094 "y.tab.c"
    break;

  case 285: /* parmset: VIEW expr ',' expr ',' expr ',' expr  */
#line 2728 "pars.yacc"
                                               {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv1 = (yyvsp[-6].dval);
	    g[whichgraph].v.yv1 = (yyvsp[-4].dval);
	    g[whichgraph].v.xv2 = (yyvsp[-2].dval);
	    g[whichgraph].v.yv2 = (yyvsp[0].dval);
	}
#line 9109 "y.tab.c"
    break;

  case 286: /* parmset: TITLE sexpr  */
#line 2738 "pars.yacc"
                      {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].labs.title, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9122 "y.tab.c"
    break;

  case 287: /* parmset: TITLE font_select  */
#line 2746 "pars.yacc"
                            {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.font = (yyvsp[0].ival);
	}
#line 9134 "y.tab.c"
    break;

  case 288: /* parmset: TITLE SIZE expr  */
#line 2753 "pars.yacc"
                          {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.charsize = (yyvsp[0].dval);
	}
#line 9146 "y.tab.c"
    break;

  case 289: /* parmset: TITLE color_select  */
#line 2760 "pars.yacc"
                             {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.color = (yyvsp[0].ival);
	}
#line 9158 "y.tab.c"
    break;

  case 290: /* parmset: SUBTITLE sexpr  */
#line 2767 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].labs.stitle, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9171 "y.tab.c"
    break;

  case 291: /* parmset: SUBTITLE font_select  */
#line 2775 "pars.yacc"
                               {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.font = (yyvsp[0].ival);
	}
#line 9183 "y.tab.c"
    break;

  case 292: /* parmset: SUBTITLE SIZE expr  */
#line 2782 "pars.yacc"
                             {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.charsize = (yyvsp[0].dval);
	}
#line 9195 "y.tab.c"
    break;

  case 293: /* parmset: SUBTITLE color_select  */
#line 2789 "pars.yacc"
                                {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.color = (yyvsp[0].ival);
	}
#line 9207 "y.tab.c"
    break;

  case 294: /* parmset: XAXES SCALE scaletype  */
#line 2797 "pars.yacc"
                                {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].xscale = (yyvsp[0].ival);
	}
#line 9219 "y.tab.c"
    break;

  case 295: /* parmset: YAXES SCALE scaletype  */
#line 2804 "pars.yacc"
                                {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].yscale = (yyvsp[0].ival);
	}
#line 9231 "y.tab.c"
    break;

  case 296: /* parmset: XAXES INVERT onoff  */
#line 2811 "pars.yacc"
                             {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].xinvert = (yyvsp[0].ival);
	}
#line 9243 "y.tab.c"
    break;

  case 297: /* parmset: YAXES INVERT onoff  */
#line 2818 "pars.yacc"
                             {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].yinvert = (yyvsp[0].ival);
	}
#line 9255 "y.tab.c"
    break;

  case 298: /* parmset: AUTOSCALE ONREAD NONE  */
#line 2825 "pars.yacc"
                                {
            autoscale_onread = AUTOSCALE_NONE;
        }
#line 9263 "y.tab.c"
    break;

  case 299: /* parmset: AUTOSCALE ONREAD XAXES  */
#line 2828 "pars.yacc"
                                 {
            autoscale_onread = AUTOSCALE_X;
        }
#line 9271 "y.tab.c"
    break;

  case 300: /* parmset: AUTOSCALE ONREAD YAXES  */
#line 2831 "pars.yacc"
                                 {
            autoscale_onread = AUTOSCALE_Y;
        }
#line 9279 "y.tab.c"
    break;

  case 301: /* parmset: AUTOSCALE ONREAD XYAXES  */
#line 2834 "pars.yacc"
                                  {
            autoscale_onread = AUTOSCALE_XY;
        }
#line 9287 "y.tab.c"
    break;

  case 302: /* parmset: DESCRIPTION sexpr  */
#line 2838 "pars.yacc"
                            {
            char *s;
            s = copy_string(NULL, get_project_description());
            s = concat_strings(s, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
            s = concat_strings(s, "\n");
            set_project_description(s);
            xfree(s);
	}
#line 9301 "y.tab.c"
    break;

  case 303: /* parmset: CLEAR DESCRIPTION  */
#line 2847 "pars.yacc"
                            {
            set_project_description(NULL);
        }
#line 9309 "y.tab.c"
    break;

  case 304: /* parmset: LEGEND onoff  */
#line 2851 "pars.yacc"
                       {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.active = (yyvsp[0].ival);
	}
#line 9321 "y.tab.c"
    break;

  case 305: /* parmset: LEGEND LOCTYPE worldview  */
#line 2858 "pars.yacc"
                                   {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.loctype = (yyvsp[0].ival);
	}
#line 9333 "y.tab.c"
    break;

  case 306: /* parmset: LEGEND VGAP nexpr  */
#line 2865 "pars.yacc"
                            {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].l.vgap = (yyvsp[0].ival);
	}
#line 9345 "y.tab.c"
    break;

  case 307: /* parmset: LEGEND HGAP nexpr  */
#line 2872 "pars.yacc"
                            {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.hgap = (yyvsp[0].ival);
	}
#line 9357 "y.tab.c"
    break;

  case 308: /* parmset: LEGEND LENGTH nexpr  */
#line 2879 "pars.yacc"
                              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.len = (yyvsp[0].ival);
	}
#line 9369 "y.tab.c"
    break;

  case 309: /* parmset: LEGEND INVERT onoff  */
#line 2886 "pars.yacc"
                              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.invert = (yyvsp[0].ival);
        }
#line 9381 "y.tab.c"
    break;

  case 310: /* parmset: LEGEND BOX FILL color_select  */
#line 2893 "pars.yacc"
                                       {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxfillpen.color = (yyvsp[0].ival);
        }
#line 9393 "y.tab.c"
    break;

  case 311: /* parmset: LEGEND BOX FILL pattern_select  */
#line 2900 "pars.yacc"
                                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxfillpen.pattern = (yyvsp[0].ival);
        }
#line 9405 "y.tab.c"
    break;

  case 312: /* parmset: LEGEND BOX color_select  */
#line 2907 "pars.yacc"
                                  {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxpen.color = (yyvsp[0].ival);
	}
#line 9417 "y.tab.c"
    break;

  case 313: /* parmset: LEGEND BOX pattern_select  */
#line 2914 "pars.yacc"
                                    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxpen.pattern = (yyvsp[0].ival);
	}
#line 9429 "y.tab.c"
    break;

  case 314: /* parmset: LEGEND BOX lines_select  */
#line 2921 "pars.yacc"
                                  {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxlines = (yyvsp[0].ival);
	}
#line 9441 "y.tab.c"
    break;

  case 315: /* parmset: LEGEND BOX linew_select  */
#line 2928 "pars.yacc"
                                  {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxlinew = (yyvsp[0].dval);
	}
#line 9453 "y.tab.c"
    break;

  case 316: /* parmset: LEGEND expr ',' expr  */
#line 2935 "pars.yacc"
                               {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legx = (yyvsp[-2].dval);
	    g[whichgraph].l.legy = (yyvsp[0].dval);
	}
#line 9466 "y.tab.c"
    break;

  case 317: /* parmset: LEGEND CHAR SIZE expr  */
#line 2943 "pars.yacc"
                                {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.charsize = (yyvsp[0].dval);
	}
#line 9478 "y.tab.c"
    break;

  case 318: /* parmset: LEGEND font_select  */
#line 2950 "pars.yacc"
                             {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.font = (yyvsp[0].ival);
	}
#line 9490 "y.tab.c"
    break;

  case 319: /* parmset: LEGEND color_select  */
#line 2957 "pars.yacc"
                              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.color = (yyvsp[0].ival);
	}
#line 9502 "y.tab.c"
    break;

  case 320: /* parmset: FRAMEP onoff  */
#line 2965 "pars.yacc"
                       {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.pen.pattern = (yyvsp[0].ival);
	}
#line 9514 "y.tab.c"
    break;

  case 321: /* parmset: FRAMEP TYPE nexpr  */
#line 2972 "pars.yacc"
                            {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.type = (yyvsp[0].ival);
	}
#line 9526 "y.tab.c"
    break;

  case 322: /* parmset: FRAMEP lines_select  */
#line 2979 "pars.yacc"
                              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.lines = (yyvsp[0].ival);
	}
#line 9538 "y.tab.c"
    break;

  case 323: /* parmset: FRAMEP linew_select  */
#line 2986 "pars.yacc"
                              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.linew = (yyvsp[0].dval);
	}
#line 9550 "y.tab.c"
    break;

  case 324: /* parmset: FRAMEP color_select  */
#line 2993 "pars.yacc"
                              {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.pen.color = (yyvsp[0].ival);
	}
#line 9562 "y.tab.c"
    break;

  case 325: /* parmset: FRAMEP pattern_select  */
#line 3000 "pars.yacc"
                                {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.pen.pattern = (yyvsp[0].ival);
	}
#line 9574 "y.tab.c"
    break;

  case 326: /* parmset: FRAMEP BACKGROUND color_select  */
#line 3008 "pars.yacc"
        { 
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.color = (yyvsp[0].ival);
        }
#line 9586 "y.tab.c"
    break;

  case 327: /* parmset: FRAMEP BACKGROUND pattern_select  */
#line 3016 "pars.yacc"
        {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.pattern = (yyvsp[0].ival);
        }
#line 9598 "y.tab.c"
    break;

  case 328: /* parmset: selectgraph onoff  */
#line 3024 "pars.yacc"
                            {
            set_graph_hidden((yyvsp[-1].ival), !(yyvsp[0].ival));
        }
#line 9606 "y.tab.c"
    break;

  case 329: /* parmset: selectgraph HIDDEN onoff  */
#line 3027 "pars.yacc"
                                   {
            set_graph_hidden((yyvsp[-2].ival), (yyvsp[0].ival));
        }
#line 9614 "y.tab.c"
    break;

  case 330: /* parmset: selectgraph TYPE graphtype  */
#line 3030 "pars.yacc"
                                     {
            set_graph_type((yyvsp[-2].ival), (yyvsp[0].ival));
        }
#line 9622 "y.tab.c"
    break;

  case 331: /* parmset: selectgraph STACKED onoff  */
#line 3033 "pars.yacc"
                                    {
            set_graph_stacked((yyvsp[-2].ival), (yyvsp[0].ival));
        }
#line 9630 "y.tab.c"
    break;

  case 332: /* parmset: selectgraph BAR HGAP expr  */
#line 3037 "pars.yacc"
                                    {
	    set_graph_bargap((yyvsp[-3].ival), (yyvsp[0].dval));
	}
#line 9638 "y.tab.c"
    break;

  case 333: /* parmset: selectgraph FIXEDPOINT onoff  */
#line 3041 "pars.yacc"
                                       {
            g[(yyvsp[-2].ival)].locator.pointset = (yyvsp[0].ival);
        }
#line 9646 "y.tab.c"
    break;

  case 334: /* parmset: selectgraph FIXEDPOINT FORMAT formatchoice formatchoice  */
#line 3044 "pars.yacc"
                                                                  {
	    g[(yyvsp[-4].ival)].locator.fx = (yyvsp[-1].ival);
	    g[(yyvsp[-4].ival)].locator.fy = (yyvsp[0].ival);
	}
#line 9655 "y.tab.c"
    break;

  case 335: /* parmset: selectgraph FIXEDPOINT PREC expr ',' expr  */
#line 3048 "pars.yacc"
                                                    {
	    g[(yyvsp[-5].ival)].locator.px = (yyvsp[-2].dval);
	    g[(yyvsp[-5].ival)].locator.py = (yyvsp[0].dval);
	}
#line 9664 "y.tab.c"
    break;

  case 336: /* parmset: selectgraph FIXEDPOINT XY expr ',' expr  */
#line 3052 "pars.yacc"
                                                  {
	    g[(yyvsp[-5].ival)].locator.dsx = (yyvsp[-2].dval);
	    g[(yyvsp[-5].ival)].locator.dsy = (yyvsp[0].dval);
	}
#line 9673 "y.tab.c"
    break;

  case 337: /* parmset: selectgraph FIXEDPOINT TYPE nexpr  */
#line 3056 "pars.yacc"
                                            {
            g[(yyvsp[-3].ival)].locator.pt_type = (yyvsp[0].ival);
        }
#line 9681 "y.tab.c"
    break;

  case 338: /* parmset: TYPE xytype  */
#line 3060 "pars.yacc"
                      {
	    curtype = (yyvsp[0].ival);
	}
#line 9689 "y.tab.c"
    break;

  case 339: /* parmset: DEFINE filtertype sexpr filtermethod sexpr  */
#line 3065 "pars.yacc"
                                                     {
	    if (add_io_filter((yyvsp[-3].ival), (yyvsp[-1].ival), (yyvsp[0].sval), (yyvsp[-2].sval)) != 0) {
	        yyerror("Failed adding i/o filter");
	    }
	    xfree((yyvsp[-2].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9701 "y.tab.c"
    break;

  case 340: /* parmset: CLEAR filtertype  */
#line 3072 "pars.yacc"
                           {
	    clear_io_filters((yyvsp[0].ival));
	}
#line 9709 "y.tab.c"
    break;

  case 341: /* parmset: SOURCE sourcetype  */
#line 3076 "pars.yacc"
                            {
	    cursource = (yyvsp[0].ival);
	}
#line 9717 "y.tab.c"
    break;

  case 342: /* parmset: FORMAT formatchoice  */
#line 3079 "pars.yacc"
                              {
	    readxformat = (yyvsp[0].ival);
	}
#line 9725 "y.tab.c"
    break;

  case 343: /* parmset: FIT nonlfitopts  */
#line 3082 "pars.yacc"
                          { }
#line 9731 "y.tab.c"
    break;

  case 344: /* parmset: FITPARM CONSTRAINTS onoff  */
#line 3083 "pars.yacc"
                                    {
	    nonl_parms[(yyvsp[-2].ival)].constr = (yyvsp[0].ival);
	}
#line 9739 "y.tab.c"
    break;

  case 345: /* actions: REDRAW  */
#line 3089 "pars.yacc"
               {
	    drawgraph();
	}
#line 9747 "y.tab.c"
    break;

  case 346: /* actions: UPDATEALL  */
#line 3092 "pars.yacc"
                    {
#ifndef NONE_GUI
            if (inwin) {
                update_all();
            }
#endif
        }
#line 9759 "y.tab.c"
    break;

  case 347: /* actions: CD sexpr  */
#line 3099 "pars.yacc"
                   {
	    set_workingdir((yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9768 "y.tab.c"
    break;

  case 348: /* actions: ECHO sexpr  */
#line 3103 "pars.yacc"
                     {
	    echomsg((yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9777 "y.tab.c"
    break;

  case 349: /* actions: ECHO expr  */
#line 3107 "pars.yacc"
                    {
	    char buf[32];
            set_locale_num(TRUE);
            sprintf(buf, "%g", (yyvsp[0].dval));
            set_locale_num(FALSE);
            echomsg(buf);
	}
#line 9789 "y.tab.c"
    break;

  case 350: /* actions: CLOSE  */
#line 3114 "pars.yacc"
                {
	    close_input = copy_string(close_input, "");
	}
#line 9797 "y.tab.c"
    break;

  case 351: /* actions: CLOSE sexpr  */
#line 3117 "pars.yacc"
                      {
	    close_input = copy_string(close_input, (yyvsp[0].sval));
	}
#line 9805 "y.tab.c"
    break;

  case 352: /* actions: EXIT  */
#line 3120 "pars.yacc"
               {
	    exit(0);
	}
#line 9813 "y.tab.c"
    break;

  case 353: /* actions: EXIT '(' iexpr ')'  */
#line 3123 "pars.yacc"
                             {
	    exit((yyvsp[-1].ival));
	}
#line 9821 "y.tab.c"
    break;

  case 354: /* actions: PRINT  */
#line 3126 "pars.yacc"
                {
	    if (!safe_mode) {
                do_hardcopy();
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	}
#line 9833 "y.tab.c"
    break;

  case 355: /* actions: PRINT TO DEVICE  */
#line 3133 "pars.yacc"
                          {
            set_ptofile(FALSE);
	}
#line 9841 "y.tab.c"
    break;

  case 356: /* actions: PRINT TO sexpr  */
#line 3136 "pars.yacc"
                         {
            set_ptofile(TRUE);
	    strcpy(print_file, (yyvsp[0].sval));
            xfree((yyvsp[0].sval));
	}
#line 9851 "y.tab.c"
    break;

  case 357: /* actions: PAGE direction  */
#line 3141 "pars.yacc"
                         {
	    switch ((yyvsp[0].ival)) {
	    case UP:
		graph_scroll(GSCROLL_UP);
		break;
	    case DOWN:
		graph_scroll(GSCROLL_DOWN);
		break;
	    case RIGHT:
		graph_scroll(GSCROLL_RIGHT);
		break;
	    case LEFT:
		graph_scroll(GSCROLL_LEFT);
		break;
	    case IN:
		graph_zoom(GZOOM_SHRINK);
		break;
	    case OUT:
		graph_zoom(GZOOM_EXPAND);
		break;
	    }
	}
#line 9878 "y.tab.c"
    break;

  case 358: /* actions: SLEEP expr  */
#line 3163 "pars.yacc"
                     {
	    if ((yyvsp[0].dval) > 0) {
	        msleep_wrap((unsigned int) (1000 * (yyvsp[0].dval)));
	    }
	}
#line 9888 "y.tab.c"
    break;

  case 359: /* actions: HELP sexpr  */
#line 3168 "pars.yacc"
                     {
#ifndef NONE_GUI
            if (inwin) {
                HelpCB((yyvsp[0].sval));
            }
            xfree((yyvsp[0].sval));
#endif
	}
#line 9901 "y.tab.c"
    break;

  case 360: /* actions: HELP  */
#line 3176 "pars.yacc"
               {
#ifndef NONE_GUI
            if (inwin) {
                HelpCB("doc/UsersGuide.html");
            }
#endif
	}
#line 9913 "y.tab.c"
    break;

  case 361: /* actions: GETP sexpr  */
#line 3183 "pars.yacc"
                     {
	    gotparams = TRUE;
	    strcpy(paramfile, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 9923 "y.tab.c"
    break;

  case 362: /* actions: PUTP sexpr  */
#line 3188 "pars.yacc"
                     {
	    if (!safe_mode) {
                FILE *pp = grace_openw((yyvsp[0].sval));
	        if (pp != NULL) {
	            putparms(whichgraph, pp, 0);
	            grace_close(pp);
	        }
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[0].sval));
	}
#line 9940 "y.tab.c"
    break;

  case 363: /* actions: selectset HIDDEN onoff  */
#line 3200 "pars.yacc"
                                 {
	    set_set_hidden((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].ival));
	}
#line 9948 "y.tab.c"
    break;

  case 364: /* actions: selectset LENGTH nexpr  */
#line 3203 "pars.yacc"
                                 {
	    setlength((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].ival));
	}
#line 9956 "y.tab.c"
    break;

  case 365: /* actions: VEC_D LENGTH nexpr  */
#line 3206 "pars.yacc"
                             {
	    realloc_vrbl((yyvsp[-2].vrbl), (yyvsp[0].ival));
	}
#line 9964 "y.tab.c"
    break;

  case 366: /* actions: selectset POINT expr ',' expr  */
#line 3209 "pars.yacc"
                                        {
	    add_point((yyvsp[-4].trgt)->gno, (yyvsp[-4].trgt)->setno, (yyvsp[-2].dval), (yyvsp[0].dval));
	}
#line 9972 "y.tab.c"
    break;

  case 367: /* actions: selectset DROP nexpr ',' nexpr  */
#line 3213 "pars.yacc"
                                         {
	    int start = (yyvsp[-2].ival) - index_shift;
	    int stop = (yyvsp[0].ival) - index_shift;
	    droppoints((yyvsp[-4].trgt)->gno, (yyvsp[-4].trgt)->setno, start, stop);
	}
#line 9982 "y.tab.c"
    break;

  case 368: /* actions: SORT selectset sorton sortdir  */
#line 3218 "pars.yacc"
                                        {
	    if (is_set_active((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno)) {
	        sortset((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[-1].ival), (yyvsp[0].ival) == ASCENDING ? 0 : 1);
	    }
	}
#line 9992 "y.tab.c"
    break;

  case 369: /* actions: COPY selectset TO selectset  */
#line 3223 "pars.yacc"
                                      {
	    do_copyset((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno);
	}
#line 10000 "y.tab.c"
    break;

  case 370: /* actions: APPEND selectset TO selectset  */
#line 3226 "pars.yacc"
                                        {
	    if ((yyvsp[-2].trgt)->gno != (yyvsp[0].trgt)->gno) {
                errmsg("Can't append sets from different graphs");
            } else {
                int sets[2];
	        sets[0] = (yyvsp[0].trgt)->setno;
	        sets[1] = (yyvsp[-2].trgt)->setno;
	        join_sets((yyvsp[-2].trgt)->gno, sets, 2);
            }
	}
#line 10015 "y.tab.c"
    break;

  case 371: /* actions: REVERSE selectset  */
#line 3236 "pars.yacc"
                            {
            reverse_set((yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno);
	}
#line 10023 "y.tab.c"
    break;

  case 372: /* actions: SPLIT selectset nexpr  */
#line 3239 "pars.yacc"
                                {
            do_splitsets((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno, (yyvsp[0].ival));
	}
#line 10031 "y.tab.c"
    break;

  case 373: /* actions: MOVE selectset TO selectset  */
#line 3242 "pars.yacc"
                                      {
	    do_moveset((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno);
	}
#line 10039 "y.tab.c"
    break;

  case 374: /* actions: SWAP selectset AND selectset  */
#line 3245 "pars.yacc"
                                       {
	    do_swapset((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno);
	}
#line 10047 "y.tab.c"
    break;

  case 375: /* actions: KILL selectset  */
#line 3248 "pars.yacc"
                         {
	    killset((yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno);
	}
#line 10055 "y.tab.c"
    break;

  case 376: /* actions: KILL selectset SAVEALL  */
#line 3251 "pars.yacc"
                                 {
            killsetdata((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno);
            setcomment((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno, "");
        }
#line 10064 "y.tab.c"
    break;

  case 377: /* actions: KILL selectgraph  */
#line 3255 "pars.yacc"
                           {
            kill_graph((yyvsp[0].ival));
        }
#line 10072 "y.tab.c"
    break;

  case 378: /* actions: KILL REGNUM  */
#line 3258 "pars.yacc"
                      {
            kill_region((yyvsp[0].ival));
        }
#line 10080 "y.tab.c"
    break;

  case 379: /* actions: FLUSH  */
#line 3261 "pars.yacc"
                {
            wipeout();
        }
#line 10088 "y.tab.c"
    break;

  case 380: /* actions: ARRANGE '(' nexpr ',' nexpr ',' expr ',' expr ',' expr ')'  */
#line 3264 "pars.yacc"
                                                                     {
            arrange_graphs_simple((yyvsp[-9].ival), (yyvsp[-7].ival), 0, FALSE, (yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval));
        }
#line 10096 "y.tab.c"
    break;

  case 381: /* actions: ARRANGE '(' nexpr ',' nexpr ',' expr ',' expr ',' expr ',' onoff ',' onoff ',' onoff ')'  */
#line 3267 "pars.yacc"
                                                                                                   {
            int order = ((yyvsp[-5].ival) * GA_ORDER_HV_INV) |
                        ((yyvsp[-3].ival) * GA_ORDER_H_INV ) |
                        ((yyvsp[-1].ival) * GA_ORDER_V_INV );
            arrange_graphs_simple((yyvsp[-15].ival), (yyvsp[-13].ival), order, FALSE, (yyvsp[-11].dval), (yyvsp[-9].dval), (yyvsp[-7].dval));
        }
#line 10107 "y.tab.c"
    break;

  case 382: /* actions: ARRANGE '(' nexpr ',' nexpr ',' expr ',' expr ',' expr ',' onoff ',' onoff ',' onoff ',' onoff ')'  */
#line 3273 "pars.yacc"
                                                                                                             {
            int order = ((yyvsp[-7].ival) * GA_ORDER_HV_INV) |
                        ((yyvsp[-5].ival) * GA_ORDER_H_INV ) |
                        ((yyvsp[-3].ival) * GA_ORDER_V_INV );
            arrange_graphs_simple((yyvsp[-17].ival), (yyvsp[-15].ival), order, (yyvsp[-1].ival), (yyvsp[-13].dval), (yyvsp[-11].dval), (yyvsp[-9].dval));
        }
#line 10118 "y.tab.c"
    break;

  case 383: /* actions: NONLFIT '(' selectset ',' nexpr ')'  */
#line 3279 "pars.yacc"
                                              {
	    gotnlfit = TRUE;
	    nlfit_gno = (yyvsp[-3].trgt)->gno;
	    nlfit_setno = (yyvsp[-3].trgt)->setno;
	    nlfit_nsteps = (yyvsp[-1].ival);
	    nlfit_warray = NULL;
	}
#line 10130 "y.tab.c"
    break;

  case 384: /* actions: NONLFIT '(' selectset ',' vexpr ',' nexpr ')'  */
#line 3286 "pars.yacc"
                                                        {
	    if (getsetlength((yyvsp[-5].trgt)->gno, (yyvsp[-5].trgt)->setno) != (yyvsp[-3].vrbl)->length) {
                errmsg("Data and weight arrays are of different lengths");
                return 1;
            } else {
	        gotnlfit = TRUE;
	        nlfit_gno = (yyvsp[-5].trgt)->gno;
	        nlfit_setno = (yyvsp[-5].trgt)->setno;
	        nlfit_nsteps = (yyvsp[-1].ival);
	        nlfit_warray = copy_data_column((yyvsp[-3].vrbl)->data, (yyvsp[-3].vrbl)->length);
            }
	}
#line 10147 "y.tab.c"
    break;

  case 385: /* actions: REGRESS '(' selectset ',' nexpr ')'  */
#line 3298 "pars.yacc"
                                              {
	    do_regress((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, (yyvsp[-1].ival), 0, -1, 0, -1);
	}
#line 10155 "y.tab.c"
    break;

  case 386: /* actions: runtype '(' selectset ',' nexpr ')'  */
#line 3301 "pars.yacc"
                                              {
	    do_runavg((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, (yyvsp[-1].ival), (yyvsp[-5].ival), -1, 0);
	}
#line 10163 "y.tab.c"
    break;

  case 387: /* actions: ffttype '(' selectset ',' nexpr ')'  */
#line 3304 "pars.yacc"
                                              {
	    do_fourier_command((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, (yyvsp[-5].ival), (yyvsp[-1].ival));
	}
#line 10171 "y.tab.c"
    break;

  case 388: /* actions: ffttype '(' selectset ',' fourierdata ',' windowtype ',' fourierloadx ',' fourierloady ')'  */
#line 3308 "pars.yacc"
                                                         {
	    switch ((yyvsp[-11].ival)) {
	    case FFT_DFT:
                do_fourier((yyvsp[-9].trgt)->gno, (yyvsp[-9].trgt)->setno, 0, (yyvsp[-1].ival), (yyvsp[-3].ival), 0, (yyvsp[-7].ival), (yyvsp[-5].ival));
	        break;
	    case FFT_INVDFT    :
                do_fourier((yyvsp[-9].trgt)->gno, (yyvsp[-9].trgt)->setno, 0, (yyvsp[-1].ival), (yyvsp[-3].ival), 1, (yyvsp[-7].ival), (yyvsp[-5].ival));
	        break;
	    case FFT_FFT:
                do_fourier((yyvsp[-9].trgt)->gno, (yyvsp[-9].trgt)->setno, 1, (yyvsp[-1].ival), (yyvsp[-3].ival), 0, (yyvsp[-7].ival), (yyvsp[-5].ival));
	        break;
	    case FFT_INVFFT    :
                do_fourier((yyvsp[-9].trgt)->gno, (yyvsp[-9].trgt)->setno, 1, (yyvsp[-1].ival), (yyvsp[-3].ival), 1, (yyvsp[-7].ival), (yyvsp[-5].ival));
	        break;
	    default:
                errmsg("Internal error");
	        break;
	    }
        }
#line 10195 "y.tab.c"
    break;

  case 389: /* actions: INTERPOLATE '(' selectset ',' vexpr ',' interpmethod ',' onoff ')'  */
#line 3327 "pars.yacc"
                                                                             {
            do_interp((yyvsp[-7].trgt)->gno, (yyvsp[-7].trgt)->setno, get_cg(), SET_SELECT_NEXT,
                (yyvsp[-5].vrbl)->data, (yyvsp[-5].vrbl)->length, (yyvsp[-3].ival), (yyvsp[-1].ival));
	}
#line 10204 "y.tab.c"
    break;

  case 390: /* actions: HISTOGRAM '(' selectset ',' vexpr ',' onoff ',' onoff ')'  */
#line 3331 "pars.yacc"
                                                                    {
            do_histo((yyvsp[-7].trgt)->gno, (yyvsp[-7].trgt)->setno, get_cg(), SET_SELECT_NEXT,
                (yyvsp[-5].vrbl)->data, (yyvsp[-5].vrbl)->length - 1, (yyvsp[-3].ival), (yyvsp[-1].ival));
	}
#line 10213 "y.tab.c"
    break;

  case 391: /* actions: DIFFERENCE '(' selectset ',' nexpr ')'  */
#line 3335 "pars.yacc"
                                                 {
	    do_differ((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, (yyvsp[-1].ival));
	}
#line 10221 "y.tab.c"
    break;

  case 392: /* actions: INTEGRATE '(' selectset ')'  */
#line 3338 "pars.yacc"
                                      {
	    do_int((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno, 0);
	}
#line 10229 "y.tab.c"
    break;

  case 393: /* actions: XCOR '(' selectset ',' selectset ',' nexpr ',' onoff ')'  */
#line 3341 "pars.yacc"
                                                                   {
	    do_xcor((yyvsp[-7].trgt)->gno, (yyvsp[-7].trgt)->setno, (yyvsp[-5].trgt)->gno, (yyvsp[-5].trgt)->setno, (yyvsp[-3].ival), (yyvsp[-1].ival));
	}
#line 10237 "y.tab.c"
    break;

  case 394: /* actions: LINCONV '(' selectset ',' selectset ')'  */
#line 3344 "pars.yacc"
                                                  {
	    do_linearc((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, (yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno);
	}
#line 10245 "y.tab.c"
    break;

  case 395: /* actions: RESTRICT '(' selectset ',' vexpr ')'  */
#line 3347 "pars.yacc"
                                               {
            int len = getsetlength((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno);
            if (len != (yyvsp[-1].vrbl)->length) {
		errmsg("Filter expression is of a wrong length");
            } else {
                char *rarray;
                rarray = xmalloc(len*SIZEOF_CHAR);
                if (rarray) {
                    int i;
                    for (i = 0; i < len; i++) {
                        rarray[i] = CAST_DBL_TO_BOOL((yyvsp[-1].vrbl)->data[i]);
                    }
                    filter_set((yyvsp[-3].trgt)->gno, (yyvsp[-3].trgt)->setno, rarray);
                    xfree(rarray);
                }
            }
	}
#line 10267 "y.tab.c"
    break;

  case 396: /* actions: RESTRICT '(' selectset ',' REGNUM ',' onoff ')'  */
#line 3364 "pars.yacc"
                                                          {
            int rtype;
            char *rarray;
            
            rtype = RESTRICT_REG0 + (yyvsp[-3].ival);

	    if (get_restriction_array((yyvsp[-5].trgt)->gno, (yyvsp[-5].trgt)->setno,
                rtype, (yyvsp[-1].ival), &rarray) != RETURN_SUCCESS) {
                errmsg("Error in region evaluation");
                return 1;
	    } else {
                filter_set((yyvsp[-5].trgt)->gno, (yyvsp[-5].trgt)->setno, rarray);
                xfree(rarray);
            }
	}
#line 10287 "y.tab.c"
    break;

  case 397: /* actions: AUTOSCALE  */
#line 3379 "pars.yacc"
                    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_XY) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	}
#line 10297 "y.tab.c"
    break;

  case 398: /* actions: AUTOSCALE XAXES  */
#line 3384 "pars.yacc"
                          {
	    if (autoscale_graph(whichgraph, AUTOSCALE_X) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	}
#line 10307 "y.tab.c"
    break;

  case 399: /* actions: AUTOSCALE YAXES  */
#line 3389 "pars.yacc"
                          {
	    if (autoscale_graph(whichgraph, AUTOSCALE_Y) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	}
#line 10317 "y.tab.c"
    break;

  case 400: /* actions: AUTOSCALE selectset  */
#line 3394 "pars.yacc"
                              {
	    autoscale_byset((yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno, AUTOSCALE_XY);
	}
#line 10325 "y.tab.c"
    break;

  case 401: /* actions: AUTOTICKS  */
#line 3397 "pars.yacc"
                    {
            autotick_axis(whichgraph, ALL_AXES);
        }
#line 10333 "y.tab.c"
    break;

  case 402: /* actions: FOCUS selectgraph  */
#line 3400 "pars.yacc"
                            {
	    int gno = (yyvsp[0].ival);
            if (is_graph_hidden(gno) == FALSE) {
                select_graph(gno);
            } else {
		errmsg("Graph is not active");
            }
	}
#line 10346 "y.tab.c"
    break;

  case 403: /* actions: READ sexpr  */
#line 3408 "pars.yacc"
                     {
	    gotread = TRUE;
	    strcpy(readfile, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 10356 "y.tab.c"
    break;

  case 404: /* actions: READ BATCH sexpr  */
#line 3413 "pars.yacc"
                           {
	    strcpy(batchfile, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 10365 "y.tab.c"
    break;

  case 405: /* actions: READ BLOCK sexpr  */
#line 3417 "pars.yacc"
                           {
	    getdata(whichgraph, (yyvsp[0].sval), SOURCE_DISK, LOAD_BLOCK);
	    xfree((yyvsp[0].sval));
	}
#line 10374 "y.tab.c"
    break;

  case 406: /* actions: READ BLOCK sourcetype sexpr  */
#line 3421 "pars.yacc"
                                      {
	    getdata(whichgraph, (yyvsp[0].sval), (yyvsp[-1].ival), LOAD_BLOCK);
	    xfree((yyvsp[0].sval));
	}
#line 10383 "y.tab.c"
    break;

  case 407: /* actions: BLOCK xytype sexpr  */
#line 3425 "pars.yacc"
                             {
            int nc, *cols, scol;
            if (field_string_to_cols((yyvsp[0].sval), &nc, &cols, &scol) != RETURN_SUCCESS) {
                errmsg("Erroneous field specifications");
	        xfree((yyvsp[0].sval));
                return 1;
            } else {
	        xfree((yyvsp[0].sval));
	        create_set_fromblock(whichgraph, NEW_SET,
                    (yyvsp[-1].ival), nc, cols, scol, autoscale_onread);
                xfree(cols);
            }
	}
#line 10401 "y.tab.c"
    break;

  case 408: /* actions: KILL BLOCK  */
#line 3438 "pars.yacc"
                     {
	    set_blockdata(NULL);
	}
#line 10409 "y.tab.c"
    break;

  case 409: /* actions: READ xytype sexpr  */
#line 3441 "pars.yacc"
                            {
	    gotread = TRUE;
	    curtype = (yyvsp[-1].ival);
	    strcpy(readfile, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 10420 "y.tab.c"
    break;

  case 410: /* actions: READ xytype sourcetype sexpr  */
#line 3447 "pars.yacc"
                                       {
	    gotread = TRUE;
	    strcpy(readfile, (yyvsp[0].sval));
	    curtype = (yyvsp[-2].ival);
	    cursource = (yyvsp[-1].ival);
	    xfree((yyvsp[0].sval));
	}
#line 10432 "y.tab.c"
    break;

  case 411: /* actions: READ NXY sexpr  */
#line 3454 "pars.yacc"
                         {
	    getdata(whichgraph, (yyvsp[0].sval), SOURCE_DISK, LOAD_NXY);
	    xfree((yyvsp[0].sval));
	}
#line 10441 "y.tab.c"
    break;

  case 412: /* actions: READ NXY sourcetype sexpr  */
#line 3458 "pars.yacc"
                                    {
	    getdata(whichgraph, (yyvsp[0].sval), (yyvsp[-1].ival), LOAD_NXY);
	    xfree((yyvsp[0].sval));
	}
#line 10450 "y.tab.c"
    break;

  case 413: /* actions: WRITE selectset  */
#line 3462 "pars.yacc"
                          {
	    if (!safe_mode) {
                outputset((yyvsp[0].trgt)->gno, (yyvsp[0].trgt)->setno, "stdout", NULL);
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	}
#line 10462 "y.tab.c"
    break;

  case 414: /* actions: WRITE selectset FORMAT sexpr  */
#line 3469 "pars.yacc"
                                       {
	    if (!safe_mode) {
	        outputset((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, "stdout", (yyvsp[0].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[0].sval));
	}
#line 10475 "y.tab.c"
    break;

  case 415: /* actions: WRITE selectset FILEP sexpr  */
#line 3477 "pars.yacc"
                                      {
	    if (!safe_mode) {
	        outputset((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].sval), NULL);
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[0].sval));
	}
#line 10488 "y.tab.c"
    break;

  case 416: /* actions: WRITE selectset FILEP sexpr FORMAT sexpr  */
#line 3485 "pars.yacc"
                                                   {
	    if (!safe_mode) {
	        outputset((yyvsp[-4].trgt)->gno, (yyvsp[-4].trgt)->setno, (yyvsp[-2].sval), (yyvsp[0].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[-2].sval));
	    xfree((yyvsp[0].sval));
	}
#line 10502 "y.tab.c"
    break;

  case 417: /* actions: SAVEALL sexpr  */
#line 3494 "pars.yacc"
                        {
            if (!safe_mode) {
                save_project((yyvsp[0].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
            xfree((yyvsp[0].sval));
        }
#line 10515 "y.tab.c"
    break;

  case 418: /* actions: LOAD sexpr  */
#line 3502 "pars.yacc"
                     {
            load_project((yyvsp[0].sval));
            xfree((yyvsp[0].sval));
        }
#line 10524 "y.tab.c"
    break;

  case 419: /* actions: NEW  */
#line 3506 "pars.yacc"
              {
            new_project(NULL);
        }
#line 10532 "y.tab.c"
    break;

  case 420: /* actions: NEW FROM sexpr  */
#line 3509 "pars.yacc"
                         {
            new_project((yyvsp[0].sval));
            xfree((yyvsp[0].sval));
        }
#line 10541 "y.tab.c"
    break;

  case 421: /* actions: PUSH  */
#line 3513 "pars.yacc"
               {
	    push_world();
	}
#line 10549 "y.tab.c"
    break;

  case 422: /* actions: POP  */
#line 3516 "pars.yacc"
              {
	    pop_world();
	}
#line 10557 "y.tab.c"
    break;

  case 423: /* actions: CYCLE  */
#line 3519 "pars.yacc"
                {
	    cycle_world_stack();
	}
#line 10565 "y.tab.c"
    break;

  case 424: /* actions: STACK nexpr  */
#line 3522 "pars.yacc"
                      {
	    if ((yyvsp[0].ival) > 0)
		show_world_stack((yyvsp[0].ival) - 1);
	}
#line 10574 "y.tab.c"
    break;

  case 425: /* actions: CLEAR STACK  */
#line 3526 "pars.yacc"
                      {
	    clear_world_stack();
	}
#line 10582 "y.tab.c"
    break;

  case 426: /* actions: CLEAR BOX  */
#line 3529 "pars.yacc"
                    {
	    do_clear_boxes();
	}
#line 10590 "y.tab.c"
    break;

  case 427: /* actions: CLEAR ELLIPSE  */
#line 3532 "pars.yacc"
                        {
	    do_clear_ellipses();
	}
#line 10598 "y.tab.c"
    break;

  case 428: /* actions: CLEAR LINE  */
#line 3535 "pars.yacc"
                     {
	    do_clear_lines();
	}
#line 10606 "y.tab.c"
    break;

  case 429: /* actions: CLEAR STRING  */
#line 3538 "pars.yacc"
                       {
	    do_clear_text();
	}
#line 10614 "y.tab.c"
    break;

  case 430: /* options: PAGE LAYOUT pagelayout  */
#line 3545 "pars.yacc"
                               {
#ifndef NONE_GUI
            set_pagelayout((yyvsp[0].ival));
#endif
        }
#line 10624 "y.tab.c"
    break;

  case 431: /* options: AUTO REDRAW onoff  */
#line 3550 "pars.yacc"
                            {
	    auto_redraw = (yyvsp[0].ival);
	}
#line 10632 "y.tab.c"
    break;

  case 432: /* options: FOCUS onoff  */
#line 3553 "pars.yacc"
                      {
	    draw_focus_flag = (yyvsp[0].ival);
	}
#line 10640 "y.tab.c"
    break;

  case 433: /* options: FOCUS SET  */
#line 3556 "pars.yacc"
                    {
	    focus_policy = FOCUS_SET;
	}
#line 10648 "y.tab.c"
    break;

  case 434: /* options: FOCUS FOLLOWS  */
#line 3559 "pars.yacc"
                        {
	    focus_policy = FOCUS_FOLLOWS;
	}
#line 10656 "y.tab.c"
    break;

  case 435: /* options: FOCUS CLICK  */
#line 3562 "pars.yacc"
                      {
	    focus_policy = FOCUS_CLICK;
	}
#line 10664 "y.tab.c"
    break;

  case 436: /* set_setprop: setprop  */
#line 3569 "pars.yacc"
                {}
#line 10670 "y.tab.c"
    break;

  case 437: /* set_setprop: setprop_obs  */
#line 3570 "pars.yacc"
                      {}
#line 10676 "y.tab.c"
    break;

  case 438: /* setprop: selectset onoff  */
#line 3574 "pars.yacc"
                        {
	    set_set_hidden((yyvsp[-1].trgt)->gno, (yyvsp[-1].trgt)->setno, !(yyvsp[0].ival));
	}
#line 10684 "y.tab.c"
    break;

  case 439: /* setprop: selectset TYPE xytype  */
#line 3577 "pars.yacc"
                                {
	    set_dataset_type((yyvsp[-2].trgt)->gno, (yyvsp[-2].trgt)->setno, (yyvsp[0].ival));
	}
#line 10692 "y.tab.c"
    break;

  case 440: /* setprop: selectset SYMBOL nexpr  */
#line 3581 "pars.yacc"
                                 {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].sym = (yyvsp[0].ival);
	}
#line 10700 "y.tab.c"
    break;

  case 441: /* setprop: selectset SYMBOL color_select  */
#line 3584 "pars.yacc"
                                        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].sympen.color = (yyvsp[0].ival);
	}
#line 10708 "y.tab.c"
    break;

  case 442: /* setprop: selectset SYMBOL pattern_select  */
#line 3587 "pars.yacc"
                                          {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].sympen.pattern = (yyvsp[0].ival);
	}
#line 10716 "y.tab.c"
    break;

  case 443: /* setprop: selectset SYMBOL linew_select  */
#line 3590 "pars.yacc"
                                        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].symlinew = (yyvsp[0].dval);
	}
#line 10724 "y.tab.c"
    break;

  case 444: /* setprop: selectset SYMBOL lines_select  */
#line 3593 "pars.yacc"
                                        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].symlines = (yyvsp[0].ival);
	}
#line 10732 "y.tab.c"
    break;

  case 445: /* setprop: selectset SYMBOL FILL color_select  */
#line 3596 "pars.yacc"
                                             {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symfillpen.color = (yyvsp[0].ival);
	}
#line 10740 "y.tab.c"
    break;

  case 446: /* setprop: selectset SYMBOL FILL pattern_select  */
#line 3599 "pars.yacc"
                                               {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symfillpen.pattern = (yyvsp[0].ival);
	}
#line 10748 "y.tab.c"
    break;

  case 447: /* setprop: selectset SYMBOL SIZE expr  */
#line 3602 "pars.yacc"
                                     {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symsize = (yyvsp[0].dval);
	}
#line 10756 "y.tab.c"
    break;

  case 448: /* setprop: selectset SYMBOL CHAR nexpr  */
#line 3605 "pars.yacc"
                                      {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symchar = (yyvsp[0].ival);
	}
#line 10764 "y.tab.c"
    break;

  case 449: /* setprop: selectset SYMBOL CHAR font_select  */
#line 3608 "pars.yacc"
                                            {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].charfont = (yyvsp[0].ival);
	}
#line 10772 "y.tab.c"
    break;

  case 450: /* setprop: selectset SYMBOL SKIP nexpr  */
#line 3611 "pars.yacc"
                                      {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symskip = (yyvsp[0].ival);
	}
#line 10780 "y.tab.c"
    break;

  case 451: /* setprop: selectset LINE TYPE nexpr  */
#line 3616 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].linet = (yyvsp[0].ival);
	}
#line 10788 "y.tab.c"
    break;

  case 452: /* setprop: selectset LINE lines_select  */
#line 3620 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].lines = (yyvsp[0].ival);
	}
#line 10796 "y.tab.c"
    break;

  case 453: /* setprop: selectset LINE linew_select  */
#line 3624 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].linew = (yyvsp[0].dval);
	}
#line 10804 "y.tab.c"
    break;

  case 454: /* setprop: selectset LINE color_select  */
#line 3628 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].linepen.color = (yyvsp[0].ival);
	}
#line 10812 "y.tab.c"
    break;

  case 455: /* setprop: selectset LINE pattern_select  */
#line 3632 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].linepen.pattern = (yyvsp[0].ival);
	}
#line 10820 "y.tab.c"
    break;

  case 456: /* setprop: selectset FILL TYPE nexpr  */
#line 3637 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].filltype = (yyvsp[0].ival);
	}
#line 10828 "y.tab.c"
    break;

  case 457: /* setprop: selectset FILL RULE nexpr  */
#line 3641 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].fillrule = (yyvsp[0].ival);
	}
#line 10836 "y.tab.c"
    break;

  case 458: /* setprop: selectset FILL color_select  */
#line 3645 "pars.yacc"
        {
	    int prop = (yyvsp[0].ival);

	    if (get_project_version() <= 40102 && get_project_version() >= 30000) {
                switch (filltype_obs) {
                case COLOR:
                    break;
                case PATTERN:
                    prop = 1;
                    break;
                default: /* NONE */
	            prop = 0;
                    break;
                }
	    }
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].setfillpen.color = prop;
	}
#line 10858 "y.tab.c"
    break;

  case 459: /* setprop: selectset FILL pattern_select  */
#line 3663 "pars.yacc"
        {
	    int prop = (yyvsp[0].ival);

	    if (get_project_version() <= 40102) {
                switch (filltype_obs) {
                case COLOR:
                    prop = 1;
                    break;
                case PATTERN:
                    break;
                default: /* NONE */
	            prop = 0;
                    break;
                }
	    }
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].setfillpen.pattern = prop;
	}
#line 10880 "y.tab.c"
    break;

  case 460: /* setprop: selectset BASELINE onoff  */
#line 3683 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].baseline = (yyvsp[0].ival);
	}
#line 10888 "y.tab.c"
    break;

  case 461: /* setprop: selectset BASELINE TYPE nexpr  */
#line 3687 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].baseline_type = (yyvsp[0].ival);
	}
#line 10896 "y.tab.c"
    break;

  case 462: /* setprop: selectset DROPLINE onoff  */
#line 3692 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].dropline = (yyvsp[0].ival);
	}
#line 10904 "y.tab.c"
    break;

  case 463: /* setprop: selectset AVALUE onoff  */
#line 3697 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].avalue.active = (yyvsp[0].ival);
	}
#line 10912 "y.tab.c"
    break;

  case 464: /* setprop: selectset AVALUE TYPE nexpr  */
#line 3701 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].avalue.type = (yyvsp[0].ival);
	}
#line 10920 "y.tab.c"
    break;

  case 465: /* setprop: selectset AVALUE CHAR SIZE expr  */
#line 3705 "pars.yacc"
        {
	    g[(yyvsp[-4].trgt)->gno].p[(yyvsp[-4].trgt)->setno].avalue.size = (yyvsp[0].dval);
	}
#line 10928 "y.tab.c"
    break;

  case 466: /* setprop: selectset AVALUE font_select  */
#line 3709 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].avalue.font = (yyvsp[0].ival);
	}
#line 10936 "y.tab.c"
    break;

  case 467: /* setprop: selectset AVALUE color_select  */
#line 3713 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].avalue.color = (yyvsp[0].ival);
	}
#line 10944 "y.tab.c"
    break;

  case 468: /* setprop: selectset AVALUE ROT nexpr  */
#line 3717 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].avalue.angle = (yyvsp[0].ival);
	}
#line 10952 "y.tab.c"
    break;

  case 469: /* setprop: selectset AVALUE FORMAT formatchoice  */
#line 3721 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].avalue.format = (yyvsp[0].ival);
	}
#line 10960 "y.tab.c"
    break;

  case 470: /* setprop: selectset AVALUE PREC nexpr  */
#line 3725 "pars.yacc"
        {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].avalue.prec = (yyvsp[0].ival);
	}
#line 10968 "y.tab.c"
    break;

  case 471: /* setprop: selectset AVALUE OFFSET expr ',' expr  */
#line 3728 "pars.yacc"
                                                {
	    g[(yyvsp[-5].trgt)->gno].p[(yyvsp[-5].trgt)->setno].avalue.offset.x = (yyvsp[-2].dval);
	    g[(yyvsp[-5].trgt)->gno].p[(yyvsp[-5].trgt)->setno].avalue.offset.y = (yyvsp[0].dval);
	}
#line 10977 "y.tab.c"
    break;

  case 472: /* setprop: selectset AVALUE PREPEND sexpr  */
#line 3733 "pars.yacc"
        {
	    strcpy(g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].avalue.prestr, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 10986 "y.tab.c"
    break;

  case 473: /* setprop: selectset AVALUE APPEND sexpr  */
#line 3738 "pars.yacc"
        {
	    strcpy(g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].avalue.appstr, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 10995 "y.tab.c"
    break;

  case 474: /* setprop: selectset ERRORBAR onoff  */
#line 3743 "pars.yacc"
                                   {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].errbar.active = (yyvsp[0].ival);
	}
#line 11003 "y.tab.c"
    break;

  case 475: /* setprop: selectset ERRORBAR opchoice_sel  */
#line 3746 "pars.yacc"
                                          {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].errbar.ptype = (yyvsp[0].ival);
	}
#line 11011 "y.tab.c"
    break;

  case 476: /* setprop: selectset ERRORBAR color_select  */
#line 3749 "pars.yacc"
                                          {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].errbar.pen.color = (yyvsp[0].ival);
	}
#line 11019 "y.tab.c"
    break;

  case 477: /* setprop: selectset ERRORBAR pattern_select  */
#line 3752 "pars.yacc"
                                            {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].errbar.pen.pattern = (yyvsp[0].ival);
	}
#line 11027 "y.tab.c"
    break;

  case 478: /* setprop: selectset ERRORBAR SIZE expr  */
#line 3755 "pars.yacc"
                                       {
            g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].errbar.barsize = (yyvsp[0].dval);
	}
#line 11035 "y.tab.c"
    break;

  case 479: /* setprop: selectset ERRORBAR linew_select  */
#line 3758 "pars.yacc"
                                          {
            g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].errbar.linew = (yyvsp[0].dval);
	}
#line 11043 "y.tab.c"
    break;

  case 480: /* setprop: selectset ERRORBAR lines_select  */
#line 3761 "pars.yacc"
                                          {
            g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].errbar.lines = (yyvsp[0].ival);
	}
#line 11051 "y.tab.c"
    break;

  case 481: /* setprop: selectset ERRORBAR RISER linew_select  */
#line 3764 "pars.yacc"
                                                {
            g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].errbar.riser_linew = (yyvsp[0].dval);
	}
#line 11059 "y.tab.c"
    break;

  case 482: /* setprop: selectset ERRORBAR RISER lines_select  */
#line 3767 "pars.yacc"
                                                {
            g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].errbar.riser_lines = (yyvsp[0].ival);
	}
#line 11067 "y.tab.c"
    break;

  case 483: /* setprop: selectset ERRORBAR RISER CLIP onoff  */
#line 3770 "pars.yacc"
                                              {
            g[(yyvsp[-4].trgt)->gno].p[(yyvsp[-4].trgt)->setno].errbar.arrow_clip = (yyvsp[0].ival);
	}
#line 11075 "y.tab.c"
    break;

  case 484: /* setprop: selectset ERRORBAR RISER CLIP LENGTH expr  */
#line 3773 "pars.yacc"
                                                    {
            g[(yyvsp[-5].trgt)->gno].p[(yyvsp[-5].trgt)->setno].errbar.cliplen = (yyvsp[0].dval);
	}
#line 11083 "y.tab.c"
    break;

  case 485: /* setprop: selectset COMMENT sexpr  */
#line 3777 "pars.yacc"
                                  {
	    strncpy(g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].comments, (yyvsp[0].sval), MAX_STRING_LENGTH - 1);
	    xfree((yyvsp[0].sval));
	}
#line 11092 "y.tab.c"
    break;

  case 486: /* setprop: selectset LEGEND sexpr  */
#line 3782 "pars.yacc"
                                 {
	    strncpy(g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].lstr, (yyvsp[0].sval), MAX_STRING_LENGTH - 1);
	    xfree((yyvsp[0].sval));
	}
#line 11101 "y.tab.c"
    break;

  case 487: /* axisfeature: onoff  */
#line 3790 "pars.yacc"
              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->active = (yyvsp[0].ival);
	}
#line 11113 "y.tab.c"
    break;

  case 488: /* axisfeature: TYPE ZERO onoff  */
#line 3797 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->zero = (yyvsp[0].ival);
	}
#line 11125 "y.tab.c"
    break;

  case 489: /* axisfeature: TICKP tickattr  */
#line 3804 "pars.yacc"
                         {}
#line 11131 "y.tab.c"
    break;

  case 490: /* axisfeature: TICKP tickattr_obs  */
#line 3805 "pars.yacc"
                             {}
#line 11137 "y.tab.c"
    break;

  case 491: /* axisfeature: TICKLABEL ticklabelattr  */
#line 3806 "pars.yacc"
                                  {}
#line 11143 "y.tab.c"
    break;

  case 492: /* axisfeature: TICKLABEL ticklabelattr_obs  */
#line 3807 "pars.yacc"
                                      {}
#line 11149 "y.tab.c"
    break;

  case 493: /* axisfeature: LABEL axislabeldesc  */
#line 3808 "pars.yacc"
                              {}
#line 11155 "y.tab.c"
    break;

  case 494: /* axisfeature: LABEL axislabeldesc_obs  */
#line 3809 "pars.yacc"
                                  {}
#line 11161 "y.tab.c"
    break;

  case 495: /* axisfeature: BAR axisbardesc  */
#line 3810 "pars.yacc"
                          {}
#line 11167 "y.tab.c"
    break;

  case 496: /* axisfeature: OFFSET expr ',' expr  */
#line 3811 "pars.yacc"
                               {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsx = (yyvsp[-2].dval);
	    g[whichgraph].t[naxis]->offsy = (yyvsp[0].dval);
	}
#line 11180 "y.tab.c"
    break;

  case 497: /* tickattr: onoff  */
#line 3822 "pars.yacc"
              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_flag = (yyvsp[0].ival);
	}
#line 11192 "y.tab.c"
    break;

  case 498: /* tickattr: MAJOR expr  */
#line 3829 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->tmajor = (yyvsp[0].dval);
	}
#line 11204 "y.tab.c"
    break;

  case 499: /* tickattr: MINOR TICKSP nexpr  */
#line 3836 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->nminor = (yyvsp[0].ival);
	}
#line 11216 "y.tab.c"
    break;

  case 500: /* tickattr: PLACE ROUNDED onoff  */
#line 3843 "pars.yacc"
                              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_round = (yyvsp[0].ival);
	}
#line 11228 "y.tab.c"
    break;

  case 501: /* tickattr: OFFSETX expr  */
#line 3851 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsx = (yyvsp[0].dval);
	}
#line 11240 "y.tab.c"
    break;

  case 502: /* tickattr: OFFSETY expr  */
#line 3858 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsy = (yyvsp[0].dval);
	}
#line 11252 "y.tab.c"
    break;

  case 503: /* tickattr: DEFAULT nexpr  */
#line 3865 "pars.yacc"
                        {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_autonum = (yyvsp[0].ival);
	}
#line 11264 "y.tab.c"
    break;

  case 504: /* tickattr: inoutchoice  */
#line 3872 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_inout = (yyvsp[0].ival);
	}
#line 11276 "y.tab.c"
    break;

  case 505: /* tickattr: MAJOR SIZE expr  */
#line 3879 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.size = (yyvsp[0].dval);
	}
#line 11288 "y.tab.c"
    break;

  case 506: /* tickattr: MINOR SIZE expr  */
#line 3886 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.size = (yyvsp[0].dval);
	}
#line 11300 "y.tab.c"
    break;

  case 507: /* tickattr: color_select  */
#line 3893 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.color = g[whichgraph].t[naxis]->mprops.color = (yyvsp[0].ival);
	}
#line 11312 "y.tab.c"
    break;

  case 508: /* tickattr: MAJOR color_select  */
#line 3900 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.color = (yyvsp[0].ival);
	}
#line 11324 "y.tab.c"
    break;

  case 509: /* tickattr: MINOR color_select  */
#line 3907 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.color = (yyvsp[0].ival);
	}
#line 11336 "y.tab.c"
    break;

  case 510: /* tickattr: linew_select  */
#line 3914 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.linew = g[whichgraph].t[naxis]->mprops.linew = (yyvsp[0].dval);
	}
#line 11348 "y.tab.c"
    break;

  case 511: /* tickattr: MAJOR linew_select  */
#line 3921 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.linew = (yyvsp[0].dval);
	}
#line 11360 "y.tab.c"
    break;

  case 512: /* tickattr: MINOR linew_select  */
#line 3928 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.linew = (yyvsp[0].dval);
	}
#line 11372 "y.tab.c"
    break;

  case 513: /* tickattr: MAJOR lines_select  */
#line 3935 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.lines = (yyvsp[0].ival);
	}
#line 11384 "y.tab.c"
    break;

  case 514: /* tickattr: MINOR lines_select  */
#line 3942 "pars.yacc"
                             {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.lines = (yyvsp[0].ival);
	}
#line 11396 "y.tab.c"
    break;

  case 515: /* tickattr: MAJOR GRID onoff  */
#line 3949 "pars.yacc"
                           {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.gridflag = (yyvsp[0].ival);
	}
#line 11408 "y.tab.c"
    break;

  case 516: /* tickattr: MINOR GRID onoff  */
#line 3956 "pars.yacc"
                           {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.gridflag = (yyvsp[0].ival);
	}
#line 11420 "y.tab.c"
    break;

  case 517: /* tickattr: opchoice_sel  */
#line 3963 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_op = (yyvsp[0].ival);
	}
#line 11432 "y.tab.c"
    break;

  case 518: /* tickattr: SPEC TYPE tickspectype  */
#line 3970 "pars.yacc"
                                 {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = (yyvsp[0].ival);
	}
#line 11444 "y.tab.c"
    break;

  case 519: /* tickattr: SPEC nexpr  */
#line 3977 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->nticks = (yyvsp[0].ival);
	}
#line 11456 "y.tab.c"
    break;

  case 520: /* tickattr: MAJOR nexpr ',' expr  */
#line 3984 "pars.yacc"
                               {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].wtpos = (yyvsp[0].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].type = TICK_TYPE_MAJOR;
	}
#line 11469 "y.tab.c"
    break;

  case 521: /* tickattr: MINOR nexpr ',' expr  */
#line 3992 "pars.yacc"
                               {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].wtpos = (yyvsp[0].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].type = TICK_TYPE_MINOR;
	}
#line 11482 "y.tab.c"
    break;

  case 522: /* ticklabelattr: onoff  */
#line 4003 "pars.yacc"
              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_flag = (yyvsp[0].ival);
	}
#line 11494 "y.tab.c"
    break;

  case 523: /* ticklabelattr: PREC nexpr  */
#line 4010 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_prec = (yyvsp[0].ival);
	}
#line 11506 "y.tab.c"
    break;

  case 524: /* ticklabelattr: FORMAT formatchoice  */
#line 4017 "pars.yacc"
                              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_format = (yyvsp[0].ival);
	}
#line 11518 "y.tab.c"
    break;

  case 525: /* ticklabelattr: FORMAT expr  */
#line 4024 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_format = (yyvsp[0].dval);
	}
#line 11530 "y.tab.c"
    break;

  case 526: /* ticklabelattr: APPEND sexpr  */
#line 4031 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    strcpy(g[whichgraph].t[naxis]->tl_appstr, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 11543 "y.tab.c"
    break;

  case 527: /* ticklabelattr: PREPEND sexpr  */
#line 4039 "pars.yacc"
                        {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    strcpy(g[whichgraph].t[naxis]->tl_prestr, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 11556 "y.tab.c"
    break;

  case 528: /* ticklabelattr: ANGLE nexpr  */
#line 4047 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = (yyvsp[0].ival);
	}
#line 11568 "y.tab.c"
    break;

  case 529: /* ticklabelattr: SKIP nexpr  */
#line 4054 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_skip = (yyvsp[0].ival);
	}
#line 11580 "y.tab.c"
    break;

  case 530: /* ticklabelattr: STAGGER nexpr  */
#line 4061 "pars.yacc"
                        {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_staggered = (yyvsp[0].ival);
	}
#line 11592 "y.tab.c"
    break;

  case 531: /* ticklabelattr: opchoice_sel  */
#line 4068 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_op = (yyvsp[0].ival);
	}
#line 11604 "y.tab.c"
    break;

  case 532: /* ticklabelattr: FORMULA sexpr  */
#line 4075 "pars.yacc"
                        {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->tl_formula =
                copy_string(g[whichgraph].t[naxis]->tl_formula, (yyvsp[0].sval));
            xfree((yyvsp[0].sval));
	}
#line 11618 "y.tab.c"
    break;

  case 533: /* ticklabelattr: START expr  */
#line 4084 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_start = (yyvsp[0].dval);
	}
#line 11630 "y.tab.c"
    break;

  case 534: /* ticklabelattr: STOP expr  */
#line 4091 "pars.yacc"
                    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stop = (yyvsp[0].dval);
	}
#line 11642 "y.tab.c"
    break;

  case 535: /* ticklabelattr: START TYPE SPEC  */
#line 4098 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_starttype = TYPE_SPEC;
	}
#line 11654 "y.tab.c"
    break;

  case 536: /* ticklabelattr: START TYPE AUTO  */
#line 4105 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_starttype = TYPE_AUTO;
	}
#line 11666 "y.tab.c"
    break;

  case 537: /* ticklabelattr: STOP TYPE SPEC  */
#line 4112 "pars.yacc"
                         {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stoptype = TYPE_SPEC;
	}
#line 11678 "y.tab.c"
    break;

  case 538: /* ticklabelattr: STOP TYPE AUTO  */
#line 4119 "pars.yacc"
                         {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stoptype = TYPE_AUTO;
	}
#line 11690 "y.tab.c"
    break;

  case 539: /* ticklabelattr: CHAR SIZE expr  */
#line 4126 "pars.yacc"
                         {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_charsize = (yyvsp[0].dval);
	}
#line 11702 "y.tab.c"
    break;

  case 540: /* ticklabelattr: font_select  */
#line 4133 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_font = (yyvsp[0].ival);
	}
#line 11714 "y.tab.c"
    break;

  case 541: /* ticklabelattr: color_select  */
#line 4140 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_color = (yyvsp[0].ival);
	}
#line 11726 "y.tab.c"
    break;

  case 542: /* ticklabelattr: nexpr ',' sexpr  */
#line 4147 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                xfree((yyvsp[0].sval));
                return 1;
            }
	    if ((yyvsp[-2].ival) >= MAX_TICKS) {
	         yyerror("Number of ticks exceeds maximum");
	         xfree((yyvsp[0].sval));
	         return 1;
	    }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].label = 
                copy_string(g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].label, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 11746 "y.tab.c"
    break;

  case 543: /* ticklabelattr: OFFSET AUTO  */
#line 4162 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gaptype = TYPE_AUTO;
	}
#line 11758 "y.tab.c"
    break;

  case 544: /* ticklabelattr: OFFSET SPEC  */
#line 4169 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gaptype = TYPE_SPEC;
	}
#line 11770 "y.tab.c"
    break;

  case 545: /* ticklabelattr: OFFSET expr ',' expr  */
#line 4176 "pars.yacc"
                               {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gap.x = (yyvsp[-2].dval);
	    g[whichgraph].t[naxis]->tl_gap.y = (yyvsp[0].dval);
	}
#line 11783 "y.tab.c"
    break;

  case 546: /* axislabeldesc: sexpr  */
#line 4187 "pars.yacc"
              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].t[naxis]->label, (yyvsp[0].sval));
	    xfree((yyvsp[0].sval));
	}
#line 11796 "y.tab.c"
    break;

  case 547: /* axislabeldesc: LAYOUT PERP  */
#line 4195 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_layout = LAYOUT_PERPENDICULAR;
	}
#line 11808 "y.tab.c"
    break;

  case 548: /* axislabeldesc: LAYOUT PARA  */
#line 4202 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_layout = LAYOUT_PARALLEL;
	}
#line 11820 "y.tab.c"
    break;

  case 549: /* axislabeldesc: PLACE AUTO  */
#line 4209 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_place = TYPE_AUTO;
	}
#line 11832 "y.tab.c"
    break;

  case 550: /* axislabeldesc: PLACE SPEC  */
#line 4216 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_place = TYPE_SPEC;
	}
#line 11844 "y.tab.c"
    break;

  case 551: /* axislabeldesc: PLACE expr ',' expr  */
#line 4223 "pars.yacc"
                              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.x = (yyvsp[-2].dval);
	    g[whichgraph].t[naxis]->label.y = (yyvsp[0].dval);
	}
#line 11857 "y.tab.c"
    break;

  case 552: /* axislabeldesc: JUST justchoice  */
#line 4231 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.just = (yyvsp[0].ival);
	}
#line 11869 "y.tab.c"
    break;

  case 553: /* axislabeldesc: CHAR SIZE expr  */
#line 4238 "pars.yacc"
                         {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.charsize = (yyvsp[0].dval);
	}
#line 11881 "y.tab.c"
    break;

  case 554: /* axislabeldesc: font_select  */
#line 4245 "pars.yacc"
                      {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.font = (yyvsp[0].ival);
	}
#line 11893 "y.tab.c"
    break;

  case 555: /* axislabeldesc: color_select  */
#line 4252 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.color = (yyvsp[0].ival);
	}
#line 11905 "y.tab.c"
    break;

  case 556: /* axislabeldesc: opchoice_sel  */
#line 4259 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_op = (yyvsp[0].ival);
	}
#line 11917 "y.tab.c"
    break;

  case 557: /* axisbardesc: onoff  */
#line 4269 "pars.yacc"
              {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbar = (yyvsp[0].ival);
	}
#line 11929 "y.tab.c"
    break;

  case 558: /* axisbardesc: color_select  */
#line 4276 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarcolor = (yyvsp[0].ival);
	}
#line 11941 "y.tab.c"
    break;

  case 559: /* axisbardesc: lines_select  */
#line 4283 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarlines = (yyvsp[0].ival);
	}
#line 11953 "y.tab.c"
    break;

  case 560: /* axisbardesc: linew_select  */
#line 4290 "pars.yacc"
                       {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarlinew = (yyvsp[0].dval);
	}
#line 11965 "y.tab.c"
    break;

  case 561: /* nonlfitopts: TITLE sexpr  */
#line 4300 "pars.yacc"
                    { 
          nonl_opts.title = copy_string(nonl_opts.title, (yyvsp[0].sval));
	  xfree((yyvsp[0].sval));
        }
#line 11974 "y.tab.c"
    break;

  case 562: /* nonlfitopts: FORMULA sexpr  */
#line 4304 "pars.yacc"
                        { 
          nonl_opts.formula = copy_string(nonl_opts.formula, (yyvsp[0].sval));
	  xfree((yyvsp[0].sval));
        }
#line 11983 "y.tab.c"
    break;

  case 563: /* nonlfitopts: WITH nexpr PARAMETERS  */
#line 4308 "pars.yacc"
                                { 
            nonl_opts.parnum = (yyvsp[-1].ival); 
        }
#line 11991 "y.tab.c"
    break;

  case 564: /* nonlfitopts: PREC expr  */
#line 4311 "pars.yacc"
                    { 
            nonl_opts.tolerance = (yyvsp[0].dval); 
        }
#line 11999 "y.tab.c"
    break;

  case 565: /* selectgraph: GRAPHNO  */
#line 4318 "pars.yacc"
        {
            (yyval.ival) = (yyvsp[0].ival);
        }
#line 12007 "y.tab.c"
    break;

  case 566: /* selectgraph: GRAPH indx  */
#line 4322 "pars.yacc"
        {
            (yyval.ival) = (yyvsp[0].ival);
        }
#line 12015 "y.tab.c"
    break;

  case 567: /* selectset: selectgraph '.' SETNUM  */
#line 4329 "pars.yacc"
        {
	    int gno = (yyvsp[-2].ival), setno = (yyvsp[0].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
#line 12032 "y.tab.c"
    break;

  case 568: /* selectset: selectgraph '.' SET indx  */
#line 4342 "pars.yacc"
        {
	    int gno = (yyvsp[-3].ival), setno = (yyvsp[0].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
#line 12049 "y.tab.c"
    break;

  case 569: /* selectset: SETNUM  */
#line 4355 "pars.yacc"
        {
	    int gno = whichgraph, setno = (yyvsp[0].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
#line 12066 "y.tab.c"
    break;

  case 570: /* selectset: SET indx  */
#line 4368 "pars.yacc"
        {
	    int gno = whichgraph, setno = (yyvsp[0].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
#line 12083 "y.tab.c"
    break;

  case 571: /* setaxis: axis axisfeature  */
#line 4383 "pars.yacc"
                         {}
#line 12089 "y.tab.c"
    break;

  case 572: /* setaxis: selectgraph axis axisfeature  */
#line 4384 "pars.yacc"
                                       {}
#line 12095 "y.tab.c"
    break;

  case 573: /* axis: XAXIS  */
#line 4388 "pars.yacc"
              { naxis =  X_AXIS; }
#line 12101 "y.tab.c"
    break;

  case 574: /* axis: YAXIS  */
#line 4389 "pars.yacc"
                { naxis = Y_AXIS; }
#line 12107 "y.tab.c"
    break;

  case 575: /* axis: ALTXAXIS  */
#line 4390 "pars.yacc"
                   { naxis = ZX_AXIS; }
#line 12113 "y.tab.c"
    break;

  case 576: /* axis: ALTYAXIS  */
#line 4391 "pars.yacc"
                   { naxis = ZY_AXIS; }
#line 12119 "y.tab.c"
    break;

  case 577: /* proctype: KEY_CONST  */
#line 4395 "pars.yacc"
                          { (yyval.ival) = CONSTANT;  }
#line 12125 "y.tab.c"
    break;

  case 578: /* proctype: KEY_UNIT  */
#line 4396 "pars.yacc"
                          { (yyval.ival) = UCONSTANT; }
#line 12131 "y.tab.c"
    break;

  case 579: /* proctype: KEY_FUNC_I  */
#line 4397 "pars.yacc"
                          { (yyval.ival) = FUNC_I;    }
#line 12137 "y.tab.c"
    break;

  case 580: /* proctype: KEY_FUNC_D  */
#line 4398 "pars.yacc"
                          { (yyval.ival) = FUNC_D;    }
#line 12143 "y.tab.c"
    break;

  case 581: /* proctype: KEY_FUNC_ND  */
#line 4399 "pars.yacc"
                          { (yyval.ival) = FUNC_ND;   }
#line 12149 "y.tab.c"
    break;

  case 582: /* proctype: KEY_FUNC_NN  */
#line 4400 "pars.yacc"
                          { (yyval.ival) = FUNC_NN;   }
#line 12155 "y.tab.c"
    break;

  case 583: /* proctype: KEY_FUNC_DD  */
#line 4401 "pars.yacc"
                          { (yyval.ival) = FUNC_DD;   }
#line 12161 "y.tab.c"
    break;

  case 584: /* proctype: KEY_FUNC_NND  */
#line 4402 "pars.yacc"
                          { (yyval.ival) = FUNC_NND;  }
#line 12167 "y.tab.c"
    break;

  case 585: /* proctype: KEY_FUNC_PPD  */
#line 4403 "pars.yacc"
                          { (yyval.ival) = FUNC_PPD;  }
#line 12173 "y.tab.c"
    break;

  case 586: /* proctype: KEY_FUNC_PPPD  */
#line 4404 "pars.yacc"
                          { (yyval.ival) = FUNC_PPPD; }
#line 12179 "y.tab.c"
    break;

  case 587: /* proctype: KEY_FUNC_PPPPD  */
#line 4405 "pars.yacc"
                          { (yyval.ival) = FUNC_PPPPD; }
#line 12185 "y.tab.c"
    break;

  case 588: /* proctype: KEY_FUNC_PPPPPD  */
#line 4406 "pars.yacc"
                          { (yyval.ival) = FUNC_PPPPPD; }
#line 12191 "y.tab.c"
    break;

  case 589: /* tickspectype: NONE  */
#line 4410 "pars.yacc"
             { (yyval.ival) =  TICKS_SPEC_NONE; }
#line 12197 "y.tab.c"
    break;

  case 590: /* tickspectype: TICKSP  */
#line 4411 "pars.yacc"
                 { (yyval.ival) = TICKS_SPEC_MARKS; }
#line 12203 "y.tab.c"
    break;

  case 591: /* tickspectype: BOTH  */
#line 4412 "pars.yacc"
               { (yyval.ival) = TICKS_SPEC_BOTH; }
#line 12209 "y.tab.c"
    break;

  case 592: /* filtertype: IFILTER  */
#line 4416 "pars.yacc"
                      { (yyval.ival) = FILTER_INPUT; }
#line 12215 "y.tab.c"
    break;

  case 593: /* filtertype: OFILTER  */
#line 4417 "pars.yacc"
                     { (yyval.ival) = FILTER_OUTPUT; }
#line 12221 "y.tab.c"
    break;

  case 594: /* filtermethod: MAGIC  */
#line 4421 "pars.yacc"
                      { (yyval.ival) = FILTER_MAGIC; }
#line 12227 "y.tab.c"
    break;

  case 595: /* filtermethod: PATTERN  */
#line 4422 "pars.yacc"
                    { (yyval.ival) = FILTER_PATTERN; }
#line 12233 "y.tab.c"
    break;

  case 596: /* xytype: XY  */
#line 4426 "pars.yacc"
           { (yyval.ival) = SET_XY; }
#line 12239 "y.tab.c"
    break;

  case 597: /* xytype: BAR  */
#line 4427 "pars.yacc"
              { (yyval.ival) = SET_BAR; }
#line 12245 "y.tab.c"
    break;

  case 598: /* xytype: BARDY  */
#line 4428 "pars.yacc"
                { (yyval.ival) = SET_BARDY; }
#line 12251 "y.tab.c"
    break;

  case 599: /* xytype: BARDYDY  */
#line 4429 "pars.yacc"
                  { (yyval.ival) = SET_BARDYDY; }
#line 12257 "y.tab.c"
    break;

  case 600: /* xytype: XYZ  */
#line 4430 "pars.yacc"
              { (yyval.ival) = SET_XYZ; }
#line 12263 "y.tab.c"
    break;

  case 601: /* xytype: XYDX  */
#line 4431 "pars.yacc"
               { (yyval.ival) = SET_XYDX; }
#line 12269 "y.tab.c"
    break;

  case 602: /* xytype: XYDY  */
#line 4432 "pars.yacc"
               { (yyval.ival) = SET_XYDY; }
#line 12275 "y.tab.c"
    break;

  case 603: /* xytype: XYDXDX  */
#line 4433 "pars.yacc"
                 { (yyval.ival) = SET_XYDXDX; }
#line 12281 "y.tab.c"
    break;

  case 604: /* xytype: XYDYDY  */
#line 4434 "pars.yacc"
                 { (yyval.ival) = SET_XYDYDY; }
#line 12287 "y.tab.c"
    break;

  case 605: /* xytype: XYDXDY  */
#line 4435 "pars.yacc"
                 { (yyval.ival) = SET_XYDXDY; }
#line 12293 "y.tab.c"
    break;

  case 606: /* xytype: XYDXDXDYDY  */
#line 4436 "pars.yacc"
                     { (yyval.ival) = SET_XYDXDXDYDY; }
#line 12299 "y.tab.c"
    break;

  case 607: /* xytype: XYHILO  */
#line 4437 "pars.yacc"
                 { (yyval.ival) = SET_XYHILO; }
#line 12305 "y.tab.c"
    break;

  case 608: /* xytype: XYR  */
#line 4438 "pars.yacc"
              { (yyval.ival) = SET_XYR; }
#line 12311 "y.tab.c"
    break;

  case 609: /* xytype: XYSIZE  */
#line 4439 "pars.yacc"
                 { (yyval.ival) = SET_XYSIZE; }
#line 12317 "y.tab.c"
    break;

  case 610: /* xytype: XYCOLOR  */
#line 4440 "pars.yacc"
                  { (yyval.ival) = SET_XYCOLOR; }
#line 12323 "y.tab.c"
    break;

  case 611: /* xytype: XYCOLPAT  */
#line 4441 "pars.yacc"
                   { (yyval.ival) = SET_XYCOLPAT; }
#line 12329 "y.tab.c"
    break;

  case 612: /* xytype: XYVMAP  */
#line 4442 "pars.yacc"
                 { (yyval.ival) = SET_XYVMAP; }
#line 12335 "y.tab.c"
    break;

  case 613: /* xytype: XYBOXPLOT  */
#line 4443 "pars.yacc"
                    { (yyval.ival) = SET_BOXPLOT; }
#line 12341 "y.tab.c"
    break;

  case 614: /* xytype: XYSTRING  */
#line 4444 "pars.yacc"
                   { (yyval.ival) = SET_XY; }
#line 12347 "y.tab.c"
    break;

  case 615: /* graphtype: XY  */
#line 4448 "pars.yacc"
           { (yyval.ival) = GRAPH_XY; }
#line 12353 "y.tab.c"
    break;

  case 616: /* graphtype: CHART  */
#line 4449 "pars.yacc"
                { (yyval.ival) = GRAPH_CHART; }
#line 12359 "y.tab.c"
    break;

  case 617: /* graphtype: POLAR  */
#line 4450 "pars.yacc"
                { (yyval.ival) = GRAPH_POLAR; }
#line 12365 "y.tab.c"
    break;

  case 618: /* graphtype: SMITH  */
#line 4451 "pars.yacc"
                { (yyval.ival) = GRAPH_SMITH; }
#line 12371 "y.tab.c"
    break;

  case 619: /* graphtype: FIXED  */
#line 4452 "pars.yacc"
                { (yyval.ival) = GRAPH_FIXED; }
#line 12377 "y.tab.c"
    break;

  case 620: /* graphtype: PIE  */
#line 4453 "pars.yacc"
                { (yyval.ival) = GRAPH_PIE;   }
#line 12383 "y.tab.c"
    break;

  case 621: /* pagelayout: FREE  */
#line 4457 "pars.yacc"
             { (yyval.ival) = PAGE_FREE; }
#line 12389 "y.tab.c"
    break;

  case 622: /* pagelayout: FIXED  */
#line 4458 "pars.yacc"
                { (yyval.ival) = PAGE_FIXED; }
#line 12395 "y.tab.c"
    break;

  case 623: /* pageorient: LANDSCAPE  */
#line 4462 "pars.yacc"
                   { (yyval.ival) = PAGE_ORIENT_LANDSCAPE; }
#line 12401 "y.tab.c"
    break;

  case 624: /* pageorient: PORTRAIT  */
#line 4463 "pars.yacc"
                   { (yyval.ival) = PAGE_ORIENT_PORTRAIT;  }
#line 12407 "y.tab.c"
    break;

  case 625: /* regiontype: ABOVE  */
#line 4467 "pars.yacc"
              { (yyval.ival) = REGION_ABOVE; }
#line 12413 "y.tab.c"
    break;

  case 626: /* regiontype: BELOW  */
#line 4468 "pars.yacc"
                 { (yyval.ival) = REGION_BELOW; }
#line 12419 "y.tab.c"
    break;

  case 627: /* regiontype: LEFT  */
#line 4469 "pars.yacc"
                { (yyval.ival) = REGION_TOLEFT; }
#line 12425 "y.tab.c"
    break;

  case 628: /* regiontype: RIGHT  */
#line 4470 "pars.yacc"
                 { (yyval.ival) = REGION_TORIGHT; }
#line 12431 "y.tab.c"
    break;

  case 629: /* regiontype: POLYI  */
#line 4471 "pars.yacc"
                 { (yyval.ival) = REGION_POLYI; }
#line 12437 "y.tab.c"
    break;

  case 630: /* regiontype: POLYO  */
#line 4472 "pars.yacc"
                 { (yyval.ival) = REGION_POLYO; }
#line 12443 "y.tab.c"
    break;

  case 631: /* regiontype: HORIZI  */
#line 4473 "pars.yacc"
                  { (yyval.ival) = REGION_HORIZI; }
#line 12449 "y.tab.c"
    break;

  case 632: /* regiontype: VERTI  */
#line 4474 "pars.yacc"
                 { (yyval.ival) = REGION_VERTI; }
#line 12455 "y.tab.c"
    break;

  case 633: /* regiontype: HORIZO  */
#line 4475 "pars.yacc"
                  { (yyval.ival) = REGION_HORIZO; }
#line 12461 "y.tab.c"
    break;

  case 634: /* regiontype: VERTO  */
#line 4476 "pars.yacc"
                 { (yyval.ival) = REGION_VERTO; }
#line 12467 "y.tab.c"
    break;

  case 635: /* scaletype: NORMAL  */
#line 4479 "pars.yacc"
                  { (yyval.ival) = SCALE_NORMAL; }
#line 12473 "y.tab.c"
    break;

  case 636: /* scaletype: LOGARITHMIC  */
#line 4480 "pars.yacc"
                      { (yyval.ival) = SCALE_LOG; }
#line 12479 "y.tab.c"
    break;

  case 637: /* scaletype: RECIPROCAL  */
#line 4481 "pars.yacc"
                     { (yyval.ival) = SCALE_REC; }
#line 12485 "y.tab.c"
    break;

  case 638: /* scaletype: LOGIT  */
#line 4482 "pars.yacc"
                { (yyval.ival) = SCALE_LOGIT; }
#line 12491 "y.tab.c"
    break;

  case 639: /* onoff: ON  */
#line 4485 "pars.yacc"
          { (yyval.ival) = TRUE; }
#line 12497 "y.tab.c"
    break;

  case 640: /* onoff: OFF  */
#line 4486 "pars.yacc"
              { (yyval.ival) = FALSE; }
#line 12503 "y.tab.c"
    break;

  case 641: /* runtype: RUNAVG  */
#line 4489 "pars.yacc"
                { (yyval.ival) = RUN_AVG; }
#line 12509 "y.tab.c"
    break;

  case 642: /* runtype: RUNSTD  */
#line 4490 "pars.yacc"
                 { (yyval.ival) = RUN_STD; }
#line 12515 "y.tab.c"
    break;

  case 643: /* runtype: RUNMED  */
#line 4491 "pars.yacc"
                 { (yyval.ival) = RUN_MED; }
#line 12521 "y.tab.c"
    break;

  case 644: /* runtype: RUNMAX  */
#line 4492 "pars.yacc"
                 { (yyval.ival) = RUN_MAX; }
#line 12527 "y.tab.c"
    break;

  case 645: /* runtype: RUNMIN  */
#line 4493 "pars.yacc"
                 { (yyval.ival) = RUN_MIN; }
#line 12533 "y.tab.c"
    break;

  case 646: /* sourcetype: DISK  */
#line 4497 "pars.yacc"
             { (yyval.ival) = SOURCE_DISK; }
#line 12539 "y.tab.c"
    break;

  case 647: /* sourcetype: PIPE  */
#line 4498 "pars.yacc"
               {
            if (!safe_mode) {
                (yyval.ival) = SOURCE_PIPE;
            } else {
                yyerror("Pipe inputs are disabled in safe mode");
                (yyval.ival) = SOURCE_DISK;
            }
        }
#line 12552 "y.tab.c"
    break;

  case 648: /* justchoice: RIGHT  */
#line 4508 "pars.yacc"
                  { (yyval.ival) = JUST_RIGHT; }
#line 12558 "y.tab.c"
    break;

  case 649: /* justchoice: LEFT  */
#line 4509 "pars.yacc"
               { (yyval.ival) = JUST_LEFT; }
#line 12564 "y.tab.c"
    break;

  case 650: /* justchoice: CENTER  */
#line 4510 "pars.yacc"
                 { (yyval.ival) = JUST_CENTER; }
#line 12570 "y.tab.c"
    break;

  case 651: /* inoutchoice: IN  */
#line 4513 "pars.yacc"
                { (yyval.ival) = TICKS_IN; }
#line 12576 "y.tab.c"
    break;

  case 652: /* inoutchoice: OUT  */
#line 4514 "pars.yacc"
              { (yyval.ival) = TICKS_OUT; }
#line 12582 "y.tab.c"
    break;

  case 653: /* inoutchoice: BOTH  */
#line 4515 "pars.yacc"
               { (yyval.ival) = TICKS_BOTH; }
#line 12588 "y.tab.c"
    break;

  case 654: /* formatchoice: DECIMAL  */
#line 4518 "pars.yacc"
                      { (yyval.ival) = FORMAT_DECIMAL; }
#line 12594 "y.tab.c"
    break;

  case 655: /* formatchoice: EXPONENTIAL  */
#line 4519 "pars.yacc"
                      { (yyval.ival) = FORMAT_EXPONENTIAL; }
#line 12600 "y.tab.c"
    break;

  case 656: /* formatchoice: GENERAL  */
#line 4520 "pars.yacc"
                  { (yyval.ival) = FORMAT_GENERAL; }
#line 12606 "y.tab.c"
    break;

  case 657: /* formatchoice: SCIENTIFIC  */
#line 4521 "pars.yacc"
                     { (yyval.ival) = FORMAT_SCIENTIFIC; }
#line 12612 "y.tab.c"
    break;

  case 658: /* formatchoice: ENGINEERING  */
#line 4522 "pars.yacc"
                      { (yyval.ival) = FORMAT_ENGINEERING; }
#line 12618 "y.tab.c"
    break;

  case 659: /* formatchoice: COMPUTING  */
#line 4523 "pars.yacc"
                    { (yyval.ival) = FORMAT_COMPUTING; }
#line 12624 "y.tab.c"
    break;

  case 660: /* formatchoice: POWER  */
#line 4524 "pars.yacc"
                { (yyval.ival) = FORMAT_POWER; }
#line 12630 "y.tab.c"
    break;

  case 661: /* formatchoice: DDMMYY  */
#line 4525 "pars.yacc"
                 { (yyval.ival) = FORMAT_DDMMYY; }
#line 12636 "y.tab.c"
    break;

  case 662: /* formatchoice: MMDDYY  */
#line 4526 "pars.yacc"
                 { (yyval.ival) = FORMAT_MMDDYY; }
#line 12642 "y.tab.c"
    break;

  case 663: /* formatchoice: YYMMDD  */
#line 4527 "pars.yacc"
                 { (yyval.ival) = FORMAT_YYMMDD; }
#line 12648 "y.tab.c"
    break;

  case 664: /* formatchoice: MMYY  */
#line 4528 "pars.yacc"
               { (yyval.ival) = FORMAT_MMYY; }
#line 12654 "y.tab.c"
    break;

  case 665: /* formatchoice: MMDD  */
#line 4529 "pars.yacc"
               { (yyval.ival) = FORMAT_MMDD; }
#line 12660 "y.tab.c"
    break;

  case 666: /* formatchoice: MONTHDAY  */
#line 4530 "pars.yacc"
                   { (yyval.ival) = FORMAT_MONTHDAY; }
#line 12666 "y.tab.c"
    break;

  case 667: /* formatchoice: DAYMONTH  */
#line 4531 "pars.yacc"
                   { (yyval.ival) = FORMAT_DAYMONTH; }
#line 12672 "y.tab.c"
    break;

  case 668: /* formatchoice: MONTHS  */
#line 4532 "pars.yacc"
                 { (yyval.ival) = FORMAT_MONTHS; }
#line 12678 "y.tab.c"
    break;

  case 669: /* formatchoice: MONTHSY  */
#line 4533 "pars.yacc"
                  { (yyval.ival) = FORMAT_MONTHSY; }
#line 12684 "y.tab.c"
    break;

  case 670: /* formatchoice: MONTHL  */
#line 4534 "pars.yacc"
                 { (yyval.ival) = FORMAT_MONTHL; }
#line 12690 "y.tab.c"
    break;

  case 671: /* formatchoice: DAYOFWEEKS  */
#line 4535 "pars.yacc"
                     { (yyval.ival) = FORMAT_DAYOFWEEKS; }
#line 12696 "y.tab.c"
    break;

  case 672: /* formatchoice: DAYOFWEEKL  */
#line 4536 "pars.yacc"
                     { (yyval.ival) = FORMAT_DAYOFWEEKL; }
#line 12702 "y.tab.c"
    break;

  case 673: /* formatchoice: DAYOFYEAR  */
#line 4537 "pars.yacc"
                    { (yyval.ival) = FORMAT_DAYOFYEAR; }
#line 12708 "y.tab.c"
    break;

  case 674: /* formatchoice: HMS  */
#line 4538 "pars.yacc"
              { (yyval.ival) = FORMAT_HMS; }
#line 12714 "y.tab.c"
    break;

  case 675: /* formatchoice: MMDDHMS  */
#line 4539 "pars.yacc"
                  { (yyval.ival) = FORMAT_MMDDHMS; }
#line 12720 "y.tab.c"
    break;

  case 676: /* formatchoice: MMDDYYHMS  */
#line 4540 "pars.yacc"
                    { (yyval.ival) = FORMAT_MMDDYYHMS; }
#line 12726 "y.tab.c"
    break;

  case 677: /* formatchoice: YYMMDDHMS  */
#line 4541 "pars.yacc"
                    { (yyval.ival) = FORMAT_YYMMDDHMS; }
#line 12732 "y.tab.c"
    break;

  case 678: /* formatchoice: DEGREESLON  */
#line 4542 "pars.yacc"
                     { (yyval.ival) = FORMAT_DEGREESLON; }
#line 12738 "y.tab.c"
    break;

  case 679: /* formatchoice: DEGREESMMLON  */
#line 4543 "pars.yacc"
                       { (yyval.ival) = FORMAT_DEGREESMMLON; }
#line 12744 "y.tab.c"
    break;

  case 680: /* formatchoice: DEGREESMMSSLON  */
#line 4544 "pars.yacc"
                         { (yyval.ival) = FORMAT_DEGREESMMSSLON; }
#line 12750 "y.tab.c"
    break;

  case 681: /* formatchoice: MMSSLON  */
#line 4545 "pars.yacc"
                  { (yyval.ival) = FORMAT_MMSSLON; }
#line 12756 "y.tab.c"
    break;

  case 682: /* formatchoice: DEGREESLAT  */
#line 4546 "pars.yacc"
                     { (yyval.ival) = FORMAT_DEGREESLAT; }
#line 12762 "y.tab.c"
    break;

  case 683: /* formatchoice: DEGREESMMLAT  */
#line 4547 "pars.yacc"
                       { (yyval.ival) = FORMAT_DEGREESMMLAT; }
#line 12768 "y.tab.c"
    break;

  case 684: /* formatchoice: DEGREESMMSSLAT  */
#line 4548 "pars.yacc"
                         { (yyval.ival) = FORMAT_DEGREESMMSSLAT; }
#line 12774 "y.tab.c"
    break;

  case 685: /* formatchoice: MMSSLAT  */
#line 4549 "pars.yacc"
                  { (yyval.ival) = FORMAT_MMSSLAT; }
#line 12780 "y.tab.c"
    break;

  case 686: /* signchoice: NORMAL  */
#line 4552 "pars.yacc"
                   { (yyval.ival) = SIGN_NORMAL; }
#line 12786 "y.tab.c"
    break;

  case 687: /* signchoice: ABSOLUTE  */
#line 4553 "pars.yacc"
                   { (yyval.ival) = SIGN_ABSOLUTE; }
#line 12792 "y.tab.c"
    break;

  case 688: /* signchoice: NEGATE  */
#line 4554 "pars.yacc"
                 { (yyval.ival) = SIGN_NEGATE; }
#line 12798 "y.tab.c"
    break;

  case 689: /* direction: UP  */
#line 4557 "pars.yacc"
              { (yyval.ival) = UP; }
#line 12804 "y.tab.c"
    break;

  case 690: /* direction: DOWN  */
#line 4558 "pars.yacc"
               { (yyval.ival) = DOWN; }
#line 12810 "y.tab.c"
    break;

  case 691: /* direction: RIGHT  */
#line 4559 "pars.yacc"
                { (yyval.ival) = RIGHT; }
#line 12816 "y.tab.c"
    break;

  case 692: /* direction: LEFT  */
#line 4560 "pars.yacc"
               { (yyval.ival) = LEFT; }
#line 12822 "y.tab.c"
    break;

  case 693: /* direction: IN  */
#line 4561 "pars.yacc"
             { (yyval.ival) = IN; }
#line 12828 "y.tab.c"
    break;

  case 694: /* direction: OUT  */
#line 4562 "pars.yacc"
              { (yyval.ival) = OUT; }
#line 12834 "y.tab.c"
    break;

  case 695: /* worldview: WORLD  */
#line 4565 "pars.yacc"
                 { (yyval.ival) = COORD_WORLD; }
#line 12840 "y.tab.c"
    break;

  case 696: /* worldview: VIEW  */
#line 4566 "pars.yacc"
               { (yyval.ival) = COORD_VIEW; }
#line 12846 "y.tab.c"
    break;

  case 697: /* datacolumn: X_TOK  */
#line 4569 "pars.yacc"
                  { (yyval.ival) = DATA_X; }
#line 12852 "y.tab.c"
    break;

  case 698: /* datacolumn: Y_TOK  */
#line 4570 "pars.yacc"
                { (yyval.ival) = DATA_Y; }
#line 12858 "y.tab.c"
    break;

  case 699: /* datacolumn: X0  */
#line 4571 "pars.yacc"
             { (yyval.ival) = DATA_X; }
#line 12864 "y.tab.c"
    break;

  case 700: /* datacolumn: Y0  */
#line 4572 "pars.yacc"
             { (yyval.ival) = DATA_Y; }
#line 12870 "y.tab.c"
    break;

  case 701: /* datacolumn: Y1  */
#line 4573 "pars.yacc"
             { (yyval.ival) = DATA_Y1; }
#line 12876 "y.tab.c"
    break;

  case 702: /* datacolumn: Y2  */
#line 4574 "pars.yacc"
             { (yyval.ival) = DATA_Y2; }
#line 12882 "y.tab.c"
    break;

  case 703: /* datacolumn: Y3  */
#line 4575 "pars.yacc"
             { (yyval.ival) = DATA_Y3; }
#line 12888 "y.tab.c"
    break;

  case 704: /* datacolumn: Y4  */
#line 4576 "pars.yacc"
             { (yyval.ival) = DATA_Y4; }
#line 12894 "y.tab.c"
    break;

  case 705: /* sortdir: ASCENDING  */
#line 4579 "pars.yacc"
                   { (yyval.ival) = ASCENDING; }
#line 12900 "y.tab.c"
    break;

  case 706: /* sortdir: DESCENDING  */
#line 4580 "pars.yacc"
                     { (yyval.ival) = DESCENDING; }
#line 12906 "y.tab.c"
    break;

  case 707: /* sorton: X_TOK  */
#line 4583 "pars.yacc"
              { (yyval.ival) = DATA_X; }
#line 12912 "y.tab.c"
    break;

  case 708: /* sorton: Y_TOK  */
#line 4584 "pars.yacc"
                { (yyval.ival) = DATA_Y; }
#line 12918 "y.tab.c"
    break;

  case 709: /* ffttype: DFT  */
#line 4587 "pars.yacc"
             { (yyval.ival) = FFT_DFT; }
#line 12924 "y.tab.c"
    break;

  case 710: /* ffttype: FFT  */
#line 4588 "pars.yacc"
              { (yyval.ival) = FFT_FFT; }
#line 12930 "y.tab.c"
    break;

  case 711: /* ffttype: INVDFT  */
#line 4589 "pars.yacc"
                 { (yyval.ival) = FFT_INVDFT; }
#line 12936 "y.tab.c"
    break;

  case 712: /* ffttype: INVFFT  */
#line 4590 "pars.yacc"
                 { (yyval.ival) = FFT_INVFFT; }
#line 12942 "y.tab.c"
    break;

  case 713: /* fourierdata: REAL  */
#line 4594 "pars.yacc"
             {(yyval.ival)=0;}
#line 12948 "y.tab.c"
    break;

  case 714: /* fourierdata: COMPLEX  */
#line 4595 "pars.yacc"
                  {(yyval.ival)=1;}
#line 12954 "y.tab.c"
    break;

  case 715: /* fourierloadx: INDEX  */
#line 4599 "pars.yacc"
              {(yyval.ival)=0;}
#line 12960 "y.tab.c"
    break;

  case 716: /* fourierloadx: FREQUENCY  */
#line 4600 "pars.yacc"
                    {(yyval.ival)=1;}
#line 12966 "y.tab.c"
    break;

  case 717: /* fourierloadx: PERIOD  */
#line 4601 "pars.yacc"
                 {(yyval.ival)=2;}
#line 12972 "y.tab.c"
    break;

  case 718: /* fourierloady: MAGNITUDE  */
#line 4605 "pars.yacc"
                  {(yyval.ival)=0;}
#line 12978 "y.tab.c"
    break;

  case 719: /* fourierloady: PHASE  */
#line 4606 "pars.yacc"
                {(yyval.ival)=1;}
#line 12984 "y.tab.c"
    break;

  case 720: /* fourierloady: COEFFICIENTS  */
#line 4607 "pars.yacc"
                       {(yyval.ival)=2;}
#line 12990 "y.tab.c"
    break;

  case 721: /* windowtype: NONE  */
#line 4611 "pars.yacc"
             {(yyval.ival)=0;}
#line 12996 "y.tab.c"
    break;

  case 722: /* windowtype: TRIANGULAR  */
#line 4612 "pars.yacc"
                     {(yyval.ival)=1;}
#line 13002 "y.tab.c"
    break;

  case 723: /* windowtype: HANNING  */
#line 4613 "pars.yacc"
                  {(yyval.ival)=2;}
#line 13008 "y.tab.c"
    break;

  case 724: /* windowtype: WELCH  */
#line 4614 "pars.yacc"
                {(yyval.ival)=3;}
#line 13014 "y.tab.c"
    break;

  case 725: /* windowtype: HAMMING  */
#line 4615 "pars.yacc"
                  {(yyval.ival)=4;}
#line 13020 "y.tab.c"
    break;

  case 726: /* windowtype: BLACKMAN  */
#line 4616 "pars.yacc"
                   {(yyval.ival)=5;}
#line 13026 "y.tab.c"
    break;

  case 727: /* windowtype: PARZEN  */
#line 4617 "pars.yacc"
                 {(yyval.ival)=6;}
#line 13032 "y.tab.c"
    break;

  case 728: /* interpmethod: LINEAR  */
#line 4621 "pars.yacc"
                  { (yyval.ival) = INTERP_LINEAR; }
#line 13038 "y.tab.c"
    break;

  case 729: /* interpmethod: SPLINE  */
#line 4622 "pars.yacc"
                  { (yyval.ival) = INTERP_SPLINE; }
#line 13044 "y.tab.c"
    break;

  case 730: /* interpmethod: ASPLINE  */
#line 4623 "pars.yacc"
                  { (yyval.ival) = INTERP_ASPLINE; }
#line 13050 "y.tab.c"
    break;

  case 731: /* stattype: MINP  */
#line 4626 "pars.yacc"
               { (yyval.ival) = MINP; }
#line 13056 "y.tab.c"
    break;

  case 732: /* stattype: MAXP  */
#line 4627 "pars.yacc"
               { (yyval.ival) = MAXP; }
#line 13062 "y.tab.c"
    break;

  case 733: /* stattype: AVG  */
#line 4628 "pars.yacc"
              { (yyval.ival) = AVG; }
#line 13068 "y.tab.c"
    break;

  case 734: /* stattype: SD  */
#line 4629 "pars.yacc"
             { (yyval.ival) = SD; }
#line 13074 "y.tab.c"
    break;

  case 735: /* stattype: SUM  */
#line 4630 "pars.yacc"
              { (yyval.ival) = SUM; }
#line 13080 "y.tab.c"
    break;

  case 736: /* stattype: IMIN  */
#line 4631 "pars.yacc"
               { (yyval.ival) = IMIN; }
#line 13086 "y.tab.c"
    break;

  case 737: /* stattype: IMAX  */
#line 4632 "pars.yacc"
               { (yyval.ival) = IMAX; }
#line 13092 "y.tab.c"
    break;

  case 738: /* font_select: FONTP nexpr  */
#line 4637 "pars.yacc"
        {
            (yyval.ival) = get_mapped_font((yyvsp[0].ival));
        }
#line 13100 "y.tab.c"
    break;

  case 739: /* font_select: FONTP sexpr  */
#line 4641 "pars.yacc"
        {
            (yyval.ival) = get_font_by_name((yyvsp[0].sval));
            xfree((yyvsp[0].sval));
        }
#line 13109 "y.tab.c"
    break;

  case 740: /* lines_select: LINESTYLE nexpr  */
#line 4649 "pars.yacc"
        {
	    int lines = (yyvsp[0].ival);
            if (lines >= 0 && lines < number_of_linestyles()) {
	        (yyval.ival) = lines;
	    } else {
	        errmsg("invalid linestyle");
	        (yyval.ival) = 1;
	    }
        }
#line 13123 "y.tab.c"
    break;

  case 741: /* pattern_select: PATTERN nexpr  */
#line 4662 "pars.yacc"
        {
	    int patno = (yyvsp[0].ival);
            if (patno >= 0 && patno < number_of_patterns()) {
	        (yyval.ival) = patno;
	    } else {
	        errmsg("invalid pattern number");
	        (yyval.ival) = 1;
	    }
        }
#line 13137 "y.tab.c"
    break;

  case 742: /* color_select: COLOR nexpr  */
#line 4675 "pars.yacc"
        {
            int c = (yyvsp[0].ival);
            if (c >= 0 && c < number_of_colors()) {
                (yyval.ival) = c;
            } else {
                errmsg("Invalid color ID");
                (yyval.ival) = 1;
            }
        }
#line 13151 "y.tab.c"
    break;

  case 743: /* color_select: COLOR sexpr  */
#line 4685 "pars.yacc"
        {
            int c = get_color_by_name((yyvsp[0].sval));
            if (c == BAD_COLOR) {
                errmsg("Invalid color name");
                c = 1;
            }
            xfree((yyvsp[0].sval));
            (yyval.ival) = c;
        }
#line 13165 "y.tab.c"
    break;

  case 744: /* color_select: COLOR '(' nexpr ',' nexpr ',' nexpr ')'  */
#line 4695 "pars.yacc"
        {
            int c;
            CMap_entry cmap;
            cmap.rgb.red = (yyvsp[-5].ival);
            cmap.rgb.green = (yyvsp[-3].ival);
            cmap.rgb.blue = (yyvsp[-1].ival);
            cmap.ctype = COLOR_MAIN;
            cmap.cname = NULL;
            c = add_color(cmap);
            if (c == BAD_COLOR) {
                errmsg("Can't allocate requested color");
                c = 1;
            }
            (yyval.ival) = c;
        }
#line 13185 "y.tab.c"
    break;

  case 745: /* linew_select: LINEWIDTH expr  */
#line 4714 "pars.yacc"
        {
            double linew;
            linew = (yyvsp[0].dval);
            if (linew < 0.0) {
                yyerror("Negative linewidth");
                linew = 0.0;
            } else if (linew > MAX_LINEWIDTH) {
                yyerror("Linewidth too large");
                linew = MAX_LINEWIDTH;
            }
            (yyval.dval) = linew;
        }
#line 13202 "y.tab.c"
    break;

  case 746: /* opchoice_sel: PLACE opchoice  */
#line 4729 "pars.yacc"
        {
            (yyval.ival) = (yyvsp[0].ival);
        }
#line 13210 "y.tab.c"
    break;

  case 747: /* opchoice: NORMAL  */
#line 4734 "pars.yacc"
                 { (yyval.ival) = PLACEMENT_NORMAL; }
#line 13216 "y.tab.c"
    break;

  case 748: /* opchoice: OPPOSITE  */
#line 4735 "pars.yacc"
                   { (yyval.ival) = PLACEMENT_OPPOSITE; }
#line 13222 "y.tab.c"
    break;

  case 749: /* opchoice: BOTH  */
#line 4736 "pars.yacc"
               { (yyval.ival) = PLACEMENT_BOTH; }
#line 13228 "y.tab.c"
    break;

  case 750: /* parmset_obs: PAGE LAYOUT pageorient  */
#line 4742 "pars.yacc"
        {
            int wpp, hpp;
            if ((yyvsp[0].ival) == PAGE_ORIENT_LANDSCAPE) {
                wpp = 792;
                hpp = 612;
            } else {
                wpp = 612;
                hpp = 792;
            }
            set_page_dimensions(wpp, hpp, FALSE);
        }
#line 13244 "y.tab.c"
    break;

  case 751: /* parmset_obs: PAGE SIZE NUMBER NUMBER  */
#line 4753 "pars.yacc"
                                  {
            set_page_dimensions((int) (yyvsp[-1].dval), (int) (yyvsp[0].dval), FALSE);
        }
#line 13252 "y.tab.c"
    break;

  case 752: /* parmset_obs: PAGE nexpr  */
#line 4756 "pars.yacc"
                     {
	    scroll_proc((yyvsp[0].ival));
	}
#line 13260 "y.tab.c"
    break;

  case 753: /* parmset_obs: PAGE INOUT nexpr  */
#line 4759 "pars.yacc"
                           {
	    scrollinout_proc((yyvsp[0].ival));
	}
#line 13268 "y.tab.c"
    break;

  case 754: /* parmset_obs: DEFAULT FONTP SOURCE expr  */
#line 4763 "pars.yacc"
                                    {
	}
#line 13275 "y.tab.c"
    break;

  case 755: /* parmset_obs: STACK WORLD expr ',' expr ',' expr ',' expr TICKP expr ',' expr ',' expr ',' expr  */
#line 4767 "pars.yacc"
        {
	    add_world(whichgraph, (yyvsp[-14].dval), (yyvsp[-12].dval), (yyvsp[-10].dval), (yyvsp[-8].dval));
	}
#line 13283 "y.tab.c"
    break;

  case 756: /* parmset_obs: BOX FILL colpat_obs  */
#line 4771 "pars.yacc"
                              {filltype_obs = (yyvsp[0].ival);}
#line 13289 "y.tab.c"
    break;

  case 757: /* parmset_obs: ELLIPSE FILL colpat_obs  */
#line 4773 "pars.yacc"
                                  {filltype_obs = (yyvsp[0].ival);}
#line 13295 "y.tab.c"
    break;

  case 758: /* parmset_obs: STRING linew_select  */
#line 4775 "pars.yacc"
                              { }
#line 13301 "y.tab.c"
    break;

  case 759: /* parmset_obs: TIMESTAMP linew_select  */
#line 4777 "pars.yacc"
                                 { }
#line 13307 "y.tab.c"
    break;

  case 760: /* parmset_obs: TITLE linew_select  */
#line 4779 "pars.yacc"
                             { }
#line 13313 "y.tab.c"
    break;

  case 761: /* parmset_obs: SUBTITLE linew_select  */
#line 4780 "pars.yacc"
                                { }
#line 13319 "y.tab.c"
    break;

  case 762: /* parmset_obs: LEGEND BOX onoff  */
#line 4782 "pars.yacc"
                           {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    if ((yyvsp[0].ival) == FALSE && get_project_version() <= 40102) {
                g[whichgraph].l.boxpen.pattern = 0;
            }
	}
#line 13333 "y.tab.c"
    break;

  case 763: /* parmset_obs: LEGEND X1 expr  */
#line 4791 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legx = (yyvsp[0].dval);
	}
#line 13345 "y.tab.c"
    break;

  case 764: /* parmset_obs: LEGEND Y1 expr  */
#line 4798 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legy = (yyvsp[0].dval);
	}
#line 13357 "y.tab.c"
    break;

  case 765: /* parmset_obs: LEGEND STRING nexpr sexpr  */
#line 4805 "pars.yacc"
                                    {
	    if (is_valid_setno(whichgraph, (yyvsp[-1].ival))) {
                strncpy(g[whichgraph].p[(yyvsp[-1].ival)].lstr, (yyvsp[0].sval), MAX_STRING_LENGTH - 1);
	    } else {
                yyerror("Unallocated set");
            }
            xfree((yyvsp[0].sval));
	}
#line 13370 "y.tab.c"
    break;

  case 766: /* parmset_obs: LEGEND BOX FILL onoff  */
#line 4813 "pars.yacc"
                                { }
#line 13376 "y.tab.c"
    break;

  case 767: /* parmset_obs: LEGEND BOX FILL WITH colpat_obs  */
#line 4814 "pars.yacc"
                                          {filltype_obs = (yyvsp[0].ival);}
#line 13382 "y.tab.c"
    break;

  case 768: /* parmset_obs: LEGEND lines_select  */
#line 4815 "pars.yacc"
                              { }
#line 13388 "y.tab.c"
    break;

  case 769: /* parmset_obs: LEGEND linew_select  */
#line 4816 "pars.yacc"
                              { }
#line 13394 "y.tab.c"
    break;

  case 770: /* parmset_obs: selectgraph LABEL onoff  */
#line 4818 "pars.yacc"
                                  { }
#line 13400 "y.tab.c"
    break;

  case 771: /* parmset_obs: selectgraph TYPE LOGX  */
#line 4820 "pars.yacc"
                                { 
	    g[(yyvsp[-2].ival)].type = GRAPH_XY;
	    g[(yyvsp[-2].ival)].xscale = SCALE_LOG;
	}
#line 13409 "y.tab.c"
    break;

  case 772: /* parmset_obs: selectgraph TYPE LOGY  */
#line 4824 "pars.yacc"
                                { 
	    g[(yyvsp[-2].ival)].type = GRAPH_XY;
	    g[(yyvsp[-2].ival)].yscale = SCALE_LOG;
	}
#line 13418 "y.tab.c"
    break;

  case 773: /* parmset_obs: selectgraph TYPE LOGXY  */
#line 4829 "pars.yacc"
        { 
	    g[(yyvsp[-2].ival)].type = GRAPH_XY;
	    g[(yyvsp[-2].ival)].xscale = SCALE_LOG;
	    g[(yyvsp[-2].ival)].yscale = SCALE_LOG;
	}
#line 13428 "y.tab.c"
    break;

  case 774: /* parmset_obs: selectgraph TYPE BAR  */
#line 4835 "pars.yacc"
        { 
	    g[(yyvsp[-2].ival)].type = GRAPH_CHART;
	    g[(yyvsp[-2].ival)].xyflip = FALSE;
	    g[(yyvsp[-2].ival)].stacked = FALSE;
	}
#line 13438 "y.tab.c"
    break;

  case 775: /* parmset_obs: selectgraph TYPE HBAR  */
#line 4841 "pars.yacc"
        { 
	    g[(yyvsp[-2].ival)].type = GRAPH_CHART;
	    g[(yyvsp[-2].ival)].xyflip = TRUE;
	}
#line 13447 "y.tab.c"
    break;

  case 776: /* parmset_obs: selectgraph TYPE STACKEDBAR  */
#line 4846 "pars.yacc"
        { 
	    g[(yyvsp[-2].ival)].type = GRAPH_CHART;
	    g[(yyvsp[-2].ival)].stacked = TRUE;
	}
#line 13456 "y.tab.c"
    break;

  case 777: /* parmset_obs: selectgraph TYPE STACKEDHBAR  */
#line 4851 "pars.yacc"
        { 
	    g[(yyvsp[-2].ival)].type = GRAPH_CHART;
	    g[(yyvsp[-2].ival)].stacked = TRUE;
	    g[(yyvsp[-2].ival)].xyflip = TRUE;
	}
#line 13466 "y.tab.c"
    break;

  case 778: /* parmset_obs: WORLD XMIN expr  */
#line 4857 "pars.yacc"
                          {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg1 = (yyvsp[0].dval);
	}
#line 13478 "y.tab.c"
    break;

  case 779: /* parmset_obs: WORLD XMAX expr  */
#line 4864 "pars.yacc"
                          {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg2 = (yyvsp[0].dval);
	}
#line 13490 "y.tab.c"
    break;

  case 780: /* parmset_obs: WORLD YMIN expr  */
#line 4871 "pars.yacc"
                          {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.yg1 = (yyvsp[0].dval);
	}
#line 13502 "y.tab.c"
    break;

  case 781: /* parmset_obs: WORLD YMAX expr  */
#line 4878 "pars.yacc"
                          {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.yg2 = (yyvsp[0].dval);
	}
#line 13514 "y.tab.c"
    break;

  case 782: /* parmset_obs: VIEW XMIN expr  */
#line 4886 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv1 = (yyvsp[0].dval);
	}
#line 13526 "y.tab.c"
    break;

  case 783: /* parmset_obs: VIEW XMAX expr  */
#line 4893 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv2 = (yyvsp[0].dval);
	}
#line 13538 "y.tab.c"
    break;

  case 784: /* parmset_obs: VIEW YMIN expr  */
#line 4900 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.yv1 = (yyvsp[0].dval);
	}
#line 13550 "y.tab.c"
    break;

  case 785: /* parmset_obs: VIEW YMAX expr  */
#line 4907 "pars.yacc"
                         {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.yv2 = (yyvsp[0].dval);
	}
#line 13562 "y.tab.c"
    break;

  case 786: /* parmset_obs: LEGEND LAYOUT expr  */
#line 4915 "pars.yacc"
                             {
	}
#line 13569 "y.tab.c"
    break;

  case 787: /* parmset_obs: FRAMEP FILL onoff  */
#line 4918 "pars.yacc"
                            { 
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.pattern = (yyvsp[0].ival);
        }
#line 13581 "y.tab.c"
    break;

  case 788: /* parmset_obs: selectgraph AUTOSCALE TYPE AUTO  */
#line 4926 "pars.yacc"
                                          {
        }
#line 13588 "y.tab.c"
    break;

  case 789: /* parmset_obs: selectgraph AUTOSCALE TYPE SPEC  */
#line 4928 "pars.yacc"
                                          {
        }
#line 13595 "y.tab.c"
    break;

  case 790: /* parmset_obs: LINE ARROW SIZE expr  */
#line 4931 "pars.yacc"
                               {
	    line_asize = 2.0*(yyvsp[0].dval);
	}
#line 13603 "y.tab.c"
    break;

  case 791: /* parmset_obs: HARDCOPY DEVICE expr  */
#line 4935 "pars.yacc"
                               { }
#line 13609 "y.tab.c"
    break;

  case 792: /* parmset_obs: PS LINEWIDTH BEGIN expr  */
#line 4936 "pars.yacc"
                                  { }
#line 13615 "y.tab.c"
    break;

  case 793: /* parmset_obs: PS LINEWIDTH INCREMENT expr  */
#line 4937 "pars.yacc"
                                      { }
#line 13621 "y.tab.c"
    break;

  case 794: /* parmset_obs: PS linew_select  */
#line 4938 "pars.yacc"
                          { }
#line 13627 "y.tab.c"
    break;

  case 795: /* axislabeldesc_obs: linew_select  */
#line 4943 "pars.yacc"
                     { }
#line 13633 "y.tab.c"
    break;

  case 796: /* axislabeldesc_obs: opchoice_sel_obs  */
#line 4944 "pars.yacc"
                           {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_op = (yyvsp[0].ival);
	}
#line 13645 "y.tab.c"
    break;

  case 797: /* setprop_obs: selectset SYMBOL FILL nexpr  */
#line 4954 "pars.yacc"
                                    {
	    switch ((yyvsp[0].ival)){
	    case 0:
	        g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symfillpen.pattern = 0;
	        break;
	    case 1:
	        g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symfillpen.pattern = 1;
	        break;
	    case 2:
	        g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symfillpen.pattern = 1;
	        g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].symfillpen.color = getbgcolor();
	        break;
	    }
	}
#line 13664 "y.tab.c"
    break;

  case 798: /* setprop_obs: selectset SKIP nexpr  */
#line 4969 "pars.yacc"
        {
	    g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].symskip = (yyvsp[0].ival);
	}
#line 13672 "y.tab.c"
    break;

  case 799: /* setprop_obs: selectset FILL nexpr  */
#line 4973 "pars.yacc"
        {
	    switch ((yyvsp[0].ival)) {
            case 0:
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].filltype = SETFILL_NONE;
                break;
            case 1:
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].filltype = SETFILL_POLYGON;
                break;
            case 2:
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].filltype = SETFILL_BASELINE;
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].baseline_type = BASELINE_TYPE_0;
                break;
            case 6:
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].filltype = SETFILL_BASELINE;
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].baseline_type = BASELINE_TYPE_GMIN;
                break;
            case 7:
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].filltype = SETFILL_BASELINE;
                g[(yyvsp[-2].trgt)->gno].p[(yyvsp[-2].trgt)->setno].baseline_type = BASELINE_TYPE_GMAX;
                break;
            }
	}
#line 13699 "y.tab.c"
    break;

  case 800: /* setprop_obs: selectset ERRORBAR TYPE opchoice_obs  */
#line 4995 "pars.yacc"
                                               {
	    g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].errbar.ptype = (yyvsp[0].ival);
	}
#line 13707 "y.tab.c"
    break;

  case 801: /* setprop_obs: selectset SYMBOL CENTER onoff  */
#line 5003 "pars.yacc"
                                        { }
#line 13713 "y.tab.c"
    break;

  case 802: /* setprop_obs: selectset lines_select  */
#line 5004 "pars.yacc"
                                 {
	    g[(yyvsp[-1].trgt)->gno].p[(yyvsp[-1].trgt)->setno].lines = (yyvsp[0].ival);
	}
#line 13721 "y.tab.c"
    break;

  case 803: /* setprop_obs: selectset linew_select  */
#line 5007 "pars.yacc"
                                 {
	    g[(yyvsp[-1].trgt)->gno].p[(yyvsp[-1].trgt)->setno].linew = (yyvsp[0].dval);
	}
#line 13729 "y.tab.c"
    break;

  case 804: /* setprop_obs: selectset color_select  */
#line 5010 "pars.yacc"
                                 {
	    g[(yyvsp[-1].trgt)->gno].p[(yyvsp[-1].trgt)->setno].linepen.color = (yyvsp[0].ival);
	}
#line 13737 "y.tab.c"
    break;

  case 805: /* setprop_obs: selectset FILL WITH colpat_obs  */
#line 5013 "pars.yacc"
                                         {filltype_obs = (yyvsp[0].ival);}
#line 13743 "y.tab.c"
    break;

  case 806: /* setprop_obs: selectset XYZ expr ',' expr  */
#line 5014 "pars.yacc"
                                      { }
#line 13749 "y.tab.c"
    break;

  case 807: /* setprop_obs: selectset ERRORBAR LENGTH expr  */
#line 5015 "pars.yacc"
                                         {
            g[(yyvsp[-3].trgt)->gno].p[(yyvsp[-3].trgt)->setno].errbar.barsize = (yyvsp[0].dval);
	}
#line 13757 "y.tab.c"
    break;

  case 808: /* setprop_obs: selectset ERRORBAR RISER onoff  */
#line 5018 "pars.yacc"
                                         { }
#line 13763 "y.tab.c"
    break;

  case 809: /* tickattr_obs: MAJOR onoff  */
#line 5023 "pars.yacc"
                    {
	    /* <= xmgr-4.1 */
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->active = (yyvsp[0].ival);
	}
#line 13776 "y.tab.c"
    break;

  case 810: /* tickattr_obs: MINOR onoff  */
#line 5031 "pars.yacc"
                      { }
#line 13782 "y.tab.c"
    break;

  case 811: /* tickattr_obs: ALT onoff  */
#line 5032 "pars.yacc"
                      { }
#line 13788 "y.tab.c"
    break;

  case 812: /* tickattr_obs: MINP NUMBER  */
#line 5033 "pars.yacc"
                        { }
#line 13794 "y.tab.c"
    break;

  case 813: /* tickattr_obs: MAXP NUMBER  */
#line 5034 "pars.yacc"
                        { }
#line 13800 "y.tab.c"
    break;

  case 814: /* tickattr_obs: LOG onoff  */
#line 5035 "pars.yacc"
                      { }
#line 13806 "y.tab.c"
    break;

  case 815: /* tickattr_obs: TYPE AUTO  */
#line 5036 "pars.yacc"
                    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_NONE;
	}
#line 13818 "y.tab.c"
    break;

  case 816: /* tickattr_obs: TYPE SPEC  */
#line 5043 "pars.yacc"
                    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if (g[whichgraph].t[naxis]->t_spec != TICKS_SPEC_BOTH) {
                g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_MARKS;
            }
	}
#line 13832 "y.tab.c"
    break;

  case 817: /* tickattr_obs: MINOR expr  */
#line 5052 "pars.yacc"
                     {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if ((yyvsp[0].dval) != 0.0) {
                g[whichgraph].t[naxis]->nminor = 
                            (int) rint(g[whichgraph].t[naxis]->tmajor / (yyvsp[0].dval) - 1);
            } else {
                g[whichgraph].t[naxis]->nminor = 0;
            }
	}
#line 13849 "y.tab.c"
    break;

  case 818: /* tickattr_obs: SIZE expr  */
#line 5064 "pars.yacc"
                    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.size = (yyvsp[0].dval);
	}
#line 13861 "y.tab.c"
    break;

  case 819: /* tickattr_obs: nexpr ',' expr  */
#line 5071 "pars.yacc"
                         {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].wtpos = (yyvsp[0].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[-2].ival)].type = TICK_TYPE_MAJOR;
	}
#line 13874 "y.tab.c"
    break;

  case 820: /* tickattr_obs: opchoice_sel_obs  */
#line 5079 "pars.yacc"
                           {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_op = (yyvsp[0].ival);
	}
#line 13886 "y.tab.c"
    break;

  case 821: /* ticklabelattr_obs: linew_select  */
#line 5089 "pars.yacc"
                     { }
#line 13892 "y.tab.c"
    break;

  case 822: /* ticklabelattr_obs: TYPE AUTO  */
#line 5090 "pars.yacc"
                    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if (g[whichgraph].t[naxis]->t_spec == TICKS_SPEC_BOTH) {
                g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_MARKS;
            }
	}
#line 13906 "y.tab.c"
    break;

  case 823: /* ticklabelattr_obs: TYPE SPEC  */
#line 5099 "pars.yacc"
                    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_BOTH;
	}
#line 13918 "y.tab.c"
    break;

  case 824: /* ticklabelattr_obs: LAYOUT SPEC  */
#line 5106 "pars.yacc"
                      { }
#line 13924 "y.tab.c"
    break;

  case 825: /* ticklabelattr_obs: LAYOUT HORIZONTAL  */
#line 5108 "pars.yacc"
                            {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = 0;
	}
#line 13936 "y.tab.c"
    break;

  case 826: /* ticklabelattr_obs: LAYOUT VERTICAL  */
#line 5115 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = 90;
	}
#line 13948 "y.tab.c"
    break;

  case 827: /* ticklabelattr_obs: PLACE ON TICKSP  */
#line 5122 "pars.yacc"
                          { }
#line 13954 "y.tab.c"
    break;

  case 828: /* ticklabelattr_obs: PLACE BETWEEN TICKSP  */
#line 5123 "pars.yacc"
                               { }
#line 13960 "y.tab.c"
    break;

  case 829: /* ticklabelattr_obs: opchoice_sel_obs  */
#line 5124 "pars.yacc"
                           {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_op = (yyvsp[0].ival);
	}
#line 13972 "y.tab.c"
    break;

  case 830: /* ticklabelattr_obs: SIGN signchoice  */
#line 5131 "pars.yacc"
                          {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    switch((yyvsp[0].ival)) {
            case SIGN_NEGATE:
                g[whichgraph].t[naxis]->tl_formula =
                    copy_string(g[whichgraph].t[naxis]->tl_formula, "-$t");
                break;
            case SIGN_ABSOLUTE:
                g[whichgraph].t[naxis]->tl_formula =
                    copy_string(g[whichgraph].t[naxis]->tl_formula, "abs($t)");
                break;
            default:
                g[whichgraph].t[naxis]->tl_formula =
                    copy_string(g[whichgraph].t[naxis]->tl_formula, NULL);
                break;
            }
	}
#line 13997 "y.tab.c"
    break;

  case 834: /* opchoice_sel_obs: OP opchoice_obs  */
#line 5159 "pars.yacc"
        {
            (yyval.ival) = (yyvsp[0].ival);
        }
#line 14005 "y.tab.c"
    break;

  case 835: /* opchoice_obs: TOP  */
#line 5164 "pars.yacc"
                  { (yyval.ival) = PLACEMENT_OPPOSITE; }
#line 14011 "y.tab.c"
    break;

  case 836: /* opchoice_obs: BOTTOM  */
#line 5165 "pars.yacc"
                 { (yyval.ival) = PLACEMENT_NORMAL; }
#line 14017 "y.tab.c"
    break;

  case 837: /* opchoice_obs: LEFT  */
#line 5166 "pars.yacc"
               { (yyval.ival) = PLACEMENT_NORMAL; }
#line 14023 "y.tab.c"
    break;

  case 838: /* opchoice_obs: RIGHT  */
#line 5167 "pars.yacc"
                { (yyval.ival) = PLACEMENT_OPPOSITE; }
#line 14029 "y.tab.c"
    break;

  case 839: /* opchoice_obs: BOTH  */
#line 5168 "pars.yacc"
               { (yyval.ival) = PLACEMENT_BOTH; }
#line 14035 "y.tab.c"
    break;


#line 14039 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 5171 "pars.yacc"


/* list of intrinsic functions and keywords */
symtab_entry ikey[] = {
	{"A0", FITPARM, NULL},
	{"A0MAX", FITPMAX, NULL},
	{"A0MIN", FITPMIN, NULL},
	{"A1", FITPARM, NULL},
	{"A1MAX", FITPMAX, NULL},
	{"A1MIN", FITPMIN, NULL},
	{"A2", FITPARM, NULL},
	{"A2MAX", FITPMAX, NULL},
	{"A2MIN", FITPMIN, NULL},
	{"A3", FITPARM, NULL},
	{"A3MAX", FITPMAX, NULL},
	{"A3MIN", FITPMIN, NULL},
	{"A4", FITPARM, NULL},
	{"A4MAX", FITPMAX, NULL},
	{"A4MIN", FITPMIN, NULL},
	{"A5", FITPARM, NULL},
	{"A5MAX", FITPMAX, NULL},
	{"A5MIN", FITPMIN, NULL},
	{"A6", FITPARM, NULL},
	{"A6MAX", FITPMAX, NULL},
	{"A6MIN", FITPMIN, NULL},
	{"A7", FITPARM, NULL},
	{"A7MAX", FITPMAX, NULL},
	{"A7MIN", FITPMIN, NULL},
	{"A8", FITPARM, NULL},
	{"A8MAX", FITPMAX, NULL},
	{"A8MIN", FITPMIN, NULL},
	{"A9", FITPARM, NULL},
	{"A9MAX", FITPMAX, NULL},
	{"A9MIN", FITPMIN, NULL},
	{"ABOVE", ABOVE, NULL},
	{"ABS", FUNC_D, (void *) fabs},
	{"ABSOLUTE", ABSOLUTE, NULL},
	{"ACOS", FUNC_D, (void *) acos},
	{"ACOSH", FUNC_D, (void *) acosh},
	{"AI", FUNC_D, (void *) ai_wrap},
	{"ALIAS", ALIAS, NULL},
	{"ALT", ALT, NULL},
	{"ALTXAXIS", ALTXAXIS, NULL},
	{"ALTYAXIS", ALTYAXIS, NULL},
	{"AND", AND, NULL},
	{"ANGLE", ANGLE, NULL},
	{"ANTIALIASING", ANTIALIASING, NULL},
	{"APPEND", APPEND, NULL},
	{"ARRANGE", ARRANGE, NULL},
	{"ARROW", ARROW, NULL},
	{"ASCENDING", ASCENDING, NULL},
	{"ASIN", FUNC_D, (void *) asin},
	{"ASINH", FUNC_D, (void *) asinh},
	{"ASPLINE", ASPLINE, NULL},
	{"ATAN", FUNC_D, (void *) atan},
	{"ATAN2", FUNC_DD, (void *) atan2},
	{"ATANH", FUNC_D, (void *) atanh},
	{"AUTO", AUTO, NULL},
	{"AUTOSCALE", AUTOSCALE, NULL},
	{"AUTOTICKS", AUTOTICKS, NULL},
	{"AVALUE", AVALUE, NULL},
	{"AVG", AVG, NULL},
	{"BACKGROUND", BACKGROUND, NULL},
	{"BAR", BAR, NULL},
	{"BARDY", BARDY, NULL},
	{"BARDYDY", BARDYDY, NULL},
	{"BASELINE", BASELINE, NULL},
	{"BATCH", BATCH, NULL},
        {"BEGIN", BEGIN, NULL},
	{"BELOW", BELOW, NULL},
	{"BETA", FUNC_DD, (void *) beta},
	{"BETWEEN", BETWEEN, NULL},
	{"BI", FUNC_D, (void *) bi_wrap},
	{"BLACKMAN", BLACKMAN, NULL},
	{"BLOCK", BLOCK, NULL},
	{"BOTH", BOTH, NULL},
	{"BOTTOM", BOTTOM, NULL},
	{"BOX", BOX, NULL},
	{"CD", CD, NULL},
	{"CEIL", FUNC_D, (void *) ceil},
	{"CENTER", CENTER, NULL},
	{"CHAR", CHAR, NULL},
	{"CHART", CHART, NULL},
	{"CHDTR", FUNC_DD, (void *) chdtr},
	{"CHDTRC", FUNC_DD, (void *) chdtrc},
	{"CHDTRI", FUNC_DD, (void *) chdtri},
	{"CHI", FUNC_D, (void *) chi_wrap},
	{"CI", FUNC_D, (void *) ci_wrap},
	{"CLEAR", CLEAR, NULL},
	{"CLICK", CLICK, NULL},
	{"CLIP", CLIP, NULL},
	{"CLOSE", CLOSE, NULL},
	{"COEFFICIENTS", COEFFICIENTS, NULL},
	{"COLOR", COLOR, NULL},
	{"COMMENT", COMMENT, NULL},
	{"COMPLEX", COMPLEX, NULL},
	{"COMPUTING", COMPUTING, NULL},
	{"CONST", KEY_CONST, NULL},
	{"CONSTRAINTS", CONSTRAINTS, NULL},
	{"COPY", COPY, NULL},
	{"COS", FUNC_D, (void *) cos},
	{"COSH", FUNC_D, (void *) cosh},
	{"CYCLE", CYCLE, NULL},
	{"DATE", DATE, NULL},
	{"DAWSN", FUNC_D, (void *) dawsn},
	{"DAYMONTH", DAYMONTH, NULL},
	{"DAYOFWEEKL", DAYOFWEEKL, NULL},
	{"DAYOFWEEKS", DAYOFWEEKS, NULL},
	{"DAYOFYEAR", DAYOFYEAR, NULL},
	{"DDMMYY", DDMMYY, NULL},
	{"DECIMAL", DECIMAL, NULL},
	{"DEF", DEF, NULL},
	{"DEFAULT", DEFAULT, NULL},
	{"DEFINE", DEFINE, NULL},
	{"DEG", UCONSTANT, (void *) deg_uconst},
	{"DEGREESLAT", DEGREESLAT, NULL},
	{"DEGREESLON", DEGREESLON, NULL},
	{"DEGREESMMLAT", DEGREESMMLAT, NULL},
	{"DEGREESMMLON", DEGREESMMLON, NULL},
	{"DEGREESMMSSLAT", DEGREESMMSSLAT, NULL},
	{"DEGREESMMSSLON", DEGREESMMSSLON, NULL},
	{"DESCENDING", DESCENDING, NULL},
	{"DESCRIPTION", DESCRIPTION, NULL},
	{"DEVICE", DEVICE, NULL},
	{"DFT", DFT, NULL},
	{"DIFF", DIFFERENCE, NULL},
	{"DIFFERENCE", DIFFERENCE, NULL},
	{"DISK", DISK, NULL},
	{"DOWN", DOWN, NULL},
	{"DPI", DPI, NULL},
	{"DROP", DROP, NULL},
	{"DROPLINE", DROPLINE, NULL},
	{"ECHO", ECHO, NULL},
	{"ELLIE", FUNC_DD, (void *) ellie},
	{"ELLIK", FUNC_DD, (void *) ellik},
	{"ELLIPSE", ELLIPSE, NULL},
	{"ELLPE", FUNC_D, (void *) ellpe_wrap},
	{"ELLPK", FUNC_D, (void *) ellpk_wrap},
	{"ENGINEERING", ENGINEERING, NULL},
	{"EQ", EQ, NULL},
	{"ER", ERRORBAR, NULL},
	{"ERF", FUNC_D, (void *) erf},
	{"ERFC", FUNC_D, (void *) erfc},
	{"ERRORBAR", ERRORBAR, NULL},
	{"EXIT", EXIT, NULL},
	{"EXP", FUNC_D, (void *) exp},
	{"EXPN", FUNC_ND, (void *) expn},
	{"EXPONENTIAL", EXPONENTIAL, NULL},
	{"FAC", FUNC_I, (void *) fac},
	{"FALSE", OFF, NULL},
	{"FDTR", FUNC_NND, (void *) fdtr},
	{"FDTRC", FUNC_NND, (void *) fdtrc},
	{"FDTRI", FUNC_NND, (void *) fdtri},
	{"FFT", FFT, NULL},
	{"FILE", FILEP, NULL},
	{"FILL", FILL, NULL},
	{"FIT", FIT, NULL},
	{"FIXED", FIXED, NULL},
	{"FIXEDPOINT", FIXEDPOINT, NULL},
	{"FLOOR", FUNC_D, (void *) floor},
	{"FLUSH", FLUSH, NULL},
	{"FOCUS", FOCUS, NULL},
	{"FOLLOWS", FOLLOWS, NULL},
	{"FONT", FONTP, NULL},
	{"FORCE", FORCE, NULL},
	{"FORMAT", FORMAT, NULL},
	{"FORMULA", FORMULA, NULL},
	{"FRAME", FRAMEP, NULL},
	{"FREE", FREE, NULL},
	{"FREQUENCY", FREQUENCY, NULL},
	{"FRESNLC", FUNC_D, (void *) fresnlc_wrap},
	{"FRESNLS", FUNC_D, (void *) fresnls_wrap},
	{"FROM", FROM, NULL},
	{"F_OF_D", KEY_FUNC_D, NULL},
	{"F_OF_DD", KEY_FUNC_DD, NULL},
        {"F_OF_I", KEY_FUNC_I, NULL},
	{"F_OF_ND", KEY_FUNC_ND, NULL},
	{"F_OF_NN", KEY_FUNC_NN, NULL},
	{"F_OF_NND", KEY_FUNC_NND, NULL},
	{"F_OF_PPD", KEY_FUNC_PPD, NULL},
	{"F_OF_PPPD", KEY_FUNC_PPPD, NULL},
	{"F_OF_PPPPD", KEY_FUNC_PPPPD, NULL},
	{"F_OF_PPPPPD", KEY_FUNC_PPPPPD, NULL},
	{"GAMMA", FUNC_D, (void *) true_gamma},
	{"GDTR", FUNC_PPD, (void *) gdtr},
	{"GDTRC", FUNC_PPD, (void *) gdtrc},
	{"GE", GE, NULL},
	{"GENERAL", GENERAL, NULL},
	{"GETP", GETP, NULL},
	{"GRAPH", GRAPH, NULL},
	{"GRID", GRID, NULL},
	{"GT", GT, NULL},
	{"HAMMING", HAMMING, NULL},
	{"HANNING", HANNING, NULL},
	{"HARDCOPY", HARDCOPY, NULL},
	{"HBAR", HBAR, NULL},
	{"HELP", HELP, NULL},
	{"HGAP", HGAP, NULL},
	{"HIDDEN", HIDDEN, NULL},
	{"HISTOGRAM", HISTOGRAM, NULL},
	{"HMS", HMS, NULL},
	{"HORIZI", HORIZI, NULL},
	{"HORIZO", HORIZO, NULL},
	{"HORIZONTAL", HORIZONTAL, NULL},
	{"HYP2F1", FUNC_PPPD, (void *) hyp2f1},
	{"HYPERG", FUNC_PPD, (void *) hyperg},
	{"HYPOT", FUNC_DD, (void *) hypot},
	{"I0E", FUNC_D, (void *) i0e},
	{"I1E", FUNC_D, (void *) i1e},
	{"ID", ID, NULL},
	{"IFILTER", IFILTER, NULL},
	{"IGAM", FUNC_DD, (void *) igam},
	{"IGAMC", FUNC_DD, (void *) igamc},
	{"IGAMI", FUNC_DD, (void *) igami},
	{"IMAX", IMAX, NULL},
	{"IMIN", IMIN, NULL},
	{"IN", IN, NULL},
	{"INCBET", FUNC_PPD, (void *) incbet},
	{"INCBI", FUNC_PPD, (void *) incbi},
	{"INCREMENT", INCREMENT, NULL},
	{"INDEX", INDEX, NULL},
	{"INOUT", INOUT, NULL},
	{"INT", INT, NULL},
	{"INTEGRATE", INTEGRATE, NULL},
	{"INTERPOLATE", INTERPOLATE, NULL},
	{"INVDFT", INVDFT, NULL},
	{"INVERT", INVERT, NULL},
	{"INVFFT", INVFFT, NULL},
	{"IRAND", FUNC_I, (void *) irand_wrap},
	{"IV", FUNC_DD, (void *) iv_wrap},
	{"JUST", JUST, NULL},
	{"JV", FUNC_DD, (void *) jv_wrap},
	{"K0E", FUNC_D, (void *) k0e},
	{"K1E", FUNC_D, (void *) k1e},
	{"KILL", KILL, NULL},
	{"KN", FUNC_ND, (void *) kn_wrap},
	{"LABEL", LABEL, NULL},
	{"LANDSCAPE", LANDSCAPE, NULL},
	{"LAYOUT", LAYOUT, NULL},
	{"LBETA", FUNC_DD, (void *) lbeta},
	{"LE", LE, NULL},
	{"LEFT", LEFT, NULL},
	{"LEGEND", LEGEND, NULL},
	{"LENGTH", LENGTH, NULL},
	{"LGAMMA", FUNC_D, (void *) lgamma},
	{"LINCONV", LINCONV, NULL},
	{"LINE", LINE, NULL},
	{"LINEAR", LINEAR, NULL},
	{"LINESTYLE", LINESTYLE, NULL},
	{"LINEWIDTH", LINEWIDTH, NULL},
	{"LINK", LINK, NULL},
	{"LN", FUNC_D, (void *) log},
	{"LOAD", LOAD, NULL},
	{"LOCTYPE", LOCTYPE, NULL},
	{"LOG", LOG, NULL},
	{"LOG10", FUNC_D, (void *) log10},
	{"LOG2", FUNC_D, (void *) log2},
	{"LOGARITHMIC", LOGARITHMIC, NULL},
	{"LOGX", LOGX, NULL},
	{"LOGXY", LOGXY, NULL},
	{"LOGY", LOGY, NULL},
	{"LOGIT", LOGIT, NULL},
	{"LT", LT, NULL},
	{"MAGIC", MAGIC, NULL},
	{"MAGNITUDE", MAGNITUDE, NULL},
	{"MAJOR", MAJOR, NULL},
	{"MAP", MAP, NULL},
	{"MAX", MAXP, NULL},
	{"MAXOF", FUNC_DD, (void *) max_wrap},
	{"MESH", MESH, NULL},
	{"MIN", MINP, NULL},
	{"MINOF", FUNC_DD, (void *) min_wrap},
	{"MINOR", MINOR, NULL},
	{"MMDD", MMDD, NULL},
	{"MMDDHMS", MMDDHMS, NULL},
	{"MMDDYY", MMDDYY, NULL},
	{"MMDDYYHMS", MMDDYYHMS, NULL},
	{"MMSSLAT", MMSSLAT, NULL},
	{"MMSSLON", MMSSLON, NULL},
	{"MMYY", MMYY, NULL},
	{"MOD", FUNC_DD, (void *) fmod},
	{"MONTHDAY", MONTHDAY, NULL},
	{"MONTHL", MONTHL, NULL},
	{"MONTHS", MONTHS, NULL},
	{"MONTHSY", MONTHSY, NULL},
	{"MOVE", MOVE, NULL},
	{"NDTR", FUNC_D, (void *) ndtr},
	{"NDTRI", FUNC_D, (void *) ndtri},
	{"NE", NE, NULL},
	{"NEGATE", NEGATE, NULL},
	{"NEW", NEW, NULL},
	{"NONE", NONE, NULL},
	{"NONLFIT", NONLFIT, NULL},
	{"NORM", FUNC_D, (void *) fx},
	{"NORMAL", NORMAL, NULL},
	{"NOT", NOT, NULL},
	{"NXY", NXY, NULL},
	{"OFF", OFF, NULL},
	{"OFFSET", OFFSET, NULL},
	{"OFFSETX", OFFSETX, NULL},
	{"OFFSETY", OFFSETY, NULL},
	{"OFILTER", OFILTER, NULL},
	{"ON", ON, NULL},
	{"ONREAD", ONREAD, NULL},
	{"OP", OP, NULL},
	{"OPPOSITE", OPPOSITE, NULL},
	{"OR", OR, NULL},
	{"OUT", OUT, NULL},
	{"PAGE", PAGE, NULL},
	{"PARA", PARA, NULL},
	{"PARAMETERS", PARAMETERS, NULL},
	{"PARZEN", PARZEN, NULL},
	{"PATTERN", PATTERN, NULL},
	{"PDTR", FUNC_ND, (void *) pdtr},
	{"PDTRC", FUNC_ND, (void *) pdtrc},
	{"PDTRI", FUNC_ND, (void *) pdtri},
	{"PERIOD", PERIOD, NULL},
	{"PERP", PERP, NULL},
	{"PHASE", PHASE, NULL},
	{"PI", CONSTANT, (void *) pi_const},
	{"PIE", PIE, NULL},
	{"PIPE", PIPE, NULL},
	{"PLACE", PLACE, NULL},
	{"POINT", POINT, NULL},
	{"POLAR", POLAR, NULL},
	{"POLYI", POLYI, NULL},
	{"POLYO", POLYO, NULL},
	{"POP", POP, NULL},
	{"PORTRAIT", PORTRAIT, NULL},
	{"POWER", POWER, NULL},
	{"PREC", PREC, NULL},
	{"PREPEND", PREPEND, NULL},
	{"PRINT", PRINT, NULL},
	{"PS", PS, NULL},
	{"PSI", FUNC_D, (void *) psi},
	{"PUSH", PUSH, NULL},
	{"PUTP", PUTP, NULL},
	{"RAD", UCONSTANT, (void *) rad_uconst},
	{"RAND", RAND, NULL},
	{"READ", READ, NULL},
	{"REAL", REAL, NULL},
	{"RECIPROCAL", RECIPROCAL, NULL},
	{"REDRAW", REDRAW, NULL},
	{"REFERENCE", REFERENCE, NULL},
	{"REGRESS", REGRESS, NULL},
	{"RESIZE", RESIZE, NULL},
	{"RESTRICT", RESTRICT, NULL},
	{"REVERSE", REVERSE, NULL},
	{"RGAMMA", FUNC_D, (void *) rgamma},
	{"RIGHT", RIGHT, NULL},
	{"RINT", FUNC_D, (void *) rint},
	{"RISER", RISER, NULL},
	{"RNORM", FUNC_DD, (void *) rnorm},
	{"ROT", ROT, NULL},
	{"ROUNDED", ROUNDED, NULL},
	{"RSUM", RSUM, NULL},
	{"RULE", RULE, NULL},
	{"RUNAVG", RUNAVG, NULL},
	{"RUNMAX", RUNMAX, NULL},
	{"RUNMED", RUNMED, NULL},
	{"RUNMIN", RUNMIN, NULL},
	{"RUNSTD", RUNSTD, NULL},
	{"SAVEALL", SAVEALL, NULL},
	{"SCALE", SCALE, NULL},
	{"SCIENTIFIC", SCIENTIFIC, NULL},
	{"SCROLL", SCROLL, NULL},
	{"SD", SD, NULL},
	{"SET", SET, NULL},
	{"SFORMAT", SFORMAT, NULL},
	{"SGN", FUNC_D, (void *) sign_wrap},
	{"SHI", FUNC_D, (void *) shi_wrap},
	{"SI", FUNC_D, (void *) si_wrap},
	{"SIGN", SIGN, NULL},
	{"SIN", FUNC_D, (void *) sin},
	{"SINH", FUNC_D, (void *) sinh},
	{"SIZE", SIZE, NULL},
	{"SKIP", SKIP, NULL},
	{"SLEEP", SLEEP, NULL},
	{"SMITH", SMITH, NULL},
	{"SORT", SORT, NULL},
	{"SOURCE", SOURCE, NULL},
	{"SPEC", SPEC, NULL},
	{"SPENCE", FUNC_D, (void *) spence},
	{"SPLINE", SPLINE, NULL},
	{"SPLIT", SPLIT, NULL},
	{"SQR", FUNC_D, (void *) sqr_wrap},
	{"SQRT", FUNC_D, (void *) sqrt},
	{"STACK", STACK, NULL},
	{"STACKED", STACKED, NULL},
	{"STACKEDBAR", STACKEDBAR, NULL},
	{"STACKEDHBAR", STACKEDHBAR, NULL},
	{"STAGGER", STAGGER, NULL},
	{"START", START, NULL},
	{"STDTR", FUNC_ND, (void *) stdtr},
	{"STDTRI", FUNC_ND, (void *) stdtri},
	{"STOP", STOP, NULL},
	{"STRING", STRING, NULL},
	{"STRUVE", FUNC_DD, (void *) struve},
	{"SUBTITLE", SUBTITLE, NULL},
	{"SUM", SUM, NULL},
	{"SWAP", SWAP, NULL},
	{"SYMBOL", SYMBOL, NULL},
	{"TAN", FUNC_D, (void *) tan},
	{"TANH", FUNC_D, (void *) tanh},
	{"TARGET", TARGET, NULL},
	{"TICK", TICKP, NULL},
	{"TICKLABEL", TICKLABEL, NULL},
	{"TICKS", TICKSP, NULL},
	{"TIMER", TIMER, NULL},
	{"TIMESTAMP", TIMESTAMP, NULL},
	{"TITLE", TITLE, NULL},
	{"TO", TO, NULL},
	{"TOP", TOP, NULL},
	{"TRIANGULAR", TRIANGULAR, NULL},
	{"TRUE", ON, NULL},
	{"TYPE", TYPE, NULL},
	{"UNIT", KEY_UNIT, NULL},
	{"UP", UP, NULL},
	{"UPDATEALL", UPDATEALL, NULL},
	{"USE", USE, NULL},
	{"VERSION", VERSION, NULL},
	{"VERTI", VERTI, NULL},
	{"VERTICAL", VERTICAL, NULL},
	{"VERTO", VERTO, NULL},
	{"VGAP", VGAP, NULL},
	{"VIEW", VIEW, NULL},
	{"VOIGT", FUNC_PPD, (void *) voigt},
	{"VX1", VX1, NULL},
	{"VX2", VX2, NULL},
	{"VXMAX", VXMAX, NULL},
	{"VY1", VY1, NULL},
	{"VY2", VY2, NULL},
	{"VYMAX", VYMAX, NULL},
	{"WELCH", WELCH, NULL},
	{"WITH", WITH, NULL},
	{"WORLD", WORLD, NULL},
	{"WRAP", WRAP, NULL},
	{"WRITE", WRITE, NULL},
	{"WX1", WX1, NULL},
	{"WX2", WX2, NULL},
	{"WY1", WY1, NULL},
	{"WY2", WY2, NULL},
	{"X", X_TOK, NULL},
	{"X0", X0, NULL},
	{"X1", X1, NULL},
	{"XAXES", XAXES, NULL},
	{"XAXIS", XAXIS, NULL},
	{"XCOR", XCOR, NULL},
	{"XMAX", XMAX, NULL},
	{"XMIN", XMIN, NULL},
	{"XY", XY, NULL},
	{"XYAXES", XYAXES, NULL},
	{"XYBOXPLOT", XYBOXPLOT, NULL},
	{"XYCOLOR", XYCOLOR, NULL},
	{"XYCOLPAT", XYCOLPAT, NULL},
	{"XYDX", XYDX, NULL},
	{"XYDXDX", XYDXDX, NULL},
	{"XYDXDXDYDY", XYDXDXDYDY, NULL},
	{"XYDXDY", XYDXDY, NULL},
	{"XYDY", XYDY, NULL},
	{"XYDYDY", XYDYDY, NULL},
	{"XYHILO", XYHILO, NULL},
	{"XYR", XYR, NULL},
	{"XYSIZE", XYSIZE, NULL},
	{"XYSTRING", XYSTRING, NULL},
	{"XYVMAP", XYVMAP, NULL},
	{"XYZ", XYZ, NULL},
	{"Y", Y_TOK, NULL},
	{"Y0", Y0, NULL},
	{"Y1", Y1, NULL},
	{"Y2", Y2, NULL},
	{"Y3", Y3, NULL},
	{"Y4", Y4, NULL},
	{"YAXES", YAXES, NULL},
	{"YAXIS", YAXIS, NULL},
	{"YEAR", YEAR, NULL},
	{"YMAX", YMAX, NULL},
	{"YMIN", YMIN, NULL},
	{"YV", FUNC_DD, (void *) yv_wrap},
	{"YYMMDD", YYMMDD, NULL},
	{"YYMMDDHMS", YYMMDDHMS, NULL},
	{"ZERO", ZERO, NULL},
	{"ZEROXAXIS", ALTXAXIS, NULL},
	{"ZEROYAXIS", ALTYAXIS, NULL},
	{"ZETA", FUNC_DD, (void *) zeta},
	{"ZETAC", FUNC_D, (void *) zetac},
	{"ZNORM", ZNORM, NULL}
};

static int maxfunc = sizeof(ikey) / sizeof(symtab_entry);

int get_parser_gno(void)
{
    return whichgraph;
}

int set_parser_gno(int gno)
{
    if (is_valid_gno(gno) == TRUE) {
        whichgraph = gno;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

int get_parser_setno(void)
{
    return whichset;
}

int set_parser_setno(int gno, int setno)
{
    if (is_valid_setno(gno, setno) == TRUE) {
        whichgraph = gno;
        whichset = setno;
        /* those will usually be overridden except when evaluating
           a _standalone_ vexpr */
        vasgn_gno = gno;
        vasgn_setno = setno;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

void realloc_vrbl(grarr *vrbl, int len)
{
    double *a;
    int i, oldlen;
    
    if (vrbl->type != GRARR_VEC) {
        errmsg("Internal error");
        return;
    }
    oldlen = vrbl->length;
    if (oldlen == len) {
        return;
    } else {
        a = xrealloc(vrbl->data, len*SIZEOF_DOUBLE);
        if (a != NULL || len == 0) {
            vrbl->data = a;
            vrbl->length = len;
            for (i = oldlen; i < len; i++) {
                vrbl->data[i] = 0.0;
            }
        } else {
            errmsg("Malloc failed in realloc_vrbl()");
        }
    }
}


#define PARSER_TYPE_VOID    0
#define PARSER_TYPE_EXPR    1
#define PARSER_TYPE_VEXPR   2

static int parser(char *s, int type)
{
    char *seekpos;
    int i;
    
    if (s == NULL || s[0] == '\0') {
        if (type == PARSER_TYPE_VOID) {
            /* don't consider an empty string as error for generic parser */
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    strncpy(f_string, s, MAX_PARS_STRING_LENGTH - 2);
    f_string[MAX_PARS_STRING_LENGTH - 2] = '\0';
    strcat(f_string, " ");
    
    seekpos = f_string;

    while ((seekpos - f_string < MAX_PARS_STRING_LENGTH - 1) && (*seekpos == ' ' || *seekpos == '\t')) {
        seekpos++;
    }
    if (*seekpos == '\n' || *seekpos == '#') {
        if (type == PARSER_TYPE_VOID) {
            /* don't consider an empty string as error for generic parser */
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    lowtoupper(f_string);
        
    pos = 0;
    interr = 0;
    expr_parsed  = FALSE;
    vexpr_parsed = FALSE;
    
    yyparse();

    /* free temp. arrays; for a vector expression keep the last one
     * (which is none but v_result), given there have been no errors
     * and it's what we've been asked for
     */
    if (vexpr_parsed && !interr && type == PARSER_TYPE_VEXPR) {
        for (i = 0; i < fcnt - 1; i++) {
            free_tmpvrbl(&(freelist[i]));
        }
    } else {
        for (i = 0; i < fcnt; i++) {
            free_tmpvrbl(&(freelist[i]));
        }
    }
    fcnt = 0;
    
    tgtn = 0;
    
    if ((type == PARSER_TYPE_VEXPR && !vexpr_parsed) ||
        (type == PARSER_TYPE_EXPR  && !expr_parsed)) {
        return RETURN_FAILURE;
    } else {
        return (interr ? RETURN_FAILURE:RETURN_SUCCESS);
    }
}

int s_scanner(char *s, double *res)
{
    int retval = parser(s, PARSER_TYPE_EXPR);
    *res = s_result;
    return retval;
}

int v_scanner(char *s, int *reslen, double **vres)
{
    int retval = parser(s, PARSER_TYPE_VEXPR);
    if (retval != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    } else {
        *reslen = v_result->length;
        if (v_result->type == GRARR_TMP) {
            *vres = v_result->data;
            v_result->length = 0;
            v_result->data = NULL;
        } else {
            *vres = copy_data_column(v_result->data, v_result->length);
        }
        return RETURN_SUCCESS;
    }
}

int scanner(char *s)
{
    int retval = parser(s, PARSER_TYPE_VOID);
    if (retval != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    }
    
    if (gotparams) {
	gotparams = FALSE;
        getparms(paramfile);
    }
    
    if (gotread) {
	gotread = FALSE;
        getdata(whichgraph, readfile, cursource, LOAD_SINGLE);
    }
    
    if (gotnlfit) {
	gotnlfit = FALSE;
        do_nonlfit(nlfit_gno, nlfit_setno, nlfit_warray, NULL, nlfit_nsteps);
        XCFREE(nlfit_warray);
    }
    return retval;
}

static void free_tmpvrbl(grarr *vrbl)
{
    if (vrbl->type == GRARR_TMP) {
        vrbl->length = 0;
        XCFREE(vrbl->data);
    }
}

static void copy_vrbl(grarr *dest, grarr *src)
{
    dest->type = src->type;
    dest->data = xmalloc(src->length*SIZEOF_DOUBLE);
    if (dest->data == NULL) {
        errmsg("Malloc failed in copy_vrbl()");
    } else {
        memcpy(dest->data, src->data, src->length*SIZEOF_DOUBLE);
        dest->length = src->length;
    }
}

grarr *get_parser_arr_by_name(char * const name)
{
     int position;
     char *s;
     
     s = copy_string(NULL, name);
     lowtoupper(s);
     
     position = findf(key, s);
     xfree(s);
     
     if (position >= 0) {
         if (key[position].type == KEY_VEC) {
            return (grarr *) key[position].data;
         }
     }
     
     return NULL;
}

grarr *define_parser_arr(char * const name)
{
     if (get_parser_arr_by_name(name) == NULL) {
	symtab_entry tmpkey;
        grarr *var;
        
        var = xmalloc(sizeof(grarr));
        var->type = GRARR_VEC;
        var->length = 0;
        var->data = NULL;
        
	tmpkey.s = name;
	tmpkey.type = KEY_VEC;
	tmpkey.data = (void *) var;
	if (addto_symtab(tmpkey) == RETURN_SUCCESS) {
	    return var;
	} else {
            return NULL;
        }
     } else {
        return NULL;
     }
}

int undefine_parser_var(void *ptr)
{
    int i;
    
    for (i = 0; i < maxfunc; i++) {
	if (key[i].data == ptr) {
            xfree(key[i].s);
            maxfunc--;
            if (i != maxfunc) {
                memmove(&(key[i]), &(key[i + 1]), (maxfunc - i)*sizeof(symtab_entry));
            }
            key = xrealloc(key, maxfunc*sizeof(symtab_entry));
            return RETURN_SUCCESS;
        }
    }
    return RETURN_FAILURE;
}

static int find_set_bydata(double *data, target *tgt)
{
    int gno, setno, ncol;
    
    if (data == NULL) {
        return RETURN_FAILURE;
    } else {
        for (gno = 0; gno < number_of_graphs(); gno++) {
            for (setno = 0; setno < number_of_sets(gno); setno++) {
                for (ncol = 0; ncol < MAX_SET_COLS; ncol++) {
                    if (getcol(gno, setno, ncol) == data) {
                        tgt->gno   = gno;
                        tgt->setno = setno;
                        return RETURN_SUCCESS;
                    }
                }
            }
        }
    }
    return RETURN_FAILURE;
}

static int findf(symtab_entry *keytable, char *s)
{

    int low, high, mid;

    low = 0;
    high = maxfunc - 1;
    while (low <= high) {
	mid = (low + high) / 2;
	if (strcmp(s, keytable[mid].s) < 0) {
	    high = mid - 1;
	} else {
	    if (strcmp(s, keytable[mid].s) > 0) {
		low = mid + 1;
	    } else {
		return (mid);
	    }
	}
    }
    return (-1);
}

static int compare_keys (const void *a, const void *b)
{
    return (int) strcmp (((const symtab_entry*)a)->s,
                         ((const symtab_entry*)b)->s);
}

/* add new entry to the symbol table */
int addto_symtab(symtab_entry newkey)
{
    int position;
    char *s;
    
    s = copy_string(NULL, newkey.s);
    lowtoupper(s);
    if ((position = findf(key, s)) < 0) {
        if ((key = (symtab_entry *) xrealloc(key, (maxfunc + 1)*sizeof(symtab_entry))) != NULL) {
	    key[maxfunc].type = newkey.type;
	    key[maxfunc].data = newkey.data;
	    key[maxfunc].s = s;
	    maxfunc++;
	    qsort(key, maxfunc, sizeof(symtab_entry), compare_keys);
	    return RETURN_SUCCESS;
	} else {
	    xfree(s);
	    return RETURN_FAILURE;
	}
    } else if (alias_force == TRUE) { /* already exists but alias_force enabled */
        key[position].type = newkey.type;
	key[position].data = newkey.data;
	return RETURN_SUCCESS;
    } else {
	xfree(s);
        return RETURN_FAILURE;
    }
}

/* initialize symbol table */
void init_symtab(void)
{
    int i;
    
    if ((key = (symtab_entry *) xmalloc(maxfunc*sizeof(symtab_entry))) != NULL) {
    	memcpy (key, ikey, maxfunc*sizeof(symtab_entry));
	for (i = 0; i < maxfunc; i++) {
	    key[i].s = xmalloc(strlen(ikey[i].s) + 1);
	    strcpy(key[i].s, ikey[i].s);
	}
	qsort(key, maxfunc, sizeof(symtab_entry), compare_keys);
	return;
    } else {
	key = ikey;
	return;
    }
}

static int getcharstr(void)
{
    if (pos >= strlen(f_string))
	 return EOF;
    return (f_string[pos++]);
}

static void ungetchstr(void)
{
    if (pos > 0)
	pos--;
}

static int yylex(void)
{
    int c, i;
    int found;
    char sbuf[MAX_PARS_STRING_LENGTH + 40];

    while ((c = getcharstr()) == ' ' || c == '\t');
    if (c == EOF) {
	return (0);
    }
    if (c == '"') {
	i = 0;
	while ((c = getcharstr()) != '"' && c != EOF) {
	    if (c == '\\') {
		int ctmp;
		ctmp = getcharstr();
		if (ctmp != '"') {
		    ungetchstr();
		}
		else {
		    c = ctmp;
		}
	    }
	    sbuf[i] = c;
	    i++;
	}
	if (c == EOF) {
	    yyerror("Nonterminating string");
	    return 0;
	}
	sbuf[i] = '\0';
	yylval.sval = copy_string(NULL, sbuf);
	return CHRSTR;
    }
    if (c == '.' || isdigit(c)) {
	double d;
	int i, gotdot = 0;

	i = 0;
	while (c == '.' || isdigit(c)) {
	    if (c == '.') {
		if (gotdot) {
		    yyerror("Reading number, too many dots");
	    	    return 0;
		} else {
		    gotdot = 1;
		}
	    }
	    sbuf[i++] = c;
	    c = getcharstr();
	}
	if (c == 'E' || c == 'e') {
	    sbuf[i++] = c;
	    c = getcharstr();
	    if (c == '+' || c == '-') {
		sbuf[i++] = c;
		c = getcharstr();
	    }
	    while (isdigit(c)) {
		sbuf[i++] = c;
		c = getcharstr();
	    }
	}
	if (gotdot && i == 1) {
	    ungetchstr();
	    return '.';
	}
	sbuf[i] = '\0';
	ungetchstr();
	sscanf(sbuf, "%lf", &d);
	yylval.dval = d;
	return NUMBER;
    }
/* graphs, sets, regions resp. */
    if (c == 'G' || c == 'S' || c == 'R') {
	int i = 0, ctmp = c, gn, sn, rn;
	c = getcharstr();
	while (isdigit(c) || c == '$' || c == '_') {
	    sbuf[i++] = c;
	    c = getcharstr();
	}
	if (i == 0) {
	    c = ctmp;
	    ungetchstr();
	} else {
	    ungetchstr();
	    if (ctmp == 'G') {
	        sbuf[i] = '\0';
		if (i == 1 && sbuf[0] == '_') {
                    gn = get_recent_gno();
                } else if (i == 1 && sbuf[0] == '$') {
                    gn = whichgraph;
                } else {
                    gn = atoi(sbuf);
                }
		if (is_valid_gno(gn) || graph_allocate(gn) == RETURN_SUCCESS) {
		    yylval.ival = gn;
		    return GRAPHNO;
		}
	    } else if (ctmp == 'S') {
	        sbuf[i] = '\0';
		if (i == 1 && sbuf[0] == '_') {
                    sn = get_recent_setno();
                } else if (i == 1 && sbuf[0] == '$') {
                    sn = whichset;
                } else {
		    sn = atoi(sbuf);
                }
		yylval.ival = sn;
		return SETNUM;
	    } else if (ctmp == 'R') {
	        sbuf[i] = '\0';
		rn = atoi(sbuf);
		if (rn >= 0 && rn < MAXREGION) {
		    yylval.ival = rn;
		    return REGNUM;
		} else {
                    errmsg("Invalid region number");
                }
	    }
	}
    }
    if (isalpha(c) || c == '$') {
	char *p = sbuf;

	do {
	    *p++ = c;
	} while ((c = getcharstr()) != EOF && (isalpha(c) || isdigit(c) ||
                  c == '_' || c == '$'));
	ungetchstr();
	*p = '\0';
#ifdef DEBUG
        if (get_debuglevel() == 2) {
	    printf("->%s<-\n", sbuf);
	}
#endif
	found = -1;
	if ((found = findf(key, sbuf)) >= 0) {
	    if (key[found].type == FITPARM) {
		int index = sbuf[1] - '0';
		yylval.ival = index;
		return FITPARM;
	    }
	    else if (key[found].type == FITPMAX) {
		int index = sbuf[1] - '0';
		yylval.ival = index;
		return FITPMAX;
	    }
	    else if (key[found].type == FITPMIN) {
		int index = sbuf[1] - '0';
		yylval.ival = index;
		return FITPMIN;
	    }

	    else if (key[found].type == KEY_VAR) {
		yylval.dptr = (double *) key[found].data;
		return VAR_D;
	    }
	    else if (key[found].type == KEY_VEC) {
		yylval.vrbl = (grarr *) key[found].data;
		return VEC_D;
	    }

	    else if (key[found].type == FUNC_I) {
		yylval.ival = found;
		return FUNC_I;
	    }
	    else if (key[found].type == CONSTANT) {
		yylval.ival = found;
		return CONSTANT;
	    }
	    else if (key[found].type == UCONSTANT) {
		yylval.ival = found;
		return UCONSTANT;
	    }
	    else if (key[found].type == FUNC_D) {
		yylval.ival = found;
		return FUNC_D;
	    }
	    else if (key[found].type == FUNC_ND) {
		yylval.ival = found;
		return FUNC_ND;
	    }
	    else if (key[found].type == FUNC_DD) {
		yylval.ival = found;
		return FUNC_DD;
	    }
	    else if (key[found].type == FUNC_NND) {
		yylval.ival = found;
		return FUNC_NND;
	    }
	    else if (key[found].type == FUNC_PPD) {
		yylval.ival = found;
		return FUNC_PPD;
	    }
	    else if (key[found].type == FUNC_PPPD) {
		yylval.ival = found;
		return FUNC_PPPD;
	    }
	    else if (key[found].type == FUNC_PPPPD) {
		yylval.ival = found;
		return FUNC_PPPPD;
	    }
	    else if (key[found].type == FUNC_PPPPPD) {
		yylval.ival = found;
		return FUNC_PPPPPD;
	    }
	    else {
	        yylval.ival = key[found].type;
	        return key[found].type;
	    }
	} else {
	    yylval.sval = copy_string(NULL, sbuf);
	    return NEW_TOKEN;
	}
    }
    switch (c) {
    case '>':
	return follow('=', GE, GT);
    case '<':
	return follow('=', LE, LT);
    case '=':
	return follow('=', EQ, '=');
    case '!':
	return follow('=', NE, NOT);
    case '|':
	return follow('|', OR, '|');
    case '&':
	return follow('&', AND, '&');
    case '\n':
	return '\n';
    default:
	return c;
    }
}

static int follow(int expect, int ifyes, int ifno)
{
    int c = getcharstr();

    if (c == expect) {
	return ifyes;
    }
    ungetchstr();
    return ifno;
}

static void yyerror(char *s)
{
    char *buf;
    
    buf = copy_string(NULL, s);
    buf = concat_strings(buf, ": ");
    buf = concat_strings(buf, f_string);
    errmsg(buf);
    xfree(buf);
    interr = 1;
}
