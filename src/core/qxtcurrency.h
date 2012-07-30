
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

#ifndef QXTCURRENCY_H
#define QXTCURRENCY_H
#include "qxtglobal.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <limits>
#include <QVariant>
#include <QTextStream>
#include <QDataStream>

#if defined(Q_OS_WIN32) && defined(_CY_DEFINED)
#define QXT_HAVE_OLE_CURRENCY
#endif

//////////////////////////////////////////////////////////////////////////////
// Supporting functions

//////////////////////////////////////////////////////////////////////////////
// QxtCurrency -- fixed-precision datatype

class QXT_CORE_EXPORT QxtCurrency
{
public:
    typedef QPair<QxtCurrency,QxtCurrency> Pair;
    // Construct as zero.
    QxtCurrency()
    {
	value = 0LL;
    }
    // Construct via a double.
    QxtCurrency(double v)
    {
	value = qRound64(v * 10000.0);
    }
    // Construct via an integer.
    QxtCurrency(int v)
    {
	value = static_cast<qlonglong>(v) * 10000LL;
    }
    // Construct via a 8-byte integer.
    explicit QxtCurrency(qlonglong v) : value(v) {}
#if defined(qdoc) || defined(QXT_HAVE_OLE_CURRENCY)
    QxtCurrency(const CY &cy) : value(cy.int64) {}
#endif
    // Construct from a string representation
    QxtCurrency(const QString &);
    QxtCurrency(const char *);

    // Test for NULL
    bool isNull() const;
    // Set to NULL
    void setNull();

    // Conversions
    // Convert to a boolean (test for valid non-zero value)
    operator bool() const;
    // Convert to a double.
    operator double() const;
    // Convert to an integer.
    operator int() const;
#if defined(qdoc) || defined(QXT_HAVE_OLE_CURRENCY)
    // Convert to CURRENCY (Windows only)
    inline operator CY() const
    {
	CY cy;
	cy.int64 = value;
	return cy;
    }
#endif
    // Convert to a string representation
    QByteArray toString() const;
    inline QString toQString() const
    {
	return QString::fromLatin1(toString());
    }
    // Store in a QVariant
    QVariant toVariant() const;
    // Extract from a QVariant
    static QxtCurrency fromVariant(const QVariant &);

    // Arithmetic
    // Unary negation
    friend QxtCurrency operator-(const QxtCurrency &rhs);
    // Binary operators, addition
    friend QxtCurrency operator+(const QxtCurrency &lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator+(const QxtCurrency &lhs,int rhs);
    friend QxtCurrency operator+(const QxtCurrency &lhs,double rhs);
    friend QxtCurrency operator+(int lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator+(double lhs,const QxtCurrency &rhs);
    // Binary operators, subtraction
    friend QxtCurrency operator-(const QxtCurrency &lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator-(const QxtCurrency &lhs,int rhs);
    friend QxtCurrency operator-(const QxtCurrency &lhs,double rhs);
    friend QxtCurrency operator-(int lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator-(double lhs,const QxtCurrency &rhs);
    // Binary operators, multiplication
    friend QxtCurrency operator*(const QxtCurrency &lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator*(const QxtCurrency &lhs,int rhs);
    friend QxtCurrency operator*(const QxtCurrency &lhs,double rhs);
    friend QxtCurrency operator*(int lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator*(double lhs,const QxtCurrency &rhs);
    // Binary operators, division
    friend QxtCurrency operator/(const QxtCurrency &lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator/(const QxtCurrency &lhs,int rhs);
    friend QxtCurrency operator/(const QxtCurrency &lhs,double rhs);
    friend QxtCurrency operator/(int lhs,const QxtCurrency &rhs);
    friend QxtCurrency operator/(double lhs,const QxtCurrency &rhs);

    // Assignment operators, addition
    QxtCurrency& operator+=(const QxtCurrency &rhs);
    QxtCurrency& operator+=(int rhs);
    QxtCurrency& operator+=(double rhs);
    // Assignment operators, subtraction
    QxtCurrency& operator-=(const QxtCurrency &rhs);
    QxtCurrency& operator-=(int rhs);
    QxtCurrency& operator-=(double rhs);
    // Assignment operators, multiplication
    QxtCurrency& operator*=(const QxtCurrency &rhs);
    QxtCurrency& operator*=(int rhs);
    QxtCurrency& operator*=(double rhs);
    // Assignment operators, division
    QxtCurrency& operator/=(const QxtCurrency &rhs);
    QxtCurrency& operator/=(int rhs);
    QxtCurrency& operator/=(double rhs);

    // Comparison
    friend bool operator<(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator>(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator<=(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator>=(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator==(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator!=(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator<(const QxtCurrency &lhs, int rhs);
    friend bool operator>(const QxtCurrency &lhs, int rhs);
    friend bool operator<=(const QxtCurrency &lhs, int rhs);
    friend bool operator>=(const QxtCurrency &lhs, int rhs);
    friend bool operator==(const QxtCurrency &lhs, int rhs);
    friend bool operator!=(const QxtCurrency &lhs, int rhs);
    friend bool operator<(const QxtCurrency &lhs, double rhs);
    friend bool operator>(const QxtCurrency &lhs, double rhs);
    friend bool operator<=(const QxtCurrency &lhs, double rhs);
    friend bool operator>=(const QxtCurrency &lhs, double rhs);
    friend bool operator==(const QxtCurrency &lhs, double rhs);
    friend bool operator!=(const QxtCurrency &lhs, double rhs);
    friend bool operator<(int lhs, const QxtCurrency &rhs);
    friend bool operator>(int lhs, const QxtCurrency &rhs);
    friend bool operator<=(int lhs, const QxtCurrency &rhs);
    friend bool operator>=(int lhs, const QxtCurrency &rhs);
    friend bool operator==(int lhs, const QxtCurrency &rhs);
    friend bool operator!=(int lhs, const QxtCurrency &rhs);
    friend bool operator<(double lhs, const QxtCurrency &rhs);
    friend bool operator>(double lhs, const QxtCurrency &rhs);
    friend bool operator<=(double lhs, const QxtCurrency &rhs);
    friend bool operator>=(double lhs, const QxtCurrency &rhs);
    friend bool operator==(double lhs, const QxtCurrency &rhs);
    friend bool operator!=(double lhs, const QxtCurrency &rhs);

    // Miscellany

    // Get null value
    static QxtCurrency null();
    // Get absolute value
    QxtCurrency abs() const;
    // Clamp value to a range
    QxtCurrency & clamp(const QxtCurrency &l, const QxtCurrency &h);
    QxtCurrency & clamp(const Pair &r);
    // Clamp value to a range
    QxtCurrency clamped(const QxtCurrency &l, const QxtCurrency &h) const;
    QxtCurrency clamped(const Pair &r) const;
    // Normalize (strip null)
    QxtCurrency & normalize();
    QxtCurrency normalized() const;
    // Round a value
    QxtCurrency round(int n=2) const;
    // Get the sign of the value
    inline int sign() const
    {
	return (*this < 0) ? -1 : ((*this > 0) ? 1 : 0);
    }
    // Insert into output stream
    friend QTextStream & operator<<(QTextStream &strm, const QxtCurrency &v)
    {
	if(!v.isNull())
	    strm << v.toString();
	return strm;
    }
    // Insert into output stream
    template<typename _CharT, typename _Traits>
    friend std::basic_ostream<_CharT, _Traits> & operator<<(
	    std::basic_ostream<_CharT, _Traits> &strm, const QxtCurrency &v)
    {
	if(!v.isNull())
	    strm << v.toString().constData();
	return strm;
    }

    friend QDataStream &operator<<(QDataStream &out, const QxtCurrency &v);
    friend QDataStream &operator>>(QDataStream &in, QxtCurrency &v);
#ifdef QDEBUG_H
    friend QDebug operator<<(QDebug dbg, const QxtCurrency &v);
#endif

    // Determine amortized payment amount
    static QxtCurrency amortizedPayment(const QxtCurrency& P, double r, int n);
    inline QxtCurrency amortizedPayment(double r, int n) const
    {
	return amortizedPayment(*this, r, n);
    }
    // Determine total amortized interest amount and final payment
    static Pair amortizedInterest(QxtCurrency P, double r, int n,
	    const QxtCurrency &p);
    inline Pair amortizedInterest(double r, int n, const QxtCurrency &p) const
    {
	return amortizedInterest(*this, r, n, p);
    }
    // Generate amortization schedule
    static QList<Pair> amortize(QxtCurrency P, double r, int n,
	    QxtCurrency p=-1);
    inline QList<Pair> amortize(double r, int n, QxtCurrency p=-1) const
    {
	return amortize(*this, r, n, p);
    }

    // Actual value
    qlonglong value;

private:
    //! Parse from text
    void parseASCII(const char *);
};
Q_DECLARE_TYPEINFO(QxtCurrency, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QxtCurrency);

namespace std {
    inline QxtCurrency abs(const QxtCurrency &v)
    {
	return v.abs();
    }
    /*! Numeric limits information for QxtCurrency class. Essentially the
     *	same as std::numeric_limits<long long>
     */
    template<>
    struct numeric_limits<QxtCurrency>
    {
      static const bool is_specialized = true;

      static QxtCurrency min() throw()
      {
	  return QxtCurrency(numeric_limits<qlonglong>::min());
      }
      static QxtCurrency max() throw()
      {
	  return QxtCurrency(numeric_limits<qlonglong>::max());
      }

      static const int digits = numeric_limits<qlonglong>::digits;
      static const int digits10 = numeric_limits<qlonglong>::digits10;
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static QxtCurrency epsilon() throw()
      { return 0; }
      static QxtCurrency round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = true;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static QxtCurrency infinity() throw()
      { return static_cast<QxtCurrency>(0); }
      static QxtCurrency quiet_NaN() throw()
      { return static_cast<QxtCurrency>(qlonglong(0x8000000000000000LL)); }
      static QxtCurrency signaling_NaN() throw()
      { return static_cast<QxtCurrency>(0); }
      static QxtCurrency denorm_min() throw()
      { return static_cast<QxtCurrency>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = numeric_limits<qlonglong>::traps;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
}

inline bool QxtCurrency::isNull() const
{
    return value == std::numeric_limits<QxtCurrency>::quiet_NaN().value;
}

inline void QxtCurrency::setNull()
{
    value = std::numeric_limits<QxtCurrency>::quiet_NaN().value;
}

inline QxtCurrency QxtCurrency::null()
{
    return std::numeric_limits<QxtCurrency>::quiet_NaN();
}

inline QxtCurrency QxtCurrency::abs() const
{
    if(*this)
	return (*this < 0) ? -*this : *this;
    else
	return QxtCurrency();
}

inline QxtCurrency::operator bool() const
{
    return (value & 0x7fffffffffffffffLL) != 0;
}

inline QxtCurrency::operator double() const
{
    if(!isNull())
	return double(value) / 10000.0;
    else
	return 0.0;
}

inline QxtCurrency::operator int() const
{
    if(!isNull())
	return int(value / 10000LL);
    else
	return 0;
}

#if defined(qdoc) || defined(QXT_HAVE_OLE_CURRENCY)
inline QxtCurrency::operator CY() const
{
    CY result;
    result.int64 = normalized().value;
    return result;
}
#endif

inline QxtCurrency & QxtCurrency::clamp(const Pair &r)
{
    return clamp(r.first, r.second);
}

inline QxtCurrency QxtCurrency::clamped(const Pair &r) const
{
    return clamped(r.first, r.second);
}

inline QVariant QxtCurrency::toVariant() const
{
    if(isNull())
	return QVariant(qMetaTypeId<QxtCurrency>(), (const void *)0);
    return QVariant::fromValue(*this);
}

// Unary minus
inline QxtCurrency operator-(const QxtCurrency &rhs)
{
    QxtCurrency result(rhs);
    if(!result.isNull())
	result.value = -rhs.value;
    return result;
}

inline QxtCurrency & QxtCurrency::normalize()
{
    if(isNull())
	value = 0;
    return *this;
}

inline QxtCurrency QxtCurrency::normalized() const
{
    return isNull() ? QxtCurrency() : *this;
}

// Addition
inline QxtCurrency operator+(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(lhs.isNull() || rhs.isNull())
	result.setNull();
    else
	result.value = lhs.value + rhs.value;
    return result;
}
inline QxtCurrency operator+(const QxtCurrency &lhs, int rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = lhs.value + rhs * 10000;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator+(const QxtCurrency &lhs, double rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = lhs.value + qRound64(rhs * 10000);
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator+(int lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = lhs * 10000 + rhs.value;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator+(double lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = qRound64(lhs * 10000) + rhs.value;
    else
	result.setNull();
    return result;
}

// Inplace addition (assignment)
inline QxtCurrency& QxtCurrency::operator+=(const QxtCurrency &rhs)
{
    if(!isNull() && !rhs.isNull())
	value += rhs.value;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator+=(int rhs)
{
    if(!isNull())
	value += rhs;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator+=(double rhs)
{
    if(!isNull())
	value += qRound64(rhs * 10000);
    return *this;
}

// Subtraction
inline QxtCurrency operator-(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(lhs.isNull() || rhs.isNull())
	result.setNull();
    else
	result.value = lhs.value - rhs.value;
    return result;
}
inline QxtCurrency operator-(const QxtCurrency &lhs, int rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = lhs.value - rhs * 10000;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator-(const QxtCurrency &lhs, double rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = lhs.value - qRound64(rhs * 10000);
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator-(int lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = (lhs * 10000) - rhs.value;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator-(double lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = qRound64(lhs * 10000) - rhs.value;
    else
	result.setNull();
    return result;
}

// Inplace subtraction (assignment)
inline QxtCurrency& QxtCurrency::operator-=(const QxtCurrency &rhs)
{
    if(!isNull() && !rhs.isNull())
	value -= rhs.value;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator-=(int rhs)
{
    if(!isNull())
	value -= rhs;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator-=(double rhs)
{
    if(!isNull())
	value -= qRound64(rhs * 10000);
    return *this;
}

// Multiplication
inline QxtCurrency operator*(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(lhs.isNull() || rhs.isNull())
	result.setNull();
    else
	result.value = lhs.value * rhs.value / 10000LL;
    return result;
}
inline QxtCurrency operator*(const QxtCurrency &lhs, int rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = lhs.value * rhs;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator*(const QxtCurrency &lhs, double rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = qRound64(lhs.value * rhs);
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator*(int lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = lhs * rhs.value;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator*(double lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = qRound64(lhs * double(rhs.value));
    else
	result.setNull();
    return result;
}

// Inplace multiplication (assignment)
inline QxtCurrency& QxtCurrency::operator*=(const QxtCurrency &rhs)
{
    if(!isNull() && !rhs.isNull())
	value = value * rhs.value / 10000LL;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator*=(int rhs)
{
    if(!isNull())
	value *= rhs;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator*=(double rhs)
{
    if(!isNull())
	value = qRound64(value * rhs);
    return *this;
}

// Division
inline QxtCurrency operator/(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(lhs.isNull() || rhs.isNull())
	result.setNull();
    else
	result.value = (lhs.value * 10000LL) / rhs.value;
    return result;
}
inline QxtCurrency operator/(const QxtCurrency &lhs, int rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = lhs.value / rhs;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator/(const QxtCurrency &lhs, double rhs)
{
    QxtCurrency result;
    if(!lhs.isNull())
	result.value = qRound64(double(lhs.value) / rhs);
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator/(int lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = lhs / rhs.value;
    else
	result.setNull();
    return result;
}
inline QxtCurrency operator/(double lhs, const QxtCurrency &rhs)
{
    QxtCurrency result;
    if(!rhs.isNull())
	result.value = qRound64(lhs / double(rhs.value));
    else
	result.setNull();
    return result;
}

// Inplace division (assignment)
inline QxtCurrency& QxtCurrency::operator/=(const QxtCurrency &rhs)
{
    if(!isNull() && !rhs.isNull())
	value = (value * 10000LL) / rhs.value;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator/=(int rhs)
{
    if(!isNull())
	value /= rhs;
    return *this;
}
inline QxtCurrency& QxtCurrency::operator/=(double rhs)
{
    if(!isNull())
	value = qRound64(value / rhs);
    return *this;
}

// Comparison
inline bool operator<(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.normalized().value < rhs.normalized().value;
}

inline bool operator>(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.normalized().value > rhs.normalized().value;
}

inline bool operator<=(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.normalized().value <= rhs.normalized().value;
}

inline bool operator>=(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.normalized().value >= rhs.normalized().value;
}

inline bool operator==(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.normalized().value == rhs.normalized().value;
}

inline bool operator!=(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.normalized().value != rhs.normalized().value;
}

inline bool operator<(const QxtCurrency &lhs, int rhs)
{
    return lhs.normalized().value < (rhs * 10000LL);
}

inline bool operator>(const QxtCurrency &lhs, int rhs)
{
    return lhs.normalized().value > (rhs * 10000LL);
}

inline bool operator<=(const QxtCurrency &lhs, int rhs)
{
    return lhs.normalized().value <= (rhs * 10000LL);
}

inline bool operator>=(const QxtCurrency &lhs, int rhs)
{
    return lhs.normalized().value >= (rhs * 10000LL);
}

inline bool operator==(const QxtCurrency &lhs, int rhs)
{
    return lhs.normalized() == QxtCurrency(rhs);
}

inline bool operator!=(const QxtCurrency &lhs, int rhs)
{
    return lhs.normalized() != QxtCurrency(rhs);
}

inline bool operator<(const QxtCurrency &lhs, double rhs)
{
    return lhs.normalized().value < rhs * 10000.0;
}

inline bool operator>(const QxtCurrency &lhs, double rhs)
{
    return lhs.normalized().value > rhs * 10000.0;
}

inline bool operator<=(const QxtCurrency &lhs, double rhs)
{
    return lhs.normalized().value <= rhs * 10000.0;
}

inline bool operator>=(const QxtCurrency &lhs, double rhs)
{
    return lhs.normalized().value >= rhs * 10000.0;
}

inline bool operator==(const QxtCurrency &lhs, double rhs)
{
    return lhs.normalized() == QxtCurrency(rhs);
}

inline bool operator!=(const QxtCurrency &lhs, double rhs)
{
    return lhs.normalized() != QxtCurrency(rhs);
}

inline bool operator<(int lhs, const QxtCurrency &rhs)
{
    return (lhs * 10000LL) < rhs.normalized().value;
}

inline bool operator>(int lhs, const QxtCurrency &rhs)
{
    return (lhs * 10000LL) > rhs.normalized().value;
}

inline bool operator<=(int lhs, const QxtCurrency &rhs)
{
    return (lhs * 10000LL) <= rhs.normalized().value;
}

inline bool operator>=(int lhs, const QxtCurrency &rhs)
{
    return (lhs * 10000LL) >= rhs.normalized().value;
}

inline bool operator==(int lhs, const QxtCurrency &rhs)
{
    return QxtCurrency(lhs) == rhs.normalized();
}

inline bool operator!=(int lhs, const QxtCurrency &rhs)
{
    return QxtCurrency(lhs) != rhs.normalized();
}

inline bool operator<(double lhs, const QxtCurrency &rhs)
{
    return lhs * 10000.0 < rhs.normalized().value;
}

inline bool operator>(double lhs, const QxtCurrency &rhs)
{
    return lhs * 10000.0 > rhs.normalized().value;
}

inline bool operator<=(double lhs, const QxtCurrency &rhs)
{
    return lhs * 10000.0 <= rhs.normalized().value;
}

inline bool operator>=(double lhs, const QxtCurrency &rhs)
{
    return lhs * 10000.0 >= rhs.normalized().value;
}

inline bool operator==(double lhs, const QxtCurrency &rhs)
{
    return QxtCurrency(lhs) == rhs.normalized();
}

inline bool operator!=(double lhs, const QxtCurrency &rhs)
{
    return QxtCurrency(lhs) != rhs.normalized();
}

#ifdef QDEBUG_H
inline QDebug operator<<(QDebug dbg, const QxtCurrency &v)
{
    if(v.isNull())
	dbg.nospace() << "QxtCurrency(null)";
    else
	dbg.nospace() << "QxtCurrency(" << v.toString() << ')';
    return dbg.space();
}
#endif

#endif // QXTCURRENCY_H
