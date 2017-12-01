#ifndef __win_h__
#define __win_h__

#include "crt.h"

/* Window title string */
typedef string titlestr;

/* Window frame characters */
typedef char framechars[10];

/* Window state record */
struct winstate {
  WORD windmin, windmax;
  BYTE wherex, wherey;
  BYTE textattr;
};

typedef titlestr* titlestrptr;
typedef titlestr* title2strptr;
typedef struct winrec* winrecptr;
struct winrec {
  winrecptr next;
  winstate state;
  titlestrptr title;
  title2strptr title2;
  BYTE ttlattr, frmattr;
  //pointer buffer;
};

/* Standard frame character sets */
const framechars blankframe = {"        "};
const framechars singleframe = {"ÚÄ¿³³ÀÄÙ"};
const framechars doubleframe = {"ÉÍ»ººÈÍ¼"};

#ifdef __win_implementation__
#undef EXTERN
#define EXTERN
#endif

EXTERN winrecptr topwindow;
#undef EXTERN
#define EXTERN extern


/* Direct write routines */
void writestr(BYTE x, BYTE y, string s, BYTE attr);
void writechar(BYTE x, BYTE y, BYTE count, char ch, BYTE attr);
/* Window handling routines */
void fillwin(char ch, BYTE attr);
void readwin(void* buf);
void writewin(void* buf);
WORD winsize();
void savewin(winstate& w);
void restorewin(winstate& w);
void framewin(titlestr title,titlestr title2, framechars& frame, BYTE titleattr, BYTE frameattr);
void unframewin();
void activewindow(boolean active);
void openwindow(BYTE x1, BYTE y1, BYTE x2, BYTE y2, titlestr t,titlestr t2, BYTE tattr, BYTE fattr);
void closewindow();

#endif
