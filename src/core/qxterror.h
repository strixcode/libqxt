
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/



#ifndef QXTERROR_H
#define QXTERROR_H
#include <qxtglobal.h>
#include <qxtnamespace.h>

/*!
\class QxtError QxtError

\inmodule QxtCore

\brief Information about Errors ocuring inside Qxt

*/

/*! \relates QxtError
droping an error inside a function that returns QxtError


short for return  QxtError(__FILE__,__LINE__,x);
*/
#define QXT_DROP(x) return QxtError(__FILE__,__LINE__,x);


/*! \relates QxtError
droping an error inside a function that returns QxtError

aditionaly specifies an errorstring \n

short for return  QxtError(__FILE__,__LINE__,x,s);
*/
#define QXT_DROP_S(x,s) return QxtError(__FILE__,__LINE__,x,s);


/*! \relates QxtError
droping no error inside a function that returns QxtError

short for return QxtError(__FILE__,__LINE__,Qxt::NoError);
*/
#define QXT_DROP_OK return QxtError(__FILE__,__LINE__,Qxt::NoError);


/*! \relates QxtError
forward a drop


drops from this function if the call inside dropped too.
the inner function must return or be a QxtError.

example
\code
QXT_DROP_F(critical_function());
\endcode

*/
#define QXT_DROP_F(call) {QxtError error_sds = call; if (error_sds != Qxt::NoError ) return error_sds; else (void)0; }

/*! \relates QxtError
check for errors

example
\code
QXT_DROP_SCOPE(error,critical_function())
 {
 qDebug()<<error;
 QXT_DROP_F(error);
 };
\endcode

short for  QxtError name = call; if (name != Qxt::NoError )

\warning: the errors name is valid outside the scope
*/
#define QXT_DROP_SCOPE(name,call) QxtError name = call; if (name != Qxt::NoError )






class QXT_CORE_EXPORT QxtError
{
public:
    QxtError(const char * file, long line, Qxt::ErrorCode errorcode, const char * errorString = 0);
    Qxt::ErrorCode errorCode() const;
    long line() const;
    const char * file() const;
    const char * errorString() const;
    operator Qxt::ErrorCode();


private:
    Qxt::ErrorCode errorcode_m;
    long line_m;
    const char * file_m;
    const char * errorString_m;
};

#endif
