/*
        Window procedure for instant help message

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
        Term of programming : 1999.11.16 - 1999.11.18

        Source file   : ktbhelp.c
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#define INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "ktbhelp.h"

static
MRESULT EXPENTRY helpWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    static CHAR szText[ 256 ] = "";
    static HPS hps = NULLHANDLE;
    static USHORT width = 0, height = 0;

    switch( msg )
    {
        case WM_CREATE :
        {
            FONTMETRICS fm;

            strcpy( szText, (( PCREATESTRUCT )mp2 )->pszText );

            hps = WinGetPS( hwnd );
            GpiQueryFontMetrics( hps, sizeof( fm ), &fm );
            width = fm.lAveCharWidth;
            height = fm.lMaxAscender + fm.lMaxDescender;

            return MRFROMSHORT( FALSE );
        }

        case WM_DESTROY :
            WinReleasePS( hps );

            return MRFROMSHORT( FALSE );

        case WM_SETWINDOWPARAMS :
        {
            PWNDPARAMS  pwp;

            pwp = ( PWNDPARAMS )mp1;
            if( pwp->fsStatus & WPM_TEXT )
                strcpy( szText, pwp->pszText );

            return MRFROMSHORT( TRUE );
        }

        case WM_PAINT :
        {
            RECTL   rcl;

            WinBeginPaint( hwnd, hps, NULL );

            WinQueryWindowRect( hwnd, &rcl );

            WinDrawBorder( hps, &rcl, 1, 1, CLR_BLACK, CLR_YELLOW, DB_INTERIOR );
            WinDrawText( hps, -1, szText, &rcl, CLR_BLACK, CLR_YELLOW,
                         DT_CENTER | DT_VCENTER );

            WinEndPaint( hps );

            return MRFROMSHORT( 0 );
        }

        case WM_QUERY_CHAR_WIDTH :
            return MRFROMSHORT( width );

        case WM_QUERY_CHAR_HEIGHT :
            return MRFROMSHORT( height );
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

BOOL createKtbHelp( HAB hab, HWND hwndParent, HWND hwndOwner )
{
    if( WinRegisterClass( hab, WC_KTBHELP, helpWndProc, CS_SIZEREDRAW, sizeof( PVOID )))
        return WinCreateWindow( hwndParent, WC_KTBHELP, "ktbHelp", WS_VISIBLE,
                    0, 0, 0, 0,
                    hwndOwner, HWND_TOP,
                    ID_KTBHELP, NULL, NULL );

    return FALSE;
}


