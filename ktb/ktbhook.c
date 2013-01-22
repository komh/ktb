/*
        DLL for menu auto selecting by mouse position
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
        Term of programming : 1999.10.13 - 1999.11.16

        Source file   : ktbhook.c
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#define INCL_DOSMODULEMGR
#define INCL_PM
#include <os2.h>

#include <string.h>

#include "ktbhook.h"

#define DLL_NAME    "KTBHOOK"

//#define XIT_COMPATIBLE

static THookData hd = { 0, };
static HMODULE hm = NULLHANDLE;
static SHORT prevId = MIT_NONE;

static BOOL isMenuClass( HWND hwnd )
{
   CHAR achClass[ 256 ];

   WinQueryClassName( hwnd, sizeof( achClass ), achClass );

   return ( BOOL )( WinFindAtom( WinQuerySystemAtomTable(), achClass ) == LOUSHORT( WC_MENU ));
}

#ifdef XIT_COMPATIBLE
VOID EXPENTRY sendMsgHook( HAB hab, PSMHSTRUCT psmhs, BOOL fInterTask )
{
    static HWND hwndInit = NULLHANDLE;

    SHORT   id;
    HWND    hwnd;

    switch( psmhs->msg )
    {
        case WM_MENUEND :
            hwnd = WinQueryWindow( HWNDFROMMP( psmhs->mp2 ), QW_OWNER );
            if( isMenuClass( hwnd ))
            {
                prevId = SHORT1FROMMR( WinSendMsg( hwnd, MM_QUERYSELITEMID,
                                           MPFROM2SHORT( 0, FALSE ),
                                           MPFROMSHORT( 0 )));
            }
            else
                prevId = MIT_NONE;

            break;
    }
}
#endif

BOOL EXPENTRY inputHook( HAB hab, PQMSG pQmsg, USHORT fsOptions )
{
    static POINTL _ptl = { -1, -1 };

    if(( pQmsg->msg == WM_MOUSEMOVE ) &&
       (( _ptl.x != pQmsg->ptl.x ) || ( _ptl.y != pQmsg->ptl.y )))
    {
        _ptl.x = pQmsg->ptl.x;
        _ptl.y = pQmsg->ptl.y;

        if( WinQueryWindow( pQmsg->hwnd, QW_PARENT ) == hd.hwnd )
            WinPostMsg( hd.hwnd, WM_PRINT_ALL_TITLE, MPFROMHWND( pQmsg->hwnd ), 0 );
        else if( isMenuClass( pQmsg->hwnd ))
        {
            POINTL      ptl;
            SHORT       count;
            SHORT       selId;
            SHORT       id;
            RECTL       rcl;
            MENUITEM    mi;
            INT         i;

            ptl = pQmsg->ptl;

            if((( selId = SHORT1FROMMR( WinSendMsg( pQmsg->hwnd, MM_QUERYSELITEMID,
                                       MPFROM2SHORT( 0, FALSE ), MPVOID ))) != MIT_NONE ) ||
                isMenuClass( WinQueryWindow( pQmsg->hwnd, QW_OWNER )))
            {
                ptl.x = SHORT1FROMMP( pQmsg->mp1 );
                ptl.y = SHORT2FROMMP( pQmsg->mp1 );

                count = SHORT1FROMMR( WinSendMsg( pQmsg->hwnd, MM_QUERYITEMCOUNT, MPVOID, MPVOID ));

                for( i = 0; i < count; i++ )
                {
                    id = LONGFROMMR( WinSendMsg( pQmsg->hwnd, MM_ITEMIDFROMPOSITION,
                                                   MPFROMSHORT( i ), MPVOID ));

                    if( id == MIT_ERROR )
                        continue;

                    WinSendMsg( pQmsg->hwnd, MM_QUERYITEMRECT,
                                MPFROM2SHORT( id, FALSE ), MPFROMP( &rcl ));

#ifdef XIT_COMPATIBLE
                    if( WinPtInRect( hab, &rcl, &ptl ) &&
                        (( prevId != id ) || ( selId != id )))
#else
                    if( WinPtInRect( hab, &rcl, &ptl ))
#endif

                    {
                        WinSendMsg( pQmsg->hwnd, MM_SELECTITEM,
                                    MPFROM2SHORT( id, FALSE ),
                                    MPFROM2SHORT( 0, FALSE ));

                        WinSendMsg( pQmsg->hwnd, MM_QUERYITEM, MPFROM2SHORT( id, FALSE ),
                                    MPFROMP( &mi ));

                        if((( mi.afStyle & MIS_SUBMENU ) == MIS_SUBMENU ) &&
                           ( SHORT1FROMMR( WinSendMsg( mi.hwndSubMenu,
                                               MM_QUERYITEMCOUNT, 0, 0 )) > 0 ))
                        {
                            if( !WinIsWindowShowing( mi.hwndSubMenu ))
                                WinSendMsg( pQmsg->hwnd, WM_CHAR,
                                            MPFROMSH2CH( KC_VIRTUALKEY, 0, 0),
                                            MPFROM2SHORT( 0, VK_RIGHT ));
#ifdef XIT_COMPATIBLE
                            else if( SHORT1FROMMR( WinSendMsg( mi.hwndSubMenu,
                                                  MM_ITEMPOSITIONFROMID,
                                                  MPFROM2SHORT( prevId, FALSE ),
                                                  0 )) == ( USHORT )MIT_NONE )
                                prevId = id;

                        }
                        else
                            prevId = id;
#else
                        }
                        prevId = id;
#endif
                        break;
                    }
                }
            }
        }
        else if( !WinPtInRect( hab, &hd.winRcl, &pQmsg->ptl ) && hd.bShow && !hd.bProcessing)
            WinPostMsg( hd.hwnd, WM_HIDE_WINDOW, 0, 0 );
        else if( WinPtInRect( hab, &hd.callRcl, &pQmsg->ptl ) && !hd.bShow && !hd.bProcessing )
            WinPostMsg( hd.hwnd, WM_SHOW_WINDOW, 0, 0 );
    }

    return FALSE;
}

BOOL EXPENTRY installHook( PHookData phd, PHookData *pphd )
{
    if( DosQueryModuleHandle( DLL_NAME, &hm ) != 0 )
        return FALSE;

    hd = *phd;

    *pphd = &hd;


    WinSetHook( WinQueryAnchorBlock( hd.hwnd ),
                NULLHANDLE, HK_INPUT, ( PFN )inputHook, hm );

#ifdef XIT_COMPATIBLE
    WinSetHook( WinQueryAnchorBlock( hd.hwnd ),
                NULLHANDLE, HK_SENDMSG, ( PFN )sendMsgHook, hm );
#endif

    return TRUE;
}

VOID EXPENTRY uninstallHook( VOID )
{
    WinReleaseHook( WinQueryAnchorBlock( hd.hwnd ),
                    NULLHANDLE, HK_INPUT, ( PFN )inputHook, hm );

#ifdef XIT_COMPATIBLE
    WinReleaseHook( WinQueryAnchorBlock( hd.hwnd ),
                    NULLHANDLE, HK_SENDMSG, ( PFN )sendMsgHook, hm );
#endif

    WinBroadcastMsg(HWND_DESKTOP, WM_NULL, 0, 0, BMSG_FRAMEONLY | BMSG_POST);
}

