#include "ptoc.h"

/*******************************************************/
/*                                                       */
/*       Turbo Pascal Version 5.5                        */
/*       Window Interface Unit                           */
/*                                                       */
/*       Copyright (C) 1989 Borland International        */
/*                                                       */
/*       Adapted by A.A. Dreyer - 3 June 1991            */
/*                                                       */
/*******************************************************/
#define __win_implementation__

/*$D-,S-*/
#include "win.h"

/*$L WIN*/

extern void writestr(BYTE x, BYTE y, string s, BYTE attr);


extern void writechar(BYTE x, BYTE y, BYTE count, char ch, BYTE attr);


extern void fillwin(char ch, BYTE attr);


extern void writewin(void* buf);


extern void readwin(void* buf);


extern WORD winsize();


extern BYTE lo(WORD w);


extern BYTE hi(WORD w);


void savewin(winstate& w)
{
  w.windmin = windmin;
  w.windmax = windmax;
  w.wherex = wherex();
  w.wherey = wherey();
  w.textattr = textattr;
}

void restorewin(winstate& w)
{
  windmin = w.windmin;
  windmax = w.windmax;
  gotoxy(w.wherex, w.wherey);
  textattr = w.textattr;
}

void framewin(titlestr title,titlestr title2, framechars& frame,
  BYTE titleattr, BYTE frameattr)
{
  WORD w, h, y;

  w = lo(windmax) - lo(windmin) + 1;
  h = hi(windmax) - hi(windmin) + 1;
  //writechar(1, 1, 1, frame[1], frameattr);
  //writechar(2, 1, w - 2, frame[2], frameattr);
  //writechar(w, 1, 1, frame[3], frameattr);
  /* Main title */
  if ((title).length() > w - 2)  title[0] = chr(w - 2);
  //writestr(((cardinal)(w - (title).length()) >> 1) + 1, 1, title, titleattr);
  for( y = 2; y <= h - 1; y ++)
  {
    //writechar(1, y, 1, frame[4], frameattr);
    //writechar(w, y, 1, frame[5], frameattr);
  }
  //writechar(1, h, 1, frame[6], frameattr);
  //writechar(2, h, w - 2, frame[7], frameattr);
  //writechar(w, h, 1, frame[8], frameattr);
  /* Second title */
  if ((title2).length() > w - 2)  title2[0] = chr(w - 2);
  //writestr(((cardinal)(w - (title2).length()) >> 1) + 1, h, title2, titleattr);
  //windmin += 0x101;
  windmax -= 0x101;
}

void unframewin()
{
  windmin -= 0x101;
  windmax += 0x101;
}

/* Procedure to set up active window
void activewindow(boolean active)
{
  if (topwindow != nil)
  {
    unframewin();
    { winrec& with = *topwindow;
      if (active)
        framewin(*with.title,*with.title2, as(framechars,doubleframe), with.ttlattr, with.frmattr);
      else
        framewin(*with.title,*with.title2, as(framechars,singleframe), with.frmattr, with.frmattr);
    }
  }
}    /* of procdure ACTIVEWINDOW */


/* Procedure to open a window at a specified position
void openwindow(BYTE x1, BYTE y1, BYTE x2, BYTE y2, titlestr t,titlestr t2, BYTE tattr, BYTE fattr)
{
  winrecptr w;

  activewindow(false);
  w = new winrec;
  {
    winrec& with = *w;

    with.next = topwindow;
    savewin(with.state);
    getmem(with.title, length(t) + 1);
    getmem(with.title2, length(t2) + 1);
    *with.title = t;
    *with.title2 = t2;
    with.ttlattr = tattr;
    with.frmattr = fattr;
    window(x1, y1, x2, y2);
    getmem(with.buffer, winsize());
    readwin(&with.buffer);
    framewin(t,t2, as(framechars,doubleframe), tattr, fattr);
  }
  topwindow = w;
  clrscr();
}    /* of procedure OPENWINDOW */


/* Procedure to close the currently active window
void closewindow()
{
  winrecptr w;

  if (topwindow != nil)
  {
    w = topwindow;
    {
      winrec& with = *w;

      unframewin();
      writewin(&with.buffer);
      freemem(with.buffer, winsize());
      freemem(with.title, length(*with.title) + 1);
/*      FreeMem(Title2, Length(Title2^) + 1);
      restorewin(with.state);
      topwindow = with.next;
    }
    delete w;
    activewindow(true);
  }
}    /* of procedure CLOSEWINDOW */

