
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

/**************************************************************************
This is private API. it might change at any time without warning.
****************************************************************************/


#ifndef QxtBdb_H_kpasd
#define QxtBdb_H_kpasd

#include <QFlags>
#include <QBuffer>
#include <QDataStream>
#include <QMetaType>

#include <QString>
#include <qxtglobal.h>
#include <cstdlib>
#include <cstdio>


///its impossible to forward anyway,
namespace BerkeleyDB
{
    extern "C"
    {
#include <db.h>
    }

    /// aparantly MSVC and GCC have different understanding of what goes into a namespace and what not.
#ifndef Q_CC_MSVC
    typedef quint32 u_int32_t;
#endif

}

class QXT_BERKELEY_EXPORT QxtBdb
{
public:
    enum OpenFlag
    {
        CreateDatabase  = 0x1,
        ReadOnly        = 0x2,
        LockFree        = 0x4

    };
    Q_DECLARE_FLAGS(OpenFlags, OpenFlag);

    QxtBdb();
    ~QxtBdb();


    bool get(void* key, int keytype, void* value, int valuetype, BerkeleyDB::u_int32_t flags = NULL, BerkeleyDB::DBC * cursor = 0) const ;
    bool get(const void* key, int keytype, void* value, int valuetype, BerkeleyDB::u_int32_t flags = NULL, BerkeleyDB::DBC * cursor = 0) const ;


    bool open(QString path, OpenFlags f = 0);
    OpenFlags openFlags();
    bool flush();
    BerkeleyDB::DB * db;
    bool isOpen;


    static QString dbErrorCodeToString(int e);


    template<class T>
    static T qxtMetaLoad(const void * data, size_t size)
    {
        T t;
        QByteArray b = QByteArray::fromRawData((const char*)data, size);
        QBuffer buffer(&b);
        buffer.open(QIODevice::ReadOnly);
        QDataStream s(&buffer);
        if (!QMetaType::load(s, qMetaTypeId<T>(), &t))
            qCritical("QMetaType::load failed. is your type registered with the QMetaType?");
        buffer.close();
        return t;
    }

    static void * qxtMetaLoad(const void * data, size_t size, int type)
    {
        void *p = QMetaType::construct(type);
        QByteArray b = QByteArray::fromRawData(static_cast<const char*>(data), size);
        QBuffer buffer(&b);
        buffer.open(QIODevice::ReadOnly);

        QDataStream s(&buffer);
        if (!QMetaType::load(s, type, p))
            qCritical("QMetaType::load failed. is your type registered with the QMetaType?");

        buffer.close();
        return p;
    }


    template<class T>
    static QByteArray qxtMetaSave(const T & t)
    {
        QByteArray d;
        QBuffer buffer(&d);
        buffer.open(QIODevice::WriteOnly);
        QDataStream s(&buffer);
        if (!QMetaType::save(s, qMetaTypeId<T>(), &t))
            qCritical("QMetaType::save failed. is your type registered with the QMetaType?");

        buffer.close();
        return d;
    }

    static void * qxtMetaSave(size_t * size, void * t, int type)
    {
        QByteArray d;
        QBuffer buffer(&d);
        buffer.open(QIODevice::WriteOnly);
        QDataStream s(&buffer);
        if (!QMetaType::save(s, type, t))
            qCritical("QMetaType::save failed. is your type registered with the QMetaType?");
        buffer.close();
        *size = d.size();
        void *p = ::malloc(d.size());
        ::memcpy(p, d.data(), d.size());
        return p;
    }















};

Q_DECLARE_OPERATORS_FOR_FLAGS(QxtBdb::OpenFlags);





#endif
