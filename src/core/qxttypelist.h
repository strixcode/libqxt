#ifndef QXTTYPELIST_H
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

#define QXTTYPELIST_H
#include <qxtnull.h>

namespace QxtType
{
    struct NoExtend
    {
        typedef QxtNull head;
        enum { length = 0, extends = false };
    };

    template < typename T1 = QxtNull, typename T2 = QxtNull, typename T3 = QxtNull, typename T4 = QxtNull, typename T5 = QxtNull,
    typename T6 = QxtNull, typename T7 = QxtNull, typename T8 = QxtNull, typename T9 = QxtNull, typename EXTEND = QxtType::NoExtend >
    struct QxtTypeList;

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename EXTEND>
    struct QxtTypeList
    {
        typedef T1 head;
        typedef QxtTypeList<T2, T3, T4, T5, T6, T7, T8, T9, NoExtend, EXTEND> tail;
        typedef EXTEND extend;
        enum { length = tail::length + 1, extends = EXTEND::extends };
    };

    template<typename EXTEND>
    struct QxtTypeList<NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, EXTEND>
    {
        typedef typename EXTEND::head head;
        typedef typename EXTEND::tail tail;
        typedef EXTEND extend;
        enum { length = EXTEND::length, extends = EXTEND::extends };
    };

    template<>
    struct QxtTypeList<NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, NoExtend, QxtType::NoExtend>
    {
        typedef QxtNull extend;
        enum { length = 0, extends = false };
    };
}

#ifndef QXT_NO_USING
using QxtType::QxtTypeList;
#endif

#ifndef QXT_NO_MACROS
/*! \relates QxtTypeList
 * Declares a one-column tuple.
 */
#define Qxt1TypeList(a) QxtTypeList<a >

/*! \relates QxtTypeList
 * Declares a two-column tuple, similar to QPair.
 */
#define Qxt2TypeList(a, b) QxtTypeList<a, b >

/*! \relates QxtTypeList
 * Declares a three-column tuple, similar to QxtTriple.
 */
#define Qxt3TypeList(a, b, c) QxtTypeList<a, b, c >

/*! \relates QxtTypeList
 * Declares a four-column tuple.
 */
#define Qxt4TypeList(a, b, c, d) QxtTypeList<a, b, c, d >

/*! \relates QxtTypeList
 * Declares a five-column tuple.
 */
#define Qxt5TypeList(a, b, c, d, e) QxtTypeList<a, b, c, d, e >

/*! \relates QxtTypeList
 * Declares a six-column tuple.
 */
#define Qxt6TypeList(a, b, c, d, e, f) QxtTypeList<a, b, c, d, e, f >

/*! \relates QxtTypeList
 * Declares a seven-column tuple.
 */
#define Qxt7TypeList(a, b, c, d, e, f, g) QxtTypeList<a, b, c, d, e, f, g >

/*! \relates QxtTypeList
 * Declares an eight-column tuple.
 */
#define Qxt8TypeList(a, b, c, d, e, f, g, h) QxtTypeList<a, b, c, d, e, f, g, h >

/*! \relates QxtTypeList
 * Declares a nine-column tuple.
 */
#define Qxt9TypeList(a, b, c, d, e, f, g, h, i) QxtTypeList<a, b, c, d, e, f, g, h, i >

/*! \relates QxtTypeList
 * Declares an extended tuple with ten or more columns. Pay special attention to the syntax of the tenth parameter, which
 * must be a QxtTypeList, not a storage type.
\code
QxtLongTypeList(int, int, int, int, int, int, int, int, int, Qxt1TypeList(int)) tuple; // correct way to implement a 10-tuple
QxtLongTypeList(int, int, int, int, int, int, int, int, int, int) tuple;              // this will produce a (very long) compile-time error
\endcode
 */
#define QxtLongTypeList(a, b, c, d, e, f, g, h, i, extend) QxtTypeList<a, b, c, d, e, f, g, h, i, extend >
#endif

#endif // QXTTYPELIST_H
