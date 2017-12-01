#include "ptoc.h"


/*******************************************************/
/*                                                       */
/*       Turbo Pascal Runtime Library                    */
/*       DOS Interface Unit                              */
/*                                                       */
/*       Copyright (C) 1988,92 Borland International     */
/*                                                       */
/*******************************************************/

#define __dos_implementation__


/*$I-,O+,S-*/

#include "dos.h"

//Added INCLUDE files
#include <windef.h>


/*$L VERS.OBJ*/           /* DOS version routine */
/*$L TIME.OBJ*/           /* Date and time routines */
/*$L CBRK.OBJ*/           /* Ctrl-Break flag handling */
/*$L VERF.OBJ*/           /* Verify flag handling */
/*$L DISK.OBJ*/           /* Disk status routines */
/*$L FATR.OBJ*/           /* File attribute routines */
/*$L FTIM.OBJ*/           /* File date and time routines */
/*$L FIND.OBJ*/           /* Directory search routines */
/*$L PTIM.OBJ*/           /* Time pack and unpack routines */
/*$L VECT.OBJ*/           /* Interrupt vector handling */
/*$L SRCH.OBJ*/           /* File search routine */
/*$L EXPN.OBJ*/           /* File name expansion routine */
/*$L SPLT.OBJ*/           /* File name split routine */
/*$L ENVS.OBJ*/           /* Environment string routines */
/*$L ENVV.OBJ*/           /* Environment variable routine */
/*$L KEEP.OBJ*/           /* TSR support routine */
/*$L EXEC.OBJ*/           /* Program execution routines */

#ifdef DPMI

/*$L INTR.OBP*/           /* Software interrupt routines */
/*$L SWAP.OBP*/           /* Interrupt vector swapping */

#else

/*$L INTR.OBJ*/           /* Software interrupt routines */
/*$L SWAP.OBJ*/           /* Interrupt vector swapping */

#endif

extern WORD dosversion();

extern void intr(BYTE intno, registers& regs);

extern void msdos(registers& regs);

extern void getdate(WORD& year,WORD& month,WORD& day,WORD& dayofweek);

extern void setdate(WORD year,WORD month,WORD day);

extern void gettime(WORD& hour,WORD& minute,WORD& second,WORD& sec100);

extern void settime(WORD hour,WORD minute,WORD second,WORD sec100);

extern void getcbreak(boolean& break_);

extern void setcbreak(boolean break_);

extern void getverify(boolean& verify);

extern void setverify(boolean verify);

extern long int diskfree(BYTE drive);

extern long int disksize(BYTE drive);

extern void getfattr(void* f, WORD& attr);

extern void setfattr(void* f, WORD attr);

extern void getftime(void* f, long int& time);

extern void setftime(void* f, long int time);

extern void findfirst(pathstr path, WORD attr, searchrec& f);


extern void findnext(searchrec& f);

extern void unpacktime(long int p, datetime& t);

extern void packtime(datetime& t, long int& p);

//extern void getintvec(BYTE intno, pointer& vector);

//extern void setintvec(BYTE intno, pointer vector);

extern pathstr fsearch(pathstr path, string dirlist);

extern pathstr fexpand(pathstr path);

extern void fsplit(pathstr path, dirstr& dir,
      namestr& name, extstr& ext);

extern integer envcount();

extern string envstr(integer index);

extern string getenv(string envvar);

extern void swapvectors();

extern void keep(WORD exitcode);

extern void exec(pathstr path, comstr comline);

extern WORD dosexitcode();


