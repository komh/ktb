/*
        implementation releated to window information
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
        Term of programming : 1999.10.06 - 1999.11.20

        Source file   : twininfo.cpp
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#include "twininfo.h"

void TWinInfoCol::freeItem( void *item )
{
    delete ( TWinInfo * )item;
}

void *TWinInfoCol::copyItem( void *item )
{
    return &((( TWinInfo * )item ) -> dup());
}

