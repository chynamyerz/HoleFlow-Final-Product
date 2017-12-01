#ifndef __crt_h__
#define __crt_h__

//Added INCLUDE files
#include <windef.h>
#include <string>

using namespace std;

/* CRT modes */
const int bw40 = 0;            /* 40x25 B/W on Color Adapter */
const int co40 = 1;            /* 40x25 Color on Color Adapter */
const int bw80 = 2;            /* 80x25 B/W on Color Adapter */
const int co80 = 3;            /* 80x25 Color on Color Adapter */
const int mono = 7;            /* 80x25 on Monochrome Adapter */
const int font8x8 = 256;          /* Add-in for ROM font */
/* Mode constants for 3.0 compatibility */
const int c40 = co40;
const int c80 = co80;
/* Foreground and background color constants */
const int black = 0;
const int blue = 1;
const int green = 2;
const int cyan = 3;
const int red = 4;
const int magenta = 5;
const int brown = 6;
const int lightgray = 7;
/* Foreground color constants */
const int darkgray = 8;
const int lightblue = 9;
const int lightgreen = 10;
const int lightcyan = 11;
const int lightred = 12;
const int lightmagenta = 13;
const int yellow = 14;
const int white = 15;
/* Add-in for blinking */

const int blink = 128;


#ifdef __crt_implementation__
#undef EXTERN
#define EXTERN
#endif

/* Interface variables */

EXTERN bool checkbreak;    /* Enable Ctrl-Break */
EXTERN bool checkeof;      /* Enable Ctrl-Z */
EXTERN bool directvideo1;   /* Enable direct video addressing */
EXTERN bool checksnow;     /* Enable snow filtering */
EXTERN WORD lastmode;         /* Current text mode */
EXTERN BYTE textattr;         /* Current text attribute */
EXTERN WORD windmin;          /* Window upper left coordinates */
EXTERN WORD windmax;
#undef EXTERN
#define EXTERN extern
                          /* Window lower right coordinates */
/* Interface procedures */
extern void assigncrt(string f);
extern bool keypressed();
extern char readkey();
extern void textmode(int mode);
extern void window(BYTE x1,BYTE y1,BYTE x2,BYTE y2);
extern void gotoxy(BYTE x,BYTE y);
extern BYTE wherex();
extern BYTE wherey();
extern void clrscr();
extern void clreol();
extern void insline();
extern void delline();
extern void textcolor(BYTE color);
extern void textbackground(BYTE color);
extern void lowvideo();
extern void highvideo();
extern void normvideo();
extern void delay(int ms);
extern void sound(int hz);
extern void nosound();
extern string getdir();
#endif
