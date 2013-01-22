/*
        KO Myung-Hun's universal macro and type definition header

        Written by KO Myung-Hun
        Term of programming : 1999.02.24

        Modified by KO Myung-Hun
        Term of programming : 1999.10.07

        Contents : BOOL type changed from 'enum' to 'unsinged long' for
                   compatible with other header files.

        Source file   : ktype.h
        Used compiler : Borland C++ 1.0 for OS/2
                        emx 0.9d + gcc 2.8.1
*/

#ifndef _KTYPE_H
#define _KTYPE_H

#ifndef min
#define min( a, b ) (( a ) < ( b ) ? ( a ) : ( b ))
#endif

#ifndef max
#define max( a, b ) (( a ) > ( b ) ? ( a ) : ( b ))
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned long BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef long int LONG;

#endif

