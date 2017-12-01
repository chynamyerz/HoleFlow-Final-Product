#ifndef __dos_h__
#define __dos_h__

//Added INCLUDE files
#include <windef.h>
#include <string>

using namespace std;

/* Flags bit masks */

const integer fcarry = 0x1;
const integer fparity = 0x4;
const integer fauxiliary = 0x10;
const integer fzero = 0x40;
const integer fsign = 0x80;
const integer foverflow = 0x800;

/* File mode magic numbers */

const integer fmclosed = 0xd7b0;
const integer fminput = 0xd7b1;
const integer fmoutput = 0xd7b2;
const integer fminout = 0xd7b3;

/* File attribute constants */

const integer readonly = 0x1;
const integer hidden = 0x2;
const integer sysfile = 0x4;
const integer volumeid = 0x8;
const integer directory = 0x10;
const integer archive = 0x20;
const integer anyfile = 0x3f;

/* String types */

typedef string comstr;        /* Command line string */
typedef string pathstr;         /* File pathname string */
typedef string dirstr;         /* Drive and directory string */
typedef string namestr;          /* File name string */
typedef string extstr;          /* File extension string */

/* Registers record used by Intr and MsDos */

struct registers {
              union {
                struct {WORD ax,bx,cx,dx,bp,si,di,ds,es,flags;} s1;
                struct {BYTE al,ah,bl,bh,cl,ch,dl,dh;} s2;
              };
};

/* Typed-file and untyped-file record */

struct filerec {
            WORD handle;
            WORD mode;
            WORD recsize;
            BYTE private_[26];
            BYTE userdata[16];
            char name[79];
};

/* Textfile record */

typedef char textbuf[127];
struct textrec {
            WORD handle;
            WORD mode;
            WORD bufsize;
            WORD private_;
            WORD bufpos;
            WORD bufend;
            textbuf* bufptr;
            //pointer openfunc;
            //pointer inoutfunc;
            //pointer flushfunc;
            //pointer closefunc;
            BYTE userdata[16];
            char name[79];
            textbuf buffer;
};

/* Search record used by FindFirst and FindNext */

struct searchrec {
              BYTE fill[21];
              BYTE attr;
              long int time;
              long int size;
              string name;
};

/* Date and time record used by PackTime and UnpackTime */

struct datetime {
             WORD year,month,day,hour,min,sec;
};


#ifdef __dos_implementation__
#undef EXTERN
#define EXTERN
#endif

/* Error status variable */

EXTERN integer doserror;
#undef EXTERN
#define EXTERN extern


/* DosVersion returns the DOS version number. The low BYTE of    */
/* the result is the major version number, and the high BYTE is  */
/* the minor version number. For example, DOS 3.20 returns 3 in  */
/* the low BYTE, and 20 in the high BYTE.                        */

WORD dosversion();

/* Intr executes a specified software interrupt with a specified */
/* Registers package.                                            */

void intr(BYTE intno, registers& regs);

/* MsDos invokes the DOS function call handler with a specified  */
/* Registers package.                                            */

void msdos(registers& regs);

/* GetDate returns the current date set in the operating system. */
/* Ranges of the values returned are: Year 1980-2099, Month      */
/* 1-12, Day 1-31 and DayOfWeek 0-6 (0 corresponds to Sunday).   */

void getdate(WORD& year,WORD& month,WORD& day,WORD& dayofweek);

/* SetDate sets the current date in the operating system. Valid  */
/* parameter ranges are: Year 1980-2099, Month 1-12 and Day      */
/* 1-31. If the date is not valid, the function call is ignored. */

void setdate(WORD year,WORD month,WORD day);

/* GetTime returns the current time set in the operating system. */
/* Ranges of the values returned are: Hour 0-23, Minute 0-59,    */
/* Second 0-59 and Sec100 (hundredths of seconds) 0-99.          */

void gettime(WORD& hour,WORD& minute,WORD& second,WORD& sec100);

/* SetTime sets the time in the operating system. Valid          */
/* parameter ranges are: Hour 0-23, Minute 0-59, Second 0-59 and */
/* Sec100 (hundredths of seconds) 0-99. If the time is not       */
/* valid, the function call is ignored.                          */

void settime(WORD hour,WORD minute,WORD second,WORD sec100);

/* GetCBreak returns the state of Ctrl-Break checking in DOS.    */
/* When off (False), DOS only checks for Ctrl-Break during I/O   */
/* to console, printer, or communication devices. When on        */
/* (True), checks are made at every system call.                 */

void getcbreak(boolean& break_);

/* SetCBreak sets the state of Ctrl-Break checking in DOS.       */

void setcbreak(boolean break_);

/* GetVerify returns the state of the verify flag in DOS. When   */
/* off (False), disk writes are not verified. When on (True),    */
/* all disk writes are verified to insure proper writing.        */

void getverify(boolean& verify);

/* SetVerify sets the state of the verify flag in DOS.           */

void setverify(boolean verify);

/* DiskFree returns the number of free BYTEs on the specified    */
/* drive number (0=Default,1=A,2=B,..). DiskFree returns -1 if   */
/* the drive number is invalid.                                  */

long int diskfree(BYTE drive);

/* DiskSize returns the size in BYTEs of the specified drive     */
/* number (0=Default,1=A,2=B,..). DiskSize returns -1 if the     */
/* drive number is invalid.                                      */

long int disksize(BYTE drive);

/* GetFAttr returns the attributes of a file. F must be a file   */
/* variable (typed, untyped or textfile) which has been assigned */
/* a name. The attributes are examined by ANDing with the        */
/* attribute masks defined as constants above. Errors are        */
/* reported in DosError.                                         */

void getfattr(void* f, WORD& attr);

/* SetFAttr sets the attributes of a file. F must be a file      */
/* variable (typed, untyped or textfile) which has been assigned */
/* a name. The attribute value is formed by adding (or ORing)    */
/* the appropriate attribute masks defined as constants above.   */
/* Errors are reported in DosError.                              */

void setfattr(void* f, WORD attr);

/* GetFTime returns the date and time a file was last written.   */
/* F must be a file variable (typed, untyped or textfile) which  */
/* has been assigned and opened. The Time parameter may be       */
/* unpacked throgh a call to UnpackTime. Errors are reported in  */
/* DosError.                                                     */

void getftime(void* f, long int& time);

/* SetFTime sets the date and time a file was last written.      */
/* F must be a file variable (typed, untyped or textfile) which  */
/* has been assigned and opened. The Time parameter may be       */
/* created through a call to PackTime. Errors are reported in    */
/* DosError.                                                     */

void setftime(void* f, long int time);

/* FindFirst searches the specified (or current) directory for   */
/* the first entry that matches the specified filename and       */
/* attributes. The result is returned in the specified search    */
/* record. Errors (and no files found) are reported in DosError. */

void findfirst(pathstr path, WORD attr, searchrec& f);

/* FindNext returs the next entry that matches the name and      */
/* attributes specified in a previous call to FindFirst. The     */
/* search record must be one passed to FindFirst. Errors (and no */
/* more files) are reported in DosError.                         */

void findnext(searchrec& f);

/* UnpackTime converts a 4-BYTE packed date/time returned by     */
/* FindFirst, FindNext or GetFTime into a DateTime record.       */

void unpacktime(long int p, datetime& t);

/* PackTime converts a DateTime record into a 4-BYTE packed      */
/* date/time used by SetFTime.                                   */

void packtime(datetime& t, long int& p);

/* GetIntVec returns the address stored in the specified         */
/* interrupt vector.                                             */

//void getintvec(BYTE intno, pointer& vector);

/* SetIntVec sets the address in the interrupt vector table for  */
/* the specified interrupt.                                      */

//void setintvec(BYTE intno, pointer vector);

/* FSearch searches for the file given by Path in the list of    */
/* directories given by DirList. The directory paths in DirList  */
/* must be separated by semicolons. The search always starts     */
/* with the current directory of the current drive. The returned */
/* value is a concatenation of one of the directory paths and    */
/* the file name, or an empty string if the file could not be    */
/* located.                                                      */

pathstr fsearch(pathstr path, string dirlist);

/* FExpand expands the file name in Path into a fully qualified  */
/* file name. The resulting name consists of a drive letter, a   */
/* colon, a root relative directory path, and a file name.       */
/* Embedded '.' and '..' directory references are removed.       */

pathstr fexpand(pathstr path);

/* FSplit splits the file name specified by Path into its three  */
/* components. Dir is set to the drive and directory path with   */
/* any leading and trailing backslashes, Name is set to the file */
/* name, and Ext is set to the extension with a preceding dot.   */
/* Each of the component strings may possibly be empty, if Path  */
/* contains no such component.                                   */

void fsplit(pathstr path, dirstr& dir,
      namestr& name, extstr& ext);

/* EnvCount returns the number of strings contained in the DOS   */
/* environment.                                                  */

integer envcount();

/* EnvStr returns a specified environment string. The returned   */
/* string is of the form "VAR=VALUE". The index of the first     */
/* string is one. If Index is less than one or greater than      */
/* EnvCount, EnvStr returns an empty string.                     */

string envstr(integer index);

/* GetEnv returns the value of a specified environment variable. */
/* The variable name can be in upper or lower case, but it must  */
/* not include the '=' character. If the specified environment   */
/* variable does not exist, GetEnv returns an empty string.      */

string getenv(string envvar);

/* SwapVectors swaps the contents of the SaveIntXX pointers in   */
/* the System unit with the current contents of the interrupt    */
/* vectors. SwapVectors is typically called just before and just */
/* after a call to Exec. This insures that the Exec'd process    */
/* does not use any interrupt handlers installed by the current  */
/* process, and vice versa.                                      */

void swapvectors();

/* Keep (or Terminate Stay Resident) terminates the program and  */
/* makes it stay in memory. The entire program stays in memory,  */
/* including data segment, stack segment, and heap. The ExitCode */
/* corresponds to the one passed to the Halt standard procedure. */

void keep(WORD exitcode);

/* Exec executes another program. The program is specified by    */
/* the Path parameter, and the command line is specified by the  */
/* CmdLine parameter. To execute a DOS internal command, run     */
/* COMMAND.COM, e.g. "Exec('\COMMAND.COM','/C DIR *.PAS');".     */
/* Note the /C in front of the command. Errors are reported in   */
/* DosError. When compiling a program that uses Exec, be sure    */
/* to specify a maximum heap size as there will otherwise not be */
/* enough memory.                                                */

void exec(pathstr path, comstr comline);

/* DosExitCode returns the exit code of a sub-process. The low   */
/* BYTE is the code sent by the terminating process. The high    */
/* BYTE
 is zero for normal termination, 1 if terminated by       */
/* Ctrl-C, 2 if terminated due to a device error, or 3 if        */
/* terminated by the Keep procedure (function call 31 hex).      */

WORD dosexitcode();

#endif
