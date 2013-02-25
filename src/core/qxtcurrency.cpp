
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

#include <QDebug>
#include "qxtcurrency.h"
#include <stdlib.h>
#include <stdint.h>

/*!
 *  \class QxtCurrency
 *  \inmodule QxtCore
 *  \brief The QxtCurrency class provides a fixed-precision currency type
 *  and associated operators and methods. Using this type avoids the
 *  inaccuracies inherent in floating point operations with regard to
 *  monetary amounts.
 *
 *  \since 0.7
 *
 *  The internal representation is that of a long-long (64-bit) integer
 *  with an implied exponent of -4. This gives a total precision of 4
 *  decimal places with an effective range of
 *  \bold -922,337,203,685,477.5807 to \bold 922,337,203,685,477.5807
 *  (inclusive). The actual lowest value is reserved to indicate a NULL
 *  status. This type is equivalent to the CY / CURRENCY type in Microsoft
 *  Windows and support for these values is provided in that environment.
 *
 *  Specializations for \c std::abs and \c std::numeric_limits are
 *  provided for STL compatibility.
 *
 *  \warning Binary arithmetic operations will produce a NULL result if the
 *  primary operand is null while in-place operations (e.g. \c +=) will
 *  treat a null right-hand operand as 0. Comparisions operate on normalized
 *  values causing null values to evaluate as zero.
 *
 *  \warning Although QxtCurrency is declared as a \i meta-type, it must
 *  be registered to be used properly in a QVariant or in conjunction with
 *  QDataStream. It is suggested that the following code be used somewhere
 *  within your initialization code to ensure this happens. The final
 *  registration is not needed for direct serialization unless you are
 *  serializing from a QVariant containing a QxtCurrency value.
 *  \code
 *  (void) qRegisterMetaType<QxtCurrency>();
 *  qRegisterMetaTypeStreamOperators<QxtCurrency>("QxtCurrency");
 *  qxtRegisterSerializationOperators<QxtCurrency>("QxtCurrency");
 *  \endcode
 */

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Supporting functions (global)

/*!
    \relates QxtCurrency

    This method provides QDataStream support for currency values. The value
    \a v is streamed into \a out.
    Returns a reference to the stream \a out.
*/
QDataStream &operator<<(QDataStream &out, const QxtCurrency &v)
{
    out << v.value;
    return out;
}

/*!
    \relates QxtCurrency

    This method provides QDataStream support for currency values. The value
    \a v is extracted from the stream \a in.
    Returns a reference to the stream \a in.
*/
QDataStream &operator>>(QDataStream &in, QxtCurrency &v)
{
    in >> v.value;
    return in;
}

//////////////////////////////////////////////////////////////////////////////
// QxtCurrency class

/*! Extracts a QxtCurrency value from a QVariant \a v. This is useful on the
 *  off chance the variant actually contains a QxtCurrency type. As a bonus,
 *  strings and NULL values (e.g. from a database query) are handled as well.
 */
QxtCurrency QxtCurrency::fromVariant(const QVariant &v)
{
    QxtCurrency result;
    if(v.isNull())
	result.setNull();
    else{
	switch(v.type()){
	case QVariant::ByteArray:
	    result = QxtCurrency(v.toByteArray().constData());
	    break;
	case QVariant::String:
	    result = QxtCurrency(v.toString());
	    break;
	case QVariant::Int:
	case QVariant::UInt:
	    result = QxtCurrency(v.toInt());
	    break;
	case QVariant::LongLong:
	case QVariant::ULongLong:
	    result = QxtCurrency(v.toLongLong());
	    break;
	case QVariant::Double:
	    result = QxtCurrency(v.toDouble());
	    break;
	case QVariant::Invalid:
	    break; // Treating as zero
	default:
	    if(v.userType() == qMetaTypeId<QxtCurrency>())
		result = v.value<QxtCurrency>();
	    else
		qWarning() << "Conversion failure in QxtCurrency::fromVariant";
	}
    }
    return result;
}

/*! Converts a string \a s into a QxtCurrency value. The decimal point used is
 *  always '.' (ASCII). More than 4 digits to the right of the decimal point
 *  will be ignored (no rounding is performed).
 */
QxtCurrency::QxtCurrency(const QString &s)
{
    parseASCII(s.toLatin1().data());
}

/*! Converts a string \a s into a QxtCurrency value. The decimal point used is
 *  always '.' (ASCII). More than 4 digits to the right of the decimal point
 *  will be ignored (no rounding is performed).
 */
QxtCurrency::QxtCurrency(const char *s)
{
    parseASCII(s);
}

/*! Private method to parse text \a s into a currency value. Conversion stops
 *  when an invalid character is encountered or 4 digits of precision have been
 *  processed. The current value, if any, is lost.
 */
void QxtCurrency::parseASCII(const char *s)
{
    value = 0LL;
    if(s){
	int mult = 10000;
	bool seenDP = false, seenSign = false;
	for(; *s; ++s){
	    if(*s == '+' || *s == '-'){
		if(seenSign)
		    break; // Error
		if(*s == '-')
		    mult = -mult;
		seenSign = true;
	    }
	    else if(*s == '.'){
		if(seenDP)
		    break; // Error
		seenDP = true;
	    }
	    else if(isdigit(*s)){
		value = value * 10 + (*s - '0');
		if(seenDP){
		    mult /= 10;
		    if(mult == 1 || mult == -1)
			break; // We're done
		}
	    }
	    else
		break; // Error
	}
	if(mult)
	    value *= mult;
    }
}

/*! Converts a QxtCurrency value to a string. The decimal point used is always
 *  '.' (ASCII). Although there are always 4 digits of precision to the right
 *  of the decimal, the string representation will omit trailing zeroes and,
 *  possibly, the decimal point itself.
 *  \sa toQString()
 */
QByteArray QxtCurrency::toString() const
{
    const int ndigs = std::numeric_limits<QxtCurrency>::digits10 + 5;
    char buf[ndigs] = {0};
    char* const end = buf + ndigs - 2;
    bool neg = value < 0;
    qlonglong v = ::llabs(value);
    if(v < 0)
	v = numeric_limits<qlonglong>::max();
#ifndef Q_CC_GNU
    register int decp = 4;
#else
    int_fast8_t decp = 4;
#endif
    // Do the raw conversion of digits right-to-left
    char* p = end;
    for(; p > buf && v; --decp){
	lldiv_t d = ::lldiv(v,10);
	v = d.quot;
	if(p != end && decp == 0)
	    *p-- = '.';
	if(d.rem || p != end || decp < 1)
	    *p-- = char('0' + d.rem);
    }
    // Check for need of zero fill (values between 0.0001 and 0.0999)
    if(p != end && decp >= 0){
	for(; decp > 0; --decp)
	    *p-- = '0';
	*p-- = '.';
    }
    // Check for need of zero preceding decimal point
    if(p == end || decp == 0)
	*p-- = '0';
    // Check for negative sign
    if(neg)
	*p-- = '-';
    // Done -- return the result now...
    return (p+1);
}

/*! Rounds a QxtCurrency value to \a n number of decimal places. Since the
 *  maximum precision is 4 decimal places, using a value 4 or greater will
 *  have no effect on the result (i.e. the current value will be returned
 *  unchanged).
 *  \warning It is permissible to use a negative number to round to the
 *  nearest hundred, thousand, etc. Exceeding the effective range, however,
 *  will cause a \c std::range_error exception to be thrown.
 */
QxtCurrency QxtCurrency::round(int n) const
{
    if(n < -10)
	throw std::range_error("rounding value too large");
    else if(n > 3)
	return *this;
    // Determine decimal shift required
#if defined(Q_CC_GNU) & defined(__USE_GNU)
    qlonglong modv = qRound64(exp10(4-n));
#else
    qlonglong modv = qRound64(pow(10.0, 4-n));
#endif
    QxtCurrency result;
    // Compute fractional value and subtract it
    qlonglong frac = value % modv;
    result.value = value - frac;
    // If the fractional value is greater than half the modulus, round up
    if(frac >= (modv >> 1))
	result.value += modv;
    return result;
}

/*! Clamps the value to specified range [\a low, \a high]. If the current
 *  value is less than \a low, it's set to \a low or if it is greater than
 *  \a high, it's set to \a high. Otherwise the value remains unchanged.
 *  If the low limit supplied is greater than the high limit, the results
 *  are undefined.
 *  Returns a reference to the value being manipulated.
 *  \warning If the current value is NULL, the value remains NULL.
 *  \sa clamped()
 */
QxtCurrency& QxtCurrency::clamp(const QxtCurrency &low, const QxtCurrency &high)
{
    if(!isNull()){
	if(*this < low)
	    *this = low;
	else if(*this > high)
	    *this = high;
    }
    return *this;
}

/*! Clamps the value to specified range [\a low, \a high]. If the current
 *  value is less than \a low, \a low is returned. If it is greater than
 *  \a high, \a high is returned. Otherwise the existing value is returned.
 *  If the low limit supplied is greater than the high limit, the results
 *  are undefined. Unlike clamp(), the existing value does not change.
 *  \warning If the current value is NULL, the result is also NULL.
 *  \sa clamp()
 */
QxtCurrency QxtCurrency::clamped(const QxtCurrency &low,
	const QxtCurrency &high) const
{
    if(!isNull()){
	if(*this < low)
	    return low;
	else if(*this > high)
	    return high;
    }
    return *this;
}

/*  Ugly hack for crippled MSVC versions missing the expm1() POSIX function.
 *
 *  The alternative implementation was obtained from John Cook.
 *
 *  http://www.johndcook.com/cpp_expm1.html
 *
 *  NOTE: MSDN indicates the "expm1()" function is available in MSVC 2012
 *  and later editions. It's not clear, however, if this includes the
 *  "Express" editions so further testing is advised.
 */
#if defined(_MSC_VER)
inline double qxt_expm1(double n)
{
    if(fabs(n) < 1e-5)
	// Small value, use 2-term Taylor series approximation
	return n + 0.5*n*n;
    else
	// Large value, use native exp() function and subtract 1.0
	return exp(n) - 1.0;
}
#else
inline double qxt_expm1(double n) { return expm1(n); }
#endif

/*! Calculates the proper payment amount for an amortized loan repayment
 *  schedule. Given a principal amount \a P (that being borrowed), a periodic
 *  interest rate \a r and \a n payments, the result is the minimum payment
 *  amount required to completely repay the principal in the number of periods
 *  given. For example, to find the payment for a typical 30 year mortgage on
 *  $250,000 at 6%, one would use \tt{amortizedPayment(250000,0.005,360)}.
 *  The rate 0.005 is obtained by dividing the APR 6% (0.06) by 12 payments.
 *  Likewise, the number of payments, 360, is the result of 30 years times
 *  12 payments. If the interest is compounded more frequently than payments
 *  are made, you should multiply the number of payments accordingly and do
 *  the same to the resulting payment amount.
 *
 *  \warning The result will be accurate to four decimal places. Since it is
 *  unlikely that fractions of a cent will be useful (or desired), the
 *  result should be rounded to the nearest cent using round().
 *  \sa amortize(), amortizedInterest()
 */
QxtCurrency QxtCurrency::amortizedPayment(const QxtCurrency &P, double r, int n)
{
    QxtCurrency pmt;
    if(n > 1){
	if(r >= 0.000001){
	    double nval = double(n)*log(1.0+r);
	    pmt = QxtCurrency(r * double(P) * exp(nval) / qxt_expm1(nval));
	}
	else
	    pmt = P / n;
    }
    else
	pmt = P;
    return pmt;
}

/*! Calculates the total interest over the term of a loan (\c Pair.first)
 *  and the principal amount remaining after the last payment has been
 *  applied (\c Pair.second). Therefore, the last payment should be the
 *  payment amount plus the balloon value to insure the principal reaches
 *  exactly zero and is not over or under paid. The balloon value also
 *  indicates how accurately the payment amount represents a properly
 *  amortized payment since, if it is significantly negative, the payment
 *  exceeds what is needed to match the term and, when positive, the payment
 *  is insufficient to meet the terms. In extreme cases, the balloon value
 *  may even exceed the original principal which indicates the payment amount
 *  is not sufficient to cover the interest, much less reduce the principal
 *  (think Quicken Loans and Countrywide). The principal amount (borrowed)
 *  is \a P, the periodic rate \a r, number of payments \a n and periodic
 *  payment amount \a p. The later may be obtained using the
 *  amortizedPayment() method and the other 3 parameters.
 *
 *  The total repayment amount (principal and interest) can be calculated
 *  by adding the principal and the total interest (\c {P + Pair.first})
 *  or by multiplying the payment amount by the term and adding the balloon
 *  amount (\c {(p * n) + Pair.second}). Both computations should yield the
 *  same amount.
 *
 *  The total interest is determined by iterating over the number of payments
 *  and applying each payment less the period's interest. Although the result
 *  could be determined mathematically without iteration, this method allows
 *  for rounding the interest calculation and, therefore, accurately matches
 *  the actual installment charges and payments as they happen.
 *
 *  \warning Once the principal reaches zero, no further interest will apply
 *  and the balloon value will accumulate the excess payments. If this happens,
 *  the term is too long or the payment amount too high.
 *  \sa amortize(), amortizedPayment()
 */
QxtCurrency::Pair QxtCurrency::amortizedInterest(QxtCurrency P, double r, int n,
	const QxtCurrency &p)
{
    Pair result;
    for(; n > 0; --n){
	QxtCurrency pint = QxtCurrency(double(P) * r).round();
	if(pint < 0)
	    pint = 0;
	P -= (p - pint);
	result.first += pint;
    }
    result.second = P;
    return result;
}

/*! Generates an amortization schedule given a principal amount \a P, periodic
 *  interest rate \a r, number of payments \a n and an optional payment amount
 *  of \a p. If the payment amount is less than zero, it will be calculated
 *  internally for the optimum amount. The resulting list provides the
 *  payment amount (\c Pair.first) and interest (\c Pair.second) for each
 *  installment.
 *
 *  A declining balance is easily computed by subtracting the payment amount
 *  less the interest during each iteration
 *  (\c {balance - (Pair.first - Pair.second)}).
 *  Once the principal reaches zero, no further interest will apply and the
 *  payment and interest amounts will be zero for all remaining entries in
 *  the list. If this happens, the term is too long or the payment amount
 *  too high. Likewise, if the payment amount is too low, the final payment
 *  entry will include the remaining unpaid principal. It's also possible
 *  the periodic interest will exceed the payment amount. If this happens
 *  the payment amount isn't even enough to cover the interest.
 *
 *  \code
 *  double apr = 6.0 / 100.0; // 6%
 *  QxtCurrency principal = 100000; // $100,000
 *  int years = 30;
 *  QxtCurrency payment = principal.amortizedPayment(apr / 12.0, years * 12);
 *  std::cout << "Payment = " << payment.toString() << std::endl;
 *  std::cout << "BeginningBalance Payment Principal Interest EndingBalance TotalPaid" << std::endl;
 *  QList<QxtCurrency::Pair> schedule = principal.amortize(apr / 12.0, years * 12, payment);
 *  QxtCurrency paid, balance = principal;
 *  foreach(const QxtCurrency::Pair inst, schedule){
 *	QString line = QString("%1 %2 %3 %4 %5 %6")
 *	    .arg(balance.toQString(), 16)
 *	    .arg(inst.first.toQString(), 7)
 *	    .arg((inst.first - inst.second).toQString(), 9)
 *	    .arg(inst.second.toQString(), 8)
 *	    .arg((balance - inst.first + inst.second).toQString(), 13)
 *	    .arg((paid + inst.first).toQString(), 9);
 *	std::cout << qPrintable(line) << std::endl;
 *	balance -= inst.first - inst.second;
 *	paid += inst.first;
 *  }
 *  \endcode
 *  \sa amortizedInterest(), amortizedPayment()
 */
QList<QxtCurrency::Pair> QxtCurrency::amortize(QxtCurrency P, double r, int n,
	QxtCurrency p)
{
    Q_ASSERT(P >= 0); Q_ASSERT(n >= 0);
    QList<Pair> result;
#if QT_VERSION >= QT_VERSION_CHECK(4,7,0)
    result.reserve(n);
#endif
    if(p < 0)
	p = amortizedPayment(P, r, n).round();
    for(; n > 0; --n){
	QxtCurrency pint = QxtCurrency(double(P) * r).round();
	if(pint < 0)
	    pint = 0;
	QxtCurrency pp = qMin(P, p);
	if(n)
	    result.push_back(Pair(pp, pint));
	else
	    result.push_back(Pair(P, pint));
	P -= (pp - pint);
    }
    return result;
}

/*! \variable QxtCurrency::value

    This is the actual value maintained by the QxtCurrency object. Although
    publicly accessible, it is not intended to be modified directly.
*/

/*! \typedef QxtCurrency::Pair

    This is a pair of currency values used as a return value by some methods.
*/

/*! \fn QxtCurrency::QxtCurrency()

    Constructs a currency object with a zero value.
*/

/*! \fn QxtCurrency::QxtCurrency(double v)

    Constructs a currency object from a double value \a v. The value will be
    rounded to the nearest 4th decimal place.
*/

/*! \fn QxtCurrency::QxtCurrency(int v)

    Constructs a currency object from an integer value \a v.
*/

/*! \fn QxtCurrency::QxtCurrency(qlonglong v)

    Constructs a currency object from a pre-adjusted integer value \a v. The
    value is expected to already assume 4 decimal places so you should first
    multiply by 10000LL if this is not the case.

*/

/*! \fn QxtCurrency::QxtCurrency(const CY &v)

    Constructs a currency object from a CY value \a v. This is identical to
    CURRENCY.

    \warning This constructor is only available in Windows builds.
*/

/*! \fn QxtCurrency QxtCurrency::abs() const

    Returns the absolute value of the current currency value. The result
    will always be positive or zero.
*/

/*! \fn QList<QxtCurrency::Pair> QxtCurrency::amortize(double r, int n, QxtCurrency p) const

    \overload
    This variation uses the currency value as the principal amount. The rate
    \a r, number of payments \a n and payment amount \a p are used as
    documented above.
*/

/*! \fn QxtCurrency::Pair QxtCurrency::amortizedInterest(double r, int n, const QxtCurrency &p) const

    \overload
    This variation uses the currency value as the principal amount. The rate
    \a r, number of payments \a n and payment amount \a p are used as
    documented above.
*/

/*! \fn QxtCurrency::Pair QxtCurrency::amortizedPayment(double r, int n) const

    \overload
    This variation uses the currency value as the principal amount. The rate
    \a r and number of payments \a n are used as documented above.
*/

/*! \fn QxtCurrency::operator bool() const

    Converts the currency value to a \i boolean type. This facilities easy
    testing for zero/non-zero values. Null and zero both return false.
    \code
    QxtCurrency myMoney;
    ...
    if(myMoney) ...
    \endcode
*/

/*! \fn QxtCurrency::operator double() const

    Converts the currency value to a \i double type. Some loss of precision
    may occur for very large values.
*/

/*! \fn QxtCurrency::operator int() const

    Converts the currency value to an \i int type. Some loss of precision
    is likely to occur (particularly the decimal places) and very large
    values could cause an overflow condition. The resulting value is
    simply truncated (i.e. no rounding is performed).
*/

/*! \fn QxtCurrency::operator CY() const

    Converts the value into a CY type. This is identical to CURRENCY.

    \warning This operator is only available in Windows builds.
*/

/*! \fn QxtCurrency & QxtCurrency::clamp(const Pair &r)
    \overload
    Clamps the value to specified range \a r [r.first, r.second].
    \sa clamped()
*/

/*! \fn QxtCurrency QxtCurrency::clamped(const Pair &r) const
    \overload
    Clamps the value to specified range \a r [r.first, r.second].
    \sa clamp()
*/

/*! \fn QxtCurrency & QxtCurrency::normalize()
 *  Normalizes the currency value. This only affects null values which are
 *  set to zero.
 *  \sa normalized()
 */

/*! \fn QxtCurrency QxtCurrency::normalized() const
 *  Returns a the current value normalized. For null values, zero is returned.
 *  Otherwise the actual value is returned (which also could be zero).
 *  \sa normalize()
 */

/*! \fn int QxtCurrency::sign() const

    Returns an integer representing the sign of the value. This will be 1 if
    the value is positive (> 0), -1 if the value is negative (< 0) and 0 if
    the value is exactly zero.
*/

/*! \fn QVariant QxtCurrency::toVariant() const

    Returns a QVariant containing the currency value. This is preferred over
    using QVariant::fromValue() or qVariantFromValue() since these will not
    preserve null status (i.e. QVariant::isNull() will always return false).
 */

/*! \fn QxtCurrency QxtCurrency::operator+(const QxtCurrency &rhs) const

    Returns the result of adding the value \a rhs to the current value.
    Various overrides are also provided for integral and floating point values.
*/

/*! \fn QxtCurrency QxtCurrency::operator+(double rhs) const
    \internal
*/

/*! \fn QxtCurrency& QxtCurrency::operator+=(const QxtCurrency &rhs)

    Adds the value \a rhs to the current value and returns a reference to this.
*/

/*! \fn QxtCurrency QxtCurrency::operator-(const QxtCurrency &rhs) const

    Returns the result of subtracting the value \a rhs from the current
    value.
    Various overrides are also provided for integral and floating point values.
*/

/*! \fn QxtCurrency QxtCurrency::operator-(double rhs) const
    \internal
*/

/*! \fn QxtCurrency& QxtCurrency::operator-=(const QxtCurrency &rhs)

    Subtracts the value \a rhs from the current value and returns a
    reference to this.
*/

/*! \fn QxtCurrency QxtCurrency::operator*(const QxtCurrency &rhs) const

    Returns the result of multiplying the current value by \a rhs.
    Various overrides are also provided for integral and floating point values.
*/

/*! \fn QxtCurrency& QxtCurrency::operator*(double rhs) const
    \internal
*/

/*! \fn QxtCurrency& QxtCurrency::operator*=(const QxtCurrency &rhs)

    Multiplies the current value by \a rhs and returns a reference to this.
*/

/*! \fn QxtCurrency QxtCurrency::operator/(const QxtCurrency &rhs) const

    Returns the result of dividing the current value by \a rhs.
    Various overrides are also provided for integral and floating point values.
*/

/*! \fn QxtCurrency QxtCurrency::operator/(double rhs) const
    \internal
*/

/*! \fn QxtCurrency& QxtCurrency::operator/=(const QxtCurrency &rhs)

    Divides the current value by \a rhs and returns a reference to this.
*/

/*! \fn QString QxtCurrency::toQString() const
 *  Converts a QxtCurrency value to a string. The decimal point used is always
 *  '.' (ASCII). Although there are always 4 digits of precision to the right
 *  of the decimal, the string representation will omit trailing zeroes and,
 *  possibly, the decimal point itself.
 *  \sa toString()
 */

/*! \fn bool QxtCurrency::isNull() const
 *  Returns true if the value contains the special "null" value
 *  (\c {std::numeric_limits<QxtCurrency>::quiet_NaN()}) and false otherwise.
 *  \sa setNull()
 */

/*! \fn void QxtCurrency::setNull()
 *  Sets the value to the special "null" status. This is indicated by the
 *  \c {std::numeric_limits<QxtCurrency>::quiet_NaN()} value.
 *  \sa isNull()
 */

/*!
    \fn QDebug operator<<(QDebug dbg, const QxtCurrency &v)
    \relates QxtCurrency

    Provides QDebug support for QxtCurrency values. The value \a v is
    emitted to the debugging stream \a dbg and a reference returned to same.
    \code
    QxtCurrency myValue;
    ...
    qDebug() << "myValue is" << myValue;
    \endcode
*/
