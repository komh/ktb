/*
        Header file for 'tcollect.cpp'

        Original file : 'tvobjs.h' of Borland C++ 3.1
        Modified by KO Myung-Hun
        Term of programming : 1999.02.21 - 1999.02.23

        Source file   : tcollect.h
        Used compiler : Borland C++ 1.0 for OS/2
                        emx 0.9d + gcc 2.8.1
*/

#ifndef _TCOLLECT_HEADER
#define _TCOLLECT_HEADER

#include <limits.h>

#include "kobjs.h"

class TCollection : public TKObject
{
public :
    static const maxCollectionSize = INT_MAX / 1024;
    static const coNoError = 0;
    static const coIndexError = 1;

    typedef enum { twForward, twBackward } TTOWARD;
    typedef int  ( *TTestFunc )( int index, void *item, void *arg );
    typedef void ( *TAppFunc )( int index, void *item, void *arg );

    TCollection() : _items( 0 ), _count ( 0 ), _limit( 0 ), _delta( 0 )
    {
    }

    TCollection( int aLimit, int aDelta ) :
        _items( 0 ), _count( 0 ), _limit( 0 ), _delta( aDelta )
    {
        setLimit( aLimit );
    }

    TCollection( const TCollection &col ) : _items( 0 ), _count( 0 ), _limit( 0 )
    {
        *this = col;
    }

    virtual ~TCollection()
    {
        freeAll();
        setLimit( 0 );
    }

    TCollection & operator= ( const TCollection &col )
    {
        freeAll();
        setLimit( 0 );

        TKObject::operator= ( col );

        _delta = col._delta;
        setLimit( col._limit );

        insert(( TCollection & )col );

        return *this;
    }

    virtual TKObject &dup() const = 0;

    void *at( int index );
    virtual int indexOf( void *item );

    void atFree( int index );
    void free( void *item );
    void freeAll();

    void atRemove( int index );
    void remove( void *item );
    void removeAll();

    void atInsert( int index, void *item );
    void atInsert( int index, TCollection &col );
    void atPut( int index, void *item );
    virtual int insert( void *item );
    virtual int insert( TCollection &col );

    int firstThat( TTestFunc test, void *arg );
    int lastThat( TTestFunc test, void *arg );
    int rangeThat( TTestFunc test, void *arg, int from, int to, TTOWARD toward );
    void forEach( TAppFunc action, void *arg );
    void rangeForEach( TAppFunc action, void *arg, int from, int to );

    void pack();
    virtual void setLimit( int aLimit );

    int count() const
    {
        return _count;
    }

protected :
    void **_items;
    int _count;
    int _limit;
    int _delta;

private :
    virtual void freeItem( void *item );

    virtual void *copyItem( int index )
    {
        return copyItem( at( index ));
    }

    virtual void *copyItem( void *item ) = 0;
};
#endif
