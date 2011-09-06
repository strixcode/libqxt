
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

#ifndef QXTPAIRLIST_H
#define QXTPAIRLIST_H

/*!
\class QxtPairList QxtPairList

\ingroup kit

\brief Searchable List of Pairs


Pair list provides a list with two values, a bit like QHash, but with the possibility to operate on both values.

in contrast to QHash, every entry has a unique id, you can work with.  like a QList.

\code
QxtPairList<int,int> list;

list.append(1,2);
list.append(1,5);
list.append(5,6);

qDebug()<< list.find(1);    //  "0,1"
qDebug()<< list.find(SKIP,5);    //  "2"
qDebug()<< list.find(5);    //  "2"

\endcode
you may allso play around with the list itself

\code
list.list.append(qMakePair(1,2));
\endcode


*/


#include <QList>
#include <QPair>
#include <QxtNullable>
#include <qxtglobal.h>



template <typename T, typename K>
class QXT_CORE_EXPORT QxtPairList
{


public:

    QxtPairList()
    {}

    QxtPairList(const QxtPairList<T, K> & other)
    {
        list = other.list;
    }

    QxtPairList operator= (const QxtPairList<T, K> & other)
    {
        list = other.list;
    }


    void append(T v1, K v2)
    {
        list.append(qMakePair(v1, v2));
    }


    /*! \brief search entries by match

    both arguments are optional, due to the use of QxtNullable

    \code
    find(SKIP,v2);
    find(v1,SKIP);
    find(v1);
    \endcode
    are all valid
    */

    QList<int> find(qxtNull(T, v1) , qxtNull(K, v2))
    {
        QList<int> found;

        if ((!v1.isNull()) and(!v2.isNull()))
        {
            for (int i = 0;i < list.count();i++)
                if ((list[i].first() == v1)and(list[i].second() == v2))
                    found.append(i);

            return found;
        }

        if ((!v1.isNull()) and(v2.isNull()))
        {
            for (int i = 0;i < list.count();i++)
                if (list[i].first() == v1)
                    found.append(i);

            return found;
        }

        if ((v1.isNull()) and(!v2.isNull()))
        {
            for (int i = 0;i < list.count();i++)
                if (list[i].second() == v2)
                    found.append(i);

            return found;
        }



    }



    ///remove an entries position by position
    void remove(int nr)
    {
        list.removeAt(nr);
    }

    ///remove a list of entries by position
    void remove(QList<int> nrs)
    {
        int i;
        Q_FOREACH(i, nrs)
            list.removeAt(i);
    }




    /*! \brief operate on the list directly

    you may use the internal list directly, but be carefull
    don't expect to work the QxPairList to work normal if you mess around with it.
    */


    QList<QPair<T, K> > list;
};

#endif // QXTPAIRLIST_H
