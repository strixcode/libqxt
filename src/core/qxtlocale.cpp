#include "qxtlocale.h"
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

#include "qxtlocale_data_p.h"
#include <QCoreApplication>
#include <QDebug>

/*!
    \class QxtLocale
    \inmodule QxtCore
    \brief The QxtLocale class has some additional data and functionality, missing in QLocale.

    QxtLocale defines currencies and continents. Methods are available to
    gather various mappings and information.
 */

/*!
    \enum QxtLocale::Continent

    This enumerated type is used to specify a language.
*/

/*!
    \enum QxtLocale::Currency

    This enumerated type is used to specify a currency.
*/

/*!
    \fn QxtLocale::currencyToCode(Currency currency)

    This static method returns the 3-letter code for the given \a currency.
 */
QString QxtLocale::currencyToCode(Currency currency)
{
    QString code;
    code.resize(3);
    const unsigned char *c = currency_code_list + 3 * (uint(currency));
    code[0] = ushort(c[0]);
    code[1] = ushort(c[1]);
    code[2] = ushort(c[2]);
    return code;
}

/*!
    \fn QxtLocale::countryToISO2Letter(QLocale::Country country)

    This static method returns the 2-letter ISO 3166 code for the given \a country.
 */
QString QxtLocale::countryToISO2Letter(QLocale::Country country)
{
    if (country == QLocale::AnyCountry)
        return QString();

    QString code;
    code.resize(2);
    const unsigned char *c = two_letter_country_code_list + 2 * (uint(country));
    code[0] = ushort(c[0]);
    code[1] = ushort(c[1]);
    return code;
}

/*!
    \fn QxtLocale::countryToISO3Letter(QLocale::Country country)

    This static method returns the 3-letter ISO 3166 code for the given \a country.
 */
QString QxtLocale::countryToISO3Letter(QLocale::Country country)
{
    if (country == QLocale::AnyCountry)
        return QString();

    QString code;
    code.resize(3);
    const unsigned char *c = three_letter_country_code_list + 3 * (uint(country));
    code[0] = ushort(c[0]);
    code[1] = ushort(c[1]);
    code[2] = ushort(c[2]);
    return code;
}

/*!
    \fn QxtLocale::currencyToName(Currency currency)

    This static method returns the translated name for given \a currency.
 */
QString QxtLocale::currencyToName(Currency currency)
{
    return QCoreApplication::instance()->translate("QxtLocale", currency_names[currency]);
}

/*!
    \fn QxtLocale::currencyForCountry(QLocale::Country country)

    This static method returns the currency for the given \a country.
 */
QxtLocale::Currency QxtLocale::currencyForCountry(QLocale::Country country)
{
    return currency_for_country_list[country];
}

/*!
    \fn QxtLocale::currencyToSymbol(Currency currency)

    This static method returns the symbol for the given \a currency.
 */
QString QxtLocale::currencyToSymbol(Currency currency)
{
    QChar* data = symbol_for_country_list[currency];
    int size = data[0].cell();
    return QString(data + 1, size);
}

/*!
    \fn QxtLocale::continentToName(Continent continent)

    This static method returns the translated name for the given \a continent.
 */
QString QxtLocale::continentToName(Continent continent)
{
    return QCoreApplication::instance()->translate("QxtLocale", continent_names[continent]);
}

/*!
    This static method returns the continent for the given \a country.
 */
QxtLocale::Continent QxtLocale::continentForCountry(QLocale::Country country)
{
    return continent_for_country_list[country];
}
