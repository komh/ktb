/*
        String class

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
        Source file   : tstring.cpp
        Used compiler : Borland C++ 1.0 for OS/2
                        emx 0.9d + gcc 2.8.1

    Change Log :

        Written by KO Myung-Hun
        Term of programming : 1999.03.16


        Modified by KO Myung-Hun
        Term of programming : 1999.10.21

        Contents :
            Bug fixed : TString::insert() member function do not
                        make null-terminated string.
        Modified by KO Myung-Hun
        Term of programming : 1999.10.26

        Contents :
            Bug fixed : TString::replace() member function always
                        remove characters as many as length of target from 0 index
*/

#include <string.h>
#include <ctype.h>

#include "ktype.h"
#include "tstring.h"

TString::TString() :
    _str( NULL ), _len( 0 ), _option( 0 ), _size( 0 ),
    _delta( 0 ), _posDelta( NULL )
{
    _posDelta = new int [ maxCode + 1 ];
}

TString::TString( char ch ) :
    _str( NULL ), _posDelta( NULL )
{
    _len = 1;

    _delta = defaultDelta;

    _size = calcSize();

    _str = new char[ _size ];

    _str[ 0 ] = ch;
    _str[ _len ] = '\0';

    _option = defaultOption;

    _posDelta = new int [ maxCode + 1 ];
}

TString::TString( const char *string, int length ) :
    _str( NULL ), _posDelta( NULL )
{
    _len = length;
    if( length == -1 )
        _len = strlen( string );

    _delta = defaultDelta;

    _size = calcSize();

    _str = new char[ _size ];

    memmove( _str, string, _len );
    _str[ _len ] = '\0';

    _option = defaultOption;

    _posDelta = new int [ maxCode + 1 ];

}

TString::TString( const unsigned char *string, int length ) :
    _str( NULL ), _posDelta( NULL )
{
    _len = length;
    if( length == -1 )
        _len = strlen(( const char * )string );

    _delta = defaultDelta;

    _size = calcSize();

    _str = new char[ _size ];

    memmove( _str, string, _len );
    _str[ _len ] = '\0';

    _option = defaultOption;

    _posDelta = new int [ maxCode + 1 ];
}

TString::TString( const TString &string ) : _str( NULL ), _posDelta( NULL )
{
    _posDelta = new int [ maxCode + 1 ];

    *this = string;
}

TString::~TString()
{
    delete[] _str;
    delete[] _posDelta;
}

TKObject &TString::dup() const
{
    return *( new TString( *this ));
}

TString & TString::operator=  ( const TString &string )
{
    delete[] _str;

    _len = string._len;
    _delta = string._delta;
    _size = string._size;
    _option = string._option;

    _str = new char[ _size ];
    memmove( _str, string._str, _len + 1 );

    return *this;
}

TString TString::operator+  ( const TString &string ) const
{
    TString temp;
    char *s;

    s = new char[ _len + string._len + 1 ];
    memmove( s, _str, _len );
    memmove( s + _len, string._str, string._len + 1 );

    temp = s;

    delete[] s;

    return temp;
}

TString & TString::operator+= ( const TString &string )
{
    return concat( string );
}

BOOL TString::operator<  ( const TString &string ) const
{
    int len;
    int result;

    len = min( _len, string._len );

    result = ( isOnCaseIgnore() == TRUE ) ? \
                memicmp( _str, string._str, len ) : memcmp( _str, string._str, len );

    if( result == 0 )
        result = _len - string._len;

    return ( BOOL )( result < 0 );
}

BOOL TString::operator== ( const TString &string ) const
{
    int result;

    if( _len != string._len )
        return FALSE;

    result = ( isOnCaseIgnore() == TRUE ) ? \
                memicmp( _str, string._str, _len ) : memcmp( _str, string._str, _len );

    return ( BOOL )( result == 0 );
}

BOOL TString::operator!= ( const TString &string ) const
{
    return ( BOOL )!( *this == string );
}

BOOL TString::operator>  ( const TString &string ) const
{
    int len;
    int result;

    len = min( _len, string._len );

    result = ( isOnCaseIgnore() == TRUE ) ? \
                memicmp( _str, string._str, len ) : memcmp( _str, string._str, len );

    if( result == 0 )
        result = _len - string._len;

    return ( BOOL )( result > 0 );
}

char TString::operator[] ( int index )
{
    clearError();
    if(( index < 0 ) || ( index >= _len ))
    {
        error( seIndexError );

        return '\0';
    }

    return _str[ index ];
}

TString & TString::upper()
{
    for( int i = 0; i < _len; i++ )
        _str[ i ] = toupper( _str[ i ]);

    return *this;
}

TString & TString::lower()
{
    for( int i = 0; i < _len; i++ )
        _str[ i ] = tolower( _str[ i ]);

    return *this;
}

TString & TString::reverse()
{
    char *s;
    int i, j;

    s = new char[ _size ];

    for( i = 0, j = _len - 1; i < _len; i++, j-- )
        s[ i ] = _str[ j ];

    s[ i ] = '\0';

    delete[] _str;

    _str = s;

    return *this;
}

TString TString::copy( int index, int length )
{
    TString temp;
    char *s;

    clearError();
    if(( index < 0 ) || ( index >= _len ))
    {
        error( seIndexError );

        return ( TString )'\0';
    }

    if(( length == -1 ) || ( index + length > _len ))
        length = _len - index;

    s = new char[ length + 1 ];
    memmove( s, _str + index, length );
    s[ length ] = '\0';

    temp = s;

    delete[] s;

    return temp;
}

TString & TString::concat( const TString &string )
{
    return insert( string, _len );
}

TString & TString::insert( const TString &string, int index )
{
    int length;

    clearError();
    if(( index < 0 ) || ( index > _len ))
    {
        error( seIndexError );

        return *this;
    }

    length = _len;

    _len += string._len;
    if( _len > _size )
    {
        char *s;

        _size = calcSize();
        s = new char[ _size ];
        memmove( s, _str, length + 1 );

        delete[] _str;

        _str = s;
    }

    // bug fixed : ( length - index ) not including null character
    memmove( _str + index + string._len, _str + index, length - index + 1 );
    memmove( _str + index, string._str, string._len );

    return *this;
}

TString & TString::remove( int index, int length )
{
    clearError();
    if(( index < 0 ) || ( index > _len ))
    {
        error( seIndexError );

        return *this;
    }

    if(( length == -1 ) || ( index + length > _len ))
        length = _len - index;

    memmove( _str + index, _str + index + length, _len - ( index + length ) + 1 );
    _len -= length;

    return *this;
}

// Boyer-Moore method
static int _search( TString &str, TString &subStr, int index, int *delta )
{
    int i, j, start;

    for( i = 0; i <= TString::maxCode; i++ )
         delta[ i ] = subStr.len();

    for( i = 0; i < subStr.len(); i++ )
         delta[ subStr[ i ]] = subStr.len() - ( i + 1 );

    for( i = subStr.len() - 1 + index; start = i, i < str.len(); )
    {
        for( j = subStr.len() - 1; subStr[ j ] == str[ i ]; i--, j-- )
        {
            if( j == 0 )
                return i;
        }

        i += delta[ str[ i ]];
        if( i <= start )
            i = start + 1;
    }

    return -1;
}

int TString::search( const TString &string, int index )
{
    int result;

    clearError();
    if(( index < 0 ) || ( index > _len ))
    {
        error( seIndexError );

        return -1;
    }

    if((( index + string._len ) > _len ) || ( string._len < 1 ))
        return -1;

    TString &str = ( TString & )this->dup();
    TString &subStr = ( TString & )string.dup();

    if( isOnCaseIgnore() == TRUE )
    {
        str.upper();
        subStr.upper();
    }

    if( isOnBackward() == TRUE )
    {
        str.reverse();
        subStr.reverse();
    }

    result = _search( str, subStr, index, _posDelta );

    if(( result != -1 ) && ( isOnBackward() == TRUE ))
        result = str.len() - ( result + subStr.len());

    destroy( str );
    destroy( subStr );

    return result;
}

TString & TString::replace( const TString &target, const TString &newStr, int index )
{
    // bug fixed : replaced index is always 0
    if(( index = search( target, index )) != -1 )
    {
        remove( index, target._len );
        insert( newStr, index );
    }

    return *this;
}

int TString::searchSet( const TString &string, BOOL inSet, int index )
{
    int i;

    clearError();
    if(( index < 0 ) || ( index > _len ))
    {
        error( seIndexError );

        return -1;
    }

    TString &str = ( TString & )this->dup();
    TString &set = ( TString & )string.dup();

    if( isOnCaseIgnore() == TRUE )
    {
        str.upper();
        set.upper();
    }

    for( i = 0; i <= maxCode; i++ )
        _posDelta[ i ] = ( int )!inSet;

    for( i = 0; i < set.len(); i++ )
        _posDelta[ set[ i ]] = ( int )inSet;

    if( isOnBackward() == TRUE )
        for( i = str.len() - 1; ( i >= index ) && ( _posDelta[ str[ i ]] == FALSE ); i-- );
    else
    {
        for( i = index; ( i < str.len()) && ( _posDelta[ str[ i ]] == FALSE ); i++ );

        if( i == str.len() )
            i = -1;
    }

    destroy( str );
    destroy( set );

    return i;
}


