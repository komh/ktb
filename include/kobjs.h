/*
        Abstract class definition header file

        Written by KO Myung-Hun
        Term of programming : 1999.03.06

        Source file   : kobjs.h
        Used compiler : Borland C++ 1.0 for OS/2
                        emx 0.9d + gcc 2.8.1
*/

#ifndef __KOBJS_H
#define __KOBJS_H

class TKObject
{
public :
    TKObject() : _error( 0 )
    {
    }

    TKObject( const TKObject &ko )
    {
        *this = ko;
    }

    virtual ~TKObject()
    {
    }

    TKObject & operator= ( const TKObject &ko )
    {
        _error = ko._error;

        return *this;
    }

    virtual TKObject &dup() const = 0;

    int error() const
    {
        return _error;
    }

    void clearError()
    {
        _error = 0;
    }

    static TKObject *none;

    static TKObject &copy( const TKObject &object )
    {
        return ( &object == 0 ) ? *none : object.dup();
    }

    static TKObject *copy( const TKObject *object )
    {
        return ( object == 0 ) ? 0 : &object->dup();
    }

    static void destroy( TKObject &object )
    {
        if( &object != 0 )
            delete &object;
    }

    static void destroy( TKObject *object )
    {
        if( object != 0 )
            delete object;
    }

protected :
    virtual void error( int ec )
    {
        _error = ec;
    }

private :
    int _error;
};

#define NOOBJECT    ( *TKObject::none )

#endif

