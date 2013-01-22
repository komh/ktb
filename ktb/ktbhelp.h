/*
        Header file for 'ktbhelp.c'

        Written by KO Myung-Hun
        Term of programming : 1999.11.16 - 1999.11.18

        Source file   : ktbhelp.h
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#ifndef __KTBHELP_H__
#define __KTBHELP_H__

#define INCL_PM
#include <os2.h>

#include "ktbtimer.h"

#define WC_KTBHELP  "ktbHelp"

#define ID_KTBHELP  ( ID_KTBTIMER - 1 )

#define WM_QUERY_CHAR_WIDTH     ( WM_USER + 100 )
#define WM_QUERY_CHAR_HEIGHT    ( WM_USER + 101 )

#ifdef __cplusplus
extern "C" {
#endif

BOOL createKtbHelp( HAB hab, HWND hwndParent, HWND hwndOwner );

#ifdef __cplusplus
}
#endif

#endif
