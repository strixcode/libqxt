#ifndef QXTSTANDARDITEMEDITORCREATOR_H
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

#define QXTSTANDARDITEMEDITORCREATOR_H

#include <QStandardItemEditorCreator>
#include "qxtitemeditorcreatorbase.h"
#include "qxtglobal.h"

/*!
    \class QxtStandardItemEditorCreator QxtStandardItemEditorCreator
    \inmodule QxtWidgets
    \brief An extended QStandardItemEditorCreator with default values for user-chosen properties.

    The QxtStandardItemEditorCreator class provides the possibility to set default values for
    user-chosen properties.

    Usage:
    \code
    QItemEditorFactory *factory = new QItemEditorFactory;

    QxtStandardItemEditorCreator<QSpinBox>* spinBoxCreator = new QxtStandardItemEditorCreator<QSpinBox>();
    QxtStandardItemEditorCreator<QLineEdit>* lineEditCreator = new QxtStandardItemEditorCreator<QLineEdit>();

    // spin boxes for degrees between -180 and 180
    spinBoxCreator->setDefaultPropertyValue("minimum", -180);
    spinBoxCreator->setDefaultPropertyValue("maximum", 180);

    // line edits for passwords
    lineEditCreator->setDefaultPropertyValue("echoMode", QLineEdit::Password);

    factory->registerEditor(QVariant::Int, spinBoxCreator);
    factory->registerEditor(QVariant::String, lineEditCreator);

    QItemEditorFactory::setDefaultFactory(factory);
    \endcode

    Setting default properties above makes sure that spin boxes have ranges from -180 to 180
    and line editors' echo mode is set to QLineEdit::Password.

    \sa QStandardItemEditorCreator
 */

template <class T>
class QXT_GUI_EXPORT QxtStandardItemEditorCreator : public QxtItemEditorCreatorBase<T>, public QStandardItemEditorCreator<T>
{
public:
    /*!
        Constructs a new QxtStandardItemEditorCreator.
     */
    inline QxtStandardItemEditorCreator() : QStandardItemEditorCreator<T>()
    {
    }

    /*!
        \reimp
     */
    inline QWidget* createWidget(QWidget* parent) const
    {
        return initializeEditor(QStandardItemEditorCreator<T>::createWidget(parent));
    }
};

#endif // QXTSTANDARDITEMEDITORCREATOR_H
