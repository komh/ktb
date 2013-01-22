/*
        Header file for 'ktbtimer.c'

        Written by KO Myung-Hun
        Term of programming : 1999.11.11 - 1999.11.18

        Source file   : ktbtimer.h
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#ifndef __KTB_TIMER__
#define __KTB_TIMER__

#define INCL_PM
#include <os2.h>

#define WC_KTBTIMER     "ktbTimer"

#define ID_KTBTIMER     1000

#ifdef __cplusplus
extern "C" {
#endif

BOOL createKtbTimer( HAB hab, HWND hwndParent, HWND hwndOwner, PRECTL prcl );

#ifdef __cplusplus
}
#endif

#endif

