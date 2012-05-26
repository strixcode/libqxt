#include "qxtapplication.h"
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

#include "qxtapplication_p.h"

/*!
    \class QxtApplication
    \inmodule QxtWidgets
    \brief The QxtApplication class is an extended QApplication with support for native event filters.

    QxtApplication lets you install native event filters. This gives an
    easy and straightforward access to platform specific native events.

    \sa QxtNativeEventFilter
 */

/*!
    \fn QxtApplication::instance()

    Returns a pointer to the instance of the application object.

    A convenience macro \l qxtApp is also available.
 */

/*!
    \macro qxtApp
    \relates QxtApplication
    \inmodule QxtWidgets

    A global pointer referring to the unique application object. It is
    equivalent to the pointer returned by QxtApplication::instance().

    \sa QxtApplication::instance()
*/

/*!
    Constructs a new QxtApplication with \a argc and \a argv.

    \sa QApplication::QApplication()
 */
QxtApplication::QxtApplication(int& argc, char** argv)
        : QApplication(argc, argv)
{
}

/*!
    Constructs a new QxtApplication with \a argc, \a argv and \a GUIenabled.

    \sa QApplication::QApplication()
 */
QxtApplication::QxtApplication(int& argc, char** argv, bool GUIenabled)
        : QApplication(argc, argv, GUIenabled)
{
}

/*!
    Constructs a new QxtApplication with \a argc, \a argv and \a type.

    \sa QApplication::QApplication()
 */
QxtApplication::QxtApplication(int& argc, char** argv, Type type)
        : QApplication(argc, argv, type)
{
}

/*!
    Destructs the QxtApplication.

    \sa QApplication::~QApplication()
 */
QxtApplication::~QxtApplication()
{
}

/*!
    Installs a native event \a filter.

    A native event filter is an object that receives all native events before they reach
    the application object. The filter can either stop the native event or forward it to
    the application object. The filter receives native events via its platform specific
    native event filter function. The native event filter function must return \c true
    if the event should be filtered, (i.e. stopped); otherwise it must return \c false.

    If multiple native event filters are installed, the filter that was installed last
    is activated first.

    \sa removeNativeEventFilter()
*/
void QxtApplication::installNativeEventFilter(QxtNativeEventFilter* filter)
{
    if (!filter)
        return;

    qxt_d().nativeFilters.removeAll(filter);
    qxt_d().nativeFilters.prepend(filter);
}

/*!
    Removes a native event \a filter. The request is ignored if such a native
    event filter has not been installed.

    \sa installNativeEventFilter()
*/
void QxtApplication::removeNativeEventFilter(QxtNativeEventFilter* filter)
{
    qxt_d().nativeFilters.removeAll(filter);
}
