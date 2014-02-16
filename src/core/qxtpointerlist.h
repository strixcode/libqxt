
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

#ifndef QXTPOINTERLIST_H
#define QXTPOINTERLIST_H

#include <QList>
#include <QObject>
#include "qxtglobal.h"

class QXT_CORE_EXPORT QxtPointerListDeleter : public QObject
{
    Q_OBJECT
protected:
    virtual void removeThisObject(QObject * obj) = 0;
private Q_SLOTS:
    void removeSender();
};

template<class T>
class QxtPointerList : public QxtPointerListDeleter, public QList<T*>
{
public:
    ///constructs a new QxtPointerList
    QxtPointerList(): QList<T*>()
    {
    }
    ///destructs the QxtPointerList
    ~QxtPointerList()
    {
        QList<T*>::clear();
    }
    ///\reimp
    QxtPointerList(const QxtPointerList<T> & other): QxtPointerListDeleter(), QList<T*>(other)
    {
        for (int i = 0;i < other.size();i++)
        {
            addThisObject(other.at(i));
        }
    }
    ///\reimp
    void append(T*  value)
    {
        addThisObject(value);
        QList<T*>::append(value);
    }
    ///\reimp
    void insert(int i, T * value)
    {
        addThisObject(value);
        QList<T*>::insert(i, value);
    }
    ///\reimp
    typename QList<T*>::iterator insert(typename QList<T*>::iterator before,  T*  value)
    {
        addThisObject(value);
        return QList<T*>::insert(before, value);
    }
    ///\reimp
    QxtPointerList<T> operator+ (const QxtPointerList<T> & other) const
    {
        QxtPointerList<T> m = *this;
        m += other;
        return m;
    }
    ///\reimp
    QxtPointerList<T> & operator+= (const QxtPointerList<T> & other)
    {
        Q_FOREACH(T*t, other)
        {
            *this << t;
        }
        return *this;
    }
    ///\reimp
    QxtPointerList<T> & operator+= (T*  value)
    {
        addThisObject(value);
        QList<T*>::operator+=(value);
        return *this;
    }
    ///\reimp
    QxtPointerList<T> & operator<< (const QxtPointerList<T> & other)
    {
        *this += other;
        return *this;
    }
    ///\reimp
    QxtPointerList<T> & operator<< (T* value)
    {
        *this += value;
        return *this;
    }
    ///\reimp
    QxtPointerList<T> & operator= (const QxtPointerList<T> & other)
    {
        QList<T*>::clear();
        Q_FOREACH(T*t, other)
        {
            *this << t;
        }
        return *this;
    }

protected:
    ///reimplement to access objects before they are removed
    virtual void removeThisObject(QObject * obj)
    {
        this->removeAll(reinterpret_cast<T*>(obj));
    }
    ///reimplement to access objects before they are added
    virtual void addThisObject(QObject * obj)
    {
        QObject::connect(obj, SIGNAL(destroyed(QObject *)), this, SLOT(removeSender()));
    }
};

#endif // QXTPOINTERLIST_H
