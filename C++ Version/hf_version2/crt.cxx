#include "ptoc.h"

/*******************************************************/
/*                                                       */
/*       Turbo Pascal Runtime Library                    */
/*       CRT Interface Unit                              */
/*                                                       */
/*       Copyright (C) 1988,92 Borland International     */
/*                                                       */
/*******************************************************/
#define __crt_implementation__

/*$I-,S-*/
#include "crt.h"

#ifdef DPMI
/*$L CRT.OBP*/
#else
/*$L CRT.OBJ*/
#endif
void break_()
{
  exit(255);
}
class unit_crt_initialize {
  public: unit_crt_initialize();
};
static unit_crt_initialize crt_constructor;

unit_crt_initialize::unit_crt_initialize() {
  //initialize();
  //assigncrt(input); reset(input);
  //assigncrt(output); rewrite(output);
}
