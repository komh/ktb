/*
        Header file for 'tstring.cpp'

        Written by KO Myung-Hun
        Term of programming : 1999.03.16

        Source file   : tstring.h
        Used compiler : Borland C++ 1.0 for OS/2
                        emx 0.9d + gcc 2.8.1
*/

#ifndef __TSTRING_H
#define __TSTRING_H

#include "ktype.h"
#include "kobjs.h"

class TString : public TKObject
{
public :
    static const seNoError      = 0;
    static const seIndexError   = 1;

    static const soNone         = 0;
    static const soCaseIgnore   = 1;
    static const soBackward     = 2;
    static const soWord         = 4;
    static const soAll          = soCaseIgnore | soBackward | soWord;

    static const maxCode        = 0xFF;

    static const defaultOption  = soNone;
    static const defaultDelta   = 25;

    TString();
    TString( char ch );
    TString( const char *string, int length = -1 );
    TString( const unsigned char *string, int length = -1 );
    TString( const TString &string );
    virtual ~TString();

    virtual TKObject &dup() const;

    TString & operator=  ( const TString &string );
    TString operator+  ( const TString &string ) const;
    TString & operator+= ( const TString &string );

    BOOL operator<  ( const TString &string ) const;
    BOOL operator== ( const TString &string ) const;
    BOOL operator!= ( const TString &string ) const;
    BOOL operator>  ( const TString &string ) const;

    char operator[] ( int index );

    operator const char * () const;
    operator const unsigned char * () const;

    virtual TString & upper();
    virtual TString & lower();

    virtual TString & reverse();

    virtual TString copy( int index, int length );
    virtual TString & concat( const TString &string );
    virtual TString & insert( const TString &string, int index );
    virtual TString & remove( int index, int length );
    virtual int search( const TString &string, int index = 0 );
    virtual TString & replace( const TString &target, const TString &newStr, int index = 0 );

    virtual int searchSet( const TString &string, BOOL inSet = TRUE, int index = 0 );

    int len() const
    {
        return _len;
    }

    BOOL isOnCaseIgnore() const
    {
        return ( BOOL )(( _option & soCaseIgnore ) == soCaseIgnore );
    }

    BOOL isOnBackward() const
    {
        return ( BOOL )(( _option & soBackward ) == soBackward );
    }

    BOOL isOnWord() const
    {
        return ( BOOL )(( _option & soWord ) == soWord );
    }

    int option() const
    {
        return _option;
    }

    virtual void option( int opt )
    {
        _option = opt;
    }

    int delta() const
    {
        return _delta;
    }

    virtual void delta( int dt )
    {
        _delta = dt;
    }

private :
    char *_str;
    int _len;
    int _option;
    int _size;
    int _delta;
    int *_posDelta;

    int calcSize();
};

inline
TString::operator const char * () const
{
    return ( const char * )_str;
}

inline
TString::operator const unsigned char * () const
{
    return ( const unsigned char * )_str;
}

inline
int TString::calcSize()
{
    return ((( _len + 1 ) + ( _delta - 1 )) / _delta * _delta );
}

#endif


