
/****************************************************************************
** Copyright (c) 2006 - 2012, the LibQxt project.
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


#ifndef QXTSQLTHREADMANAGER_H
#define QXTSQLTHREADMANAGER_H
#include <QThreadStorage>
#include <QSqlDatabase>
#include <qxtglobal.h>

class QXT_SQL_EXPORT QxtSqlThreadManager
{
    Q_DISABLE_COPY(QxtSqlThreadManager)

public:
    virtual ~QxtSqlThreadManager();
    void selectDatabase(const QString &);
    inline QString connectionName() const { return this->name; }
    inline QSqlDatabase database() const
    {
	return QSqlDatabase::database(connectionName());
    }

    // Static methods for thread use
    static QxtSqlThreadManager * manager(const QString & masterName =
	    QLatin1String(QSqlDatabase::defaultConnection));
    inline static QSqlDatabase connection(const QString & masterName =
	    QLatin1String(QSqlDatabase::defaultConnection))
    {
	return manager(masterName)->database();
    }

protected:
    QxtSqlThreadManager(const QString &);
    QString name;

private:
    static QThreadStorage<QxtSqlThreadManager *> connections;
};

#endif // QXTSQLTHREADMANAGER_H
