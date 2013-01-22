/*
        K TaskBar/2

        Copyright (C) 1999 by KO Myung-Hun

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Change Log :

        Written by KO Myung-Hun
        Term of programming : 1999.10.06 - 1999.11.18

        Source file   : ktb.cpp
        Used compiler : emx 0.9d + gcc 2.8.1

    History :

        v0.0.0  -   1999.11.20
                    initial version.
                    support only task switcing, system clock
*/

#define INCL_WIN
#include <os2.h>

#include <stdlib.h>

#include "twininfo.h"
#include "ktbhook.h"
#include "ktbtimer.h"
#include "ktbhelp.h"
#include "ktb.h"

#define ID_WINBASE      ID_KTBTIMER

#define CHAR_HEIGHT     ( ktbInfo->fm.lMaxAscender + ktbInfo->fm.lMaxDescender )
#define CHAR_WIDTH      ( ktbInfo->fm.lAveCharWidth )

#define QueryStringWidth( n ) ((( n ) + 3 ) * CHAR_WIDTH )

#define MAX_CHARS       15
#define MAX_WIDTH       ( QueryStringWidth( MAX_CHARS ))

#define WIN_INNER_SIZE  4
#define WIN_BAR_HEIGHT  ( CHAR_HEIGHT + WIN_INNER_SIZE * 2 )
#define WIN_HEIGHT      ( WIN_INNER_SIZE * 3 + WIN_BAR_HEIGHT )

#define EXIT_BTN_TITLE  (( PSZ )"kTaskBar/2")
#define EXIT_BTN_LEN    10
#define EXIT_BTN_WIDTH  ( QueryStringWidth( EXIT_BTN_LEN ))

MRESULT EXPENTRY windowProc( HWND, ULONG, MPARAM, MPARAM );

INT main( VOID )
{
    HAB     hab;
    HMQ     hmq;
    ULONG   flFrameFlags;
    HWND    hwndFrame;
    HWND    hwndClient;
    QMSG    qm;

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0);

    WinRegisterClass( hab, (PSZ)WC_KTB2, windowProc, CS_SIZEREDRAW, sizeof( PVOID ));

    flFrameFlags = FCF_DLGBORDER | FCF_NOBYTEALIGN;

    hwndFrame = WinCreateStdWindow(
                    HWND_DESKTOP,               // parent window handle
                    WS_VISIBLE,                 // frame window style
                    &flFrameFlags,              // window style
                    (PSZ)WC_KTB2,               // class name
                    (PSZ)"kTaskBar/2",          // window title
                    0L,                         // default client style
                    NULLHANDLE,                 // resource in exe file
                    ID_KTB2,                    // frame window id
                    &hwndClient                 // client window handle
                );

    WinSetWindowPos( hwndFrame, HWND_TOP,
                     0, 0, 0, 0, SWP_HIDE );

    if( hwndFrame != NULLHANDLE )
    {
        while( WinGetMsg( hab, &qm, NULLHANDLE, 0, 0 ))
            WinDispatchMsg( hab, &qm );

        WinDestroyWindow( hwndFrame );
    }

    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    return 0;
}

MRESULT EXPENTRY windowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    static HPS hps = NULLHANDLE;

    TKTBInfo *ktbInfo;

    ktbInfo = ( TKTBInfo * )WinQueryWindowPtr( hwnd, 0 );

    switch( msg )
    {
        case WM_CREATE :
        {
            THookData   hd;
            RECTL       rcl;

            ktbInfo = new TKTBInfo;
            WinSetWindowPtr( hwnd, 0, ktbInfo );

            ktbInfo->hwndParent = WinQueryWindow( hwnd, QW_PARENT );

            hps = WinGetPS( hwnd );
            GpiQueryFontMetrics( hps, sizeof( ktbInfo->fm ), &ktbInfo->fm );

            ktbInfo->pwic = new TWinInfoCol( 5, 5 );

            hd.hwnd = hwnd;

            hd.callRcl.xLeft = hd.winRcl.xLeft = 0;
            hd.callRcl.xRight = hd.winRcl.xRight =
                WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );

            hd.callRcl.yBottom = hd.winRcl.yBottom = 0;
            hd.winRcl.yTop = WIN_HEIGHT;
            hd.callRcl.yTop = hd.callRcl.yBottom + 1;

            hd.bShow = FALSE;
            hd.bProcessing = FALSE;

            installHook( &hd, &ktbInfo->phd );

            rcl.xLeft = WIN_INNER_SIZE;
            rcl.yBottom = WIN_INNER_SIZE;
            rcl.xRight = rcl.xLeft + ( EXIT_BTN_LEN + 3 ) * CHAR_WIDTH - 1;
            rcl.yTop = rcl.yBottom + WIN_BAR_HEIGHT - 1;

            createKtbTimer( WinQueryAnchorBlock( hwnd ), hwnd, hwnd, &rcl );

            createKtbHelp( WinQueryAnchorBlock( hwnd ), HWND_DESKTOP, hwnd );

            WinSetWindowPos( ktbInfo->hwndParent, HWND_TOP,
                             ktbInfo->phd->winRcl.xLeft,
                             ktbInfo->phd->winRcl.yBottom,
                             ktbInfo->phd->winRcl.xRight - ktbInfo->phd->winRcl.xLeft + 1,
                             ktbInfo->phd->winRcl.yTop - ktbInfo->phd->winRcl.yBottom + 1,
                             SWP_MOVE | SWP_SIZE | SWP_HIDE );

            WinStartTimer( WinQueryAnchorBlock( hwnd ), hwnd, 1, 1000 );

            return MRFROMSHORT( FALSE );
        }

        case WM_DESTROY :
            WinStopTimer( WinQueryAnchorBlock( hwnd ), hwnd, 1 );
            uninstallHook();

            WinReleasePS( hps );

            delete ktbInfo->pwic;
            delete ktbInfo;

            return MRFROMSHORT( 0 );

        case WM_TIMER :
            WinInvalidateRect( WinWindowFromID( hwnd, ID_KTBTIMER ), NULL, TRUE );
            return MRFROMSHORT( 0 );

        case WM_COMMAND :
            if( SHORT1FROMMP( mp2 ) == CMDSRC_PUSHBUTTON )
            {
                USHORT id;

                id = SHORT1FROMMP( mp1 ) - ( ID_WINBASE + 1 );
                WinSwitchToProgram( ktbInfo->pwic->at( id ).swe().hswitch );
                WinPostMsg( hwnd, WM_HIDE_WINDOW, 0, 0 );

                return MRFROMSHORT( 0 );
            }
            break;

        case WM_PAINT :
        {
            RECTL   rcl;

            WinBeginPaint( hwnd, hps, NULL );

            WinQueryWindowRect( hwnd, &rcl );
            WinFillRect( hps, &rcl, SYSCLR_BUTTONMIDDLE );

            WinEndPaint( hps );

            return MRFROMSHORT( 0 );
        }

        case WM_SHOW_WINDOW :
            if( !ktbInfo->phd->bShow && !ktbInfo->phd->bProcessing )
            {
                HAB         hab;
                PSWBLOCK    swb;
                ULONG       swbLength;
                ULONG       swCount;

                RECTL       rcl;
                INT         winWidth;
                INT         winChars;
                TString     title;

                ktbInfo->phd->bProcessing = TRUE;

                hab = WinQueryAnchorBlock( hwnd );
                swCount = WinQuerySwitchList( hab, NULL, 0 );

                swbLength = sizeof( ULONG ) + sizeof( SWENTRY ) * swCount;
                swb = ( PSWBLOCK )malloc( swbLength );
                WinQuerySwitchList( hab, swb, swbLength );

                for( ULONG i = 1; i < swCount; i++ )
                {
                    if( swb->aswentry[ i ].swctl.uchVisibility == SWL_VISIBLE )
                    {
                        TString title( swb->aswentry[ i ].swctl.szSwtitle );

                        while( title != title.replace( "\r\n", " " ));

                        ktbInfo->pwic->insert( new TWinInfo( swb->aswentry[ i ],
                            *new TString( title ), TRUE,
                            swb->aswentry[ i ].swctl.hwnd == WinQueryActiveWindow( HWND_DESKTOP )));
                    }
                }
                free( swb );

                WinQueryWindowRect( hwnd, &rcl );
                winWidth = ( rcl.xRight - rcl.xLeft + 1 ) -
                           ( WIN_INNER_SIZE + EXIT_BTN_WIDTH );
                winWidth = min(( winWidth - ( WIN_INNER_SIZE * ktbInfo->pwic->count()))/ \
                                 ktbInfo->pwic->count(), MAX_WIDTH );
                winChars = winWidth / CHAR_WIDTH;

                for( int i = 0; i < ktbInfo->pwic->count(); i++ )
                {
                    title = ktbInfo->pwic->at( i ).title();
                    if( title.len() > winChars )
                    {
                        int j;

                        for( j = 0; j < winChars - 3; j++ )
                        {
                            if(( BYTE )title[ j ] > 0x7F )
                                j++;
                        }

                        title.remove( j > ( winChars - 3 ) ? ( j - 2 ) : j, -1 );
                        title.concat( "..." );

                        ktbInfo->pwic->at( i ).needHelp( TRUE );
                    }

                    WinCreateWindow( hwnd, WC_BUTTON,
                                     title, WS_VISIBLE | BS_PUSHBUTTON | BS_TEXT,
                                     ( WIN_INNER_SIZE + EXIT_BTN_WIDTH ) + WIN_INNER_SIZE +
                                     ( winWidth + WIN_INNER_SIZE ) * i,
                                     WIN_INNER_SIZE,
                                     winWidth, WIN_BAR_HEIGHT,
                                     hwnd, HWND_TOP, ( ID_WINBASE + 1 ) + i,
                                     NULL, NULL );
                }

                WinSetWindowPos( ktbInfo->hwndParent, HWND_TOP,
                                 0, 0, 0, 0,
                                 SWP_SHOW | SWP_ZORDER );

                ktbInfo->phd->bShow = TRUE;
                ktbInfo->phd->bProcessing = FALSE;
            }

            return MRFROMSHORT( 0 );

        case WM_HIDE_WINDOW :
            if( ktbInfo->phd->bShow && !ktbInfo->phd->bProcessing )
            {
                ktbInfo->phd->bProcessing = TRUE;

                while( ktbInfo->pwic->count() > 0 )
                {
                    WinDestroyWindow( WinWindowFromID( hwnd, ID_WINBASE + ktbInfo->pwic->count()));
                    ktbInfo->pwic->atFree( ktbInfo->pwic->count() - 1 );
                }

                WinSetWindowPos( ktbInfo->hwndParent, HWND_TOP, 0, 0, 0, 0,
                                 SWP_HIDE );

                WinShowWindow( WinWindowFromID( HWND_DESKTOP, ID_KTBHELP ), FALSE );

                ktbInfo->phd->bShow = FALSE;
                ktbInfo->phd->bProcessing = FALSE;
            }

            return MRFROMSHORT( 0 );

        case WM_PRINT_ALL_TITLE :
        {
            INT     i;
            HWND    hwndHelp;

            for( i = 0; ( i < ktbInfo->pwic->count()) && \
                        ( WinWindowFromID( hwnd, ID_WINBASE + i + 1 ) != HWNDFROMMP( mp1 )); i++ );

            hwndHelp = WinWindowFromID( HWND_DESKTOP, ID_KTBHELP );

            if( i < ktbInfo->pwic->count())
            {
                if( ktbInfo->pwic->at( i ).needHelp())
                {
                    SWP     swp;
                    LONG    x, cxScreen, cxString;
                    USHORT  width, height;

                    WinSetWindowText( hwndHelp, ktbInfo->pwic->at( i ).title());

                    WinQueryWindowPos( HWNDFROMMP( mp1 ), &swp );

                    width = SHORT1FROMMR( WinSendMsg( hwndHelp, WM_QUERY_CHAR_WIDTH, 0, 0 ));
                    height = SHORT1FROMMR( WinSendMsg( hwndHelp, WM_QUERY_CHAR_HEIGHT, 0, 0 ));

                    cxScreen = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
                    cxString = width * ( ktbInfo->pwic->at( i ).title().len() + 3 );
                    x = swp.x + swp.cx / 2;

                    if( x + cxString > cxScreen )
                        x = cxScreen - cxString;
                       // = x - ( x + cxString - cxScreen )

                    WinSetWindowPos( hwndHelp, HWND_TOP,
                                     x, swp.y + swp.cy + WIN_INNER_SIZE * 3,
                                     cxString, height + WIN_INNER_SIZE * 2,
                                     SWP_SHOW | SWP_MOVE | SWP_SIZE );
                }
                else
                    WinShowWindow( hwndHelp, FALSE );
            }
            else
                WinShowWindow( hwndHelp, FALSE );

            return MRFROMSHORT( 0 );
        }
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}


