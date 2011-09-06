
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

#include "qxtlinkedtree.h"



/*!
\class QxtLinkedTree
\inmodule QxtCore
\brief The QxtLinkedTree class is a fast container for tree structured data

this template class can be used to store data easily in a tree structure.
Internally it uses the doublelinked list scheme, but adds client/parent links.

There are no random access functions, you have to use QxtLinkedTree::iterator to access the data.
This is very fast and efficient.


\code
QxtLinkedTree<int> tree(1);

QxtLinkedTreeIterator<int> it= tree.root();
it.append(34);
qDebug()<<it<<it.child(); //returns "1 34"

\endcode

In order to be able to store an iterator into other data structures (eg. for QAbstractItemModel or QAbstractXmlNodeModel) functions are provided to create and store a linked item from and into a void pointer.

\code
void * root= tree.toVoid(tree.root());
QxtLinkedTreeIterator<int> it= tree.fromVoid(root);
\endcode


TODO: {implicitshared}
*/

/*!
\fn QxtLinkedTree::QxtLinkedTree();
constructs a QxtLinkedTree with a default constructed root node.
*/

/*!
\fn QxtLinkedTree::QxtLinkedTree(T t);
constructs a QxtLinkedTree.
sets the rootnode to \a t
*/

/*!
\fn QxtLinkedTree::~QxtLinkedTree()
the destructor deletes all items, when they are no longer referenced by any other instance.
*/

/*!
\fn void QxtLinkedTree::clear();
deletes all nodes recursively. this might take forever depending on the size of your tree.
*/

/*!
\fn QxtLinkedTreeIterator QxtLinkedTree::root();
returns an iterator on the root node
*/

/*!
\fn void * QxtLinkedTree::toVoid  (QxtLinkedTreeIterator);
get an unique void pointer to be able to stuff an iterator into other structures.
You must not do anything else but pass this to fromVoid().
the pointer is invalid when the actual data has been removed and passing it to fromVoid will crash. You have been warned. 
*/

/*!
\fn QxtLinkedTreeIterator QxtLinkedTree::fromVoid  (void *);
returns an iterator pre positioned on the item specified with toVoid.
passing anything that has not being created by toVoid() will crash.
also note that passing invalidated nodes will crash too.
Be extremly carefull. It is easy to currupt your data with this!
*/

/*!
\class QxtLinkedTreeIterator
\inmodule QxtCore
\brief The QxtLinkedTreeIterator class provides fast access to an QxtLinkedTree
*/

/*!
\fn QxtLinkedTreeIterator    QxtLinkedTreeIterator::child   () const;
returns an iterator to the first child item of this or an invalid iterator when there are no children
*/

/*!
\fn QxtLinkedTreeIterator    QxtLinkedTreeIterator::parent   () const;
returns an iterator to the parent item of this. or an invalid iterator when this is the root node
*/

/*!
\fn QxtLinkedTreeIterator    QxtLinkedTreeIterator::previous   () const;
returns an iterator to the previous item of this or an invalid iterator when this is the first one in the next/previous chain
*/

/*!
\fn QxtLinkedTreeIterator    QxtLinkedTreeIterator::next   () const;
returns an iterator to the next item of this in the previous/next chain or an invalid iterator when this is the last one
*/

/*!
\fn bool    QxtLinkedTreeIterator::isValid   () const;
verfies if this iterator points to a valid location inside the tree
an invalid node is decoupled from the iteration. it can not be used for anything anymore.
\code
QxtLinkedTree<int> tree(1);
QxtLinkedTreeIterator<int> it= tree.begin();

it++; //invalid. there are no siblings.
it--; //still invalid!
\endcode
*/

/*!
\fn int  QxtLinkedTreeIterator::children() const
returns the amount of childnodes.
*/

/*!
\fn T &  QxtLinkedTreeIterator::operator* () const;
Returns a modifiable reference to the current item.
You can change the value of an item by using operator*() on the left side of an assignment, for example:
 \code
 if (*it == "Hello")
     *it = "Bonjour";
 \endcode
*/

/*!
\fn T   QxtLinkedTreeIterator::operator T () const;
returns a copy of the current item.
*/

/*!
\fn QxtLinkedTreeIterator    QxtLinkedTreeIterator:: operator + ( int j ) const;
*/
/*!
\fn QxtLinkedTreeIterator &  QxtLinkedTreeIterator::operator ++ ();
*/
/*!
\fn QxtLinkedTreeIterator &  QxtLinkedTreeIterator::operator ++ (int);
*/
/*!
\fn QxtLinkedTreeIterator &  QxtLinkedTreeIterator::operator += ( int j );
*/
/*!
\fn QxtLinkedTreeIterator    QxtLinkedTreeIterator::operator - ( int j ) const;
*/
/*!
\fn QxtLinkedTreeIterator &  QxtLinkedTreeIterator::operator -- ();
*/
/*!
\fn QxtLinkedTreeIterator &  QxtLinkedTreeIterator::operator -- (int);
*/
/*!
\fn QxtLinkedTreeIterator &  QxtLinkedTreeIterator::operator -= ( int j );
*/
/*!
\fn bool QxtLinkedTreeIterator::operator== ( const iterator & other ) const;
compares
*/
/*!
\fn bool QxtLinkedTreeIterator::operator!= ( const iterator & other ) const;
compares
*/
/*!
\fn QxtLinkedTreeIterator QxtLinkedTreeIterator::erase  () const;
deletes the current item. returns an iterator to the next sibling. this instance is then invalid.
deleting is recursive. all subitems will be deleted too.
*/
/*!
\fn QxtLinkedTreeIterator QxtLinkedTreeIterator::append (const T & value ) const;
appens an item to the children of this item. returns an iterator to the new item.
*/
