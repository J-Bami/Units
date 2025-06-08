#ifndef UNITS_UNIT_H
#define UNITS_UNIT_H
#include "conversions.h"

namespace units
{
    /// <summary>
    /// Class Unit. essentially a wrapper for a numerical type (float type or vector/matrix/tensor type, which implement
    /// the +, -, *, and / operators) that _also_ allows for constexpr type expression, and conversions between user-defined units
    /// </summary>
    /// <typeparam name="NumericType">type which implements operators +, -, *, /. typically float or double</typeparam>
    /// <typeparam name="QuantityType">units::Quantity type. defines the dimensions of the unit</typeparam>
    /// <typeparam name="ConversionImpl">units::Conversion type. defines how to convert between an unscaled unit and this unit</typeparam>
    template<typename NumericType, typename QuantityType, typename ConversionImpl = NoConversion>
    class Unit : protected Conversion<ConversionImpl>
    {
        template<typename T, typename Q, typename C>
        friend class Unit;
    public:
        using ValueType = NumericType;
        using Quantity = QuantityType;

        constexpr Unit()noexcept(noexcept(ValueType())) :m_value{} {}
        explicit constexpr Unit(const ValueType& t)noexcept(noexcept(ValueType{ t })) :m_value{ t } {}
        explicit constexpr Unit(ValueType&& t)noexcept : m_value{ t } {}
        constexpr Unit(const Unit&) = default;
        constexpr Unit(Unit&&) = default;

        template<typename U, typename OtherConversionImpl>
        constexpr Unit(const Unit<U, Quantity, OtherConversionImpl>& unit);

        ~Unit() = default;

        constexpr explicit operator ValueType()noexcept { return m_value; }
        constexpr explicit operator const ValueType& ()noexcept { return m_value; }
        constexpr explicit operator bool()noexcept { return m_value; }

        constexpr Unit& operator=(const ValueType& new_val)noexcept(noexcept(m_value = new_val)) { m_value = new_val; return *this; }
        constexpr Unit& operator=(ValueType&& new_val)noexcept { m_value = new_val; return *this; }
        constexpr Unit& operator=(Unit&&)noexcept = default;
        constexpr Unit& operator=(const Unit&) = default;

        template<typename U, typename OtherConversion>
        constexpr Unit& operator=(const Unit<U, Quantity, OtherConversion>& unit);

        constexpr Unit& operator+()noexcept { return *this; }
        constexpr Unit& operator-()noexcept(noexcept(-m_value)) { m_value = -m_value;  return *this; }

        template<typename NumericType2, typename OtherConversion>
        constexpr Unit& operator+=(const Unit<NumericType2, Quantity, OtherConversion>& other);

        template<typename NumericType2, typename OtherConversion>
        constexpr Unit& operator-=(const Unit<NumericType2, Quantity, OtherConversion>& other);

        template<typename NumericType2 = ValueType>
        constexpr Unit& operator*=(NumericType2&& s);

        template<typename NumericType2 = ValueType>
        constexpr Unit& operator/=(NumericType2&& s);

        //converts the unscaled/standard value to the corresponding unit value
        template<typename NumericType2 = ValueType>
        Unit& fromUnscaled(NumericType2&& value);

        //converts this to an unscaled/standard unit
        template<typename NumericType2 = ValueType>
        constexpr Unit<NumericType2, Quantity, NoConversion> toUnscaled()const;

        template<typename NewConversionImpl, typename NumericType2 = ValueType>
        constexpr Unit<NumericType2, Quantity, NewConversionImpl> toUnit()const;

        constexpr ValueType& value()noexcept { return m_value; }
        constexpr const ValueType& value()const noexcept { return m_value; }

        constexpr void setValue(const ValueType& new_value)noexcept(noexcept(m_value = new_value)) { m_value = new_value; }
        constexpr void setValue(ValueType&& new_value)noexcept { m_value = new_value; }
    private:
        //ValueType standardToUnit() { return Conversion::standardToUnit(m_value); }
        //ValueType unitToStandard() { return Conversion::unitToStandard(m_value); }

        ValueType m_value;
    };

    template<typename T>
    constexpr bool b_is_unit = false;

    template<typename N, typename Q, typename C>
    constexpr bool b_is_unit<Unit<N, Q, C>> = true;

    template<typename N, typename Q, typename C>
    template<typename T, typename O>
    constexpr Unit<N, Q, C>::Unit(const Unit<T, Q, O>& other):
        m_value{standardToUnit(other.unitToStandard(other.m_value))}
    {
        
    }

    template<typename N, typename Q, typename C>
    template<typename T, typename O>
    constexpr Unit<N, Q, C>& Unit<N, Q, C>::operator=(const Unit<T, Q, O>& other)
    {
        m_value = standardToUnit(other.unitToStandard(other.m_value));
        return *this;
    }

    template<typename N, typename Q, typename C>
    template<typename T>
    Unit<N, Q, C>& Unit<N, Q, C>::fromUnscaled(T&& t)
    {
        m_value = this->standardToUnit(t);
        return *this;
    }

    template<typename N, typename Q, typename C>
    template<typename T>
    constexpr Unit<T, Q, NoConversion> Unit<N, Q, C>::toUnscaled()const
    {
        return Unit<T, Q, NoConversion>(this->unitToStandard(m_value));
    }

    template<typename N, typename Q, typename C>
    template<typename NumericType2>
    constexpr Unit<N, Q, C>& units::Unit<N, Q, C>::operator*=(NumericType2&& s)
    {
        m_value *= s;
        return *this;
    }

    template<typename N, typename Q, typename C>
    template<typename NumericType2>
    inline constexpr Unit<N, Q, C>& units::Unit<N, Q, C>::operator/=(NumericType2&& s)
    {
        m_value /= s;
        return *this;
    }

    template<typename N, typename Q, typename C>
    template<typename O, typename T>
    constexpr Unit<T, Q, O> Unit<N, Q, C>::toUnit()const
    {
        return Unit<T, Q, O>{this->unitToStandard(m_value)};
    }

    template<typename N, typename Q, typename C>
    template<typename U, typename O>
    constexpr Unit<N, Q, C>& units::Unit<N, Q, C>::operator+=(const Unit<U, Q, O>& other)
    {
        m_value += this->standardToUnit(other.unitToStandard(other.m_value));
        return *this;
    }
    
    template<typename N, typename Q, typename C>
    template<typename U, typename O>
    constexpr Unit<N, Q, C>& units::Unit<N, Q, C>::operator-=(const Unit<U, Q, O>& other)
    {
        m_value -= this->standardToUnit(other.unitToStandard(other.m_value));
        return *this;
    }

    template<typename NumericType1, typename QuantityType, typename Conversion1, typename NumericType2, typename Conversion2,
    typename N = AddType<NumericType1, NumericType2>,
    typename C = BoolTypePredicate<b_is_same<Conversion1, Conversion2>, NoConversion, Conversion1>>
        Unit<N, QuantityType, C> operator+(const Unit<NumericType1, QuantityType, Conversion1>& c1, const Unit<NumericType2, QuantityType, Conversion2>& c2)
    {
        Unit<N, QuantityType, C> out;
        return out.fromUnscaled(c1.toUnscaled().value() + c2.toUnscaled().value());
    }

    template<typename NumericType1, typename QuantityType, typename Conversion1, typename NumericType2, typename Conversion2,
        typename N = SubtractType<NumericType1, NumericType2>,
        typename C = BoolTypePredicate<b_is_same<Conversion1, Conversion2>, NoConversion, Conversion1>>
        Unit<N, QuantityType, C> operator-(const Unit<NumericType1, QuantityType, Conversion1>& c1, const Unit<NumericType2, QuantityType, Conversion2>& c2)
    {
        Unit<N, QuantityType, C> out;
        return out.fromUnscaled(c1.toUnscaled().value() - c2.toUnscaled().value());
    }

    template<typename NumericType1, typename QuantityType1, typename Conversion1, typename NumericType2, typename QuantityType2, typename Conversion2,
        typename N = MultiplyType<NumericType1, NumericType2>,
        typename Q = MultiplyType<QuantityType1, QuantityType2>,
        typename C = BoolTypePredicate<b_is_same<Conversion1, Conversion2>, NoConversion, Conversion1>>
        Unit<N, Q, C> operator*(const Unit<NumericType1, QuantityType1, Conversion1>& c1, const Unit<NumericType2, QuantityType2, Conversion2>& c2)
    {
        Unit<N, Q, C> out;
        return out.fromUnscaled(c1.toUnscaled().value() * c2.toUnscaled().value());
    }

    template<typename NumericType1, typename QuantityType, typename Conversion, typename NumericType2,
        typename N = typename TypePredicate<!b_is_unit<NumericType2>, MultiplyType<NumericType1, NumericType2>>::type>
        Unit<N, QuantityType, Conversion> operator*(const Unit<NumericType1, QuantityType, Conversion>& u, const NumericType2& f)
    {
        return Unit<N, QuantityType, Conversion>(u.value() * f);
    }

    template<typename NumericType1, typename NumericType2, typename QuantityType, typename Conversion,
        typename N = typename TypePredicate<!b_is_unit<NumericType1>, MultiplyType<NumericType1, NumericType2>>::type>
        Unit<N, QuantityType, Conversion> operator*(const NumericType1& f, const Unit<NumericType2, QuantityType, Conversion>& u)
    {
        return Unit<N, QuantityType, Conversion>(f * u.value());
    }

    template<typename NumericType1, typename QuantityType1, typename Conversion1, typename NumericType2, typename QuantityType2, typename Conversion2,
        typename N = DivideType<NumericType1, NumericType2>,
        typename Q = DivideType<QuantityType1, QuantityType2>,
        typename C = BoolTypePredicate<b_is_same<Conversion1, Conversion2>, NoConversion, Conversion1>>
        Unit<N, Q, C> operator/(const Unit<NumericType1, QuantityType1, Conversion1>& c1, const Unit<NumericType2, QuantityType2, Conversion2>& c2)
    {
        Unit<N, Q, C> out;
        return out.fromUnscaled(c1.toUnscaled().value() / c2.toUnscaled().value());
    }

    template<typename NumericType1, typename QuantityType, typename Conversion, typename NumericType2,
        typename N = typename TypePredicate<!b_is_unit<NumericType2>, DivideType<NumericType1, NumericType2>>::type>
        Unit<N, QuantityType, Conversion> operator/(const Unit<NumericType1, QuantityType, Conversion>& u, const NumericType2& f)
    {
        return Unit<N, QuantityType, Conversion>(u.value() / f);
    }

    template<typename NumericType1, typename NumericType2, typename QuantityType, typename Conversion,
        typename N = typename TypePredicate<!b_is_unit<NumericType1>, DivideType<NumericType1, NumericType2>>::type>
        Unit<N, QuantityType, Conversion> operator/(const NumericType1& f, const Unit<NumericType2, QuantityType, Conversion>& u)
    {
        return Unit<N, QuantityType, Conversion>(f / u.value());
    }
}

#endif