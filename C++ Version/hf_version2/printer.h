#ifndef __printer_h__
#define __printer_h__

//Added INCLUDE files
#include <string>

using namespace std;

#ifdef __printer_implementation__
#undef EXTERN
#define EXTERN
#endif

EXTERN string lst;
#undef EXTERN
#define EXTERN extern

#endif
