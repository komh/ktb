// start 1999.10.13
/*
        Header file for 'ktbhook.c'

        Written by KO Myung-Hun
        Term of programming : 1999.10.13 - 1999.11.16

        Source file   : ktbhook.h
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#ifndef __KTBHOOK_H_
#define __KTBHOOK_H_

#define INCL_PM
#include <os2.h>

#define WM_SHOW_WINDOW      ( WM_USER + 256 )
#define WM_HIDE_WINDOW      ( WM_USER + 257 )
#define WM_PRINT_ALL_TITLE  ( WM_USER + 258 )

typedef struct tagHookData
{
    HWND    hwnd;
    RECTL   winRcl;
    RECTL   callRcl;
    BOOL    bShow;
    BOOL    bProcessing;
} THookData, *PHookData;

#ifdef __cplusplus
extern "C" {
#endif

BOOL EXPENTRY installHook( PHookData, PHookData * );
VOID EXPENTRY uninstallHook( VOID );

#ifdef __cplusplus
}
#endif

#endif

