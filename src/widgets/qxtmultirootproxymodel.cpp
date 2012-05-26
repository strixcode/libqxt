
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

/*!
  \class QxtMultiRootProxyModel
  \inmodule QxtCore
  \brief The QxtMultiRootProxyModel class provides a tree proxy model with shortcuts

  QxtMultiRootProxyModel allows you to restructure a tree model by placing a selection
  of indexes at the top level. For example, a proxy on QFileSystemModel could provide for
  a view on several distinct folders in different places on the system.
  */

// TODO: someone extend the description

#include "qxtmultirootproxymodel_p.h"
#include <QStack>
#include <QTimer>
#include <QtDebug>

#define QXT_MRPM_SIGNAL(x) { SIGNAL(x), SLOT(on_ ## x) }
static const char* qxt_multirootproxymodel_signals[][2] = {
    QXT_MRPM_SIGNAL(dataChanged(QModelIndex,QModelIndex)),
    QXT_MRPM_SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
    QXT_MRPM_SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(rowsInserted(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(columnsInserted(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(rowsRemoved(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(columnsRemoved(QModelIndex,int,int)),
    QXT_MRPM_SIGNAL(layoutAboutToBeChanged()),
    QXT_MRPM_SIGNAL(layoutChanged()),
    QXT_MRPM_SIGNAL(modelAboutToBeReset()),
    QXT_MRPM_SIGNAL(modelReset()),
    {0, 0}
};

QxtMultiRootProxyModelPrivate::QxtMultiRootProxyModelPrivate() : QObject(0)
{
}

QModelIndexList QxtMultiRootProxyModelPrivate::mapAllFromSource(const QModelIndex& sourceIndex) const
{
    // One source index can map to multiple proxy indexes. mapFromSource selects the one with
    // the lowest depth, but data modification signals require us to notify about all of them.

    QModelIndexList rv;
    QModelIndex idx = sourceIndex;
    QList<int> rows, cols;
    int rootPos = -1;

    // Bubble up through the tree; each time we find a root, we add it to the list
    while(idx.isValid()) {
        // Keep track of the structure of the index
        if((rootPos = roots.indexOf(idx)) != -1 || !idx.parent().isValid()) {
            // idx is one of the roots
            QModelIndex newIndex = qxt_p().index(rootPos, sourceIndex.column(), QModelIndex());
            for(int j = rows.count() - 1; j >= 0; --j) {
                // Reconstruct the index
                newIndex = qxt_p().index(rows[j], cols[j], newIndex);
            }
            rv << newIndex;
        }

        rows.append(idx.row());
        cols.append(idx.column());
        idx = idx.parent();
    }

    return rv;
}

void QxtMultiRootProxyModelPrivate::on_dataChanged(const QModelIndex& tl, const QModelIndex& br)
{
    QModelIndexList tls = mapAllFromSource(tl);
    QModelIndexList brs = mapAllFromSource(br);
    Q_ASSERT(tls.count() == brs.count());

    // First emit notifications for all of the ranges 
    for(int i = tls.count() - 1; i >= 0; --i) {
        emit qxt_p().dataChanged(tls[i], brs[i]);
    }
    // Then if any of our roots are in any of the ranges, emit for them
    foreach(const QModelIndex& root, roots) {
        if(qxt_p().mapToSource(root).parent() == tl.parent()) 
            emit qxt_p().dataChanged(root, root);
    }
}

void QxtMultiRootProxyModelPrivate::on_rowsAboutToBeInserted(const QModelIndex& parent, int start, int end)
{
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        qxt_p().beginInsertRows(idx, start, end);
    }
    // TODO: does the parents list need updated here?
}

void QxtMultiRootProxyModelPrivate::on_rowsInserted(const QModelIndex& parent, int, int)
{
    // TODO: Does this need to be a stack of ints instead of re-querying?
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        Q_UNUSED(idx);
        qxt_p().endInsertRows();
    }
}

void QxtMultiRootProxyModelPrivate::on_rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        qxt_p().beginRemoveRows(idx, start, end);
    }
    // TODO: does the parents list need updated here?
}

void QxtMultiRootProxyModelPrivate::on_rowsRemoved(const QModelIndex& parent, int, int)
{
    // TODO: Does this need to be a stack of ints instead of re-querying?
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        Q_UNUSED(idx);
        qxt_p().endRemoveRows();
    }
}

void QxtMultiRootProxyModelPrivate::on_columnsAboutToBeInserted(const QModelIndex& parent, int start, int end)
{
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        qxt_p().beginInsertColumns(idx, start, end);
    }
    // TODO: does the parents list need updated here?
}

void QxtMultiRootProxyModelPrivate::on_columnsInserted(const QModelIndex& parent, int, int)
{
    // TODO: Does this need to be a stack of ints instead of re-querying?
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        Q_UNUSED(idx);
        qxt_p().endInsertColumns();
    }
}

void QxtMultiRootProxyModelPrivate::on_columnsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        qxt_p().beginRemoveColumns(idx, start, end);
    }
    // TODO: does the parents list need updated here?
}

void QxtMultiRootProxyModelPrivate::on_columnsRemoved(const QModelIndex& parent, int, int)
{
    // TODO: Does this need to be a stack of ints instead of re-querying?
    foreach(const QModelIndex& idx, mapAllFromSource(parent)) {
        Q_UNUSED(idx);
        qxt_p().endRemoveColumns();
    }
}

/*!
  Constructs a QxtMultiRootProxyModel with the specified \a parent.
  */
QxtMultiRootProxyModel::QxtMultiRootProxyModel(QObject* parent) : QAbstractProxyModel(parent)
{
    QXT_INIT_PRIVATE(QxtMultiRootProxyModel);
}

/*!
  \reimp
  */
void QxtMultiRootProxyModel::setSourceModel(QAbstractItemModel* model)
{
    QXT_D(QxtMultiRootProxyModel);
    beginResetModel();

    if(QAbstractItemModel* source = sourceModel()) {
        // Disconnect all of the signals we cared about on the old model.
        // Don't disconnect anything else JUST IN CASE someone's hooked up something odd.
        for(int i = 0; qxt_multirootproxymodel_signals[i][0] != 0; i++) {
            QObject::disconnect(source, qxt_multirootproxymodel_signals[i][0], &d, qxt_multirootproxymodel_signals[i][1]);
        }
    }

    qxt_d().roots.clear();
    qxt_d().parents.clear();
    QAbstractProxyModel::setSourceModel(model);

    // Connect up a bunch of signals
    for(int i = 0; qxt_multirootproxymodel_signals[i][0] != 0; i++) {
        QObject::connect(model, qxt_multirootproxymodel_signals[i][0], &d, qxt_multirootproxymodel_signals[i][1]);
    }

    endResetModel();
}

/*!
  Returns the current list of roots.

  \sa setRoots
  */
QList<QPersistentModelIndex> QxtMultiRootProxyModel::roots() const
{
    return qxt_d().roots;
}

/*!
  Sets the list of roots used by the proxy model.

  Each specified root will appear as a top-level entry in the proxy model.

  \sa roots
  */
void QxtMultiRootProxyModel::setRoots(const QList<QPersistentModelIndex>& roots)
{
    beginResetModel();
    qxt_d().roots = roots;
    endResetModel();
}

/*!
  \overload
  Sets the list of roots used by the proxy model.

  Each specified root will appear as a top-level entry in the proxy model.

  \sa roots
  */
void QxtMultiRootProxyModel::setRoots(const QModelIndexList& roots)
{
    QList<QPersistentModelIndex> pRoots;
    foreach(const QModelIndex& root, roots)
        pRoots << QPersistentModelIndex(root);
    setRoots(pRoots);
}

/*!
  \reimp
  */
QModelIndex QxtMultiRootProxyModel::mapFromSource(const QModelIndex& sourceIndex) const
{
    QModelIndex idx = sourceIndex;
    QStack<int> rows, cols;
    int rootPos = -1;

    // Bubble up through the tree until we find one of our roots
    while(idx.isValid() && (rootPos = qxt_d().roots.indexOf(idx)) == -1) {
        // Keep track of the structure of the index
        rows.push(idx.row());
        cols.push(idx.column());
        idx = idx.parent();
    }

    // If the index doesn't descend from one of our roots, we've filtered it out
    if(rootPos == -1) return QModelIndex();

    // now idx is one of the roots, or an invalid (that is, top-level) index
    QModelIndex rv = index(rootPos, sourceIndex.column(), QModelIndex());
    while(!rows.isEmpty()) {
        // Reconstruct the index
        rv = index(rows.pop(), cols.pop(), rv);
    }
    return rv;
}

/*!
  \reimp
  */
QModelIndex QxtMultiRootProxyModel::mapToSource(const QModelIndex& proxyIndex) const
{
    // If we have no roots, the model is empty
    int numRoots = qxt_d().roots.count();
    if(numRoots == 0) return QModelIndex();

    QModelIndex root = proxyIndex;
    QStack<int> rows, cols;
    bool rootRequested = true;

    // Find the index root, which is one of our root elements
    while(root.parent().isValid()) {
        // Keep track of the structure of the index
        rows.push(root.row());
        cols.push(root.column());
        root = root.parent();
        rootRequested = false;
    }

    // If no root was found, we've filtered the index out
    if(!root.isValid()) return QModelIndex();

    // Get the index of the root from the source model
    int rootPos = root.row();
    if(rootPos < 0 || rootPos >= numRoots) return QModelIndex();

    QModelIndex rv = qxt_d().roots[rootPos];

    if(rootRequested) {
        // The requested index already is a root
        // Keep the requested column, use the stored parent and row
        return sourceModel()->index(rv.row(), proxyIndex.column(), rv.parent());
    }
    
    while(!rows.isEmpty()) {
        // Reconstruct the index
        rv = sourceModel()->index(rows.pop(), cols.pop(), rv);
    }

    return rv;
}

/*!
  \reimp
  */
bool QxtMultiRootProxyModel::hasChildren(const QModelIndex& parent) const
{
    if(!parent.isValid()) return true;
    return sourceModel()->hasChildren(mapToSource(parent));
}

/*!
  \reimp
  */
int QxtMultiRootProxyModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid()) {
        return sourceModel()->rowCount(mapToSource(parent));
    }
    return qxt_d().roots.count();
}

/*!
  \reimp
  */
int QxtMultiRootProxyModel::columnCount(const QModelIndex& parent) const
{
    return sourceModel()->columnCount(mapToSource(parent));
}

/*!
  \reimp
  */
QModelIndex QxtMultiRootProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    // If the coordinates are bogus, return invalid
    if(row < 0 || column < 0) return QModelIndex();

    // If the parent is invalid, it's one of our roots
    if(!parent.isValid()) {
        // But if it's out of range, return invalid
        if(row >= qxt_d().roots.count() || column >= columnCount()) return QModelIndex();
        return createIndex(row, column, -1);
    }

    // Otherwise, map it
    int pos = qxt_d().parents.indexOf(parent);
    if(pos < 0) {
        pos = qxt_d().parents.count();
        qxt_d().parents << parent;
    }
    return createIndex(row, column, pos);
}

/*!
  \reimp
  */
QModelIndex QxtMultiRootProxyModel::parent(const QModelIndex& index) const
{
    // Invalid index has invalid parent
    if(!index.isValid()) return QModelIndex();

    int id = index.internalId();

    // Out of range index has invalid parent
    if(id < 0 || id >= qxt_d().parents.count()) return QModelIndex();

    return qxt_d().parents[id];
}

/*!
  \reimp
  */
bool QxtMultiRootProxyModel::canFetchMore(const QModelIndex& parent) const
{
    bool rv = sourceModel()->canFetchMore(mapToSource(parent));
    return rv;
}

/*!
  \reimp
  */
void QxtMultiRootProxyModel::fetchMore(const QModelIndex& parent) 
{
    sourceModel()->fetchMore(mapToSource(parent));
}
