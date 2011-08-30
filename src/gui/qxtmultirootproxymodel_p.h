/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtCore module of the Qxt library.
 **
 ** This library is free software; you can redistribute it and/or modify it
 ** under the terms of the Common Public License, version 1.0, as published
 ** by IBM, and/or under the terms of the GNU Lesser General Public License,
 ** version 2.1, as published by the Free Software Foundation.
 **
 ** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
 ** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 ** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
 ** FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** You should have received a copy of the CPL and the LGPL along with this
 ** file. See the LICENSE file and the cpl1.0.txt/lgpl-2.1.txt files
 ** included with the source distribution for more information.
 ** If you did not receive a copy of the licenses, contact the Qxt Foundation.
 **
 ** <http://libqxt.org>  <foundation@libqxt.org>
 **
 ****************************************************************************/

#ifndef QXTMULTIROOTPROXYMODEL_P_H
#define QXTMULTIROOTPROXYMODEL_P_H

#include "qxtmultirootproxymodel.h"

class QxtMultiRootProxyModelPrivate : public QObject, public QxtPrivate<QxtMultiRootProxyModel>
{
Q_OBJECT
public:
    QxtMultiRootProxyModelPrivate();
    QXT_DECLARE_PUBLIC(QxtMultiRootProxyModel);

    QModelIndexList roots;
    mutable QList<QModelIndex> parents;

    QModelIndexList mapAllFromSource(const QModelIndex& index) const;

public slots:
    inline void on_modelAboutToBeReset() { qxt_p().beginResetModel(); }
    inline void on_modelReset() { qxt_p().endResetModel(); }
    inline void on_layoutAboutToBeChanged() { emit qxt_p().layoutAboutToBeChanged(); }
    inline void on_layoutChanged() { emit qxt_p().layoutChanged(); }
    inline void on_headerDataChanged(Qt::Orientation o, int f, int l) { emit qxt_p().headerDataChanged(o, f, l); }
    void on_dataChanged(const QModelIndex&, const QModelIndex&);
    void on_rowsAboutToBeInserted(const QModelIndex&, int, int);
    void on_rowsInserted(const QModelIndex&, int, int);
    void on_rowsAboutToBeRemoved(const QModelIndex&, int, int);
    void on_rowsRemoved(const QModelIndex&, int, int);
    void on_columnsAboutToBeInserted(const QModelIndex&, int, int);
    void on_columnsInserted(const QModelIndex&, int, int);
    void on_columnsAboutToBeRemoved(const QModelIndex&, int, int);
    void on_columnsRemoved(const QModelIndex&, int, int);
};

#endif
