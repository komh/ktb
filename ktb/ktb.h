/*
        Header file for 'ktb.cpp'

        Written by KO Myung-Hun
        Term of programming : 1999.10.06 - 1999.11.18

        Source file   : ktb.h
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#ifndef __KTB_H
#define __KTB_H

#define INCL_GPILCIDS
#include <os2.h>

#include "twininfo.h"

#define ID_KTB2  1
#define WC_KTB2  "kTaskBar/2"

typedef struct tagKTBInfo
{
    HWND        hwndParent;
    FONTMETRICS fm;
    PHookData   phd;
    TWinInfoCol *pwic;
} TKTBInfo, *PKTBInfo;

#endif
