/*
        TCollection implementation file

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

    Environment :
        Source file   : tcollect.cpp
        Used compiler : Borland C++ 1.0 for OS/2
                        emx 0.9d + gcc 2.8.1

    Change Log :
        Original file : 'tcollect.cpp' of Borland C++ 3.1
        Modified by KO Myung-Hun
        Term of programming : 1999.02.21

        Modified by KO Myung-Hun
        Term of programming : 1999.02.26

        Modified contents :
            bug fix : In function TCollection::atPut(),
                      memory allocated to previous item is not released.

        Modified by KO Myung-Hun
        Term of programming : 1999.03.08

        Modified contents :
            add rangeThat() function.
            add rangeForEach() function.

        Modified by KO Myung-Hun
        Term of programming : 1999.03.10

        Modified contents :
            add class version of atInsert() and insert() function.
*/

#include <string.h>

#include "tcollect.h"

void *TCollection::at( int index )
{
    error( coNoError );
    if(( index < 0 ) ||  ( index >= _count ))
    {
        error( coIndexError );

        return NULL;
    }

    return _items[ index ];
}

void TCollection::atRemove( int index )
{
    error( coNoError );
    if(( index < 0 ) || ( index >= _count ))
    {
        error( coIndexError );

        return;
    }

    _count --;
    memmove( &_items[ index ], &_items[ index + 1 ], ( _count - index ) * sizeof( void * ));
}

void TCollection::atFree( int index )
{
    void *item = at( index );

    atRemove( index );
    freeItem( item );
}

void TCollection::atInsert( int index, void *item )
{
    error( coNoError );
    if(( index < 0 ) || ( index > _count ))
    {
        error( coIndexError );

        return;
    }

    if( _count == _limit )
        setLimit( _count + _delta );

    memmove( &_items[ index + 1 ], &_items[ index ], ( _count - index ) * sizeof( void * ));
    _count ++;

    _items[ index ] = item;
}

void TCollection::atInsert( int index, TCollection &col )
{
    error( coNoError );
    if(( index < 0 ) || ( index > _count ))
    {
        error( coIndexError );

        return;
    }

    if( col._count == 0 )
        return;

    if(( _count + col._count ) > _limit )
        setLimit(( _count + col._count + ( _delta - 1 )) / _delta * _delta );

    memmove( &_items[ index + col._count ], &_items[ index ], ( _count - index ) * sizeof( void * ));
    _count += col._count;

    for( int i = 0; i < col._count; i++ )
        _items[ index + i ] = col.copyItem( i );
}

void TCollection::atPut( int index, void *item )
{
    error( coNoError );
    if(( index < 0 ) || ( index >= _count ))
    {
        error( coIndexError );
        return;
    }

    freeItem( _items[ index ]); // appended routine by KO Myung-Hun

    _items[ index ] = item;
}

void TCollection::remove( void *item )
{
    atRemove( indexOf( item ));
}

void TCollection::removeAll()
{
    _count = 0;
}

int TCollection::firstThat( TTestFunc test, void *arg )
{
    return rangeThat( test, arg, 0, _count - 1, twForward );
}

int TCollection::lastThat( TTestFunc test, void *arg )
{
    return rangeThat( test, arg, 0, _count - 1, twBackward );
}

static int forward( TCollection::TTestFunc test, void **items, void *arg, int from, int to )
{
    for( int i = from; i <= to; i ++ )
    {
        if( test( i, items[ i ], arg ) != 0 )
            return i;
    }

    return -1;
}

static int backward( TCollection::TTestFunc test, void **items, void *arg, int from, int to )
{
    for( int i = to; i >= from; i -- )
    {
        if( test( i, items[ i ], arg ) != 0 )
            return i;
    }

    return -1;
}

typedef int ( *TMatchFunc )( TCollection::TTestFunc, void **, void *, int, int );

int TCollection::rangeThat( TTestFunc test, void *arg, int from, int to, TTOWARD toward  )
{
    TMatchFunc matchThat[ 2 ] = { forward, backward };

    error( coNoError );
    if(( from < 0 ) || ( from >= _count ) || ( to < 0 ) || ( to >= _count ))
    {
        error( coIndexError );
        return -1;
    }

    return matchThat[ toward ]( test, _items, arg, from, to );
}

void TCollection::forEach( TAppFunc action, void *arg )
{
    rangeForEach( action, arg, 0, _count - 1 );
}

void TCollection::rangeForEach( TAppFunc action, void *arg, int from, int to )
{
    error( coNoError );
    if(( from < 0 ) || ( from >= _count ) || ( to < 0 ) || ( to >= _count ))
    {
        error( coIndexError );
        return;
    }

    for( int i = from; i <= to; i++ )
        action( i, _items[ i ], arg );
}

void TCollection::free( void *item )
{
    remove( item );
    freeItem( item );
}

void TCollection::freeAll()
{
    for( int i =  0; i < _count; i++ )
        freeItem( at( i ));

    _count = 0;
}

void TCollection::freeItem( void *item )
{
    delete item;
}

#pragma warn -rvl
int TCollection::indexOf( void *item )
{
    error( coNoError );
    for( int i = 0; i < _count; i++ )
        if( item == _items[ i ] )
            return i;

    error( coIndexError );

    return -1;
}
#pragma warn .rvl

int TCollection::insert( void *item )
{
    int loc = _count;

    atInsert( _count, item );

    return loc;
}

int TCollection::insert( TCollection &col )
{
    int loc = _count;

    atInsert( _count, col );

    return loc;
}

void TCollection::pack()
{
    void **curDst = _items;
    void **curSrc = _items;
    void **last = _items + _count;

    while( curSrc < last )
    {
        if( *curSrc != NULL )
            *curDst ++ = *curSrc;

        curSrc ++;
    }

    _count = curDst - _items;
}

void TCollection::setLimit( int aLimit )
{
    if( aLimit < _count )
        aLimit =  _count;

    if( aLimit > maxCollectionSize )
        aLimit = maxCollectionSize;

    if( aLimit != _limit )
    {
        void **aItems;

        if( aLimit == 0 )
            aItems = NULL;
        else
        {
            aItems = new void *[ aLimit ];
            if( _count !=  0 )
                memcpy( aItems, _items, _count * sizeof( void * ));
        }

        delete _items;

        _items =  aItems;
        _limit =  aLimit;
    }
}

