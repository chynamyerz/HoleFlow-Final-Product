#ifndef __hf_menu_h__
#define __hf_menu_h__

#include "crt.h"
#include "dos.h"
#include "hf_util.h"
#include "Printer.h"
#include "Win.h"

const string version = "v3.1";
const integer maxnumsets = 50;
const integer nholetypes = 7;
const string holetypes[nholetypes] = {"Swirler     ",
                                               "Vaporiser   ",
                                               "Plain       ",
                                               "Plunged     ",
                                               "Splash hole ",
                                               "Wiggle strip",
                                               "Machnd. ring"};
const string holepositions[3] = {"OnDome","Inner ","Outer "};

#ifdef __hf_menu_implementation__
#undef EXTERN
#define EXTERN
#endif

EXTERN string casename;
EXTERN integer cctype,nholesets,ncans;
EXTERN bool exitflag,filmcooling,simplified;
EXTERN double tambient,eli,elo,eci,eco,kl,kc,tl,tc,
mdot,afr,p0,t0,cp,gamma,ch,lcv,pstatcan,maod,
split,mprz,mprp,ms,md,afrst,lrz,lpz,lsz,lcomb,qref,dp34,lmax;
EXTERN integer cdspecification[maxnumsets],holesetorder[maxnumsets],holetype[maxnumsets],nholes[maxnumsets],holeposition[maxnumsets];
EXTERN double holesetmassflow[maxnumsets],holesetvelocity[maxnumsets],holecd[maxnumsets],dhole[maxnumsets],xhole[maxnumsets],dliner[maxnumsets],dcasing[maxnumsets],dlineri[maxnumsets],
dcasingi[maxnumsets],dcasingrem[maxnumsets],primaryrzy[maxnumsets],primaryrpy[maxnumsets],secondaryy[maxnumsets],dilutiony[maxnumsets],
holeflowangle[maxnumsets],acd[maxnumsets],bcd[maxnumsets],ccd[maxnumsets],dcd[maxnumsets],slotdepth[maxnumsets];
#undef EXTERN
#define EXTERN extern


  /* Global procedures and functions */
  void writedata(string filename);
  void readdata(string filename,bool readerror);
  void initialise();
  void holespecinputmenu();
  void mainmenu();

#endif
