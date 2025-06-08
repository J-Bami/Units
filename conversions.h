#ifndef UNITS_CONVERSION_H
#define UNITS_CONVERSION_H

#include "util.h"
#include <tgmath.h>

namespace units
{
    /// <summary>
    /// struct Conversion. wrapper for a struct whihc defines the templated static member functions:
    /// unitToStandard(T) and standardToUnit(T). to be used within unit to convert between units.
    /// </summary>
    /// <typeparam name="Impl"></typeparam>
    template<typename Impl>
    struct Conversion
    {
        template<typename NumericType>
        static NumericType unitToStandard(const NumericType& unitValue)
        {
            return Impl::unitToStandard(unitValue);
        }

        template<typename NumericType>
        static NumericType standardToUnit(const NumericType& standardValue)
        {
            return Impl::standardToUnit(standardValue);
        }
    };
    
    struct NoConversion
    {
        template<typename NumericType>
        static constexpr NumericType unitToStandard(const NumericType& unitValue)noexcept
        {
            return unitValue;
        }

        template<typename NumericType>
        static constexpr NumericType standardToUnit(const NumericType& standardValue)noexcept
        {
            return standardValue;
        }
    };

#define CREATE_RATIO_CONVERSION(name, ratio)\
struct name\
{\
template<typename NumericType>\
static constexpr NumericType unitToStandard(const NumericType& unitValue)noexcept{ return (ratio) * unitValue;}\
\
template<typename NumericType>\
static constexpr NumericType standardToUnit(const NumericType& unitValue)noexcept { return unitValue/(ratio); }\
};

#define CREATE_LINEAR_CONVERSION(name, intercept, gradient)\
struct name\
{\
    template<typename NumericType>\
    static constexpr NumericType unitToStandard(const NumericType& unitValue)noexcept { return (gradient)*unitValue + (intercept); }\
    \
    template<typename NumericType>\
    static constexpr NumericType standardToUnit(const NumericType& unitValue)noexcept { return (unitValue  - (intercept))/(gradient); }\
};

#define CREATE_LOGARITHMIC_CONVERSION(name, base, multiplier)\
struct name\
{\
    template<typename NumericType>\
    static NumericType unitToStandard(const NumericType& unitValue)noexcept { return (multiplier) * log(unitValue/log((base))); }\
    \
    template<typename NumericType>\
    static NumericType standardToUnit(const NumericType& unitValue)noexcept { return pow((base), unitValue /(multiplier)); }\
};

    namespace conversions
    {
        //define some common conversions
        CREATE_RATIO_CONVERSION(giga, 1e9);
        CREATE_RATIO_CONVERSION(mega, 1e6);
        CREATE_RATIO_CONVERSION(kilo, 1000.0);
        CREATE_RATIO_CONVERSION(hecta, 100.0);
        CREATE_RATIO_CONVERSION(deca, 10.0);
        CREATE_RATIO_CONVERSION(deci, 0.1);
        CREATE_RATIO_CONVERSION(centi, 0.01);
        CREATE_RATIO_CONVERSION(milli, 0.001);
        CREATE_RATIO_CONVERSION(micro, 1e-6);
        CREATE_RATIO_CONVERSION(nano, 1e-9);

        CREATE_LOGARITHMIC_CONVERSION(decibel, 10.0, 10.0);
    }
}

#endif 