
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

#include "qxtcountrymodel.h"
#include "qxtcountrymodel_p.h"
#include "qxtlocale.h"
#include <QIcon>
#include <QLocale>
#include <QApplication>

class QxtCountry;
typedef QList<QxtCountry> QxtCountryList;

class QxtCountry
{
public:
    explicit QxtCountry(QLocale::Country country)
            : _mCountry(country)
    {
        _mName = qApp->translate("QLocale", qPrintable(QLocale::countryToString(_mCountry)));
        _mCurrency = QxtLocale::currencyForCountry(_mCountry);
    }

    const QString& name() const
    {
        return _mName;
    }
    QLocale::Country country() const
    {
        return _mCountry;
    }
    QxtLocale::Currency currency() const
    {
        return _mCurrency;
    }

    bool operator<(const QxtCountry& country) const
    {
        return _mName.localeAwareCompare(country._mName) < 0;
    }

    static const QxtCountryList& loadCountries()
    {
        if (!_smCountryNames.empty())
            return _smCountryNames;

        for (int idx = 0; idx < QLocale::LastCountry; ++idx)
        {
            QLocale::Country c = static_cast<QLocale::Country>(idx);
            if (c == QLocale::LastCountry)
                continue;
            if (c == QLocale::AnyCountry)
                continue;

            _smCountryNames.push_back(QxtCountry(c));
        }

        qSort(_smCountryNames);
        return _smCountryNames;
    }

private:
    QString             _mName;
    QLocale::Country    _mCountry;
    QxtLocale::Currency _mCurrency;

private:
    static QxtCountryList _smCountryNames;
};

QxtCountryList QxtCountry::_smCountryNames;

QxtCountryModelPrivate::QxtCountryModelPrivate()
{
}

QVariant QxtCountryModelPrivate::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const QxtCountry& c = QxtCountry::loadCountries().operator[](index.row());
    if (role == Qt::DecorationRole && index.column() == 0)
        return QIcon(":/flags/" + QxtLocale::countryToISO2Letter(c.country()) + ".png");

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return c.name();
        case 1:
            return QxtLocale::countryToISO2Letter(c.country());
        case 2:
            return c.country();
        case 3:
            return QxtLocale::countryToISO3Letter(c.country());
        case 4:
            return QxtLocale::currencyToName(c.currency());
        case 5:
            return QxtLocale::currencyToCode(c.currency());
        case 6:
            return QxtLocale::currencyToSymbol(c.currency());
        case 7:
            return QxtLocale::continentToName(QxtLocale::continentForCountry(c.country()));
        default:
            return QVariant();
        }
    }
    return QVariant();
}

/*!
    \class QxtCountryModel
    \inmodule QxtWidgets
    \brief The QxtCountryModel class is a subclass of QAbstractTableModel to display country data.

    QxtCountryModel is a specialized QAbstractTableModel to display countries.
    The countries are taken from QLocale::Country.

    \image qxtcountrymodel.png "QxtCountryModel in a QTableView in Plastique style."
 */

/*!
    Constructs a new QxtCountryModel with \a parent.
 */
QxtCountryModel::QxtCountryModel(QObject* parent)
        : QAbstractTableModel(parent)
{
    QXT_INIT_PRIVATE(QxtCountryModel);
}

/*!
    Destructs the country model.
 */
QxtCountryModel::~QxtCountryModel()
{
}

/*!
    Returns the number of countries for \a parent in the model.
 */
int QxtCountryModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : QxtCountry::loadCountries().size();
}

/*!
    Returns the number of columns \a parent in the model.
 */
int QxtCountryModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : 8;
}

/*!
    Returns the data for the given \a index for a specific \a role.
 */
QVariant QxtCountryModel::data(const QModelIndex& index, int role) const
{
    return qxt_d().data(index, role);
}

/*!
    Returns the column or row name for the given \a section in \a orientation for a sepcific \a role.
 */
QVariant QxtCountryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("Name");
        case 1:
            return tr("ISO 3166 Alpha 2");
        case 2:
            return tr("QLocale");
        case 3:
            return tr("ISO 3166 Alpha 3");
        case 4:
            return tr("Currency");
        case 5:
            return tr("Currency Code");
        case 6:
            return tr("Currency Symbol");
        case 7:
            return tr("Continent");
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
