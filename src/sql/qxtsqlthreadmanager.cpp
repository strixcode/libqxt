
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

#include "qxtsqlthreadmanager.h"
#include <QThread>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/*!
\class QxtSqlThreadManager

\inmodule QxtSql

\brief The QxtSqlThreadManager class provides a thread-safe means of using a QSqlDatabase connection

All use of a given QSqlDatabase must be performed within the thread which
created it. This class makes management of thread-specific instances of your
QSqlDatabase relatively painless.

Main thread:
\code
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
db.setDatabase("somedatabasename");
db.setUser("johndoe");
...
if(!db.open())
    throw an_exception();
\endcode

Worker thread:
\code
QSqlQuery myQuery(QxtSqlThreadManager::connection());
myQuery.exec(...);
\endcode

The connection() method will obtain a reference to a QSqlDatabase assigned to
the current thread, creating one as necessary. When the thread exits, the
connection and all other resources are cleaned up automatically.

The QxtSqlThreadManager's constructor is private and never accessed directly
by client code. Use the static manager() method if any of the other members
or properties need to be referenced.

\warning Care must be taken to avoid passing references to any of the QSql
objects to other threads. Use of the QxtSqlPackage class may help in this
regard.

\warning Although you may specify a name for the primary thread's master
connection, this class only manages a single set of connections and is
typically used with the default connection (omitting the name).
*/

/*!
\internal

This is the actual thread-storage object. Since it is a static member, it
will have global allocation scope.
*/
QThreadStorage<QxtSqlThreadManager*> QxtSqlThreadManager::connections;

/*!
\internal

Constructs a connection for the calling thread. The resulting class must
never be accessed from any other thread than the one for which it was
constructed. The \a masterName must reference an existing database
connection which is already open.
*/
QxtSqlThreadManager::QxtSqlThreadManager(const QString &masterName)
{
    // Build a name for the new connection
    this->name = QString("$qxt$tc_%1$%2")
	.arg((unsigned long)(void*)QThread::currentThreadId(), 0, 36)
	.arg(masterName);
    // Clone the primary thread's connection
    Q_ASSERT(QSqlDatabase::contains(masterName));
    Q_ASSERT(QSqlDatabase::database(masterName).isOpen() == true);
    QSqlDatabase conn = QSqlDatabase::cloneDatabase(
	    QSqlDatabase::database(masterName), this->name);
    // Open the connection (should not fail but ...)
    if(!conn.open())
	qWarning() << Q_FUNC_INFO
	    << "Failed to open connection to database" << conn.databaseName()
	    << ", error: " << conn.lastError().text();
    qDebug() << "Constructed database connection" << this->name;
}

/*! Destroys the connection being managed
*/
QxtSqlThreadManager::~QxtSqlThreadManager()
{
    // Close the database if it's currently open
    {
	// Scoped so our local copy doesn't exist during removal
	QSqlDatabase conn = QSqlDatabase::database(this->name);
	if(conn.isOpen())
	    conn.close();
    }
    QSqlDatabase::removeDatabase(this->name);
    qDebug() << "Removed database connection" << this->name;
}

/*! Returns a pointer to the QxtSqlThreadManager for the current (calling)
 *  thread. A manager will be automatically created if one does not
 *  already exist. If the database connection being managed is not the
 *  default connection, it's name must be supplied as the \a masterName
 *  parameter on every invokation so it will be available in cases where
 *  it is needed.
 *  The pointer returned will never be \i null.
 *  \warning This pointer must never be deleted. If the manager is deleted,
 *  the next time the pointer is retrieved it will reference a dangling
 *  pointer and memory corruption is certain. Likewise, once the thread exits,
 *  the program will crash. \bold {You have been warned!}
 */
QxtSqlThreadManager * QxtSqlThreadManager::manager(const QString &masterName)
{
    if(!connections.hasLocalData())
	connections.setLocalData(new QxtSqlThreadManager(masterName));
    return connections.localData();
}

/*! Selects a new database name \a dbname for the connection. If the
 *  connection is already using the named database, nothing happens.
 *  For those database backends which do not support the \c USE statement,
 *  the connection will be closed and reopened as needed. This name is \i not
 *  the same as the connection's name.
 *  \warning Every effort is made to avoid leaving the connection in a closed
 *  (or otherwise unusable state) but all bets are off when a non-existant
 *  database name is requested.
 */
void QxtSqlThreadManager::selectDatabase(const QString &dbname)
{
    QSqlDatabase conn = database();
    if(conn.isOpen()){
	// We're already open -- check to see if we can avoid a close
	if(conn.databaseName() == dbname)
	    return; // No need to do anything regardless
	if(conn.driverName().startsWith("QMYSQL", Qt::CaseInsensitive)){
	    // Database supports "USE" -- so do it that way
	    QSqlQuery qry(conn);
	    if(!qry.exec("USE "+conn.driver()->escapeIdentifier(dbname,
			    QSqlDriver::TableName)))
		qWarning() << Q_FUNC_INFO << "Failed to change database to"
		    << dbname << ", error:" << qry.lastError().text();
	    // Set name so we'll see it next time (then we're done here)
	    conn.setDatabaseName(dbname);
	    return;
	}
	// Couldn't swap names on the current connection -- so close it
	conn.close();
    }
    // Reaching here means we've got to set the database name & open it
    conn.setDatabaseName(dbname);
    if(!conn.open())
	qWarning() << Q_FUNC_INFO
	    << "Failed to open connection to database" << dbname
	    << ", error: " << conn.lastError().text();
}

/*!
\fn  QString QxtSqlThreadManager::connectionName() const
\brief Returns the name of the managed QSqlDatabase connection.
*/

/*!
\fn  QSqlDatabase QxtSqlThreadManager::database() const
\brief Returns the QSqlDatabase object being managed by the
QxtSqlThreadManager class.
*/

/*!
\fn QSqlDatabase QxtSqlThreadManager::connection(const QString &masterName)
\brief Returns a QSqlDatabase connection for the current thread.
This is usually used as the parameter when constructing a QSqlQuery within
a worker thread. If the database connection being managed is not the
default connection, it's name must be supplied as the \a masterName
parameter on every invokation.
*/
