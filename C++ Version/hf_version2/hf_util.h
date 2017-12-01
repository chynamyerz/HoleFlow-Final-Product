#ifndef __hf_util_h__
#define __hf_util_h__

#include "crt.h"
#include "dos.h"
#include "printer.h"
#include "win.h"

/* Global variables */
typedef string strarr[23];
typedef string strarr2[3];

#ifdef __hf_util_implementation__
#undef EXTERN
#define EXTERN
#endif

EXTERN double time1;
EXTERN string rootdir;
EXTERN string maintitle;
EXTERN WORD h1,m1,s1,s1x,h2,m2,s2,s2x;
EXTERN int titleattr,menuattr,highlightattr;
EXTERN int brightattr,firstcharattr;
EXTERN int normalattr,firstcharhighlightattr;
#undef EXTERN
#define EXTERN extern


  /* Global procedures and functions */
  void cdeditmenu(double& a,double& b,double& c,double& d);
  double tanh(double a);
  double p(double aa,double bb);
  double log10(double aa);
  real asin(real a);
  double dt(double phi,double t3,double p3);
  string ucstring(string s);
  void starttimer();
  double endtimer();
  bool accessdisk(string defdir);
  bool accessprinter();
  string readstring(BYTE n,string default_,bool displaychars);
  void windowbox(int x1,int y1,int width,int height,string title);
  void windowbox2(int x1,int y1,int width,int height,string title,string title2);
  void heading(string title,bool clrscreen);
  string rts(double x,int width,int decimals);
  string its(int x,int width);
  void displaymenu(int numentr,int firstpos,strarr item,string title,int& selected);
  void choosefile(string& dir,string mask,string heading,string& chosen);
  void choosefile0(string& dir,string mask,string& chosen);
  void cursoroff();
  void cursoron();
  void entrybox(int numentr,int xpos,int ypos,int excess,strarr2 item,string title);
  double realprompt(double default_,double min,double max,int code,int xpos,int ypos,strarr2 item);
  int intprompt(int default_,int min,int max,int code,int xpos,int ypos,strarr2 item);
  string filenameprompt(string default_,int xpos,int ypos,string question);
  string stringprompt(string default_,int xpos,int ypos,string question);
  int digitprompt(int default_,int min,int max,int xpos,int ypos,string question);
  char charprompt(char default_,string choices,int xpos,int ypos,string textstring);
  void displaymessage(int xpos,int ypos,string question,bool readkeyboard);
  void displaymessage2(int xpos,int ypos,string question);
#endif
