/*
        Window procedure for Timer

        Copyright (C) 1999 by KO Myung-Hun

        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Library General Public
        License as published by the Free Software Foundation; either
        version 2 of the License, or any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Library General Public License for more details.

        You should have received a copy of the GNU Library General Public
        License along with this library; if not, write to the Free
        Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Change Log :

        Written by KO Myung-Hun
        Term of programming : 1999.11.11 - 1999.11.18

        Source file   : ktbtimer.c
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#define INCL_DOSDATETIME
#define INCL_PM
#include <os2.h>

#include <stdio.h>
#include <string.h>

#include "ktbhook.h"
#include "ktbtimer.h"

static
MRESULT EXPENTRY timerWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    static BOOL bDateTime = FALSE;
    static BOOL bButton1Down = FALSE, bButton2Down = FALSE;

    switch( msg )
    {
        case WM_CREATE :
            bButton1Down = FALSE;
            bButton2Down = FALSE;

            return MRFROMSHORT( FALSE );

        case WM_PAINT :
        {
            CHAR        szDateTime[ 20 ];
            DATETIME    dt;
            HPS         hps;
            RECTL       wndRcl, dbRcl;

            DosGetDateTime( &dt );

            if( bDateTime )
                sprintf( szDateTime, "%4d/%02d/%02d",
                         ( INT )dt.year, ( INT )dt.month, ( INT )dt.day );
            else
                sprintf( szDateTime, "%02d:%02d:%02d",
                         ( INT )dt.hours, ( INT )dt.minutes, ( INT )dt.seconds );

            hps = WinBeginPaint( hwnd, NULLHANDLE, NULL );

            WinQueryWindowRect( hwnd, &wndRcl );

            WinDrawBorder( hps, &wndRcl, 1, 1, SYSCLR_MENUTEXT, SYSCLR_BUTTONMIDDLE,
                            DB_INTERIOR );

            dbRcl = wndRcl;
            dbRcl.xLeft = dbRcl.xRight - 1;
            WinDrawBorder( hps, &dbRcl, 1, 0, SYSCLR_BUTTONLIGHT, SYSCLR_BUTTONMIDDLE,
                            DB_PATCOPY );

            dbRcl.xLeft = wndRcl.xLeft;
            dbRcl.yTop = dbRcl.yBottom + 1;
            WinDrawBorder( hps, &dbRcl, 0, 1, SYSCLR_BUTTONLIGHT, SYSCLR_BUTTONMIDDLE,
                            DB_PATCOPY );

            WinDrawText( hps, -1, szDateTime, &wndRcl, SYSCLR_MENUTEXT, SYSCLR_BUTTONMIDDLE,
                         DT_CENTER | DT_VCENTER);

            WinEndPaint( hps );

            return MRFROMSHORT( 0 );
        }

        case WM_BUTTON1DOWN :
            bButton1Down = TRUE;
            if( bButton2Down )
                WinPostMsg( WinQueryWindow( hwnd, QW_PARENT ), WM_QUIT, 0, 0 );

            return MRFROMSHORT( TRUE );

        case WM_BUTTON1UP :
            bButton1Down = FALSE;

            return MRFROMSHORT( TRUE );

        case WM_BUTTON2DOWN :
            bButton2Down = TRUE;
            if( bButton1Down )
                WinPostMsg( WinQueryWindow( hwnd, QW_PARENT ), WM_QUIT, 0, 0 );

            return MRFROMSHORT( TRUE );

        case WM_BUTTON2UP :
            bButton2Down = FALSE;

            return MRFROMSHORT( TRUE );

        case WM_BUTTON1CLICK :
            bDateTime = !bDateTime;
            WinInvalidateRect( hwnd, NULL, TRUE );

            return MRFROMSHORT( TRUE );

        case WM_BUTTON2CLICK :
        {
            WinSendMsg( WinQueryWindow( hwnd, QW_PARENT ), WM_HIDE_WINDOW, 0, 0 );

            WinOpenObject( WinQueryObject( "<WP_CLOCK>" ), 2, // OPEN_SETTING
                           FALSE );

            return MRFROMSHORT( TRUE );
        }
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

BOOL createKtbTimer( HAB hab, HWND hwndParent, HWND hwndOwner, PRECTL prcl )
{
    if( WinRegisterClass( hab, WC_KTBTIMER, timerWndProc, CS_SIZEREDRAW, 0 ))
        return WinCreateWindow( hwndParent, WC_KTBTIMER, "ktbTimer", WS_VISIBLE,
                    prcl->xLeft, prcl->yBottom,
                    prcl->xRight - prcl->xLeft + 1,
                    prcl->yTop - prcl->yBottom + 1,
                    hwndOwner, HWND_TOP,
                    ID_KTBTIMER, NULL, NULL );

    return FALSE;
}


