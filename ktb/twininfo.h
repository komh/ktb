// start 1999.10.06
/*
        Header file for 'twininfo.cpp'

        Written by KO Myung-Hun
        Term of programming : 1999.10.06 - 1999.11.20

        Source file   : twininfo.h
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#ifndef __TWININFO_HEADER__
#define __TWININFO_HEADER__

#define INCL_PM
#include <os2.h>

#include <string.h>

#include "ktype.h"
#include "kobjs.h"
#include "tstring.h"
#include "tcollect.h"

class TWinInfo : public TKObject
{
public :
    TWinInfo() :
        _title( 0 ), _isShow( FALSE ), _isActive( FALSE ),
        _needHelp( FALSE )
    {
        memset( &_swe, 0, sizeof( _swe ));
    }

    TWinInfo( const SWENTRY &swe, TString &title, BOOL isShow, BOOL isActive ) :
        _swe( swe ), _title( &title ), _isShow( isShow ), _isActive( isActive ),
        _needHelp( FALSE )
    {
    }

    TWinInfo( const TWinInfo &wi ) :
        _title( 0 ), _isShow( FALSE ), _isActive( FALSE ),
        _needHelp( FALSE )

    {
        *this = wi;
    }

    virtual ~TWinInfo()
    {
        destroy( _title );
    }

    TWinInfo & operator= ( const TWinInfo &wi )
    {
        TKObject::operator= ( wi );

        destroy( _title );

        _swe = wi._swe;
        _title = ( wi._title == 0 ) ? ( 0 ) : (( TString * )&wi._title->dup());
        _isShow = wi._isShow;
        _isActive = wi._isActive;
        _needHelp = wi._needHelp;

        return *this;
    }

    virtual TKObject &dup() const
    {
        return *new TWinInfo( *this );
    }

    const SWENTRY &swe() const
    {
        return _swe;
    }

    virtual void swe( const SWENTRY &swe )
    {
        _swe = swe;
    }

    TString &title() const
    {
        return *_title;
    }

    virtual void title( TString &title )
    {
        destroy( _title );

        _title = &title;
    }

    BOOL isShow() const
    {
        return _isShow;
    }

    virtual void isShow( BOOL isShow )
    {
        _isShow = isShow;
    }

    BOOL isActive() const
    {
        return _isActive;
    }

    virtual void isActive( BOOL isActive )
    {
        _isActive = isActive;
    }

    BOOL needHelp() const
    {
        return _needHelp;
    }

    virtual void needHelp( BOOL needHelp )
    {
        _needHelp = needHelp;
    }

private :
    SWENTRY _swe;
    TString *_title;
    BOOL    _isShow;
    BOOL    _isActive;
    BOOL    _needHelp;
};

class TWinInfoCol : public TCollection
{
public :
    TWinInfoCol()
    {
    }

    TWinInfoCol( int aLimit, int aDelta ) : TCollection( aLimit, aDelta )
    {
    }

    TWinInfoCol( const TWinInfoCol &wic )
    {
        *this = wic;
    }

    virtual ~TWinInfoCol()
    {
    }

    TWinInfoCol & operator=( const TWinInfoCol &wic )
    {
        TCollection::operator=( wic );

        return *this;
    }

    virtual TKObject &dup() const
    {
        return *( new TWinInfoCol( *this ));
    }

    TWinInfo &at( int index )
    {
        return *( TWinInfo * )TCollection::at( index );
    }

private :
    virtual void freeItem( void *item );
    virtual void *copyItem( void *item );
};

#endif
